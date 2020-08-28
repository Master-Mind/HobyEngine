#include "VSProjParser.h"
#include "rapidxml.hpp"
#include <boost/tokenizer.hpp>

using namespace std::filesystem;

VSProjParser::VSProjParser(path &projFile):
	_projFile(projFile)
{
	rapidxml::xml_document<> doc;

	FILE* infile = fopen(_projFile.string().c_str(), "rb");

	if (infile)
	{
		char* filetext = new char[file_size(_projFile) + 1];

		if (fread(filetext, 1, file_size(_projFile), infile))
		{
			filetext[file_size(_projFile)] = 0;

			doc.parse<0>(filetext);

			auto proj = doc.first_node();

			//figure out whether it is an app or a lib
			auto props = proj->first_node();

			while (props)
			{
				auto attr = props->first_attribute("Label");
				if(attr && strcmp(attr->value(), "Configuration") == 0)
				{
					break;
				}
				props = props->next_sibling();
			}

			if (props)
			{
				auto configType = props->first_node("ConfigurationType");

				_isApp = strstr(configType->value(), "Application");
			}

			//if it is an app, track its dependancies
			if (_isApp)
			{
				auto defgroup = proj->first_node("ItemDefinitionGroup");
				auto links = defgroup->first_node("Link");
				auto deps = links->first_node("AdditionalDependencies");
				if(deps)
				{
					boost::char_separator<char> sep(";");
					std::string depstr = deps->value();

					boost::tokenizer tok(depstr, sep);

					for (path dep : tok)
					{
						if (dep.c_str()[0] != '%')
						{
							std::string projname = dep.filename().string();

							projname.erase(projname.find_last_of('.'));

							_dependencies.push_back(projname);
						}
					}
				}
			}

			//retrieve headers and sources
			auto sourceGroup = proj->first_node();

			while (sourceGroup)
			{
				sourceGroup = sourceGroup->next_sibling("ItemGroup");

				if (sourceGroup && !sourceGroup->first_attribute())
				{
					auto include = sourceGroup->first_node();

					while(include)
					{
						auto inattrib = include->first_attribute();

						path p = _projFile.parent_path();

						p.append(inattrib->value());

						if (exists(p))
						{
							//TODO: change this if there are ever different kinds of files
							if (p.extension() == ".cpp")
							{
								_sourceFiles.push_back(p);
							}
							else if (p.extension() == ".h" || p.extension() == ".hpp")
							{
								_headerFiles.push_back(p);
							}
						}
						else
						{
							printf("Error! the path \"%S\" does not exist!\n", p.c_str());
						}

						include = include->next_sibling();
					}
				}
			}

			delete[] filetext;
			fclose(infile);
		}

		
	}
}

const std::vector<path>& VSProjParser::GetHeaderFiles()
{
	return _headerFiles;
}

const std::vector<std::string>& VSProjParser::GetDependencies() const
{
	return _dependencies;
}

const path& VSProjParser::GetPath()
{
	return _projFile;
}

bool VSProjParser::IsApp()
{
	return _isApp;
}

