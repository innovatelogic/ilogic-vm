#ifndef __registry_h__
#define __registry_h__

#ifdef _WIN32
#pragma once
#endif

#include "game_types.h"
#include "..\Foundation\StdafxFoundation.h"

class CORESDK_API Registry
{
	struct TypeInfo
	{
		bool		bEdVisible;
		std::string IconRef;
	};

	typedef std::map<std::string, TypeInfo>		TMapTypeInfo;
	typedef std::map<std::string, std::string>	TMapStringString;
	
public:
	Registry();
	virtual ~Registry();

	bool Load(const std::string &filename);
    
	Vector2f GetStartResolution() { return Resolution; }

	/** 
	 * Get object visible for direct property edit in objects browser.
	 *
	 * @return true if visible of false in other case.
	 */
	bool IsEditorVisible(const char * TypeName) const;
    
    /** app resolution */
	Vector2f				Resolution;

	TMapTypeInfo			MapTypeInfo;
	TMapStringString		MapTypeIcons;
	TMapStringString		MapOprationsIcons;

	Vector2f				ControlAxisX;
	Vector2f				ControlAxisY;
	Vector2f				ControlBox;
	Vector2f				ControlBoxOrigin;

protected:
private:
	Registry(const Registry & Source);
};

#endif//__registry_h__