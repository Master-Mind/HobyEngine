#pragma once
class Member;
#include <string>
#include "MetaReference.h"

class MetaInterface
{
public:
	//returns string for ease of use
	virtual const char *GetName()const = 0;
	virtual const Member *GetMember(int memNum)const = 0;
	virtual size_t GetMemberCount() const = 0;
	virtual void Construct(void *memblock) const {}
	virtual std::string ToString(void *memblock) const { return ""; }
	//calls constructor
	virtual void FromString(const char *str, void *memblock) const {}
	virtual bool IsRange() const { return false; };
	virtual bool IsBase() const { return false; }
	virtual size_t GetID() const = 0;
	virtual bool HasAttribute(size_t type_id) const = 0;
	virtual const std::vector<MetaReference>& GetSubClasses() const = 0;
	virtual MetaReference GetParentClass() const = 0;
private:
	//these should only be used by metadb, hence the friend decl
	friend class MetaDB;
	virtual void AddSubClass(MetaReference ref) = 0;
};