#include "FileParser.h"
#include <clang-c/Index.h>
#include "AST.h"
#include <chrono>
#include <iostream>
#include "ASTWriter.h"
#include <mutex>
#include "IncludesManager.h"
#include "FileTimeChecker.h"
#include <set>
#include <unordered_set>

using namespace std::filesystem;

const char codeGenHookStartDef[] = "#define PARSERHOOKSTART(...) //used to find various different sections of the codegen file\n";
const char codeGenHookEndDef[] = "#define PARSERHOOKEND(...) //used to find various different section of the codegen file\n";
const char universalVodGenIncludes[] = "#include \"MetaDB.h\"\n#include \"MetaReference.h\"\n";

FileParser::FileParser() : cmdargs{ "-std=c++1z", "-DCLANG", "-DCLANG_ATTR(...)=__attribute__((annotate(#__VA_ARGS__)))" }
{
	
}


FileParser::~FileParser()
{
	for (char* arg : cmdargs)
	{
		//free(arg);
	}

	for (auto &it : _finalwriters)
	{
		delete it.second;
	}
}

void FileParser::Parse()
{
	std::set<path> pathsToParse;

	//get a list of all 
	for (auto project : _appProjects)
	{
		for (const auto& file : project.second->GetHeaderFiles())
		{
			if(FileTimeChecker::IsDirty(file))
			{
				pathsToParse.insert(file);
			}
		}
	}

	for (auto project : _libProjects)
	{
		for (const auto& file : project.second->GetHeaderFiles())
		{
			if (FileTimeChecker::IsDirty(file))
			{
				pathsToParse.insert(file);
			}
		}
	}

	auto includes = IncludesManager::GetIncludePaths();

	for (const path& include : includes)
	{
		std::string temp = "--include-directory=";
		temp += include.string();

		char* temp2 = (char*)malloc(temp.size() + 1);

		strcpy(temp2, temp.c_str());

		cmdargs.push_back(temp2);
	}

	{
		std::vector<std::thread> threadPool;
		std::vector<const path *> curpaths;

		for (auto& path : pathsToParse)
		{
			curpaths.push_back(&path);
			if (pathsToParse.size() < std::thread::hardware_concurrency() || curpaths.size() % pathsToParse.size() / std::thread::hardware_concurrency())
			{
				threadPool.emplace_back(&FileParser::parseTreaded, this, curpaths);
				curpaths.clear();
			}
		}

		for (auto& thread : threadPool)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}
	
	for (const auto& project : _appProjects)
	{
		std::unordered_map<std::string, ASTWriter *> curWritters;

		//only write out the files that this particular project cares about
		for (const auto &dep : project.second->GetDependencies())
		{
			if(_libProjects.count(dep))
			{
				for (const path& header_file : _libProjects[dep]->GetHeaderFiles())
				{
					if(_finalwriters.count(header_file.string()) && !curWritters.count(header_file.string()))
					{
						curWritters.insert({ header_file.string(), _finalwriters[header_file.string()] });
					}
				}
			}
		}

		//write out this projects files
		for (const path& header_file : project.second->GetHeaderFiles())
		{
			if (_finalwriters.count(header_file.string()) && !curWritters.count(header_file.string()))
			{
				curWritters.insert({ header_file.string(), _finalwriters[header_file.string()] });
			}
		}

		path codeGenFileLoc = project.second->GetPath().parent_path();
		codeGenFileLoc.append("codegen.cpp");

		if (!exists(codeGenFileLoc))
		{
			FILE* outfile = fopen(codeGenFileLoc.string().c_str(), "wb");
			fclose(outfile);
		}

		FILE* outfile = fopen(codeGenFileLoc.string().c_str(), "rb");

		if (outfile)
		{
			size_t fsize = file_size(codeGenFileLoc);

			std::string outstr;
			outstr.reserve(fsize * 2);

			if (fsize)
			{
				char* outttext = new char[fsize + 1];
				fread(outttext, 1, fsize, outfile);
				outttext[fsize] = 0;

				outstr = outttext;

				if (outstr.find(codeGenHookStartDef) == std::string::npos)
				{
					printf("invalid codegen! cannot parse!");
				}
			}
			else
			{
				outstr.append(codeGenHookStartDef);
				outstr.append(codeGenHookEndDef);
				outstr.append(universalVodGenIncludes);
			}

			fclose(outfile);

			for (auto& writer : curWritters)
			{
				std::string starthook = "PARSERHOOKSTART(\"" + writer.second->GetSourceFile() + "\");\n";
				std::string endhook = "PARSERHOOKEND(\"" + writer.second->GetSourceFile() + "\");\n";
				size_t starthookloc = outstr.find(starthook);
				if (starthookloc == std::string::npos)
				{
					outstr += "PARSERHOOKSTART(\"" + writer.second->GetSourceFile() + "\");\n";
					outstr += writer.second->write();
					outstr += "PARSERHOOKEND(\"" + writer.second->GetSourceFile() + "\");\n";
				}
				else
				{
					size_t endhookloc = outstr.find(endhook);

					outstr.erase(starthookloc, endhookloc);
					outstr += "PARSERHOOKSTART(\"" + writer.second->GetSourceFile() + "\");\n";
					outstr += writer.second->write();
					outstr += "PARSERHOOKEND(\"" + writer.second->GetSourceFile() + "\");\n";
				}
			}

			outfile = fopen(codeGenFileLoc.string().c_str(), "wb");

			fwrite(outstr.c_str(), 1, outstr.size(), outfile);

			fclose(outfile);
		}
	}

	
}

void FileParser::AddProject(VSProjParser* proj)
{
	if(proj->IsApp())
	{
		_appProjects.insert_or_assign(proj->GetPath().string(), proj);
	}
	else
	{
		_libProjects.insert_or_assign(proj->GetPath().stem().string(), proj);
	}
}

void FileParser::parseTreaded(std::vector<const path*> filenames)
{
	CXIndex index = clang_createIndex(0, 0);
	for (auto filepath : filenames)
	{
		Translate(index, *filepath);
	}
	clang_disposeIndex(index);
}

using namespace std::chrono;

void display_time(nanoseconds dur, const wchar_t *filepath)
{
	auto h = duration_cast<hours>(dur);
	dur -= h;
	auto m = duration_cast<minutes>(dur);
	dur -= m;
	auto s = duration_cast<seconds>(dur);
	dur -= s;
	auto ms = duration_cast<milliseconds>(dur);
	dur -= ms;
	auto us = duration_cast<microseconds>(dur);
	dur -= us;
	auto ns = duration_cast<nanoseconds>(dur);
	dur -= ns;

#define TIME_TO_STR(tim) std::string(tim.count() ? std::to_string(tim.count()) + std::string(#tim ":") : "")

	std::mutex displayMutex;
	displayMutex.lock();
	printf("Parsed %ls...\n", filepath);
	std::cout << TIME_TO_STR(h) << TIME_TO_STR(m) << TIME_TO_STR(s) << TIME_TO_STR(ms) << TIME_TO_STR(us) << TIME_TO_STR(ns) << std::endl;
	displayMutex.unlock();

}

bool FileParser::Translate(CXIndex index, const path& filename)
{
	auto startTim = high_resolution_clock::now();

	AST *ast = new AST(index, filename.string().c_str(), cmdargs);

	ast->Parse();

	ASTWriter *writer = new ASTWriter(*ast);

	//std::cout << writter.write("#include \"" + filename.string() + "\"\n");
	writer->write(filename.string());
	display_time((high_resolution_clock::now() - startTim), filename.c_str());
	_writtersLock.lock();
	_finalwriters.insert({filename.string(), writer });
	_writtersLock.unlock();
	//ast.dump();

	return true;
}