#pragma once
class MetaInterface;
class MetaReference
{
public:
	MetaReference();
	MetaReference(const MetaInterface *metaData);
	MetaReference(size_t id);
	MetaReference &operator=(const MetaInterface *meta);
	MetaReference &operator=(const MetaReference &meta);
	const MetaInterface &operator*();
	const MetaInterface *operator->();
	operator bool() const;
private:
	size_t _myMeta;
	//these have to be used by the metadb, hence the friend
	friend class MetaDB;
	MetaInterface& GetDat();
};
