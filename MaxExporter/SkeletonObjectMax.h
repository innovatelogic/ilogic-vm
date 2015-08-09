#ifndef __skeletonobjectmax_h__
#define __skeletonobjectmax_h__

#ifdef _WIN32
#pragma once
#endif

#include "platform_specific.h"
#include "mathlib.h"

class _BoneObject
{
public:
	_BoneObject();
	virtual ~_BoneObject();

	void SetMaxNode(class IGameNode* userdata) { MaxNode = userdata; }
	void SetMaxNodeID(int id) { MaxBoneID = id; }
	void SetName(const std::string & name) { Name = name; }
	void SetID(int id) { ID = id; }
	void SetParentID(int parentID)	{ ParentID = parentID; }

	class IGameNode*	GetMaxNode() const { return MaxNode; }
	int					GetMaxBoneId() const { return MaxBoneID; }
	const std::string&  GetName() const { return Name; }
	int					GetID() const { return ID; }
	int					GetParentID() const { return ParentID; }

	void		LinkChildBone(_BoneObject * childBone);
	_BoneObject* GetParent() const { return Parent; }
	_BoneObject* GetChild() const { return Child; }
	_BoneObject* GetNext() const { return Next; }

	void		SetLTM(Matrix & ltm) { LTM = ltm; }
	void		Serialize(std::stringstream & ss);
	void		SerializeXForm(std::stringstream & ss);
	void		BuildFrame(class AnimFrame * frame, int Index);

protected:
	class IGameNode* MaxNode;
	int				 MaxBoneID;
	std::string		 Name;
	int				 ID;
	int				 ParentID;

public:
	_BoneObject*		 Parent;
	_BoneObject*		 Next;
	_BoneObject*		 Child;

	std::vector<_BoneObject*> Childs;
	Matrix			 LTM;
};


class SkeletonObjectMax
{
	typedef std::vector<_BoneObject*>	bonesVec;

public:
	SkeletonObjectMax();
	virtual ~SkeletonObjectMax();
	
	void					Serialize(std::stringstream &ss);
	void					SerializeXForms(std::stringstream &ss);

	virtual	int				Build(class IGameObject * Object);

	virtual size_t			GetNumBones() const { return Bones.size(); }
	_BoneObject*			GetRawBone(size_t Index) const { return Bones.at(Index); }
	_BoneObject*			GetRootBone() const { return RootBone; }
	virtual int				GetBoneIDByMaxID(int boneMaxID) const;
	virtual _BoneObject*	FindBoneByMaxID(int maxBoneID) const;

	virtual _BoneObject*	AddMaxBone(class IGameNode* boneNode, int maxBoneID);

protected:
public:
	bonesVec				Bones;
	_BoneObject*			RootBone;
};

#endif//__skeletonobjectmax_h__
