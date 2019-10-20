#include "MetaDB.h"

MetaReference::MetaReference() : _myMeta(0)
{
}

MetaReference::MetaReference(const MetaInterface* metaData) : _myMeta(metaData->GetID())
{
}

MetaReference::MetaReference(size_t id) : _myMeta(id)
{
}

MetaReference& MetaReference::operator=(const MetaInterface* meta)
{
	_myMeta = meta->GetID();
	return *this;
}

MetaReference& MetaReference::operator=(const MetaReference& meta)
{
	_myMeta = meta._myMeta;
	return *this;
}

const MetaInterface& MetaReference::operator*()
{
	return *MetaDB::getMap()[_myMeta];
}

const MetaInterface* MetaReference::operator->()
{
	return MetaDB::getMap()[_myMeta];
}

MetaReference::operator bool() const
{
	return _myMeta && MetaDB::getMap().count(_myMeta);
}

MetaInterface& MetaReference::GetDat()
{
	return *MetaDB::getMap()[_myMeta];
}
