#ifndef __xmlparser_h__
#define __xmlparser_h__

#ifdef _WIN32
#pragma once
#endif

#include <../tinyxml/tinyxml.h>
#include "platform_specific.h"
#include "mathlib.h"

//------------------------------------------------------------------------
#define XML_TYPE_NODE TiXmlElement* 

//------------------------------------------------------------------------
#define XML_CURRENT_NODE xml_current_tree

//------------------------------------------------------------------------
#define XML_PARSE_DATA( XML_DATA )\
	for( bool xml_parse_once = true; xml_parse_once == true; )\
	for( TiXmlDocument xml_document; xml_parse_once==true;)\
	if( (xml_parse_once = xml_document.LoadData(XML_DATA)) == true )\
	for(TiXmlElement * xml_current_tree = xml_document.FirstChildElement();xml_current_tree != 0;\
	xml_current_tree = xml_current_tree->NextSiblingElement())\
	for( bool xml_else_if = false; xml_parse_once == true;xml_parse_once = false)\

//------------------------------------------------------------------------
#define XML_PARSE_FILE( XML_NAME )\
	for( bool xml_parse_once = true ; xml_parse_once == true; )\
	for( \
	TiXmlDocument xml_document;\
	xml_parse_once==true;\
	)\
	if( (xml_parse_once = xml_document.LoadFile(XML_NAME)) == true )\
	for(\
	TiXmlElement * xml_current_tree = xml_document.FirstChildElement();\
	xml_current_tree != 0;\
	xml_current_tree = xml_current_tree->NextSiblingElement()\
	)\
	for( bool xml_else_if = false; xml_parse_once == true;xml_parse_once = false)\

//------------------------------------------------------------------------
#define XML_FOR_EACH()\
	for(\
	TiXmlElement *\
	xml_it_b = xml_current_tree->FirstChildElement();\
	xml_it_b != 0;\
	xml_it_b = xml_it_b->NextSiblingElement()\
	)\
	for( bool parse_once = false; parse_once == false;)\
	for( bool xml_else_if = false; xml_parse_once == false;)\
	for( TiXmlElement * xml_current_tree = xml_it_b;\
	xml_parse_once == false;\
	xml_parse_once = true \
	)

//------------------------------------------------------------------------
#define XML_FOR_EACH_TREE( XML_TREE )\
	for(\
	TiXmlElement *\
	xml_it_b = (XML_TREE)->FirstChildElement();\
	xml_it_b != 0;\
	xml_it_b = xml_it_b->NextSiblingElement()\
	)\
	for( bool xml_parse_once = false; xml_parse_once == false;)\
	for( bool xml_else_if = false; xml_parse_once == false;)\
	for( TiXmlElement * xml_current_tree = xml_it_b;\
	xml_parse_once == false;\
	xml_parse_once = true,		xml_else_if = false\
	)

//------------------------------------------------------------------------
#define XML_ATTRIBUTES_NODE_CHARP( NAME )\
	(xml_current_tree->Attribute( NAME ))

//------------------------------------------------------------------------
#define XML_ATTRIBUTES_NODE( NAME )\
	(xml_current_tree->AttributeStr( NAME ))

//------------------------------------------------------------------------
#define XML_TITLE_NODE_CHARP\
	(xml_current_tree->Value())

//------------------------------------------------------------------------
#define XML_TITLE_NODE\
	(std::string(XML_TITLE_NODE_CHARP))

//------------------------------------------------------------------------
#define XML_VALUE_NODE()\
	((xml_current_tree->FirstChild())?\
	xml_current_tree->FirstChild()->ValueStr():\
	return std::string(););\

//------------------------------------------------------------------------
#define XML_CHECK_NODE( TITLE )\
	if( xml_else_if == true )continue;else\
	if( XML_TITLE_NODE == TITLE )\
	for(;xml_else_if == false;xml_else_if=true)

//------------------------------------------------------------------------
#define XML_ATTRIBUTES_NODE( NAME )\
	(xml_current_tree->AttributeStr( NAME ))

//------------------------------------------------------------------------
#define XML_DEF_ATTRIBUTES_NODE_W( NAME )\
	const std::wstring &NAME = XML_ATTRIBUTES_NODE( #NAME );

//------------------------------------------------------------------------
#define XML_DEF_ATTRIBUTES_NODE( NAME )\
	const std::string &NAME = XML_ATTRIBUTES_NODE( #NAME );
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

			if( VALUE.empty() == true )
			{
				return false;
			}

			sscanf(VALUE.c_str(),Type,&Value);

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
		inline bool GetValueAttribute<int>(int &Value, const std::string &Name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string &VALUE = XML_ATTRIBUTES_NODE(Name);

			if( VALUE.empty() == true )
			{
				return false;
			}

			sscanf(VALUE.c_str(),"%i", &Value);

			return true;
		}

		template<>
		inline bool GetValueAttribute<float>(float &Value, const std::string &Name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string &VALUE = XML_ATTRIBUTES_NODE(Name);

			if( VALUE.empty() == true )
			{
				return false;
			}

			sscanf(VALUE.c_str(),"%f", &Value);

			return true;
		}

		template<>
		inline bool GetValueAttribute<Vector2f>(Vector2f &Value, const std::string &Name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string &VALUE = XML_ATTRIBUTES_NODE(Name);

			if( VALUE.empty() == true )
			{
				return false;
			}

			sscanf(VALUE.c_str(),"%f;%f"
				,&Value.x,&Value.y
				);

			return true;
		}

		template<>
		inline bool GetValueAttribute<Vector>(Vector &Value, const std::string &Name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string &VALUE = XML_ATTRIBUTES_NODE(Name);

			if( VALUE.empty() == true )
			{
				return false;
			}

			sscanf(VALUE.c_str(),"%f;%f;%f"
				,&Value.x,&Value.y,&Value.z
				);

			return true;
		}

		template<>
		inline bool GetValueAttribute<Vector4f>(Vector4f &Value, const std::string &Name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string &VALUE = XML_ATTRIBUTES_NODE(Name);

			if( VALUE.empty() == true )
			{
				return false;
			}

			sscanf(VALUE.c_str(),"%f;%f;%f;%f"
				,&Value.x,&Value.y,&Value.z,&Value.w
				);

			return true;
		}

		template<>
		inline bool GetValueAttribute<Matrix>(Matrix &Value, const std::string &Name, XML_TYPE_NODE xml_current_tree)
		{
			const std::string &VALUE = XML_ATTRIBUTES_NODE(Name);

			if( VALUE.empty() == true )
			{
				return false;
			}

			sscanf(VALUE.c_str(),"%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;%f;",
				&Value._11,&Value._12,&Value._13,&Value._14,
				&Value._21,&Value._22,&Value._23,&Value._24,
				&Value._31,&Value._32,&Value._33,&Value._34,
				&Value._41,&Value._42,&Value._43,&Value._44
				);

			return true;
		}
	}
}

#endif//__xmlparser_h__

