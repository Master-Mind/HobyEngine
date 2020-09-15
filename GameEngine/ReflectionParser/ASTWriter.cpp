#include "ASTWriter.h"
#include <cstdarg>
#include "ParserOutputFormats.h"

void string_appendf(std::string &out, size_t fmtLen, const char *fmt_str, ...) {
	long long final_n, n = fmtLen * 2; /* Reserve two times as much as the length of the fmt_str */
	va_list ap = nullptr;
	size_t curOutSize = out.size();

	while (1)
	{
		out.resize(curOutSize + n);

		va_start(ap, fmt_str);
		final_n = vsnprintf(&out[curOutSize], n, fmt_str, ap);
		va_end(ap);
		if (final_n < 0 || final_n >= n)
			n += abs(final_n - n + 1);
		else
			break;
	}

	out.resize(curOutSize + final_n);
}

ASTWriter::ASTWriter(const AST& ast) : ast_(ast)
{
}

const std::string& ASTWriter::write(const std::string& sourceFile)
{
	//print nothing if there is no output or we have already written
	if(!ast_.root_.children.empty() && output_.empty())
	{
		output_ += "#include \"" + sourceFile + "\"\n";
		write_internal(ast_.root_);
		_sourceFile = sourceFile;
	}

	return output_;
}

const std::string& ASTWriter::GetSourceFile()
{
	return _sourceFile;
}

ASTWriter::~ASTWriter()
{
}

void printAttribs(std::string &out, const std::vector<std::string> &attribs)
{
	for (size_t i = 0; i < attribs.size(); ++i)
	{
		out += attribs[i];

		if (i != attribs.size() - 1)
		{
			out += ',';
		}
	}
}

void ASTWriter::write_internal(const AST::Node& curnode)
{
	const AST::Class* curClass = nullptr;
	//switch (curnode.kind_)
	//{
	//case CXCursor_ClassDecl:
	//	curClass = dynamic_cast<const AST::Class*>(&curnode);
	//
	//	for (auto& namespace_ : curnode.GetHandler()->GetNameSpaces())
	//	{
	//		output_ += "using namespace " + namespace_ + ";\n";
	//	}
	//
	//	string_appendf(output_, sizeof(classReflStart), classReflStart, curnode.spelling_.c_str(), curnode.spelling_.c_str(), curnode.spelling_.c_str(), curnode.spelling_.c_str(), curnode.spelling_.c_str());
	//
	//	output_ += "\t_properties.AddAttributes(";
	//
	//	printAttribs(output_, curnode.attributes_);
	//
	//	output_ += ");\n";
	//
	//	for (AST::Variable* const var : curClass->member_vars_)
	//	{
	//		if (var->GetAccess() != CX_CXXPublic && var->get_attributes().empty())
	//		{
	//			continue;
	//		}
	//
	//		string_appendf(output_, sizeof(AddMember), AddMember, var->spelling_.c_str(), var->spelling_.c_str());
	//
	//		if(!var->attributes_.empty())
	//		{
	//			output_ += ',';
	//			printAttribs(output_, var->attributes_);
	//		}
	//
	//		output_ += ");//" + var->default_value_ + '\n';
	//	}
	//
	//	if(!curClass->base_class_.empty())
	//	{
	//		output_ += "\t_parentClass = MetaDB::getMeta<" + curClass->base_class_.substr(curClass->base_class_.find(' ', 5) + 1) + ">();\n";
	//	}
	//
	//	string_appendf(output_, sizeof(classReflEnd), classReflEnd, curnode.spelling_.c_str(), curnode.spelling_.c_str(), curnode.spelling_.c_str());
	//	break;
	//default:
	//	break;
	//}

	const ParserOutputFormat* curFormat = POFManager::GetFormatter(curnode.kind_);

	if (curFormat)
	{
		curFormat->Format(output_, curnode, true);
	}
	
	for (AST::Node* const child : curnode.children)
	{
		write_internal(*child);
	}
}
