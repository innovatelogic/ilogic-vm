/*==============================================================================
// Copyright (c) 1998-2008 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Reference Notifier
// AUTHOR: Michael Zyracki created 2008
//***************************************************************************/

#ifndef __NOTIFIER__H__
#define __NOTIFIER__H__

#include <max.h>
#include <Maxapi.h>

class NotifyCallback
{
public:
	virtual RefResult NotifyFunc(const Interval& changeInt,
            RefTargetHandle hTarget, PartID& partID,
            RefMessage message, BOOL propagate)=0;

#pragma warning( push )
#pragma warning( disable : 4481 )
private:
	// We want the NotifyCallback::NotifyFunc  method with the following signature to be sealed to force derived classes
	// that implement this method with this signature to modify their implementation to the new signature.
	//! \deprecated Deprecated method in terms of implementation as of 3ds Max 2015 - re-implement with const Interval& and propagate method signature
	virtual RefResult NotifyFunc(Interval changeInt,
		RefTargetHandle hTarget, PartID& partID,
		RefMessage message) MAX_SEALED		
	{ 
		UNUSED_PARAM(changeInt); UNUSED_PARAM(hTarget); 
		UNUSED_PARAM(partID); UNUSED_PARAM(message);
		DbgAssert(false); 
		return REF_FAIL; 
	}
#pragma warning( pop )
};

class NotifyMgr : public ReferenceTarget {
   private:      
      NotifyCallback *mpNotifyCB;
      Tab<RefTargetHandle> mpReferences;
   public:
      // --- Inherited Virtual Methods From Animatable ---
      void GetClassName(MSTR &s) { s = _T("Notifier"); }

      // --- Inherited Virtual Methods From ReferenceMaker ---
      RefResult NotifyRefChanged(const Interval& changeInt, RefTargetHandle hTarget,
         PartID& partID, RefMessage message, BOOL propagate);
      int NumRefs();
      RefTargetHandle GetReference(int i);
      void SetReference(int i, RefTargetHandle rtarg);
	  void DeleteThis();
      // --- Methods From NotifyMgr ---
      NotifyMgr::NotifyMgr();
      NotifyMgr::~NotifyMgr();
	  void SetNumberOfReferences(int num);
      BOOL CreateReference(int which, RefTargetHandle hTarget);
      BOOL RemoveReference(int which);
	  void RemoveAllReferences(); 
      void SetNotifyCB(NotifyCallback *cb);
      void ResetNotifyFunc();
};

#endif
