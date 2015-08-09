#ifndef __exportypes_h__
#define __exportypes_h__

#ifdef WIN32
#pragma once
#endif

#include <vector>
#include <sstream> 
#include "mathlib.h"

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CollisionShapeBase
{
public:
	CollisionShapeBase()
		: TypeId(INDEX_NONE)
	{
	}

	virtual void Serialize(std::ofstream &Stream) = 0;

	int GetTypeId() const { return TypeId; }

	Matrix xform;
	int TypeId;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CollisionShapeBox : public CollisionShapeBase
{
public:
	CollisionShapeBox()
		: CollisionShapeBase()
		, Ext(0.f, 0.f, 0.f)
	{
		 TypeId = 0;
	}

	virtual void Serialize(std::ofstream &Stream)
	{
		Stream << "<shape type=\"box\" ";
		Stream << "Ext=\"" << Ext.x << ";" << Ext.y << ";" << Ext.z << "\" ";
		Stream << "LTM=\"" << xform[0] << ";" << xform[1] << ";" << xform[2] << ";" << xform[3] << ";" <<
							  xform[4] << ";" << xform[5] << ";" << xform[6] << ";" << xform[7] << ";" <<
							  xform[8] << ";" << xform[9] << ";" << xform[10] << ";" << xform[11] << ";" <<
							  xform[12] << ";" << xform[13] << ";" << xform[14] << ";" << xform[15] << "\"";
		Stream << "/>" << std::endl;
	}

	Vector Ext;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CollisionShapeSphere : public CollisionShapeBase
{
public:

	CollisionShapeSphere()
		: CollisionShapeBase()
		, Radius(0.f)
	{
		TypeId = 1;
	}

	virtual void Serialize(std::ofstream &Stream)
	{
		/*ss << GetTypeId() << " " <<
			xform[0] << " " << xform[1] << " " << xform[2] << " " << xform[3] << " " <<
			xform[4] << " " << xform[5] << " " << xform[6] << " " << xform[7] << " " <<
			xform[8] << " " << xform[9] << " " << xform[10] << " " << xform[11] << " " <<
			xform[12] << " " << xform[13] << " " << xform[14] << " " << xform[15] << " " <<
			Radius << std::endl;*/
	}

	float Radius;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CollisionShapeCapsule : public CollisionShapeBase
{
public:
	CollisionShapeCapsule()
		: CollisionShapeBase()
		, Radius(0.f)
		, Height(0.f)
	{
		TypeId = 2;
	}

	virtual void Serialize(std::ofstream &Stream)
	{
		/*ss << GetTypeId() << " " <<
			xform[0] << " " << xform[1] << " " << xform[2] << " " << xform[3] << " " <<
			xform[4] << " " << xform[5] << " " << xform[6] << " " << xform[7] << " " <<
			xform[8] << " " << xform[9] << " " << xform[10] << " " << xform[11] << " " <<
			xform[12] << " " << xform[13] << " " << xform[14] << " " << xform[15] << " " <<
			Radius << " " << Height << std::endl;*/
	}

	float Radius;
	float Height;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CollisionShapeMesh : public CollisionShapeBase
{
public:
	CollisionShapeMesh()
		: CollisionShapeBase()
	{
		TypeId = 3;
	}

	virtual void Serialize(std::ofstream &Stream)
	{
		/*ss << GetTypeId() << " " <<
				xform[0] << " " << xform[1] << " " << xform[2] << " " << xform[3] << " " <<
				xform[4] << " " << xform[5] << " " << xform[6] << " " << xform[7] << " " <<
				xform[8] << " " << xform[9] << " " << xform[10] << " " << xform[11] << " " <<
				xform[12] << " " << xform[13] << " " << xform[14] << " " << xform[15] << std::endl;

		ss << Points.size() << std::endl;
		for (size_t Index = 0; Index < Points.size(); ++Index){
			ss << Points[Index].x << " " << Points[Index].y << " " << Points[Index].z << std::endl;
		}

		ss << Idndexes.size() << std::endl;
		for (size_t Index = 0; Index < Idndexes.size(); ++Index){
			ss << Idndexes[Index] << std::endl;
		}*/
	}

	std::vector<Vector>			Points;
	std::vector<unsigned int>	Idndexes;
};

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
class CollisionShapeConvex : public CollisionShapeMesh
{
public:
	CollisionShapeConvex()
		: CollisionShapeMesh()
	{
		TypeId = 4;
	}
};

#endif//__exportypes_h__