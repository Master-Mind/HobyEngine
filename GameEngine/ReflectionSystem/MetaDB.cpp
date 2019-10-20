#include "MetaDB.h"
#include <string>
#include "GameObject.h"
#include "../glm/glm/vec2.hpp"
#include "../glm/glm/vec3.hpp"
#include "../glm/glm/vec4.hpp"
#include "../glm/glm/gtc/quaternion.hpp"
#include "VariantRange.h"
#include "GraphicsComps/SpriteRenderer.h"
MetaDB *MetaDB::METADB;
MetaDB::MetaDB()
{
	METADB = this;
}

void MetaDB::Init()
{
	MetaFixup();
}

const char *MetaDB::getMetaName(size_t id)
{
	return getMap()[id]->GetName();
}

MetaReference MetaDB::getMeta(std::string type)
{
	return getNameMap()[type];
}

const MetaReference MetaDB::getMeta(size_t id)
{
	return getMap()[id];
}

const size_t MetaDB::getTheFuckingOffsetForFuckingLuaBecauseFuckingHeaderFiles(const size_t id, const int member)
{
	return getMap()[id]->GetMember(member)->GetOffset();
}

void MetaDB::MetaFixup()
{
	FixupInheritance();
}

void MetaDB::FixupInheritance()
{
	for (auto met : getMap())
	{
		if (met.second->GetParentClass())
		{
			met.second->GetParentClass().GetDat().AddSubClass(met.second);
		}
	}
}

DEFINE_META(SpriteRenderer)
ADD_ASSET(_mySprite)
END_META(SpriteRenderer) //Compiler gets rid of exactly this one and no others except for here
using namespace std;
DEFINE_META(char)
_toStr = [](char i) { return to_string(i); };
_fromStr = [](const char *str) {	return str[0]; };
_isBase = true;
END_META(char)

DEFINE_META(U8)
_toStr = [](unsigned char i) { return to_string(i); };
_fromStr = [](const char *str) {	return str[0]; };
_isBase = true;
END_META(U8)

DEFINE_META(S16)
_toStr = [](short i) { return to_string(i); };
_fromStr = [](const char *str) {	return stoi(str); };
_isBase = true;
END_META(S16)

DEFINE_META(U16)
_toStr = [](unsigned short i) { return to_string(i); };
_fromStr = [](const char *str) {	return stoi(str); };
_isBase = true;
END_META(U16)

DEFINE_META(S32)
_toStr = [](int i) { return to_string(i); };
_fromStr = [](const char *str) {	return stoi(str); };
_isBase = true;
END_META(S32)

DEFINE_META(U32)
_toStr = [](unsigned i) { return to_string(i); };
_fromStr = [](const char *str) {	return stoi(str); };
_isBase = true;
END_META(U32)

DEFINE_META(float)
_toStr = [](float i) { return to_string(i); };
_fromStr = [](const char *str) {	return stof(str); };
_isBase = true;
END_META(float)

DEFINE_META(S64)
_toStr = [](long i) { return to_string(i); };
_fromStr = [](const char *str) {	return stol(str); };
_isBase = true;
END_META(S64)

DEFINE_META(U64)
_toStr = [](unsigned long i) { return to_string(i); };
_fromStr = [](const char *str) {	return stoul(str); };
_isBase = true;
END_META(U64)

DEFINE_META(double)
_toStr = [](double i) { return to_string(i); };
_fromStr = [](const char *str) {	return stod(str); };
END_META(double)

DEFINE_META(long double)
_toStr = [](long double i) { return to_string(i); };
_fromStr = [](const char *str) {	return stod(str); };
_isBase = true;
MetaDB::AddMetaData<T>(static_cast<MetaInterface *>(this)); } MetaData<long double> Datalongdouble("long double");

DEFINE_META(string)
_toStr = [](string i) { return i; };
_fromStr = [](const char *str) {	return str; };
_isBase = true;
END_META(string)

DEFINE_META(NameType)
_toStr = [](NameType i) { return i.toString(); };
_fromStr = [](const char *str) {	return str; };
_isBase = true;
END_META(NameType)


using namespace glm;
DEFINE_META(vec2)
ADD_MEMBER(x)
ADD_MEMBER(y)
END_META(vec2)

DEFINE_META(vec3)
ADD_MEMBER(x)
ADD_MEMBER(y)
ADD_MEMBER(z)
END_META(vec3)

DEFINE_META(vec4)
ADD_MEMBER(x)
ADD_MEMBER(y)
ADD_MEMBER(z)
ADD_MEMBER(w)
END_META(vec4)

DEFINE_META(quat)
ADD_MEMBER(x)
ADD_MEMBER(y)
ADD_MEMBER(z)
ADD_MEMBER(w)
END_META(quat)

DEFINE_META(GameObjMap)
_toRange = [](VariantRange &range, GameObjMap &map) {VariantRange::AssignData(range, map); };
END_META(GameObjMap)

DEFINE_META(CompMap)
_toRange = [](VariantRange &range, CompMap &map) {VariantRange::AssignData(range, map); };
END_META(CompMap)