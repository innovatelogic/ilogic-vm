#ifndef __triggerobject_h__
#define __triggerobject_h__

#ifdef WIN32
#pragma once
#endif

#include "ActorAllocator.h"

class CORESDK_API TriggerObject : public ActorAllocator
{
	DECLARE_CLASS_SIMPLE(TriggerObject, ActorAllocator);
public:
	// Constructor/Destructor
	TriggerObject(const CObjectAbstract * Parent);
	TriggerObject(const TriggerObject &Source);
	virtual ~TriggerObject();

	virtual void	OnPropertyChanged(const char* PropertyName);

	/**
	* Serialize/Deserialize from XML file
	* Save object hierarchy in to file with all object properties.
	*
	* @param [in] Tree.
	*/
	virtual void	SuperDeserializer(TiXmlElement *xml_current_tree);
	virtual void	SuperSerializer(std::ofstream &Stream, bool bSaveAsExternal = false);

	virtual void	Initialize();

	virtual void	DoDraw();

	virtual void	OnChangePivot(const CActor * Sender, ESystemEventID EventId);

	// properties
	Vector	GetExtends() const { return Extends; }
	void	SetExtends(Vector &Ext) { Extends = Ext; }

	bool	Get_OnEnter() { return OnEnter; }
	void	Set_OnEnter(bool flag) { OnEnter = flag; }

	bool	Get_OnStay() { return OnStay; }
	void	Set_OnStay(bool flag) { OnStay = flag; }

	bool	Get_OnLeave() { return OnLeave; }
	void	Set_OnLeave(bool flag) { OnLeave = flag; }

	void	OnTriggerEvent(CActor* OtherActor, TriggerShapeFlag Flag);

protected:
	void	CreatePhysActor();

private:
	Vector	Extends;

	bool	OnEnter;
	bool	OnStay;
	bool	OnLeave;

	class ActionTrigger *ActionEvent;

// 	class ActionBase	* ActionOnEnter;
// 	class ActionBase	* ActionOnStay;
// 	class ActionBase	* ActionOnLeave;
};

#endif//__triggerobject_h__