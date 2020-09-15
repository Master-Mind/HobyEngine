#pragma once
#include <clang-c/Index.h>
#include <string>
#include <vector>
#include <unordered_map>

#define ASTFRIENDS \
friend class AST;\
friend CXChildVisitResult ast_full_parse(CXCursor c, CXCursor parent, CXClientData astVoid);\
friend CXChildVisitResult ast_visitor(CXCursor c, CXCursor parent, CXClientData astVoid);\
friend CXChildVisitResult ast_class_parse(CXCursor c, CXCursor parent, CXClientData nodeVoid);\
friend CXChildVisitResult ast_var_parse(CXCursor c, CXCursor parent, CXClientData varVoid);\
friend CXChildVisitResult ast_func_parse(CXCursor c, CXCursor parent, CXClientData funcVoid);\
friend class ASTWriter;\
friend class ParserOutputFormat

class AST
{
public:
	class Node
	{
	public:
		Node(CXCursor c, const std::string &attributes, AST *handler);
		virtual ~Node();
		virtual const std::string &GetSpelling() const;
		virtual const std::string &GetKindSpelling() const;
		virtual std::vector<std::string> get_attributes() const;
		void set_attributes(const std::string& attributes);
		const AST* GetHandler() const;
		CX_CXXAccessSpecifier GetAccess() { return access_; }
	private:
		ASTFRIENDS;
		std::string spelling_;
		std::vector<std::string> attributes_;
		std::string attrStr_;
		std::vector<Node*> children;
		CXCursorKind kind_;
		std::string kindSpelling_;
		AST* handler_;
		CX_CXXAccessSpecifier access_ = CX_CXXPublic;
	};

	friend class Node;
	class Variable : public Node
	{
	public:
		Variable(const CXCursor& c, const std::string& attributes, AST* handler)
			: Node(c, attributes, handler)
		{
		}
	private:
		std::string default_value_;
		ASTFRIENDS;
	};

	class Function : public Node
	{
	public:
		Function(const CXCursor& c, const std::string& attributes, AST* handler)
			: Node(c, attributes, handler)
		{
		}

		~Function()
		{
			for (Variable* param : params_)
			{
				delete param;
			}
		}
	private:
		ASTFRIENDS;
		std::vector<Variable*> params_;
	};

	class Class : public Node
	{
	public:
		Class(const CXCursor& c, const std::string& attributes, AST* handler)
			: Node(c, attributes, handler)
		{
		}

		~Class()
		{
			for (Function* func : member_funcs_)
			{
				delete func;
			}

			for (Function* func : static_funcs_)
			{
				delete func;
			}

			for (Variable* var : member_vars_)
			{
				delete var;
			}

			for (Variable* var : static_vars_)
			{
				delete var;
			}
		}
	private:
		std::string base_class_;
		std::vector<Function*> member_funcs_;
		std::vector<Function*> static_funcs_;
		std::vector<Variable*> member_vars_;
		std::vector<Variable*> static_vars_;
		ASTFRIENDS;
	};

	AST(CXIndex index, const char* filename, const std::vector<char*> &cmdargs);
	void Parse();
	~AST();
	void dump() const;

	const std::vector<std::string> &GetNameSpaces() const;
	const std::string& GetAttrsForSymbol(const std::string& symbol) const;
private:
	void dump_internal(const Node& node, int level) const;
	void dump_func(std::string& tabs, const Function& func, bool is_member, bool is_static) const;
	void dump_var(std::string& tabs, const Variable& var, bool is_member, bool is_static) const;
	void ScanForAttrs(CXCursor wholeFileCursor);
	ASTFRIENDS;
	Node root_;
	CXTranslationUnit unit_;
	size_t outputGuestimate_ = 0;
	std::vector<std::string> _namespaces;
	std::unordered_map<std::string, std::string> _symbolsWithAttrs;
};

