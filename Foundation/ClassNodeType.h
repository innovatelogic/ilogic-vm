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

#ifndef __classnodetype_h__
#define __classnodetype_h__

#ifdef WIN32
#pragma once
#endif

#include "ClassNode.h"

template<typename T>
class EXPORT ClassNodeType : public ClassNode
{
	typedef T OwnerClass;

public:
	ClassNodeType(const char *name)
		: ClassNode(name)
	{
	}

	ClassNodeType(const char *name, TVecProperties &PropInfo)
		: ClassNode(name, PropInfo)
	{
	}

	virtual ~ClassNodeType()
	{
	}

	virtual bool PrePropertyChange(const void *pPtr, class Property_Base* Prop)
	{
		OwnerClass *pClass = const_cast<OwnerClass*>(reinterpret_cast<const OwnerClass*>(pPtr));

		return pClass->OnPrePropertyChange(Prop->GetName());
	}

	virtual void PostPropertyChange(const void *pPtr, class Property_Base* Prop)
	{
		OwnerClass *pClass = const_cast<OwnerClass*>(reinterpret_cast<const OwnerClass*>(pPtr));

		pClass->OnPropertyChanged(Prop->GetName());
	}

protected:
private:
};

#endif//__classnodetype_h__