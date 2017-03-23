#pragma once

#include "property.h"

namespace oes
{
	namespace rflex
	{
		
		//----------------------------------------------------------------------------------------------
		typedef std::vector<class Property_Base*>	TVecPropertyBase;
		typedef TVecPropertyBase::iterator			TVecPropertyBaseIter;
		typedef TVecPropertyBase::const_iterator	TVecPropertyBaseConstIter;

		struct SPropertyClass
		{
			std::string			ClassName;
			TVecPropertyBase	InheritProperties;
			int					nOverrideByteShift; // overrides byte shift for interface 
			void				*pDataObject;       // pointer to actor

			SPropertyClass(const char *Name, void *object, int shift = -1)
				: ClassName(Name)
				, pDataObject(object)
				, nOverrideByteShift(shift)
			{
			}
		};

		typedef std::vector<struct SPropertyClass*>	TVecPropertyClass;
		typedef TVecPropertyClass::const_iterator	TVecPropertyClassConstIter;
		typedef TVecPropertyClass::iterator			TVecPropertyClassIter;
	}
}