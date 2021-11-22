#include "GameSerializer.h"
#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "MetaDB.h"
#include "GenericSerializer.h"
#include "../Memory/GameObjFactory.h"
#include "../Memory/ComponentFactory.h"
using namespace  rapidxml;

void GameSerializer::serializeComponent(const char *comptype, Component *comp, xml_node<> *dRoot, xml_document<> &doc)
{
	MetaReference meta = MetaDB::getMeta(comptype);

	GenericSerializer::serializeInternal(comp, meta, *dRoot, doc, comptype);

	//for (int i = 0; i < meta->GetMemberCount(); ++i)
	//{
	//	const Member *member = meta->GetMember(i);
	//
	//	auto memnode = doc.allocate_node(node_element, member->GetName());
	//	memnode->append_attribute(doc.allocate_attribute("type", member->GetMeta()->GetName()));
	//
	//	GenericSerializer::serializeInternal(reinterpret_cast<char *>(comp) + member->GetOffset(), member->GetMeta(), *dRoot, doc, comptype);
	//}
}
void GameSerializer::serializeGameObject(GameObject *root, xml_node<> *dRoot, xml_document<> &doc)
{
	auto components = doc.allocate_node(node_element, "Components");
	for (auto child : root->GetComponents())
	{
		auto new_child = doc.allocate_node(node_element, MetaDB::getMetaName(child.first));
		serializeComponent(MetaDB::getMetaName(child.first), child.second, new_child, doc);
		components->append_node(new_child);
	}
	dRoot->append_node(components);

	auto children = doc.allocate_node(node_element, "Children");
	for (auto child : root->GetChildren())
	{
		auto new_child = doc.allocate_node(node_element, child.first);
		serializeGameObject(child.second, new_child, doc);
		children->append_node(new_child);
	}
	dRoot->append_node(children);
}


void GameSerializer::Serialize(GameObject *root, std::string &out)
{
	xml_document<> doc;
	auto doc_root =  doc.allocate_node(node_element, root->GetName());
	
	serializeGameObject(root, doc_root, doc);
	doc.append_node(doc_root);

	print(std::back_inserter(out), doc, 0);
}

Component* GameSerializer::deserializeComponent(GameObject *obj, xml_node<>* dRoot, xml_document<>& doc)
{
	Component *ret = ComponentFactory::CreateComponentID(obj, MetaDB::getMeta(dRoot->name())->GetID());

	GenericSerializer::deserializeInternal(ret, MetaDB::getMeta(dRoot->name()), *dRoot, doc, "wew");

	return ret;
}

GameObject* GameSerializer::deserializeGameObject(GameObject* root, xml_node<>* dRoot, xml_document<>& doc)
{
	GameObject *ret =  GameObjFactory::CreateObject(root, dRoot->name());
	
	auto compsNode = dRoot->first_node("Components");
	
	for(auto compNode = compsNode->first_node(); compNode; compNode = compNode->next_sibling())
	{
		ret->AddComponentGeneric(deserializeComponent(ret, compNode, doc), MetaDB::getMeta(compNode->name())->GetID());
		if(compNode == compsNode->last_node())
		{
			break;
		}
	}

	auto childsNode = dRoot->first_node("Children");
	for (auto childNode = childsNode->first_node(); childNode; childNode = childsNode->next_sibling())
	{
		deserializeGameObject(ret, childNode, doc);
		if (childNode == compsNode->last_node())
		{
			break;
		}
	}
	return ret;
}
GameObject* GameSerializer::DeSerialize(char *in)
{
	xml_document<> doc;
	doc.parse<0>(in);
	GameObject * ret = deserializeGameObject(nullptr, doc.first_node(), doc);
	ret->OnLoad();
	return ret;
}
