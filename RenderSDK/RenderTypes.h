#ifndef __rendertypes_h__
#define __rendertypes_h__

#ifdef _WIN32
#pragma once
#endif

#include "StdafxFoundation.h"

struct AnimState
{
	std::string	AnimName;
	float		weight;
	float		DeltaTime;
	bool		bTrackTarget;
	Vector		PointTarget;
	std::vector<unsigned int> BonesMask;
	bool		bValid;

	bool operator == (const AnimState& Other) { return AnimName == Other.AnimName; }
	bool IsValid() const { return bValid; }

	AnimState(const std::string & name, float deltaTime, float w)
		: AnimName(name)
		, weight(w)
		, bTrackTarget(false)
		, PointTarget(0.f, 0.f, 0.f)
		, DeltaTime(deltaTime)
		, bValid(true)
	{
	}
};

struct SetAnimState
{
	std::vector<AnimState> States;
};

// mesh 
struct MeshMaterial
{
	std::string Name;
	int id;

	std::string FX;
	std::string tehnique;

	// material colors...
	Vector4f            diffuse;        // diffuse color
	Vector4f            specular;       // specular color
	float               shininess;      // specular exponent
	Vector4f            ambient;        // ambient color
	Vector4f            emission;       // emitted light intensity of the material
	float               transparent;    // level of transparency
	// fog properties...
	bool                fog;            // fog
	Vector4f            fog_color;      // fog color

	std::vector<unsigned int>	textures;
	std::vector<unsigned int>	tex_channel;
};

struct MeshMaterialSet
{
	std::string	ObjectFile;
	std::string	SklFile;
	std::string	TrkFile;
	std::string MrkFile;
	std::string ColFile;

	std::vector<std::string> Textures;
	std::vector<MeshMaterial> Materials;
};
#endif//__rendertypes_h__