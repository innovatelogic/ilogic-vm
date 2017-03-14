#ifndef __actionbase_h__
#define __actionbase_h__

#ifdef WIN32
#pragma once
#endif

#include "platform_specific.h"
#include "ObjectAbstract.h"

enum ELinkDirection
{
	ELink_In  = 0,
	ELink_Out = 1,
};

struct SocketAction
{
	SocketAction(class ActionBase * owner, ELinkDirection dir);
	~SocketAction();

	void	Render(HDC hdc, const Matrix &ViewMatrix) const;
	bool	ProcessMouseClick(const Vector2f& MousePos, bool link = false) const;

	class ActionBase * Owner;
	struct SocketAction * LinkSocket;
	ELinkDirection	Dir;
};


class CORESDK_API ActionBase : public CObjectAbstract
{
	DECLARE_CLASS_SIMPLE(ActionBase, CObjectAbstract);

	typedef std::vector<class ActionBase*>			TVecActionChild;
	typedef TVecActionChild::iterator				TVecActionChildIterator;
	typedef TVecActionChild::const_iterator			TVecActionChildConstIterator;
	typedef TVecActionChild::reverse_iterator		TVecActionChildRevIterator;

	template<class T> class EventAbstract;

	typedef std::vector<EventAbstract<ActionBase>*> TVecEvents;
	typedef TVecEvents::iterator					TVecEventsIterator;
	typedef TVecEvents::const_iterator				TVecConstEventsIterator;

	typedef std::vector<struct SocketAction*>		TVecSockets;
	typedef TVecSockets::iterator					TVecSocketsIterator;
	typedef TVecSockets::const_iterator				TVecSocketsConstIterator;
	typedef TVecSockets::reverse_iterator			TVecSocketsRevIterator;

public:
	ActionBase(const CObjectAbstract * Parent);
	ActionBase(const ActionBase & Source);
	virtual ~ActionBase();

	/**
	* Serialize/Deserialize from XML file
	* Save object hierarchy in to file with all object properties.
	*
	* @param [in] Tree.
	*/
	virtual void	SuperDeserializer(tinyxml2::XMLElement *pTree);
	virtual void	SuperSerializer(std::ofstream &stream, bool bSaveAsExternal = false);
	virtual void	SuperSerializer(std::stringstream &stream, bool bSaveAsExternal = false);

	/**
	* Serialization option
	* Assemble linkage string value.
	*
	* @return assembled line.
	*/
	virtual std::string	GetLinkageString();

	/** 
	 * Release and dealloc memory.
	 * DO NOT USE OBJECT AFTER THIS CALL 
	 * 
	 * @param [in] ForceOperation true release All objects not only pending to delete.
	 */
	virtual bool	ProcessRelease(bool ForceOperation = false);

	/** 
	 *  Initialization function. calls once per object create
	 *	ATTENTION! Do not call it twice 
	 */
	virtual void	Initialize();

	/** 
	 * Append new child object to parent. Or optionally place it in specific position.
	 *
	 * @param [in] actor - pointer to object.
	 * @param [in] Index - optional place for object in array. default value INDEX_NONE means object will be appended to the tail of array.
	 */
	virtual bool	AddChildNode(const ActionBase * actor, int Index = INDEX_NONE);

	/**
	 * Returns Root Node Actor. 
	 * Try to avoid call this function in time critical sections.
	 *
	 * @return root node object
	 */
	ActionBase*		GetRootNode();

	/** 
	 * @return parent object or NULL if object is root node.
	 */
	ActionBase*		GetParent() const { return ParentAction; }

	/** 
	 * Set's the new Parent object for object.
	 * WARNING function does not check any possible collisions and set it immediately.
	 *
	 * @param [in] New pointer to parent object. 
	 */
	void			SetParent(const ActionBase * Parent);
	void			Execute(class CActor * EventActor);

	// @TODO: Redesign
	virtual void	Render(HDC hdc, const Matrix &ViewMatrix);

	inline_ void			SetAppMain(class CCoreSDK * App) { AppMain = App; }
	inline_ class CCoreSDK* GetAppMain() const { return AppMain; }

	void			SetOwnerActor(class CActor * Owner) { OwnerActor = Owner; }
	CActor*			GetOwnerActor() const { return OwnerActor; }

	void			SetPosition2f(Vector2f &pos) { Position = pos; }
	Vector2f		GetPosition2f() const { return Position; }

	void			SetSize(const Vector2f& size) { Size = size; }
	Vector2f		GetSize() const { return Size; }

	virtual const ActionBase*		ProcessMouseClick(const Vector2f& MousePos) const;
	virtual struct SocketAction*	ProcessMouseClickSocket(const Vector2f& MousePos, bool link = false) const;

protected:
	virtual void	DoExecute(class CActor * EventActor) {}
	
	virtual void	RenderCaption(HDC hdc);
	virtual void	RenderFuncZone(HDC hdc);

	bool			RegisterSocket(struct SocketAction *Socket);
	bool			UnRegisterSocket(struct SocketAction *Socket);

private:
public:
	class CActor	*OwnerActor;

	ActionBase		*ParentAction;
	TVecActionChild	 ChildNodes;

	ActionBase		*ActionIn;
	TVecActionChild	 ActionsOut;

	bool			bPlayOnce;
	unsigned int	Count;

	EventAbstract<ActionBase>*	EventIn;
	TVecEvents					EventsOut;

	TVecSockets		VecSockets;

protected:
	Vector2f		Position;
	Vector2f		Size;

private:
	class CCoreSDK * AppMain;
};

#endif//__actionbase_h__