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

#include "../IRefObject.h"
#include "FragmentGraphBase.h"
#include "IVariableDictionary.h"

namespace MaxSDK { namespace Graphics {
	
	/** built-in variable types. For more details please see class Variable.
	*/
	enum VarType
	{
		/** A single char type. 
		*/
		VT_Char, 
		/** unsigned char type.
		*/
		VT_UnsignedChar, 
		/** wchar_t type.
		*/
		VT_WideChar, 
		/** short type.
		*/
		VT_Short, 
		/** unsigned short type.
		*/
		VT_UnsignedShort, 
		/** int type.
		*/
		VT_Int, 
		/** unsigned int type.
		*/
		VT_UnsignedInt, 
		/** long type.
		*/
		VT_Long, 
		/** unsigned long type.
		*/
		VT_UnsignedLong, 
		/** __int64 type.
		*/
		VT_Int64, 
		/** unsigned __int64 type.
		*/
		VT_UnsignedInt64, 
		/** float type.
		*/
		VT_Float, 
		/** double type.
		*/
		VT_Double, 
		/** Ascii string type.
		*/
		VT_StringA, 
		/** Unicode string type.
		*/
		VT_StringW, 
		/** dictionary type. If a variable is this type, it is actually a map. 
		the key is a multi-byte string, while the value is another variable.
		*/
		VT_Dictionary, 
		/** A IRefObject type. The variable will hold a reference of that object.
		*/
		VT_IRefObject, 
	};
	
	/** Check if the given type is number type.
	\param[in] type type value.
	\return true if the type is number type, e.g. char, wide char, int, double. 
	false otherwise. 
	*/
	FragmentGraphAPI bool IsNumberType(VarType type);

	/** Check if two types can be converted to each other. 
	\param[in] lhs The first type.
	\param[in] rhs The second type.
	\return true if two types are equal or both are numbers, false otherwise.
	*/
	FragmentGraphAPI bool IsCompatibleType(VarType lhs, VarType rhs);

	/** Variable is a simple class that stores certain data that developer can 
	decide at runtime. We use this class to transfer parameter from fragment to 
	fragment.
	*/
	class Variable : public MaxHeapOperators
	{
	public:
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifndef DISABLE_UNIT_TESTS
		// for unit test only
		static size_t GetNetStringCount();
#endif
#endif
		/** Default constructor. The default type of the variable will be int and 
		the default value is zero.
		*/
		FragmentGraphAPI Variable();
		/** Construct a char-type variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(char iValue);
		/** Construct a unsigned char variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned char iValue);
		/** Construct a wchar_t variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(wchar_t iValue);
		/** Construct a short variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(short iValue);
		/** Construct a unsigned short variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned short iValue);
		/** Construct a int variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(int iValue);
		/** Construct a unsigned int variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned int iValue);
		/** Construct a long variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(long iValue);
		/** Construct a unsigned long variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned long iValue);
		/** Construct a __int64 variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(__int64 iValue);
		/** Construct a unsigned __int64 variable with the given value.
		\param[in] iValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(unsigned __int64 iValue);
		/** Construct a float variable with the given value.
		\param[in] fValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(float fValue);
		/** Construct a double variable with the given value.
		\param[in] fValue The initial value of this variable.
		*/
		FragmentGraphAPI Variable(double fValue);
		/** Construct a multi-byte string variable with the given value.
		\param[in] str The initial value of this variable. Cannot be NULL.
		*/
		FragmentGraphAPI Variable(const char* str);
		/** Construct an unicode string variable with the given value.
		\param[in] str The initial value of this variable. Cannot be NULL.
		*/
		FragmentGraphAPI Variable(const wchar_t* str);
		/** Construct a IRefObject auto-ptr variable with the given value.
		\param[in] pRefObject The initial value of this variable. Can be NULL.
		*/
		FragmentGraphAPI Variable(MaxSDK::Graphics::IRefObject* pRefObject);
		/** Copy constructor.
		\param[in] from The default value of this variable. 
		*/
		FragmentGraphAPI Variable(const Variable& from);
		/** Destructor.
		*/
		FragmentGraphAPI ~Variable();

		/** Change this variable to certain type, and reset the value. 
		For numbers, reset to zero.
		For string, reset to empty string.
		For RefObject, reset to NULL.
		\param[in] type The type that this variable will be changed to.
		*/
		FragmentGraphAPI void ChangeType(VarType type);

		/** Get the type of this variable.
		\return the type of this variable.
		*/
		VarType GetType() const
		{
			return mType;
		}

	public:
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return A char value.
		*/
		FragmentGraphAPI char AsChar() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return An unsigned char value.
		*/
		FragmentGraphAPI unsigned char AsUnsignedChar() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return A wchar_t value.
		*/
		FragmentGraphAPI wchar_t AsWideChar() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return A short value.
		*/
		FragmentGraphAPI short AsShort() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return An unsigned short value.
		*/
		FragmentGraphAPI unsigned short AsUnsignedShort() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return An int value.
		*/
		FragmentGraphAPI int AsInt() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return An unsigned int value.
		*/
		FragmentGraphAPI unsigned int AsUnsignedInt() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return A long value.
		*/
		FragmentGraphAPI long AsLong() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return An unsigned long value.
		*/
		FragmentGraphAPI unsigned long AsUnsignedLong() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return An __int64 value.
		*/
		FragmentGraphAPI __int64 AsInt64() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return An unsigned __int64 value.
		*/
		FragmentGraphAPI unsigned __int64 AsUnsignedInt64() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return A float value.
		*/
		FragmentGraphAPI float AsFloat() const;
		/** Try to convert this variable to char and return the value.
		If this variable is not a number, an exception will be thrown out.
		\return A double value.
		*/
		FragmentGraphAPI double AsDouble() const;
		/** Try to get the multi-byte string value of this variable. If this 
		variable is not a multi-byte string, an exception will be thrown out.
		\return multi-byte string pointer. It's life time is controlled by the 
		variable, developer don't need to delete it.
		*/
		FragmentGraphAPI const char* AsStringA() const;
		/** Try to get the unicode string value of this variable. If this 
		variable is not a unicode string, an exception will be thrown out.
		\return unicode string pointer. It's life time is controlled by the 
		variable, developer don't need to delete it.
		*/
		FragmentGraphAPI const wchar_t* AsStringW() const;
		/** Try to get the IRefObject pointer that referenced by this variable. 
		If this variable is not an IRefObject variable, an exception will be 
		thrown out.
		\return This function directly returns the pointer to the IRefObject. It 
		doesn't increase the reference count of that IRefObject.
		*/
		FragmentGraphAPI MaxSDK::Graphics::IRefObject* AsRefObject() const;
		/** Try to get the dictionary pointer that referenced by this variable. 
		If this variable is not a dictionary variable, an exception will be 
		thrown out.
		\return This function directly returns the pointer to the dictionary. It's 
		life time is controlled by the variable, developer don't need to delete it.
		*/
		FragmentGraphAPI IVariableDictionary* AsDictionary() const;

		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(char iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned char iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(wchar_t iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(short iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned short iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(int iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned int iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(long iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned long iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(__int64 iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(unsigned __int64 iValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] fValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(float fValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] fValue input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(double fValue);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] str input value.
		\return true if the variable is a multi-byte string and is successfully 
		updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(const char* str);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] str input value.
		\return true if the variable is an unicode string and is successfully 
		updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(const wchar_t* str);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] iValue input value.
		\return true if the variable is a IRefObject pointer and is successfully 
		updated, false otherwise. If succeeded, this function will try to release 
		the old IRefObject that referenced by the variable and increase the reference 
		count of the input value.
		*/
		FragmentGraphAPI bool TryUpdateValue(MaxSDK::Graphics::IRefObject* pRefObject);
		/** Try to update this variable's value, without changing this variable's type.
		If the input value cannot be converted to this variable's type, return false.
		\param[in] var input value.
		\return true if the variable is successfully updated, false otherwise. 
		*/
		FragmentGraphAPI bool TryUpdateValue(const Variable& var);

		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (char rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned char rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (wchar_t rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (short rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned short rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (int rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned int rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (long rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned long rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (__int64 rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (unsigned __int64 rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (float rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (double rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (const char* rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (const wchar_t* rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] rhs input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (MaxSDK::Graphics::IRefObject* rhs);
		/** convert this variable to the input value type. Then assign 
		the input value to this variable.
		\param[in] var input value.
		\return the reference to the variable itself.
		*/
		FragmentGraphAPI Variable& operator = (const Variable& var);

	private:

		void Release();

		void CleanMembers(VarType type)
		{
			mType = type;
			mValue.mUnsignedInt64 = 0;
		}

		VarType mType;
		union ValueStorage
		{
			char mChar;
			unsigned char mUnsignedChar;
			wchar_t mWideChar;
			short mShort;
			unsigned mUnsignedShort;
			int mInt;
			unsigned int mUnsignedInt;
			long mLong;
			unsigned long mUnsignedLong;
			__int64 mInt64;
			unsigned __int64 mUnsignedInt64;
			float mFloat;
			double mDouble;
			char* mpStringA;
			wchar_t* mpStringW;
			MaxSDK::Graphics::IRefObject* mpRefObject;
			IVariableDictionary* mpDictionary;
		};
		ValueStorage mValue;
	};

} } // namespace MaxSDK::Graphics
