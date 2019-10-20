#pragma once
#include "GameObject.h"
#include "rapidxml.hpp"

class GameSerializer
{
public:
	static void Serialize(GameObject *root, std::string &out);
	static GameObject * DeSerialize(char *in);
private:
	static void serializeComponent(const char *comptype, Component *comp, rapidxml::xml_node<> *dRoot, rapidxml::xml_document<> &doc);
	static void serializeGameObject(GameObject *root, rapidxml::xml_node<> *dRoot, rapidxml::xml_document<> &doc);
	static  Component *deserializeComponent(GameObject* obj, rapidxml::xml_node<>* dRoot, rapidxml::xml_document<>& doc);
	static GameObject * deserializeGameObject(GameObject *root, rapidxml::xml_node<> *dRoot, rapidxml::xml_document<> &doc);
};
