#include "AST.h"
#include <cstdio>
#include <string>
#include <iostream>
#include <unordered_set>

std::unordered_set<std::string> cppKeywords = { "alignas",
"alignof",
"and",
"and_eq",
"asm",
"atomic_cancel",
"atomic_commit",
"atomic_noexcept",
"auto",
"bitand",
"bitor",
"bool",
"break",
"case",
"catch",
"char",
"char8_t",
"char16_t",
"char32_t",
"class",
"compl",
"concept",
"const",
"consteval",
"constexpr",
"constinit",
"const_cast",
"continue",
"co_await",
"co_return",
"co_yield",
"decltype",
"default",
"delete",
"do",
"double",
"dynamic_cast",
"else",
"enum",
"explicit",
"export",
"extern",
"false",
"float",
"for",
"friend",
"goto",
"if",
"inline",
"int",
"long",
"mutable",
"namespace",
"new",
"noexcept",
"not",
"not_eq",
"nullptr",
"operator",
"or",
"or_eq",
"private",
"protected",
"public",
"reflexpr",
"register",
"reinterpret_cast",
"requires",
"return",
"short",
"signed",
"sizeof",
"static",
"static_assert",
"static_cast",
"struct",
"switch",
"synchronized",
"template",
"this",
"thread_local",
"throw",
"true",
"try",
"typedef",
"typeid",
"typename",
"union",
"unsigned",
"using",
"virtual",
"void",
"volatile",
"wchar_t",
"while",
"xor",
"xor_eq" };

//disposes str
std::string makestr(CXString &str)
{
	std::string ret = clang_getCString(str);

	clang_disposeString(str);

	return ret;
}

CXChildVisitResult ast_full_parse(CXCursor c, CXCursor parent, CXClientData nodeVoid);

static CXChildVisitResult attributeScanner(CXCursor c, CXCursor parent, CXClientData foundAttr)
{
	if (clang_getCursorKind(c) == CXCursor_AnnotateAttr)
	{
		CXString spelling = clang_getCursorSpelling(c);
		if (strstr(clang_getCString(spelling), "MetaAttr::Reflect()"))
		{
			*static_cast<std::string*>(foundAttr) = clang_getCString(spelling);

			return CXChildVisit_Break;
		}
		else
		{
			clang_disposeString(spelling);
		}
	}

	return CXChildVisit_Continue;
}

static CXChildVisitResult ast_var_parse(CXCursor c, CXCursor parent, CXClientData varVoid)
{
	AST::Variable* var = static_cast<AST::Variable*>(varVoid);

	CXSourceRange range = clang_getCursorExtent(c);
	CXToken* tokens = 0;
	unsigned int nTokens = 0;
	clang_tokenize(var->handler_->unit_, range, &tokens, &nTokens);

	bool foundEquals = false;

	for (unsigned int i = 0; i < nTokens; i++)
	{
		std::string curTok = makestr(clang_getTokenSpelling(var->handler_->unit_, tokens[i]));

		if(foundEquals)
		{
			var->default_value_ += curTok;
		}

		foundEquals = foundEquals || curTok[0] == '=';
	}

	clang_disposeTokens(var->handler_->unit_, tokens, nTokens);

	const std::string& attribs = var->GetHandler()->GetAttrsForSymbol("");

	if(!attribs.empty())
	{
		var->set_attributes(attribs);
	}

	return CXChildVisit_Break;
}

static CXChildVisitResult ast_func_parse(CXCursor c, CXCursor parent, CXClientData funcVoid)
{
	//iterate over the parameters
	AST::Function* func = static_cast<AST::Function*>(funcVoid);

	if(clang_getCursorKind(c) != CXCursor_AnnotateAttr)
	{
		AST::Variable* param = new AST::Variable(c, "", func->handler_);

		ast_var_parse(c, parent, param);

		func->params_.push_back(param);
	}
	else
	{
		func->set_attributes(makestr(clang_getCursorSpelling(c)));
	}

	return CXChildVisit_Continue;
}

static CXChildVisitResult ast_class_parse(CXCursor c, CXCursor parent, CXClientData nodeVoid)
{
	AST::Class* node = static_cast<AST::Class*>(nodeVoid);
	CXChildVisitResult ret = CXChildVisit_Continue;
	AST::Node* new_node = nullptr;

	switch (clang_getCursorKind(c))
	{
	case CXCursor_AnnotateAttr:
		node->set_attributes(makestr(clang_getCursorSpelling(c)));
		break;
	case CXCursor_Constructor:
	case CXCursor_Destructor:
	case CXCursor_CXXMethod:
		new_node = new AST::Function(c, "", node->handler_);
		new_node->access_ = clang_getCXXAccessSpecifier(c);
		clang_visitChildren(
			c,
			ast_func_parse,
			new_node);
		node->member_funcs_.push_back(static_cast<AST::Function*>(new_node));
		break;
	case CXCursor_FieldDecl:
		new_node = new AST::Variable(c, "", node->handler_);
		new_node->access_ = clang_getCXXAccessSpecifier(c);
		ast_var_parse(c, parent, new_node);
		node->member_vars_.push_back(static_cast<AST::Variable*>(new_node));
		break;
	case CXCursor_CXXBaseSpecifier:
		node->base_class_ = makestr(clang_getCursorSpelling(c));
		break;
	default:
		ret = ast_full_parse(c, parent, nodeVoid);
		break;
	}

	return ret;
}

static CXChildVisitResult ast_full_parse(CXCursor c, CXCursor parent, CXClientData nodeVoid)
{
	AST::Node *node = static_cast<AST::Node*>(nodeVoid);
	AST::Node* new_node = nullptr;


	switch (clang_getCursorKind(c))
	{
	case CXCursor_ClassDecl:
		new_node = new AST::Class(c, "", node->handler_);
		clang_visitChildren(
			c,
			ast_class_parse,
			new_node);
		break;
	case CXCursor_CXXBaseSpecifier:
		break;
	default:
		new_node = new AST::Node(c, "", node->handler_);
		clang_visitChildren(
			c,
			ast_full_parse,
			new_node);
		break;
	}

	const std::string& attributes = node->GetHandler()->GetAttrsForSymbol(makestr(clang_getCursorSpelling(c)));

	if (!attributes.empty())
	{
		new_node->set_attributes(attributes);
	}

	node->children.push_back(new_node);

	return CXChildVisit_Continue;
}

static CXChildVisitResult ast_visitor(CXCursor c, CXCursor parent, CXClientData astVoid)
{
	AST& ast = *static_cast<AST*>(astVoid);

	if (clang_Location_isFromMainFile(clang_getCursorLocation(c)) == 0) {
		return CXChildVisit_Continue;
	}

	if(clang_getCursorKind(c) == CXCursor_Namespace)
	{
		ast._namespaces.push_back(makestr(clang_getCursorSpelling(c)));
		clang_visitChildren(
			c,
			ast_visitor,
			&ast);
	}
	else
	{
		const std::string &attributes = ast.GetAttrsForSymbol(makestr(clang_getCursorSpelling(c)));

		if (!attributes.empty())
		{
			//do full parse of element
			ast_full_parse(c, parent, &ast.root_);
		}

	}

	return CXChildVisit_Continue;
}

AST::Node::Node(CXCursor c, const std::string &attributes, AST* handler)
{
	handler_ = handler;
	spelling_ = makestr(clang_getCursorSpelling(c));
	kind_ = clang_getCursorKind(c);
	kindSpelling_ = kind_ ? makestr(clang_getCursorKindSpelling(kind_)) : "";

	handler->outputGuestimate_ += attributes.size() + spelling_.size() * 2;

	set_attributes(attributes);
}

AST::Node::~Node()
{
	for (Node* child : children)
	{
		delete child;
	}
}

const std::string &AST::Node::GetSpelling() const
{
	return spelling_;
}

const std::string& AST::Node::GetKindSpelling() const
{
	return kindSpelling_;
}

std::vector<std::string> AST::Node::get_attributes() const
{
	return attributes_;
}

void AST::Node::set_attributes(const std::string &attributes)
{
	attrStr_ = attributes;
	const char* lastPtr = &attrStr_[0];
	size_t i = 0;

	for (; i < attrStr_.size(); ++i)
	{
		if (attrStr_[i] == ',')
		{
			attributes_.push_back(std::string(lastPtr, &attrStr_[i] - lastPtr));

			lastPtr = &attrStr_[i + 1];
		}
	}

	if(!attrStr_.empty())
	{
		attributes_.push_back(std::string(lastPtr, &attrStr_[i] - lastPtr));
	}
}

const AST* AST::Node::GetHandler() const
{
	return handler_;
}

AST::AST(CXIndex index, const char* filename, const std::vector<char *>& cmdargs) : root_({}, "", this)
{
	root_.spelling_ = "the root";

	std::vector<const char *> args;

	unit_ = clang_parseTranslationUnit(
		index,
		filename, &cmdargs[0], static_cast<int>(cmdargs.size()),
		nullptr, 0,
		CXTranslationUnit_KeepGoing | CXTranslationUnit_SkipFunctionBodies);
}


AST::~AST()
{
}

void AST::dump() const
{
	dump_internal(root_, 0);
}

const std::vector<std::string>& AST::GetNameSpaces() const
{
	return _namespaces;
}

const std::string& AST::GetAttrsForSymbol(const std::string& symbol) const
{
	static const std::string eck = "";

	return _symbolsWithAttrs.count(symbol) ? _symbolsWithAttrs.at(symbol) : eck;
}

void AST::dump_internal(const Node& node, int level) const
{
	std::string tabs(level, '\t');

	std::cout << tabs << "Spelling: " << node.spelling_ << ", Kind: " << node.GetKindSpelling();

	if(!node.attrStr_.empty())
	{
		std::cout << ", Attributes: " << node.attrStr_;
	}

	std::cout << std::endl;

	if (dynamic_cast<const Class*>(&node))
	{
		const Class &class_ = *dynamic_cast<const Class*>(&node);

		for (const Function* c : class_.member_funcs_)
		{
			dump_func(tabs, *c, true, false);
		}

		for (const Function* c : class_.static_funcs_)
		{
			dump_func(tabs, *c, true, true);
		}

		for (const Variable* c : class_.member_vars_)
		{
			dump_var(tabs, *c, true, false);
		}

		for (const Variable* c : class_.static_vars_)
		{
			dump_var(tabs, *c, true, true);
		}
	}

	for (Node* const c : node.children)
	{
		dump_internal(*c, level + 1);
	}
}

void AST::dump_func(std::string &tabs, const Function& func, bool is_member, bool is_static) const
{
	std::string discription;

	if(is_member)
	{
		switch (func.access_)
		{
		case CX_CXXInvalidAccessSpecifier:
			discription = "invalid";
			break;
		case CX_CXXPublic:
			discription = "public";
			break;
		case CX_CXXProtected:
			discription = "protected";
			break;
		case CX_CXXPrivate:
			discription = "private";
			break;
		default:;
		}

		if (is_static)
		{
			discription += " static";
		}

		discription += " member function ";
	}

	std::cout << tabs << '\t' << discription << func.GetSpelling() << "(";

	for (size_t i = 0; i < func.params_.size(); ++i)
	{
		const Variable* param = func.params_[i];

		std::string defaultVal = param->default_value_.empty() ? "" : " = " + param->default_value_;
		std::cout << param->spelling_ << defaultVal << (i != func.params_.size() - 1 ? ", " : "");
	}

	std::cout << ")";

	if(!func.attributes_.empty())
	{
		std::cout << " with attributes " << func.attrStr_;
	}

	std::cout << std::endl;
}

void AST::dump_var(std::string& tabs, const Variable& var, bool is_member, bool is_static) const
{
	std::string discription;
	std::string defaultValue;

	if (is_member)
	{
		switch (var.access_)
		{
		case CX_CXXInvalidAccessSpecifier:
			discription = "invalid";
			break;
		case CX_CXXPublic:
			discription = "public";
			break;
		case CX_CXXProtected:
			discription = "protected";
			break;
		case CX_CXXPrivate:
			discription = "private";
			break;
		default:;
		}

		if (is_static)
		{
			discription += " static";
		}

		discription += " member variable ";
	}

	if(!var.default_value_.empty())
	{
		defaultValue = " with default value " + var.default_value_;
	}

	std::cout << tabs << '\t' << discription << var.GetSpelling() << defaultValue << std::endl;
}

bool tokIsIdentifier(const std::string &tok)
{
	return tok[0] == '_' || ((tok[0] > 'a' && tok[0] < 'z') || (tok[0] > 'A' && tok[0] < 'Z'));
}

void AST::ScanForAttrs(CXCursor wholeFileCursor)
{
	auto range = clang_getCursorExtent(wholeFileCursor);

	CXToken* tokens = 0;
	unsigned int nTokens = 0;
	clang_tokenize(unit_, range, &tokens, &nTokens);

	bool foundOpenBrak = false;
	bool foundCloseBrak = false;
	//in an attrib sequence, record everything
	bool recordAttribs = false;
	bool lookingForName = false;
	//the attribs we are currently recording
	std::string curattribseq;

	curattribseq.reserve(64);

	for (unsigned int i = 0; i < nTokens; i++)
	{
		std::string curtok = makestr(clang_getTokenSpelling(unit_, tokens[i]));

		if (curtok[0] == '[')
		{
			if (foundOpenBrak)
			{
				foundOpenBrak = false;
				recordAttribs = true;
			}
			else
			{
				foundOpenBrak = true;
			}
		}
		else
		{
			foundOpenBrak = false;

			if (recordAttribs && curtok[0] != ']')
			{
				curattribseq += curtok;
			}
			else if (lookingForName && tokIsIdentifier(curtok) && !cppKeywords.count(curtok))
			{
				_symbolsWithAttrs.try_emplace(curtok, curattribseq);
				curattribseq.clear();
				lookingForName = false;
			}
		}

		if (curtok[0] == ']')
		{
			if (foundCloseBrak)
			{
				foundCloseBrak = false;
				recordAttribs = false;
				lookingForName = true;
			}
			else
			{
				foundCloseBrak = true;
			}
		}
		else
		{
			foundCloseBrak = false;
		}
	}

	clang_disposeTokens(unit_, tokens, nTokens);
}

void AST::Parse()
{
	if (unit_ == nullptr)
	{
		printf("Unable to parse. Quitting.\n");
	}
	else
	{
		auto cursor = clang_getTranslationUnitCursor(unit_);

		ScanForAttrs(cursor);

		clang_visitChildren(
			cursor,
			ast_visitor,
			this);
	}
}
