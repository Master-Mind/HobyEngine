#include "ParserOutputFormats.h"
#include <fstream>
#include <sstream>
using namespace std::filesystem;

std::unordered_map<std::string, ParserOutputFormat*> POFManager::formats;
std::unordered_map<CXCursorKind, ParserOutputFormat*> POFManager::formatsByKind;

bool POFManager::Init()
{
	if (exists("./ParserFormats"))
	{
		for (const auto& entry : directory_iterator("./ParserFormats"))
		{
			if (entry.is_regular_file() && !formats.count(entry.path().stem().string()))
			{
				auto slot = formats.insert({ entry.path().stem().string(), new ParserOutputFormat(entry.path()) });

				if (slot.first->second->GetKind() != CXCursor_InvalidCode && !formatsByKind.count(slot.first->second->GetKind()))
				{
					formatsByKind.insert({slot.first->second->GetKind(), slot.first->second});
				}
			}
		}
		
		return true;
	}

	return false;
}

const ParserOutputFormat* POFManager::GetFormatter(std::string nameOfFormat)
{
	return formats[nameOfFormat];
}

const ParserOutputFormat *POFManager::GetFormatter(CXCursorKind kindToFormat)
{
	return formatsByKind[kindToFormat];
}

ParserOutputFormat::ParserOutputFormat(const path& filename)
{
	std::ifstream t;
	size_t length;
	t.open(filename, std::ios::in | std::ios::binary);
	t.seekg(0, std::ios::end);
	length = t.tellg();
	t.seekg(0, std::ios::beg);
	buffer = new char[length + 1];
	t.read(buffer, length);
	t.close();
	buffer[length] = 0;
	try
	{
		doc.parse<0>(buffer);
	}
	catch (rapidxml::parse_error &e)
	{
		printf("Parser error when parsing %ls at %s: %s", filename.c_str(), e.where<char>(), e.what());

		throw e;
	}
	
	root = doc.first_node("format");
	
	if (root)
	{
		rapidxml::xml_attribute<>* cursorType = root->first_attribute("forCursorType");

		if (cursorType && strcmp(cursorType->value(), "ClassDecl") == 0)
		{
			kind = CXCursor_ClassDecl;
		}
	}
}

ParserOutputFormat::~ParserOutputFormat()
{
	if (buffer)
	{
		delete[] buffer;
	}
}

CXCursorKind ParserOutputFormat::GetKind() const
{
	return kind;
}

void ParserOutputFormat::Format(std::string &output, const AST::Node& curnode, bool debugPrint) const
{
	FormatInternal(output, curnode, root->first_node(), debugPrint);
}

void ParserOutputFormat::FormatInternal(std::string& output, const AST::Node& curnode,
	const rapidxml::xml_node<>* curxmlNode, bool debugPrint) const
{
	const AST::Class* curclass = dynamic_cast<const AST::Class*>(&curnode);
	const AST::Function* curfunc = dynamic_cast<const AST::Function*>(&curnode);
	const AST::Variable* curvar = dynamic_cast<const AST::Variable*>(&curnode);
	
	while (curxmlNode)
	{
		if(strcmp(curxmlNode->name(), "cond") == 0)
		{
			rapidxml::xml_attribute<>* condition = curxmlNode->first_attribute("activeif");

			if (EvaluateFormattingCondition(curnode, condition))
			{
				rapidxml::xml_node<>* child = curxmlNode->first_node();

				FormatInternal(output, curnode, child, debugPrint);				
			}
		}
		else if (strcmp(curxmlNode->name(), "var") == 0)
		{
			rapidxml::xml_attribute<> *formatToUse = curxmlNode->first_attribute("useformat");
			
			if (formatToUse && formatToUse->value_size())
			{
				const ParserOutputFormat *format = POFManager::GetFormatter(formatToUse->value());

				if (format)
				{
					const AST::Node* node = GetVarAsNode(curnode, curxmlNode->value(), 0);

					if (node)
					{
						if (IsVarAnArray(curnode, curxmlNode->value()))
						{
							for (int i = 1; node; ++i)
							{
								format->Format(output, *node, debugPrint);

								node = GetVarAsNode(curnode, curxmlNode->value(), i);
							}
						}
						else
						{
							format->Format(output, *GetVarAsNode(curnode, curxmlNode->value()), debugPrint);
						}
					}
				}
				else
				{
					throw std::exception((std::string("FORMAT ERROR: Could not find a format by the name of ")
						+ formatToUse->value()).c_str());
				}
			}
			else
			{
				FormatVar(output, curnode, curxmlNode, debugPrint);
			}
		}
		else
		{
			if(curxmlNode->type() == rapidxml::node_data)
			{
				if (debugPrint)
				{
					printf("%s", curxmlNode->value());
				}

				output += curxmlNode->value();
			}

			rapidxml::xml_node<>* child = curxmlNode->first_node();

			while (child)
			{
				FormatInternal(output, curnode, child, debugPrint);
				child = child->next_sibling();
			}
		}

		curxmlNode = curxmlNode->next_sibling();
	}
}

bool ParserOutputFormat::EvaluateFormattingCondition(const AST::Node& curnode,
	const rapidxml::xml_attribute<>* condition) const
{
	const AST::Class* curclass = dynamic_cast<const AST::Class*>(&curnode);
	const AST::Function* curfunc = dynamic_cast<const AST::Function*>(&curnode);
	const AST::Variable* curvar = dynamic_cast<const AST::Variable*>(&curnode);

	if (condition)
	{
		if (strstr(condition->value(), "hasvar") != nullptr)
		{
			std::string varname = condition->value();
			std::string::size_type offset = varname.find('(');
			varname = varname.substr(offset + 1, varname.find(')') - offset - 1);
			
			return GetVarAsString(curnode, varname.c_str()).length();
		}
		else
		{
			throw std::exception((std::string("FORMAT ERROR: Unrecognizable condition ") + condition->value() + "\n").c_str());
		}
	}
	else
	{
		throw std::exception("FORMAT ERROR: Cannot use cond without activeif attribute\n");

		return false;
	}
}

std::string ParserOutputFormat::GetVarAsString(const AST::Node& curnode, const char* varName) const
{
	//go through variables common to all node types first
	if (strcmp(varName, "spelling") == 0)
	{
		return curnode.GetSpelling();
	}
	else if (strcmp(varName, "attributes") == 0)
	{
		std::string ret;
		const std::vector<std::string>& attribs = curnode.get_attributes();
		
		//guess at size
		ret.reserve(attribs.size() * 8);

		for (int i = 0; i < attribs. size(); ++i)
		{
			if (i != 0)
			{
				ret += ", ";
			}
			
			ret += attribs[i];
		}
		
		return ret;
	}
	//specific node types
	else
	{
		const AST::Class* curclass = dynamic_cast<const AST::Class*>(&curnode);
		const AST::Function* curfunc = dynamic_cast<const AST::Function*>(&curnode);
		const AST::Variable* curvar = dynamic_cast<const AST::Variable*>(&curnode);


		if (strcmp(varName, "base_class") == 0)
		{
			if (curclass)
			{
				return curclass->base_class_;
			}
			else
			{
				throw std::exception("FORMAT ERROR: Conditional wanted a class but the node it oporates on is not a class\n");
			}
		}
	}
	
	//throw std::exception((std::string("FORMAT ERROR: Unknown var name ") + varName).c_str());
	return "";
}

const AST::Node* ParserOutputFormat::GetVarAsNode(const AST::Node& curnode, const char* varName, int optIndex) const
{
	const AST::Class* curclass = dynamic_cast<const AST::Class*>(&curnode);
	const AST::Function* curfunc = dynamic_cast<const AST::Function*>(&curnode);
	const AST::Variable* curvar = dynamic_cast<const AST::Variable*>(&curnode);

	if (strcmp(varName, "member_vars") == 0)
	{
		if (curclass)
		{
			if (curclass->member_vars_.size() > optIndex && optIndex >= 0)
			{
				return curclass->member_vars_[optIndex];
			}
		}
		else
		{
			throw std::exception((std::string("FORMAT ERROR: Class type expected when using the var ") + varName).c_str());
		}
	}
	
	return nullptr;
}

bool ParserOutputFormat::IsVarAnArray(const AST::Node& curnode, const char* varName) const
{
	if (strcmp(varName, "member_vars") == 0)
	{
		return true;
	}

	return false;
}

void ParserOutputFormat::FormatVar(std::string& output, const AST::Node& curnode,
                                   const rapidxml::xml_node<>* curxmlNode, bool debugPrint) const
{
	std::string temp = GetVarAsString(curnode, curxmlNode->value());
	
	if (debugPrint)
	{
		printf("%s", temp.c_str());
	}

	output += temp;
}
