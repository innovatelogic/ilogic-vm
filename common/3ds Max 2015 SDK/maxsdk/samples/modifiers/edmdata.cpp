/**********************************************************************
 *<
	FILE: edmdata.cpp

	DESCRIPTION:  Edit Mesh temporary data structures

	CREATED BY: Rolf Berteig

	HISTORY: created 1 September, 1995

 *>	Copyright (c) 1994, All Rights Reserved.
 **********************************************************************/
#include "mods.h"
#include "MeshDLib.h"
#include "editmesh.h"

MeshTempData *EditMeshData::TempData (TimeValue t) {
	if (!mod) return NULL;
	if (!tempData) tempData = new MeshTempData();
	tempData->SetMesh (GetMesh (t));
	return tempData;
}

void EditMeshData::Invalidate(ChannelMask channels,BOOL meshValid) {
	if (lockInvalidate) return;
	if (!meshValid && mesh) {
		delete mesh;
		mesh = NULL;
		mValid.SetEmpty ();
	}
	if (channels & TOPO_CHANNEL) topoValid.SetEmpty();
	if (channels & GEOM_CHANNEL) geomValid.SetEmpty();
	if (tempData) tempData->Invalidate (channels);
	if (mod && (channels & SELECT_CHANNEL)) mod->InvalidateNumberSelected();
	if (mod && (channels & (TOPO_CHANNEL|SELECT_CHANNEL|VERTCOLOR_CHANNEL))) mod->InvalidateSurfaceUI ();
}

BOOL EditMeshData::MeshCached(TimeValue t) {
	return (mValid.InInterval(t) && mesh);
}

void EditMeshData::UpdateCache (TimeValue t, TriObject *triOb) {
	if (tempData) {
		ChannelMask invalidators = 0;
		if (!geomValid.InInterval (t)) invalidators |= GEOM_CHANNEL;
		if (!topoValid.InInterval (t)) invalidators |= TOPO_CHANNEL;
		if (invalidators) tempData->Invalidate (invalidators);
	}

	if ( mesh ) delete mesh;
	mesh = new Mesh(triOb->GetMesh());

	mValid = FOREVER;

	// These are the channels we care about.
	geomValid = triOb->ChannelValidity (t, GEOM_CHAN_NUM);
	topoValid = triOb->ChannelValidity (t, TOPO_CHAN_NUM);
	mValid &= geomValid;
	mValid &= topoValid;
	mValid &= triOb->ChannelValidity(t,TEXMAP_CHAN_NUM);
	mValid &= triOb->ChannelValidity(t,VERT_COLOR_CHAN_NUM);
}

Mesh *EditMeshData::GetMesh (TimeValue t) {
	if (mValid.InInterval(t) && mesh) return mesh;
	if (!mod) return NULL;

	SetFlag(EMD_UPDATING_CACHE,TRUE);
	lockInvalidate = TRUE;
	mod->NotifyDependents(Interval(t,t), PART_GEOM|PART_SELECT|PART_SUBSEL_TYPE|PART_DISPLAY|PART_TOPO, REFMSG_MOD_EVAL);
	lockInvalidate = FALSE;
	// Following 2 lines basically active when the pipeline fails to provide us with a TriObject.
	// We set ourselves to be the empty mesh.
	if (!mValid.InInterval(t)) mValid.SetInstant (t);
	if (!mesh) mesh = new Mesh;
	SetFlag(EMD_UPDATING_CACHE,FALSE);
	return mesh;
}


//----------------------------------------------------------
// MeshDeltaUserData methods  (see also triedui.cpp for ui-related MeshDeltaUser stuff

void EditMeshData::MoveSelection(int level, TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, BOOL localOrigin)
{
}

void EditMeshData::RotateSelection(int level, TimeValue t, Matrix3& partm, Matrix3& tmAxis, Quat& val, BOOL localOrigin)
{
}

void EditMeshData::ScaleSelection(int level, TimeValue t, Matrix3& partm, Matrix3& tmAxis, Point3& val, BOOL localOrigin)
{
}

void EditMeshData::ExtrudeSelection(int level, BitArray* sel, float amount, float bevel, BOOL groupNormal, Point3* direction)
{
}

