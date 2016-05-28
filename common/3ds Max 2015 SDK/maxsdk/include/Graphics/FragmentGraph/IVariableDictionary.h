//
// Copyright 2013 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//
//

#pragma once

#include "../../Noncopyable.h"
#include "../../strclass.h"

namespace MaxSDK { namespace Graphics {

	class Variable;

	/** IVariableDictionary is a map structure that contains the key/variable pairs. 
	We use this class to store some built-in parameters for fragment or fragment graph.
	Every built-in parameter is given with one name as the key.
	*/
	class IVariableDictionary : public MaxSDK::Util::Noncopyable
	{
	public:
		/** Default constructor.
		*/
		IVariableDictionary(){};
		/** Destructor.
		*/
		virtual ~IVariableDictionary(){};
		/** Check if the dictionary has the given key. 
		\param[in] key the key to check.
		\return true if this variable is a dictionary and has that key, false otherwise.
		*/
		virtual bool HasKey(const MSTR& key) const = 0;
		/** Try to find the value of the given key.
		\param[in] key the key to check.
		\return the variable that associated with that key. If failed to find such 
		a variable, returns NULL.
		*/
		virtual Variable* Find(const MSTR& key) = 0;
		/** Try to find the value of the given key.
		\param[in] key the key to check.
		\return the variable that associated with that key. If failed to find such 
		a variable, returns NULL.
		*/
		virtual const Variable* Find(const MSTR& key) const = 0;
		/** Try to find or create a key-variable pair, and returns the reference to the 
		given key.
		\param[in] key the key.
		\return reference to the variable that associated with that key. If failed 
		to find such a variable, simply create the key-variable association and 
		returns the newly created variable.
		*/
		virtual Variable& operator[](const MSTR& key) = 0;
		/** Try to find or create a key-variable pair, and returns the reference to the 
		given key.
		\param[in] key the key.
		\return reference to the variable that associated with that key. If failed 
		to find such a variable, simply create the key-variable association and 
		returns the newly created variable.
		*/
		virtual const Variable& operator[](const MSTR& key) const = 0;
		/** Remove one key. 
		\param[in] key the name of one key.
		\return true if remove successfully, false otherwise.
		*/
		virtual bool RemoveKey(const MSTR& key) = 0;

		/** Clear all key/variable pairs. 
		*/
		virtual void Clear() = 0;

		/** Get the count of all keys. 
		\return the count of all keys, 0 if no key.
		*/
		virtual size_t Size() const = 0;

		/** Get one key by using the index
		\param[in] index the index of this key in the container.
		\return one key,empty string if no index match.
		*/
		virtual const MSTR& GetKey(size_t index) const = 0;

		/** Get one variable by using the index
		\param[in] index the index of this variable in the container.
		\return one variable, one int type variable if no index match.
		*/
		virtual const Variable& GetVariable(size_t index) const = 0;
	};

} } // namespace MaxSDK::Graphics
