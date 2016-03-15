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

#ifndef __classnodeinterface_h__
#define __classnodeinterface_h__

#ifdef WIN32
#pragma once
#endif

#include "OEMBase.h"
#include "IBaseInterface.h"
#include "ClassNode.h"

template<typename T>
class COMMON_BASE_EXPORT CClassNodeInterface : public ClassNode
{
	typedef T OwnerClass;

public:
	CClassNodeInterface(const char *name)
		: ClassNode(name)
	{
	}

	CClassNodeInterface(const char *name, TVecProperties &PropInfo)
	: ClassNode(name, PropInfo)
	{
	}
	
	virtual ~CClassNodeInterface()
	{
	}

	virtual bool PrePropertyChange(const void *pPrt, class Property_Base *pProp)
	{
		return true;
	}
	
	virtual void PostPropertyChange(const void *pPtr, class Property_Base *pProp)
	{
		CActor *pClass = const_cast<CActor*>(reinterpret_cast<const CActor*>(pPtr));

		pClass->OnPropertyChanged(pProp->GetName());
	}

protected:
private:
public:
};

#endif//__classnodeinterface_h__