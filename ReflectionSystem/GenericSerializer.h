#pragma once
#include <string>
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "MetaDB.h"

class MetaReference;

class GenericSerializer
{
public:
	template<typename T>
	static std::string serialize(T *root, const char *filename)
	{
		std::string out;
		rapidxml::xml_document<> doc;
		auto doc_root = doc.allocate_node(rapidxml::node_element, filename);
		MetaReference rootMeta = GetMetaData<T>();
		serializeInternal(root, rootMeta, doc_root, doc);
		doc.append_node(doc_root);

		rapidxml::print(std::back_inserter(out), doc, 0);

		return out;
	}
	friend class GameSerializer;
private:
	static void serializeInternal(void *root, MetaReference ref, rapidxml::xml_node<> &dRoot, rapidxml::xml_document<> &doc, const char *rname);
	static void deserializeInternal(void *root, MetaReference ref, rapidxml::xml_node<> &dRoot, rapidxml::xml_document<> &doc, const char *rname);

};
