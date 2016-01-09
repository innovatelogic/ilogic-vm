#ifndef __styleset_h__
#define __styleset_h__

#ifdef _WIN32
#pragma once
#endif

#include "../Foundation/xmlParser.h"
#include "VisualStyle.h"

class StyleSet
{
public:
	StyleSet();
   ~StyleSet();

    BOOL Load(tinyxml2::XMLElement *Tree);

	bool GetStyle(const std::string& Name, VisualStyle *& OutStyle);

	std::map<std::string, VisualStyle*> MapStyleset;
};


#endif//__styleset_h__