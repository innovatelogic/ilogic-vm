#pragma once

#include "OEMBase.h"
#include "ClassNode.h"

template<typename T>
class COMMON_BASE_EXPORT ClassNodeType : public ClassNode
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