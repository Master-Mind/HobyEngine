#include "GenericSerializer.h"
#include "MetaDB.h"
using namespace rapidxml;


void GenericSerializer::serializeInternal(void* root, MetaReference ref, xml_node<>& dRoot, xml_document<>& doc, const char *rname)
{
	char *rc = reinterpret_cast<char*>(root);
	for (int i = 0; i < ref->GetMemberCount(); ++i)
	{
		auto memptr = ref->GetMember(i);
		xml_node<char>* member;

		if(memptr->GetMeta()->IsBase())
		{
			std::string str = memptr->GetMeta()->ToString(rc + memptr->GetOffset());
			auto val = doc.allocate_string(str.c_str(), str.size());
			member = doc.allocate_node(node_element, ref->GetMember(i)->GetName(), val, std::strlen(memptr->GetName()), str.size());
		}
		else
		{
			member = doc.allocate_node(node_element, ref->GetMember(i)->GetName());
			
			serializeInternal(rc + memptr->GetOffset(), memptr->GetMeta(), *member, doc, rname);
		}
		member->append_attribute(doc.allocate_attribute("type", memptr->GetMeta()->GetName()));

		dRoot.append_node(member);
	}
}

void GenericSerializer::deserializeInternal(void* root, MetaReference ref, xml_node<>& dRoot, xml_document<>& doc, const char* rname)
{
	char *rc = reinterpret_cast<char*>(root);
	for (int i = 0; i < ref->GetMemberCount(); ++i)
	{
		auto memptr = ref->GetMember(i);
		xml_node<> *mem_node = dRoot.first_node(memptr->GetName());
		if(mem_node)
		{
			if(memptr->GetMeta()->IsBase())
			{
				memptr->GetMeta()->FromString(mem_node->value(), rc + memptr->GetOffset());
			}
			else
			{
				deserializeInternal(rc + memptr->GetOffset(), memptr->GetMeta(), *mem_node, doc, rname);
			}
		}
	}
}
