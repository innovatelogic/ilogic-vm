#ifndef __actor_h__
#define __actor_h__

#ifdef _WIN32
#pragma once
#endif

#include "ObjectAbstract.h"

class EXPORT CActor : public CObjectAbstract
{
	DECLARE_CLASS_SIMPLE(CActor, CObjectAbstract);

public:
	/**
	 * Constructor / Destructor
	 */
    CActor(const CObjectAbstract *pParent);
	CActor(const CActor &Source);
    virtual ~CActor();

	virtual bool OnPrePropertyChange(const char *PropertyName);
	virtual void OnPropertyChanged(const char *PropertyName);
	
	/**
	* Serialize/Deserialize from XML file
	* Save object hierarchy in to file with all object properties.
	*
	* @param [in] Tree.
	*/
	virtual void			SuperDeserializer(tinyxml2::XMLElement *pTree);
	virtual void			SuperSerializer(std::ofstream &stream, bool bSaveAsExternal = false);
	virtual void			SuperSerializer(std::stringstream &stream, bool bSaveAsExternal = false);
	/** 
	 * Calls every time to finalize object generation.
	 */
	virtual void			FinishGeneration();

	/** 
	 *  Initialization function. calls once per object create
	 *	ATTENTION! Do not call it twice 
	 */
	virtual void			Initialize();

	/** 
	* Post deserialization's linkage object by indexes.
	*/
	virtual void			PostLink();

	/** 
	 * Reset initialize flag.
	 * 
	 */
	virtual void			ResetInitialize();

	/**
	 * Load objects-tree from file. Use SuperDeserializer method.
	 *
	 * @param [in] filename.
	 */
	virtual bool			LoadFromFile(const char *filename, bool bLoadAsExternal = false);

	/**
	* Save objects-tree from file. Use SuperSerializer method.
	*
	* @param [in] filename.
	*/
	bool					SaveToFile(const char *filename, bool bSaveAsExternal = false);

	/**
	* Save objects-tree from file to stream. Use SuperSerializer method.
	*
	* @param [in] stream.
	*/
	bool					SaveToBuffer(std::stringstream &stream, bool bSaveAsExternal = false);

	/** 
	 * Rename object. Name not necessarily should be unique.
	 *
	 * @param [in] name - new alias for object.
	 */
	bool					Rename(const char *name);
	
	/**  
	 * Copy buffer operations interface
	 * 
	 * @param [in] Source - object to copy.
	 */
	virtual void			Copy(CActor * Source) {}

	/**  
	 * Copy buffer operations interface
	 * 
	 * @param [in] Parent - object to paste in.
	 */
	virtual void			Paste(CActor * Parent = nullptr) {}

	/** child-node management */
	/**
	 * Returns Root Node Actor. 
	 * Try to avoid call this function in time critical sections.
	 *
	 * @return root node object
	 */
	CActor*					GetRootNode();

	/** 
	 * @return parent object or NULL if object is root node.
	 */
	CActor*					GetParent() const { return m_pParentActor; }

	/** 
	 * Set's the new Parent object for object.
	 * WARNING function does not check any possible collisions and set it immediately.
	 *
	 * @param [in] New pointer to parent object. 
	 */
	void					SetParent(const CActor *pParent);

	/** 
	 * Append new child object to parent. Or optionally place it in specific position.
	 *
	 * @param [in] actor - pointer to object.
	 * @param [in] Index - optional place for object in array. default value INDEX_NONE means object will be appended to the tail of array.
	 */
	virtual bool			AddChildNode(const CActor *actor, int Index = INDEX_NONE);

	/** 
	 * Mark object pending to delete.
	 * 
	 * ATTENTION: Do not release object immediately. Object will be released in next GC call.
	 * DO NOT USE OBJECT AFTER THIS CALL 
	 */
	virtual void			Release();

	/** 
	 * Mark child pending to delete.
	 * DO NOT USE CHILD OBJECT AFTER THIS CALL 
	 * 
	 * @param [in] bSkipTransient True to skip transient child, False release All objects.
	 */
	bool					ReleaseChilds(bool bSkipTransient = false);

	/** 
	 * Call this function to know does it need to process GC clean.
	 *
	 * @return true if some object need to release otherwise false.
	 */
	bool					IsNeedCollect();

	/** 
	 * Release and dealloc memory.
	 * DO NOT USE OBJECT AFTER THIS CALL 
	 * 
	 * @param [in] ForceOperation true release All objects not only pending to delete.
	 */
	virtual bool			ProcessRelease(bool ForceOperation = false);

	/**
	 * Pre release object's call.
	 * Calls prior memory deallocation.
	 *
	 * Suggestion: signal to unregister object will be delete soon
	 */
	virtual void			PreRelease();

	/** 
	 * subscribers management.
	 * Adds subscriber object to current.
	 *
	 * @param [in] Subscriber object to add.
	 */
	bool					AddSubscriber(CActor * Subscriber);

	/** removes from child list. not delete */
	bool					RemoveChildNode(CActor * actor);
	
	/** helper function iterate all actor's tree with invoke callback call */
	void					ForeachNodes(int (*CALLBACK_ActorInvoke) (const CActor* Source)) const;
	void					ForeachTreeNodes(int (*CALLBACK_ActorInvoke) (const CActor* Source)) const;
	
	/** apply all pending modificators */
	void					ApplyModificators();

	/** modificator interface. redefine for object modificators */
	virtual void			ApplyModificator(const CActor * Sender) {}
	
	void					AddKeyBinding(KeyCode Key);
	bool					IsKeyBind(KeyCode Key);
	
	/**	Find actor by name. */
	CActor*					FindActor(const char *Name, bool bIterate = true) const;
	CActor*					FindActorByPath(const std::string &Path);

	/** return const array of childs by selected class type*/
	void					GetObjectsByClass(std::vector<const CActor*> &OutArray, std::string CLASS_TYPE = "", bool bIterate = false) const;
	
	/** return secified iterator for object*/
	TVecActorChildIterator	GetChildIteratorNext(const TVecActorChildIterator& Iterator, std::string CLASS_TYPE = "");
	TVecActorChildIterator	GetChildIteratorPrev(const TVecActorChildIterator& Iterator, std::string CLASS_TYPE = "");

	inline_ virtual bool	HitTest(const Vector2f& Position) { return false; }

	/** 
	 *	Enabled and disable object stay visible. Not enabled object's disable from input.
	 *
	 *	@return true if object enabled otherwise false.
	 */
	bool					GetEnabled() const { return bEnabled; }

	/** 
	 *	Enable or disable object. object stay visible but disables for input.
	 *
	 *	@param true for enable object false to disable.
	 */

	virtual void			SetEnabled(bool enable) { bEnabled = enable; }

	/** non movable static object */
	virtual void			SetLocked(bool locked) { Locked = locked; }
	virtual bool			GetLocked() { return Locked; }

	/** Editor functions */
	virtual bool			UIEditor_ProcessDelete() { return false; }

	void					BroadcastEvent(ESystemEventID Id);
	void					BroadcastEvent(ESystemEventID Id, CActor *pParam);

	// [Editor]
	/**
		editor specific state Unlocked Locked
		specified for object Drug&Drop handling.
	*/
	void				SetControlState(EActorState state, bool IsIterative = false, bool bIterateUp = true);
	EActorState			GetControlState() const { return ActorState; }

	virtual bool		DoEventPressed(const MouseInputData &InputData);
	virtual bool		DoEventReleased(const MouseInputData &InputData);
	virtual bool		DoEventMove(const MouseMoveInputData &InputData);

	/**
	* Update all registered entities object's World position was changed
	*/	
	void				UpdateEntitiesChangeWorldPos(const Matrix &World);

	/**
	* [Editor] specific movement type.
	*/
	EScrObjectEvent		GetControlMode() const { return ControlEvent; }
	bool				SetControlMode(EScrObjectEvent Mode) { ControlEvent = Mode; return true; }

	// UNOPTIMAL
	int					GetDepth();

	static bool			FindCommonParent(CActor **pOutParent, const CActor *pObjectA, const CActor *pObjectB);
	static bool			GetRelativePath(std::string &Path, const CActor *pSourceObject, const CActor *pObjectB);
	
	/** resolve unique name based by user-preffered name*/
	static std::string	ResolveName(std::string &PrefferName, const CActor *pInPlaceObject);

	static bool			MoveObjectHierarchy(CActor *pActorMove, bool Up = true, bool bMoveEditorVisible = false);
	static bool			MoveObjectHierarchyBound(CActor *pActorMove, bool Up = true);

	/** 
	 * Tests if child object directly nested from parent
	 *
	 * @param [in] pointer to parent object. 
	 * @param [in] pointer to child object. 
	 *
	 * @return true if object nested.
	 */
	static bool			IsChildOf(const CActor *pParent, const CActor *pChild);
	static bool			IsChildOf(const CActor *pParent1, const CActor *pParent2, const CActor *pChild);

private:
	template <class T> void SerializeImpl(T &stream, bool bSaveAsExternal = false)
	{
		T StreamInternal;

		// template method to save some specific staff
		bool bSaveExt = SuperSerializerInternal(StreamInternal);

		const bool bContainChilds = !m_ChildNodes.empty() || bSaveExt;

		std::string OutValue;

		GetValueString(OutValue, bSaveAsExternal);

		stream << "<" << GetType() << OutValue << (bContainChilds ? ">" : "/>");

		if (bSaveExt){
			//stream << StreamInternal;
		}

		if (!GetExternal() || bSaveAsExternal)
		{
			TVecActorChildIterator IterActor = m_ChildNodes.begin();

			while (IterActor != m_ChildNodes.end())
			{
				(*IterActor)->SerializeImpl<T>(stream);
				++IterActor;
			}
		}

		if (bContainChilds){
			stream << "</" << GetType() << ">";
		}
	}

protected:
	/** controller event */
	EScrObjectEvent		ControlEvent;

public:
	/** tree interface */
	CActor			   *m_pParentActor;

	/** Child nodes array
		Convention: first N values reserved for non movable transient objects */
	TVecActorChild		m_ChildNodes;

private:
	/** control state */
	mutable bool		Locked;
	
	/** enable state flag */
	bool				bEnabled;

	mutable EActorState	ActorState;

public:
	/** user event delegates*/
	TVecEvents			m_VEvents;

	/** key bindings */
	TVecInterruptKeys	InterruptKeys; 

	/** helper flag to simplifly tree navigation*/
	bool				bMarkerDeleteFlag;
};

#endif //__actor_h__