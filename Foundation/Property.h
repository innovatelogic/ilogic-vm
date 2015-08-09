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

#ifndef __property_h__
#define __property_h__

#ifdef _WIN32
#pragma once
#endif

#include "StdAfxFoundation.h"

#define NO_READ_WRITE 0
#define READ_ONLY	  1
#define WRITE_ONLY	  2
#define READ_WRITE	  3

#define CTRL_NONE			0
#define CTRL_EDIT			1
#define CTRL_EDIT_RESOURCE	2
#define CTRL_COMBO			3
#define CTRL_SLIDER			4
#define CTRL_COLOR			5
#define CTRL_PROGRESS		6
#define CTRL_ARRAY			7
#define CTRL_VECTOR			8
#define CTRL_VECTOR2f		9
#define CTRL_VECTOR4f		10
#define CTRL_MATRIX9		11
#define CTRL_MATRIX16		12

#define SERIALIZABLE		true
#define NON_SERIALIZABLE	false

#define COMMON_PROP			true
#define NON_COMMON_PROP		false

#define EXT_PROP			true
#define INT_PROP			false

//----------------------------------------------------------------------------------------------
struct StructDataMapping 
{
	std::string Name;
	INT			SubOffset;
};

//----------------------------------------------------------------------------------------------
struct EXPORT SInterfaceDecl
{
	const char	*strType;
	unsigned int byteShift;

	SInterfaceDecl(const char *type, unsigned int shift)
		: strType(type)
		, byteShift(shift)
	{}
};

//----------------------------------------------------------------------------------------------
class EXPORT IPropertiesAllocator
{
public:
	virtual const Property_Base** GetProps() const = 0;
	virtual unsigned int GetSize() const = 0;

	virtual const SInterfaceDecl** GetInterfaces() const = 0;
	virtual unsigned int GetIntfSize() const = 0;
};

//----------------------------------------------------------------------------------------------
class EXPORT Property_Base
{
public:
	Property_Base(const char *name,
				  int offset,
				  const char *classname,
				  const char *group,
				  int policy = READ_WRITE,
				  int ctrl = CTRL_EDIT,
				  bool serializable = NON_SERIALIZABLE,
				  bool bCommon = NON_COMMON_PROP,
				  bool bExtProp = INT_PROP,
				  Property_Base *prev = 0,
				  Property_Base *next = 0);

	virtual ~Property_Base() {}

	/**
	 * @return property's name.
	 */
	virtual const char* GetName() const { return m_Name.c_str(); }
	
	/**
	 * @return property's class group name.
	 */
	virtual const char* GetClassName() const { return m_ClassName.c_str(); }

	/**
	 * @return property's policy: NO_READ_WRITE, READ_ONLY, WRITE_ONLY, READ_WRITE etc..
	 */
	virtual int			GetPolicy() const { return m_Policy; }

	/**
	 * @return UI related data etc..
	 */
	virtual int			GetCtrl() const { return m_Ctrl; }

	virtual bool		IsSerializable() const { return m_bSerializable; }
	virtual bool		IsCommonValue() const { return m_bCommonValue; }

	/**
	 * data set
	 */
	virtual void		SetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false) { DoSetProperty(Ptr, Value, byteOffset, bSilent); }
	virtual char*		GetProperty(const void * Ptr, char * OutBuffer) const { return ""; }

	virtual void		DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false) { assert(false); }

	// return one property by default
	virtual size_t		GetSize() const { return m_ElementDim; }
	virtual std::string GetGroupName() const { return m_GroupName; }
	virtual size_t		GetElementOffset(size_t Index) const { return 0; }
	virtual char*		GetElementName(size_t Index) const { return ""; }

	void				SetMemOffset(INT Offset) { m_MemOffset = Offset; }

	void				SetElementSize(int Size) { m_ElementSize = Size; }
	int					GetElementSize() const { return m_ElementSize; }
	/**
	 * set previous property node.
	 * @prev pointer to previous property
	 */
	void				SetPrev(Property_Base *prev) { m_pNodePrev = prev; }

	/**
	 * @return previous property node.
	 */
	Property_Base*		GetPrev() const { return m_pNodePrev; }

	/**
	 * set next property node.
	 * @next pointer to next property
	 */
	void				SetNext(Property_Base *next) { m_pNodeNext = next; }

	/**
	 * @return next property node.
	 */
	Property_Base*		GetNext() const { return m_pNodeNext; }

	/**
	 * @return true if property external staff
	 */
	virtual bool IsExternalProp() const { return m_bExternalProp; }

	/**
	 * @return true if value exists
	 * pOut feels the null terminated string value
	 */
	virtual bool		GetDefaultValue(char *pOut) const = 0;

public:
	std::string			m_ClassName;
	std::string			m_GroupName;
	std::string			m_Name;
	bool				m_bSerializable;
	int					m_Policy;
	int					m_Ctrl;
	bool				m_bCommonValue;

	// external property unlike internal staff may store in external source file
	// internal property describes some object's internal state and should be stored inside
	// ex. objects transformation - external staff stores in base storage
	// color - stores in nested storage where base point to
	bool				m_bExternalProp;  
	int					m_ElementDim;
	int					m_ElementSize;

	//memory
	int					m_MemOffset;

	std::vector<std::string> PredefinedValues;

	class ClassNode		*m_ClassNodePtr;

protected:
	// double linked list for aggregated types
	Property_Base		*m_pNodePrev;
	Property_Base		*m_pNodeNext;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyImpl : public Property_Base
{
public:
	PropertyImpl(const char *name, int offset, const char *classname, const char *group,
		int policy = READ_WRITE, int ctrl = CTRL_EDIT, bool serializable = NON_SERIALIZABLE, bool bCommon = NON_COMMON_PROP, bool bExtProp = INT_PROP)
		: Property_Base(name, offset, classname, group, policy, ctrl, serializable, bCommon, bExtProp)
	{
	}

	virtual ~PropertyImpl() {}
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyINT : public Property_Base
{
public:
	PropertyINT(const char *name,
				int offset,	
				const char *classname,
				const char *group,
				int policy = READ_WRITE,
				int ctrl = CTRL_EDIT,
				bool serializable = NON_SERIALIZABLE,
				bool bCommon = NON_COMMON_PROP,
				bool bExtProp = INT_PROP,
				Property_Base *prev = 0,
				Property_Base *next = 0,
				const int *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset = 0, bool bSilent = false);
	
private:
	const int *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyUINT : public Property_Base
{
public:
	PropertyUINT(const char *name,
				 int offset,
				 const char *classname,
				 const char *group,
				 int policy = READ_WRITE,
				 int ctrl = CTRL_EDIT,
				 bool serializable = NON_SERIALIZABLE,
				 bool bCommon = NON_COMMON_PROP,
				 bool bExtProp = INT_PROP,
				 Property_Base *prev = 0,
				 Property_Base *next = 0,
				 const unsigned int *pDefaultValue = 0);

	void	SetValue(void *pDst, unsigned int Value);
	void	CopyValue(void *pDst, const void *pSrc);

	virtual char*	GetProperty(const void *Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char *pValue, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const unsigned int *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyFLOAT : public Property_Base
{
public:
	PropertyFLOAT(const char *name,
				  int offset,	
				  const char *classname,
				  const char *group,
				  int policy = READ_WRITE,
				  int ctrl = CTRL_EDIT,
				  bool serializable = NON_SERIALIZABLE,
				  bool bCommon = NON_COMMON_PROP,
				  bool bExtProp = INT_PROP,
				  Property_Base *prev = 0,
				  Property_Base *next = 0,
				  const float *pDefaultValue = 0);

	virtual char*	GetProperty(const void * Ptr, char * OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const float *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyBYTE : public Property_Base
{
public:
	PropertyBYTE(const char *name,
				 int offset,
				 const char *classname,
				 const char *group,
				 int policy = READ_WRITE,
				 int ctrl = CTRL_EDIT,
				 bool serializable = NON_SERIALIZABLE,
				 bool bCommon = NON_COMMON_PROP,
				 bool bExtProp = INT_PROP,
				 Property_Base *prev = 0,
				 Property_Base *next = 0,
				 const BYTE *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const BYTE *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyBOOL : public Property_Base
{
public:
	PropertyBOOL(const char *name, 
				 int offset,
				 const char *classname,
				 const char *group,
				 int policy = READ_WRITE,
				 int ctrl = CTRL_EDIT,
				 bool serializable = NON_SERIALIZABLE,
				 bool bCommon = NON_COMMON_PROP,
				 bool bExtProp = INT_PROP,
				 Property_Base *prev = 0,
				 Property_Base *next = 0,
				 const bool *pDefaultValue = 0);

	virtual char*	GetProperty(const void * Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const bool *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyString : public Property_Base
{
public:
	PropertyString(const char *name, 
				   int offset,
				   const char *classname,
				   const char *group,
				   int policy = READ_WRITE,
				   int ctrl = CTRL_EDIT,
				   bool serializable = NON_SERIALIZABLE,
				   bool bCommon = NON_COMMON_PROP,
				   bool bExtProp = INT_PROP,
				   Property_Base *prev = 0,
				   Property_Base *next = 0,
				   const char *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const char *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyStringW : public Property_Base
{
public:
	PropertyStringW(const char *name,
					int offset,
					const char *classname,
					const char *group,
					int policy = READ_WRITE,
					int ctrl = CTRL_EDIT,
					bool serializable = NON_SERIALIZABLE,
					bool bCommon = NON_COMMON_PROP,
					bool bExtProp = INT_PROP,
					Property_Base *prev = 0,
					Property_Base *next = 0,
					const wchar_t *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const wchar_t *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyVector2f : public Property_Base
{
public:
	PropertyVector2f(const char *name,
					 int offset,
					 const char *classname,
					 const char *group,
					 int policy = READ_WRITE,
					 int ctrl = CTRL_EDIT,
					 bool serializable = NON_SERIALIZABLE,
					 bool bCommon = NON_COMMON_PROP,
					 bool bExtProp = INT_PROP,
					 Property_Base *prev = 0,
					 Property_Base *next = 0,
					 const Vector2f *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const Vector2f *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyVector : public Property_Base
{
public:
	PropertyVector(const char *name,
				   int offset,
				   const char *classname,
				   const char *group,
				   int policy = READ_WRITE,
				   int ctrl = CTRL_EDIT,
				   bool serializable = NON_SERIALIZABLE,
				   bool bCommon = NON_COMMON_PROP,
				   bool bExtProp = INT_PROP,
				   Property_Base *prev = 0,
				   Property_Base *next = 0,
				   const Vector *pDefaultValue = 0);

	virtual char*	GetProperty(const void * Ptr, char * OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const Vector *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyVector4f : public Property_Base
{
public:
	PropertyVector4f(const char *name,
					 int offset,
					 const char *classname,
					 const char *group,
					 int policy = READ_WRITE,
					 int ctrl = CTRL_EDIT,
					 bool serializable = NON_SERIALIZABLE,
					 bool bCommon = NON_COMMON_PROP,
					 bool bExtProp = INT_PROP,
					 Property_Base *prev = 0,
					 Property_Base *next = 0,
					 const Vector4f *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const Vector4f *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyMatrix3x3 : public Property_Base
{
public:
	PropertyMatrix3x3(const char *name,
		int offset,	
		const char *classname,
		const char *group,
		int policy = READ_WRITE,
		int ctrl = CTRL_EDIT,
		bool serializable = NON_SERIALIZABLE,
		bool bCommon = NON_COMMON_PROP,
		bool bExtProp = INT_PROP,
		Property_Base *prev = 0,
		Property_Base *next = 0,
		const Matrix3f *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const Matrix3f *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyMatrix : public Property_Base
{
public:
	PropertyMatrix(const char *name,
				   int offset,	
				   const char *classname,
				   const char *group,
				   int policy = READ_WRITE,
				   int ctrl = CTRL_EDIT,
				   bool serializable = NON_SERIALIZABLE,
				   bool bCommon = NON_COMMON_PROP,
				   bool bExtProp = INT_PROP,
				   Property_Base *prev = 0,
				   Property_Base *next = 0,
				   const Matrix *pDefaultValue = 0);

	virtual char*	GetProperty(const void * Ptr, char * OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const Matrix *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyBounds3f : public Property_Base
{
public:
	PropertyBounds3f(const char *name,
					 int offset,
					 const char *classname,
					 const char *group,
					 int policy = READ_WRITE,
					 int ctrl = CTRL_EDIT,
					 bool serializable = NON_SERIALIZABLE,
					 bool bCommon = NON_COMMON_PROP,
					 bool bExtProp = INT_PROP,
					 Property_Base *prev = 0,
					 Property_Base *next = 0,
					 const Bounds3f *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const Bounds3f *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyPixelTransform : public Property_Base
{
public:
	PropertyPixelTransform(const char *name,
							int offset,
							const char *classname,
							const char *group,
							int policy = READ_WRITE,
							int ctrl = CTRL_EDIT,
							bool serializable = NON_SERIALIZABLE,
							bool bCommon = NON_COMMON_PROP,
							bool bExtProp = INT_PROP,
							Property_Base *prev = 0,
							Property_Base *next = 0,
							const int *pDefaultValue = 0);

	virtual char*	GetProperty(const void *Ptr, char *OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void *Ptr, const char *Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const int *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertySceneInputmode : public Property_Base
{
public:
	PropertySceneInputmode(const char *name, 
							int offset,					
							const char *classname,
							const char *group,
							int policy = READ_WRITE,
							int ctrl = CTRL_EDIT,
							bool serializable = NON_SERIALIZABLE,
							bool bCommon = NON_COMMON_PROP,
							bool bExtProp = INT_PROP,
							Property_Base *prev = 0,
							Property_Base *next = 0,
							const int *pDefaultValue = 0);

	virtual char*	GetProperty(const void * Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const int *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyEHandleInputKey : public Property_Base
{
public:
	PropertyEHandleInputKey(const char *name, 
		int offset,					
		const char *classname,
		const char *group,
		int policy = READ_WRITE,
		int ctrl = CTRL_EDIT,
		bool serializable = NON_SERIALIZABLE,
		bool bCommon = NON_COMMON_PROP,
		bool bExtProp = INT_PROP,
		Property_Base *prev = 0,
		Property_Base *next = 0,
		const int *pDefaultValue = 0);

	virtual char*	GetProperty(const void * Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const int *m_pDefaultValue;
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyEHandleInputMouse : public Property_Base
{
public:
	PropertyEHandleInputMouse(const char *name, 
		int offset,					
		const char *classname,
		const char *group,
		int policy = READ_WRITE,
		int ctrl = CTRL_EDIT,
		bool serializable = NON_SERIALIZABLE,
		bool bCommon = NON_COMMON_PROP,
		bool bExtProp = INT_PROP,
		Property_Base *prev = 0,
		Property_Base *next = 0,
		const int *pDefaultValue = 0);

	virtual char*	GetProperty(const void * Ptr, char *pOutBuffer) const;

	virtual bool	GetDefaultValue(char *pOutBuffer) const;

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

private:
	const int *m_pDefaultValue;
};
//----------------------------------------------------------------------------------------------
class EXPORT PropertyArrayStatic : public Property_Base
{
public:
	PropertyArrayStatic(const char *name, 
		int offset,					
		const char *classname,
		const char *group,
		Property_Base *source,
		unsigned int elementSize,
		unsigned int elementDim,
		int policy = READ_WRITE,
		int ctrl = CTRL_EDIT,
		bool serializable = NON_SERIALIZABLE,
		bool bCommon = NON_COMMON_PROP,
		bool bExtProp = INT_PROP,
		Property_Base *prev = 0,
		Property_Base *next = 0);

	virtual char*	GetProperty(const void * Ptr, char * OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOut) const { return false; }

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);
};

//----------------------------------------------------------------------------------------------
class EXPORT PropertyArray : public Property_Base
{
public:
	PropertyArray(const char *name,
					int offset,
					const char *classname,
					const char *group,
					int policy = READ_WRITE,
					int ctrl = CTRL_EDIT,
					bool serializable = NON_SERIALIZABLE,
					bool bCommon = NON_COMMON_PROP,
					bool bExtProp = INT_PROP,
					Property_Base *prev = 0,
					Property_Base *next = 0);

	virtual char*	GetProperty(const void * Ptr, char * OutBuffer) const;

	virtual bool	GetDefaultValue(char *pOut) const { return false; }

private:
	virtual void	DoSetProperty(const void * Ptr, const char* Value, unsigned int byteOffset = 0, bool bSilent = false);

public:
	std::vector<StructDataMapping> MapInfo;
};


//----------------------------------------------------------------------------------------------
class CommandImpl
{ 
public:
	virtual void ExecuteCommand() = 0;
};

//----------------------------------------------------------------------------------------------
template <class Container>
class PropertyCommand : public Property_Base, public CommandImpl
{
public:
	typedef boost::signal<void()> TEventCommand;

	PropertyCommand(Container * Container, const char *name, const char *classname, const char *group,
		int policy = INDEX_NONE, int ctrl = INDEX_NONE, bool serializable = NON_SERIALIZABLE , bool bCommon = NON_COMMON_PROP, bool bExtProp = INT_PROP)
		: Property_Base(name, 0, classname, group, policy, ctrl, serializable, bCommon, bExtProp)
		, m_Container(Container)
	{
	}

	virtual ~PropertyCommand()
	{
		EventCommand.disconnect_all_slots();
	}

	virtual void DoSetProperty(const std::string& Value) 
	{
		EventCommand();
	}

	virtual bool IsSerializable() { return false; }

	virtual void ExecuteCommand()
	{
		EventCommand();
	}

	Container * m_Container;
	TEventCommand EventCommand;
};

#endif//__property_h__