#ifndef __brush_abstractinterface_h__
#define __brush_abstractinterface_h__

#ifdef _WIN32
#pragma once
#endif

#include "platform_specific.h"

class Brush_AbstractInterface
{
public:
	Brush_AbstractInterface();
	virtual ~Brush_AbstractInterface();

	std::string				GetTypeGenerated() { return TypeGenerated; }
	void					SetTypeGenerated(std::string &type) { TypeGenerated = type; }

	std::string				GetKeyTypeGenerated() { return KeyTypeGenerated; }
	void					SetKeyTypeGenerated(std::string &type) { KeyTypeGenerated = type; }

protected:
private:	
	std::string				TypeGenerated;
	std::string				KeyTypeGenerated;
};

#endif//__brush_abstractinterface_h__