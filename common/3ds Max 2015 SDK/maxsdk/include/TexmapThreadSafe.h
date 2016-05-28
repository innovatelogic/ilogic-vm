//
// Copyright 2013 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//

#pragma once

#include ".\BaseInterface.h"

/** Interface ID of class TexmapThreadSafe. */
#define ITEXMAP_THREAD_SAFE Interface_ID(0x47956311, 0x2216740a)

namespace MaxSDK
{
	/** TexmapThreadSafe provides any texture plugin the thread safe support information in 3ds Max 2015. 
	Max system will use multiple thread to accelerates texmap render if the Texmap function is thread safe.
	Every Texmap will have an TexmapThreadSafe available as it is added as a BaseInterface by Max system. 
	To set thread safe flag would look like this:
	\code
	MaxSDK::TexmapThreadSafe* pTexmapThreadSafe = static_cast<MaxSDK::TexmapThreadSafe*>(GetInterface(ITEXMAP_THREAD_SAFE));
	DbgAssert(pTexmapThreadSafe != nullptr);
	pTexmapThreadSafe->SetFunctionFlag(MaxSDK::TexmapThreadSafe::FunctionEvalMono, true);
	pTexmapThreadSafe->SetFunctionFlag(MaxSDK::TexmapThreadSafe::FunctionEvalColor, true);
	\endcode
	*/
	class TexmapThreadSafe : public BaseInterface
	{
	public:

		/** This enum type tells Texmap function.*/
		enum TexmapFunction
		{
			FunctionEvalColor = 1,			/*!< FunctionEvalColor EvalColor(ShadeContext& sc) of class Texmap */
			FunctionEvalMono = 1 << 1,		/*!< FunctionEvalMono  EvalMono(ShadeContext& sc) of class Texmap*/
		};

		/** Retrieves the interface ID of TexmapThreadSafe.
		*/
		virtual Interface_ID GetID() { return ITEXMAP_THREAD_SAFE; }

		/** Specifies the function of the texmap with thread safe flag.
		It requires the specific Texmap function should be thread safe between different instances of Texmap.
		\param[in] function the specific Texmap function.
		\param[in] bThreadSafe whether the specific function is thread safe.
		*/
		virtual void SetFunctionFlag(TexmapFunction function, bool bThreadSafe) = 0;

		/** Returns the specific Texmap function with thread safe flag.
		\param[in] function the specific Texmap function.
		\return thread safe flag. Default is false.
		*/
		virtual bool GetFunctionFlag(TexmapFunction function) = 0;

		/** Specifies the function of the texmap with strict thread safe flag.
		It requires the specific Texmap function should be thread safe with one instance of Texmap. 
		It's stricter than SetFunctionFlag with thread safe.
		\param[in] function Texmap function.
		\param[in] bThreadSafe whether the specific function is thread safe.
		*/
		virtual void SetFunctionStrictFlag(TexmapFunction function, bool bThreadSafe) = 0;
		/** Returns the specific Texmap function with thread safe flag.
		\param[in] function the specific Texmap function.
		\return thread safe flag. Default is false.
		*/
		virtual bool GetFunctionStrictFlag(TexmapFunction function) = 0;
	};
}