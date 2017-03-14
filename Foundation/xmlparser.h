

#pragma once

#include "tinyxml2.h"
#include "platform_specific.h"
#include "mathlib.h"

//------------------------------------------------------------------------
#define XML_TYPE_NODE tinyxml2::XMLElement* 

//------------------------------------------------------------------------
#define XML_CURRENT_NODE xml_current_tree

//------------------------------------------------------------------------
#define XML_PARSE_DATA( XML_DATA )\
	for( bool xml_parse_once = true; xml_parse_once == true; )\
	for( tinyxml2::XMLDocument xml_document; xml_parse_once==true;)\
	if( xml_parse_once = (xml_document.Parse(XML_DATA) == tinyxml2::XML_SUCCESS) )\
	for (tinyxml2::XMLElement *xml_current_tree = xml_document.FirstChildElement(); xml_current_tree != nullptr;\
	xml_current_tree = xml_current_tree->NextSiblingElement())\
	for( bool xml_else_if = false; xml_parse_once == true;xml_parse_once = false)\

//------------------------------------------------------------------------
#define XML_PARSE_FILE( XML_NAME )\
	for (bool xml_parse_once = true ; xml_parse_once == true; )\
	for (\
	tinyxml2::XMLDocument xml_document;\
	xml_parse_once==true;\
	)\
	if (xml_parse_once = (xml_document.LoadFile(XML_NAME) == tinyxml2::XML_SUCCESS) )\
	for (\
	tinyxml2::XMLElement *xml_current_tree = xml_document.FirstChildElement();\
	xml_current_tree != nullptr;\
	xml_current_tree = xml_current_tree->NextSiblingElement()\
	)\
	for (bool xml_else_if = false; xml_parse_once == true; xml_parse_once = false)\

//------------------------------------------------------------------------
#define XML_FOR_EACH()\
	for(\
	tinyxml2::XMLElement*\
	xml_it_b = xml_current_tree->FirstChildElement();\
	xml_it_b != nullptr;\
	xml_it_b = xml_it_b->NextSiblingElement()\
	)\
	for( bool parse_once = false; parse_once == false;)\
	for( bool xml_else_if = false; xml_parse_once == false;)\
	for( tinyxml2::XMLElement *xml_current_tree = xml_it_b;\
	xml_parse_once == false;\
	xml_parse_once = true \
	)

//------------------------------------------------------------------------
#define XML_FOR_EACH_TREE( XML_TREE )\
	for(\
	tinyxml2::XMLElement *\
	xml_it_b = (XML_TREE)->FirstChildElement();\
	xml_it_b != nullptr;\
	xml_it_b = xml_it_b->NextSiblingElement()\
	)\
	for( bool xml_parse_once = false; xml_parse_once == false;)\
	for( bool xml_else_if = false; xml_parse_once == false;)\
	for( tinyxml2::XMLElement *xml_current_tree = xml_it_b;\
	xml_parse_once == false;\
	xml_parse_once = true,	xml_else_if = false\
	)

//------------------------------------------------------------------------
#define XML_ATTRIBUTES_NODE_CHARP( NAME )\
	(xml_current_tree->Attribute( NAME ))

//------------------------------------------------------------------------
#define XML_TITLE_NODE_CHARP\
	(xml_current_tree->Value())

//------------------------------------------------------------------------
#define XML_TITLE_NODE\
	(std::string(XML_TITLE_NODE_CHARP))

//------------------------------------------------------------------------
#define XML_VALUE_NODE()\
	((xml_current_tree->FirstChild())?\
	xml_current_tree->FirstChild()->Value():\
	return std::string(););\

//------------------------------------------------------------------------
#define XML_CHECK_NODE( TITLE )\
	if( xml_else_if == true )continue;else\
	if( XML_TITLE_NODE == TITLE )\
	for(;xml_else_if == false;xml_else_if=true)

//------------------------------------------------------------------------
#define XML_ATTRIBUTES_NODE( NAME )\
	(xml_current_tree->Attribute( NAME ))

//------------------------------------------------------------------------
#define XML_DEF_ATTRIBUTES_NODE_W( NAME )\
	const char *NAME = XML_ATTRIBUTES_NODE( #NAME );

//------------------------------------------------------------------------
#define XML_DEF_ATTRIBUTES_NODE( NAME )\
	std::string NAME;\
    if (const char *RAW##NAME = XML_ATTRIBUTES_NODE(#NAME))\
    {\
        NAME = RAW##NAME;\
    }


//------------------------------------------------------------------------
#define XML_MEMBER_ATTRIBUTES_NODE( NAME )\
	NAME = XML_ATTRIBUTES_NODE( #NAME );

//------------------------------------------------------------------------
#define XML_DEF_ATTRIBUTES_NODE_BOOL(NAME)\
	std::string NAME##INTERNAL_STRING = XML_ATTRIBUTES_NODE( #NAME );\
	std::transform(NAME##INTERNAL_STRING.begin(), NAME##INTERNAL_STRING.end(), NAME##INTERNAL_STRING.begin(), _toupper);\
	const BOOL NAME = (NAME##INTERNAL_STRING == "TRUE") ? TRUE : FALSE;

//------------------------------------------------------------------------
#define XML_VALUE_ATTRIBUTE( NAME, VALUE )\
	xml::parse::GetValueAttribute( (VALUE) , (NAME) , XML_CURRENT_NODE )
//------------------------------------------------------------------------
namespace xml
{
	namespace parse
	{
		template<class T>
		bool GetValueAttributeSimple(T &Value, const std::string &Name, const char *Type, XML_TYPE_NODE xml_current_tree)
		{
			const std::string &VALUE = XML_ATTRIBUTES_NODE(Name);

			if (VALUE.empty()){
				return false;
			}

			std::sscanf(VALUE.c_str(), Type, &Value);

			return true;
		}

		template<class T>
		inline bool GetValueAttribute(T &Value, const std::string &Name, XML_TYPE_NODE xml_current_tree)
		{
			char strError[1024];

			sprintf(strError,"Error parse xml: Invalid Attribute '%s'"
				, Name.c_str()
				);

			throw std::exception(strError);

			return false;
		}

		template<>
		inline bool GetValueAttribute<int>(int &value, const std::string &Name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string VALUE(XML_ATTRIBUTES_NODE(Name.c_str()));

			if (VALUE.empty()){
				return false;
			}

            value = std::stoi(VALUE);

			return true;
		}

		template<>
		inline bool GetValueAttribute<float>(float &value, const std::string &name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string VALUE(XML_ATTRIBUTES_NODE(name.c_str()));

			if (VALUE.empty()){
				return false;
			}

            value = std::stof(VALUE);

			return true;
		}

		template<>
		inline bool GetValueAttribute<Vector2f>(Vector2f &value, const std::string &name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string VALUE(XML_ATTRIBUTES_NODE(name.c_str()));

			if (VALUE.empty()){
				return false;
			}

            std::stringstream ss(VALUE);
            float number = 0.f;
            size_t n = 0;

            while (ss >> number)
            {
                assert(n < 2);
                value[n++] = number;
                if (ss.peek() == ';') {
                    ss.ignore();
                }
            }
			return true;
		}

		template<>
		inline bool GetValueAttribute<Vector>(Vector &value, const std::string &name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string VALUE(XML_ATTRIBUTES_NODE(name.c_str()));

			if (VALUE.empty()){
				return false;
			}

            std::stringstream ss(VALUE);
            float number = 0.f;
            size_t n = 0;

            while (ss >> number)
            {
                assert(n < 3);
                value[n++] = number;
                if (ss.peek() == ';') {
                    ss.ignore();
                }
            }
			return true;
		}

		template<>
		inline bool GetValueAttribute<Vector4f>(Vector4f &value, const std::string &name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string VALUE(XML_ATTRIBUTES_NODE(name.c_str()));

			if (VALUE.empty()){
				return false;
			}

            std::stringstream ss(VALUE);

            float number = 0.f;
            size_t n = 0;
            while (ss >> number) 
            {
                assert(n < 4);
                value[n++] = number;
                if (ss.peek() == ';') {
                    ss.ignore();
                }
            }
			return true;
		}

		template<>
		inline bool GetValueAttribute<Matrix>(Matrix &value, const std::string &name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string VALUE(XML_ATTRIBUTES_NODE(name.c_str()));

			if (VALUE.empty()){
				return false;
			}

            std::stringstream ss(VALUE);

            float number = 0.f;
            size_t n = 0;
            while (ss >> number)
            {
                assert(n < 16);
                value[n++] = number;
                if (ss.peek() == ';') {
                    ss.ignore();
                }
            }
			return true;
		}
	}
}