//----------------------------------------------------------------------------------------------
// OpenES: Open Entertainment System
// Copyright (C) 2010  Yura Gunko email: yura.gunko@gmail.com
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------------------------
#ifndef __classnode_h__
#define __classnode_h__

#ifdef WIN32
#pragma once
#endif

#include "platform_specific.h"

class EXPORT ClassNode
{
public:
	typedef std::vector<class ClassNode*>		TVecClassNode;
	typedef TVecClassNode::iterator				TVecClassNodeIterator;
	typedef TVecClassNode::const_iterator		TVecClassNodeConstIterator;
	typedef TVecClassNode::reverse_iterator		TVecClassNodeRevIterator;

	typedef std::vector<struct SInterfaceDecl*>	TVecInterfaces;
	typedef TVecInterfaces::iterator			TVecInterfaceIter;
	typedef TVecInterfaces::const_iterator		TVecInterfaceConstIter;

	typedef std::vector<class Property_Base*>	TVecProperties;
	typedef TVecProperties::iterator			TVecPropertyIterator;
	typedef TVecProperties::const_iterator		TVecPropertyConstIterator;
	
public:
	ClassNode(const char *name);
	ClassNode(const char *name, TVecProperties &PropInfo);
	virtual ~ClassNode();

	void		Release();

	ClassNode*	Find(const char *Type);
	ClassNode*	AddChild(ClassNode *pNode);
	ClassNode*	RemoveNode(ClassNode *pNode);

	class Property_Base*	GetProperty(const char* Name) const;
	bool					AddProperty(const char* Name, Property_Base *pProp);

	void	SetProprties(const class IPropertiesAllocator *pPropAlloc);
	void	SetProprties(const class Property_Base** Arr, int Count);

	class ClassNode* GetRootNode() const { return m_pRootNode; }
	void			 SetRootNode(ClassNode *pNode) { m_pRootNode = pNode; }

	virtual bool	PrePropertyChange(const void *pPrt, class Property_Base *pProp);
	virtual void	PostPropertyChange(const void *pPrt, class Property_Base *pProp);

	const char* GetName() const { return Name.c_str(); }

	Property_Base** GetPropsRaw() const { return m_pPropsRaw; }
	unsigned int	GetPropsSize() const { return m_PropsSize; }

	const ClassNode::TVecInterfaces& GetInterfaceProps() const { return m_VecInterfaces; }
	const ClassNode::TVecProperties& GetProperties() const { return PropertyMap; }

	SInterfaceDecl* GetInterfaceDecl(const char *Name) const;

	bool IsInheritBaseClass(const char *Name) const;

	const TVecClassNode&	GetChilds() const { return Childs; }

	const TVecInterfaces&	GetInterfaces() const { return m_VecInterfaces; }

public:
	std::string				Name;

	TVecClassNode			Childs;
	TVecInterfaces			m_VecInterfaces;

	/** property map */
	TVecProperties			PropertyMap;

private:
	ClassNode				*m_pRootNode;
	
	IPropertiesAllocator	*m_pPropAlloc;

	Property_Base			**m_pPropsRaw;
	unsigned int			m_PropsSize;

	struct SInterfaceDecl	**m_pInterfaces;
	unsigned int			m_nIntfSize;
};

#endif//__classnode_h__