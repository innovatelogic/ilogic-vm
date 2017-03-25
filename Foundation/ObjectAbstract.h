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
#pragma once

#include "StdafxFoundation.h"
#include "EventDesc.h"
#include "property.h"
#include "ObjectFactory.h"
#include "IObjectAbstract.h"
#include "IEventManager.h"

namespace oes {
    namespace common_base {
        class IRenderInterface;
        class ICollisionInterface;
    }
}

class EXPORT CObjectAbstract : public oes::common_base::IObjectAbstract
{
	friend class ValueParser;

	DECLARE_CLASS_SIMPLE_NOBASE(CObjectAbstract);

public:
	/** 
	 * Constructor/Destructor
	 */
	CObjectAbstract(const CObjectAbstract *parent);
	virtual ~CObjectAbstract();
	
	/** 
	 * Calls every time to finalize object generation.
	 */
	virtual void	FinishGeneration();

	/**
	 * Test object generation finished.
	 *
	 * @return true if object fully generated
	 */
	virtual bool	IsGenerationFinished();

	/** 
	 * Calls from Property class to signalize that it have change.
	 * 
	 * @param PropertyName [in] current property name have change.
	 */
	virtual bool	OnPrePropertyChange(const char *PropertyName) { return true; }

	/*
	*  Property change handlers.
	*/
	virtual void	OnPropertyChanged(const char *PropertyName) {}

	/**
	* Serialize/Deserialize from XML file
	* Save object hierarchy in to file with all object properties.
	*
	* @param [in] Tree.
	*/
	virtual void	SuperDeserializer(tinyxml2::XMLElement *xml_current_tree);
	virtual void	SuperDeserializerExternal(tinyxml2::XMLElement *xml_current_tree);

	virtual void	SuperSerializer(std::ofstream &stream, bool bSaveAsExternal = false) {}
	virtual void	SuperSerializer(std::stringstream &stream, bool bSaveAsExternal = false) {}

	/** 
	 * Returns deserialization's string value
	 *
	 * @param [out] string value.
	 */
	std::string&	GetValueString(std::string &OutValue, bool bIgnoreExtPolicy = false) const;

	/**
	 * Every object calls Initialize function to finish generation.
	 * Warning: CALL THIS FUNCTION SECOND TIME WILL FAIL.
	 */
	virtual void	Initialize();

	/**
	* Set object the name.
	* 
	* @param name [in] new object's name.
	*/
	void			SetName(const char *name) override { m_Name = name; }

	/**
	 * @return object's name.
	 */
	const char*		GetName() const override { return m_Name.c_str(); }

	/** 
	 * Check object name equal with param value.
	 *
	 * @param Value [in] string value to test.
	 *
	 * @return true if name is equal or false in other case.
	 */
	bool			IsNameEqual(const char *value);

	/** 
	 * Set type information as a unique class name string.
	 *
	 * @param type [in] type name.
	 */
	void			SetType(const char * type) override { m_Type = type; }

	/**
	 * @return unique class name value.
	 */
	const char*		GetType() const override { return m_Type.c_str(); }

	/** 
	 * @return self class name.
	 */
	std::string		GetThisClassName() { return oes::rflex::GetThisClassName(m_TypeId); }

	/** 
	 * Set unique class type id.
	 * 
	 * @param type_id [in] class type unique type id.
	 */
	void			SetTypeId(size_t type_id) override { m_TypeId = type_id; }

	/** 
	 * @return unique class type id.
	 */
	size_t			GetTypeId() const { return m_TypeId; }

	/** user data management */
	void			SetUserData(void* user, unsigned int Index = INDEX_NONE);
	void*			GetUserData(unsigned int Index = INDEX_NONE) const;

	/** 
	 * Set pending to delete object's flag.
	 * Not release object immediately it will be release in next garbage collector call.
	 */
	virtual void	Release();

	/** 
	 * Pre release object's call.
	 * Calls prior memory deallocation.
	 */
	virtual void	PreRelease() {}
	
	/** 
	* Process pending to deleate object memory deallocation.
	* DO NOT USE OBJECT AFTER THIS CALL.
	*/
	virtual bool	ProcessRelease(bool ForceOperation = false);

	/**
	* Set listener to object
	*
	* @param [in] pointer to listener object.
	*/
	void	SetListener(class IListener* pListener) { m_pListener = pListener; }

	/**
	* Get listener
	*
	* @return listener pointer.
	*/
	class IListener*	GetListener() const { return m_pListener; }

    /*!
        returns session unique id 
    */
    long    GetUID() const override { return m_uID; }

    /** 
	 * @return true if object already initialized (flag m_bIsInitialize).
	 */
	bool			IsInitialized() const { return m_bIsInitialize; }

	/** 
	 * Pending to delete means object will be delete in next garbage collector call.
	 *
	 * @return true if object pending to delete otherwise false.
	 */
	bool			IsPendingToDelete() const { return m_bPendingToDelete; }

	/** 
	 * Reset initialize flag.
	 * @HACK Uses for reinitialize root object.
	 */
	virtual void	ResetInitialize() { m_bIsInitialize = false; }

	/** 
	 * Transient means object not creates inline as a functional child of Parent object
	 * but during file deserialization it takes it properties from file if it exists.
	 *
	 * @return true if object is transient, false otherwise.
	 */
	bool			IsTransient() const { return m_bTransient; }

	/** 
	 * Set object transient.
	 * WARNING Does it really need? should be better set during creation.
	 */
	void			SetTransient(bool flag) { m_bTransient = flag; }

	void			RegisterInterfaceImpl(class IBaseInterface *pInt);
	void			UnregisterInterfaceImpl(class IBaseInterface *pInt);

	bool			RegisterRenderEntity(IRenderInterface *pPtr);
	bool			UnregisterRenderEntity(IRenderInterface *pPtr);

	bool			RegisterCollisionEntity(ICollisionInterface *pPtr);
	bool			UnregisterCollisionEntity(ICollisionInterface *pPtr);

	void			SetPlainDeserializationPos(unsigned int Value) { m_nPlainDeserializationPos = Value; }
	unsigned int	GetPlainDeserializationPos() const { return m_nPlainDeserializationPos; }

	/** object's externality */
	void			SetExternal(bool Flag) { m_bExternal = Flag; }
	bool			GetExternal() const { return m_bExternal; }

	const char*		GetFilenameTag() const { return m_FilenameTag.c_str(); }
	void			SetFilenameTag(const char *filename) { m_FilenameTag = filename; }

	static void		DbgAllocCall();

    static void SetEventMgr(oes::foundation::IEventManager *mgr);

    static bool	CompByPlainPos(const CObjectAbstract * A1, const CObjectAbstract * A2)
    {
        return A1->m_nPlainDeserializationPos < A2->m_nPlainDeserializationPos;
    }

    static oes::rflex::TClassFactory* GetClassFactoryStatic()
    {
        DbgAllocCall();
        return oes::rflex::TGlobalClassFactory::GetInstance();
    }

protected:
        /**
        * Release object.
        */
        virtual void	DoRelease() {}

        /**
        * Internal child serializer / deserializer
        */
        virtual void	SuperDeserializerInternal(tinyxml2::XMLElement *pTree) {}
        virtual bool	SuperSerializerInternal(std::ofstream &stream) { return false; }
        virtual bool	SuperSerializerInternal(std::stringstream &stream) { return false; }

private:
	/** Resource name */
	std::string		m_Name;

	/** type info */
	size_t			m_TypeId;
	std::string		m_Type;

    /** unique id until process session */
    volatile long   m_uID;

    volatile LONG	m_bGenerationFinished;
	bool			m_bIsInitialize;
	bool			m_bPendingToDelete;

	/** true for non seriazable objects  */
	bool			m_bTransient;

	/** ordered position */
	unsigned int	m_nPlainDeserializationPos;

	/** 
	 * true if object is in external file
	 */
	bool				m_bExternal;

	/** filename */
	std::string			m_FilenameTag;

	/** user data fields */
	void				*m_pUserData;
	void				*m_pArrayUserData[3];

public:
	std::vector<class IBaseInterface*>		m_VecInterfacesImp;
	std::vector<class IRenderInterface*>	m_VecRenderEntities;
	std::vector<class ICollisionInterface*> m_VecCollisionEntities;

public:
	static int			DbgAllocNumber;

private:
	class IListener		*m_pListener;

protected:
    static oes::foundation::IEventManager *m_pEventManager;
};

//------------------------------------------------------------------------
//
//------------------------------------------------------------------------
extern "C"
{
#define NEW_OBJECT(OBJECT_CLASS, NAME, PARENT)\
	static_cast<OBJECT_CLASS*>(GENERATE_OBJECT_SDK(#OBJECT_CLASS, NAME, PARENT));

#define CLONE_OBJECT(OBJECT_CLASS, SOURCE, NAME, PARENT)\
	static_cast<OBJECT_CLASS*>(CLONE_OBJECT_SDK(SOURCE, NAME, PARENT));

#define NEW_OBJECT_CHILD(SOURCE_OBJECT, OBJECT_CLASS, NAME, PARENT)\
	SOURCE_OBJECT = NEW_OBJECT(OBJECT_CLASS, NAME, PARENT)\
	assert(SOURCE_OBJECT);\
	if(PARENT){\
		PARENT->AddChildNode(SOURCE_OBJECT);\
	}

#define NEW_OBJECT_TRANSIENT_CHILD(SOURCE_OBJECT, OBJECT_CLASS, NAME, PARENT)\
	NEW_OBJECT_CHILD(SOURCE_OBJECT, OBJECT_CLASS, NAME, PARENT);\
	SOURCE_OBJECT->SetTransient(true);

#define NEW_OBJECT_TRANSIENT(SOURCE_OBJECT, OBJECT_CLASS, NAME)\
	SOURCE_OBJECT = NEW_OBJECT(OBJECT_CLASS, NAME, nullptr)\
	assert(SOURCE_OBJECT);\
	SOURCE_OBJECT->SetTransient(true);

	EXTERN_EXPORT class CObjectAbstract* GENERATE_OBJECT_SDK(const char *Type, const char *Name, class CObjectAbstract *pParent = nullptr);
	EXTERN_EXPORT class CObjectAbstract* CLONE_OBJECT_SDK(const class CObjectAbstract * Source, const char *Name = nullptr, CObjectAbstract *pNewParent = nullptr);
	EXTERN_EXPORT class CObjectAbstract* SLICE_CLONE_OBJECT(const CObjectAbstract *pSource, const char *NewType, const char *Name = nullptr);
	
    EXTERN_EXPORT void* __cdecl	memory_new(size_t Size);
	EXTERN_EXPORT void	__cdecl memory_delete(void* Ptr);
}

//
// C++ style memory allocation.
//
#ifdef USE_MEMORY_POOL
// inline void* operator new(size_t Size){
// 	return memory_new(Size);
// }
// 
// inline void	operator delete(void* Ptr){
// 	memory_delete(Ptr);
// }
// 
// inline void* operator new[](size_t Size){
// 	return memory_new(Size);
// }
// 
// inline void	operator delete[](void* Ptr){
// 	memory_delete(Ptr);
// }
#endif//USE_MEMORY_POOL