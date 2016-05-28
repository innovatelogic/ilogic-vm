/**********************************************************************

FILE: BonesDef.cpp

DESCRIPTION:  Simple Bones Deformation Plugin

CREATED BY: Peter Watje

HISTORY: 8/5/98

*>	Copyright (c) 1998, All Rights Reserved.

**********************************************************************

5.1.01 adds left/right justification for bone names in the weight table
adds an action item for right/left justification
fixes bug 471559 which was envelope load crasher when there were more inc bones
than target bones
5.1.02 fixes bug 481214 which  a load by index to the envelope load dialog bug
5.1.03 fixes bug 483252 which allows us to toggle the stretch on/off

**********************************************************************/

#include "max.h"
#include "mods.h"
#include "shape.h"
#include "spline3d.h"
#include "splshape.h"
#include "surf_api.h"
#include "polyobj.h"

// This uses the linked-list class templates
#include "linklist.h"
#include "bonesdef.h"
#include "macrorec.h"
#include "modstack.h"
#include "ISkin.h"
#include "MaxIcon.h"
#include "iXrefItem.h"

#include "3dsmaxport.h"
#include "PerformanceTools.h"
#include <omp.h>

#include "Util\IniUtil.h"

#ifdef _DEBUG
#undef _DEBUG
#include <atlbase.h>
#define _DEBUG
#else
#include <atlbase.h>
#endif

// ini file defines
#define SKIN_INI_FILENAME					_T("skin.ini")
#define LOAD_OLE_SKIN_ENGINE				_T("Load OLE Skin Engine")
#define SKIN_ENABLED						_T("Enabled")

HWND BonesDefMod::hParam  = NULL;
HWND BonesDefMod::hParamAdvance  = NULL;
HWND BonesDefMod::hParamGizmos = NULL;
CreateCrossSectionMode* BonesDefMod::CrossSectionMode   = NULL;
//CreatePaintMode*        BonesDefMod::PaintMode   = NULL;
ICustButton* BonesDefMod::iCrossSectionButton   = NULL;
ICustButton* BonesDefMod::iEditEnvelopes   = NULL;
ICustButton* BonesDefMod::iLock   = NULL;
ICustButton* BonesDefMod::iAbsolute   = NULL;
ICustButton* BonesDefMod::iEnvelope   = NULL;
ICustButton* BonesDefMod::iFalloff   = NULL;
ICustButton* BonesDefMod::iCopy   = NULL;
ICustButton* BonesDefMod::iPaste   = NULL;
ICustButton* BonesDefMod::iPaintButton  = NULL;
ICustToolbar* BonesDefMod::iParams = NULL;
ICustToolbar* BonesDefMod::iMirrorParams = NULL;

IGizmoBuffer* BonesDefMod::copyGizmoBuffer = NULL;
ICustButton* BonesDefMod::iWeightTool   = NULL;
ICustButton* BonesDefMod::iWeightTable   = NULL;

ICustButton* BonesDefMod::iSortText = NULL;

static GenSubObjType SOT_DefPoints(32);

//--- ClassDescriptor and class vars ---------------------------------

//IParamMap       *BonesDefMod::pmapParam = NULL;
IObjParam       *BonesDefMod::ip        = NULL;
BonesDefMod     *BonesDefMod::editMod   = NULL;
MoveModBoxCMode *BonesDefMod::moveMode  = NULL;

VertexWeightCopyBuffer BonesDefMod::vertexWeightCopyBuffer = VertexWeightCopyBuffer();

int MyEnumProc::proc(ReferenceMaker *rmaker)
{
	if (rmaker->SuperClassID()==BASENODE_CLASS_ID)
	{
		Nodes.Append(1, (INode **)&rmaker);
		count++;
		return DEP_ENUM_SKIP;
	}
	return DEP_ENUM_CONTINUE;
}

BOOL BonesDefMod::DependOnTopology(ModContext &mc) {
	BoneModData *bmd = (BoneModData*)mc.localData;
	BOOL topo = FALSE;
	if (bmd)
	{
		for ( int i =0; i <bmd->VertexData.Count(); i++)
		{
			if (bmd->VertexData[i]->IsModified())
			{
				topo = TRUE;
				i = bmd->VertexData.Count();
			}
		}
	}
	return topo;
}

int BonesDefMod::NumRefs() {
	int ct = refHandleList.Count();
	if (refHandleList.Count() ==0)
		return 7;
	else return refHandleList.Count()+5;

	ct = 1;

	for (int i = 0; i<BoneData.Count();i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.RefEndPt1ID > ct) ct = boneData.RefEndPt1ID;
		if (boneData.RefEndPt2ID > ct) ct = boneData.RefEndPt2ID;
		if (boneData.BoneRefID > ct) ct = boneData.BoneRefID;
		for (int crossID = 0; crossID<boneData.CrossSectionList.Count();crossID++)
		{
			if (boneData.CrossSectionList[crossID].RefInnerID > ct) ct = boneData.CrossSectionList[crossID].RefInnerID;
			if (boneData.CrossSectionList[crossID].RefOuterID > ct) ct = boneData.CrossSectionList[crossID].RefOuterID;
		}
	}
	return (ct+5);
}

class ReevalModEnumProc : public ModContextEnumProc {
public:
	BonesDefMod *lm;
	BOOL ev;
	BOOL rd;
	ReevalModEnumProc(BonesDefMod *l, BOOL e, BOOL r)
	{
		lm = l;
		ev = e;
		rd = r;
	}
private:
	BOOL proc (ModContext *mc);
};

BOOL ReevalModEnumProc::proc (ModContext *mc) {
	if (mc->localData == NULL) return TRUE;

	BoneModData *bmd = (BoneModData *) mc->localData;
	bmd->reevaluate = ev;
	bmd->reevaluateActiveBone = rd;
	return TRUE;
}

void BonesDefMod::Reevaluate(BOOL eval)
{
	MyEnumProc dep;
	DoEnumDependents(&dep);
	//this puts back the original state of the node vc mods and shade state
	int nodeCount = 0;
	for (int  i = 0; i < dep.Nodes.Count(); i++)
	{
		BoneModData *bmd = GetBMD(dep.Nodes[i]);
		if (bmd)
		{
			bmd->reevaluate = eval;
			bmd->reevaluateActiveBone = FALSE;
		}
	}
}

void BonesDefMod::ReevaluateActiveBone()
{
	MyEnumProc dep;
	DoEnumDependents(&dep);
	//this puts back the original state of the node vc mods and shade state
	int nodeCount = 0;
	for (int  i = 0; i < dep.Nodes.Count(); i++)
	{
		BoneModData *bmd = GetBMD(dep.Nodes[i]);
		if (bmd)
		{
			bmd->reevaluate = FALSE;
			bmd->reevaluateActiveBone = TRUE;
		}
	}
}

class ForceRecomputeModEnumProc : public ModContextEnumProc {
public:
	BonesDefMod *lm;
	BOOL ev;
	ForceRecomputeModEnumProc(BonesDefMod *l, BOOL e)
	{
		lm = l;
		ev = e;
	}
private:
	BOOL proc (ModContext *mc);
};

BOOL ForceRecomputeModEnumProc::proc (ModContext *mc) {
	if (mc->localData == NULL) return TRUE;

	BoneModData *bmd = (BoneModData *) mc->localData;
	bmd->forceRecomuteBaseNode = ev;
	return TRUE;
}

void BonesDefMod::ForceRecomuteBaseNode(BOOL eval)
{
	MyEnumProc dep;
	DoEnumDependents(&dep);
	//this puts back the original state of the node vc mods and shade state
	int nodeCount = 0;
	for (int  i = 0; i < dep.Nodes.Count(); i++)
	{
		BoneModData *bmd = GetBMD(dep.Nodes[i]);
		if (bmd)
		{
			bmd->forceRecomuteBaseNode = eval;
		}
	}
}

void BonesDefMod::CopyBone()
{
	if (IsValidBoneIndex(ModeBoneIndex))
	{
		BoneDataClass &boneData = BoneData[ModeBoneIndex];
		//get end point1
		Interval v;
		boneData.EndPoint1Control->GetValue(currentTime,&CopyBuffer.E1,v,CTRL_ABSOLUTE);
		//get end point2
		boneData.EndPoint2Control->GetValue(currentTime,&CopyBuffer.E2,v,CTRL_ABSOLUTE);
		//need to set in local space

		CopyBuffer.absolute = FALSE;
		if (boneData.flags & BONE_ABSOLUTE_FLAG)
			CopyBuffer.absolute = TRUE;

		CopyBuffer.showEnvelope = FALSE;
		if (boneData.flags & BONE_DRAW_ENVELOPE_FLAG)
			CopyBuffer.showEnvelope = TRUE;

		CopyBuffer.falloffType = boneData.FalloffType;

		//get cross sections
		CopyBuffer.CList.ZeroCount();
		for (int crossID = 0; crossID < boneData.CrossSectionList.Count();crossID++)
		{
			CopyCrossClass c;
			c.u = boneData.CrossSectionList[crossID].u;
			boneData.CrossSectionList[crossID].InnerControl->GetValue(currentTime,&c.inner,v);
			boneData.CrossSectionList[crossID].OuterControl->GetValue(currentTime,&c.outer,v);
			CopyBuffer.CList.Append(1,&c,1);
		}

		if ((iPaste!=NULL) && (CopyBuffer.CList.Count() != 0))
			iPaste->Enable();
	}
}

void BonesDefMod::PasteBone()
{
	if (IsValidBoneIndex(ModeBoneIndex))
	{
		BoneDataClass &boneData = BoneData[ModeBoneIndex];
		//transform end points back

		//Delete all old cross sections
		int ct = boneData.CrossSectionList.Count();

		if (CopyBuffer.CList.Count() == 0) return;

		CopyBuffer.E1 = CopyBuffer.E1;
		CopyBuffer.E2 = CopyBuffer.E2;

		boneData.FalloffType = CopyBuffer.falloffType;

		if (CopyBuffer.absolute)
			boneData.flags |= BONE_ABSOLUTE_FLAG;
		else boneData.flags &= ~BONE_ABSOLUTE_FLAG;

		if (CopyBuffer.showEnvelope)
			boneData.flags |= BONE_DRAW_ENVELOPE_FLAG;
		else boneData.flags &= ~BONE_DRAW_ENVELOPE_FLAG;

		UpdatePropInterface();

		for (int i =(ct-1); i >= 0 ; i--)
			RemoveCrossSection(ModeBoneIndex, i);
		for (int i =0; i < CopyBuffer.CList.Count() ; i++)
		{
			AddCrossSection(ModeBoneIndex,CopyBuffer.CList[i].u,CopyBuffer.CList[i].inner,CopyBuffer.CList[i].outer);
		}
	}
}

void BonesDefMod::PasteToSomeBones()
{
	if (CopyBuffer.CList.Count() == 0) return;
	int ctl = pasteList.Count();
	for (int m =0; m < ctl; m ++)
	{
		int k = ConvertBoneIDToBoneIndex(pasteList[m]);
		//transform end points back
		if (k != -1)
		{
			BoneDataClass &boneData = BoneData[k];
			//Delete all old cross sections
			int ct = boneData.CrossSectionList.Count();

			CopyBuffer.E1 = CopyBuffer.E1;
			CopyBuffer.E2 = CopyBuffer.E2;

			boneData.FalloffType = CopyBuffer.falloffType;

			if (CopyBuffer.absolute)
				boneData.flags |= BONE_ABSOLUTE_FLAG;
			else boneData.flags &= ~BONE_ABSOLUTE_FLAG;

			if (CopyBuffer.showEnvelope)
				boneData.flags |= BONE_DRAW_ENVELOPE_FLAG;
			else boneData.flags &= ~BONE_DRAW_ENVELOPE_FLAG;

			UpdatePropInterface();

			for (int i =(ct-1); i >= 0 ; i--)
				RemoveCrossSection(k, i);
			for (int i =0; i < CopyBuffer.CList.Count() ; i++)
			{
				AddCrossSection(k,CopyBuffer.CList[i].u,CopyBuffer.CList[i].inner,CopyBuffer.CList[i].outer);
			}
		}
	}
}

void BonesDefMod::PasteToAllBones()
{
	if (CopyBuffer.CList.Count() == 0) return;

	for (int k =0; k < BoneData.Count(); k ++)
	{
		BoneDataClass &boneData = BoneData[k];
		//transform end points back
		if (boneData.Node)
		{
			//Delete all old cross sections
			int ct = boneData.CrossSectionList.Count();

			CopyBuffer.E1 = CopyBuffer.E1;
			CopyBuffer.E2 = CopyBuffer.E2;

			boneData.FalloffType = CopyBuffer.falloffType;

			if (CopyBuffer.absolute)
				boneData.flags |= BONE_ABSOLUTE_FLAG;
			else boneData.flags &= ~BONE_ABSOLUTE_FLAG;

			if (CopyBuffer.showEnvelope)
				boneData.flags |= BONE_DRAW_ENVELOPE_FLAG;
			else boneData.flags &= ~BONE_DRAW_ENVELOPE_FLAG;

			UpdatePropInterface();

			for (int i =(ct-1); i >= 0 ; i--)
				RemoveCrossSection(k, i);
			for (int i =0; i < CopyBuffer.CList.Count() ; i++)
			{
				AddCrossSection(k,CopyBuffer.CList[i].u,CopyBuffer.CList[i].inner,CopyBuffer.CList[i].outer);
			}
		}
	}
}

void BonesDefMod::AddCrossSection(int BoneIndex, float u, float inner, float outer, BOOL update)
{
	if (!IsValidBoneIndex(BoneIndex, false))
		return;
	class CrossSectionClass t;
	int crossID = -1;
	t.u = u;

	int CrossInnerRefID = GetOpenID();
	int CrossOuterRefID = GetOpenID();
	t.RefInnerID = CrossInnerRefID;
	t.RefOuterID  = CrossOuterRefID;
	t.InnerControl = NULL;
	t.OuterControl = NULL;

	BoneDataClass &boneData = BoneData[BoneIndex];
	if ( (boneData.CrossSectionList.Count() == 0) || (boneData.CrossSectionList.Count() == 1))
	{
		crossID = boneData.CrossSectionList.Count();
		boneData.CrossSectionList.Append(1,&t,1);
	}
	else
	{
		for (int i = 0; i < boneData.CrossSectionList.Count();i++)
		{
			if (boneData.CrossSectionList[i].u>=u)
			{
				crossID = i;
				break;
			}
		}
		if (crossID ==-1)
		{
			boneData.CrossSectionList.Append(1,&t);
			crossID = boneData.CrossSectionList.Count()-1;
		}
		else boneData.CrossSectionList.Insert(crossID,1,&t);
	}
	//create 2 float controls
	ReplaceReference(CrossInnerRefID,NewDefaultFloatController());
	ReplaceReference(CrossOuterRefID,NewDefaultFloatController());

	AddToRefHandleList(CrossInnerRefID, boneData.CrossSectionList[crossID].InnerControl);
	AddToRefHandleList(CrossOuterRefID, boneData.CrossSectionList[crossID].OuterControl);

	BOOL animate = FALSE;
	pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);

	if (!animate)
	{
		SuspendAnimate();
		AnimateOff();
	}

	boneData.CrossSectionList[crossID].InnerControl->SetValue(currentTime,&inner,TRUE,CTRL_ABSOLUTE);
	boneData.CrossSectionList[crossID].OuterControl->SetValue(currentTime,&outer,TRUE,CTRL_ABSOLUTE);

	if (!animate)
		ResumeAnimate();

	if (update)
		NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);
}

void BonesDefMod::AddCrossSection(float u)
{
	if (!IsValidBoneIndex(ModeBoneIndex))
		return;
	//get current selected bone
	BoneDataClass &boneData = BoneData[ModeBoneIndex];
	// compute the falloff at the u of this bone
	class CrossSectionClass t;

	float ui,uo,li,lo;
	float u_dist;
	int index = -1;
	t.u = u;
	for (int i = 0; i < boneData.CrossSectionList.Count();i++)
	{
		if (boneData.CrossSectionList[i].u>=u)
		{
			index =i;
			i = boneData.CrossSectionList.Count();
		}
	}

	int lowerbound, upperbound;
	lowerbound = index-1;
	upperbound = index;
	Interval v;
	boneData.CrossSectionList[lowerbound].InnerControl->GetValue(currentTime,&li,v);
	boneData.CrossSectionList[lowerbound].OuterControl->GetValue(currentTime,&lo,v);
	boneData.CrossSectionList[upperbound].InnerControl->GetValue(currentTime,&ui,v);
	boneData.CrossSectionList[upperbound].OuterControl->GetValue(currentTime,&uo,v);

	u_dist = boneData.CrossSectionList[upperbound].u - boneData.CrossSectionList[lowerbound].u;
	u = (u-boneData.CrossSectionList[lowerbound].u) /u_dist;
	float Inner = (ui-li) * u + li;
	float Outer = (uo-lo) * u + lo;

	int CrossInnerRefID = GetOpenID();
	int CrossOuterRefID = GetOpenID();
	t.RefInnerID = CrossInnerRefID;
	t.RefOuterID  = CrossOuterRefID;
	t.InnerControl = NULL;
	t.OuterControl = NULL;
	//create 2 float controls
	boneData.CrossSectionList.Insert(index,1,&t);

	ReplaceReference(CrossInnerRefID,NewDefaultFloatController());
	ReplaceReference(CrossOuterRefID,NewDefaultFloatController());

	AddToRefHandleList(CrossInnerRefID, boneData.CrossSectionList[index].InnerControl);
	AddToRefHandleList(CrossOuterRefID, boneData.CrossSectionList[index].OuterControl);

	BOOL animate = FALSE;
	pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);

	if (!animate)
	{
		SuspendAnimate();
		AnimateOff();
	}

	boneData.CrossSectionList[index].InnerControl->SetValue(currentTime,&Inner,TRUE,CTRL_ABSOLUTE);
	boneData.CrossSectionList[index].OuterControl->SetValue(currentTime,&Outer,TRUE,CTRL_ABSOLUTE);

	if (!animate)
		ResumeAnimate();

	if (index <= ModeBoneEnvelopeIndex)
	{
		ModeBoneEnvelopeIndex++;
		if (ModeBoneEnvelopeIndex >= boneData.CrossSectionList.Count())
			ModeBoneEnvelopeIndex = boneData.CrossSectionList.Count()-1;
	}
	NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);
}

void BonesDefMod::GetCrossSectionRanges(float &inner, float &outer, int boneIndex, int CrossID)
{
	if (!IsValidBoneIndex(boneIndex))
		return;
	Interval v;
	BoneDataClass &boneData = BoneData[boneIndex];
	boneData.CrossSectionList[CrossID].InnerControl->GetValue(currentTime,&inner,v);
	boneData.CrossSectionList[CrossID].OuterControl->GetValue(currentTime,&outer,v);
}

float BonesDefMod::GetU(ViewExp *vpt,Point3 a, Point3 b, IPoint2 p)
{
	if ( ! vpt || ! vpt->IsAlive() )
	{
		// why are we here
		DbgAssert(!_T("Invalid viewport!"));
		return FALSE;
	}

	if ( !ip ) return 0.0f;

	//mouse spot
	INodeTab nodes;
	Point2 fp = Point2((float)p.x, (float)p.y);
	float u;

	GraphicsWindow *gw = vpt->getGW();
	gw->setTransform(Matrix3(1));
	Point2 spa = ProjectPointF(gw, a);
	Point2 spb = ProjectPointF(gw, b);
	u = Length(spa-fp)/Length(spa-spb);

	return u;
}

void BonesDefMod::GetEndPoints(BoneModData *bmd, TimeValue t, Point3 &l1, Point3 &l2, int boneIndex)
{
	if (!IsValidBoneIndex(boneIndex, false))
		return;
	BoneDataClass &boneData = BoneData[boneIndex];
	if ((boneData.flags & BONE_SPLINE_FLAG) && (boneData.Node != NULL) )
	{
		ObjectState os = boneData.Node->EvalWorldState(t);
		ShapeObject *pathOb = (ShapeObject*)os.obj;
		if (pathOb->NumberOfCurves() == 0)
		{
			l1 = Point3(0.0f,0.0f,0.0f);
			l2 = Point3(0.0f,0.0f,0.0f);
			return;
		}

		l1  = pathOb->InterpPiece3D(t, 0, 0, 0.0f ) * Inverse(boneData.tm);
		l2  = pathOb->InterpPiece3D(t, 0, 0, 1.0f ) * Inverse(boneData.tm);
	}
	else
	{
		l1 = bmd->tempTableL1[boneIndex];
		l2 = bmd->tempTableL2[boneIndex];
	}
}

void BonesDefMod::GetEndPointsLocal(BoneModData *bmd, TimeValue t, Point3 &l1, Point3 &l2, int boneIndex)
{
	if (!IsValidBoneIndex(boneIndex, false))
		return;
	BoneDataClass &boneData = BoneData[boneIndex];
	if ((boneData.flags & BONE_SPLINE_FLAG) && (boneData.Node != NULL) )
	{
		ShapeObject *pathOb = NULL;
		ObjectState os = boneData.Node->EvalWorldState(t);
		pathOb = (ShapeObject*)os.obj;
		if (pathOb->NumberOfCurves() == 0)
		{
			l1 = Point3(0.0f,0.0f,0.0f);
			l2 = Point3(0.0f,0.0f,0.0f);
			return;
		}

		l1  = pathOb->InterpPiece3D(t, 0, 0, 0.0f );
		l2  = pathOb->InterpPiece3D(t, 0, 0, 1.0f );
	}
	else
	{
		if (stopMessagePropogation)
		{
			Point3 tl1, tl2;

			Interval v;
			boneData.EndPoint1Control->GetValue(currentTime,&tl1,v);
			boneData.EndPoint2Control->GetValue(currentTime,&tl2,v);
			l1 = tl1 * bmd->tmCacheToObjectSpace[boneIndex];
			l2 = tl2 * bmd->tmCacheToObjectSpace[boneIndex];
		}
		else
		{
			l1 = bmd->tempTableL1ObjectSpace[boneIndex];
			l2 = bmd->tempTableL2ObjectSpace[boneIndex];
		}
	}
}

float BonesDefMod::ModifyU(TimeValue t, float LineU, int boneIndex, int sid)
{
	if (!IsValidBoneIndex(boneIndex))
		return 0.0f;
	BoneDataClass &boneData = BoneData[boneIndex];
	if (boneData.flags & BONE_SPLINE_FLAG)
	{
		ObjectState os = boneData.Node->EvalWorldState(t);
		ShapeObject *pathOb = (ShapeObject*)os.obj;
		if (pathOb->NumberOfCurves() == 0)
		{
			return 0.0f;
		}
		int SubCount = boneData.referenceSpline.Segments();

		float start,inc;
		inc = 1.0f/(float)SubCount;
		start = inc * sid;
		LineU = start + (LineU * inc);
	}
	return LineU;
}

float BonesDefMod::ComputeInfluence(TimeValue t, float Influence, float LineU, int boneIndex, int StartCross, int EndCross, int sid)
{
	if (!IsValidBoneIndex(boneIndex))
		return 0.0f;

	float Inner, Outer;
	float LInner, LOuter;

	GetCrossSectionRanges(Inner, Outer, boneIndex, StartCross);
	GetCrossSectionRanges(LInner, LOuter, boneIndex, EndCross);

	LineU = ModifyU(t, LineU,boneIndex,sid);

	BoneDataClass &boneData = BoneData[boneIndex];
	float udist = boneData.CrossSectionList[EndCross].u - boneData.CrossSectionList[StartCross].u;
	LineU = LineU - boneData.CrossSectionList[StartCross].u;
	float per = LineU/udist;

	Inner = Inner + (LInner - Inner) * per;
	Outer = Outer + (LOuter - Outer) * per;

	//inside inner envelope
	if (Influence <= Inner)
	{
		Influence = 1.0f;
	}
	// is it oustide  outer
	else if (Influence <= Outer)
	{
		float r1,r2;
		r1 = Outer - Inner;
		r2 = Influence - Inner;
		Influence = 1.0f - (r2/r1);
		ComputeFalloff(Influence,boneData.FalloffType);
	}
	//outside puter envelope
	else
	{
		Influence = 0.0f;
	}
	return Influence;
}

int BonesDefMod::ConvertBoneIDToBoneIndex(int boneID)
{
	int id = -1;
	for (int i = 0; i < BoneData.Count(); i++)
	{
		if (BoneData[i].Node != NULL) id++;
		if (boneID == id)
			return i;
	}
	return -1;
}

int BonesDefMod::ConvertBoneIndexToBoneID(int boneIndex)
{
	if (!IsValidBoneIndex(boneIndex))
		return -1;
	int boneID = 0;
	for (int i = 0; i < boneIndex; i++)
	{
		if (BoneData[i].Node != NULL) boneID++;
	}
	return boneID;
}

int BonesDefMod::ConvertBoneIndexToListID(int boneIndex)
{
	if (!hParam) return -1;

	HWND hListBox = GetDlgItem(hParam, IDC_LIST1);
	int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);

	if (count == 0) return -1;

	int listID = LB_ERR;
	for (int i = 0; i < count; i ++)
	{
		int temp = SendMessage(hListBox, LB_GETITEMDATA, (WPARAM)i, 0);
		if (temp == boneIndex)
		{
			listID = i;
			break;
		}
	}

	DbgAssert(listID != LB_ERR);

	return listID;
}

int BonesDefMod::ConvertListIDToBoneIndex(int listID)
{
	// if the user open a new scene, at this time this dialog was not opened yet.
	if (!hParam) return -1;
	if (0 == SendMessage(GetDlgItem(hParam, IDC_LIST1), LB_GETCOUNT, 0, 0)) return -1;
	int boneIndex = SendMessage(GetDlgItem(hParam, IDC_LIST1), LB_GETITEMDATA, listID, 0);

	DbgAssert(boneIndex != LB_ERR);
	return boneIndex;
}

int BonesDefMod::NumberNonNullBones()
{
	int count = 0;
	for (int i =0; i < BoneData.Count(); i ++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node)
		{
			DbgAssert(!(boneData.flags & BONE_DEAD_FLAG));
			count++;
		}
		else
		{
			DbgAssert(boneData.flags & BONE_DEAD_FLAG);
		}
	}
	return count;
}

bool BonesDefMod::HasNonNullBones()
{
	for (int i =0; i < BoneData.Count(); i ++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node)
		{
			DbgAssert(!(boneData.flags & BONE_DEAD_FLAG));
			return true;
		}
		else
		{
			DbgAssert(boneData.flags & BONE_DEAD_FLAG);
		}
	}
	return false;
}

static bool Comp(const TCHAR* left, const TCHAR* right, bool ascend)
{
	// set to right locale, so all locale characters can be compared rightly.
	TCHAR* prevLocale = _tsetlocale(LC_ALL, NULL); //Get and copy the locale string
	if (prevLocale != NULL)
	{
		prevLocale = _tcsdup(prevLocale);
	}
	_tsetlocale(LC_ALL, _T("")); //Set the new locale

	bool ret = false;
	if (ascend && _tcsicmp(left, right) < 0)
	{
		ret = true;
	}
	else if (!ascend && _tcsicmp(left, right) > 0)
	{
		ret = true;
	}

	_tsetlocale(LC_ALL, prevLocale); //Restore previous locale
	if (prevLocale != NULL) free(prevLocale);

	return ret;
}

int BonesDefMod::FindListInsertPos(const TCHAR *toInsert)
{
	HWND hListBox = GetDlgItem(hParam,IDC_LIST1);

	int count = SendMessage(hListBox, LB_GETCOUNT, 0, 0);
	int left = 0, right = count - 1;
	TCHAR buff[500];

	while (left <= right)
	{
		int mid = (left + right) / 2;

		SendMessage(hListBox, LB_GETTEXT, mid, (LPARAM)buff);
		if (Comp(toInsert, buff, sortByIncreace))
		{
			right = mid - 1;
		}
		else
		{
			left = mid + 1;
		}
	}

	if (left == count) left = -1;

	return left;
}

void BonesDefMod::RefillListBox()
{
	if (!hParam) return;

	SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_RESETCONTENT,0,0);

	RECT rect;

	GetWindowRect(GetDlgItem(hParam,IDC_LIST1), &rect);
	int width = rect.right - rect.left-2;

	BOOL shorten = FALSE;
	pblock_advance->GetValue(skin_advance_shortennames,0,shorten,FOREVER);

	HDC hdc = GetDC(GetDlgItem(hParam,IDC_LIST1));
	HFONT hOldFont = (HFONT)SelectObject(hdc, GetCOREInterface()->GetAppHFont());

	int textWidth = 0;

	TCHAR title[500];

	for (int i=0;i<BoneData.Count();i++)
	{
		BoneDataClass& boneData = BoneData[i];
		if(boneData.bFilterOut == TRUE)
		{
			continue;
		}
		if (boneData.Node != NULL)
		{
			Class_ID bid(BONE_CLASS_ID,0);
			ObjectState os = boneData.Node->EvalWorldState(RefFrame);

			if (( os.obj->ClassID() == bid) && (!boneData.name.isNull()) )
			{
				_tcscpy(title,boneData.name);
			}
			else _tcscpy(title,boneData.Node->GetName());

			SIZE size;

			int titleLen = static_cast<int>(_tcslen(title));
			TSTR tempstr;
			tempstr.printf(_T("%s"),title);
			DLGetTextExtent(hdc,  (LPCTSTR)tempstr,&size);

			if (size.cx > textWidth) textWidth = size.cx;

			if ((shorten) && ( size.cx > width))
			{
				BOOL done = FALSE;
				while (!done)
				{
					int len = static_cast<int>(_tcslen(title));
					int mid = static_cast<int>(_tcslen(title)/2);
					for (int j = mid; j < (len); j++)
						title[j] = title[j+1];
					DLGetTextExtent(hdc, title, (LPSIZE)&size);
					if (size.cx < width)
					{
						title[mid-1] = _T('.');
						title[mid] = _T('.');
						title[mid+1] = _T('.');
						done = TRUE;
					}
				}
			}

			int insertIndex = FindListInsertPos(title);
			insertIndex = SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_INSERTSTRING, insertIndex, (LPARAM)title);
			SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_SETITEMDATA, insertIndex, i);
		}
	}
	if (!shorten)
	{
		SendDlgItemMessage(hParam, IDC_LIST1, LB_SETHORIZONTALEXTENT, (textWidth+8), 0);
	}
	SelectObject(hdc, hOldFont);
	ReleaseDC(GetDlgItem(hParam,IDC_LIST1),hdc);
}

void BonesDefMod::RemoveBone()
{
	if (inAddBoneMode) AddFromViewEnd();

	int fsel = SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_GETCURSEL,0,0);
	int boneIndex = ConvertListIDToBoneIndex(fsel);

	if (boneIndex>=0)
	{
		SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_DELETESTRING, (WPARAM) fsel,0);

		BoneDataClass &boneData = BoneData[boneIndex];
		//nuke cross sections
		int ct = boneData.CrossSectionList.Count();
		for (int i =(ct-1); i >= 0 ; i--)
			RemoveCrossSectionNoNotify(boneIndex, i);

		//nuke reference
		//nuke end points
		DeleteReference(boneData.RefEndPt1ID);
		boneData.EndPoint1Control = NULL;
		AddToRefHandleList(boneData.RefEndPt1ID, NULL);

		DeleteReference(boneData.RefEndPt2ID);
		boneData.EndPoint2Control = NULL;
		AddToRefHandleList(boneData.RefEndPt2ID, NULL);

		RefTable[boneData.RefEndPt1ID-BONES_REF] = 0;
		RefTable[boneData.RefEndPt2ID-BONES_REF] = 0;
		recompBoneMap = true;
		DeleteReference(boneData.BoneRefID);
		RefTable[boneData.BoneRefID-BONES_REF] = 0;
		boneData.Node = NULL;
		AddToRefHandleList(boneData.BoneRefID, NULL);

		boneData.flags = BONE_DEAD_FLAG;

		boneData.RefEndPt1ID = -1;
		boneData.RefEndPt2ID = -1;
		boneData.BoneRefID = -1;

		BOOL noBonesLeft = FALSE;
		if (SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_SETCURSEL,fsel,0) == LB_ERR)
		{
			if (fsel != 0)
				fsel--;
			else noBonesLeft = TRUE;
			SelectBoneByListBoxIndex(fsel);
		}
		if (noBonesLeft)
		{
			boneIndex = -1;
			fsel = -1;
		}
		else boneIndex = ConvertListIDToBoneIndex(fsel);
		ModeBoneIndex = boneIndex;
		ModeBoneEndPoint = -1;
		ModeBoneEnvelopeIndex = -1;
		ModeBoneEnvelopeSubType = -1;
		LastSelected = fsel;
		UpdatePropInterface();

		BoneMoved = TRUE;
		Reevaluate(TRUE);
		cacheValid = FALSE;
		NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);
	}

	if  (!HasNonNullBones())
	{
		DisableButtons();
	}

	NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);

	//WEIGHTTABLE
	weightTableWindow.RecomputeBones();

	BOOL shorten = FALSE;
	pblock_advance->GetValue(skin_advance_shortennames,0,shorten,FOREVER);
	if (!shorten)
	{
		HDC hdc = GetDC(GetDlgItem(hParam,IDC_LIST1));
		HFONT hOldFont = (HFONT)SelectObject(hdc, GetCOREInterface()->GetAppHFont());
		int textWidth = 0;
		for (int i=0;i<BoneData.Count();i++)
		{
			BoneDataClass &boneData = BoneData[i];
			if (boneData.Node != NULL)
			{
				TCHAR title[500];
				_tcscpy(title,boneData.Node->GetName());
				SIZE size;

				int titleLen = static_cast<int>(_tcslen(title));
				TSTR tempstr;
				tempstr.printf(_T("%s"),title);
				GetTextExtentPoint32(hdc,  (LPCTSTR)tempstr, tempstr.Length(),&size);

				if (size.cx > textWidth) textWidth = size.cx;
			}
		}
		SendDlgItemMessage(hParam, IDC_LIST1, LB_SETHORIZONTALEXTENT, (textWidth+8), 0);
		SelectObject(hdc, hOldFont);
		ReleaseDC(GetDlgItem(hParam,IDC_LIST1),hdc);
	}

	PaintAttribList(TRUE);
}

void BonesDefMod::BuildBaseNodeData()
{
	MyEnumProc dep;
	DoEnumDependents(&dep);
	//this puts back the original state of the node vc mods and shade state
	int nodeCount = 0;
	for (int  i = 0; i < dep.Nodes.Count(); i++)
	{
		BoneModData *bmd = GetBMD(dep.Nodes[i]);
		if (bmd) nodeCount++;
	}

	loadBaseNodeData.SetCount(nodeCount);
	int ct = 0;
	for (int i = 0; i < dep.Nodes.Count(); i++)
	{
		BoneModData *bmd = GetBMD(dep.Nodes[i]);
		if (bmd)
		{
			loadBaseNodeData[ct].node = dep.Nodes[i];
			loadBaseNodeData[ct].bmd = bmd;
			loadBaseNodeData[ct].matchData = NULL;
			ct++;
		}
	}
	//now match inc data to current data using names
	for (int i = 0; i < loadBaseNodeData.Count(); i++)
	{
		if (loadBaseNodeData[i].matchData == NULL)
		{
			for (int j = 0; j < vertexLoadList.Count(); j++)
			{
				const TCHAR *name = loadBaseNodeData[i].node->GetName();
				const TCHAR *name2 = vertexLoadList[j]->name;
				if ( (_tcscmp(name,name2)==0) || ( (loadBaseNodeData.Count() == 1)  &&(vertexLoadList.Count()==1)))
				{
					loadBaseNodeData[i].matchData = vertexLoadList[j];
					vertexLoadList.Delete(j,1);
					break;
				}
			}
		}
	}

	//now match inc data to current data using vert count
	for (int i = 0; i < loadBaseNodeData.Count(); i++)
	{
		if (loadBaseNodeData[i].matchData == NULL)
		{
			for (int j = 0; j < vertexLoadList.Count(); j++)
			{
				if (loadBaseNodeData[i].bmd->VertexData.Count()== vertexLoadList[j]->vertexData.Count())
				{
					loadBaseNodeData[i].matchData = vertexLoadList[j];
					vertexLoadList.Delete(j,1);
					break;
				}
			}
		}
	}
	Tab<LoadVertexDataClass*> tempList;
	tempList.SetCount(vertexLoadList.Count());
	for (int i = 0; i < vertexLoadList.Count(); i++)
	{
		tempList[i] = vertexLoadList[i];
		vertexLoadList[i] = NULL;
	}
	vertexLoadList.SetCount(loadBaseNodeData.Count());
	for (int i = 0; i < loadBaseNodeData.Count(); i++)
	{
		vertexLoadList[i] = loadBaseNodeData[i].matchData;
	}
	for (int i = 0; i < tempList.Count(); i++)
	{
		vertexLoadList.Append(1,&tempList[i]);
	}
}

void BonesDefMod::RemoveBone(int boneIndex)
{
	if (inAddBoneMode) AddFromViewEnd();

	if (IsValidBoneIndex(boneIndex))
	{
		int fsel = ConvertBoneIndexToListID(boneIndex);
		SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_DELETESTRING, (WPARAM) fsel,0);

		//nuke reference
		//nuke cross sections

		if (theHold.Holding() )
		{
			theHold.Put(new DeleteBoneRestore(this,boneIndex));

			MyEnumProc dep;
			DoEnumDependents(&dep);
			//this puts back the original state of the node vc mods and shade state
			for (int  i = 0; i < dep.Nodes.Count(); i++)
			{
				BoneModData *bmd = GetBMD(dep.Nodes[i]);
				if (bmd)
					theHold.Put(new WeightRestore(this,bmd));
			}
		}

		BoneDataClass& boneData = BoneData[boneIndex];
		int ct = boneData.CrossSectionList.Count();
		for (int i =(ct-1); i >= 0 ; i--)
			RemoveCrossSectionNoNotify(boneIndex, i);

		//nuke end points
		DeleteReference(boneData.RefEndPt1ID);
		boneData.EndPoint1Control = NULL;
		AddToRefHandleList(boneData.RefEndPt1ID, NULL);

		DeleteReference(boneData.RefEndPt2ID);
		boneData.EndPoint2Control = NULL;
		AddToRefHandleList(boneData.RefEndPt2ID, NULL);

		RefTable[boneData.RefEndPt1ID-BONES_REF] = 0;
		RefTable[boneData.RefEndPt2ID-BONES_REF] = 0;

		DeleteReference(boneData.BoneRefID);
		RefTable[boneData.BoneRefID-BONES_REF] = 0;
		boneData.Node = NULL;
		AddToRefHandleList(boneData.BoneRefID, NULL);

		recompBoneMap = true;
		boneData.flags = BONE_DEAD_FLAG;

		boneData.RefEndPt1ID = -1;
		boneData.RefEndPt2ID = -1;
		boneData.BoneRefID = -1;

		ModeBoneEndPoint = -1;
		ModeBoneEnvelopeIndex = -1;
		ModeBoneEnvelopeSubType = -1;

		ModeBoneIndex = boneIndex;
		SelectBoneByBoneIndex(ModeBoneIndex);

		BoneMoved = TRUE;
		Reevaluate(TRUE);
		NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);
	}

	if  (!HasNonNullBones())
	{
		DisableButtons();
	}

	NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);

	//WEIGHTTABLE
	weightTableWindow.RecomputeBones();
}

void BonesDefMod::RemoveCrossSection()
{
	if ( (ModeBoneEnvelopeIndex <=0)  || (ModeBoneIndex < 0)  ||
		(ModeBoneEnvelopeIndex >= (BoneData[ModeBoneIndex].CrossSectionList.Count()-1))
		)
		return;

	BoneDataClass& boneData = BoneData[ModeBoneIndex];

	DeleteReference(boneData.CrossSectionList[ModeBoneEnvelopeIndex].RefInnerID);
	boneData.CrossSectionList[ModeBoneEnvelopeIndex].InnerControl = NULL;
	AddToRefHandleList(boneData.CrossSectionList[ModeBoneEnvelopeIndex].RefInnerID, NULL);

	DeleteReference(boneData.CrossSectionList[ModeBoneEnvelopeIndex].RefOuterID);
	boneData.CrossSectionList[ModeBoneEnvelopeIndex].OuterControl = NULL;
	AddToRefHandleList(boneData.CrossSectionList[ModeBoneEnvelopeIndex].RefOuterID, NULL);

	RefTable[boneData.CrossSectionList[ModeBoneEnvelopeIndex].RefInnerID-BONES_REF] = 0;
	RefTable[boneData.CrossSectionList[ModeBoneEnvelopeIndex].RefOuterID-BONES_REF] = 0;

	boneData.CrossSectionList.Delete(ModeBoneEnvelopeIndex,1);

	Reevaluate(TRUE);
	NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);
	NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);
}

void BonesDefMod::RemoveCrossSection(int boneIndex, int eid)
{
	BoneDataClass& boneData = BoneData[boneIndex];
	DeleteReference(boneData.CrossSectionList[eid].RefInnerID);
	boneData.CrossSectionList[eid].InnerControl = NULL;
	AddToRefHandleList(boneData.CrossSectionList[eid].RefInnerID, NULL);

	DeleteReference(boneData.CrossSectionList[eid].RefOuterID);
	boneData.CrossSectionList[eid].OuterControl = NULL;
	AddToRefHandleList(boneData.CrossSectionList[eid].RefOuterID, NULL);

	RefTable[boneData.CrossSectionList[eid].RefInnerID-BONES_REF] = 0;
	RefTable[boneData.CrossSectionList[eid].RefOuterID-BONES_REF] = 0;

	boneData.CrossSectionList.Delete(eid,1);

	Reevaluate(TRUE);
	NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);
	NotifyDependents(FOREVER, 0, REFMSG_SUBANIM_STRUCTURE_CHANGED);
}

void BonesDefMod::RemoveCrossSectionNoNotify(int boneIndex, int eid)
{
	BoneDataClass& boneData = BoneData[boneIndex];
	DeleteReference(boneData.CrossSectionList[eid].RefInnerID);
	boneData.CrossSectionList[eid].InnerControl = NULL;
	AddToRefHandleList(boneData.CrossSectionList[eid].RefInnerID, NULL);

	DeleteReference(boneData.CrossSectionList[eid].RefOuterID);
	boneData.CrossSectionList[eid].OuterControl = NULL;
	AddToRefHandleList(boneData.CrossSectionList[eid].RefOuterID, NULL);

	RefTable[boneData.CrossSectionList[eid].RefInnerID-BONES_REF] = 0;
	RefTable[boneData.CrossSectionList[eid].RefOuterID-BONES_REF] = 0;

	boneData.CrossSectionList.Delete(eid,1);

	Reevaluate(TRUE);
}

//--- Affect region mod methods -------------------------------

BonesDefMod::BonesDefMod()
{
	RefFrame = 0;
	displayCallback = NULL;
	updateDialogs = TRUE;
	showWeightTool = FALSE;
	weightToolHWND = NULL;
	hasStretchTM = TRUE;
	loadByIndex = TRUE;

	enableFastSubAnimList = TRUE;
	rebuildSubAnimList = TRUE;

	stopEvaluation = FALSE;

	stopMessagePropogation = FALSE;
	updateOnMouseUp = FALSE;

	updateListBox = FALSE;
	inAddBoneMode=FALSE;
	resolvedModify = TRUE;

	splinePresent = FALSE;
	editing = FALSE;
	inRender = FALSE;
	fastUpdate = FALSE;

	ver = 4;
	recompInitTM = false;
	recompBoneMap = true;

	sortByIncreace = true;

	pblock_param = pblock_display = pblock_gizmos = pblock_advance = pblock_weighttable = pblock_mirror = NULL;
	p1Temp = NULL;
	GetBonesDefModDesc()->MakeAutoParamBlocks(this);

	pblock_param->SetValue(skin_effect,0,0.0f);
	pblock_param->SetValue(skin_cross_radius,0,10.0f);

	pblock_param->SetValue(skin_filter_vertices,0,0);
	pblock_param->SetValue(skin_filter_bones,0,1);
	pblock_param->SetValue(skin_filter_envelopes,0,1);

	pblock_display->SetValue(skin_display_draw_all_envelopes,0,0);
	pblock_display->SetValue(skin_display_draw_vertices,0,1);

	pblock_param->SetValue(skin_paint_feather,0,0.7f);
	pblock_param->SetValue(skin_paint_radius,0,24.0f);

	pblock_param->SetValue(skin_paint_str,0,0.1f);

	pblock_advance->SetValue(skin_advance_ref_frame,0,0);
	pblock_advance->SetValue(skin_advance_always_deform,0,1);

	RefTable.ZeroCount();

	BoneData.New();
	Reevaluate(FALSE);
	reset = FALSE;
	BoneMoved = FALSE;
	Point3 p(0.0f,0.0f,0.0f);
	ReplaceReference(POINT1_REF,NewDefaultPoint3Controller());
	p1Temp->SetValue(0,p,TRUE,CTRL_ABSOLUTE);
	ModeEdit = 0;
	ModeBoneIndex = -1;
	ModeBoneEndPoint  = -1;
	ModeBoneEnvelopeIndex = -1;
	ModeBoneEnvelopeSubType = -1;
	FilterVertices = 0;
	FilterBones = 0;
	FilterEnvelopes = 0;
	DrawEnvelopes = 0;

	cacheValid = FALSE;
	OldVertexDataCount = 0;
	unlockBone = FALSE;
	unlockAllBones = FALSE;

	painting = FALSE;
	inPaint = FALSE;
	reloadSplines = FALSE;

	splineChanged = FALSE;
	updateP = FALSE;

	bindNode = NULL;
	initialXRefTM.IdentityMatrix();
	xRefTM.IdentityMatrix();

	RegisterNotification(NotifyPreDeleteNode, this, NOTIFY_SYSTEM_PRE_RESET);
	RegisterNotification(NotifyPreDeleteNode, this, NOTIFY_SYSTEM_SHUTDOWN);

	RegisterNotification(NotifyPreSave, this, NOTIFY_FILE_PRE_SAVE);
	RegisterNotification(NotifyPostSave, this, NOTIFY_FILE_POST_SAVE);

	//WEIGHTTABLE
	weightTableWindow.InitMod(this);
	hWeightTable = NULL;
	vcState = FALSE;

	backTransform = TRUE;

	loadVertData = TRUE;
	loadExclusionData = TRUE;

	LastSelected = 0;
	mParameterListSelection = 0;

	//get the painterinterface
	ReferenceTarget *ref  = (ReferenceTarget *) (GetCOREInterface()->CreateInstance(REF_TARGET_CLASS_ID,PAINTERINTERFACE_CLASS_ID));
	if (ref)
	{
		pPainterInterface = static_cast<IPainterInterface_V5 *> (ref->GetInterface(PAINTERINTERFACE_V5));
	}

	SetupColors();

	BOOL iret;
	TSTR name,category;

	name.printf(_T("%s"),GetString(IDS_NOWEIGHTCOLOR));
	category.printf(_T("%s"),GetString(IDS_SKINCOLOR));
	COLORREF noWeightColor;
	noWeightColor = RGB(128,128,128);
	iret = ColorMan()->RegisterColor(NOWEIGHTCOLORID,name,category, noWeightColor);
}

BonesDefMod::~BonesDefMod()
{
	if (hWeightTable)
	{
		weightTableWindow.ClearMod();
		DestroyWindow(hWeightTable);
	}

	DeleteAllRefsFromMe();
	p1Temp = NULL;

	for (int i=0;i<BoneData.Count();i++)
		BoneData[i].CrossSectionList.ZeroCount();

	BoneData.New();

	for (int i = 0; i < splineList.Count(); i++)
	{
		if (splineList[i])
		{
			delete splineList[i];
			splineList[i] = NULL;
		}
	}
	UnRegisterNotification(NotifyPreDeleteNode, this,	NOTIFY_SYSTEM_PRE_RESET);
	UnRegisterNotification(NotifyPreDeleteNode, this,	NOTIFY_SYSTEM_SHUTDOWN);

	UnRegisterNotification(NotifyPreSave, this, NOTIFY_FILE_PRE_SAVE);
	UnRegisterNotification(NotifyPostSave, this, NOTIFY_FILE_POST_SAVE);
}

void BonesDefMod::SetupColors()
{
	pblock_display->SetCount(skin_display_weightcolors,25);
	Point3 color(0.0f,0.0f,0.0f);
	int ct = 0;

	color = Point3(0.9f,0.0f,0.0f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.0f,0.9f,0.0f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.0f,0.0f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.9f,0.90f,0.0f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.0f,0.9f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.9f,0.0f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.49f,0.0f,0.0f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.0f,0.49f,0.0f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.0f,0.0f,0.49f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.49f,0.49f,0.0f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.0f,0.49f,0.49f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.9f,0.49f,0.0f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.0f,0.49f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.49f,0.9f,0.0f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.0f,0.9f,0.49f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.49f,0.0f,0.49f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.9f,0.0f,0.49f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.49f,0.0f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.49f,0.49f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.9f,0.49f,0.49f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.9f,0.49f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.9f,0.9f,0.49f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.49f,0.9f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.49f,0.49f,0.49f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);

	color = Point3(0.9f,0.9f,0.9f);
	pblock_display->SetValue(skin_display_weightcolors,0,color,ct++);
}

int BonesDefMod::RenderBegin(TimeValue t, ULONG flags)
{
	inRender= TRUE;

	if (fastUpdate)
		NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);
	return 0;
}

int BonesDefMod::RenderEnd(TimeValue t)
{
	inRender= FALSE;
	return 0;
}

RefTargetHandle BonesDefMod::Clone(RemapDir& remap)
{
	BonesDefMod *mod = new BonesDefMod();
	mod->ReplaceReference(PBLOCK_PARAM_REF,remap.CloneRef(pblock_param));
	mod->ReplaceReference(POINT1_REF,remap.CloneRef(p1Temp));

	mod->ReplaceReference(PBLOCK_DISPLAY_REF,remap.CloneRef(pblock_display));
	mod->ReplaceReference(PBLOCK_GIZMOS_REF,remap.CloneRef(pblock_gizmos));
	mod->ReplaceReference(PBLOCK_ADVANCE_REF,remap.CloneRef(pblock_advance));
	//WEIGHTTABLE
	mod->ReplaceReference(PBLOCK_WEIGHTTABLE_REF,remap.CloneRef(pblock_weighttable));
	//MIRROR
	mod->ReplaceReference(PBLOCK_MIRROR_REF,remap.CloneRef(pblock_mirror));

	//copy controls
	mod->RefTable = RefTable;

	for (int i = 0; i<BoneData.Count(); i++)
	{
		BoneDataClass b = BoneData[i];
		b.Node = NULL;
		b.EndPoint1Control= NULL;
		b.EndPoint2Control = NULL;
		b.CrossSectionList.SetCount(BoneData[i].CrossSectionList.Count());
		for (int crossID = 0; crossID < b.CrossSectionList.Count(); crossID++)
		{
			b.CrossSectionList[crossID].InnerControl = NULL;
			b.CrossSectionList[crossID].OuterControl = NULL;
		}
		mod->BoneData.Append(b);
	}

	mod->refHandleList.SetCount(refHandleList.Count());
	for (int i=0; i < refHandleList.Count(); i++)
		mod->refHandleList[i]= NULL;

	for (int i=0;i < BoneData.Count();i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.EndPoint1Control)
			mod->ReplaceReference(boneData.RefEndPt1ID,remap.CloneRef(boneData.EndPoint1Control));
		if (boneData.EndPoint2Control)
			mod->ReplaceReference(boneData.RefEndPt2ID,remap.CloneRef(boneData.EndPoint2Control));
		if (boneData.Node)
			mod->ReplaceReference(boneData.BoneRefID,boneData.Node);

		for (int crossID=0;crossID < boneData.CrossSectionList.Count();crossID++)
		{
			if (boneData.CrossSectionList[crossID].InnerControl)
				mod->ReplaceReference(boneData.CrossSectionList[crossID].RefInnerID,remap.CloneRef(boneData.CrossSectionList[crossID].InnerControl));
			if (boneData.CrossSectionList[crossID].OuterControl)
				mod->ReplaceReference(boneData.CrossSectionList[crossID].RefOuterID,remap.CloneRef(boneData.CrossSectionList[crossID].OuterControl));
		}
	}

	mod->cacheValid = FALSE;

	for (int i = 0; i < pblock_gizmos->Count(skin_gizmos_list) ; i++)
	{
		ReferenceTarget *ref;
		ref = mod->pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,i);
		GizmoClass *gizmo = (GizmoClass *)ref;
		gizmo->bonesMod = mod;
		mod->pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,i);
	}

	mod->hasStretchTM = hasStretchTM;

	BaseClone(this, mod, remap);
	return mod;
}

class NullView: public View {
public:
	Point2 ViewToScreen(Point3 p) { return Point2(p.x,p.y); }
	NullView() { worldToView.IdentityMatrix(); screenW=640.0f; screenH = 480.0f; }
};

void BonesDefMod::BuildEnvelopes(INode *bnode, Object *obj, Point3 l1, Point3 l2, float &el1, float &el2)
{
	float closestEnd = -1.0f,closestStart = -1.0f,closestMid = -1.0f;

	Matrix3 tm = bnode->GetObjectTM(RefFrame);

	l1 = l1 * Inverse(tm);
	l2 = l2 * Inverse(tm);

	for (int i =0; i < obj->NumPoints(); i++)
	{
		float u;
		float dist;
		Point3 p = obj->GetPoint(i);
		dist = LineToPoint(p, l1, l2, u);
		//loop through all points finding finding closest perpendicular
		if ((u>0.0f) && (u <1.0f))
		{
			if ((closestMid < 0.0f) || (dist<closestMid))
			{
				closestMid = dist;
			}
		}
		//closest first quarter
		if ((u>0.0f) && (u <0.150f))
		{
			if ((closestStart < 0.0f) || (dist<closestStart))
			{
				closestStart = dist;
			}
		}

		//closest last quarter
		if ((u>.85f) && (u < 1.0f))
		{
			if ((closestEnd < 0.0f) || (dist<closestEnd))
			{
				closestEnd = dist;
			}
		}
	}
	if (closestMid < 0.0f)
	{
		el1 = Length(l2-l1);
		el2 = Length(l2-l1);
		return;
	}
	if (closestEnd < 0.0f)
	{
		closestEnd = closestMid;
	}
	if (closestStart < 0.0f)
	{
		closestStart = closestMid;
	}
	if (closestStart < el1) el1 = closestStart;
	if (closestEnd < el2) el2 = closestEnd;
}

void BonesDefMod::BuildMajorAxis(INode *node, Point3 &l1, Point3 &l2, float &el1, Matrix3 *tm)
{
	DbgAssert(tm);
	if (tm == nullptr)
		return;
	//get object state
	ObjectState os;

	os = node->EvalWorldState(RefFrame);

	//get bounding box
	Box3 bb,bbLocalSpace;
	//get longest axis
	os.obj->GetDeformBBox(0,bb);

	float dx,dy,dz,axislength;
	dx = bb.pmax.x - bb.pmin.x;
	dy = bb.pmax.y - bb.pmin.y;
	dz = bb.pmax.z - bb.pmin.z;
	int axis;
	axislength = dx;
	axis = 0;

	Point3 vecX(0.0f,0.0f,0.0f),vecY(0.0f,0.0f,0.0f),vecZ(0.0f,0.0f,0.0f);
	vecX.x = dx;
	vecY.y = dy;
	vecZ.z = dz;
	vecX = VectorTransform(*tm,vecX ) ;
	vecY = VectorTransform(*tm,vecY );
	vecZ = VectorTransform(*tm,vecZ );
	float vecXLength,vecYLength,vecZLength;

	vecXLength = Length(vecX);
	vecYLength = Length(vecY);
	vecZLength = Length(vecZ);

	if (dy > axislength)
	{
		axis = 1;
		axislength = dy;
	}
	if (dz > axislength)
	{
		axis = 2;
		axislength = dz;
	}

	if (axis ==0)
	{
		l1.x = bb.pmax.x;
		l2.x = bb.pmin.x;

		l1.y = (bb.pmax.y + bb.pmin.y) *0.5f;
		l2.y = (bb.pmax.y + bb.pmin.y) *0.5f;
		l1.z = (bb.pmax.z + bb.pmin.z) *0.5f;
		l2.z = (bb.pmax.z + bb.pmin.z) *0.5f;

		if (vecYLength > vecZLength)
			el1 = vecYLength;
		else 
			el1 =  vecZLength;
	}
	else if (axis ==1)
	{
		l1.y = bb.pmax.y;
		l2.y = bb.pmin.y;

		l1.x = (bb.pmax.x + bb.pmin.x) *0.5f;
		l2.x = (bb.pmax.x + bb.pmin.x) *0.5f;
		l1.z = (bb.pmax.z + bb.pmin.z) *0.5f;
		l2.z = (bb.pmax.z + bb.pmin.z) *0.5f;

		if (vecXLength > vecZLength)
			el1 = vecXLength;
		else 
			el1 = vecZLength;
	}
	else if (axis ==2)
	{
		l1.z = bb.pmax.z;
		l2.z = bb.pmin.z;

		l1.x = (bb.pmax.x + bb.pmin.x) *0.5f;
		l2.x = (bb.pmax.x + bb.pmin.x) *0.5f;
		l1.y = (bb.pmax.y + bb.pmin.y) *0.5f;
		l2.y = (bb.pmax.y + bb.pmin.y) *0.5f;

		if (vecXLength > vecYLength)
			el1 =  vecXLength;
		else 
			el1 =  vecYLength;
	}
}

Matrix3 BonesDefMod::CompMatrix(TimeValue t,INode *inode,ModContext *mc)
{
	Interval iv;
	Matrix3 tm(1);
	if (inode)
		tm = tm * inode->GetObjectTM(t,&iv);
	return tm;
}

void BonesDefMod::ClearVertexSelections(BoneModData *bmd)
{
	for (int i = 0; i <bmd->selected.GetSize(); i++)
	{
		bmd->selected.Set(i,FALSE);
	}
	EnableEffect(FALSE);
	ip->RedrawViews(ip->GetTime());

	//WEIGHTTABLE
	PaintAttribList();
}

void BonesDefMod::ClearBoneEndPointSelections()
{
	for (int i = 0; i <BoneData.Count(); i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node != NULL)
		{
			boneData.end1Selected = FALSE;
			boneData.end2Selected = FALSE;
		}
	}

	ip->RedrawViews(ip->GetTime());
}

void BonesDefMod::ClearEnvelopeSelections()
{
	for (int i = 0; i <BoneData.Count(); i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node != NULL)
		{
			for (int crossID = 0; crossID <boneData.CrossSectionList.Count(); crossID++)
			{
				boneData.CrossSectionList[crossID].innerSelected = FALSE;
				boneData.CrossSectionList[crossID].outerSelected = FALSE;
			}
		}
	}
	ModeBoneEnvelopeIndex = -1;
	ip->RedrawViews(ip->GetTime());
}

void BonesDefMod::SyncSelections()
{
	for (int i = 0; i < BoneData.Count();i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node != NULL)
		{
			boneData.end1Selected = FALSE;
			boneData.end2Selected = FALSE;

			for (int crossID=0;crossID<boneData.CrossSectionList.Count();crossID++)
			{
				boneData.CrossSectionList[crossID].innerSelected = FALSE;
				boneData.CrossSectionList[crossID].outerSelected = FALSE;
			}
		}
	}

	if (BoneData.Count() == 0)
	{
		ModeBoneIndex = -1;
	}

	if (IsValidBoneIndex(ModeBoneIndex))
	{
		BoneDataClass &boneData = BoneData[ModeBoneIndex];
		if (ModeBoneEnvelopeIndex == 0)
		{
			boneData.end1Selected = TRUE;
		}
		else if (ModeBoneEnvelopeIndex == 1)
		{
			boneData.end2Selected = TRUE;
		}
		if (ModeBoneEnvelopeIndex != -1)
		{
			if (ModeBoneEnvelopeSubType < 4)
				boneData.CrossSectionList[ModeBoneEnvelopeIndex].innerSelected = TRUE;
			else boneData.CrossSectionList[ModeBoneEnvelopeIndex].outerSelected = TRUE;
			EnableRadius(TRUE);
		}
		else
			EnableRadius(FALSE);
	}
	//update list box
	if (ModeBoneIndex != -1)
	{
		SelectBoneByBoneIndex(ModeBoneIndex);
	}
	//WEIGHTTABLE
	PaintAttribList();
}

int BonesDefMod::HitTest(
	TimeValue t, INode* inode,
	int type, int crossing, int flags,
	IPoint2 *p, ViewExp *vpt, ModContext* mc)
{
	if ( ! vpt || ! vpt->IsAlive() )
	{
		// why are we here
		DbgAssert(!_T("Invalid viewport!"));
		return FALSE;
	}

	pblock_param->GetValue(skin_filter_vertices,t,FilterVertices,FOREVER);
	pblock_param->GetValue(skin_filter_bones,t,FilterBones,FOREVER);
	pblock_param->GetValue(skin_filter_envelopes,t,FilterEnvelopes,FOREVER);

	FilterVertices = !FilterVertices;
	FilterBones = !FilterBones;
	FilterEnvelopes = !FilterEnvelopes;

	ModeEdit = 0;

	GraphicsWindow *gw = vpt->getGW();
	Point3 pt;
	HitRegion hr;
	int savedLimits, res = 0;
	Matrix3 tm = CompMatrix(t,inode,mc);

	BoneModData *bmd = (BoneModData *) mc->localData;

	if (bmd==NULL) return 0;
	if (BoneData.Count() == 0) return 0;

	BOOL backfaceCull = TRUE;
	BitArray visibleVerts;
	visibleVerts.SetSize(bmd->VertexData.Count());
	visibleVerts.SetAll();

	savedLimits = gw->getRndLimits();

	MakeHitRegion(hr,type, crossing,4,p);

	pblock_param->GetValue(skin_backfacecull,t,backfaceCull,FOREVER);
	if (backfaceCull )
	{
		visibleVerts.ClearAll();
		Matrix3 mat = inode->GetObjectTM(t);
		gw->setTransform(mat);
		gw->setRndLimits(((gw->getRndLimits()) | GW_PICK) & ~GW_ILLUM);
		gw->setRndLimits(gw->getRndLimits()|  GW_BACKCULL);

		SubObjHitList hitList;

		gw->setHitRegion(&hr);
		gw->clearHitCode();

		if (bmd->mesh)
		{
			SubObjHitList hitList;
			MeshSubHitRec *rec;
			for (int i = 0 ; i < bmd->VertexData.Count(); i++)
			{
				if (i < bmd->mesh->numVerts)
					bmd->mesh->verts[i] = bmd->VertexData[i]->LocalPosPostDeform;
			}
			BitArray hiddenFaces;
			hiddenFaces.SetSize( bmd->mesh->numFaces);
			hiddenFaces.ClearAll();
			for (int i = 0; i < bmd->mesh->numFaces; i++)
			{
				if (bmd->mesh->faces[i].flags & FACE_HIDDEN)
					hiddenFaces.Set(i,TRUE);
				bmd->mesh->faces[i].Show();
			}

			int res = bmd->mesh->SubObjectHitTest(gw, gw->getMaterial(), &hr,
				SUBHIT_FACES|SUBHIT_SELSOLID, hitList);
			rec = hitList.First();

			for (int i = 0; i < bmd->mesh->numFaces; i++)
			{
				if (hiddenFaces[i])
					bmd->mesh->faces[i].Hide();
			}

			while (rec)
			{
				int hitIndex = rec->index;
				DWORD *vv = bmd->mesh->faces[hitIndex].v;
				for (int j=0; j<3; j++)
					visibleVerts.Set(vv[j],true);
				rec = rec->Next();
			}
		}
		else if (bmd->mnMesh)
		{
			SubObjHitList hitList;
			MeshSubHitRec *rec;
			for (int i = 0 ; i < bmd->VertexData.Count(); i++)
			{
				if (i < bmd->mnMesh->numv)
					bmd->mnMesh->v[i].p = bmd->VertexData[i]->LocalPosPostDeform;
			}

			BitArray hiddenFaces;
			hiddenFaces.SetSize( bmd->mnMesh->numf);
			hiddenFaces.ClearAll();
			for (int i = 0; i < bmd->mnMesh->numf; i++)
			{
				if (bmd->mnMesh->f[i].GetFlag(MN_HIDDEN))
					hiddenFaces.Set(i,TRUE);
				bmd->mnMesh->f[i].ClearFlag(MN_HIDDEN);
			}

			int res = bmd->mnMesh->SubObjectHitTest(gw, gw->getMaterial(), &hr,
				SUBHIT_MNFACES|SUBHIT_SELSOLID, hitList);

			for (int i = 0; i < bmd->mnMesh->numf; i++)
			{
				if (hiddenFaces[i])
					bmd->mnMesh->f[i].SetFlag(MN_HIDDEN);
			}

			rec = hitList.First();
			while (rec)
			{
				int hitIndex = rec->index;
				int deg = bmd->mnMesh->f[hitIndex].deg;
				for (int j=0; j<deg; j++)
					visibleVerts.Set(bmd->mnMesh->f[hitIndex].vtx[j],true);
				rec = rec->Next();
			}
		}
		else if (bmd->patch)
		{
			HitRegion testhr;

			testhr.type = RECT_RGN;
			testhr.crossing  = true;
			int w = gw->getWinSizeX();
			int h = gw->getWinSizeY();
			testhr.rect.left = 0;
			testhr.rect.top = 0;
			testhr.rect.right = w;
			testhr.rect.bottom = h;

			SubPatchHitList hitList;

			BitArray hiddenFaces;
			hiddenFaces.SetSize( bmd->patch->numPatches);
			hiddenFaces.ClearAll();
			for (int i = 0; i < bmd->patch->numPatches; i++)
			{
				if (bmd->patch->patches[i].IsHidden())
					hiddenFaces.Set(i,TRUE);
				bmd->patch->patches[i].SetHidden(FALSE);
			}

			int res = bmd->patch->SubObjectHitTest(gw, gw->getMaterial(), &testhr,
				SUBHIT_PATCH_PATCHES|SUBHIT_SELSOLID|SUBHIT_PATCH_IGNORE_BACKFACING, hitList);

			for (int i = 0; i < bmd->patch->numPatches; i++)
			{
				if (hiddenFaces[i])
					bmd->patch->patches[i].SetHidden(TRUE);
			}

			PatchSubHitRec *rec = hitList.First();

			while (rec)
			{
				int hitIndex = rec->index;
				int deg = 4;
				if (bmd->patch->patches[hitIndex].type == PATCH_TRI)
					deg = 3;
				for (int j=0; j<deg; j++)
				{
					visibleVerts.Set(bmd->patch->patches[hitIndex].v[j],true);
					visibleVerts.Set(bmd->patch->patches[hitIndex].interior[j] + bmd->patch->numVerts,true);
					visibleVerts.Set(bmd->patch->patches[hitIndex].vec[j*2] + bmd->patch->numVerts,true);
					visibleVerts.Set(bmd->patch->patches[hitIndex].vec[j*2+1] + bmd->patch->numVerts,true);
				}
				rec = rec->Next();
			}
		}
		else
		{
			visibleVerts.SetAll();
		}
	}

	BOOL showHiddenVerts;
	pblock_display->GetValue(skin_display_showhiddenvertices,t,showHiddenVerts,FOREVER);
	if (!showHiddenVerts)
	{
		for (int i = 0; i < bmd->VertexData.Count(); i++)
		{
			if (bmd->VertexData[i]->IsHidden())
				visibleVerts.Set(i,FALSE);
		}
	}

	gw->setHitRegion(&hr);
	gw->setRndLimits((savedLimits  | GW_PICK) & ~GW_ILLUM);
	gw->setTransform(tm);

	// Hit test start point
	// loop through points checking for selection
	// get selected bone
	int fsel = SendMessage(GetDlgItem(hParam,IDC_LIST1),LB_GETCURSEL,0,0);
	int boneIndex = ConvertListIDToBoneIndex(fsel);

	if (bmd->tmCacheToObjectSpace.Count() != BoneData.Count()) return 0;

	BOOL vertHit = FALSE;

	if ( (boneIndex >= 0) && (ip && ip->GetSubObjectLevel() == 1) )
	{
		res = 1;
		ObjectState os;

		os = inode->EvalWorldState(t);
		//loop through points checking for selection and then marking for points
		//bug fix 276830 Jan 29 2001
		BOOL difTopology = FALSE;
		if (os.obj->NumPoints() != bmd->VertexData.Count())
			difTopology = TRUE;

		//hittest gizmos if they are editing them
		GizmoClass *gizmo = NULL;
		if ( (pblock_gizmos->Count(skin_gizmos_list) > 0) && (currentSelectedGizmo<(pblock_gizmos->Count(skin_gizmos_list)) ) )
		{
			ReferenceTarget *ref;
			ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
			gizmo = (GizmoClass *)ref;
		}
		if ( (gizmo) && (gizmo->IsEditing()))
		{
			int iret =  gizmo->HitTest(t,inode, type, crossing, flags, p, vpt,mc, Inverse(tm));
			gw->setRndLimits(savedLimits);
			return iret;
		}
		else if (FilterVertices == 0)
		{
			res =0;
			Interval iv;
			Matrix3 atm = inode->GetObjTMAfterWSM(t,&iv);
			Matrix3 ctm = inode->GetObjectTM(t,&iv);

			Matrix3 vtm;
			vpt->GetAffineTM(vtm);
			vtm = ctm *vtm;

			BOOL isWorldSpace = FALSE;

			if ((atm.IsIdentity()) && (ip->GetShowEndResult ()))
				isWorldSpace = TRUE;

			if (bmd->autoInteriorVerts.GetSize() != bmd->VertexData.Count())
			{
				bmd->autoInteriorVerts.SetSize(bmd->VertexData.Count());
				bmd->autoInteriorVerts.ClearAll();
			}

			Tab<int> hitID;
			Tab<Point3> zDist;
			int i;
			for (i=0;i<bmd->VertexData.Count();i++)
			{
				if (!bmd->autoInteriorVerts[i])
				{
					Point3 pt;
					gw->clearHitCode();

					if (difTopology)
						pt = bmd->VertexData[i]->LocalPos;
					else pt = os.obj->GetPoint(i) ;

					if (isWorldSpace)
						pt = pt * Inverse(ctm);

					gw->setColor(LINE_COLOR, 1.0f,1.0f,1.0f);
					gw->marker(&pt,POINT_MRKR);
					if (gw->checkHitCode() && visibleVerts[i])
					{
						if ((flags&HIT_SELONLY   &&  bmd->selected[i]) ||
							(flags&HIT_UNSELONLY && !bmd->selected[i]) ||
							!(flags&(HIT_UNSELONLY|HIT_SELONLY)) )
						{
							if (type == HITTYPE_POINT)
							{
								hitID.Append(1,&i,100);
								DWORD z = gw->getHitDistance();
								pt = pt * vtm ;
								zDist.Append(1,&pt,100);
							}
							else
							{
								vpt->LogHit(inode, mc, gw->getHitDistance(), i,
									new BoneHitDataClass(i,-1,-1,-1,-1));
								res = 1;
							}
						}
						vertHit = TRUE;
					}
				}
			}

			if (type == HITTYPE_POINT)
			{
				int id = -1;
				float z = 0.0f;
				for (int j = 0; j < hitID.Count(); j++)
				{
					if ((id == -1) || (zDist[j].z > z))
					{
						id = hitID[j];
						z = zDist[j].z;
					}
				}
				if (id != -1)
				{
					vpt->LogHit(inode, mc, z, id,
						new BoneHitDataClass(i,-1,-1,-1,-1));
					res = 1;
				}
			}
			//vertices take priority if selected select nothing else
			if (res)
			{
				gw->setRndLimits(savedLimits);
				return res;
			}
		}

		int ct = bmd->VertexData.Count();

		BOOL showEnvelopes;
		pblock_display->GetValue(skin_display_shownoenvelopes,0,showEnvelopes,FOREVER);
		showEnvelopes = !showEnvelopes;

		if ((!vertHit) &&(type != HITTYPE_CIRCLE))
		{
			for (int i =0;i<BoneData.Count();i++)
			{
				BoneDataClass &boneData = BoneData[i];
				if (boneData.Node != NULL)
				{
					//add in envelopes start and end
					ObjectState os;
					ShapeObject *pathOb = NULL;

					if (FilterBones == 0)
					{
						Point3 pta,ptb;

						GetEndPointsLocal(bmd, t,pta, ptb, i);

						Point3 midPt = (pta+ptb) * 0.5f;
						// hit test start half bone.
						if ((flags&HIT_SELONLY   &&  boneData.end1Selected) ||
							(flags&HIT_UNSELONLY && !boneData.end1Selected) ||
							!(flags&(HIT_UNSELONLY|HIT_SELONLY)) )
						{
							gw->clearHitCode();
							gw->setColor(LINE_COLOR, 1.0f,1.0f,1.0f);
							Point3 pt;
							Interval v;

							Point3 invA;
							invA = (ptb-pta) *.1f;

							pta += invA;

							gw->marker(&pta,POINT_MRKR);
							Point3 lp[3];
							lp[0] = pta;
							lp[1] = midPt;
							gw->polyline(2, lp, NULL, NULL, 0);

							if (gw->checkHitCode())
							{
								vpt->LogHit(inode, mc, gw->getHitDistance(), ct,
									new BoneHitDataClass(-1,i,0,-1,-1));
								res = 1;
							}
						}

						// hit test end half bone.
						if ((flags&HIT_SELONLY   &&  boneData.end2Selected) ||
							(flags&HIT_UNSELONLY && !boneData.end2Selected) ||
							!(flags&(HIT_UNSELONLY|HIT_SELONLY)) )
						{
							gw->clearHitCode();
							gw->setColor(LINE_COLOR, 1.0f,1.0f,1.0f);
							Point3 pt;
							Interval v;

							Point3 invB;
							invB = (pta-ptb) *.1f;

							ptb += invB;

							gw->marker(&ptb,POINT_MRKR);

							Point3 lp[3];
							lp[0] = ptb;
							lp[1] = midPt;
							gw->polyline(2, lp, NULL, NULL, 0);

							if (gw->checkHitCode()) {
								vpt->LogHit(inode, mc, gw->getHitDistance(), ct,
									new BoneHitDataClass(-1,i,1,-1,-1));
								res = 1;
							}
						}
					}

					//add in envelope inner and outer
					if ((FilterEnvelopes == 0) && (i == ModeBoneIndex) && (showEnvelopes))
					{
						if (boneData.flags & BONE_SPLINE_FLAG)
						{
							ObjectState os = boneData.Node->EvalWorldState(t);
							pathOb = (ShapeObject*)os.obj;
						}
						Point3 l1,l2;
						Interval v;

						GetEndPointsLocal(bmd, t,l1, l2, i);

						Interval valid;
						Matrix3 ntm = boneData.Node->GetObjTMBeforeWSM(t,&valid);

						Point3 align = (l2-l1);
						Point3 nvec = align;

						for (int crossID=0;crossID<boneData.CrossSectionList.Count();crossID++)
						{
							Point3 sectionPts[32];
							Point3 ept(0.0f,0.0f,0.0f);
							Point3 vec;

							if ((pathOb) && (pathOb->NumberOfCurves() != 0) &&(boneData.flags & BONE_SPLINE_FLAG))
							{
								ept = pathOb->InterpCurve3D(t, 0,boneData.CrossSectionList[crossID].u) * ntm  * bmd->InverseBaseTM;
								align = VectorTransform(ntm  * bmd->InverseBaseTM,pathOb->TangentCurve3D(t, 0,boneData.CrossSectionList[crossID].u));
								float Inner,Outer;
								boneData.CrossSectionList[crossID].InnerControl->GetValue(currentTime,&Inner,v);
								boneData.CrossSectionList[crossID].OuterControl->GetValue(currentTime,&Outer,v);
								GetCrossSection(ept, align, Inner,boneData.temptm, sectionPts, 16);
								GetCrossSection(ept, align, Outer,boneData.temptm, &sectionPts[16], 16);
							}
							else
							{
								align = (l2-l1);
								ept = l1;
								float Inner,Outer;
								boneData.CrossSectionList[crossID].InnerControl->GetValue(currentTime,&Inner,v);
								boneData.CrossSectionList[crossID].OuterControl->GetValue(currentTime,&Outer,v);

								ept = ept + nvec * boneData.CrossSectionList[crossID].u;
								GetCrossSection(ept, align, Inner,boneData.temptm, sectionPts, 16);
								GetCrossSection(ept, align, Outer,boneData.temptm, &sectionPts[16], 16);
							}

							for (int m=0;m<4;++m) // hit test inner cross section.
							{
								if ((flags&HIT_SELONLY   &&  boneData.CrossSectionList[crossID].innerSelected) ||
									(flags&HIT_UNSELONLY && !boneData.CrossSectionList[crossID].innerSelected) ||
									!(flags&(HIT_UNSELONLY|HIT_SELONLY)) )
								{
									gw->clearHitCode();
									gw->setColor(LINE_COLOR, 1.0f,1.0f,1.0f);

									gw->marker(&sectionPts[m*4 + 3],POINT_MRKR);

									// if user click the fan round the marker, also select the cross section.
									Point3 lp[6];
									lp[0] = sectionPts[(m*4 + 1)];
									lp[1] = sectionPts[(m*4 + 2)];
									lp[2] = sectionPts[(m*4 + 3)];
									lp[3] = sectionPts[(m*4 + 4)%16]; //possible larger than 16, so mod 16.
									lp[4] = sectionPts[(m*4 + 5)%16];
									gw->polyline(5, lp, NULL, NULL, 0);

									if (gw->checkHitCode())
									{
										vpt->LogHit(inode, mc, gw->getHitDistance(), ct,
											new BoneHitDataClass(-1,i,-1,crossID,m));
										res = 1;
									}
								}
							}

							for (int m=4;m<8;++m) // hit test outer cross section.
							{
								if ((flags&HIT_SELONLY   &&  boneData.CrossSectionList[crossID].outerSelected) ||
									(flags&HIT_UNSELONLY && !boneData.CrossSectionList[crossID].outerSelected) ||
									!(flags&(HIT_UNSELONLY|HIT_SELONLY)) )
								{
									gw->clearHitCode();
									gw->setColor(LINE_COLOR, 1.0f,1.0f,1.0f);

									gw->marker(&sectionPts[m*4 + 3],POINT_MRKR);

									// if user click the circle round the marker, also select the cross section.
									Point3 lp[6];
									lp[0] = sectionPts[(m*4 + 1)];
									lp[1] = sectionPts[(m*4 + 2)];
									lp[2] = sectionPts[(m*4 + 3)];
									lp[3] = sectionPts[(m*4 + 4)%16 + 16]; //possible larger than 32, make sure it is between 16 and 31.
									lp[4] = sectionPts[(m*4 + 5)%16 + 16];
									gw->polyline(5, lp, NULL, NULL, 0);

									if (gw->checkHitCode())
									{
										vpt->LogHit(inode, mc, gw->getHitDistance(), ct,
											new BoneHitDataClass(-1,i,-1,crossID,m));
										res = 1;
									}
								}
							}
						}
					}
				}
			}
		}
	}

	gw->setRndLimits(savedLimits);
	return res;
}

void BonesDefMod::ZoomToBone(BOOL all)
{
	Box3 box;
	if (ip)
	{
		Point3 pt1, pt2;
		box.Init();

		float l = 0.0f;
		if (IsValidBoneIndex(ModeBoneIndex))
		{
			BoneDataClass &boneData = BoneData[ModeBoneIndex];
			TimeValue t = ip->GetTime();
			Interval valid;
			Matrix3 ntm = boneData.Node->GetObjTMBeforeWSM(t,&valid);

			Point3 pta;
			Interval v;
			boneData.EndPoint1Control->GetValue(currentTime,&pta,v);

			box += pta* ntm;
			boneData.EndPoint2Control->GetValue(currentTime,&pta,v);
			box += pta* ntm;
			for (int k = 0; k < boneData.CrossSectionList.Count();k++)
			{
				float outer;
				Interval v;
				boneData.CrossSectionList[k].OuterControl->GetValue(currentTime,&outer,v);

				if (outer > l) l = outer;
			}
			box.EnlargeBy(l+2.0f);  // this is a fudge since I am using large tick boxes

			ip->ZoomToBounds(all,box);
		}
	}
}

void BonesDefMod::ZoomToGizmo(BoneModData *bmd, BOOL all)
{
	Box3 box;
	if (ip)
	{
		Point3 pt1, pt2;
		box.Init();

		float l = 0.0f;
		int i = currentSelectedGizmo;
		if ( (i != -1) && (i < bmd->gizmoData.Count() ))
		{
			ip->ZoomToBounds(all,currentGizmoBounds);
		}
	}
}

void BonesDefMod::GetWorldBoundBox(	TimeValue t,INode* inode, ViewExp *vpt, Box3& box, ModContext *mc)
{
	if ( ! vpt || ! vpt->IsAlive() )
	{
		box.Init();
		return;
	}

	BoneModData *bmd = (BoneModData *) mc->localData;

	if (!bmd) return ;

	if ( ip && (ip->GetExtendedDisplayMode()& EXT_DISP_ZOOMSEL_EXT) )
	{
		return;
	}

	//MIRROR
	if (mirrorData.Enabled())
	{
		box.Init();
		box += mirrorData.GetMirrorBounds();
		box = box * inode->GetObjTMBeforeWSM(t,&FOREVER);

		for (int i = 0; i < BoneData.Count(); i++)
		{
			BoneDataClass &boneData = BoneData[i];
			INode *node = boneData.Node;
			if (node)
			{
				Point3 pa,pb;
				boneData.EndPoint1Control->GetValue(t,&pa,FOREVER);
				boneData.EndPoint2Control->GetValue(t,&pb,FOREVER);

				Matrix3 tm = node->GetObjectTM(t);
				box +=  pa * tm;
				box +=  pb * tm;
			}
		}

		mirrorData.worldBounds = box;
		return;
	}

	box.Init();

	float l = 0.0f;
	for (int i =0;i<BoneData.Count();i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node != NULL)
		{
			Interval valid;
			Matrix3 ntm = boneData.Node->GetObjTMBeforeWSM(t,&valid);

			Point3 pta;
			Interval v;
			boneData.EndPoint1Control->GetValue(currentTime,&pta,v);

			box += pta* ntm;
			boneData.EndPoint2Control->GetValue(currentTime,&pta,v);
			box += pta* ntm;
			for (int k = 0; k < boneData.CrossSectionList.Count();k++)
			{
				float outer;
				Interval v;
				boneData.CrossSectionList[k].OuterControl->GetValue(currentTime,&outer,v);
				if (stopMessagePropogation)
				{
					if (outer > l) l = (outer*4.0f);
				}
				else if (outer > l) l = (outer);
			}
		}
	}
	box.EnlargeBy(l*1.75f);  // this is a fudge since this  a bounds box and the real volume is a sphere

	for(int  j = 0 ; j < bmd->gizmoData.Count() ; j++)
	{
		int id = bmd->gizmoData[j]->whichGizmo;
		int gizmoCount = pblock_gizmos->Count(skin_gizmos_list);
		ReferenceTarget *ref;
		if (id < gizmoCount)
		{
			ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,id);
			if (ref)
			{
				GizmoClass *gizmo = (GizmoClass *)ref;
				Box3 b;
				b.Init();
				if (gizmo)
				{
					gizmo->GetWorldBoundBox(t,inode, vpt, b, mc);
					if (j==currentSelectedGizmo)
						currentGizmoBounds = b;
					box += b;
				}
			}
		}
	}

	if (pPainterInterface && pPainterInterface->InPaintMode())
	{
		float *radiusList = pPainterInterface->GetStrokeRadius();
		Point3 *worldHitList = pPainterInterface->GetStrokePointWorld();
		int hitCt = pPainterInterface->GetStrokeCount();
		float r = 0.0f;
		for ( int i = 0; i < hitCt; i++)
		{
			box += worldHitList[i];
			if ( radiusList[i] > r) r = radiusList[i];
		}
		box.EnlargeBy(r);
	}
}

void BonesDefMod::LimitInnerRadius(float outer)
{
	float innerRadius;
	BoneDataClass &boneData = BoneData[ModeBoneIndex];
	boneData.CrossSectionList[ModeBoneEnvelopeIndex].InnerControl->GetValue(currentTime,&innerRadius,FOREVER);
	if (innerRadius > outer)
	{
		innerRadius = outer - 5.0f;
		if (innerRadius < 0.0f) innerRadius  = 0.0f;

		BOOL animate = FALSE;
		pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);

		if (!animate)
		{
			SuspendAnimate();
			AnimateOff();
		}

		boneData.CrossSectionList[ModeBoneEnvelopeIndex].InnerControl->SetValue(currentTime,&innerRadius,TRUE,CTRL_ABSOLUTE);

		if (!animate)
			ResumeAnimate();
	}
}

void BonesDefMod::LimitOuterRadius(float inner)
{
	float outerRadius;
	BoneDataClass &boneData = BoneData[ModeBoneIndex];
	boneData.CrossSectionList[ModeBoneEnvelopeIndex].OuterControl->GetValue(currentTime,&outerRadius,FOREVER);
	if (outerRadius < inner)
	{
		outerRadius = inner + 5.0f;

		BOOL animate = FALSE;
		pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);

		if (!animate)
		{
			SuspendAnimate();
			AnimateOff();
		}

		boneData.CrossSectionList[ModeBoneEnvelopeIndex].OuterControl->SetValue(currentTime,&outerRadius,TRUE,CTRL_ABSOLUTE);

		if (!animate)
			ResumeAnimate();
	}
}

void BonesDefMod::TransformStart(TimeValue t)
{
	if (updateOnMouseUp)
		stopMessagePropogation = TRUE;
}

void BonesDefMod::TransformHoldingStart(TimeValue t)
{
	theHold.Put( new SelectBoneMoveRestore(this));
}

void BonesDefMod::TransformFinish(TimeValue t)
{
	stopMessagePropogation = FALSE;
	if (updateOnMouseUp)
	{
		NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);
		ip->ForceCompleteRedraw();
	}
	UpdateWeightToolVertexStatus();
}

void BonesDefMod::TransformCancel(TimeValue t)
{
	stopMessagePropogation = FALSE;
}

void BonesDefMod::Move(TimeValue t, Matrix3& partm, Matrix3& tmAxis,
					   Point3& val, BOOL localOrigin)
{
	//check of points
	//check for envelopes

	if ( !ip ) return;

	//MIRROR
	if (mirrorData.Enabled()) return;

	//hittest gizmos if they are editing them
	GizmoClass *gizmo = NULL;
	if ( (pblock_gizmos->Count(skin_gizmos_list) > 0) && (currentSelectedGizmo<(pblock_gizmos->Count(skin_gizmos_list)) ) )
	{
		ReferenceTarget *ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
		gizmo = (GizmoClass *)ref;
	}
	if ( (gizmo) && (gizmo->IsEditing()))
	{
		gizmo->Move(t, partm, tmAxis, val, localOrigin);
		return;
	}

	ModContextList mcList;
	INodeTab nodes;
	ip->GetModContexts(mcList,nodes);
	int objects = mcList.Count();

	BoneModData* bmd = NULL;
	for ( int i = 0; i < objects; i++ )
	{
		BoneModData* tbmd = (BoneModData*)mcList[i]->localData;

		int mode = 1;
		if (mode > 0 )
		{
			if (ModeBoneEndPoint == 0)
			{
				tbmd->CurrentCachePiece = -1;
			}
			else if (ModeBoneEndPoint == 1)
			{
				tbmd->CurrentCachePiece = -1;
			}
		}
		if (updateOnMouseUp)
		{
			INode* node = nodes[i];
			if (node)
			{
				node->InvalidateRect(t);
			}
		}

		if (nodes[i]->Selected())
		{
			bmd = tbmd;
		}
	}

	if (bmd == NULL)
		bmd = (BoneModData*)mcList[0]->localData;

	int mode = 1;
	if (mode >0 )
	{
		ModeEdit = 1;
		val = VectorTransform(tmAxis*Inverse(bmd->baseNodeOffsetTM),val);

		if (ModeBoneEndPoint == 0)
		{
			BoneDataClass& boneData = BoneData[ModeBoneIndex];
			val = VectorTransform(bmd->tmCacheToBoneSpace[ModeBoneIndex],val);

			BOOL animate = FALSE;
			pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);

			if (!animate)
			{
				SuspendAnimate();
				AnimateOff();
			}

			boneData.EndPoint1Control->SetValue(currentTime,&val,TRUE,CTRL_RELATIVE);

			if (!animate)
				ResumeAnimate();

			int tempID = ConvertBoneIndexToBoneID(ModeBoneIndex);
			Interval iv;
			boneData.EndPoint1Control->GetValue(currentTime,&val,iv,CTRL_ABSOLUTE);
			macroRecorder->FunctionCall(_T("skinOps.setStartPoint"), 3, 0, mr_reftarg, this, mr_int, tempID+1, mr_point3, &val);
		}
		else if (ModeBoneEndPoint == 1)
		{
			BoneDataClass& boneData = BoneData[ModeBoneIndex];
			val = VectorTransform(bmd->tmCacheToBoneSpace[ModeBoneIndex],val);

			BOOL animate = FALSE;
			pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);

			if (!animate)
			{
				SuspendAnimate();
				AnimateOff();
			}

			boneData.EndPoint2Control->SetValue(currentTime,&val,TRUE,CTRL_RELATIVE);

			if (!animate)
				ResumeAnimate();

			int tempID = ConvertBoneIndexToBoneID(ModeBoneIndex);
			Interval iv;
			boneData.EndPoint2Control->GetValue(currentTime,&val,iv,CTRL_ABSOLUTE);
			macroRecorder->FunctionCall(_T("skinOps.setEndPoint"), 3, 0, mr_reftarg, this, mr_int, tempID+1, mr_point3, &val);
		}
		else
		{
			if ((ModeBoneEnvelopeIndex != -1) && (ModeBoneEnvelopeSubType != -1))
			{
				BoneDataClass& boneData = BoneData[ModeBoneIndex];
				val = VectorTransform(bmd->tmCacheToBoneSpace[ModeBoneIndex],val);

				BOOL animate = FALSE;
				pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);

				if (!animate)
				{
					SuspendAnimate();
					AnimateOff();
				}

				p1Temp->SetValue(0,val,TRUE,CTRL_RELATIVE);

				if (!animate)
					ResumeAnimate();

				Interval v;

				ObjectState os;
				ShapeObject *pathOb = NULL;
				Point3 nvec;
				Point3 vec;
				Point3 lp;
				Point3 l1,l2;
				boneData.EndPoint1Control->GetValue(currentTime,&l1,v);
				boneData.EndPoint2Control->GetValue(currentTime,&l2,v);

				Point3 p(0.0f,0.0f,0.0f);
				Interval iv = FOREVER;
				p1Temp->GetValue(0,&p,iv);

				if (boneData.flags & BONE_SPLINE_FLAG)
				{
					ObjectState os = boneData.Node->EvalWorldState(t);
					pathOb = (ShapeObject*)os.obj;
					if (pathOb->NumberOfCurves() != 0)
						lp = pathOb->InterpCurve3D(t, 0,boneData.CrossSectionList[ModeBoneEnvelopeIndex].u);
				}
				else
				{
					nvec = l2-l1;
					lp = l1 + nvec * boneData.CrossSectionList[ModeBoneEnvelopeIndex].u;
				}

				if (ModeBoneEnvelopeSubType<4)
				{
					//10-9-00 new method used to compute the distance constrains the manip axis
					float angle;
					Point3 vecA,vecB;
					vecA = localVec-localStart;
					vecB = p-localStart;
					float dot = DotProd(Normalize(vecA),Normalize(vecB));
					angle = acos(dot);

					vecB = Inverse(bmd->tmCacheToBoneSpace[ModeBoneIndex]).VectorTransform(vecB);

					float inner;
					if (dot == 1.0f)
						inner = Length(vecB) ;
					else inner = Length(vecB) * cos(angle);

					if (inner < 0.0f) inner = 0.0005f;

					BOOL animate= FALSE;
					pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);
					if (!animate)
					{
						SuspendAnimate();
						AnimateOff();
					}
					boneData.CrossSectionList[ModeBoneEnvelopeIndex].InnerControl->SetValue(currentTime,&inner,TRUE,CTRL_ABSOLUTE);
					if (!animate)
						ResumeAnimate();

					macroRecorder->Disable();
					pblock_param->SetValue(skin_cross_radius,0,inner);
					macroRecorder->Enable();

					LimitOuterRadius(inner);

					int tempID = ConvertBoneIndexToBoneID(ModeBoneIndex);
					macroRecorder->FunctionCall(_T("skinOps.setInnerRadius"), 4, 0, mr_reftarg, this, mr_int, tempID+1, mr_int, ModeBoneEnvelopeIndex+1, mr_float, inner);
				}
				else if (ModeBoneEnvelopeSubType<8)
				{
					//10-9-00 new method used to compute the distance constrains the manip axis
					float angle;
					Point3 vecA,vecB;
					vecA = localVec-localStart;
					vecB = p-localStart;
					float dot = DotProd(Normalize(vecA),Normalize(vecB));
					angle = acos(dot);

					vecB = Inverse(bmd->tmCacheToBoneSpace[ModeBoneIndex]).VectorTransform(vecB);

					float outer;
					if (dot == 1.0f)
						outer = Length(vecB) ;
					else
						outer = Length(vecB) * cos(angle);

					if (outer < 0.0f)
						outer = 0.001f;

					BOOL animate = FALSE;
					pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);
					if (!animate)
					{
						SuspendAnimate();
						AnimateOff();
					}
					boneData.CrossSectionList[ModeBoneEnvelopeIndex].OuterControl->SetValue(currentTime,&outer,TRUE,CTRL_ABSOLUTE);
					if (!animate)
						ResumeAnimate();

					macroRecorder->Disable();
					pblock_param->SetValue(skin_cross_radius,0,outer);
					macroRecorder->Enable();

					LimitInnerRadius(outer);

					int tempID = ConvertBoneIndexToBoneID(ModeBoneIndex);
					macroRecorder->FunctionCall(_T("skinOps.setOuterRadius"), 4, 0, mr_reftarg, this, mr_int, tempID+1, mr_int, ModeBoneEnvelopeIndex+1, mr_float, outer);
				}
			}
		}
		//move the right controller
	}

	if (updateOnMouseUp)
	{
		ip->RedrawViews(t);
	}
}

void BonesDefMod::GetSubObjectCenters(
	SubObjAxisCallback *cb,TimeValue t,
	INode *node,ModContext *mc)
{
	BoneModData *bmd = (BoneModData *) mc->localData;

	if (!bmd) return;

	if (bmd->tmCacheToObjectSpace.Count() != BoneData.Count()) return;

	Matrix3 tm = CompMatrix(t,node,mc);
	Point3 pt(0,0,0), p;

	//hittest gizmos if they are editing them
	GizmoClass *gizmo = NULL;
	if ( (pblock_gizmos->Count(skin_gizmos_list) > 0) && (currentSelectedGizmo<(pblock_gizmos->Count(skin_gizmos_list)) ) )
	{
		ReferenceTarget *ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
		gizmo = (GizmoClass *)ref;
	}
	if ( (gizmo) && (gizmo->IsEditing()))
	{
		gizmo->GetSubObjectCenters(cb,t,node, Inverse(tm));
		return;
	}

	if (!IsValidBoneIndex(ModeBoneIndex, false))
		return;
	if (BoneData[ModeBoneIndex].Node == NULL) //something has gone wrong we should not be able to get to this point assert and fix
	{
		for (int i = 0; i < BoneData.Count(); i++)
		{
			if (BoneData[i].Node)
			{
				ModeBoneIndex = i;
				break;
			}
		}
		return;
	}

	if (cb->Type() == SO_CENTER_SELECTION)
	{
		MyEnumProc dep;
		DoEnumDependents(&dep);
		//this puts back the original state of the node vc mods and shade state
		int nodeCount = dep.Nodes.Count();
		int ct = 0;
		int numSelected = 0;
		Point3 center(0.0f,0.0f,0.0f);
		for (int  i = 0; i < nodeCount; i++)
		{
			if (dep.Nodes[i])
			{
				Matrix3 tm = dep.Nodes[i]->GetObjectTM(t);
				BoneModData *bmd = GetBMD(dep.Nodes[i]);

				if (bmd)
				{
					numSelected += bmd->selected.NumberSet() ;
					if (bmd->selected.NumberSet() > 0)
					{
						ct++;
						Box3 bounds;
						bounds.Init();

						for (int i = 0; i < bmd->VertexData.Count(); i++)
						{
							if (bmd->selected[i])
								bounds += bmd->VertexData[i]->LocalPosPostDeform;
						}
						Point3 c = bounds.Center() * tm;
						center += c;
					}
				}
			}
		}
		if (ct && numSelected)
		{
			cb->Center(center/(float)ct,0);
			return;
		}
	}

	if (ModeBoneEndPoint == 0)
	{
		BoneDataClass& boneData = BoneData[ModeBoneIndex];
		Interval iv;
		if (boneData.EndPoint1Control == NULL)
			DbgAssert(1);
		else boneData.EndPoint1Control->GetValue(currentTime,&bmd->localCenter,iv,CTRL_ABSOLUTE);
		bmd->localCenter = bmd->localCenter *bmd->tmCacheToObjectSpace[ModeBoneIndex];
	}
	else if (ModeBoneEndPoint == 1)
	{
		BoneDataClass& boneData = BoneData[ModeBoneIndex];
		Interval iv;
		if (boneData.EndPoint2Control == NULL)
			DbgAssert(1);
		else boneData.EndPoint2Control->GetValue(currentTime,&bmd->localCenter,iv,CTRL_ABSOLUTE);

		bmd->localCenter = bmd->localCenter *bmd->tmCacheToObjectSpace[ModeBoneIndex];
	}
	else if ((ModeBoneEnvelopeIndex != -1) && (ModeBoneEnvelopeSubType != -1))
	{
		Interval v;
		Point3 p;
		p1Temp->GetValue(0,p,v,CTRL_ABSOLUTE);
		float angle;
		Point3 vecA,vecB;

		vecA = localVec-localStart;
		vecB = p-localStart;
		float dot = DotProd(Normalize(vecA),Normalize(vecB));
		angle = acos(dot);

		float dist;
		if (dot == 1.0f)
			dist = Length(vecB) ;
		else dist = Length(vecB) * cos(angle);

		if (dist < 0.0f) dist = 0.0f;

		bmd->localCenter = localStart+(Normalize(vecA) * dist);

		bmd->localCenter = bmd->localCenter *bmd->tmCacheToObjectSpace[ModeBoneIndex];
	}
	else
	{
		BoneDataClass& boneData = BoneData[ModeBoneIndex];
		if (boneData.Node)
		{
			Interval iv;
			Point3 a,b;
			if (boneData.EndPoint1Control)
				boneData.EndPoint1Control->GetValue(currentTime,&a,iv,CTRL_ABSOLUTE);
			bmd->localCenter = a *bmd->tmCacheToObjectSpace[ModeBoneIndex];

			if (boneData.EndPoint2Control)
				boneData.EndPoint2Control->GetValue(currentTime,&b,iv,CTRL_ABSOLUTE);
			bmd->localCenter += b *bmd->tmCacheToObjectSpace[ModeBoneIndex];
			bmd->localCenter *= 0.05f;
		}
	}

	pt = bmd->localCenter;

	tm.PreTranslate(pt);
	cb->Center(tm.GetTrans(),0);
}

void BonesDefMod::GetSubObjectTMs(
	SubObjAxisCallback *cb,TimeValue t,
	INode *node,ModContext *mc)
{
	//hittest gizmos if they are editing them
	Matrix3 tm = CompMatrix(t,node,mc);

	GizmoClass *gizmo = NULL;
	if ( (pblock_gizmos->Count(skin_gizmos_list) > 0) && (currentSelectedGizmo<(pblock_gizmos->Count(skin_gizmos_list)) ) )
	{
		ReferenceTarget *ref;

		ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
		gizmo = (GizmoClass *)ref;
	}
	if ( (gizmo) && (gizmo->IsEditing()))
	{
		gizmo->GetSubObjectTMs(cb,t,node,Inverse(tm));
		return;
	}

	BoneModData *bmd = (BoneModData *) mc->localData;

	if (!bmd) return;

	if (bmd->tmCacheToObjectSpace.Count() != BoneData.Count()) return;

	if (IsValidBoneIndex(ModeBoneIndex))
	{
		INode *boneNode = BoneData[ModeBoneIndex].Node;
		tm = boneNode->GetNodeTM(t);
	}

	Point3 pt(0.0f,0.0f,0.0f);
	Interval iv;
	Point3 center;
	BOOL useLocalAxis = TRUE;

	if (ModeBoneEndPoint == 0)
	{
	if (IsValidBoneIndex(ModeBoneIndex))
		BoneData[ModeBoneIndex].EndPoint1Control->GetValue(currentTime,&center,iv,CTRL_ABSOLUTE);
	}
	else if (ModeBoneEndPoint == 1)
	{
		if (IsValidBoneIndex(ModeBoneIndex))
			BoneData[ModeBoneIndex].EndPoint2Control->GetValue(currentTime,&center,iv,CTRL_ABSOLUTE);
	}
	else if ((ModeBoneEnvelopeIndex != -1) && (ModeBoneEnvelopeSubType != -1))
	{
		Interval v;
		Point3 p;
		p1Temp->GetValue(0,p,v,CTRL_ABSOLUTE);
		Point3 vecA,vecB;

		vecA = localVec-localStart;
		vecB = p-localStart;
		float dot = DotProd(Normalize(vecA),Normalize(vecB));

		float dist;
		if (fabs(1.0f-dot) < std::numeric_limits<float>::epsilon() )
			dist = Length(vecB) ;
		else
		{
			float angle = acos(dot);
			dist = Length(vecB) * cos(angle);
		}

		if (dist < 0.0f) dist = 0.0f;

		center = localStart+(Normalize(vecA) * dist);
	}
	else if (IsValidBoneIndex(ModeBoneIndex))
	{
		BoneDataClass& boneData = BoneData[ModeBoneIndex];
		useLocalAxis = FALSE;
		Interval iv;
		Point3 a,b;
		boneData.EndPoint1Control->GetValue(currentTime,&a,iv,CTRL_ABSOLUTE);
		center = a;

		boneData.EndPoint2Control->GetValue(currentTime,&b,iv,CTRL_ABSOLUTE);
		center += b;
		center *= 0.05f;
	}

	pt = center * tm;
	tm.SetRow(3,pt);
	cb->TM(tm,0);
}

void BonesDefMod::ResetSelection()
{
	updateP = TRUE;

	if ( (ModeBoneEnvelopeIndex==-1) &&  (ModeBoneEnvelopeSubType==-1) && (BoneData.Count() >0))
	{
		if (!IsValidBoneIndex(ModeBoneIndex))
		{
			ModeBoneIndex = -1;
			for (int i = 0; i < BoneData.Count(); i++)
			{
				if (BoneData[i].Node != NULL)
				{
					ModeBoneIndex = i;
					break;
				}
			}
		}

		ModeBoneEnvelopeIndex=0;
		ModeBoneEnvelopeSubType=5;
		//clear selection flags;
		for (int bct = 0; bct < BoneData.Count(); bct++)
		{
			BoneDataClass& boneData = BoneData[bct];
			boneData.end1Selected = FALSE;
			boneData.end2Selected = FALSE;
			for (int cct = 0; cct < boneData.CrossSectionList.Count(); cct++)
			{
				boneData.CrossSectionList[cct].innerSelected = FALSE;
				boneData.CrossSectionList[cct].outerSelected = FALSE;
			}
		}
		if ( (ModeBoneIndex != -1) && (BoneData[ModeBoneIndex].CrossSectionList.Count()) )
			BoneData[ModeBoneIndex].CrossSectionList[0].outerSelected=TRUE;
	}
	UpdatePropInterface();
}

void BonesDefMod::UpdatePropInterface(BOOL checkGeomType)
{
	if ( (ip) && IsValidBoneIndex(ModeBoneIndex))
	{
		BoneDataClass& boneData = BoneData[ModeBoneIndex];
		if (boneData.flags & BONE_ABSOLUTE_FLAG)
		{
			iAbsolute->SetCheck(FALSE);
		}
		else
		{
			iAbsolute->SetCheck(TRUE);
		}

		if (boneData.flags & BONE_DRAW_ENVELOPE_FLAG)
		{
			iEnvelope->SetCheck(TRUE);
		}
		else
		{
			iEnvelope->SetCheck(FALSE);
		}
		if (boneData.FalloffType == BONE_FALLOFF_X_FLAG)
			iFalloff->SetCurFlyOff(0,FALSE);
		else if (boneData.FalloffType == BONE_FALLOFF_SINE_FLAG)
			iFalloff->SetCurFlyOff(1,FALSE);
		else if (boneData.FalloffType == BONE_FALLOFF_X3_FLAG)
			iFalloff->SetCurFlyOff(3,FALSE);
		else if (boneData.FalloffType == BONE_FALLOFF_3X_FLAG)
			iFalloff->SetCurFlyOff(2,FALSE);

		if ((ModeBoneIndex >=0) && (ModeBoneIndex <pblock_param->Count(skin_local_squash)))
		{
			float val = 0.;
			pblock_param->GetValue(skin_local_squash,ip->GetTime(), val, FOREVER, ModeBoneIndex);
			ISpinnerControl* spin = SetupFloatSpinner(hParam, IDC_SQUASHSPIN, IDC_SQUASH, 0.0f, 10.0f, val, .1f);
			ReleaseISpinner(spin);
		}

		EnableButtons(checkGeomType);
		EnableWindow(GetDlgItem(hParam,IDC_REMOVE),TRUE);
	}
	else
	{
		DisableButtons();
		EnableWindow(GetDlgItem(hParam,IDC_REMOVE),FALSE);
	}
}

void BonesDefMod::UpdateP(BoneModData* bmd)
{
	if (!ip) return;

	if (!IsValidBoneIndex(ModeBoneIndex))
		return;

	BoneDataClass& boneData = BoneData[ModeBoneIndex];
	if ( (ModeBoneEnvelopeIndex < 0) || (ModeBoneEnvelopeIndex >= boneData.CrossSectionList.Count()) ) return;

	Point3 align;
	Point3 vec;

	Point3 p_edge[8];
	Point3 ept(0.0f,0.0f,0.0f);

	ObjectState os;
	ShapeObject *pathOb = NULL;

	Interval valid;
	Matrix3 ntm = boneData.Node->GetObjTMBeforeWSM(ip->GetTime(),&valid);

	if ((boneData.flags & BONE_SPLINE_FLAG) && (boneData.Node != NULL))
	{
		ObjectState os = boneData.Node->EvalWorldState(ip->GetTime());
		pathOb = (ShapeObject*)os.obj;
		if (pathOb->NumberOfCurves() != 0)
		{
			ept = pathOb->InterpCurve3D(ip->GetTime(), 0,boneData.CrossSectionList[ModeBoneEnvelopeIndex].u);
			align = pathOb->TangentCurve3D(ip->GetTime(), 0,boneData.CrossSectionList[ModeBoneEnvelopeIndex].u);
			ept = ept * bmd->tmCacheToObjectSpace[ModeBoneIndex];
			align = VectorTransform(bmd->tmCacheToObjectSpace[ModeBoneIndex],align);
		}
	}
	else
	{
		Point3 l1,l2;
		Interval v;
		GetEndPointsLocal(bmd,ip->GetTime(),l1,l2, ModeBoneIndex);

		align = l2-l1;
		Point3 nvec = align;
		ept = l1;
		ept = ept + nvec * boneData.CrossSectionList[ModeBoneEnvelopeIndex].u;
	}

	float inner, outer;
	Interval v;
	boneData.CrossSectionList[ModeBoneEnvelopeIndex].InnerControl->GetValue(currentTime,&inner,v);
	boneData.CrossSectionList[ModeBoneEnvelopeIndex].OuterControl->GetValue(currentTime,&outer,v);
	GetCrossSection(ept, align, inner, boneData.temptm,  p_edge, 4);
	GetCrossSection(ept, align, outer, boneData.temptm,  &p_edge[4], 4);

	if (ModeBoneEnvelopeSubType < 4)
	{
		pblock_param->SetValue(skin_cross_radius,0,inner);
		boneData.CrossSectionList[ModeBoneEnvelopeIndex].innerSelected = TRUE;
	}
	else
	{
		pblock_param->SetValue(skin_cross_radius,0,outer);
		boneData.CrossSectionList[ModeBoneEnvelopeIndex].outerSelected = TRUE;
	}
	Point3 p = p_edge[ModeBoneEnvelopeSubType] * bmd->tmCacheToBoneSpace[ModeBoneIndex];
	bmd->localCenter = p_edge[ModeBoneEnvelopeSubType];

	localVec = p;
	localStart = ((p_edge[0]+p_edge[1]+p_edge[2]+p_edge[3]) *.25f)* bmd->tmCacheToBoneSpace[ModeBoneIndex];

	p1Temp->SetValue(0,p,TRUE,CTRL_ABSOLUTE);
}

void BonesDefMod::SelectSubComponent(
	HitRecord *hitRec, BOOL selected, BOOL all, BOOL invert)
{
	if (ip == nullptr)
		return;
	// Pass selection test to gizmo if editing
	GizmoClass *gizmo = NULL;
	if ( (pblock_gizmos->Count(skin_gizmos_list) > 0) && (currentSelectedGizmo<(pblock_gizmos->Count(skin_gizmos_list)) ))
	{
		ReferenceTarget *ref;
		ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
		gizmo = (GizmoClass *)ref;
	}

	if ( (gizmo) && (gizmo->IsEditing()))
	{
		gizmo->SelectSubComponent(hitRec, selected, all, invert);
		return;
	}

	ModeEdit = 0;

	Tab<BoneModData*> bmdList;
	HitRecord *head = hitRec;
	BOOL vertexSelected = FALSE;
	while (hitRec)
	{
		BoneModData *tbmd = (BoneModData*)hitRec->modContext->localData;
		BOOL found = FALSE;
		for (int i = 0; i < bmdList.Count(); i++)
		{
			if (bmdList[i] == tbmd)
			{
				found = TRUE;
				i = bmdList.Count();
			}
		}
		if (!found) bmdList.Append(1,&tbmd,1);

		BoneHitDataClass *bhd = (BoneHitDataClass *) hitRec->hitData;
		// When the selected bone in the view port/scene is not in the listbox after filtering,
		// we need to empty name picker and refill the corresponding list box
		int iSelectedBoneId = bhd->BoneId;
		DbgAssert((iSelectedBoneId >= -1) && (iSelectedBoneId < BoneData.Count()));
		if((iSelectedBoneId != -1) && (iSelectedBoneId < BoneData.Count()) && (BoneData[iSelectedBoneId].bFilterOut == TRUE))
		{
			RefreshNamePicker();
		}

		if (bhd->VertexId != -1)
			vertexSelected = TRUE;

		hitRec = hitRec->Next();
	}
	hitRec = head;

	for (int i = 0; i < bmdList.Count(); i++)
	{
		if (theHold.Holding() ) theHold.Put(new SelectionRestore(this,bmdList[i]));
	}

	BOOL sub = !selected;

	int Count = 0;
	BOOL state = selected;

	BoneHitDataClass *bhd;

	int mode = -1;

	BOOL vertSelected = FALSE;
	BOOL boneSelected = FALSE;

	while (hitRec) {
		state = hitRec->hitInfo;
		BoneModData *bmd = (BoneModData*)hitRec->modContext->localData;

		//MIRROR
		if (mirrorData.Enabled())
		{
			bhd = (BoneHitDataClass *) hitRec->hitData;
			if (bhd->VertexId == -1)
			{
				if (sub)
					mirrorData.SelectBone(bhd->BoneId,FALSE);
				else mirrorData.SelectBone(bhd->BoneId,TRUE);
			}
		}

		if (sub)
		{
			if (state < bmd->selected.GetSize())
			{
				bmd->selected.Set(state,FALSE);
				vertSelected = TRUE;
			}
		}
		else
		{
			bhd = (BoneHitDataClass *) hitRec->hitData;
			if ((bhd->VertexId == -1) && (!vertexSelected))
			{
				mode = 1;
				if (ModeBoneIndex == bhd->BoneId)
				{
					ModeBoneIndex = bhd->BoneId;
					if ( (ModeBoneEnvelopeIndex != bhd->CrossId) ||
						(ModeBoneEnvelopeSubType != bhd->CrossHandleId) )
					{
						if (bhd->CrossHandleId < 4)
							macroRecorder->FunctionCall(_T("skinOps.SelectCrossSection"), 3, 0, mr_reftarg, this, mr_int, bhd->CrossId+1, mr_int, 0);
						else macroRecorder->FunctionCall(_T("skinOps.SelectCrossSection"), 3, 0, mr_reftarg, this, mr_int, bhd->CrossId+1, mr_int, 1);
					}
					ModeBoneEnvelopeIndex = bhd->CrossId;

					ModeBoneEndPoint = bhd->EndPoint;
					ModeBoneEnvelopeIndex = bhd->CrossId;
					ModeBoneEnvelopeSubType = bhd->CrossHandleId;
					boneSelected = TRUE;
				}
				else
				{
					ModeBoneIndex = bhd->BoneId;
					ModeBoneEnvelopeIndex = -1;
					if (bhd->EndPoint ==0)
						BoneData[ModeBoneIndex].end1Selected = TRUE;
					else if (bhd->EndPoint ==1)
						BoneData[ModeBoneIndex].end2Selected = TRUE;
					ModeBoneEndPoint = bhd->EndPoint;
					ModeBoneEnvelopeIndex = -1;
					ModeBoneEnvelopeSubType = -1;
					int tempID = ConvertBoneIndexToBoneID(ModeBoneIndex);
					macroRecorder->FunctionCall(_T("skinOps.SelectBone"), 2, 0, mr_reftarg, this, mr_int, tempID+1);
					boneSelected = TRUE;
				}

				UpdatePropInterface();
			}
			else
			{
				if ((state < bmd->selected.GetSize()) && (bhd->VertexId != -1))
				{
					bmd->selected.Set(state,TRUE);
					vertSelected = TRUE;
				}
			}
		}

		hitRec = hitRec->Next();
		Count++;
	}

	BOOL selectElement = TRUE;
	pblock_param->GetValue(skin_selectelement,0,selectElement,FOREVER);
	if (selectElement && vertSelected)
	{
		for (int i = 0; i < bmdList.Count(); i++)
		{
			if (bmdList[i])
				SelectElement(bmdList[i],sub);
		}
	}

	if (!vertSelected && boneSelected)
	{
		for (int i = 0; i < bmdList.Count(); i++)
		{
			if (bmdList[i])
			{
				if (bmdList[i]->selected.GetSize() == bmdList[i]->tempSelected.GetSize())
					bmdList[i]->selected = bmdList[i]->tempSelected;
			}
		}
	}

	for (int i = 0; i < bmdList.Count(); i++)
	{
		BoneModData *bmd = bmdList[i];

		if (bmd->selected.NumberSet() > 0)
		{
			EnableEffect(TRUE);
			macroRecorder->FunctionCall(_T("skinOps.SelectVertices"), 2, 0, mr_reftarg, this, mr_bitarray, &bmd->selected);
			NotifyDependents(FOREVER, PART_SELECT, REFMSG_CHANGE);
		}
		else
		{
			EnableEffect(FALSE);
			NotifyDependents(FOREVER, PART_SELECT, REFMSG_CHANGE);
		}

		if (mode >0 )
		{
			Point3 p;
			//clear selection flags;
			for (int bct = 0; bct < BoneData.Count(); bct++)
			{
				BoneDataClass& boneData = BoneData[bct];
				boneData.end1Selected = FALSE;
				boneData.end2Selected = FALSE;
				for (int cct = 0; cct < boneData.CrossSectionList.Count(); cct++)
				{
					boneData.CrossSectionList[cct].innerSelected = FALSE;
					boneData.CrossSectionList[cct].outerSelected = FALSE;
				}
			}
			if (ModeBoneEndPoint == 0)
			{
				BoneData[ModeBoneIndex].end1Selected = TRUE;
				Point3 tp;
				GetEndPoints(bmd, ip->GetTime(), bmd->localCenter, tp, ModeBoneIndex);
				macroRecorder->FunctionCall(_T("skinOps.SelectStartPoint"), 1, 0, mr_reftarg, this);
			}
			else if (ModeBoneEndPoint == 1)
			{
				BoneData[ModeBoneIndex].end2Selected = TRUE;
				Point3 tp;
				GetEndPoints(bmd, ip->GetTime(), tp, bmd->localCenter, ModeBoneIndex);
				macroRecorder->FunctionCall(_T("skinOps.SelectEndPoint"), 1, 0, mr_reftarg, this);
			}
			else if (ModeBoneEnvelopeIndex>=0)
			{
				BoneDataClass& boneData = BoneData[ModeBoneIndex];
				Point3 align;
				Point3 vec;

				Point3 p_edge[8];
				Point3 ept(0.0f,0.0f,0.0f);

				ObjectState os;
				ShapeObject *pathOb = NULL;

				Interval valid;
				Matrix3 ntm = boneData.Node->GetObjTMBeforeWSM(ip->GetTime(),&valid);

				if ((boneData.flags & BONE_SPLINE_FLAG) && (boneData.Node != NULL))
				{
					ObjectState os = boneData.Node->EvalWorldState(ip->GetTime());
					pathOb = (ShapeObject*)os.obj;
					if (pathOb->NumberOfCurves() != 0)
					{
						ept = pathOb->InterpCurve3D(ip->GetTime(), 0,boneData.CrossSectionList[ModeBoneEnvelopeIndex].u);
						align = pathOb->TangentCurve3D(ip->GetTime(), 0,boneData.CrossSectionList[ModeBoneEnvelopeIndex].u);
						ept = ept * bmd->tmCacheToObjectSpace[ModeBoneIndex];
						align = VectorTransform(bmd->tmCacheToObjectSpace[ModeBoneIndex],align);
					}
				}
				else
				{
					Point3 l1,l2;
					Interval v;
					GetEndPointsLocal(bmd,ip->GetTime(),l1,l2, ModeBoneIndex);

					align = l2-l1;
					Point3 nvec = align;
					ept = l1;
					ept = ept + nvec * boneData.CrossSectionList[ModeBoneEnvelopeIndex].u;
				}

				float inner, outer;
				Interval v;
				boneData.CrossSectionList[ModeBoneEnvelopeIndex].InnerControl->GetValue(currentTime,&inner,v);
				boneData.CrossSectionList[ModeBoneEnvelopeIndex].OuterControl->GetValue(currentTime,&outer,v);

				GetCrossSection(ept, align, inner, boneData.temptm, p_edge, 4);
				GetCrossSection(ept, align, outer, boneData.temptm, &p_edge[4], 4);

				if (ModeBoneEnvelopeSubType < 4)
				{
					pblock_param->SetValue(skin_cross_radius,0,inner);
					boneData.CrossSectionList[ModeBoneEnvelopeIndex].innerSelected = TRUE;
				}
				else
				{
					pblock_param->SetValue(skin_cross_radius,0,outer);
					boneData.CrossSectionList[ModeBoneEnvelopeIndex].outerSelected = TRUE;
				}
				p = p_edge[ModeBoneEnvelopeSubType] * bmd->tmCacheToBoneSpace[ModeBoneIndex];
				bmd->localCenter = p_edge[ModeBoneEnvelopeSubType];
				localVec = p;
				localStart = ((p_edge[0]+p_edge[1]+p_edge[2]+p_edge[3]) *.25f)* bmd->tmCacheToBoneSpace[ModeBoneIndex];;
			}

			if  (ModeBoneEnvelopeIndex == -1) EnableRadius(FALSE);
			else EnableRadius(TRUE);

			p1Temp->SetValue(0,p,TRUE,CTRL_ABSOLUTE);

			//select in list box also
			SelectBoneByBoneIndex(ModeBoneIndex);
		}

		UpdateEffectSpinner(bmd);

		int nset = bmd->selected.NumberSet();
		int total = bmd->selected.GetSize();
	}
	ip->ClearCurNamedSelSet();

	//WEIGHTTABLE
	PaintAttribList();

	//MIRROR
	if (mirrorData.Enabled())
		mirrorData.EmitBoneSelectionScript();
}

void BonesDefMod::UpdateEffectSpinner(BoneModData*bmd)
{
	if (bmd->selected.NumberSet() > 0)
	{
		int sct = 0;
		float v = -1.0f;
		BOOL first = TRUE;
		BOOL idnt = FALSE;

		for (int i = 0; i < bmd->selected.GetSize(); i++)
		{
			if (bmd->selected[i])
			{
				BOOL match = FALSE;
				for (int ct =0 ; ct <bmd->VertexData[i]->WeightCount(); ct++)
				{
					if (bmd->VertexData[i]->GetBoneIndex(ct) == ModeBoneIndex)
					{
						match = TRUE;
						float tempV = RetrieveNormalizedWeight(bmd,i,ct);
						if (first)
						{
							v = tempV;
							first = FALSE;
						}
						else if (v != tempV)
						{
							idnt = TRUE;
						}
					}
				}
				if (!match)
				{
					if (first)
					{
						v = 0.0f;
						first = FALSE;
					}
					else if (v != 0.0f)
					{
						idnt = TRUE;
					}
				}
			}
		}

		if (idnt)
		{
			ISpinnerControl *spin2 = GetISpinner(GetDlgItem(hParam,IDC_EFFECTSPIN));
			if (spin2)
			{
				spin2->SetIndeterminate(TRUE);
				ReleaseISpinner(spin2);
			}
		}
		else
		{
			ISpinnerControl *spin2 = GetISpinner(GetDlgItem(hParam,IDC_EFFECTSPIN));
			if (spin2)
			{
				spin2->SetIndeterminate(FALSE);
				spin2->SetValue(v,0);
				ReleaseISpinner(spin2);
			}
			pblock_param->SetValue(skin_effect,0,v);
			bmd->effect = v;
		}

		BOOL unNorm = FALSE;
		BOOL indeter = FALSE;
		BOOL firstVert = TRUE;
		for (int i = 0; i < bmd->selected.GetSize(); i++)
		{
			if (bmd->selected[i])
			{
				if (firstVert)
				{
					unNorm = bmd->VertexData[i]->IsUnNormalized();
					firstVert = FALSE;
				}
				else
				{
					BOOL iret = bmd->VertexData[i]->IsUnNormalized();
					if (unNorm != bmd->VertexData[i]->IsUnNormalized())
						indeter = TRUE;
				}
			}
		}

		EnableWindow(GetDlgItem(hParam,IDC_NORMALIZE_CHECK),TRUE);

		if (!indeter)
		{
			if (unNorm)
				CheckDlgButton(hParam,IDC_NORMALIZE_CHECK,BST_UNCHECKED);
			else CheckDlgButton(hParam,IDC_NORMALIZE_CHECK,BST_CHECKED);
		}
		else CheckDlgButton(hParam,IDC_NORMALIZE_CHECK,BST_INDETERMINATE);

		BOOL rigid = FALSE;
		indeter = FALSE;
		firstVert = TRUE;
		for (int i = 0; i < bmd->selected.GetSize(); i++)
		{
			if (bmd->selected[i])
			{
				if (firstVert)
				{
					rigid = bmd->VertexData[i]->IsRigid();
					firstVert = FALSE;
				}
				else
				{
					BOOL iret = bmd->VertexData[i]->IsRigid();
					if (rigid != bmd->VertexData[i]->IsRigid())
						indeter = TRUE;
				}
			}
		}

		EnableWindow(GetDlgItem(hParam,IDC_RIGID_CHECK),TRUE);

		if (!indeter)
		{
			if (rigid)
				CheckDlgButton(hParam,IDC_RIGID_CHECK,BST_CHECKED);
			else CheckDlgButton(hParam,IDC_RIGID_CHECK,BST_UNCHECKED);
		}
		else CheckDlgButton(hParam,IDC_RIGID_CHECK,BST_INDETERMINATE);

		BOOL rigidHandle = FALSE;
		indeter = FALSE;
		firstVert = TRUE;
		for (int i = 0; i < bmd->selected.GetSize(); i++)
		{
			if (bmd->selected[i])
			{
				if (firstVert)
				{
					rigidHandle = bmd->VertexData[i]->IsRigidHandle();
					firstVert = FALSE;
				}
				else
				{
					BOOL iret = bmd->VertexData[i]->IsRigidHandle();
					if (rigidHandle != bmd->VertexData[i]->IsRigidHandle())
						indeter = TRUE;
				}
			}
		}

		EnableWindow(GetDlgItem(hParam,IDC_RIGIDHANDLES_CHECK),TRUE);

		if (!indeter)
		{
			if (rigidHandle)
				CheckDlgButton(hParam,IDC_RIGIDHANDLES_CHECK,BST_CHECKED);
			else CheckDlgButton(hParam,IDC_RIGIDHANDLES_CHECK,BST_UNCHECKED);
		}
		else CheckDlgButton(hParam,IDC_RIGIDHANDLES_CHECK,BST_INDETERMINATE);
	}
	else
	{
		EnableWindow(GetDlgItem(hParam,IDC_RIGID_CHECK),FALSE);
		EnableWindow(GetDlgItem(hParam,IDC_RIGIDHANDLES_CHECK),FALSE);
		EnableWindow(GetDlgItem(hParam,IDC_NORMALIZE_CHECK),FALSE);
	}
}

void BonesDefMod::ClearSelection(int selLevel)
{
	//MIRROR
	if (mirrorData.Enabled())
	{
		mirrorData.ClearBoneSelection();
	}

	if (selLevel == 1)
	{
		//Passs selection test to gizmo if editing
		GizmoClass *gizmo = NULL;
		if ( (pblock_gizmos->Count(skin_gizmos_list) > 0) && (currentSelectedGizmo<(pblock_gizmos->Count(skin_gizmos_list)) ))
		{
			ReferenceTarget *ref;
			ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
			gizmo = (GizmoClass *)ref;
		}

		if ( (gizmo) && (gizmo->IsEditing()))
		{
			gizmo->ClearSelection(selLevel);
			return;
		}

		ModContextList mcList;
		INodeTab nodes;

		ip->GetModContexts(mcList,nodes);
		int objects = mcList.Count();

		TimeValue t = GetCOREInterface()->GetTime();
		for ( int i = 0; i < objects; i++ )
		{
			BoneModData *bmd = (BoneModData*)mcList[i]->localData;

			if (theHold.Holding() )
				theHold.Put(new SelectionRestore(this,bmd));

			bmd->tempSelected = bmd->selected;

			int numSelected = bmd->selected.GetSize();
			for (int j =0; j < numSelected; j++)
				bmd->selected.Set(j,FALSE);

			UpdateEffectSpinner(bmd);

			INode* node = nodes[i]->GetActualINode();
			if (node)
			{
				node->InvalidateRect(t);
			}
		}

		GetCOREInterface()->RedrawViews(t);
		GetCOREInterface()->ClearCurNamedSelSet();
	}
	//WEIGHTTABLE
	PaintAttribList();
}

void BonesDefMod::SelectAll(int selLevel)
{
	if (selLevel == 1)
	{
		//Passs selection test to gizmo if editing
		GizmoClass *gizmo = NULL;
		if ( (pblock_gizmos->Count(skin_gizmos_list) > 0) && (currentSelectedGizmo<(pblock_gizmos->Count(skin_gizmos_list)) ))
		{
			ReferenceTarget *ref;
			ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
			gizmo = (GizmoClass *)ref;
		}

		if ( (gizmo) && (gizmo->IsEditing()))
		{
			gizmo->SelectAll(selLevel);
			return;
		}

		ModContextList mcList;
		INodeTab nodes;

		ip->GetModContexts(mcList,nodes);
		int objects = mcList.Count();

		BOOL showHiddenVerts = FALSE;
		pblock_display->GetValue(skin_display_showhiddenvertices,0,showHiddenVerts,FOREVER);

		for ( int i = 0; i < objects; i++ )
		{
			BoneModData *bmd = (BoneModData*)mcList[i]->localData;

			if (theHold.Holding() ) theHold.Put(new SelectionRestore(this,bmd));

			for (int i =0;i<bmd->selected.GetSize();i++)
			{
				if (!showHiddenVerts)
				{
					if (!bmd->VertexData[i]->IsHidden())
						bmd->selected.Set(i,TRUE);
				}
				else bmd->selected.Set(i,TRUE);
			}

			UpdateEffectSpinner(bmd);

			NotifyDependents(FOREVER, PART_SELECT, REFMSG_CHANGE);
		}
		ip->ClearCurNamedSelSet();
	}
	//WEIGHTTABLE
	PaintAttribList();
}

int BonesDefMod::HoldWeights()
{
	ModContextList mcList;
	INodeTab nodes;

	theHold.SuperBegin();
	if (ip)
	{
		ip->GetModContexts(mcList,nodes);
		int objects = mcList.Count();
		theHold.Begin();
		for ( int i = 0; i < objects; i++ )
		{
			BoneModData *bmd = (BoneModData*)mcList[i]->localData;
			theHold.Put(new WeightRestore(this,bmd));
		}
		theHold.Accept(GetString(IDS_PW_WEIGHTCHANGE));
	}
	return 1;
}

int BonesDefMod::AcceptWeights(BOOL accept)
{
	if (accept) theHold.SuperAccept(GetString(IDS_PW_WEIGHTCHANGE));
	else theHold.SuperCancel();
	return 1;
}

void BonesDefMod::InvertSelection(int selLevel)
{
	if (selLevel == 1)
	{
		//Pass selection test to gizmo if editing
		GizmoClass *gizmo = NULL;
		if ( (pblock_gizmos->Count(skin_gizmos_list) > 0) && (currentSelectedGizmo<(pblock_gizmos->Count(skin_gizmos_list)) ))
		{
			ReferenceTarget *ref;
			ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
			gizmo = (GizmoClass *)ref;
		}

		if ( (gizmo) && (gizmo->IsEditing()))
		{
			gizmo->InvertSelection(selLevel);
			return;
		}

		ModContextList mcList;
		INodeTab nodes;

		ip->GetModContexts(mcList,nodes);
		int objects = mcList.Count();

		for ( int i = 0; i < objects; i++ )
		{
			BoneModData *bmd = (BoneModData*)mcList[i]->localData;

			for (int i =0;i<bmd->selected.GetSize();i++)
			{
				BOOL v = !bmd->selected[i];
				bmd->selected.Set(i,v);
			}

			UpdateEffectSpinner(bmd);

			NotifyDependents(FOREVER, PART_SELECT, REFMSG_CHANGE);
		}
		ip->ClearCurNamedSelSet();
	}

	//WEIGHTTABLE
	PaintAttribList();

	NotifyDependents(FOREVER, PART_SELECT, REFMSG_CHANGE);
}

void BonesDefMod::EnableRadius(BOOL enable)
{
	if (enable)
		SpinnerOn(hParam,IDC_ERADIUSSPIN,IDC_ERADIUS);
	else SpinnerOff(hParam,IDC_ERADIUSSPIN,IDC_ERADIUS);
}

void BonesDefMod::EnableEffect(BOOL enable)
{
	if (enable)
		SpinnerOn(hParam,IDC_EFFECTSPIN,IDC_EFFECT);
	else SpinnerOff(hParam,IDC_EFFECTSPIN,IDC_EFFECT);
}

void BonesDefMod::EnableButtons(BOOL checkGeomType)
{
	if (ip == nullptr)
		return;
	BOOL turnOnFilters = FALSE;
	if (ip->GetSubObjectLevel() == 1)
		turnOnFilters = TRUE;

	if (iCrossSectionButton!=NULL)
		iCrossSectionButton->Enable(turnOnFilters);
	if (checkGeomType && (iPaintButton!=NULL))
	{
		//Defect 724761 - The user is not supposed to be painting
		//on a spline object or on Nurbs Curve with the Skin modifier
		//Paint Weights tools option.
		BOOL turnOffFilter = FALSE;

		MyEnumProc dep;
		DoEnumDependents(&dep);
		//this puts back the original state of the node vc mods and shade state

		for (int  i = 0; i < dep.Nodes.Count(); i++)
		{
			INode *node = dep.Nodes[i];

			//get object state
			ObjectState os;
			os = node->EvalWorldState(RefFrame);

			//If the sub class of the Object is a Nurbs
			//we need to check if is't a nurbs curve or a nurbs surface.
			if (os.obj->IsSubClassOf(EDITABLE_SURF_CLASS_ID))
			{
				NURBSSet NurbsGroup;
				BOOL objectIsANurbs = GetNURBSSet(os.obj, GetCOREInterface()->GetTime(), NurbsGroup, TRUE);
				if (objectIsANurbs)
				{
					for(int j = 0; j < NurbsGroup.GetNumObjects(); j++)
					{
						//If the type of the Nurbs object is a curve (point curve or
						//CV curve) the paint weights button is turned off.
						if (NurbsGroup.GetNURBSObject(j)->GetType() == kNPointCurve ||
							NurbsGroup.GetNURBSObject(j)->GetType() == kNCVCurve)
						{
							iPaintButton->Enable(turnOffFilter);
							break;
						}
						//If the type of the Nurbs is a surface
						//the paint weights button is turned on.
						else
						{
							iPaintButton->Enable(turnOnFilters);
							break;
						}
					}
				}

				break;
			}
			else
			{
				//If the sub class of the Object is a spline
				//the Paint Weights button is turned off.
				if(os.obj->IsSubClassOf(splineShapeClassID))
				{
					iPaintButton->Enable(turnOffFilter);
					break;
				}
				//If the sub class of the Object is a mesh, patch,
				//or poly the Paint Weights button is turned on.
				else
				{
					iPaintButton->Enable(turnOnFilters);
					break;
				}
			}
		}
	}
	EnableWindow(GetDlgItem(hParam,IDC_CREATE_REMOVE_SECTION),turnOnFilters);

	EnableWindow(GetDlgItem(hParam,IDC_FILTER_VERTICES_CHECK),turnOnFilters);
	EnableWindow(GetDlgItem(hParam,IDC_FILTER_BONES_CHECK),turnOnFilters);
	EnableWindow(GetDlgItem(hParam,IDC_FILTER_ENVELOPES_CHECK),turnOnFilters);

	BOOL filter = FALSE;
	pblock_param->GetValue(skin_filter_vertices,0,filter,FOREVER);

	if (filter)
	{
		EnableWindow(GetDlgItem(hParam,IDC_SELECTELEMENT_CHECK),turnOnFilters);
		EnableWindow(GetDlgItem(hParam,IDC_BACKFACECULL_CHECK),turnOnFilters);

		EnableWindow(GetDlgItem(hParam,IDC_RING),turnOnFilters);
		EnableWindow(GetDlgItem(hParam,IDC_LOOP),turnOnFilters);
		EnableWindow(GetDlgItem(hParam,IDC_GROW),turnOnFilters);
		EnableWindow(GetDlgItem(hParam,IDC_SHRINK),turnOnFilters);
	}
	else
	{
		EnableWindow(GetDlgItem(hParam,IDC_SELECTELEMENT_CHECK),FALSE);
		EnableWindow(GetDlgItem(hParam,IDC_BACKFACECULL_CHECK),FALSE);

		EnableWindow(GetDlgItem(hParam,IDC_RING),FALSE);
		EnableWindow(GetDlgItem(hParam,IDC_LOOP),FALSE);
		EnableWindow(GetDlgItem(hParam,IDC_GROW),FALSE);
		EnableWindow(GetDlgItem(hParam,IDC_SHRINK),FALSE);
	}

	EnableWindow(GetDlgItem(hParam,IDC_DRAWALL_ENVELOPES_CHECK),TRUE);
	EnableWindow(GetDlgItem(hParam,IDC_DRAW_VERTICES_CHECK),TRUE);

	ICustToolbar *iGizm0ToolsParams;
	iGizm0ToolsParams = GetICustToolbar(GetDlgItem(hParamGizmos,IDC_GIZMO_TOOLBAR));
	if (iGizm0ToolsParams)
	{
		ICustButton*   iToolButton;
		iToolButton =  iGizm0ToolsParams->GetICustButton(IDC_ADD);
		if (iToolButton)
		{
			iToolButton->Enable(TRUE);
			ReleaseICustButton(iToolButton);
		}

		iToolButton =  iGizm0ToolsParams->GetICustButton(IDC_REMOVE);
		if (iToolButton)
		{
			iToolButton->Enable(TRUE);
			ReleaseICustButton(iToolButton);
		}
		ReleaseICustToolbar(iGizm0ToolsParams);
	}

	ICustToolbar *iWeightToolsParams;
	iWeightToolsParams = GetICustToolbar(GetDlgItem(hParam,IDC_WEIGHT_TOOLBAR));
	if (iWeightToolsParams)
	{
		ICustButton *iBut = iWeightToolsParams->GetICustButton(IDC_EXCLUDE);
		if (iBut)
		{
			iBut->Enable(turnOnFilters);
			ReleaseICustButton(iBut);
		}
		iBut = iWeightToolsParams->GetICustButton(IDC_INCLUDE);
		if (iBut)
		{
			iBut->Enable(turnOnFilters);
			ReleaseICustButton(iBut);
		}
		iBut = iWeightToolsParams->GetICustButton(IDC_SELECT_EXCLUDED);
		if (iBut)
		{
			iBut->Enable(turnOnFilters);
			ReleaseICustButton(iBut);
		}
		ReleaseICustToolbar(iWeightToolsParams);
	}

	if (iAbsolute!=NULL)
		iAbsolute->Enable();
	if (iEnvelope!=NULL)
		iEnvelope->Enable();
	if (iFalloff!=NULL)
		iFalloff->Enable();
	if (iCopy!=NULL)
		iCopy->Enable();

	if ((iPaste!=NULL) && (CopyBuffer.CList.Count() != 0))
		iPaste->Enable();

	if ((ModeBoneEnvelopeIndex>=0) && (ModeBoneEnvelopeIndex<BoneData[ModeBoneIndex].CrossSectionList.Count()))
		SpinnerOn(hParam,IDC_ERADIUSSPIN,IDC_ERADIUS);
	else SpinnerOff(hParam,IDC_ERADIUSSPIN,IDC_ERADIUS);
	SpinnerOn(hParam,IDC_SQUASHSPIN,IDC_SQUASH);

	ModContextList mcList;
	INodeTab nodes;
	ip->GetModContexts(mcList,nodes);
	int objects = mcList.Count();

	BOOL vertexSelected = FALSE;
	for ( int i = 0; i < objects; i++ )
	{
		BoneModData *bmd = (BoneModData*)mcList[i]->localData;
		if (bmd->selected.NumberSet() > 0)
			vertexSelected = TRUE;
	}

	EnableWindow(GetDlgItem(hParam,IDC_NORMALIZE_CHECK),vertexSelected);
	EnableWindow(GetDlgItem(hParam,IDC_RIGID_CHECK),vertexSelected);
	EnableWindow(GetDlgItem(hParam,IDC_RIGIDHANDLES_CHECK),vertexSelected);
}

void BonesDefMod::DisableButtons()
{
	if (ip == nullptr)
		return;
	if (iCrossSectionButton!=NULL)
	{
		iCrossSectionButton->Disable();
		if ( ip )
		{
			if (ip->GetCommandMode() == CrossSectionMode)
				ip->SetStdCommandMode(CID_OBJMOVE);
		}
	}
	if (iPaintButton!=NULL)
		iPaintButton->Disable();

	EnableWindow(GetDlgItem(hParam,IDC_CREATE_REMOVE_SECTION),FALSE);

	EnableWindow(GetDlgItem(hParam,IDC_FILTER_VERTICES_CHECK),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_FILTER_BONES_CHECK),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_FILTER_ENVELOPES_CHECK),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_DRAWALL_ENVELOPES_CHECK),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_DRAW_VERTICES_CHECK),FALSE);

	EnableWindow(GetDlgItem(hParam,IDC_SELECTELEMENT_CHECK),FALSE);

	EnableWindow(GetDlgItem(hParam,IDC_RING),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_LOOP),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_GROW),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_SHRINK),FALSE);

	ICustToolbar *iGizm0ToolsParams;
	iGizm0ToolsParams = GetICustToolbar(GetDlgItem(hParamGizmos,IDC_GIZMO_TOOLBAR));
	if (iGizm0ToolsParams)
	{
		ICustButton*   iToolButton;
		iToolButton =  iGizm0ToolsParams->GetICustButton(IDC_ADD);
		if (iToolButton)
		{
			iToolButton->Enable(FALSE);
			ReleaseICustButton(iToolButton);
		}

		iToolButton =  iGizm0ToolsParams->GetICustButton(IDC_REMOVE);
		if (iToolButton)
		{
			iToolButton->Enable(FALSE);
			ReleaseICustButton(iToolButton);
		}
		ReleaseICustToolbar(iGizm0ToolsParams);
	}

	ICustToolbar *iWeightToolsParams;
	iWeightToolsParams = GetICustToolbar(GetDlgItem(hParam,IDC_WEIGHT_TOOLBAR));
	if (iWeightToolsParams)
	{
		ICustButton *iBut = iWeightToolsParams->GetICustButton(IDC_EXCLUDE);
		if (iBut)
		{
			iBut->Enable(FALSE);
			ReleaseICustButton(iBut);
		}
		iBut = iWeightToolsParams->GetICustButton(IDC_INCLUDE);
		if (iBut)
		{
			iBut->Enable(FALSE);
			ReleaseICustButton(iBut);
		}
		iBut = iWeightToolsParams->GetICustButton(IDC_SELECT_EXCLUDED);
		if (iBut)
		{
			iBut->Enable(FALSE);
			ReleaseICustButton(iBut);
		}
		ReleaseICustToolbar(iWeightToolsParams);
	}

	if (iAbsolute!=NULL)
		iAbsolute->Disable();
	if (iEnvelope!=NULL)
		iEnvelope->Disable();
	if (iFalloff!=NULL)
		iFalloff->Disable();
	if (iCopy!=NULL)
		iCopy->Disable();
	if (iPaste!=NULL)
		iPaste->Disable();

	SpinnerOff(hParam,IDC_ERADIUSSPIN,IDC_ERADIUS);
	SpinnerOff(hParam,IDC_SQUASHSPIN,IDC_SQUASH);

	EnableWindow(GetDlgItem(hParam,IDC_NORMALIZE_CHECK),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_RIGID_CHECK),FALSE);
	EnableWindow(GetDlgItem(hParam,IDC_RIGIDHANDLES_CHECK),FALSE);

	EnableWindow(GetDlgItem(hParam,IDC_BACKFACECULL_CHECK),FALSE);
}

void BonesDefMod::SetVCMode()
{
	if (ip && (vcState == FALSE))
	{
		ModContextList list;
		INodeTab NodeTab;
		ip->GetModContexts(list,NodeTab);
		for( int i = 0 ; i < NodeTab.Count() ; i++)
		{
			NodeTab[i]->SetShadeCVerts(FALSE);// default display unshaded vertex color.
			NodeTab[i]->SetCVertMode(TRUE);
			NodeTab[i]->SetVertexColorType(nvct_color);
		}
	}
	vcState = TRUE;
}

void BonesDefMod::CollectVCData(Tab<vcSaveData>& vcDataTab)
{
	MyEnumProc dep;
	DoEnumDependents(&dep);

	//this gets the state of the node vertex color mods and shade state and stores them
	vcDataTab.ZeroCount();

	for (int  i = 0; i < dep.Nodes.Count(); i++)
	{
		vcSaveData tempVCData;
		tempVCData.node = dep.Nodes[i];
		tempVCData.shade = dep.Nodes[i]->GetShadeCVerts();
		tempVCData.vcmode = dep.Nodes[i]->GetCVertMode();
		tempVCData.type = dep.Nodes[i]->GetVertexColorType();

		vcDataTab.Append(1,&tempVCData);
	}
}

void BonesDefMod::SetVCData(Tab<vcSaveData>& vcDataTab)
{
	//this puts back the state in vcDataTab
	if (ip && (vcState == FALSE))
	{
		MyEnumProc dep;
		DoEnumDependents(&dep);
		for (int  i = 0; i < dep.Nodes.Count(); ++i)
		{
			INode *node = dep.Nodes[i];
			for (int  j = 0; j < vcDataTab.Count(); ++j)
			{
				//need to check to make sure the node still exist since the user can delete a node without leaving the subobj mode
				if (vcDataTab[j].node == node)
				{
					node->SetShadeCVerts(vcDataTab[j].shade);
					node->SetCVertMode(vcDataTab[j].vcmode);
					node->SetVertexColorType(vcDataTab[j].type);
					break;
				}
			}
		}
	}
	vcState = TRUE;
}

void BonesDefMod::RestoreVCMode()
{
	//this puts back the original state of the node vc mods and shade state
	if (vcState)
	{
		MyEnumProc dep;
		DoEnumDependents(&dep);
		for (int  i = 0; i < dep.Nodes.Count(); ++i)
		{
			INode *node = dep.Nodes[i];
			for (int  j = 0; j < vcSaveDataList.Count(); ++j)
			{
				//need to check to make sure the node still exist since the user can delete a node without leaving the subobj mode
				if (vcSaveDataList[j].node == node)
				{
					node->SetShadeCVerts(vcSaveDataList[j].shade);
					node->SetCVertMode(vcSaveDataList[j].vcmode);
					node->SetVertexColorType(vcSaveDataList[j].type);
					j = vcSaveDataList.Count();
				}
			}
		}
	}
	vcState = FALSE;
}

void BonesDefMod::ActivateSubobjSel(int level, XFormModes& modes)
{
	if (ip == nullptr)
		return;
	switch (level) {
	case 0:
		{
			RestoreVCMode();

			DisableButtons();
			iEditEnvelopes->SetCheck(FALSE);

			if (ip->GetCommandMode() == CrossSectionMode) {
				ip->SetStdCommandMode(CID_OBJMOVE);
			}
			if (pblock_gizmos->Count(skin_gizmos_list) > 0)
			{
				ReferenceTarget *ref;
				if (currentSelectedGizmo != -1)
				{
					ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
					GizmoClass *gizmo = (GizmoClass *)ref;
					if (gizmo)
					{
						gizmo->EndEditing();
						gizmo->EnableEditing(FALSE);
					}
				}
			}
			EnableRadius(FALSE);
			EnableEffect(FALSE);
			//MIRROR
			pblock_mirror->SetValue(skin_mirrorenabled,0,FALSE);
			mirrorData.EnableMirrorButton(FALSE);

			if (displayCallback != NULL)
			{
				INodeDisplayControl* ndc = GetNodeDisplayControl(ip);
				if (ndc)
				{
					ndc->SetNodeCallback(NULL);
					ndc->UnRegisterNodeDisplayCallback(displayCallback);
					delete displayCallback;
					displayCallback = NULL;
				}
			}

			if (pPainterInterface)
			{
				if (pPainterInterface->InPaintMode())
				{
					StartPaintMode();
					if (iPaintButton)
						iPaintButton->SetCheck(FALSE);
					ip->SetStdCommandMode(CID_OBJMOVE);
				}
			}

			break;
		}
	case 1: // Points
		{
			INodeDisplayControl* ndc = GetNodeDisplayControl(ip);
			//remove if need be

			if ( (displayCallback == NULL) && ndc)
			{
				displayCallback = new BonesDefNodeDisplayCallback();
				displayCallback->mod = this;
				ndc->RegisterNodeDisplayCallback(displayCallback);
				ndc->SetNodeCallback(displayCallback);
			}

			//MIRROR
			mirrorData.EnableMirrorButton(TRUE);

			SetupNamedSelDropDown();

			CollectVCData(vcSaveDataList);

			if (HasNonNullBones())
			{
				EnableButtons();
			}

			if ( (ModeBoneEnvelopeIndex != -1) && IsValidBoneIndex(ModeBoneIndex))
				EnableRadius(TRUE);
			else EnableRadius(FALSE);

			iEditEnvelopes->SetCheck(TRUE);

			if (pblock_gizmos->Count(skin_gizmos_list) > 0)
			{
				ReferenceTarget *ref;
				if (currentSelectedGizmo != -1)
				{
					ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,currentSelectedGizmo);
					GizmoClass *gizmo = (GizmoClass *)ref;
					if (gizmo)
					{
						gizmo->EnableEditing(TRUE);
					}
				}
			}

			ModContextList mcList;
			INodeTab nodes;

			ip->GetModContexts(mcList,nodes);
			int objects = mcList.Count();

			for ( int i = 0; i < objects; i++ )
			{
				BoneModData *bmd = (BoneModData*)mcList[i]->localData;
				UpdateEffectSpinner(bmd);
			}

			modes = XFormModes(moveMode,NULL,NULL,NULL,NULL,NULL);
			break;
		}
	}

	EnableWeightToolControls();
	NotifyDependents(FOREVER,PART_DISPLAY,REFMSG_CHANGE);
}

Interval BonesDefMod::LocalValidity(TimeValue t)
{
	// aszabo|feb.05.02 If we are being edited,
	// return NEVER to forces a cache to be built after previous modifier.
	if (TestAFlag(A_MOD_BEING_EDITED))
		return NEVER;
	return GetValidity(t);
}

//aszabo|feb.06.02 - When LocalValidity is called by ModifyObject,
// it returns NEVER and thus the object channels are marked non valid
// As a result, the mod stack enters and infinite evaluation of the modifier
// ModifyObject now calls GetValidity and CORE calls LocalValidity to
// allow for building a cache on the input of this modifier when it's
// being edited
Interval BonesDefMod::GetValidity(TimeValue t)
{
	Interval valid = FOREVER;
	for (int i =0;i<BoneData.Count();i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node != NULL)
		{
			boneData.Node->GetObjTMBeforeWSM(t,&valid);
			if (boneData.flags & BONE_SPLINE_FLAG)
			{
				ObjectState osp = boneData.Node->EvalWorldState (t);
				valid &= osp.obj->ObjectValidity (t);
			}
		}
	}
	for(int j = 0 ; j < pblock_gizmos->Count(skin_gizmos_list) ; j++)
	{
		ReferenceTarget *ref;
		ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,j);
		GizmoClass *gizmo = (GizmoClass *)ref;
		if (gizmo)
			valid &= gizmo->LocalValidity( t);
	}

	return valid;
}

Point3 BonesDefMod::VertexAnimation(TimeValue t, BoneModData * bmd, int vertex, int bone, Point3 p)
{
	Point3 ps(0.0f,0.0f,0.0f),pr(0.0f,0.0f,0.0f),pdef(0.0f,0.0f,0.0f),pt(0.0f,0.0f,0.0f);
	Point3 MovedU,MovedTan;

	int boneIndex = bmd->VertexData[vertex]->GetBoneIndex(bone);
	BoneDataClass& boneData = BoneData[boneIndex];
	if (boneData.Node == NULL) return p;

	ShapeObject *pathOb = NULL;
	Interface *ip = GetCOREInterface();
	ObjectState os = boneData.Node->EvalWorldState(t);
	pathOb = (ShapeObject*)os.obj;

	int cid = bmd->VertexData[vertex]->GetCurveID(bone);
	int sid = bmd->VertexData[vertex]->GetSegID(bone);
	float u = bmd->VertexData[vertex]->GetCurveU(bone);

	Matrix3 ntm = boneData.Node->GetObjectTM(t);
	Matrix3 tm    = Inverse(bmd->BaseTM * Inverse(ntm));

	if (pathOb->NumberOfCurves() == 0) return p;
	if (sid >= pathOb->NumberOfVertices(t,cid)) return p;

	MovedU = pathOb->InterpPiece3D(t, cid,sid,u );
	MovedU = MovedU * tm;
	MovedTan = pathOb->TangentPiece3D(t, cid, sid, u);
	MovedTan = VectorTransform(tm,MovedTan);

	Point3 OPoint;
	OPoint = bmd->VertexData[vertex]->GetOPoint(bone) * tm;
	Point3 OTan = VectorTransform(tm,bmd->VertexData[vertex]->GetTangent(bone));

	float s = 1.0f;  //scale
	float angle = 0.0f;
	float influ = RetrieveNormalizedWeight(bmd,vertex,bone);

	OTan = Normalize(OTan);
	MovedTan = Normalize(MovedTan);
	if ( OTan != MovedTan)
		angle = (float) acos(DotProd(OTan,MovedTan)) * influ;

	Point3 perp = CrossProd(OTan,MovedTan);
	Matrix3 RotateMe(1);

	RotateMe = RotAngleAxisMatrix(Normalize(perp), angle);

	ps = p-OPoint;
	pr = ps * RotateMe + OPoint;
	pt = (MovedU - OPoint) * influ;
	pdef = pr + pt;
	return pdef;
}

class BonesDefDeformer: public Deformer {
public:
	BonesDefMod *Cluster;
	BoneModData *bmd;
	TimeValue t;
	BonesDefDeformer(BonesDefMod *C, BoneModData *bm, TimeValue tv){Cluster = C;bmd = bm; t= tv;}
	Point3 Map(int i, Point3 p);
	//this is a fall back deformer in case the com engine fails
	Point3 Map2(int i, Point3 p)
	{
		if (bmd->VertexData.Count()>0)
		{
			if (bmd->VertexData[i]->WeightCount() > 0 )
			{
				Point3 tp(0.0f,0.0f,0.0f);
				float influence = 0.0f;
				if (bmd->VertexData[i]->WeightCount()==1)
				{
					Point3 vec;
					float influ = Cluster->RetrieveNormalizedWeight(bmd,i,0);

					vec = (p*Cluster->BoneData[bmd->VertexData[i]->GetBoneIndex(0)].temptm);
					vec = vec - p;
					vec = vec * influ;
					p += vec;
					int bid;
					bid = bmd->VertexData[i]->GetBoneIndex(0);
					if ((Cluster->BoneData[bid].flags & BONE_SPLINE_FLAG) && (influ != 0.0f))
					{
						p = Cluster->VertexAnimation(t,bmd,i,0,p);
					}

					return p;
				}
				for (int boneid=0;boneid<bmd->VertexData[i]->WeightCount();boneid++)
				{
					float influ = Cluster->RetrieveNormalizedWeight(bmd,i,boneid);

					if (influ != 0.0f)
					{
						tp  += (p*Cluster->BoneData[bmd->VertexData[i]->GetBoneIndex(boneid)].temptm)*influ;
						influence += influ;
					}
				}
				//do vertex snimation if it is a splineanimation

				for (int boneid=0;boneid<bmd->VertexData[i]->WeightCount();boneid++)
				{
					int bid;
					bid = bmd->VertexData[i]->GetBoneIndex(boneid);

					if (Cluster->BoneData[bid].flags & BONE_SPLINE_FLAG)
					{
						float influ = Cluster->RetrieveNormalizedWeight(bmd,i,boneid);

						if (influ != 0.0f)
						{
							tp = Cluster->VertexAnimation(t,bmd,i,boneid,tp);
						}
					}
				}

				if (influence > 0.00001)
					return tp;
			}
			else return p;
		}
		return p;
	}
};

class StaticBonesDefDeformer: public Deformer {
public:
	BonesDefMod *Cluster;
	BoneModData *bmd;
	TimeValue t;
	StaticBonesDefDeformer(BonesDefMod *C, BoneModData *bm, TimeValue tv){Cluster = C;bmd = bm; t= tv;}
	Point3 Map(int i, Point3 p) {
		if (bmd->VertexData.Count()>0)
		{
			if ( i <bmd->VertexData.Count())
				p = bmd->VertexData[i]->LocalPos;
		}
		return p;
	}
};

void BonesDefMod::RecomputeAllBones(BoneModData *bmd, TimeValue t, ObjectState *os)
{
	bmd->reevaluate = FALSE;

	int nv = os->obj->NumPoints();

	for (int i=0; i<bmd->VertexData.Count(); i++)
		bmd->VertexData[i]->SetClosestBone(-1);

	if ( (bmd->VertexData.Count() != nv) || (reset))
	{
		reset = FALSE;
		//temp point3 weight table
		Tab<VertexListClass*> newWeights;
		newWeights.SetCount(nv);
		for (int i=0; i<nv; i++)
		{
			newWeights[i] = NULL;
		}
		for (int i = 0; i < nv; i++)
		{
			Point3 p = os->obj->GetPoint(i);
			for (int j = 0; j < bmd->VertexData.Count(); j++)
			{
				if (bmd->VertexData[j]->LocalPos ==p)
				{
					if (bmd->VertexData[j]->IsModified())
					{
						VertexListClass *vc = new VertexListClass;
						*vc = *bmd->VertexData[j];
						newWeights[i] = vc;
					}
				}
			}
		}

		for (int i = 0; i < bmd->VertexData.Count(); i++)
		{
			if (bmd->VertexData[i] != NULL)
				delete (bmd->VertexData[i]);
			bmd->VertexData[i] = NULL;
		}

		bmd->VertexData.ZeroCount();
		bmd->VertexData.SetCount(nv);

		for (int i=0; i<nv; i++) {
			if (newWeights[i])
			{
				bmd->VertexData[i] = newWeights[i];
			}
			else
			{
				VertexListClass *vc = new VertexListClass;
				bmd->VertexData[i] = vc;
				bmd->VertexData[i]->Modified (FALSE);
				bmd->VertexData[i]->ZeroWeights();;
			}
		}
	}
	else
	{
		MaxSDK::PerformanceTools::ThreadTools threadData;
		int numThreads = threadData.GetNumberOfThreads(MaxSDK::PerformanceTools::ThreadTools::kDeformationThreading,bmd->VertexData.Count());
		omp_set_num_threads(numThreads);
#pragma omp parallel for
		for (int i = 0; i < bmd->VertexData.Count(); i++)
		{
			VertexListClass* pData = bmd->VertexData[i];
			if (!pData->IsModified ())
				pData->ZeroWeights();
			else
			{
				for (int boneid = 0; boneid < pData->WeightCount(); boneid++)
				{
					int boneIndex = pData->GetBoneIndex(boneid);
					if (!IsValidBoneIndex(boneIndex))
					{
						pData->DeleteWeight(boneid);
						boneid--;
						if (pData->WeightCount() == 0)
							break;
					}
				}
			}
		}
	}

	int bonecount = 0;
	int crosscount = 0;
	for (int i =0;i<BoneData.Count();i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node != NULL)
		{
			bonecount++;
			for (int ccount = 0; ccount < boneData.CrossSectionList.Count();ccount++)
				crosscount++;
		}
	}

	//build bounding box list for hit testing;
	Tab<Box3> BBoxList;
	BBoxList.ZeroCount();
	bmd->rebuildWeights = TRUE;
	for ( int i =0;i<BoneData.Count();i++)
	{
		Point3 l1,l2;

		Box3 b;
		float Outer,l = 0.0f;
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node != NULL)
		{
			if (boneData.flags & BONE_SPLINE_FLAG)
			{
				ObjectState tos;
				tos = boneData.Node->EvalWorldState(RefFrame);
				//get bounding box
				tos.obj->GetDeformBBox(RefFrame,b);

				Matrix3 ntm = Inverse(boneData.tm);
				b = b * ntm * bmd->InverseBaseTM;
			}
			else
			{
				GetEndPoints(bmd,RefFrame,l1, l2, i);

				b.Init();
				b.MakeCube(l1,1.0f);
				b += l2;
			}
			for (int ccount = 0; ccount < boneData.CrossSectionList.Count();ccount++)
			{
				float inner;

				GetCrossSectionRanges(inner, Outer, i, ccount);
				if (inner>Outer) Outer  = inner;
				if (Outer > l ) l = Outer;
			}
			b.EnlargeBy(l);
		}

		BBoxList.Append(1,&b,1);
	}

	//Get largest radius

	//New Fallof method
	int numThreads = 1;
	MaxSDK::PerformanceTools::ThreadTools threadData;
	if (os->obj->IsSubClassOf(triObjectClassID)
		|| os->obj->IsSubClassOf(polyObjectClassID))
	{
		numThreads = threadData.GetNumberOfThreads(MaxSDK::PerformanceTools::ThreadTools::kDeformationThreading,nv);
	}
	omp_set_num_threads(numThreads);
#pragma omp parallel for schedule(static, nv / numThreads)
	for (int i=0; i<nv; i++) {
		//get total distance

		float TotalDistance = 0.0f;
		Point3 p,BoneCenter;
		if (!bmd->VertexData[i]->IsModified())
		{
			p = os->obj->GetPoint(i);

			for (int boneid =0;boneid<BoneData.Count();boneid++)
			{
				BoneDataClass& boneData = BoneData[boneid];
				BOOL excluded = FALSE;
				if (boneid < bmd->exclusionList.Count())
				{
					if (bmd->exclusionList[boneid])
					{
						if (bmd->isExcluded(boneid,i))
						{
							excluded = TRUE;
							break;
						}
					}
				}
				if ((boneData.Node != NULL) && (!excluded))
				{
					if (BBoxList[boneid].Contains(p))
					{
						float Influence = 1.0f;
						Point3 l1,l2;
						GetEndPoints(bmd,t,l1, l2, boneid);

						float LineU,SplineU = 0.0f;
						Point3 op,otan;
						int sid = 0;
						int cid = 0;
						if ((boneData.flags & BONE_SPLINE_FLAG) && (boneData.Node != NULL))
						{
							Matrix3 ntm = boneData.tm;
							ntm =bmd->BaseTM * ntm;

							Influence = SplineToPoint(p,
								&boneData.referenceSpline,
								LineU,op,otan,cid,sid,ntm);

							SplineU = LineU;
						}
						else
						{
							Influence = LineToPoint(p,l1,l2,LineU);
						}

						//find cross section that bound this point
						int StartCross = 0, EndCross = 0;
						float tu = ModifyU(t,LineU,  boneid, sid);

						for (int ccount = 0; ccount < boneData.CrossSectionList.Count();ccount++)
						{
							if (boneData.CrossSectionList[ccount].u>=tu)
							{
								EndCross =ccount;
								ccount = boneData.CrossSectionList.Count();
							}
						}
						StartCross = EndCross -1;

						if (StartCross == -1)
						{
							StartCross = 0;
							EndCross++;
						}
						Influence = ComputeInfluence(t,Influence,LineU, boneid,StartCross, EndCross,sid);

						if (Influence != 0.0f)
						{
							VertexInfluenceListClass td;
							td.Bones = boneid;
							td.Influences = Influence;

							td.u = SplineU;
							td.Tangents = otan;
							td.OPoints = op;
							td.SubCurveIds = cid;
							td.SubSegIds = sid;
							if (!(boneData.flags & BONE_LOCK_FLAG))
							{
								int found = -1;
								for (int vdcount = 0; vdcount < bmd->VertexData[i]->WeightCount();vdcount++)
								{
									if (bmd->VertexData[i]->GetBoneIndex(vdcount) == boneid)
									{
										bmd->VertexData[i]->PasteSingleWeight(vdcount, td);
										found = bmd->VertexData[i]->WeightCount();
										break;
									}
								}
								if (found == -1)
									bmd->VertexData[i]->AppendWeight(td);
							}
						}
						else
						{
							for (int vdcount = 0; vdcount < bmd->VertexData[i]->WeightCount();vdcount++)
							{
								if (bmd->VertexData[i]->GetBoneIndex(vdcount) == boneid)
								{
									bmd->VertexData[i]->DeleteWeight(vdcount);
									vdcount = bmd->VertexData[i]->WeightCount();
								}
							}
						}
					}

					else
					{
						if (!(boneData.flags & BONE_LOCK_FLAG))
						{
							for (int vdcount = 0; vdcount < bmd->VertexData[i]->WeightCount();vdcount++)
							{
								if (bmd->VertexData[i]->GetBoneIndex(vdcount) == boneid)
								{
									bmd->VertexData[i]->DeleteWeight(vdcount);
									vdcount = bmd->VertexData[i]->WeightCount();
								}
							}
						}
					}
				}
				else
				{
					for (int vdcount = 0; vdcount < bmd->VertexData[i]->WeightCount();vdcount++)
					{
						if (bmd->VertexData[i]->GetBoneIndex(vdcount) == boneid)
						{
							bmd->VertexData[i]->DeleteWeight(vdcount);
							vdcount = bmd->VertexData[i]->WeightCount();
						}
					}
				}
			}
		}
	}

	BOOL weightAll = FALSE;
	pblock_advance->GetValue(skin_advance_weightall,0,weightAll,FOREVER);

	if (weightAll)
	{
		for (int i=0; i<nv; i++)
		{
			if (!bmd->VertexData[i]->IsModified() && (bmd->VertexData[i]->WeightCount() == 0))
			{
				int cachedClosestBone = bmd->VertexData[i]->GetClosestBoneCache();
				if (cachedClosestBone != -1 && IsValidBoneIndex(cachedClosestBone))
				{
					bmd->VertexData[i]->SetClosestBone(cachedClosestBone);
				}
				else
				{
					Point3 p = os->obj->GetPoint(i);
					int closestBone = -1;
					float closestDist = 0.0f;
					for (int boneid =0;boneid<BoneData.Count();boneid++)
					{
						BoneDataClass& boneData = BoneData[boneid];
						if ((boneData.Node != NULL) && (!(boneData.flags & BONE_SPLINE_FLAG)))
						{
							Point3 l1,l2;
							GetEndPoints(bmd,t,l1, l2, boneid);
							float LineU;
							float influence = LineToPoint(p,l1,l2,LineU);
							if ((influence < closestDist) || (closestBone == -1))
							{
								closestDist = influence;
								closestBone = boneid;
							}
						}
					}
					bmd->VertexData[i]->SetClosestBone(closestBone);
					bmd->VertexData[i]->SetClosestBoneCache(closestBone);
				}
			}
		}
	}
}

void BonesDefMod::RecomputeBone(BoneModData *bmd, int BoneIndex, TimeValue t, ObjectState *os)
{
	if (!IsValidBoneIndex(BoneIndex))
		return;

	bmd->reevaluateActiveBone= FALSE;
	BoneDataClass& boneData = BoneData[BoneIndex];

	for (int i=0; i<bmd->VertexData.Count(); i++)
		bmd->VertexData[i]->SetClosestBone(-1);

	BuildCache(bmd, BoneIndex,  t, os);
	bmd->reevaluate = FALSE;
	bmd->rebuildWeights = TRUE;
	int nv = os->obj->NumPoints();

	//Get largest radius

	//New Fallof method
	for (int i=0; i<nv; i++) 
	{
		if (!bmd->VertexData[i]->IsModified())
		{
			Point3 p = os->obj->GetPoint(i);
			BOOL excluded = FALSE;
			if (BoneIndex < bmd->exclusionList.Count())
			{
				if (bmd->exclusionList[BoneIndex])
				{
					if (bmd->isExcluded(BoneIndex,i))
					{
						excluded = TRUE;
						break;
					}
				}
			}

			if (!excluded)
			{
				Point3 l1,l2;
				GetEndPoints(bmd,t,l1, l2, BoneIndex);

				float Influence = bmd->DistCache[i].dist;
				float LineU = bmd->DistCache[i].u;
				float SplineU = LineU;
				int cid = bmd->DistCache[i].SubCurveIds;
				int sid = bmd->DistCache[i].SubSegIds;
				Point3 otan = bmd->DistCache[i].Tangents;
				Point3 op = bmd->DistCache[i].OPoints;
				//find cross section that bound this point
				int StartCross = 0, EndCross = 0;
				float tu = ModifyU(t,LineU,  BoneIndex, sid);

				for (int ccount = 0; ccount < boneData.CrossSectionList.Count();ccount++)
				{
					if (boneData.CrossSectionList[ccount].u>=tu)
					{
						EndCross =ccount;
						ccount = boneData.CrossSectionList.Count();
					}
				}
				StartCross = EndCross -1;

				if (StartCross == -1)
				{
					StartCross = 0;
					EndCross++;
				}

				Influence = ComputeInfluence(t,Influence,LineU, BoneIndex,StartCross, EndCross, sid);

				if (Influence != 0.0f)
				{
					VertexInfluenceListClass td;
					td.Bones = BoneIndex;
					td.Influences = Influence;
					td.u = SplineU;
					td.Tangents = otan;
					td.OPoints = op;
					td.SubCurveIds = cid;
					td.SubSegIds = sid;

					BOOL found = FALSE;
					if (!(boneData.flags & BONE_LOCK_FLAG))
					{
						for (int bic = 0; bic < bmd->VertexData[i]->WeightCount(); bic++)
						{
							if (bmd->VertexData[i]->GetBoneIndex(bic) == BoneIndex)
							{
								bmd->VertexData[i]->PasteSingleWeight(bic, td);
								found = TRUE;
								break;
							}
						}
						if (!found)
							bmd->VertexData[i]->AppendWeight(td);
					}
				}
				else
				{
					if (!(boneData.flags & BONE_LOCK_FLAG))
					{
						BOOL found = FALSE;
						for (int bic = 0; bic < bmd->VertexData[i]->WeightCount(); bic++)
						{
							if (bmd->VertexData[i]->GetBoneIndex(bic) == BoneIndex)
							{
								bmd->VertexData[i]->DeleteWeight(bic);
								found = TRUE;
								break;
							}
						}
					}
				}
			}
		}
	}

	BOOL weightAll = FALSE;
	pblock_advance->GetValue(skin_advance_weightall,0,weightAll,FOREVER);

	if (weightAll)
	{
		for (int i=0; i<nv; i++)
		{
			if (!bmd->VertexData[i]->IsModified() && (bmd->VertexData[i]->WeightCount() == 0))
			{
				int cachedClosestBone = bmd->VertexData[i]->GetClosestBoneCache();
				if (cachedClosestBone != -1 && IsValidBoneIndex(cachedClosestBone))
				{
					bmd->VertexData[i]->SetClosestBone(cachedClosestBone);
				}
				else
				{
					Point3 p = os->obj->GetPoint(i);
					int closestBone = -1;
					float closestDist = 0.0f;
					for (int boneid =0;boneid<BoneData.Count();boneid++)
					{
						BoneDataClass& boneData = BoneData[boneid];
						if ((boneData.Node != NULL) && (!(boneData.flags & BONE_SPLINE_FLAG)))
						{
							Point3 l1,l2;
							GetEndPoints(bmd,t,l1, l2, boneid);
							float LineU;
							float influence = LineToPoint(p,l1,l2,LineU);
							if ((influence < closestDist) || (closestBone == -1))
							{
								closestDist = influence;
								closestBone = boneid;
							}
						}
					}
					bmd->VertexData[i]->SetClosestBone(closestBone);
					bmd->VertexData[i]->SetClosestBoneCache(closestBone);
				}
			}
		}
	}
}

void BonesDefMod::UnlockBone(BoneModData *bmd,TimeValue t, ObjectState *os)
{
	//loop through verts and remove and associations to thos bone
	for (int i=0;i<bmd->VertexData.Count();i++)
	{
		for (int bic = 0; bic < bmd->VertexData[i]->WeightCount(); bic++)
		{
			if (bmd->VertexData[i]->GetBoneIndex(bic) == ModeBoneIndex)
			{
				bmd->VertexData[i]->DeleteWeight(bic);
				break;
			}
		}
	}
	//loop through find all verts in radius and set them to unmodified

	if (!IsValidBoneIndex(ModeBoneIndex))
		return;
	BoneDataClass& boneData = BoneData[ModeBoneIndex];

	cacheValid = FALSE;
	bmd->CurrentCachePiece = -1;

	BuildCache(bmd, ModeBoneIndex,  t, os);
	bmd->reevaluate=TRUE;
	int nv =  os->obj->NumPoints();

	//Get largest radius

	//New Fallof method
	for (int i=0; i<nv; i++) {
		//get total distance
		float TotalDistance = 0.0f;
		Point3 p,BoneCenter;
		p = os->obj->GetPoint(i);

		int FullStrength =0;

		BOOL excluded = FALSE;
		if (ModeBoneIndex < bmd->exclusionList.Count())
		{
			if (bmd->exclusionList[ModeBoneIndex])
			{
				if (bmd->isExcluded(ModeBoneIndex,i))
				{
					excluded = TRUE;
					break;
				}
			}
		}

		if (!excluded)
		{
			float Influence = 1.0f;
			Point3 l1,l2;

			GetEndPoints(bmd,t,l1, l2, ModeBoneIndex);

			float LineU,SplineU = 0.0f;
			Point3 op,otan;
			int cid,sid;
			Influence = bmd->DistCache[i].dist;
			LineU = bmd->DistCache[i].u;
			SplineU = LineU;
			cid = bmd->DistCache[i].SubCurveIds;
			sid = bmd->DistCache[i].SubSegIds;
			otan = bmd->DistCache[i].Tangents;
			op = bmd->DistCache[i].OPoints;
			//find cross section that bound this point
			int StartCross = 0, EndCross = 0;
			float tu = ModifyU(t,LineU,  ModeBoneIndex, sid);

			for (int ccount = 0; ccount < boneData.CrossSectionList.Count();ccount++)
			{
				if (boneData.CrossSectionList[ccount].u>=tu)
				{
					EndCross =ccount;
					break;
				}
			}
			StartCross = EndCross -1;

			if (StartCross == -1)
			{
				StartCross = 0;
				EndCross++;
			}

			Influence = ComputeInfluence(t,Influence,LineU, ModeBoneIndex,StartCross, EndCross, sid);
			if (Influence > 0.0f)
			{
				bmd->VertexData[i]->Modified(FALSE);
			}
		}
	}
}

void BonesDefMod::UnlockAllBones(BoneModData *bmd,TimeValue t, ObjectState *os)
{
	//loop through verts and remove and associations to thos bone
	for (int boneid = 0; boneid < BoneData.Count(); boneid++)
	{
		BoneDataClass& boneData = BoneData[boneid];
		if (boneData.Node)
		{
			for (int i=0;i<bmd->VertexData.Count();i++)
			{
				for (int bic = 0; bic < bmd->VertexData[i]->WeightCount(); bic++)
				{
					if (bmd->VertexData[i]->GetBoneIndex(bic) == boneid)
					{
						bmd->VertexData[i]->DeleteWeight(bic);
						break;
					}
				}
			}
			//loop through find all verts in radius and set them to unmodified

			cacheValid = FALSE;
			bmd->CurrentCachePiece = -1;

			BuildCache(bmd, boneid,  t, os);
			bmd->reevaluate=TRUE;
			int nv =  os->obj->NumPoints();

			//Get largest radius

			//New Fallof method
			for (int i=0; i<nv; i++) {
				//get total distance
				float TotalDistance = 0.0f;
				Point3 p,BoneCenter;
				p = os->obj->GetPoint(i);

				int FullStrength =0;

				BOOL excluded = FALSE;
				if (boneid < bmd->exclusionList.Count())
				{
					if (bmd->exclusionList[boneid])
					{
						if (bmd->isExcluded(boneid,i))
						{
							excluded = TRUE;
							break;
						}
					}
				}

				if (!excluded)
				{
					float Influence = 1.0f;
					Point3 l1,l2;

					GetEndPoints(bmd,t,l1, l2, boneid);

					float LineU,SplineU = 0.0f;
					Point3 op,otan;
					int cid,sid;
					Influence = bmd->DistCache[i].dist;
					LineU = bmd->DistCache[i].u;
					SplineU = LineU;
					cid = bmd->DistCache[i].SubCurveIds;
					sid = bmd->DistCache[i].SubSegIds;
					otan = bmd->DistCache[i].Tangents;
					op = bmd->DistCache[i].OPoints;
					//find cross section that bound this point
					int StartCross = 0, EndCross = 0;
					float tu = ModifyU(t,LineU,  boneid, sid);

					for (int ccount = 0; ccount < boneData.CrossSectionList.Count();ccount++)
					{
						if (boneData.CrossSectionList[ccount].u>=tu)
						{
							EndCross =ccount;
							break;
						}
					}
					StartCross = EndCross -1;

					if (StartCross == -1)
					{
						StartCross = 0;
						EndCross++;
					}

					Influence = ComputeInfluence(t,Influence,LineU, boneid,StartCross, EndCross, sid);
					if (Influence > 0.0f)
					{
						bmd->VertexData[i]->Modified(FALSE);
					}
				}
			}
		}
	}
}

void BonesDefMod::UpdateTMCacheTable(BoneModData *bmd, TimeValue t, Interval& valid,BOOL forceCompleteUpdate)
{
	if (bmd == NULL) return;

	if (bmd->tempTableL1.Count() != BoneData.Count())
	{
		bmd->tempTableL1.SetCount(BoneData.Count());
		forceCompleteUpdate = TRUE;
	}
	if (bmd->tempTableL2.Count() != BoneData.Count())
	{
		bmd->tempTableL2.SetCount(BoneData.Count());
		forceCompleteUpdate = TRUE;
	}
	if (bmd->tempTableL1ObjectSpace.Count() != BoneData.Count())
	{
		bmd->tempTableL1ObjectSpace.SetCount(BoneData.Count());
		forceCompleteUpdate = TRUE;
	}
	if (bmd->tempTableL2ObjectSpace.Count() != BoneData.Count())
	{
		bmd->tempTableL2ObjectSpace.SetCount(BoneData.Count());
		forceCompleteUpdate = TRUE;
	}

	if (bmd->tmCacheToBoneSpace.Count() != BoneData.Count())
	{
		bmd->tmCacheToBoneSpace.SetCount(BoneData.Count());
		forceCompleteUpdate = TRUE;
	}
	if (bmd->tmCacheToObjectSpace.Count() != BoneData.Count())
	{
		bmd->tmCacheToObjectSpace.SetCount(BoneData.Count());
		forceCompleteUpdate = TRUE;
	}
	if ((editing) || (bmd->pSE==NULL) || (bmd->reevaluate)  || forceCompleteUpdate)
	{
		for (int boneid =0;boneid<BoneData.Count();boneid++)
		{
			BoneDataClass& boneData = BoneData[boneid];
			if (boneData.Node != NULL)
			{
				Point3 l1, l2;

				Interval v;
				boneData.EndPoint1Control->GetValue(currentTime,&l1,v);
				boneData.EndPoint2Control->GetValue(currentTime,&l2,v);

				//new opt

				Matrix3 inverseBoneDataTm = Inverse(boneData.tm)* bmd->InverseBaseTM;
				bmd->tempTableL1[boneid] = l1* inverseBoneDataTm ;
				bmd->tempTableL2[boneid] = l2* inverseBoneDataTm;

				//optimize this we really only need to update the cache when a bone is added or when a bone is moved or time has changed
				if ((bmd->lastTMCacheTime != t) || forceCompleteUpdate || (bmd->reevaluate) )
				{
					//if BID then
					Matrix3 ntm;
					ntm = boneData.Node->GetObjTMBeforeWSM(t,&valid);

					Matrix3 ntmByInverseBaseTM;
					ntmByInverseBaseTM;// = ntm * bmd->InverseBaseTM;
					Matrix3 tmCacheToBoneSpace;
					bmd->baseNodeOffsetTM = bmd->BaseTM;
					if (bindNode)
					{
						xRefTM = bindNode->GetObjectTM(t);

						boneData.temptm = bmd->BaseTM * boneData.tm * ntm *
							initialXRefTM * Inverse(xRefTM) * bmd->InverseBaseTM;
						ntmByInverseBaseTM = ntm * initialXRefTM * Inverse(xRefTM) * bmd->InverseBaseTM;
						tmCacheToBoneSpace = bmd->BaseTM * initialXRefTM * Inverse(xRefTM) * Inverse(ntm);
					}

					else if ( (backTransform) && ( bmd->meshNode))
					{
						Matrix3 backTM = bmd->meshNode->GetNodeTM(t);

						boneData.temptm = bmd->BaseTM * boneData.tm * ntm *
							bmd->BaseNodeTM * Inverse(backTM) * bmd->InverseBaseTM;

						ntmByInverseBaseTM = ntm * Inverse(backTM) * bmd->BaseNodeTM  * bmd->InverseBaseTM;  //fix for envelopes not in the right pos
						tmCacheToBoneSpace = bmd->BaseTM   * Inverse(bmd->BaseNodeTM * Inverse(backTM))  * Inverse(ntm);
						Matrix3 tempTMCacheToBoneSpace = bmd->BaseTM  *  Inverse(ntm);
						bmd->baseNodeOffsetTM = backTM;
					}

					else
					{
						ntmByInverseBaseTM = ntm * bmd->InverseBaseTM;
						boneData.temptm = bmd->BaseTM * boneData.tm * ntmByInverseBaseTM;
						tmCacheToBoneSpace = bmd->BaseTM * Inverse(ntm);
					}

					//cache for for matrices
					bmd->tmCacheToBoneSpace[boneid] = tmCacheToBoneSpace;
					//new opt
					bmd->tmCacheToObjectSpace[boneid] = ntmByInverseBaseTM;
				}

				bmd->tempTableL1ObjectSpace[boneid] = l1 * bmd->tmCacheToObjectSpace[boneid];
				bmd->tempTableL2ObjectSpace[boneid] = l2 * bmd->tmCacheToObjectSpace[boneid];
			}
		}
		bmd->lastTMCacheTime = t;
	}

	if ((backTransform) && ( bmd->meshNode))
	{
		Matrix3 baseNodeObjectTM(1);

		baseNodeObjectTM = bmd->meshNode->GetObjectTM(t);

		bmd->gizmoPutBackDoubleOffset = baseNodeObjectTM * Inverse(bmd->BaseTM);
		bmd->gizmoRemoveDoubleOffset =  bmd->BaseTM * Inverse(baseNodeObjectTM);
	}
	else if (bindNode)
	{
		Matrix3 baseNodeObjectTM(1);

		baseNodeObjectTM = bindNode->GetObjectTM(t);

		bmd->gizmoPutBackDoubleOffset = baseNodeObjectTM * Inverse(bmd->BaseTM);
		bmd->gizmoRemoveDoubleOffset =  bmd->BaseTM * Inverse(baseNodeObjectTM);
	}

	else
	{
		bmd->gizmoPutBackDoubleOffset.IdentityMatrix();
		bmd->gizmoRemoveDoubleOffset.IdentityMatrix();
	}
}

class XRefEnumProc : public DependentEnumProc
{
public :
	XRefEnumProc() { nukeME = FALSE; }
	virtual int proc(ReferenceMaker *rmaker);
	INodeTab Nodes;
	BOOL nukeME;
};

int XRefEnumProc::proc(ReferenceMaker *rmaker)
{
	if (rmaker->SuperClassID()==BASENODE_CLASS_ID)
	{
		Nodes.Append(1, (INode **)&rmaker);
		nukeME = TRUE;
		return DEP_ENUM_SKIP;
	}

	return DEP_ENUM_CONTINUE;
}

BOOL RecurseXRefTree(INode *n, INode *target)
{
	for (int i = 0; i < n->NumberOfChildren(); i++)
	{
		INode *child = n->GetChildNode(i);

		if (child == target)
		{
			return TRUE;
		}
		else RecurseXRefTree(child,target);
	}
	return FALSE;
}

void BonesDefMod::NotifyInputChanged(const Interval& changeInt, PartID partID, RefMessage message, ModContext *mc)
{
	if (!mc->localData) return;
	BoneModData *bmd = (BoneModData *) mc->localData;
	switch (message) {
	case REFMSG_OBJECT_CACHE_DUMPED:
		{
			bmd->mCacheDumped = true;
			if (partID & PART_TOPO)
				if (!bmd->inputObjectIsNURBS)
					bmd->reevaluate=TRUE;
		}
	}
}

void BonesDefMod::UpdateEndPointDelta()
{
	int deform;
	Interval iv;
	pblock_advance->GetValue(skin_advance_always_deform,0,deform,iv);
	endPointDelta.SetCount(BoneData.Count());
	if (!deform)
	{
		for (int boneid =0;boneid<BoneData.Count();boneid++)
		{
			BoneDataClass& boneData = BoneData[boneid];
			if (boneData.Node != NULL)
			{
				Class_ID bid(BONE_CLASS_ID,0);
				Matrix3 ntm =boneData.Node->GetObjectTM(RefFrame);

				boneData.InitObjectTM = ntm;
				boneData.tm = Inverse(ntm);

				boneData.InitStretchTM = boneData.Node->GetStretchTM(RefFrame);
				if (hasStretchTM)
					boneData.InitNodeTM = boneData.Node->GetNodeTM(RefFrame);
				else boneData.InitNodeTM = boneData.InitStretchTM * boneData.Node->GetNodeTM(RefFrame);

				//copy initial reference spline into our spline
				ObjectState sos = boneData.Node->EvalWorldState(RefFrame);
				if (sos.obj->ClassID() == bid)
				{
					//now need to look at child and move
					//loop through children looking for a matching name
					int childCount;
					childCount = boneData.Node->NumberOfChildren();
					INode *childNode = NULL;
					for (int ci = 0; ci < childCount; ci++)
					{
						childNode = boneData.Node->GetChildNode(ci);
						TSTR childName;
						childName = childNode->GetName();
						if (childName == boneData.name)
						{
							Point3 l2(0.0f,0.0f,0.0f);
							Matrix3 ChildTM = childNode->GetObjectTM(RefFrame);
							l2 = l2 * ChildTM;
							l2 = l2 * boneData.tm;
							Point3 d;
							boneData.EndPoint2Control->GetValue(currentTime,&d,iv,CTRL_ABSOLUTE);
							ci = childCount;
							endPointDelta[boneid] = d-l2;
						}
					}
				}
			}
		}
	}
}

void BonesDefMod::ShadeVerts(Mesh *msh, BoneModData *bmd)
{
	if (ip)
	{
		BOOL showAllWeights = FALSE;
		Point3 colorList[100];
		pblock_display->GetValue(skin_display_colorallweights,0,showAllWeights,FOREVER);
		int ct = pblock_display->Count(skin_display_weightcolors);
		for (int i = 0; i < ct; i++)
			pblock_display->GetValue(skin_display_weightcolors,0,colorList[i],FOREVER,i);

		int nv = msh->getNumVerts();
		msh->setMapSupport(0, TRUE);
		msh->setNumVertCol(nv);
		int nf = msh->getNumFaces();
		Point3 selSoft = GetUIColor(COLOR_SUBSELECTION_SOFT);
		Point3 selMedium = GetUIColor(COLOR_SUBSELECTION_MEDIUM);
		Point3 selHard = GetUIColor(COLOR_SUBSELECTION_HARD);

		Point3 selSubColor = GetUIColor(COLOR_SUBSELECTION);

		Point3 noColor = ColorMan()->GetColorAsPoint3(NOWEIGHTCOLORID);

		int vct = bmd->VertexData.Count();

		Point3 *vc = msh->vertCol;

		//put in mirror check here
		int mirroredBone = -1;
		if (pPainterInterface)
		{
			if (pPainterInterface->GetMirrorEnable() && pPainterInterface->InPaintMode())
			{
				mirroredBone = mirrorIndex;
			}
		}

		for (int i=0;i<vct;i++)
		{
			VertexListClass *vd = bmd->VertexData[i];
			*vc = noColor;
			if (showAllWeights && (ct > 0))
			{
				Point3 c;
				c.x = 0.0f;
				c.y = 0.0f;
				c.z = 0.0f;

				if (rigidVerts)
				{
					int rigidBoneID;
					rigidBoneID = bmd->VertexData[i]->GetMostAffectedBone();
					c = colorList[rigidBoneID%ct];
				}
				else
				{
					for (int boneid=0;boneid<bmd->VertexData[i]->WeightCount();boneid++)
					{
						float infl = vd->GetNormalizedWeight(boneid);
						int boneID = vd->GetBoneIndex(boneid);
						if ( boneID == ModeBoneIndex)
						{
							Point3 color = colorList[boneID%ct];
							color.x *= 1.1f;
							color.y *= 1.1f;
							color.z *= 1.1f;
							c += color * infl;
						}
						else c += colorList[boneID%ct] * infl;
					}
				}
				*vc = c;
				vc++;
			}
			else
			{
				int wct = vd->WeightCount();
				for (int boneid=0;boneid< wct;boneid++)
				{
					int rigidBoneID = 0;
					if (rigidVerts)
						rigidBoneID = vd->GetMostAffectedBone();

					int bid = vd->GetBoneIndex(boneid);

					if ( ( (bid == ModeBoneIndex) || (bid == mirroredBone))		 )
					{
						float infl = vd->GetNormalizedWeight(boneid);
						if ((infl > 0.0f) && (!((rigidVerts) && (ModeBoneIndex != rigidBoneID))))
						{
							boneid = wct;
							Color selColor(1.0f,1.0f,1.0f);

							if (infl > 0.0f)
							{
								if ( (infl<0.33f) && (infl > 0.0f))
								{
									selColor = selSoft + ( (selMedium-selSoft) * (infl/0.33f));
								}
								else if (infl <.66f)
								{
									selColor = selMedium + ( (selHard-selMedium) * ((infl-0.1f)/0.66f));
								}
								else if (infl < 0.99f)
								{
									selColor = selHard + ( (selSubColor-selHard) * ((infl-0.66f)/0.33f));
								}

								else
								{
									selColor = selSubColor;
								}
							}

							*vc = selColor;
						}
					}
				}
				vc++;
			}
		}

		Face *f =  msh->faces;
		TVFace *uvf = msh->vcFace;
		for (int i=0; i < nf; i++)
		{
			uvf->t[0] = f->v[0];
			uvf->t[1] = f->v[1];
			uvf->t[2] = f->v[2];
			uvf++;
			f++;
		}
	}
}

#ifndef NO_PATCHES

void BonesDefMod::ShadeVerts(PatchMesh *msh, BoneModData *bmd)
{
	{
		if (ip)
		{
			//get from mesh based on cahne
			PatchTVert *tVerts = NULL;
			TVPatch *tvFace = NULL;
			int CurrentChannel=0;
			if (!msh->getMapSupport(CurrentChannel))
			{
				msh->setNumMaps(CurrentChannel+1);
			}

			int nv = msh->numVerts;

			msh->setNumMapPatches(CurrentChannel,msh->getNumPatches());
			msh->setNumMapVerts (CurrentChannel,nv);

			BOOL showAllWeights = FALSE;
			Point3 colorList[100];
			pblock_display->GetValue(skin_display_colorallweights,0,showAllWeights,FOREVER);
			int ct = pblock_display->Count(skin_display_weightcolors);
			for (int i = 0; i < ct; i++)
				pblock_display->GetValue(skin_display_weightcolors,0,colorList[i],FOREVER,i);

			tVerts = msh->tVerts[CurrentChannel];
			tvFace = msh->tvPatches[CurrentChannel];

			int nf = msh->numPatches;
			Point3 selSoft = GetUIColor(COLOR_SUBSELECTION_SOFT);
			Point3 selMedium = GetUIColor(COLOR_SUBSELECTION_MEDIUM);
			Point3 selHard = GetUIColor(COLOR_SUBSELECTION_HARD);
			Point3 selSubColor = GetUIColor(COLOR_SUBSELECTION);

			Point3 noColor = ColorMan()->GetColorAsPoint3(NOWEIGHTCOLORID);

			int mirroredBone = -1;
			if (pPainterInterface)
			{
				if (pPainterInterface->GetMirrorEnable() && pPainterInterface->InPaintMode())
				{
					mirroredBone = mirrorIndex;
				}
			}

			int vct = bmd->VertexData.Count();
			for (int i=0; i<vct; i++)
			{
				VertexListClass *vd = bmd->VertexData[i];
				Point3 col = noColor;

				if (showAllWeights && (ct > 0))
				{
					Point3 c;
					c.x = 0.0f;
					c.y = 0.0f;
					c.z = 0.0f;

					if (rigidVerts)
					{
						int rigidBoneID;
						rigidBoneID = vd->GetMostAffectedBone();
						c = colorList[rigidBoneID%ct];
					}
					else
					{
						for (int boneid=0;boneid<vd->WeightCount();boneid++)
						{
							float infl = vd->GetNormalizedWeight(boneid);
							int boneID = vd->GetBoneIndex(boneid);
							if ( boneID == ModeBoneIndex)
							{
								Point3 color = colorList[boneID%ct];
								color.x *= 1.1f;
								color.y *= 1.1f;
								color.z *= 1.1f;
								c += color * infl;
							}
							else c += colorList[boneID%ct] * infl;
						}
					}
					if ( i < nv)
						tVerts[i] = c;
				}
				else
				{
					if ( i < nv)
						tVerts[i] = col;
					int bct = vd->WeightCount();
					for (int boneid=0;boneid<bct;boneid++)
					{
						int rigidBoneID = 0;
						if (rigidVerts)
							rigidBoneID = vd->GetMostAffectedBone();
						//put in mirror check here

						int bid = vd->GetBoneIndex(boneid);
						if ( ( (bid == ModeBoneIndex) || (bid == mirroredBone))
							&& (vd->GetWeight(boneid) != 0.0f)  )
						{
							if (!((rigidVerts) && (ModeBoneIndex != rigidBoneID)))
							{
								Point3 pt;
								float infl;

								infl = vd->GetNormalizedWeight(boneid);
								boneid = bct;
								Color selColor(1.0f,1.0f,1.0f);
								if (infl > 0.0f)
								{
									if ( (infl<0.33f) && (infl > 0.0f))
									{
										selColor = selSoft + ( (selMedium-selSoft) * (infl/0.33f));
									}
									else if (infl <.66f)
									{
										selColor = selMedium + ( (selHard-selMedium) * ((infl-0.1f)/0.66f));
									}
									else if (infl < 1.0f)
									{
										selColor = selHard + ( (selSubColor-selHard) * ((infl-0.66f)/0.33f));
									}

									else
									{
										selColor = selSubColor;
									}
								}

								if ( i < nv)
									tVerts[i]=selColor;
							}
						}
					}
				}
			}

			for (int i=0; i < nf; i++)
			{
				tvFace[i].tv[0] = msh->patches[i].v[0];
				tvFace[i].tv[1] = msh->patches[i].v[1];
				tvFace[i].tv[2] = msh->patches[i].v[2];
				tvFace[i].tv[3] = msh->patches[i].v[3];
			}
		}
	}
}
#endif

void BonesDefMod::ShadeVerts(MNMesh *msh, BoneModData *bmd)
{
	{
		if (ip)
		{
			//get from mesh based on cahne
			Point3 *tVerts = NULL;
			MNMapFace *tvFace = NULL;
			int CurrentChannel=0;

			int numMaps = msh->MNum ();
			if (CurrentChannel >= numMaps)
			{
				msh->SetMapNum(CurrentChannel+1);
				msh->InitMap(CurrentChannel);
			}

			int nv = msh->numv;

			tvFace = msh->M(CurrentChannel)->f;

			if (!tvFace)
			{
				msh->InitMap(CurrentChannel);
				tvFace = msh->M(CurrentChannel)->f;
			}

			msh->M(CurrentChannel)->setNumVerts( nv);
			tVerts = msh->M(CurrentChannel)->v;

			BOOL showAllWeights = FALSE;
			Point3 colorList[100];
			pblock_display->GetValue(skin_display_colorallweights,0,showAllWeights,FOREVER);
			int ct = pblock_display->Count(skin_display_weightcolors);
			for (int i = 0; i < ct; i++)
				pblock_display->GetValue(skin_display_weightcolors,0,colorList[i],FOREVER,i);

			int nf = msh->numf;
			Point3 selSoft = GetUIColor(COLOR_SUBSELECTION_SOFT);
			Point3 selMedium = GetUIColor(COLOR_SUBSELECTION_MEDIUM);
			Point3 selHard = GetUIColor(COLOR_SUBSELECTION_HARD);
			Point3 selSubColor = GetUIColor(COLOR_SUBSELECTION);

			Point3 noColor = ColorMan()->GetColorAsPoint3(NOWEIGHTCOLORID);

			//put in mirror check here
			int mirroredBone = -1;
			if (pPainterInterface)
			{
				if (pPainterInterface->GetMirrorEnable() && pPainterInterface->InPaintMode())
				{
					mirroredBone = mirrorIndex;
				}
			}

			int vct = bmd->VertexData.Count();
			Point3 *uv = tVerts;
			for (int i=0; i < vct; i++)
			{
				VertexListClass *vd = bmd->VertexData[i];
				Point3 col = noColor;
				if ( i < nv)
					*uv = col;
				if (showAllWeights && (ct > 0))
				{
					Point3 c;
					c.x = 0.0f;
					c.y = 0.0f;
					c.z = 0.0f;

					if (rigidVerts)
					{
						int rigidBoneID;
						rigidBoneID = vd->GetMostAffectedBone();
						c = colorList[rigidBoneID%ct];
					}
					else
					{
						int bct = vd->WeightCount();
						for (int boneid=0;boneid<bct;boneid++)
						{
							float infl = vd->GetNormalizedWeight(boneid);
							int boneID = vd->GetBoneIndex(boneid);
							if ( boneID == ModeBoneIndex)
							{
								Point3 color = colorList[boneID%ct];
								color.x *= 1.1f;
								color.y *= 1.1f;
								color.z *= 1.1f;
								c += color * infl;
							}
							else c += colorList[boneID%ct] * infl;
						}
					}
					*uv = c;
				}
				else
				{
					int bct = vd->WeightCount();
					for (int boneid=0; boneid < bct; boneid++)
					{
						int rigidBoneID = 0;
						if (rigidVerts)
							rigidBoneID = vd->GetMostAffectedBone();

						int bid = vd->GetBoneIndex(boneid);
						if ( ( (bid == ModeBoneIndex) || (bid == mirroredBone))
							&& (vd->GetWeight(boneid) != 0.0f)  )
						{
							if (!((rigidVerts) && (ModeBoneIndex != rigidBoneID)))
							{
								Point3 pt;
								float infl;

								infl = vd->GetNormalizedWeight(boneid);
								boneid = bct;
								Color selColor(1.0f,1.0f,1.0f);
								if (infl > 0.0f)
								{
									if ( (infl<0.33f) && (infl > 0.0f))
									{
										selColor = selSoft + ( (selMedium-selSoft) * (infl/0.33f));
									}
									else if (infl <.66f)
									{
										selColor = selMedium + ( (selHard-selMedium) * ((infl-0.1f)/0.66f));
									}
									else if (infl < 1.0f)
									{
										selColor = selHard + ( (selSubColor-selHard) * ((infl-0.66f)/0.33f));
									}

									else
									{
										selColor = selSubColor;
									}
								}

								if ( i < nv)
									*uv=selColor;
							}
						}
					}
				}
				uv++;
			}

			MNFace *gf = msh->f;
			MNMapFace *uvf = tvFace;

			for (int i=0; i < nf; i++)
			{
				if (gf->GetFlag (MN_DEAD)) continue;
				int ct =  gf->deg;
				if (uvf->deg != ct) uvf->SetSize (ct);

				for (int j = 0; j < ct; j++)
					uvf->tv[j] = gf->vtx[j];
				gf++;
				uvf++;
			}
		}
	}
}

class BonesDefModGeomPipelineEnumProc : public GeomPipelineEnumProc
{
public:
	BonesDefModGeomPipelineEnumProc(ModContext& mc) : mFound(false), mModContext(mc) {}
	PipeEnumResult proc(ReferenceTarget *object, IDerivedObject *derObj, int index);
	bool mFound;
	ModContext& mModContext;

protected:
	BonesDefModGeomPipelineEnumProc(); // disallowed
	BonesDefModGeomPipelineEnumProc(BonesDefModGeomPipelineEnumProc& rhs); // disallowed
	BonesDefModGeomPipelineEnumProc& operator=(const BonesDefModGeomPipelineEnumProc& rhs); // disallowed
};

PipeEnumResult BonesDefModGeomPipelineEnumProc::proc(
	ReferenceTarget *object,
	IDerivedObject *derObj,
	int index)
{
	ModContext* curModContext = NULL;
	if ((derObj != NULL) && (curModContext = derObj->GetModContext(index)) == &mModContext)
	{
		DbgAssert(object != NULL);
		mFound = true;
		return PIPE_ENUM_STOP;
	}
	return PIPE_ENUM_CONTINUE;
}

INode* BonesDefMod::GetNodeFromModContext(ModContext *mc, int &which)
{
	MyEnumProc dep;
	DoEnumDependents(&dep);
	for ( int i = 0; i < dep.Nodes.Count(); i++)
	{
		INode *node = dep.Nodes[i];
		BOOL found = FALSE;

		if (node)
		{
			Object* obj = node->GetObjectRef();

			BonesDefModGeomPipelineEnumProc pipeEnumProc(*mc);
			EnumGeomPipeline(&pipeEnumProc, obj);
			if (pipeEnumProc.mFound)
			{
				which = i;
				return node;
			}
		}
	}
	return NULL;
}

void BonesDefMod::CheckForXRefs(TimeValue t)
{
	INode *rootNode = GetCOREInterface()->GetRootNode();
	int xct = rootNode->GetXRefFileCount();

	if (xct > 0)
	{
		XRefEnumProc dep;
		DoEnumDependents(&dep);

		INode *XRefNode = dep.Nodes[0];

		for (int xid = 0; xid < xct; xid++)
		{
			INode *xroot = rootNode->GetXRefTree(xid);
			BOOL amIanXRef = FALSE;
			if (xroot)
				amIanXRef = RecurseXRefTree(xroot,XRefNode);
			if (amIanXRef)
			{
				INode *tempBindNode = rootNode->GetXRefParent(xid);
				if (tempBindNode == NULL)
				{
					bindNode = NULL;
				}
				else if ((tempBindNode) && (bindNode!=tempBindNode))
				{
					BOOL isThereAnInitialMatrix=FALSE;
					TSTR there(_T("InitialMatrix"));
					TSTR entry;
					//check if inode has initial matrix property if use that else
					if (bindNode!=NULL)
					{
						bindNode->GetUserPropBool(there,isThereAnInitialMatrix);
					}
					tempBindNode->GetUserPropBool(there,isThereAnInitialMatrix);
					if (isThereAnInitialMatrix)
					{
						Point3 r1,r2,r3,r4;
						entry.printf(_T("r1x"));
						tempBindNode->GetUserPropFloat(entry,r1.x);
						entry.printf(_T("r1y"));
						tempBindNode->GetUserPropFloat(entry,r1.y);
						entry.printf(_T("r1z"));
						tempBindNode->GetUserPropFloat(entry,r1.z);
						entry.printf(_T("r2x"));
						tempBindNode->GetUserPropFloat(entry,r2.x);
						entry.printf(_T("r2y"));
						tempBindNode->GetUserPropFloat(entry,r2.y);
						entry.printf(_T("r2z"));
						tempBindNode->GetUserPropFloat(entry,r2.z);
						entry.printf(_T("r3x"));
						tempBindNode->GetUserPropFloat(entry,r3.x);
						entry.printf(_T("r3y"));
						tempBindNode->GetUserPropFloat(entry,r3.y);
						entry.printf(_T("r3z"));
						tempBindNode->GetUserPropFloat(entry,r3.z);
						entry.printf(_T("r4x"));
						tempBindNode->GetUserPropFloat(entry,r4.x);
						entry.printf(_T("r4y"));
						tempBindNode->GetUserPropFloat(entry,r4.y);
						entry.printf(_T("rz"));
						tempBindNode->GetUserPropFloat(entry,r4.z);
						initialXRefTM.SetRow(0,r1);
						initialXRefTM.SetRow(1,r2);
						initialXRefTM.SetRow(2,r3);
						initialXRefTM.SetRow(3,r4);
					}
					else
					{
						initialXRefTM = tempBindNode->GetObjectTM(t);
						tempBindNode->SetUserPropBool(there,TRUE);
						Point3 r1,r2,r3,r4;
						r1 = initialXRefTM.GetRow(0);
						r2 = initialXRefTM.GetRow(1);
						r3 = initialXRefTM.GetRow(2);
						r4 = initialXRefTM.GetRow(3);
						entry.printf(_T("r1x"));
						tempBindNode->SetUserPropFloat(entry,r1.x);
						entry.printf(_T("r1y"));
						tempBindNode->SetUserPropFloat(entry,r1.y);
						entry.printf(_T("r1z"));
						tempBindNode->SetUserPropFloat(entry,r1.z);
						entry.printf(_T("r2x"));
						tempBindNode->SetUserPropFloat(entry,r2.x);
						entry.printf(_T("r2y"));
						tempBindNode->SetUserPropFloat(entry,r2.y);
						entry.printf(_T("r2z"));
						tempBindNode->SetUserPropFloat(entry,r2.z);
						entry.printf(_T("r3x"));
						tempBindNode->SetUserPropFloat(entry,r3.x);
						entry.printf(_T("r3y"));
						tempBindNode->SetUserPropFloat(entry,r3.y);
						entry.printf(_T("r3z"));
						tempBindNode->SetUserPropFloat(entry,r3.z);
						entry.printf(_T("r4x"));
						tempBindNode->SetUserPropFloat(entry,r4.x);
						entry.printf(_T("r4y"));
						tempBindNode->SetUserPropFloat(entry,r4.y);
						entry.printf(_T("rz"));
						tempBindNode->SetUserPropFloat(entry,r4.z);
					}
					bindNode = tempBindNode;
				}
			}
		}
	}
}

BOOL BonesDefMod::RebuildTMLocalData(TimeValue t, ModContext &mc, BoneModData *bmd)
{
	int which;
	INode *node = GetNodeFromModContext(&mc, which);

	if (node)
	{
		bmd->meshNode = node;
		UpdateTMCacheTable(bmd,t,FOREVER);
	}
	return TRUE;
}

BOOL BonesDefMod::InitLocalData(TimeValue t, ModContext &mc, ObjectState *os, INode *_node, Interval valid)
{
	int which;
	INode *node = GetNodeFromModContext(&mc, which);
	DbgAssert(_node == nullptr || node == _node);
	if (node)
	{
		BoneModData *d = new BoneModData(this);
		d->meshNode = node;
		d->BaseTM = node->GetObjectTM(RefFrame);
		d->BaseNodeTM = node->GetNodeTM(RefFrame);
		d->InverseBaseTM = Inverse(d->BaseTM);
		UpdateTMCacheTable(d,t,valid);

		if ((OldVertexDataCount != 0))
		{
			d->VertexData.SetCount(OldVertexDataCount);
			for (int j = 0; j < OldVertexDataCount; j++)
			{
				VertexListClass *vc = new VertexListClass;
				d->VertexData[j] = vc;

				d->selected.Set(j,OldVertexData[j]->selected);
				d->VertexData[j]->Modified (OldVertexData[j]->modified);
				d->VertexData[j]->LocalPos = OldVertexData[j]->LocalPos;
				d->VertexData[j]->PasteWeights(OldVertexData[j]->d);;
			}
			for   (int boneid = 0; boneid < OldVertexDataCount; boneid++)
				delete (OldVertexData[boneid]);

			OldVertexData.ZeroCount();
			OldVertexDataCount = -1;
		}

		mc.localData = d;
	}
	return FALSE;
}

void BonesDefMod::ModifyObject(
	TimeValue t, ModContext &mc, ObjectState *os, INode *node)
{
	if (stopEvaluation)
	{
		os->obj->UpdateValidity(GEOM_CHAN_NUM,Interval(t,t));
		return;
	}
	currentTime = t;

	TimeValue tps = GetTicksPerFrame();

	//grabs all our data from the param block
	Interval valid = FOREVER;

	pblock_param->GetValue(skin_filter_vertices,t,FilterVertices,valid);
	pblock_param->GetValue(skin_filter_bones,t,FilterBones,valid);
	pblock_param->GetValue(skin_filter_envelopes,t,FilterEnvelopes,valid);

	FilterVertices = !FilterVertices;
	FilterBones = !FilterBones;
	FilterEnvelopes = !FilterEnvelopes;

	pblock_display->GetValue(skin_display_all_vertices,t,drawAllVertices,valid);
	pblock_display->GetValue(skin_display_all_gizmos,t,displayAllGizmos,valid);
	pblock_display->GetValue(skin_display_draw_all_envelopes,t,DrawEnvelopes,valid);
	pblock_display->GetValue(skin_display_draw_vertices,t,DrawVertices,valid);

	pblock_advance->GetValue(skin_advance_ref_frame,t,RefFrame,valid);
	pblock_advance->GetValue(skin_advance_always_deform,t,AlwaysDeform,valid);

	pblock_advance->GetValue(skin_advance_rigid_verts,t,rigidVerts,valid);
	pblock_advance->GetValue(skin_advance_rigid_handles,t,rigidHandles,valid);
	pblock_advance->GetValue(skin_advance_fast_update,t,fastUpdate,valid);

	pblock_advance->GetValue(skin_advance_fast_update,t,fastUpdate,valid);

	BOOL noUpdate = FALSE;
	pblock_advance->GetValue(skin_advance_no_update,t,noUpdate,valid);

	pblock_advance->GetValue(skin_advance_updateonmouseup,t,updateOnMouseUp,valid);
	pblock_advance->GetValue(skin_advance_bonelimit,t,boneLimit,valid);
	pblock_advance->GetValue(skin_advance_backtransform,t,backTransform,valid);

	pblock_advance->GetValue(skin_advance_fastsubanims,t,enableFastSubAnimList,valid);

	BOOL useFastTMCache = TRUE;
	pblock_advance->GetValue(skin_advance_fasttmcache,t,useFastTMCache,valid);

	BOOL useFastVertexWeighting = TRUE;
	pblock_advance->GetValue(skin_advance_fastvertexweighting,t,useFastVertexWeighting,valid);

	pblock_advance->GetValue(skin_advance_fastgizmo,t,fastGizmo,valid);

	if (ModeBoneIndex > BoneData.Count() ) 
	{
		ModeBoneIndex = -1;
		int ct = NumberNonNullBones();
		if (ct != 0)
			ModeBoneIndex = ConvertBoneIDToBoneIndex(ct-1);
	}

	RefFrame = RefFrame*tps;

	//this updates the list box for when it gets out of sync because of an unod
	if (updateListBox)
	{
		if (ip)
		{
			RefillListBox();
			if (IsValidBoneIndex(ModeBoneIndex))
			{
				SelectBoneByBoneIndex(ModeBoneIndex);
			}
			UpdatePropInterface(FALSE);
		}
		updateListBox = FALSE;
	}

	CheckForXRefs(t);
	// If this flag is set, we have to recompute the InitNodeTM. That means, that we check the current difference between the node TM
	// and the ObjectOffset TM and remove the inverse of that from the InitObject TM. Thus we get the NodeTM at init time.
	// This only works, if the object offset TM didn't change since the bones were assigned !!! This is done, so we can make Skin
	// work with Node TM's and not with Object TM's as in the shipping version.
	//this will not get called for max 4.5 and beyond files since this data is now saved.
	if(recompInitTM)
	{
		for (int i = 0; i < BoneData.Count(); i++)
		{
			BoneDataClass &boneData = BoneData[i];
			INode *node = boneData.Node;
			Matrix3 diffTM(TRUE);
			Matrix3 stretchTM;
			if(node)
			{
				stretchTM = node->GetStretchTM(RefFrame);
				diffTM =  node->GetNodeTM(RefFrame) * Inverse(node->GetObjectTM(RefFrame));
			}

			if (hasStretchTM)
				boneData.InitNodeTM = diffTM * Inverse(boneData.tm);
			else boneData.InitNodeTM = stretchTM * diffTM * Inverse(boneData.tm);
			boneData.InitStretchTM = stretchTM;
		}
		recompInitTM = false;
	}

	//build each instance local data
	if (mc.localData == NULL)
	{
		if (InitLocalData(t, mc, os, node,valid)) return;
	}

	BoneModData *bmd = (BoneModData *) mc.localData;

	if (bmd == NULL) return;

	if (bmd->mod == NULL) bmd->mod = this;

	BOOL backfaceCull = TRUE;
	pblock_param->GetValue(skin_backfacecull,t,backfaceCull,valid);

	BOOL selectElement = TRUE;
	pblock_param->GetValue(skin_selectelement,t,selectElement,valid);

	if ( (editMod) && (ip && (ip->GetSubObjectLevel() == 1)))
	{
		if (os->obj->IsSubClassOf(triObjectClassID))
		{
			TriObject *tobj = (TriObject*)os->obj;
			Mesh &mesh = tobj->GetMesh();

			if ((bmd->mesh == NULL) || (mesh.numVerts != bmd->mesh->numVerts) || (mesh.numFaces != bmd->mesh->numFaces))
			{
				bmd->ClearMeshData();
				bmd->mesh = new Mesh(mesh);
			}
			else
			{
				for (int i = 0; i < mesh.numVerts; i++)
					bmd->mesh->verts[i] = mesh.verts[i];
			}
		}
		else if (os->obj->IsSubClassOf(polyObjectClassID))
		{
			PolyObject *polyObj = (PolyObject*)os->obj;
			MNMesh &mesh = polyObj->GetMesh();
			if ((bmd->mnMesh == NULL) || (mesh.numv != bmd->mnMesh->numv) || (mesh.numf != bmd->mnMesh->numf))
			{
				bmd->ClearMeshData();
				bmd->mnMesh = new MNMesh(polyObj->GetMesh());
			}
			else
			{
				for (int i = 0; i < mesh.numv; i++)
					bmd->mnMesh->v[i].p = mesh.v[i].p;
			}
		}
#ifndef NO_PATCHES
		else if (os->obj->IsSubClassOf(patchObjectClassID))
		{
			PatchObject *patOb = (PatchObject *)os->obj;
			bmd->ClearMeshData();
			bmd->patch = new PatchMesh(patOb->patch);
		}
#endif
	}
	else
	{
		bmd->ClearMeshData();
	}

	BOOL showHiddenVerts;
	pblock_display->GetValue(skin_display_showhiddenvertices,t,showHiddenVerts,FOREVER);

	//this looks through all our bones seeing if any of their cross sections or
	//end/start points are animated.  If they are they are thrown into the
	//invalidBones since they need to be recomputed
	Tab<int> invalidBones;

	BOOL animatedEnvelopes = FALSE;
	if (bmd->VertexData.Count() > 0)
	{
		for (int i = 0; i < BoneData.Count(); i++)
		{
			BoneDataClass &boneData = BoneData[i];
			if (boneData.Node)
			{
				Interval boneValid = FOREVER;
				Point3 dummyPoint;
				float dummyFloat;
				//we need to get all the end/start point intervals by just getting the value
				if (boneData.EndPoint1Control->IsAnimated())
					boneData.EndPoint1Control->GetValue(t,&dummyPoint,boneValid);
				if (boneData.EndPoint2Control->IsAnimated())
					boneData.EndPoint2Control->GetValue(t,&dummyPoint,boneValid);
				//we need to get all the crosssection intervals
				for (int crossID = 0; crossID < boneData.CrossSectionList.Count(); crossID++)
				{
					if (boneData.CrossSectionList[crossID].InnerControl->IsAnimated())
						boneData.CrossSectionList[crossID].InnerControl->GetValue(t,&dummyFloat,boneValid);
					if (boneData.CrossSectionList[crossID].OuterControl->IsAnimated())
						boneData.CrossSectionList[crossID].OuterControl->GetValue(t,&dummyFloat,boneValid);
				}
				//since these can be animated they must be part of our interval now also
				valid &= boneValid;
				//any that are not forever need to be tagged to be recomputed

				if (!(boneValid == FOREVER))  //hack we dont have a != method on interval and adding it would be an SDK break
				{
					bmd->reevaluate = TRUE;
					animatedEnvelopes = TRUE;
					invalidBones.Append(1,&i);
				}
			}
		}
	}

	if (bmd->needTMsRebuilt)
	{
		RebuildTMLocalData(t, mc,bmd);
		bmd->needTMsRebuilt = FALSE;
	}

	if (bmd->meshNode == NULL)
	{
		int which;
		bmd->meshNode =  GetNodeFromModContext(&mc, which);
	}

	if ((bmd->meshNode) && (bmd->meshNode->SuperClassID()!=BASENODE_CLASS_ID))  //hack a alert because of compound objects the mesh node may dissappearl
		bmd->meshNode = NULL;

	// If this flag is set, we have to recompute the InitMeshTM. That means, that we check the current difference between the node TM
	// and the ObjectOffset TM and remove the inverse of that from the InitObject TM. Thus we get the NodeTM at init time.
	// This only works, if the object offset TM didn't change since the bones were assigned !!! This is done, so we can make Skin
	// work with Node TM's and not with Object TM's as in the shipping version.

	if(bmd->recompInitMeshTM && ip)
	{
		ModContextList mcList;
		INodeTab nodes;

		ip->GetModContexts(mcList,nodes);

		for (int i = 0; i < mcList.Count(); i++)
		{
			if(bmd == mcList[i]->localData)
			{
				Matrix3 diffTM(TRUE);
				//5.1.03
				Matrix3 stretchTM;

				stretchTM = nodes[i]->GetStretchTM(RefFrame);
				diffTM = nodes[i]->GetNodeTM(RefFrame) * Inverse(nodes[i]->GetObjectTM(RefFrame));

				bmd->BaseNodeTM = diffTM * bmd->BaseTM;
			}
		}
		bmd->recompInitMeshTM = false;
	}

	bmd->inputObjectIsNURBS = os->obj->ClassID() == EDITABLE_SURF_CLASS_ID;

	resolvedModify = TRUE;

	if (BoneData.Count() == 0) return;

	BOOL forceRebuildCache = TRUE;
	if (useFastTMCache ) forceRebuildCache = FALSE;
	UpdateTMCacheTable(bmd,t,valid,forceRebuildCache);

	if ( (os->obj->NumPoints()==0) || (noUpdate))
	{
		os->obj->UpdateValidity(GEOM_CHAN_NUM,valid);
		return;
	}

#ifndef NO_PATCHES
	if (os->obj->IsSubClassOf(patchObjectClassID))
	{
		PatchObject *pobj = (PatchObject*)os->obj;

		int knots = pobj->patch.numVerts;
		bmd->autoInteriorVerts.SetSize(knots+pobj->patch.numVecs);
		{
			bmd->vecOwnerList.SetCount(knots+pobj->patch.numVecs);
			for (int vecID = 0; vecID < knots+pobj->patch.numVecs; vecID++)
				bmd->vecOwnerList[vecID] = -1;
		}

		bmd->autoInteriorVerts.ClearAll();
		for (int ipatch=0;ipatch<pobj->patch.numPatches;ipatch++)
		{
			int pc = 3;
			if (pobj->patch.patches[ipatch].type == PATCH_QUAD) pc = 4;
			if ((pobj->patch.patches[ipatch].flags &  PATCH_AUTO))
			{
				for (int ivec = 0; ivec < pc; ivec++)
				{
					//need to check if manual interio and mark if the  manuaul interio bug gets fixed
					bmd->autoInteriorVerts.Set(knots + pobj->patch.patches[ipatch].interior[ivec]);
				}
			}
		}
		{
			for (int ivec = 0; ivec < pobj->patch.numVecs; ivec++)
			{
				//need to check if manual interio and mark if the  manuaul interio bug gets fixed
				PatchVec pv = pobj->patch.vecs[ivec];
				bmd->vecOwnerList[knots + ivec] = pv.vert;
			}
		}
	}
#endif

	splinePresent = FALSE;
	for (int i =0;i<BoneData.Count();i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if ((boneData.Node != NULL) &&  (boneData.flags & BONE_SPLINE_FLAG) )
		{
			splinePresent = TRUE;
			break;
		}
	}

	if (!painting)
	{
		if (splineChanged)
		{
			if (IsValidBoneIndex(whichSplineChanged))
			{
				BoneDataClass& boneData = BoneData[whichSplineChanged];
				ObjectState os = boneData.Node->EvalWorldState(t);
				if (os.obj->SuperClassID()!=SHAPE_CLASS_ID)
				{
					//if not convert it to a regular node
					boneData.flags &= ~BONE_SPLINE_FLAG;
					bmd->reevaluate=TRUE;
					Point3 a,b;
					Interval v;
					float el1;
					Matrix3 ident(1);
					BuildMajorAxis(boneData.Node,a,b,el1,&ident);

					BOOL animate;
					pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);
					if (!animate)
					{
						SuspendAnimate();
						AnimateOff();
					}

					boneData.EndPoint1Control->SetValue(currentTime,a,TRUE,CTRL_ABSOLUTE);
					boneData.EndPoint2Control->SetValue(currentTime,b,TRUE,CTRL_ABSOLUTE);
					if (!animate)
						ResumeAnimate();

					cacheValid = FALSE;
				}
				else
				{
					ObjectState sos = boneData.Node->EvalWorldState(RefFrame);
					BezierShape bShape;
					ShapeObject *shape = (ShapeObject *)sos.obj;
					if (shape)
					{
						if(shape->CanMakeBezier())
						{
							shape->MakeBezier(RefFrame, bShape);
						}
						else {
							PolyShape pShape;
							shape->MakePolyShape(RefFrame, pShape);
							bShape = pShape;	// UGH -- Convert it from a PolyShape -- not good!
						}
					}
					if ((shape) && (bShape.splineCount >0) &&
						(bShape.splines[0]->Segments() != boneData.referenceSpline.Segments()))
					{
						boneData.referenceSpline = *bShape.splines[0];
						bmd->reevaluate=TRUE;

						for (int j = 0; j < bmd->VertexData.Count(); j++)
						{
							if (bmd->VertexData[j]->IsModified())
							{
								int numberBones = bmd->VertexData[j]->WeightCount();
								BOOL resetVert = FALSE;
								for (int k = 0; k < numberBones; k++)
								{
									int bid = bmd->VertexData[j]->GetBoneIndex(k);
									if (bid == whichSplineChanged)
									{
										resetVert = TRUE;
									}
								}
								if (resetVert) bmd->VertexData[j]->Modified(FALSE);
							}
						}
					}
				}
			}
			splineChanged = FALSE;
		}

		if (bmd->unlockVerts)
		{
			bmd->unlockVerts = FALSE;
			bmd->reevaluate=TRUE;
			for (int i=0;i<bmd->VertexData.Count();i++)
			{
				if (bmd->selected[i])
					bmd->VertexData[i]->Modified (FALSE);
			}
		}

		if (updateP)
		{
			updateP = FALSE;
			UpdateP(bmd);
		}

		if ((((t == (RefFrame)) && (BoneMoved)) && (!AlwaysDeform))  || bmd->forceRecomuteBaseNode)
		{
			//readjust TMs for frame 0
			BoneMoved = FALSE;
			if (AlwaysDeform)
				bmd->reevaluate=TRUE;
			cacheValid = FALSE;
			bmd->forceRecomuteBaseNode = FALSE;
			bmd->CurrentCachePiece = -1;

			int which;

			INode *node = GetNodeFromModContext(&mc, which);
			if (node)
			{
				bmd->meshNode = node;
				bmd->BaseTM = node->GetObjectTM(RefFrame);
				bmd->BaseNodeTM = node->GetNodeTM(RefFrame);
				bmd->InverseBaseTM = Inverse(bmd->BaseTM);
			}

			for (int boneid =0;boneid<BoneData.Count();boneid++)
			{
				BoneDataClass &boneData = BoneData[boneid];
				if (boneData.Node != NULL)
				{
					Class_ID bid(BONE_CLASS_ID,0);
					Matrix3 ntm =boneData.Node->GetObjectTM(RefFrame);

					boneData.InitObjectTM = ntm;
					boneData.tm = Inverse(ntm);
					//5.1.03
					boneData.InitStretchTM = boneData.Node->GetStretchTM(RefFrame);
					if (hasStretchTM)
						boneData.InitNodeTM = boneData.Node->GetNodeTM(RefFrame);
					else boneData.InitNodeTM = boneData.InitStretchTM * boneData.Node->GetNodeTM(RefFrame);
					//copy initial reference spline into our spline

					ObjectState sos = boneData.Node->EvalWorldState(RefFrame);
					if (boneData.flags & BONE_SPLINE_FLAG)
					{
						BezierShape bShape;
						ShapeObject *shape = (ShapeObject *)sos.obj;
						if (shape)
						{
							if(shape->CanMakeBezier())
								shape->MakeBezier(t, bShape);
							else {
								PolyShape pShape;
								shape->MakePolyShape(t, pShape);
								bShape = pShape;	// UGH -- Convert it from a PolyShape -- not good!
							}
							if (bShape.splineCount >0)
								boneData.referenceSpline = *bShape.splines[0];
						}
					}
					//need to readjust the child endpoints for bone type objects also
					else if (sos.obj->ClassID() == bid)
					{
						//now need to look at child and move
						//loop through children looking for a matching name
						int childCount;
						childCount = boneData.Node->NumberOfChildren();
						INode *childNode = NULL;
						BOOL animate;
						pblock_advance->GetValue(skin_advance_animatable_envelopes,0,animate,FOREVER);
						if (!animate)
						{
							SuspendAnimate();
							AnimateOff();
						}
						for (int ci = 0; ci < childCount; ci++)
						{
							childNode = boneData.Node->GetChildNode(ci);
							TSTR childName;
							childName = childNode->GetName();
							if (childName == boneData.name)
							{
								Point3 l2(0.0f,0.0f,0.0f);
								Matrix3 ChildTM = childNode->GetObjectTM(RefFrame);
								l2 = l2 * ChildTM;
								l2 = l2 * boneData.tm;
								if (boneid < endPointDelta.Count())
								{
									l2 = l2+endPointDelta[boneid];
									boneData.EndPoint2Control->SetValue(currentTime,&l2,TRUE,CTRL_ABSOLUTE);
								}
								else boneData.EndPoint2Control->SetValue(currentTime,&l2,TRUE,CTRL_ABSOLUTE);
								ci = childCount;
							}
						}
						if (!animate)
							ResumeAnimate();
					}
				}
			}
			UpdateTMCacheTable(bmd,t,valid);
		}
		if (reloadSplines)
		{
			for (int boneid =0;boneid<BoneData.Count();boneid++)
			{
				BoneDataClass &boneData = BoneData[boneid];
				if (boneData.Node != NULL)
				{
					//copy initial reference spline into our spline
					if (boneData.flags & BONE_SPLINE_FLAG)
					{
						ObjectState sos = boneData.Node->EvalWorldState(RefFrame);

						if (sos.obj->SuperClassID()==SHAPE_CLASS_ID)
						{
							BezierShape bShape;
							ShapeObject *shape = (ShapeObject *)sos.obj;
							if (shape)
							{
								if(shape->CanMakeBezier())
									shape->MakeBezier(t, bShape);
								else {
									PolyShape pShape;
									shape->MakePolyShape(t, pShape);
									bShape = pShape;	// UGH -- Convert it from a PolyShape -- not good!
								}
								if (bShape.splineCount >0)
									boneData.referenceSpline = *bShape.splines[0];
							}
						}
						else boneData.flags &= ~BONE_SPLINE_FLAG;
					}
				}
			}
			reloadSplines = FALSE;
		}

		if (bmd->selected.GetSize() != os->obj->NumPoints())
			bmd->selected.SetSize(os->obj->NumPoints(),TRUE);

		//set validty based on TM's
		for (int i =0;i<BoneData.Count();i++)
		{
			BoneDataClass &boneData = BoneData[i];
			if (boneData.Node != NULL)
			{
				boneData.Node->GetObjectTM(t,&valid);
				if (boneData.flags & BONE_SPLINE_FLAG)
				{
					ObjectState osp = boneData.Node->EvalWorldState (t);
					valid &= osp.obj->ObjectValidity (t);
				}
			}
		}

		//get selected bone
		if (bmd->VertexData.Count() != os->obj->NumPoints())
		{
			//readjust vertices using nearest vertices as sample
			bmd->reevaluate = TRUE;
			bmd->CurrentCachePiece = -1;
			if (bmd->VertexData.Count() != 0)
			{
				Box3 bbox;
				os->obj->GetDeformBBox(t, bbox);
				float threshold = Length(bbox.pmin-bbox.pmax)/10.0f;
				RemapExclusionData(bmd, threshold, -1,os->obj);
				RemapLocalGimzoData(bmd, threshold, -1,os->obj);
				RemapVertData(bmd, threshold, -1,os->obj);
				bmd->CleanUpExclusionLists();
			}
		}

		if (!showHiddenVerts && (editMod) && (ip && (ip->GetSubObjectLevel() == 1)))
		{
			SetHiddenVerts(t, bmd, os->obj);
		}

		if (unlockBone)
		{
			unlockBone = FALSE;
			bmd->reevaluate = TRUE;
			UnlockBone(bmd,t,os);
		}

		if (unlockAllBones)
		{
			unlockAllBones = FALSE;
			bmd->reevaluate = TRUE;
			UnlockAllBones(bmd,t,os);
		}

		//Need to recompute any bone that was not valid, this will completely fubar our cache but not much we can do about it
		//Will need to rethink how to cache this
		BOOL repaintWeightTable = FALSE;
		for (int i = 0; i < invalidBones.Count(); i++)
		{
			if (!((ip) || (bmd->reevaluate)))
				RecomputeBone(bmd,invalidBones[i],t,os);
		}

		if ((ip) || (bmd->reevaluate))
		{
			if ( (bmd->reevaluate) )
			{
				RecomputeAllBones(bmd,t,os);
				cacheValid = FALSE;
				bmd->CurrentCachePiece = -1;
				//WEIGHTTABLE
				repaintWeightTable = TRUE;
				//since we rebuild all the bones we need to renormalize all our weights agsin
				bmd->rebuildWeights = TRUE;
			}

			else if ( (bmd->reevaluateActiveBone) && (ModeBoneIndex!=-1) && (ip && ip->GetSubObjectLevel() == 1))//&& (ModeEdit ==1) )
			{
				RecomputeBone(bmd,ModeBoneIndex,t,os);
				//WEIGHTTABLE
				repaintWeightTable = TRUE;
				//since we rebuild all the bones we need to renormalize all our weights agsin
				bmd->rebuildWeights = TRUE;
			}
		}

		if (repaintWeightTable) PaintAttribList(FALSE);

		if (bmd->mCacheDumped )
		{
			VertexListClass **vd;
			Object *obj = os->obj;
			vd = bmd->VertexData.Addr(0);
			for (int i = 0; i < os->obj->NumPoints(); i++)
			{
				(*vd)->LocalPos = obj->GetPoint(i);
				vd++;
			}
			bmd->mCacheDumped = false;
		}
	}
	valid &= GetValidity(t);

#ifndef NO_PATCHES
	if ((os->obj->IsSubClassOf(patchObjectClassID)) && (rigidHandles))
	{
		//loop through vecs looking for owners
		for (int i = 0; i < bmd->vecOwnerList.Count(); i++)
		{
			if (bmd->vecOwnerList[i] > 0)
			{
				int owner = bmd->vecOwnerList[i];
				bmd->VertexData[i]->PasteWeights( bmd->VertexData[owner]->CopyWeights());
			}
		}
	}
#endif

	if (os->obj->IsSubClassOf(triObjectClassID))
	{
		bmd->isMesh = TRUE;
		bmd->isPatch = FALSE;
	}
#ifndef NO_PATCHES
	else if (os->obj->IsSubClassOf(patchObjectClassID))
	{
		bmd->isMesh = FALSE;
		bmd->isPatch = TRUE;
	}
#endif
	else if (os->obj->IsParamSurface())
	{
		bmd->isMesh = FALSE;
		bmd->isPatch = FALSE;
	}
	else
	{
		//ask if can convert to mesh
		if (os->obj->CanConvertToType(triObjectClassID))
		{
			bmd->isMesh = TRUE;
			bmd->isPatch = FALSE;
		}
		else
		{
			bmd->isMesh = FALSE;
			bmd->isPatch = FALSE;
		}
	}

	//new fix 2
	gizmoDefList.SetCount(pblock_gizmos->Count(skin_gizmos_list));
	gizmoDefListCount = pblock_gizmos->Count(skin_gizmos_list);
	for(int  i = 0 ; i < bmd->gizmoData.Count() ; i++)
	{
		int id = bmd->gizmoData[i]->whichGizmo;
		int gizmoCount = pblock_gizmos->Count(skin_gizmos_list);
		ReferenceTarget *ref;
		if (id < gizmoCount)
		{
			ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,id);
			if (ref)
			{
				GizmoClass *gizmo = (GizmoClass *)ref;
				if (gizmo->IsVolumeBased())
					bmd->gizmoData[i]->FreeDeformingList();
				else bmd->gizmoData[i]->BuildDeformingList();
			}
		}
	}

	for (int i = 0; i < pblock_gizmos->Count(skin_gizmos_list); i++)
	{
		gizmoDefList[i] = NULL;
		ReferenceTarget *ref;
		ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,i);

		//lets the gizmo know about the double transform
		IGizmoClass2 *giz2 = (IGizmoClass2 *) ref->GetInterface(I_GIZMO2);
		if (giz2)
			giz2->SetBackTransFormMatrices(bmd->gizmoRemoveDoubleOffset,bmd->gizmoPutBackDoubleOffset);

		GizmoClass *gizmo = (GizmoClass *)ref;
		gizmoDefList[i] = gizmo;
		if (gizmo) gizmo->PreDeformSetup( t);
	}

	for (int i = 0; i < splineList.Count(); i++)
	{
		if (splineList[i])
		{
			delete splineList[i];
			splineList[i] = NULL;
		}
	}
	splineList.SetCount(BoneData.Count());
	for (int i = 0; i < BoneData.Count(); i++)
	{
		splineList[i] = NULL;
		BoneDataClass &boneData = BoneData[i];
		if ((boneData.Node) && (boneData.flags & BONE_SPLINE_FLAG))
		{
			ObjectState sos = boneData.Node->EvalWorldState(t);
			BezierShape bShape;
			ShapeObject *shape = (ShapeObject *)sos.obj;
			if (shape->NumberOfCurves() > 0)
			{
				int sct = shape->NumberOfPieces(t,0);
				int rct = boneData.referenceSpline.Segments();

				if ((shape) && (sct != rct))
				{
					splineList[i] = new Spline3D();

					if(shape->CanMakeBezier())
						shape->MakeBezier(t, bShape);
					else {
						PolyShape pShape;
						shape->MakePolyShape(t, pShape);
						bShape = pShape;	// UGH -- Convert it from a PolyShape -- not good!
					}

					*splineList[i] = *bShape.splines[0];
					splineList[i]->InvalidateGeomCache();
					splineList[i]->ComputeBezPoints();
				}
			}
		}
	}

	// Here comes the COM engine setup
	if (bmd->pSE)
	{
		//5.1.03
		BOOL ignoreBoneScale;
		pblock_advance->GetValue(skin_advance_ignorebonescale,0,ignoreBoneScale,FOREVER);

		bmd->pSE->SetNumPoints(os->obj->NumPoints());
		bmd->pSE->SetInitTM((float *)&bmd->BaseTM);
		bmd->pSE->SetNumBones(BoneData.Count());
		for( int i = 0 ; i < BoneData.Count() ; i++)
		{
			BoneDataClass &boneData = BoneData[i];
			if (boneData.Node != NULL)
			{
				Matrix3 ntm;

				// For some cases (Some CAT objects & Splines) we want to use the object offset TM
				// For CAT, this is because the object offset is used to propagate some transform
				// back up the node hierarchy (twisty bones, mainly).
				if(boneData.flags & BONE_SPLINE_FLAG)
				{
					ntm = boneData.Node->GetObjectTM(t);
					if (bindNode)
					{
						Matrix3 xRefTM = bindNode->GetObjectTM(t);
						ntm =  ntm *  Inverse(xRefTM) * initialXRefTM ;
					}
					else if ((backTransform) && (bmd->meshNode))
					{
						Matrix3 backTM = bmd->meshNode->GetNodeTM(t);
						ntm =  ntm *  Inverse(backTM) * bmd->BaseNodeTM ;
					}

					bmd->pSE->SetBoneTM(i, (float *) &ntm);
					bmd->pSE->SetInitBoneTM(i, (float *) &boneData.InitObjectTM);
				}
				else
				{
					if (!hasStretchTM)
						ntm = GetStretchTM(t, boneData.Node, i) * boneData.Node->GetNodeTM(t);
					else if (ignoreBoneScale)
						ntm =  boneData.Node->GetNodeTM(t);
					else ntm = GetStretchTM(t, boneData.Node, i) * boneData.Node->GetNodeTM(t);

					if (bindNode)
					{
						Matrix3 xRefTM = bindNode->GetObjectTM(t);
						ntm =   ntm *  Inverse(xRefTM) * initialXRefTM ;
					}
					else if ((backTransform) && (bmd->meshNode))
					{
						Matrix3 backTM = bmd->meshNode->GetNodeTM(t);
						ntm =  ntm *  Inverse(backTM) * bmd->BaseNodeTM ;
					}

					bmd->pSE->SetBoneTM(i, (float *) &ntm);
					if (!hasStretchTM)
						bmd->pSE->SetInitBoneTM(i, (float *) &(boneData.InitNodeTM));
					else if (ignoreBoneScale)
						bmd->pSE->SetInitBoneTM(i, (float *) &(boneData.InitNodeTM));
					else bmd->pSE->SetInitBoneTM(i, (float *) &(boneData.InitStretchTM*boneData.InitNodeTM));
				}
			}
			bmd->pSE->SetBoneFlags(i,boneData.flags);
		}
	}

	bmd->CurrentTime = t;

	if ((t == RefFrame) && (!AlwaysDeform))
	{
	}
	else
	{
		//Optimize this we only need to build this is they are editing or there is an animated bone
		if ((useFastVertexWeighting == FALSE) || animatedEnvelopes || bmd->rebuildWeights)
		{
			BuildNormalizedWeight(bmd);
			bmd->rebuildWeights = FALSE;
		}

		BonesDefDeformer deformer(this,bmd,t);
		os->obj->Deform(&deformer, TRUE);
	}

	//new fix 2
	for(int i = 0 ; i < bmd->gizmoData.Count() ; i++)
	{
		int id = bmd->gizmoData[i]->whichGizmo;
		int gizmoCount = pblock_gizmos->Count(skin_gizmos_list);
		ReferenceTarget *ref;
		if (id < gizmoCount)
		{
			ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,id);
			if (ref)
			{
				GizmoClass *gizmo = (GizmoClass *)ref;
				bmd->gizmoData[i]->FreeDeformingList();
			}
		}
	}

	for (int i = 0; i < pblock_gizmos->Count(skin_gizmos_list); i++)
	{
		ReferenceTarget *ref;
		ref = pblock_gizmos->GetReferenceTarget(skin_gizmos_list,0,i);
		GizmoClass *gizmo = (GizmoClass *)ref;
		if (gizmo) gizmo->PostDeformSetup( t);
	}

	pblock_display->GetValue(skin_display_shadeweights,0,shadeVC,FOREVER);

	if (ip)
	{
		if ( (os->obj->IsSubClassOf(triObjectClassID)) || (os->obj->IsSubClassOf(polyObjectClassID)) )
		{
			EnableWindow(GetDlgItem(hParam,IDC_FILTER_VERTICES_CHECK2),TRUE);
		}
		else
		{
			EnableWindow(GetDlgItem(hParam,IDC_FILTER_VERTICES_CHECK2),FALSE);
		}
	}

	if ((ip) &&  (shadeVC) && (ip->GetSubObjectLevel() == 1) )
	{
		if (os->obj->IsSubClassOf(triObjectClassID))
		{
			SetVCMode();

			TriObject *tobj = (TriObject*)os->obj;
			Mesh *msh = &tobj->GetMesh();
			ShadeVerts(msh,bmd);
			os->obj->UpdateValidity(VERT_COLOR_CHAN_NUM, FOREVER);
		}
#ifndef NO_PATCHES
		else if (os->obj->IsSubClassOf(patchObjectClassID))
		{
			SetVCMode();

			PatchObject *pobj = (PatchObject*)os->obj;
			PatchMesh &msh = pobj->patch;
			ShadeVerts(&msh,bmd);
			os->obj->UpdateValidity(VERT_COLOR_CHAN_NUM, FOREVER);
		}
#endif
		else if (os->obj->IsSubClassOf(polyObjectClassID))
		{
			SetVCMode();

			PolyObject *tobj = (PolyObject*)os->obj;
			// Apply our mapping
			MNMesh &mesh = tobj->GetMesh();
			ShadeVerts(&mesh,bmd);
			os->obj->UpdateValidity(VERT_COLOR_CHAN_NUM, FOREVER);
		}
		else
		{
			shadeVC = FALSE;
		}
	}
	os->obj->UpdateValidity(GEOM_CHAN_NUM,valid);
}

float BonesDefMod::GetSquash(TimeValue t, INode *node)
{
	Matrix3 tm = node->GetStretchTM(t);
	int dir = node->GetBoneAxis();
	Point3 a,b,c;
	a = tm.GetRow(0);
	b = tm.GetRow(1);
	c = tm.GetRow(2);
	if (dir == 0) return b[1];
	else if (dir == 1) return c[2];
	else return a[0];
}

Matrix3 BonesDefMod::GetStretchTM(TimeValue t, INode *node, int index)
{
	if ( (index>=pblock_param->Count(skin_initial_squash))  || (index>=pblock_param->Count(skin_local_squash)) )
	{
		for (int i =0; i < index+1; i++)
		{
			INode *n = BoneData[i].Node;
			if (n)
			{
				float f = 1.0f;
				pblock_param->Append(skin_local_squash,1,&f);
				f = GetSquash(RefFrame, n);
				pblock_param->Append(skin_initial_squash,1,&f);
			}
			else
			{
				float f = 1.0f;
				pblock_param->Append(skin_local_squash,1,&f);
				pblock_param->Append(skin_initial_squash,1,&f);
			}
		}
	}
	Matrix3 tm = node->GetStretchTM(t);
	if ((index >=0) &&  (index<pblock_param->Count(skin_local_squash)) && (index<pblock_param->Count(skin_initial_squash)))
	{
		float squash = 0.;
		float initialSquash = 0.;
		pblock_param->GetValue(skin_local_squash,t,squash,FOREVER,index);
		pblock_param->GetValue(skin_initial_squash,t,initialSquash,FOREVER,index);
		float currentSquash = GetSquash(t, node) ;
		squash = 1.0f + (1.0f - squash);
		squash= initialSquash - ((initialSquash- currentSquash )*squash);

		Point3 scale(squash,squash,squash);
		int dir = node->GetBoneAxis();
		Point3 a;
		a = tm.GetRow(dir);
		scale[dir] = a[dir];
		a = tm.GetRow(0);
		a[0] = scale[0];
		tm.SetRow(0,a);

		a = tm.GetRow(1);
		a[1] = scale[1];
		tm.SetRow(1,a);

		a = tm.GetRow(2);
		a[2] = scale[2];
		tm.SetRow(2,a);
	}
	return tm;
}

#define ID_CHUNK 0x1000

IOResult BonesDefMod::SaveLocalData(ISave *isave, LocalModData *pld)
{
	ULONG		nb;

	BoneModData *bmd = (BoneModData*)pld;

	isave->BeginChunk(BASE_TM_CHUNK);
	bmd->BaseTM.Save(isave);
	isave->EndChunk();

	int c = bmd->VertexData.Count();
	//save vertex influence info
	isave->BeginChunk(VERTEX_COUNT_CHUNK);
	isave->Write(&c,sizeof(c),&nb);
	isave->EndChunk();

	isave->BeginChunk(VERTEXV5_DATA_CHUNK);
	for (int i = 0; i < c; i++)
	{
		//write number of influences
		int ic;
		ic = bmd->VertexData[i]->WeightCount();
		isave->Write(&ic,sizeof(ic),&nb);
		int save_i;
		float save_f;

		DWORD save_dword;
		save_dword = bmd->VertexData[i]->flags;
		isave->Write(&save_dword,sizeof(save_dword),&nb);

		for (int j = 0; j < ic; j++)
		{
			save_i = bmd->VertexData[i]->GetBoneIndex(j);
			save_f = bmd->VertexData[i]->GetWeight(j);
			isave->Write(&save_i,sizeof(save_i),&nb);
			isave->Write(&save_f,sizeof(save_f),&nb);

			save_i = bmd->VertexData[i]->GetCurveID(j);
			isave->Write(&save_i,sizeof(save_i),&nb);
			save_i = bmd->VertexData[i]->GetSegID(j);
			isave->Write(&save_i,sizeof(save_i),&nb);

			save_f = bmd->VertexData[i]->GetCurveU(j);
			isave->Write(&save_f,sizeof(save_f),&nb);

			Point3 save_p;
			save_p = bmd->VertexData[i]->GetTangent(j);
			isave->Write(&save_p,sizeof(save_p),&nb);

			save_p = bmd->VertexData[i]->GetOPoint(j);
			isave->Write(&save_p,sizeof(save_p),&nb);
		}
	}
	isave->EndChunk();

	isave->BeginChunk(CLOSESTBONE_CHUNK);
	for (int i = 0; i <  bmd->VertexData.Count(); i++)
	{
		int save_i = bmd->VertexData[i]->GetClosestBone();
		isave->Write(&save_i,sizeof(save_i),&nb);
	}
	isave->EndChunk();

	isave->BeginChunk(EXCLUSION_CHUNK);
	//count then data
	c = bmd->exclusionList.Count();
	isave->Write(&c,sizeof(c),&nb);
	//active count
	int exct = 0;
	for (int i = 0; i < c; i++)
	{
		if (bmd->exclusionList[i])
			exct++;
	}
	isave->Write(&exct,sizeof(exct),&nb);

	//tab of which ones
	for (int i=0; i < c; i ++)
	{
		//then data
		if (bmd->exclusionList[i])
		{
			isave->Write(&i,sizeof(i),&nb);
			bmd->exclusionList[i]->Save(isave);
		}
	}
	isave->EndChunk();

	isave->BeginChunk(GIZMOCOUNT_CHUNK);
	//count then data
	c = bmd->gizmoData.Count();
	isave->Write(&c,sizeof(c),&nb);
	for ( int i =0; i < c; i ++)
		isave->Write(&bmd->gizmoData[i]->whichGizmo,sizeof(bmd->gizmoData[i]->whichGizmo),&nb);

	isave->EndChunk();

	isave->BeginChunk(GIZMODATA_CHUNK);
	//count then data
	c = bmd->gizmoData.Count();
	for (int  i=0; i < c; i ++)
	{
		bmd->gizmoData[i]->Save(isave);
	}
	isave->EndChunk();

	isave->BeginChunk(NAMEDSEL_BITSCOUNT_CHUNK);
	//count then data
	c = bmd->namedSelSets.Count();
	isave->Write(&c,sizeof(c),&nb);
	isave->EndChunk();

	for (int i=0; i < c; i++)
	{
		if (bmd->namedSelSets[i])
		{
			isave->BeginChunk(NAMEDSEL_BITSID_CHUNK);
			int id  =i;
			isave->Write(&i,sizeof(i),&nb);
			isave->EndChunk();

			isave->BeginChunk(NAMEDSEL_BITS_CHUNK);
			bmd->namedSelSets[i]->Save(isave);
			isave->EndChunk();
		}
	}

	isave->BeginChunk(BASENODE_TM_CHUNK);
	bmd->BaseNodeTM.Save(isave);
	isave->EndChunk();

	//5.1.03
	if (bmd->meshNode)
	{
		ULONG id = isave->GetRefID(bmd->meshNode);

		isave->BeginChunk(MESHNODEBACKPATCH_CHUNK);
		isave->Write(&id,sizeof(ULONG),&nb);
		isave->EndChunk();
	}

	return IO_OK;
}

IOResult BonesDefMod::LoadLocalData(ILoad *iload, LocalModData **pld)
{
	IOResult	res;
	ULONG		nb;

	BoneModData *bmd = new BoneModData(this);
	*pld = bmd;
	bmd->effect = -1.0f;
	bmd->reevaluate = TRUE;

	int exsize = 0, exsizeActive = 0, exID = 0;
	int k;
	int lastID = 0;

	bmd->recompInitMeshTM = true;

	while (IO_OK==(res=iload->OpenChunk()))
	{
		switch(iload->CurChunkID())  {
		case NAMEDSEL_BITSCOUNT_CHUNK:
			{
				int ct = 0;
				iload->Read(&ct,sizeof(ct), &nb);
				bmd->namedSelSets.SetCount(ct);
				for (int i =0; i < ct; i++)
					bmd->namedSelSets[i] = NULL;
				break;
			}
		case NAMEDSEL_BITSID_CHUNK:
			{
				iload->Read(&lastID,sizeof(lastID), &nb);
				break;
			}

		case NAMEDSEL_BITS_CHUNK:
			{
				BitArray *bits = new BitArray;
				bits->Load(iload);
				bmd->namedSelSets[lastID] = bits;
				break;
			}

		case GIZMOCOUNT_CHUNK:
			{
				int c = 0;
				iload->Read(&c,sizeof(int), &nb);
				bmd->gizmoData.SetCount(c);
				for (k = 0; k < c; k++)
				{
					bmd->gizmoData[k] = new LocalGizmoData();
					iload->Read(&bmd->gizmoData[k]->whichGizmo,sizeof(bmd->gizmoData[k]->whichGizmo), &nb);
				}

				break;
			}
		case GIZMODATA_CHUNK:
			{
				for (k = 0; k < bmd->gizmoData.Count(); k++)
				{
					bmd->gizmoData[k]->Load(iload);
				}

				break;
			}

		case EXCLUSION_CHUNK:
			//read size
			iload->Read(&exsize,sizeof(int), &nb);
			iload->Read(&exsizeActive,sizeof(int), &nb);
			bmd->exclusionList.SetCount(exsize);
			for (k = 0; k < exsize; k++)
				bmd->exclusionList[k] = NULL;
			for (k = 0; k < exsizeActive; k++)
			{
				iload->Read(&exID,sizeof(int), &nb);

				bmd->exclusionList[exID] = new ExclusionListClass;
				bmd->exclusionList[exID]->Load(iload);
			}

			break;

		case BASE_TM_CHUNK:
			{
				bmd->BaseTM.Load(iload);
				bmd->InverseBaseTM = Inverse(bmd->BaseTM);

				// Make sure, the InitMesh Node TM will be recomputed (see ModifyObject) ns
				bmd->recompInitMeshTM = true;
				break;
			}

		case VERTEX_COUNT_CHUNK:
			{
				int c = 0;
				iload->Read(&c,sizeof(c),&nb);
				bmd->VertexData.ZeroCount();
				bmd->VertexData.SetCount(c);
				for (int i=0; i<c; i++) {
					VertexListClass *vc = new VertexListClass;
					bmd->VertexData[i] = vc;
					bmd->VertexData[i]->Modified (FALSE);
					bmd->VertexData[i]->flags = 0;
					bmd->VertexData[i]->ZeroWeights();
				}

				break;
			}
			//version 4.2 and less
		case VERTEX_DATA_CHUNK:
			{
				for (int i=0; i < bmd->VertexData.Count(); i++)
				{
					int c = 0;
					BOOL load_b = 0;
					iload->Read(&c,sizeof(c),&nb);
					bmd->VertexData[i]->SetWeightCount(c);

					iload->Read(&load_b,sizeof(load_b),&nb);
					bmd->VertexData[i]->Modified (load_b);
					float load_f = 0.;
					int load_i = 0;
					Point3 load_p;
					float sum = 0.0f;
					for (int boneid=0; boneid<c; boneid++) {
						iload->Read(&load_i,sizeof(load_i),&nb);
						iload->Read(&load_f,sizeof(load_f),&nb);
						bmd->VertexData[i]->SetBoneIndex(boneid, load_i);
						bmd->VertexData[i]->SetWeight(boneid,load_f);
						sum += load_f;

						iload->Read(&load_i,sizeof(load_i),&nb);
						bmd->VertexData[i]->SetCurveID(boneid, load_i);
						iload->Read(&load_i,sizeof(load_i),&nb);
						bmd->VertexData[i]->SetSegID(boneid,load_i);

						iload->Read(&load_f,sizeof(load_f),&nb);
						bmd->VertexData[i]->SetCurveU(boneid, load_f);

						iload->Read(&load_p,sizeof(load_p),&nb);
						bmd->VertexData[i]->SetTangent(boneid, load_p);

						iload->Read(&load_p,sizeof(load_p),&nb);
						bmd->VertexData[i]->SetOPoint(boneid, load_p);
					}
					if (bmd->VertexData[i]->IsModified() && (fabs(sum-1.0f) > 0.01f))
					{
						if (c==1)
						{
							int boneIndex = bmd->VertexData[i]->GetBoneIndex(0);
							if ((IsValidBoneIndex(boneIndex)) && (!(BoneData[boneIndex].flags&BONE_ABSOLUTE_FLAG)))
								bmd->VertexData[i]->UnNormalized(TRUE);
							else 	bmd->VertexData[i]->SetWeight(0, 1.0f);
						}
						else bmd->VertexData[i]->UnNormalized(FALSE);
					}
					else bmd->VertexData[i]->NormalizeWeights();
				}

				break;
			}
			//version 4.5 and greater
		case VERTEXV5_DATA_CHUNK:
			{
				for (int i=0; i < bmd->VertexData.Count(); i++)
				{
					int c = 0;
					DWORD load_dword = 0;
					iload->Read(&c,sizeof(c),&nb);
					bmd->VertexData[i]->SetWeightCount(c);

					iload->Read(&load_dword,sizeof(load_dword),&nb);
					bmd->VertexData[i]->flags = load_dword;

					float load_f;
					int load_i;
					Point3 load_p;
					for (int boneid=0; boneid<c; boneid++) {
						iload->Read(&load_i,sizeof(load_i),&nb);
						iload->Read(&load_f,sizeof(load_f),&nb);
						bmd->VertexData[i]->SetBoneIndex(boneid, load_i);
						bmd->VertexData[i]->SetWeight(boneid, load_f);
						bmd->VertexData[i]->SetNormalizedWeight(boneid,load_f) ;

						iload->Read(&load_i,sizeof(load_i),&nb);
						bmd->VertexData[i]->SetCurveID(boneid, load_i);
						iload->Read(&load_i,sizeof(load_i),&nb);
						bmd->VertexData[i]->SetSegID(boneid, load_i);

						iload->Read(&load_f,sizeof(load_f),&nb);
						bmd->VertexData[i]->SetCurveU(boneid, load_f);

						iload->Read(&load_p,sizeof(load_p),&nb);
						bmd->VertexData[i]->SetTangent(boneid, load_p);

						iload->Read(&load_p,sizeof(load_p),&nb);
						bmd->VertexData[i]->SetOPoint(boneid, load_p);
					}
				}

				break;
			}
		case CLOSESTBONE_CHUNK:
			for (int i=0; i < bmd->VertexData.Count(); i++)
			{
				int load_i = 0;
				iload->Read(&load_i,sizeof(load_i),&nb);
				bmd->VertexData[i]->SetClosestBone(load_i);
			}
			break;

		case BASENODE_TM_CHUNK:
			{
				bmd->BaseNodeTM.Load(iload);
				bmd->recompInitMeshTM = false;
				break;
			}
		}
		iload->CloseChunk();
		if (res!=IO_OK) return res;
	}

	int c = bmd->VertexData.Count();
	//add m crossection
	bmd->selected.SetSize(c);
	bmd->selected.ClearAll();
	bmd->CurrentCachePiece = -1;

	return IO_OK;
}

void BonesDefMod::SaveEnvelopeDialog(BOOL defaultToBinary)
{
	static TCHAR fname[256] = {'\0'};
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	FilterList fl;
	fl.Append( GetString(IDS_PW_ENVFILES));
	fl.Append( _T("*.env"));
	fl.Append( GetString(IDS_PW_ENVTEXTFILES));
	fl.Append( _T("*.envASCII"));
	TSTR title = GetString(IDS_PW_SAVEENVELOPES);

	ofn.lStructSize     = sizeof(OPENFILENAME);
	ofn.hwndOwner       = GetCOREInterface()->GetMAXHWnd();
	ofn.lpstrFilter     = fl;
	ofn.lpstrFile       = fname;
	ofn.nMaxFile        = 256;
	ofn.Flags           = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_NOREADONLYRETURN | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING;
	ofn.lpfnHook        = NULL;
	if (defaultToBinary)
		ofn.lpstrDefExt     = _T("env");
	else ofn.lpstrDefExt     = _T("envASCII");
	ofn.lpstrTitle      = title;

tryAgain:
	if (GetSaveFileName(&ofn)) {
		if (DoesFileExist(fname)) {
			TSTR buf1;
			TSTR buf2 = GetString(IDS_PW_SAVEENVELOPES);
			buf1.printf(GetString(IDS_PW_FILEEXISTS),fname);
			if (IDYES!=MessageBox(
				hParam,
				buf1,buf2,MB_YESNO|MB_ICONQUESTION)) {
					goto tryAgain;
			}
		}
		//save stuff here
		// this is timed slice so it will not save animation not sure how to save controller info but will neeed to later on in other plugs

		TSTR name(fname),p,f,e;
		SplitFilename(name,&p, &f, &e);

		TSTR scriptName;
		for (int i = 0; i < name.Length();i++)
		{
			TSTR appendName(scriptName);
			if (appendName.Length() > 0)
				scriptName.printf(_T("%s%c"),appendName,fname[i]);
			else scriptName.printf(_T("%c"),fname[i]);
			if (fname[i] == _T('\\'))
			{
				TSTR appendName2(scriptName);
				if (appendName2.Length() > 0)
					scriptName.printf(_T("%s\\"),appendName2);
				else scriptName.printf(_T("\\"));
			}
		}

		if (_tcscmp(e,_T(".envASCII")) == 0)
		{
			macroRecorder->FunctionCall(_T("skinOps.SaveEnvelopeAsASCII"), 2, 0, mr_reftarg, this, mr_string, scriptName );
			SaveEnvelope(fname,TRUE);
		}
		else
		{
			macroRecorder->FunctionCall(_T("skinOps.SaveEnvelope"), 2, 0, mr_reftarg, this, mr_string, scriptName );
			SaveEnvelope(fname,FALSE);
		}
	}
}

void BonesDefMod::SaveEnvelope(const TCHAR *fname, BOOL asText)
{
	MaxSDK::Util::TextFile::Writer textfile;
	FILE *file = NULL;

	Interface14 *iface = GetCOREInterface14();
	UINT codepage  = iface-> DefaultTextSaveCodePage(true);
	bool saveAsWCHAR = false;
	if(codepage == CP_UTF8) saveAsWCHAR = true;

	if (asText)
	{
		textfile.Open(fname, false, MaxSDK::Util::TextFile::Writer::WRITE_BOM | codepage );
		if( !textfile.CanWrite() )
			return;
	}
	else
	{
		// for a360 support - allows binary diff syncing
		MaxSDK::Util::Path storageNamePath(fname);
		storageNamePath.SaveBaseFile();

		file = _tfopen(fname,_T("wb"));
		if( !file )
			return;
	}

	//ver
	int ver = 3;
	if(saveAsWCHAR) ver = 4;
	if (asText)
		textfile.Printf(_T("ver %d\n"),ver);
	else fwrite(&ver, sizeof(ver), 1,file);

	//number bones
	int ct = NumberNonNullBones();
	if (asText)
		textfile.Printf(_T("numberBones %d\n"),ct);
	else fwrite(&ct, sizeof(ct), 1,file);

	for (int i=0; i < BoneData.Count(); i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node != NULL)
		{
			Class_ID bid(BONE_CLASS_ID,0);
			ObjectState os = boneData.Node->EvalWorldState(RefFrame);
			TCHAR title[255];
			if (( os.obj->ClassID() == bid) && (!boneData.name.isNull()) )
			{
				_tcscpy(title,boneData.name);
			}
			else _tcscpy(title,boneData.Node->GetName());

			int fnameLength = static_cast<int>(_tcslen(title)+1);

			if (asText)
				textfile.Printf(_T("[boneName] %s\n"),title);
			else if(saveAsWCHAR)
			{
				//bone name length size
				fwrite(&fnameLength, sizeof(fnameLength), 1,file);
				//bone name
				fwrite(title, sizeof(TCHAR)*fnameLength, 1,file);
			}
			else
			{
				TSTR titleStr = title;
				MaxSDK::Util::MaxString titleMstr = titleStr.ToMaxString();
				const char* strPt = titleMstr.ToCP(codepage);
				fnameLength = (int)strlen(strPt)+1;
				//bone name length size
				fwrite(&fnameLength, sizeof(fnameLength), 1,file);
				//bone name
				fwrite(strPt, fnameLength, 1,file);
			}
			if (asText)
				textfile.Printf(_T("[boneID] %d\n"),i);
			else
				fwrite(&i, sizeof(i), 1,file);

			//flags
			if (asText)
			{
				textfile.Printf(_T("  boneFlagLock %d\n"),boneData.flags&BONE_LOCK_FLAG);
				textfile.Printf(_T("  boneFlagAbsolute %d\n"),boneData.flags&BONE_ABSOLUTE_FLAG);
				textfile.Printf(_T("  boneFlagSpline %d\n"),boneData.flags&BONE_SPLINE_FLAG);
				textfile.Printf(_T("  boneFlagSplineClosed %d\n"),boneData.flags&BONE_SPLINECLOSED_FLAG);
				textfile.Printf(_T("  boneFlagDrawEnveloe %d\n"),boneData.flags&BONE_DRAW_ENVELOPE_FLAG);
				textfile.Printf(_T("  boneFlagIsOldBone %d\n"),boneData.flags&BONE_BONE_FLAG);
				textfile.Printf(_T("  boneFlagDead %d\n"),boneData.flags&BONE_DEAD_FLAG);
			}
			else fwrite(&boneData.flags, sizeof(boneData.flags), 1,file);
			//FalloffType;
			if (asText)
			{
				textfile.Printf(_T("  boneFalloff %d\n"),boneData.FalloffType);
			}
			else fwrite(&boneData.FalloffType, sizeof(boneData.FalloffType), 1,file);
			//start point
			Point3 p;
			Interval iv;
			boneData.EndPoint1Control->GetValue(currentTime,&p,iv,CTRL_ABSOLUTE);
			if (asText)
			{
				textfile.Printf(_T("  boneStartPoint %f %f %f\n"),p.x,p.y,p.z);
			}
			else fwrite(&p, sizeof(p), 1,file);
			//end point
			boneData.EndPoint2Control->GetValue(currentTime,&p,iv,CTRL_ABSOLUTE);
			if (asText)
			{
				textfile.Printf(_T("  boneEndPoint %f %f %f\n"),p.x,p.y,p.z);
			}
			else fwrite(&p, sizeof(p), 1,file);
			//number cross sections
			int crossCount = boneData.CrossSectionList.Count();
			if (asText)
			{
				textfile.Printf(_T("  boneCrossSectionCount %d\n"),crossCount);
			}
			else fwrite(&crossCount, sizeof(crossCount), 1,file);

			for (int crossID=0;crossID<crossCount; crossID++)
			{
				//inner
				//outer
				float inner, outer,u;
				boneData.CrossSectionList[crossID].InnerControl->GetValue(currentTime,&inner,iv,CTRL_ABSOLUTE);
				boneData.CrossSectionList[crossID].OuterControl->GetValue(currentTime,&outer,iv,CTRL_ABSOLUTE);
				u = boneData.CrossSectionList[crossID].u;

				if (asText)
				{
					textfile.Printf(_T("    boneCrossSectionInner%d %f\n"),crossID,inner);
					textfile.Printf(_T("    boneCrossSectionOuter%d %f\n"),crossID,outer);
					textfile.Printf(_T("    boneCrossSectionU%d %f\n"),crossID,u);
				}
				else
				{
					fwrite(&inner, sizeof(inner), 1,file);
					fwrite(&outer, sizeof(outer), 1,file);
					fwrite(&u, sizeof(u), 1,file);
				}
			}
		}
	}

	//put in vertex data here
	if (asText)
	{
		textfile.Printf(_T("[Vertex Data]\n"));
	}
	MyEnumProc dep;
	DoEnumDependents(&dep);
	//number of nodes instanced to this one modifier
	int nodeCount = 0; //dep.Nodes.Count();

	for (int i = 0; i < dep.Nodes.Count(); i++)
	{
		BoneModData *bmd = GetBMD(dep.Nodes[i]);
		if (bmd) nodeCount++;
	}

	if (asText)
		textfile.Printf(_T("  nodeCount %d\n"),nodeCount);
	else fwrite(&nodeCount, sizeof(nodeCount), 1,file);
	for (int i = 0; i < dep.Nodes.Count(); i++)
	{
		BoneModData *bmd = GetBMD(dep.Nodes[i]);
		if (bmd)
		{
			//write node name
			TCHAR title[255];
			ObjectState os = dep.Nodes[i]->EvalWorldState(RefFrame);
			_tcscpy(title,dep.Nodes[i]->GetName());

			int fnameLength = static_cast<int>(_tcslen(title)+1);

			if (asText)
				textfile.Printf(_T("  [baseNodeName] %s\n"),title);
			else if(saveAsWCHAR)
			{
				//bone name length size
				fwrite(&fnameLength, sizeof(fnameLength), 1,file);
				//bone name
				fwrite(title, sizeof(TCHAR)*fnameLength, 1,file);
			}
			else
			{
				TSTR titleStr = title;
				MaxSDK::Util::MaxString titleMstr = titleStr.ToMaxString();
				const char* strPt = titleMstr.ToCP(codepage);
				fnameLength = (int)strlen(strPt)+1;
				//bone name length size
				fwrite(&fnameLength, sizeof(fnameLength), 1,file);
				//bone name
				fwrite(strPt, fnameLength, 1,file);
			}

			//write number of vertices
			int vertexCount = bmd->VertexData.Count();
			if (asText)
				textfile.Printf(_T("    vertexCount %d\n"),vertexCount);
			else fwrite(&vertexCount, sizeof(vertexCount), 1,file);

			for (int j = 0; j < vertexCount; j++)
			{
				if (asText)
					textfile.Printf(_T("    [vertex%d]\n"),j);
				int weightCount = bmd->VertexData[j]->WeightCount();

				//write flags modified, rigid, rigid handle, unnormalized
				if (asText)
				{
					textfile.Printf(_T("      vertexIsModified %d\n"),bmd->VertexData[j]->IsModified());
					textfile.Printf(_T("      vertexIsRigid %d\n"),bmd->VertexData[j]->IsRigid());
					textfile.Printf(_T("      vertexIsRigidHandle %d\n"),bmd->VertexData[j]->IsRigidHandle());
					textfile.Printf(_T("      vertexIsUnNormalized %d\n"),bmd->VertexData[j]->IsUnNormalized());
				}
				else fwrite(&bmd->VertexData[j]->flags, sizeof(bmd->VertexData[j]->flags), 1,file);
				//local pos
				if (asText)
				{
					textfile.Printf(_T("      vertexLocalPosition %f %f %f\n"),bmd->VertexData[j]->LocalPos.x,bmd->VertexData[j]->LocalPos.y,bmd->VertexData[j]->LocalPos.z);
				}
				else fwrite(&bmd->VertexData[j]->LocalPos, sizeof(bmd->VertexData[j]->LocalPos), 1,file);

				//write number weights
				if (asText)
					textfile.Printf(_T("      vertexWeightCount %d\n"),weightCount);
				else fwrite(&weightCount, sizeof(weightCount), 1,file);

				//write weights
				if (asText)
					textfile.Printf(_T("      vertexWeight"));

				for (int k = 0; k < weightCount; k++)
				{
					int id;
					float weight;
					id = bmd->VertexData[j]->GetBoneIndex(k);
					if (!bmd->VertexData[j]->IsModified())
						weight = bmd->VertexData[j]->GetNormalizedWeight(k);
					else weight = bmd->VertexData[j]->GetWeight(k);

					if ( (weightCount == 0) && (!bmd->VertexData[j]->IsUnNormalized()))
						weight = 1.0f;

					if (asText)
						textfile.Printf(_T(" %d,%f"),id,weight);
					else
					{
						fwrite(&id, sizeof(id), 1,file);
						fwrite(&weight, sizeof(weight), 1,file);
					}
				}
				if (asText)
					textfile.Printf(_T("\n"));

				//write spline data
				if (asText)
					textfile.Printf(_T("      vertexSplineData"));

				for (int k = 0; k < weightCount; k++)
				{
					float u = bmd->VertexData[j]->GetCurveU(k);
					int curve = bmd->VertexData[j]->GetCurveID(k);
					int seg = bmd->VertexData[j]->GetSegID(k);
					Point3 tan = bmd->VertexData[j]->GetTangent(k);
					Point3 p = bmd->VertexData[j]->GetOPoint(k);

					if (asText)
					{
						TSTR test;
						test.printf(_T("%f"),u);
						if ( (_tcscmp(_T("-1.#QNAN0"),test) == 0) || (_tcscmp(_T("1.#QNAN0"),test) == 0) )
							u = 0.0f;

						test.printf(_T("%f"),p.x);
						if ((_tcscmp(_T("-1.#QNAN0"),test) == 0) || (_tcscmp(_T("1.#QNAN0"),test) == 0) )
							p.x = 0.0f;
						test.printf(_T("%f"),p.y);
						if ((_tcscmp(_T("-1.#QNAN0"),test) == 0) ||(_tcscmp(_T("1.#QNAN0"),test) == 0) )
							p.y = 0.0f;
						test.printf(_T("%f"),p.z);
						if ((_tcscmp(_T("-1.#QNAN0"),test) == 0) || (_tcscmp(_T("1.#QNAN0"),test) == 0))
							p.z = 0.0f;

						test.printf(_T("%f"),tan.x);
						if ((_tcscmp(_T("-1.#QNAN0"),test) == 0) || (_tcscmp(_T("1.#QNAN0"),test) == 0) )
							tan.x = 0.0f;
						test.printf(_T("%f"),tan.y);
						if ((_tcscmp(_T("-1.#QNAN0"),test) == 0) || (_tcscmp(_T("1.#QNAN0"),test) == 0) )
							tan.y = 0.0f;
						test.printf(_T("%f"),tan.z);
						if ((_tcscmp(_T("-1.#QNAN0"),test) == 0) || (_tcscmp(_T("1.#QNAN0"),test) == 0) )
							tan.z = 0.0f;

						textfile.Printf(_T(" %f %d %d %f %f %f %f %f %f   "),u,curve,seg,p.x,p.y,p.z,tan.x,tan.y,tan.z);
					}
					else
					{
						fwrite(&u, sizeof(u), 1,file);
						fwrite(&curve, sizeof(curve), 1,file);
						fwrite(&seg, sizeof(seg), 1,file);
						fwrite(&p, sizeof(p), 1,file);
						fwrite(&tan, sizeof(tan), 1,file);
					}
				}
				if (asText)
					textfile.Printf(_T("\n"));
			}

			//write exclusion lists
			int exclusionTotal = bmd->exclusionList.Count();
			if (asText)
			{
				textfile.Printf(_T("  numberOfExclusinList %d\n"),exclusionTotal);
			}
			else fwrite(&exclusionTotal, sizeof(exclusionTotal), 1,file);

			for (int j = 0; j < exclusionTotal; j++)
			{
				int exclusionCount = 0;
				if (asText)
					textfile.Printf(_T("  [exclusionListForBone %d]\n"),j);
				if (bmd->exclusionList[j]) exclusionCount = bmd->exclusionList[j]->Count();
				if (asText)
				{
					textfile.Printf(_T("    exclusionVertexCount %d\n"),exclusionCount);
				}
				else fwrite(&exclusionCount, sizeof(exclusionCount), 1,file);
				if (exclusionCount > 0)
				{
					if (asText)
						textfile.Printf(_T("    excludedVerts "));

					for (int k = 0; k < exclusionCount; k++)
					{
						int id = bmd->exclusionList[j]->Vertex(k);
						if (asText)
							textfile.Printf(_T(" %d"),id);
						else fwrite(&id, sizeof(id), 1,file);
					}

					if (asText)
						textfile.Printf(_T("\n"));
				}
			}
		}
	}
	if(file) fclose(file);
}

void BonesDefMod::LoadEnvelopeDialog(BOOL defaultToBinary)
{
	static TCHAR fname[256] = {_T('\0')};
	OPENFILENAME ofn;
	memset(&ofn,0,sizeof(ofn));
	FilterList fl;
	fl.Append( GetString(IDS_PW_ENVFILES));
	fl.Append( _T("*.env"));
	fl.Append( GetString(IDS_PW_ENVTEXTFILES));
	fl.Append( _T("*.envASCII"));

	TSTR title = GetString(IDS_PW_LOADENVELOPES);

	ofn.lStructSize     = sizeof(OPENFILENAME);  // No OFN_ENABLEHOOK
	ofn.hwndOwner       = GetCOREInterface()->GetMAXHWnd();
	ofn.lpstrFilter     = fl;
	ofn.lpstrFile       = fname;
	ofn.nMaxFile        = 256;
	//ofn.lpstrInitialDir = ip->GetDir(APP_EXPORT_DIR);
	ofn.Flags           = OFN_HIDEREADONLY|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLEHOOK | OFN_ENABLESIZING;
	ofn.lpfnHook        = NULL;
	if (defaultToBinary)
		ofn.lpstrDefExt     = _T("env");
	else ofn.lpstrDefExt     = _T("envASCII");
	ofn.lpstrTitle      = title;

	if (GetOpenFileName(&ofn)) {
		//load stuff here
		TSTR name(fname),p,f,e;
		SplitFilename(name,&p, &f, &e);

		TSTR scriptName;
		for (int i = 0; i < name.Length();i++)
		{
			TSTR appendName(scriptName);
			if (appendName.Length() > 0)
				scriptName.printf(_T("%s%c"),appendName,fname[i]);
			else scriptName.printf(_T("%c"),fname[i]);
			if (fname[i] == _T('\\'))
			{
				TSTR appendName2(scriptName);
				if (appendName2.Length() > 0)
					scriptName.printf(_T("%s\\"),appendName2);
				else scriptName.printf(_T("\\"));
			}
		}

		if (_tcscmp(e,_T(".envASCII"))==0)
		{
			macroRecorder->FunctionCall(_T("skinOps.LoadEnvelopeAsASCII"), 2, 0, mr_reftarg, this, mr_string, scriptName );
			LoadEnvelope(fname,TRUE);
		}
		else
		{
			macroRecorder->FunctionCall(_T("skinOps.LoadEnvelope"), 2, 0, mr_reftarg, this, mr_string, scriptName );
			LoadEnvelope(fname);
		}
	}
}

void* BonesDefMod::GetInterface(ULONG id)
{
	switch(id)
	{
	case I_SKIN : return (ISkin *) this;
		break;
	case I_SKINIMPORTDATA : return (ISkinImportData *) this;
		break;
	case PAINTERCANVASINTERFACE_V5 : return (IPainterCanvasInterface_V5 *) this;
		break;
	case PAINTERCANVASINTERFACE_V5_1 : return (IPainterCanvasInterface_V5_1 *) this;
		break;
	case I_SKIN2 : return (ISkin2 *) this;
		break;
	default: return Modifier::GetInterface(id);
		break;
	}
}

BaseInterface* BonesDefMod::GetInterface(Interface_ID id)
{
	if (id==IID_LOCKED_CONTAINER_UPDATE)
		return static_cast<ILockedContainerUpdate *>(this);
	else
		return Modifier::GetInterface(id);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ILockedContainerUpdate  Interface
///////////////////////////////////////////////////////////////////////////////////////////////////////
//This function is called during local edits. We need to

bool BonesDefMod::PreReplacement(INode *mergedNode, Animatable *merged, INode *currentNode, Animatable *replacedAnim, IContainerUpdateReplacedNode *man, TSTR &log)
{
	//Save the weights on the merged in data
	//First we need to get the modData
	BoneModData *mergedBMD = GetBMD(mergedNode);
	if(mergedBMD)
	{
		//Save the weights out fo the merged in one.
		CopyWeights(mergedBMD,mergedNode);
	}
	else
		return false;
	return true;
}

bool BonesDefMod::PostReplacement(INode *mergedNode, Animatable *merged, INode *currentNode,  IContainerUpdateReplacedNode *man, TSTR &log)
{
	//First the we do, is get the IDerivedObject of the modifier that we will be replacing.
	//This let's us replace it with the merged in modifier.
	IDerivedObject *derObj;
	int modIndex;

	//get the the BMD on the currentNode (remember this is after it's pasted) so we have two instances, on on merged, and one on pasted.
	BoneModData *currentBMD = GetBMDAndDerObj(currentNode,&derObj,modIndex);

	//both the merged in and the one to be replaced are good
	if(currentBMD )
	{
		//at this point the weights we have saved our lost. we need to paste them,
		//which for the skin paste function requires setting up the ModContextList and a
		//node tab list.
		ModContextList mcList;
		ModContext *modC = derObj->GetModContext(modIndex);
		mcList.Append(1,&modC);
		INodeTab nodes;
		if(modC)
			nodes.Append(1,&currentNode);
		if(nodes.Count()>0)
		{
			float tolerance = 0.;
			pblock_param->GetValue(skin_weighttool_tolerance,0,tolerance,FOREVER);
			PasteWeights(GetCOREInterface()->GetTime(),FALSE,tolerance,FALSE,mcList,nodes);
		}
	}
	else
		return false;
	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ISkin Interface :
///////////////////////////////////////////////////////////////////////////////////////////////////////

int BonesDefMod::GetBoneInitTM(INode *pNode, Matrix3 &InitTM, bool bObjOffset)
{
	UpdateBoneMap();

	for(int i = 0 ; i < GetNumBones() ; i++)
	{
		if(pNode == BoneData[BoneMap[i]].Node)
		{
			if(bObjOffset)
				InitTM = BoneData[BoneMap[i]].InitObjectTM;
			else
				InitTM = BoneData[BoneMap[i]].InitNodeTM;
			return SKIN_OK;
		}
	}
	return SKIN_INVALID_NODE_PTR;
}

int BonesDefMod::GetSkinInitTM(INode *pNode, Matrix3 &InitTM, bool bObjOffset)
{
	BoneModData *bmd = GetBMD(pNode);

	if(!bmd) return SKIN_INVALID_NODE_PTR;

	if(bmd->recompInitMeshTM)
	{
		Matrix3 diffTM(TRUE);
		Matrix3 stretchTM(TRUE);
		stretchTM = pNode->GetStretchTM(RefFrame);
		diffTM =  pNode->GetNodeTM(RefFrame) * Inverse(pNode->GetObjectTM(RefFrame));
		bmd->BaseNodeTM = diffTM * bmd->BaseTM;
		bmd->recompInitMeshTM = false;
	}

	if(bObjOffset)
		InitTM = bmd->BaseTM;
	else
		InitTM = bmd->BaseNodeTM;

	return SKIN_OK;
}

int BonesDefMod::GetNumBones()
{
	UpdateBoneMap();
	return BoneMap.Count();
}

INode *BonesDefMod::GetBone(int boneID)
{
	UpdateBoneMap();
	if (boneID < 0 || boneID >= BoneMap.Count())
		return nullptr;
	return BoneData[BoneMap[boneID]].Node;
}

void BonesDefMod::UpdateBoneMap()
{
	if(recompBoneMap)
	{
		int cnt = 0;
		BoneMap.SetCount(BoneData.Count());
		for( int i = 0 ; i < BoneData.Count() ; i++ )
		{
			if(BoneData[i].Node != NULL)
			{
				BoneMap[cnt++] = i;
			}
		}
		BoneMap.SetCount(cnt);
		recompBoneMap = false;
	}
}

DWORD BonesDefMod::GetBoneProperty(int boneID)
{
	UpdateBoneMap();
	if (boneID < 0 || boneID >= BoneMap.Count())
		return 0;
	return BoneData[BoneMap[boneID]].flags;
}

int BonesDefMod::GetSelectedBone()
{
	return ModeBoneIndex;
}

void BonesDefMod::GetEndPoints(int boneIndex, Point3 &l1, Point3 &l2)
{
	if (!IsValidBoneIndex(boneIndex)) return;
	BoneDataClass &boneData = BoneData[boneIndex];
	boneData.EndPoint1Control->GetValue(currentTime,&l1,FOREVER);
	boneData.EndPoint2Control->GetValue(currentTime,&l2,FOREVER);
}

Matrix3 BonesDefMod::GetBoneTm(int boneIndex)
{
	if (!IsValidBoneIndex(boneIndex)) return Matrix3(1);
	return BoneData[boneIndex].tm;
}

INode *BonesDefMod::GetBoneFlat(int boneIndex)
{
	if (!IsValidBoneIndex(boneIndex)) return NULL;
	return BoneData[boneIndex].Node;
}

int BonesDefMod::GetNumBonesFlat()
{
	return BoneData.Count();
}

int BonesDefMod::GetRefFrame()
{
	return RefFrame;
}

ISkinContextData *BonesDefMod::GetContextInterface(INode *pNode)
{
	BoneModData *bmd = GetBMD(pNode);
	if(!bmd)
		return NULL;
	else
		return (ISkinContextData*) bmd;
}

//to use pass in a bonedefmod to the contstructor, this is then
//used to find that skin modifier on the stack
//BonesDefModFindOnStack pipeEnumProc(this);
//EnumGeomPipeline(&pipeEnumProc, pObj);
class BonesDefModFindOnStack : public GeomPipelineEnumProc
{
public:
	BonesDefModFindOnStack(ReferenceTarget *me) : mFound(false), mRef(me),mDerObj(NULL),mIndex(-1) {}
	PipeEnumResult proc(ReferenceTarget *object, IDerivedObject *derObj, int index);
	bool mFound;
	ReferenceTarget *mRef;  //what we are looking for on the stack
	//what we basically return, the derived object and the index.
	IDerivedObject *mDerObj;
	int mIndex;

protected:
	BonesDefModFindOnStack(); // disallowed
	BonesDefModFindOnStack(BonesDefModFindOnStack& rhs); // disallowed
	BonesDefModFindOnStack& operator=(const BonesDefModFindOnStack& rhs); // disallowed
};

PipeEnumResult BonesDefModFindOnStack::proc(
	ReferenceTarget *object,
	IDerivedObject *derObj,
	int index)
{
	ModContext* curModContext = NULL;
	if ((derObj != NULL) && (object == mRef))
	{
		mDerObj = derObj;
		mIndex = index;
		mFound = true;
		return PIPE_ENUM_STOP;
	}
	return PIPE_ENUM_CONTINUE;
}

BoneModData * BonesDefMod::GetBMDAndDerObj(INode *pNode, IDerivedObject **derObj, int &Idx)
{
	ModContext *mc = NULL;

	Object* pObj = pNode->GetObjectRef();

	if (!pObj) return NULL;

	BonesDefModFindOnStack pipeEnumProc(this);
	EnumGeomPipeline(&pipeEnumProc, pObj);
	if (pipeEnumProc.mFound)
	{
		*derObj = pipeEnumProc.mDerObj;
		Idx = pipeEnumProc.mIndex;
		mc = (*derObj)->GetModContext(Idx);
		if(mc && mc->localData)
		{
			BoneModData *bmd = (BoneModData *) mc->localData;
			return bmd;
		}
		return NULL;
	}
	return NULL;
}

BoneModData *BonesDefMod::GetBMD(INode *pNode)
{
	IDerivedObject *derObj;
	int idx;
	return GetBMDAndDerObj(pNode, &derObj,idx);
}

int BonesDefMod::NumSubObjTypes()
{
	return 1;
}

ISubObjType *BonesDefMod::GetSubObjType(int i)
{
	static bool initialized = false;
	if(!initialized)
	{
		initialized = true;
		SOT_DefPoints.SetName(GetString(IDS_RB_BONESDEFPOINTS));
	}

	switch(i)
	{
	case 0:
		return &SOT_DefPoints;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// LocalModData Implementation
///////////////////////////////////////////////////////////////////////////////////////////////////////

void BoneModData::InitSkinEngine()
{
	static bool sCheckWetherToInitSkinEngine = true;
	static BOOL sLoadSkinEngine = TRUE;
	if (sCheckWetherToInitSkinEngine)
	{
		sCheckWetherToInitSkinEngine = false;
		TSTR strCfgFile;
		strCfgFile.printf(_T("%s\\%s"), GetCOREInterface()->GetDir(APP_PLUGCFG_DIR), SKIN_INI_FILENAME );
		TCHAR buffer[256];
		if (GetPrivateProfileString(LOAD_OLE_SKIN_ENGINE, SKIN_ENABLED, _T(""), buffer, _countof(buffer), strCfgFile) > 0)
			sLoadSkinEngine = (BOOL) _ttoi(buffer);
		else
		{
			swprintf(buffer, _countof(buffer), _T("%d"), sLoadSkinEngine);
			MaxSDK::Util::WritePrivateProfileString(LOAD_OLE_SKIN_ENGINE,SKIN_ENABLED, buffer, strCfgFile);
		}
	}

	pSE = NULL;

	if (!sLoadSkinEngine)
		return;

	CoInitialize(NULL);

	// This is two steps in one, CreateInstance IUnknown + QureyInterface IFlexEngine
	HRESULT hr = CoCreateInstance( CLSID_SkinEngine, NULL, CLSCTX_INPROC_SERVER,IID_ISkinEngine, (void **)&pSE);

	static bool sPreviouslyFailed = false; // only try to register once
	if( FAILED(hr))
	{
		if (sPreviouslyFailed)
		{
			CoUninitialize();
			return;
		}
		else
		{
			sPreviouslyFailed = true;
			//if failed try to register the dll
			Interface* ip = GetCOREInterface();

			const TCHAR *path = ip->GetDir(APP_MAX_SYS_ROOT_DIR);
			ShellExecute(ip->GetMAXHWnd(), _T("open"), _T("regsvr32.exe"), _T("/s atl.dll"), NULL, SW_SHOWNORMAL);
			Sleep(3000);
			ShellExecute(ip->GetMAXHWnd(), _T("open"), _T("regsvr32.exe"), _T("/s MAXComponents.dll"), path, SW_SHOWNORMAL);
			Sleep(3000);
			hr = CoCreateInstance( CLSID_SkinEngine, NULL, CLSCTX_INPROC_SERVER,IID_ISkinEngine, (void **)&pSE);

			//if failed again something bad has happened and bail
			if( FAILED(hr))
			{
				TSTR msg = _T("CoCreateInstance() failed\nPlease check your registry entries\nCLSID {F088EA74-2E87-11D3-B1F3-00C0F03C37D3} and make sure you are logged in as an administrator");
				TSTR title = _T("COM Error");
				ip->Log()->LogEntry(SYSLOG_ERROR,DISPLAY_DIALOG, title, _T("%s\n"),msg);
				CoUninitialize();
				return;
			}
		}
	}

	// Register the callback
	if (!pSkinCallback)
	{
		pSkinCallback	= new CSkinCallback(this);
		HRESULT hr	= AtlAdvise(pSE, (IUnknown *)pSkinCallback,IID__ISkinEngineEvents,&cookie);
		if (FAILED(hr))
		{
			pSkinCallback = 0;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ISkinContextData Interface
///////////////////////////////////////////////////////////////////////////////////////////////////////

int BoneModData::GetNumPoints()
{
	return VertexData.Count();
}

int BoneModData::GetNumAssignedBones(int pointIdx)
{
	if (pointIdx>=VertexData.Count()) {
		// TODO: Peter - should this be happening?  pointIdx >= GetNumPoints()?
		// (Happens most easily on freshly-created SkinWraps, which you then "Convert to Skin".)
		// - Steve Anderson, 9/16/04
		DbgAssert (false);
		return 0;
	}
	return VertexData[pointIdx]->WeightCount();
}

int BoneModData::GetAssignedBone(int pointIdx, int boneIdx)
{
	mod->UpdateBoneMap();

	for(int i = 0 ; i < mod->BoneMap.Count() ; i++)
	{
		if(mod->BoneMap[i] == VertexData[pointIdx]->GetBoneIndex(boneIdx))
			return i;
	}
	return -1;
}

float BoneModData::GetBoneWeight(int pointIdx, int boneIdx)
{
	return VertexData[pointIdx]->GetNormalizedWeight(boneIdx);
}

int BoneModData::GetSubCurveIndex(int pointIdx, int boneIdx)
{
	return VertexData[pointIdx]->GetCurveID(boneIdx);
}

int BoneModData::GetSubSegmentIndex(int pointIdx, int boneIdx)
{
	return VertexData[pointIdx]->GetSegID(boneIdx);
}

float BoneModData::GetSubSegmentDistance(int pointIdx, int boneIdx)
{
	return VertexData[pointIdx]->GetCurveU(boneIdx);
}

Point3 BoneModData::GetTangent(int pointIdx, int boneIdx)
{
	return VertexData[pointIdx]->GetTangent(boneIdx);
}

Point3 BoneModData::GetOPoint(int pointIdx, int boneIdx)
{
	return VertexData[pointIdx]->GetOPoint(boneIdx);
}

BOOL ExclusionListClass::isInList(int vertID, int &where)
{
	for (int i = 0; i < exList.Count(); i++)
	{
		if (exList[i] == vertID)
		{
			where= i;
			return TRUE;
		}
	}
	return FALSE;
}

BOOL ExclusionListClass::isExcluded(int vertID)
{
	for (int i = 0; i < exList.Count(); i++)
	{
		if (exList[i] == vertID) return TRUE;
	}
	return FALSE;
}

void ExclusionListClass::ExcludeVerts(const Tab<int>& exclusionList)
{
	for (int i = 0; i < exclusionList.Count(); i++)
	{
		int vertID = exclusionList[i];
		int where;
		if (!isInList(vertID,where))
		{
			exList.Append(1,&vertID);
		}
	}
}

void ExclusionListClass::SetExclusionVerts(const Tab<int>& exclusionList)
{
	exList.ZeroCount();
	exList = exclusionList;
}

void ExclusionListClass::IncludeVerts(const Tab<int>& inclusionList)
{
	for (int i = 0; i < inclusionList.Count(); i++)
	{
		int vertID = inclusionList[i];
		int where;
		if (isInList(vertID,where))
		{
			exList.Delete(where,1);
		}
	}
}

IOResult ExclusionListClass::Save(ISave *isave)
{
	unsigned long nb;
	int c = exList.Count();
	IOResult ior;
	ior = isave->Write(&c,sizeof(c),&nb);
	for (int i = 0; i < exList.Count(); i++)
	{
		c = exList[i];
		ior = isave->Write(&c,sizeof(c),&nb);
	}
	return ior;
}

IOResult ExclusionListClass::Load(ILoad *iload)
{
	unsigned long nb;
	int c = 0;
	IOResult ior;
	ior = iload->Read(&c,sizeof(c),&nb);
	exList.SetCount(c);
	for (int i = 0; i < exList.Count(); i++)
	{
		int id = 0;
		ior = iload->Read(&id,sizeof(id),&nb);
		exList[i] = id;
	}

	return ior;
}

BOOL BoneModData::isExcluded(int boneIndex, int vertID)
{
	if (boneIndex < 0 || boneIndex>=exclusionList.Count())
		return FALSE;
	if (exclusionList[boneIndex])
	{
		return exclusionList[boneIndex]->isExcluded(vertID);
	}
	return FALSE;
}

void BoneModData::ExcludeVerts(int boneIndex, const Tab<int>& exList, BOOL cleanUpVerts)
{
	if (boneIndex < 0)
		return;
	if (boneIndex>=exclusionList.Count())
	{
		int oldCount = exclusionList.Count();
		exclusionList.SetCount(boneIndex+1);
		for (int i = oldCount; i < exclusionList.Count(); i++)
			exclusionList[i] = NULL;
	}
	if (!exclusionList[boneIndex])
	{
		exclusionList[boneIndex] = new ExclusionListClass();
		exclusionList[boneIndex]->ExcludeVerts(exList);
	}
	else exclusionList[boneIndex]->ExcludeVerts(exList);

	//now loop through all vertices on this bone remove them if they are excluded
	if (cleanUpVerts)
	{
		for (int i =0; i < VertexData.Count(); i++)
		{
			for (int j = VertexData[i]->WeightCount()-1; j >= 0; j--)
			{
				if ( isExcluded(boneIndex, i) && (VertexData[i]->GetBoneIndex(j) == boneIndex))
					VertexData[i]->DeleteWeight(j);
			}
		}
	}
}

void BoneModData::CleanUpExclusionLists()
{
	for (int m=0; m < exclusionList.Count(); m++)
	{
		if (exclusionList[m])
		{
			for (int i =0; i < VertexData.Count(); i++)
			{
				for (int j = VertexData[i]->WeightCount()-1; j >= 0; j--)
				{
					if ( isExcluded(m, i) && (VertexData[i]->GetBoneIndex(j) == m))
						VertexData[i]->DeleteWeight(j);
				}
			}
		}
	}
}

void BoneModData::IncludeVerts(int boneIndex, const Tab<int>& inList)
{
	if (boneIndex < 0 || boneIndex>=exclusionList.Count())
		return;
	else
	{
		if (exclusionList[boneIndex])
		{
			exclusionList[boneIndex]->IncludeVerts(inList);
		}
	}
}

void BoneModData::SelectExcludedVerts(int boneIndex)
{
	if (boneIndex < 0 || boneIndex>=exclusionList.Count())
		return;
	else
	{
		if (exclusionList[boneIndex])
		{
			selected.ClearAll();
			for (int i = 0; i < VertexData.Count(); i++)
				selected.Set (i,FALSE);

			for (int i = 0; i < exclusionList[boneIndex]->Count(); i++)
			{
				int id = exclusionList[boneIndex]->Vertex(i);
				selected.Set(id,1);
			}
		}
	}
}

void BoneModData::BuildEdgeList()
{
	if (meshNode == NULL) return;

	ObjectState sos = meshNode->EvalWorldState(GetCOREInterface()->GetTime());
	blurredWeights.ZeroCount();
	if (sos.obj->IsSubClassOf(triObjectClassID))
	{
		TriObject *tobj = (TriObject*)sos.obj;
		Mesh *msh = &tobj->GetMesh();

		//if point count different bail
		if (msh->numVerts != VertexData.Count()) return;
		edgeList = new AdjEdgeList(*msh);
		blurredWeights.SetCount(msh->numVerts);
		for (int i = 0; i < msh->numVerts; i++)
			blurredWeights[i] = 0.0f;
	}

	else if (sos.obj->IsSubClassOf(polyObjectClassID))
	{
		PolyObject *tobj = (PolyObject*)sos.obj;
		// Apply our mapping
		MNMesh &mesh = tobj->GetMesh();
		mnEdges.SetCount(mesh.nume);
		for (int i = 0; i < mesh.nume; i++)
			mnEdges[i] = mesh.e[i];

		blurredWeights.SetCount(mesh.numv);
		for (int i = 0; i < mesh.numv; i++)
			blurredWeights[i] = 0.0f;
	}
#ifndef NO_PATCHES

	else if (sos.obj->IsSubClassOf(patchObjectClassID))
	{
		PatchObject *tobj = (PatchObject*)sos.obj;
		// Apply our mapping
		PatchMesh &pmesh = tobj->patch;
		patchEdges.SetCount(pmesh.numEdges);
		for (int i = 0; i < pmesh.numEdges; i++)
		{
			patchEdges[i].v1 = pmesh.edges[i].v1;
			patchEdges[i].v2 = pmesh.edges[i].v2;
		}

		blurredWeights.SetCount(VertexData.Count());
		for (int i = 0; i < VertexData.Count(); i++)
			blurredWeights[i] = -1.0f;
	}
#endif
	//if mesh
	//get adr edge list
	//clone it local
	//if poly
	//etc
}

void BoneModData::BuildBlurData( int boneIndex )
{
	if (VertexData.Count() != blurredWeights.Count())
	{
		blurredWeights.SetCount(0);
		return;
	}

	if (edgeList)
	{
		for (int i = 0; i < VertexData.Count(); i++)
		{
			//find all the neighbors
			int numberOfNeighors = edgeList->list[i].Count();
			//sum there weights and divide
			float weight = 0.0f;

			for (int j = 0; j < numberOfNeighors; j++)
			{
				int neighborEdge = edgeList->list[i][j];
				int neighbor = edgeList->edges[neighborEdge].v[0];

				if (neighbor == i) neighbor = edgeList->edges[neighborEdge].v[1];

				//set that as the blurred weight
				//look for a bone
				int numberOfBones = 0;

				if ((neighbor >= 0) && (neighbor < VertexData.Count()))
				{
					numberOfBones = VertexData[neighbor]->WeightCount();
					for (int k = 0; k <numberOfBones; k++)
					{
						int neighbor_boneIndex = VertexData[neighbor]->GetBoneIndex(k);
						if (neighbor_boneIndex == boneIndex)
						{
							//found a vertex with some weight
							weight +=  VertexData[neighbor]->GetNormalizedWeight(k);
						}
					}
				}
				else
				{
				}
			}

			blurredWeights[i] = weight/numberOfNeighors;
		}
	}
	else if (mnEdges.Count() != 0)
	{
		Tab<int> weightCounts;
		weightCounts.SetCount(VertexData.Count());

		for (int i = 0; i < weightCounts.Count(); i++)
		{
			weightCounts[i] = 0.0f;
			blurredWeights[i] = 0.0f;
		}
		for (int i = 0; i < mnEdges.Count(); i++)
		{
			int v1 = mnEdges[i].v1;
			int v2 = mnEdges[i].v2;

			int numberOfBones = VertexData[v1]->WeightCount();
			for (int k = 0; k <numberOfBones; k++)
			{
				int neighbor_boneIndex = VertexData[v1]->GetBoneIndex(k);
				if (neighbor_boneIndex == boneIndex)
				{
					//found a vertex with some weight
					blurredWeights[v2] +=  VertexData[v1]->GetNormalizedWeight(k);
					weightCounts[v2] += 1;
				}
			}

			v1 = mnEdges[i].v2;
			v2 = mnEdges[i].v1;

			numberOfBones = VertexData[v1]->WeightCount();
			for (int k = 0; k <numberOfBones; k++)
			{
				int neighbor_boneIndex = VertexData[v1]->GetBoneIndex(k);
				if (neighbor_boneIndex == boneIndex)
				{
					//found a vertex with some weight
					blurredWeights[v2] +=  VertexData[v1]->GetNormalizedWeight(k);
					weightCounts[v2] += 1;
				}
			}
		}
		for (int i = 0; i < weightCounts.Count(); i++)
		{
			if (weightCounts[i] != 0)
				blurredWeights[i] = blurredWeights[i]/(float)weightCounts[i];
		}
	}
	else if (patchEdges.Count() != 0)
	{
		Tab<int> weightCounts;
		weightCounts.SetCount(VertexData.Count());

		for (int i = 0; i < weightCounts.Count(); i++)
		{
			weightCounts[i] = 0.0f;
			blurredWeights[i] = 0.0f;
		}
		for (int i = 0; i < patchEdges.Count(); i++)
		{
			int v1 = patchEdges[i].v1;
			int v2 = patchEdges[i].v2;

			int numberOfBones = VertexData[v1]->WeightCount();
			for (int k = 0; k <numberOfBones; k++)
			{
				int neighbor_boneIndex = VertexData[v1]->GetBoneIndex(k);
				if (neighbor_boneIndex == boneIndex)
				{
					//found a vertex with some weight
					blurredWeights[v2] +=  VertexData[v1]->GetNormalizedWeight(k);
					weightCounts[v2] += 1;
				}
			}

			v1 = patchEdges[i].v2;
			v2 = patchEdges[i].v1;

			numberOfBones = VertexData[v1]->WeightCount();
			for (int k = 0; k <numberOfBones; k++)
			{
				int neighbor_boneIndex = VertexData[v1]->GetBoneIndex(k);
				if (neighbor_boneIndex == boneIndex)
				{
					//found a vertex with some weight
					blurredWeights[v2] +=  VertexData[v1]->GetNormalizedWeight(k);
					weightCounts[v2] += 1;
				}
			}
		}
		for (int i = 0; i < weightCounts.Count(); i++)
		{
			if (weightCounts[i] != 0)
				blurredWeights[i] = blurredWeights[i]/(float)weightCounts[i];
		}
	}
}

void BoneModData::FreeEdgeList()
{
	if (edgeList) delete edgeList;
	blurredWeights.ZeroCount();
	edgeList = NULL;
	mnEdges.ZeroCount();
	patchEdges.ZeroCount();
}

BoneModData::BoneModData( BonesDefMod *m )
{
	unlockVerts = FALSE;
	edgeList = NULL;

	lastTMCacheTime = -999931;
	recompInitMeshTM = false; //ns
	pSkinCallback = NULL;
	InitSkinEngine();
	isMesh = FALSE;
	effect = -1.0f;
	mod = m;
	reevaluate = FALSE;
	reevaluateActiveBone = FALSE;
	nukeValidCache = TRUE;

	meshNode = NULL;
	forceRecomuteBaseNode = FALSE;
	needTMsRebuilt = FALSE;
	rebuildWeights = TRUE;

	mesh = NULL;
	patch = NULL;
	mnMesh = NULL;
	mCacheDumped = true;

	RegisterNotification(DeleteNodeNotifyProc, (void*)this, NOTIFY_SCENE_PRE_DELETED_NODE);
}

BoneModData::~BoneModData()
{
	if (mesh) delete mesh;
	if (patch) delete patch;
	if (mnMesh) delete mnMesh;

	if(pSE)
	{
		// Unregister the Connection point
		HRESULT hr	= AtlUnadvise(pSE,IID__ISkinEngineEvents,cookie);
		pSE->Release();
		pSE = NULL;
	}

	for (int i=0;i<VertexData.Count();i++)	//free all our weights
	{
		VertexData[i]->ZeroWeights();
		if (VertexData[i] != NULL)
			delete (VertexData[i]);
		VertexData[i] = NULL;
	}
	VertexData.ZeroCount();

	for (int i = 0; i < exclusionList.Count(); i++)	//free the exclusions list
	{
		if (exclusionList[i])
			delete exclusionList[i];
	}
	for (int i = 0; i < gizmoData.Count(); i++)	//free the local gizmo data
	{
		if (gizmoData[i])
			delete gizmoData[i];
	}

	UnRegisterNotification(DeleteNodeNotifyProc, (void*)this, NOTIFY_SCENE_PRE_DELETED_NODE);
}

LocalModData* BoneModData::Clone()
{
	BoneModData* d = new BoneModData(NULL);
	d->reevaluate = FALSE;
	d->reevaluateActiveBone = FALSE;
	d->meshNode = NULL;
	d->forceRecomuteBaseNode = FALSE;
	d->needTMsRebuilt = TRUE;

	d->BaseTM = BaseTM;
	d->BaseNodeTM = BaseNodeTM; //ns
	d->InverseBaseTM = InverseBaseTM;

	int vertexCount = VertexData.Count();
	d->VertexData.SetCount(VertexData.Count());
	for (int i = 0; i < vertexCount; i++)
	{
		if (VertexData[i])
		{
			d->VertexData[i] = new VertexListClass();
			*d->VertexData[i] = *VertexData[i];
		}
		else d->VertexData[i] = NULL;
	}

	//need to clone exclusions
	d->exclusionList.SetCount(exclusionList.Count());
	for (int i = 0; i < exclusionList.Count(); i++)
	{
		if (exclusionList[i])
		{
			d->exclusionList[i] = new ExclusionListClass();
			*d->exclusionList[i] = *exclusionList[i];
		}
		else d->exclusionList[i]=NULL;
	}

	d->gizmoData.SetCount(gizmoData.Count());
	for (int i = 0; i < gizmoData.Count(); i++)
	{
		if (gizmoData[i])
		{
			d->gizmoData[i] = new LocalGizmoData();
			*d->gizmoData[i] = *gizmoData[i];
		}
		else d->gizmoData[i]=NULL;
	}

	return d;
}

void BoneModData::ClearMeshData()
{
	if (mesh) delete mesh;
	if (patch) delete patch;
	if (mnMesh) delete mnMesh;
	mesh = NULL;
	patch = NULL;
	mnMesh = NULL;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Deformer Implementation
///////////////////////////////////////////////////////////////////////////////////////////////////////

Point3 BonesDefDeformer::Map(int i, Point3 p)
{
	Point3 pout(0,0,0);

	Point3 initialP = p;

	if (bmd->isPatch)
	{
		if (bmd->autoInteriorVerts[i]) return p;
	}
	VertexListClass *vd = bmd->VertexData[i];
	int wct = vd->WeightCount();
	for( int j = 0 ; j < wct ; j++)
	{
		// This sets the normalizedInfluences values
		vd->GetNormalizedWeight(j);
	}

	if (bmd->pSE)
	{
		int boneID = vd->GetClosestBone();
		float val = 1.0f;
		int id = 0;
		Point3 tp(0.0f,0.0f,1.0f);
		Point3 op(0.0f,0.0f,0.0f);
		float fzero = 0.0f;
		int testID = 0;

		if	( (Cluster->fastUpdate && (!Cluster->inRender) && (vd->WeightCount() > 0)) ||
			((Cluster->rigidVerts) && (vd->WeightCount() > 0))
			)
		{
			int boneID = vd->GetMostAffectedBoneID();

			if ( (vd->WeightCount() == 1) && (vd->GetClosestBone() != -1))
			{
				testID = boneID;
				bmd->pSE->SetPointData(i, 1,
					sizeof(VertexInfluenceListClass), &boneID,
					sizeof(VertexInfluenceListClass), &val,
					sizeof(VertexInfluenceListClass), &id,
					sizeof(VertexInfluenceListClass), &id,
					sizeof(VertexInfluenceListClass), &fzero,
					sizeof(VertexInfluenceListClass), (float *)&tp,
					sizeof(VertexInfluenceListClass), (float *)&op);
			}
			else
			{
				testID =  vd->GetBoneIndex(boneID);
				bmd->pSE->SetPointData(i, 1,
					sizeof(VertexInfluenceListClass), vd->GetBoneIndexAddr(boneID),
					sizeof(VertexInfluenceListClass), &val,
					sizeof(VertexInfluenceListClass), vd->GetCurveIDAddr(boneID),
					sizeof(VertexInfluenceListClass), vd->GetSegIDAddr(boneID),
					sizeof(VertexInfluenceListClass), vd->GetCurveUAddr(boneID),
					sizeof(VertexInfluenceListClass), (float *) vd->GetTangentAddr(boneID),
					sizeof(VertexInfluenceListClass), (float *) vd->GetOPointAddr(boneID));
			}
		}
		else if ( (vd->WeightCount() == 1) && (vd->GetClosestBone() != -1))
		{
			testID = boneID;
			bmd->pSE->SetPointData(i, 1,
				sizeof(VertexInfluenceListClass), &boneID,
				sizeof(VertexInfluenceListClass), &val,
				sizeof(VertexInfluenceListClass), &id,
				sizeof(VertexInfluenceListClass), &id,
				sizeof(VertexInfluenceListClass), &fzero,
				sizeof(VertexInfluenceListClass), (float *)&tp,
				sizeof(VertexInfluenceListClass), (float *)&op);
		}

		else if(vd->WeightCount() > 0)
		{
			testID = vd->GetBoneIndex(0);
			bmd->pSE->SetPointData(i, vd->WeightCount(),
				sizeof(VertexInfluenceListClass), vd->GetBoneIndexAddr(0),
				sizeof(VertexInfluenceListClass), vd->GetNormalizedWeightAddr(0),
				sizeof(VertexInfluenceListClass), vd->GetCurveIDAddr(0),
				sizeof(VertexInfluenceListClass), vd->GetSegIDAddr(0),
				sizeof(VertexInfluenceListClass), vd->GetCurveUAddr(0),
				sizeof(VertexInfluenceListClass), (float *) vd->GetTangentAddr(0),
				sizeof(VertexInfluenceListClass), (float *) vd->GetOPointAddr(0));
		}
		else
			bmd->pSE->SetPointData(i, vd->WeightCount(),
			sizeof(VertexInfluenceListClass), NULL,
			sizeof(VertexInfluenceListClass), NULL,
			sizeof(VertexInfluenceListClass), NULL,
			sizeof(VertexInfluenceListClass), NULL,
			sizeof(VertexInfluenceListClass), NULL,
			sizeof(VertexInfluenceListClass), NULL,
			sizeof(VertexInfluenceListClass), NULL);

		if ((testID >= 0) && (testID < Cluster->BoneData.Count()))
			bmd->pSE->MapPoint(i,p,pout);
		else
		{
			DbgAssert(0);
		}
	}
	else
		pout = Map2(i,p);

	if	( (!Cluster->fastUpdate) || (Cluster->inRender) )
	{
		if (bmd->gizmoData.Count() > 0)
		{
			LocalGizmoData** gizmoPtr = bmd->gizmoData.Addr(0);
			for( int j = 0 ; j < bmd->gizmoData.Count() ; j++)
			{
				int id = gizmoPtr[j]->whichGizmo;
				if (id < Cluster->gizmoDefListCount/*gizmoCount*/)
				{
					GizmoClass *gizmo = Cluster->gizmoDefList[id];//(GizmoClass *)ref;
					if ((gizmo) && (gizmo->IsEnabled()))
					{
						if  (gizmo->IsVolumeBased() && gizmo->IsInVolume(initialP,bmd->BaseTM))
						{
							if (gizmo)
							{
								if (Cluster->backTransform) pout = pout * bmd->gizmoPutBackDoubleOffset;
								pout = gizmo->DeformPoint(t,i,initialP,pout,bmd->BaseTM);
								if (Cluster->backTransform) pout = pout * bmd->gizmoRemoveDoubleOffset;
							}
						}
						else
						{
							BOOL inList = FALSE;
							if (Cluster->fastGizmo)
								inList = gizmoPtr[j]->IsInDeformingList(i);
							else inList = gizmoPtr[j]->IsAffected(i);

							if (!gizmo->IsVolumeBased() && inList)
							{
								if (gizmo)
								{
									if (Cluster->backTransform) pout = pout * bmd->gizmoPutBackDoubleOffset;
									pout = gizmo->DeformPoint(t,i,initialP,pout,bmd->BaseTM);
									if (Cluster->backTransform) pout = pout * bmd->gizmoRemoveDoubleOffset;
								}
							}
						}
					}
				}
			}
		}
	}

	vd->LocalPosPostDeform = pout;

	return pout;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Skin Callback implementation
///////////////////////////////////////////////////////////////////////////////////////////////////////

ULONG CSkinCallback::AddRef()
{
	return ++m_cRef;
}

ULONG CSkinCallback::Release()
{
	if(--m_cRef != 0)
		return m_cRef;
	delete this;
	return 0;
}

HRESULT CSkinCallback::QueryInterface(REFIID riid, void** ppv)
{
	if(riid == IID_IUnknown)
	{
		*ppv = (IUnknown*)this;
	}
	else if(riid == IID__ISkinEngineEvents)
	{
		*ppv = (_ISkinEngineEvents*)this;
	}
	else
	{
		*ppv = NULL;
		return E_NOINTERFACE;
	}
	AddRef();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CSkinCallback::GetInterpCurvePiece3D( int BoneId,int CurveId,int SegId,float distance,float __RPC_FAR *pPoint)
{
	Point3 *pp = (Point3 *) pPoint;
	ObjectState os = bmd->mod->BoneData[BoneId].Node->EvalWorldState(bmd->CurrentTime);
	ShapeObject *pathOb = (ShapeObject*)os.obj;

	if (pathOb->NumberOfVertices(bmd->CurrentTime) > 0)
	{
		if ( (bmd->mod->splineList[BoneId]) && (SegId < bmd->mod->splineList[BoneId]->Segments()))
		{
			*pp = bmd->mod->splineList[BoneId]->InterpBezier3D(SegId, distance);
		}
		else *pp = pathOb->InterpPiece3D(bmd->CurrentTime, CurveId, SegId, distance );
		return S_OK;
	}
	else return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CSkinCallback::GetTangentPiece3D(int BoneId,int CurveId,int SegId,float distance,float __RPC_FAR *pPoint)
{
	Point3 *pp = (Point3 *) pPoint;
	ObjectState os = bmd->mod->BoneData[BoneId].Node->EvalWorldState(bmd->CurrentTime);
	ShapeObject *pathOb = (ShapeObject*)os.obj;
	if (pathOb->NumberOfVertices(bmd->CurrentTime) > 0)
	{
		if ((bmd->mod->splineList[BoneId])  && (SegId < bmd->mod->splineList[BoneId]->Segments()))
		{
			*pp = bmd->mod->splineList[BoneId]->TangentBezier3D(SegId, distance);
		}
		else *pp = pathOb->TangentPiece3D(bmd->CurrentTime, CurveId, SegId, distance );
		return S_OK;
	}
	else return S_FALSE;
}

//--- Named selection sets -----------------------------------------

int BonesDefMod::FindSelSetIndex(int index)
{
	int ct = 0;
	for (int i =0; i < namedSel.Count(); i++)
	{
		if (namedSel[i])
		{
			if (ct == index) return ct;
			ct++;
		}
	}
	return 0;
}

int BonesDefMod::FindSet(TSTR &setName)
{
	for (int i=0; i<namedSel.Count(); i++)
	{
		if ((namedSel[i]) && (setName == *namedSel[i])) return i;
	}
	return -1;
}

DWORD BonesDefMod::AddSet(TSTR &setName)
{
	DWORD id = 0;
	TSTR *name = new TSTR(setName);
	for (int i = 0; i < namedSel.Count(); i ++)
	{
		if (namedSel[i] == NULL)
		{
			namedSel[i] = name;
			return i;
		}
	}
	namedSel.Append(1,&name);
	return namedSel.Count()-1;
}

void BonesDefMod::RemoveSet(TSTR &setName)
{
	int i = FindSet(setName);
	if (i<0) return;
	delete namedSel[i];
	namedSel[i] = NULL;
}

void BonesDefMod::ClearSetNames()
{
	for (int j=0; j<namedSel.Count(); j++)
	{
		delete namedSel[j];
		namedSel[j] = NULL;
	}
}

void BonesDefMod::ActivateSubSelSet(TSTR &setName)
{
	ModContextList mcList;
	INodeTab nodes;
	int index = FindSet (setName);
	if (index<0 || !ip)
		return;

	ip->GetModContexts(mcList,nodes);
	TimeValue t = GetCOREInterface()->GetTime();

	for (int i = 0; i < mcList.Count(); i++)
	{
		BoneModData *bmd = (BoneModData*)mcList[0]->localData;
		if (!bmd)
			continue;

		bmd->selected.ClearAll();

		BitArray* list = bmd->namedSelSets[index];
		if (list)
		{
			for (int j = 0; j < list->GetSize(); j++)
			{
				if ( (*list)[j] )
				{
					if ((j <0) || (j >= bmd->selected.GetSize()))
					{
						// do nothing
					}
					else
					{
						bmd->selected.Set(j,TRUE);
					}
				}
			}
		}

		INode* node = nodes[0];
		if (node)
		{
			node->InvalidateRect(t);
		}
	}
	GetCOREInterface()->RedrawViews(t);

	PaintAttribList();
}

void BonesDefMod::NewSetFromCurSel(TSTR &setName) {
	DWORD id = -1;
	int index = FindSet(setName);
	if (index<0) id = AddSet(setName);
	else id = index;

	ModContextList mcList;
	INodeTab nodes;
	ip->GetModContexts(mcList,nodes);

	for (int i = 0; i < mcList.Count(); i++)
	{
		BoneModData *bmd = (BoneModData*)mcList[0]->localData;
		if (!bmd) continue;

		BitArray *set = new BitArray();

		int s = bmd->selected.GetSize();
		set->SetSize(s);
		*set = bmd->selected;

		int ct = bmd->namedSelSets.Count();
		if (id >= ct)
		{
			bmd->namedSelSets.SetCount(id+1);
			for (int j = ct; j < bmd->namedSelSets.Count(); j++)
				bmd->namedSelSets[j] = NULL;
		}

		if (bmd->namedSelSets[id] != NULL)
			delete bmd->namedSelSets[id];
		bmd->namedSelSets[id] = set;
	}
}

void BonesDefMod::RemoveSubSelSet(TSTR &setName) {
	int index = FindSet (setName);
	if (index<0 || !ip) return;

	ModContextList mcList;
	INodeTab nodes;
	ip->GetModContexts(mcList,nodes);

	for (int i = 0; i < mcList.Count(); i++)
	{
		BoneModData *bmd = (BoneModData*)mcList[0]->localData;
		if (!bmd) continue;

		if ( (index >= 0) && (index < bmd->namedSelSets.Count()))
		{
			if (bmd->namedSelSets[index])
			{
				delete bmd->namedSelSets[index];
				bmd->namedSelSets[index] = NULL;
			}
		}
	}

	RemoveSet(setName);
	nodes.DisposeTemporary();
}

void BonesDefMod::SetupNamedSelDropDown() {
	ip->ClearSubObjectNamedSelSets();
	for (int i=0; i<namedSel.Count(); i++)
	{
		if (namedSel[i]) ip->AppendSubObjectNamedSelSet(*namedSel[i]);
	}
}

int BonesDefMod::NumNamedSelSets() {
	int ct = 0;
	for (int i =0; i < namedSel.Count(); i++)
	{
		if (namedSel[i])
		{
			ct++;
		}
	}
	return ct;
}

TSTR BonesDefMod::GetNamedSelSetName(int i) {
	int index = 0;
	index = FindSelSetIndex(i);
	return *namedSel[index];
}

void BonesDefMod::SetNamedSelSetName(int i,TSTR &newName) {
	int index = 0;
	index = FindSelSetIndex(i);

	*namedSel[index] = newName;
}

void BonesDefMod::NewSetByOperator(TSTR &newName,Tab<int> &sets,int op) {
	DWORD id = -1;
	int index = FindSet(newName);
	if (index<0) id = AddSet(newName);
	else id = index;

	BOOL delSet = TRUE;

	ModContextList mcList;
	INodeTab nodes;
	ip->GetModContexts(mcList,nodes);

	for (int i = 0; i < mcList.Count(); i++) {
		BoneModData *bmd = (BoneModData*)mcList[0]->localData;
		if (!bmd) continue;

		BitArray *bits = new BitArray();

		int index = FindSelSetIndex(sets[0]);
		*bits = *bmd->namedSelSets[index];

		for (int i=1; i<sets.Count(); i++) {
			index = FindSelSetIndex(sets[i]);
			if (bmd->namedSelSets[index])
			{
				switch (op) {
				case NEWSET_MERGE:
					*bits |= *bmd->namedSelSets[index];
					break;

				case NEWSET_INTERSECTION:
					*bits &= *bmd->namedSelSets[index];
					break;

				case NEWSET_SUBTRACT:
					*bits &= ~(*bmd->namedSelSets[index]);
					break;
				}
			}

			int ct = bmd->namedSelSets.Count();
			if (id >= ct)
			{
				bmd->namedSelSets.SetCount(id+1);
				for (int j = ct; j < bmd->namedSelSets.Count(); j++)
					bmd->namedSelSets[j] = NULL;
			}

			if (bmd->namedSelSets[id] != NULL)
				delete bmd->namedSelSets[id];
			bmd->namedSelSets[id] = bits;
		}
		if (bits->NumberSet()) delSet = FALSE;
	}
	if (delSet) RemoveSubSelSet(newName);
}

void BonesDefMod::NotifyPreDeleteNode(void* parm, NotifyInfo* arg)
{
	BonesDefMod* mod = (BonesDefMod*)parm;
	if(mod == NULL) return;
	mod->bindNode = NULL;
}

void BonesDefMod::NotifyPreSave(void* parm, NotifyInfo* arg)
{
	BonesDefMod* mod = (BonesDefMod*)parm;
	if(mod == NULL) return;
	//put back to normal

	if ((mod->ip)  && (mod->ip->GetSubObjectLevel() == 1) &&  (mod->shadeVC) )
	{
		mod->CollectVCData(mod->vcCurDataList);
		mod->RestoreVCMode();
	}
}

void BonesDefMod::NotifyPostSave(void* parm, NotifyInfo* arg)
{
	BonesDefMod* mod = (BonesDefMod*)parm;
	if(mod == NULL) return;
	//put back vc
	if ((mod->ip) &&  (mod->shadeVC) && (mod->ip->GetSubObjectLevel() == 1) )
	{
		mod->SetVCData(mod->vcCurDataList);
	}
}

BOOL BonesDefMod::AssignController(Animatable *control,int subAnim)
{
	int refID = -1;
	refID = SubNumToRefNum(subAnim);

	if (refID >= 0)
	{
		ReplaceReference(refID, (Control*)control);
		NotifyDependents(FOREVER,PART_GEOM,REFMSG_CHANGE);
		NotifyDependents(FOREVER,0,REFMSG_SUBANIM_STRUCTURE_CHANGED);
		return TRUE;
	}
	else return FALSE;
}

void BonesDefMod::GetVertexSelection(INode *skinNode, BitArray &sel)
{
	BoneModData *bmd = GetBMD(skinNode);
	if (bmd!= NULL)
	{
		sel.SetSize(bmd->selected.GetSize());
		sel = bmd->selected;
	}
}

void BonesDefMod::SetVertexSelection(INode *skinNode, BitArray &sel)
{
	BoneModData* bmd = GetBMD(skinNode);
	if ((bmd != NULL) && (sel.GetSize() == bmd->selected.GetSize()))
	{
		bmd->selected = sel;
		TimeValue t = GetCOREInterface()->GetTime();
		skinNode->InvalidateRect(t);
		GetCOREInterface()->RedrawViews(t);
	}
}

void BonesDefMod::UnlockVerts()
{
	ModContextList mcList;
	INodeTab nodes;

	GetCOREInterface()->GetModContexts(mcList,nodes);
	for (int i = 0; i < mcList.Count(); i++)
	{
		BoneModData *bmd = (BoneModData*)mcList[i]->localData;
		if (bmd) bmd->unlockVerts = TRUE;
	}
}

void BonesDefMod::SelectBone(int boneIndex)
{
	if (ip)
	{
		ModContextList mcList;
		INodeTab nodes;
		ip->GetModContexts(mcList,nodes);
		int objects = mcList.Count();

		theHold.Begin();
		for ( int i = 0; i < objects; i++ )
		{
			BoneModData *bmd = (BoneModData*)mcList[i]->localData;
			theHold.Put(new SelectionRestore(this,bmd));
		}
		if (IsValidBoneIndex(boneIndex))
		{
			TSTR holdMessage;
			holdMessage.printf(_T("%s %s"),GetString(IDS_PW_SELECT),BoneData[boneIndex].Node->GetName());
			theHold.Accept(holdMessage);
		}
		else theHold.Accept(GetString(IDS_PW_SELECT));
	}

	if (ip)
	{
		SelectBoneByBoneIndex(boneIndex);
	}

	if (IsValidBoneIndex(boneIndex))
		ModeBoneIndex = boneIndex;
	if (ip)
		UpdatePropInterface();
	updateP = TRUE;

	NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);

	GetCOREInterface()->RedrawViews(GetCOREInterface()->GetTime());
}

void BonesDefMod::SelectChild()
{
	//get our current bone
	if (!IsValidBoneIndex(ModeBoneIndex))
		return;

	Tab<INode*> currentNodes;
	INode *currentNode = BoneData[ModeBoneIndex].Node;
	currentNodes.Append(1,&currentNode,100);

	BOOL done = FALSE;
	//while not done
	while (!done)
	{
		//get the all the children
		Tab<INode*> currentChildren;
		for (int i = 0; i < currentNodes.Count(); i++)
		{
			INode *currentNode = currentNodes[i];
			if (currentNode)
			{
				int numberOfChildren = currentNode->NumberOfChildren();
				for (int boneid = 0; boneid < numberOfChildren; boneid++)
				{
					INode *childNode = currentNode->GetChildNode(boneid);
					currentChildren.Append(1,&childNode,100);
				}
			}
		}
		//if no children we are done
		if (currentChildren.Count() == 0) done = TRUE;
		//check list to see if we have a match if so we are done
		for (int i = 0; i < currentChildren.Count(); i++)
		{
			for (int boneid = 0; boneid < BoneData.Count(); boneid++)
			{
				if (BoneData[boneid].Node == currentChildren[i])
				{
					if (BoneData[boneid].Node != NULL)
					{
						SelectBone(boneid);
						done = TRUE;
						i = currentChildren.Count();
						boneid = BoneData.Count();
					}
				}
			}
		}
		currentNodes = currentChildren;
	}
}
void BonesDefMod::SelectParent()
{
	//get our current bone
	if (!IsValidBoneIndex(ModeBoneIndex))
		return;

	INode *currentNode = BoneData[ModeBoneIndex].Node;

	BOOL done = FALSE;
	//while not done
	while (!done)
	{
		INode *parentNode = currentNode->GetParentNode();
		//if no children we are done
		if (parentNode == NULL) done = TRUE;
		else
		{
			//check list to see if we have a match if so we are done
			for (int boneid = 0; boneid < BoneData.Count(); boneid++)
			{
				if (BoneData[boneid].Node == parentNode)
				{
					SelectBone(boneid);
					done = TRUE;
					break;
				}
			}
			currentNode = parentNode;
		}
	}
}

void BonesDefMod::SelectSibling(BOOL next)
{
	//get our current bone
	if (!IsValidBoneIndex(ModeBoneIndex))
		return;

	INode *currentNode = BoneData[ModeBoneIndex].Node;

	Tab<INode*> siblings;
	int depth = 1;
	//find our first branch

	INode *parentNode = currentNode->GetParentNode();
	while (parentNode->NumberOfChildren() <= 1)
	{
		parentNode = parentNode->GetParentNode();
		depth++;
	}
	//gather up all the children
	Tab<INode*> currentNodes;
	currentNodes.Append(1,&parentNode);
	for (int i = 0; i < depth; i++)
	{
		siblings.SetCount(0);
		for (int j = 0; j < currentNodes.Count(); j++)
		{
			INode *currentNode = currentNodes[j];
			int numberOfChildren = currentNode->NumberOfChildren();
			for (int k = 0; k < numberOfChildren; k++)
			{
				INode *childNode = currentNode->GetChildNode(k);
				siblings.Append(1,&childNode,100);
			}
		}
		currentNodes = siblings;
	}

	INode *currentBone = BoneData[ModeBoneIndex].Node;

	int dir = 1;
	if (!next) dir = -1;

	int startIndex = -1;
	for (int j = 0; j < siblings.Count(); j++)
	{
		if (siblings[j] == currentBone)
		{
			startIndex = j;
			break;
		}
	}

	int currentIndex = startIndex;
	if (startIndex != -1)
	{
		BOOL done = FALSE;
		while (!done)
		{
			currentIndex += dir;
			if (currentIndex < 0) currentIndex = siblings.Count() -1;
			if (currentIndex >=  siblings.Count()) currentIndex = 0;
			INode *targetNode = siblings[currentIndex];

			for (int boneid = 0; boneid < BoneData.Count(); boneid++)
			{
				if (BoneData[boneid].Node == targetNode)
				{
					if (targetNode != NULL)
					{
						SelectBone(boneid);
						done = TRUE;
						break;
					}
				}
			}
		}
	}
}

void BonesDefNodeDisplayCallback::AddNodeCallbackBox(TimeValue t, INode *node, ViewExp *vpt, Box3& box,Object *pObj)
{
	if ( ! vpt || ! vpt->IsAlive() )
	{
		// why are we here
		DbgAssert(!_T("Invalid viewport!"));
		return;
	}

	if (mod && mod->ip && (mod->ip->GetSubObjectLevel() == 1))
	{
		if (mod->ip->GetExtendedDisplayMode()& EXT_DISP_ZOOMSEL_EXT)
		{
			MyEnumProc dep;
			mod->DoEnumDependents(&dep);
			//this puts back the original state of the node vc mods and shade state
			int nodeCount = 0;
			Box3 checkBox;
			checkBox.Init();
			BOOL newBounds = FALSE;

			for (int  i = 0; i < dep.Nodes.Count(); i++)
			{
				BoneModData *bmd = mod->GetBMD(dep.Nodes[i]);
				if (bmd)
				{
					if (bmd->selected.NumberSet() > 0)
					{
						if (dep.Nodes[i])
						{
							Matrix3 tm = dep.Nodes[i]->GetObjectTM(t);
							for (int i = 0; i < bmd->VertexData.Count(); i++)
							{
								if (bmd->selected[i])
								{
									checkBox += bmd->VertexData[i]->LocalPosPostDeform * tm;
								}
							}
							newBounds= TRUE;
						}
					}
				}
			}
			if (!newBounds)
			{
				Point3 pt1, pt2;

				float l = 0.0f;
				if (mod->IsValidBoneIndex(mod->ModeBoneIndex))
				{
					BoneDataClass &boneData = mod->BoneData[mod->ModeBoneIndex];
					box.Init();
					newBounds = TRUE;

					Interval valid;
					Matrix3 ntm = boneData.Node->GetObjTMBeforeWSM(t,&valid);

					Point3 pta;
					Interval v;
					boneData.EndPoint1Control->GetValue(t,&pta,v);

					checkBox += pta* ntm;
					boneData.EndPoint2Control->GetValue(t,&pta,v);
					checkBox += pta* ntm;
					for (int k = 0; k < boneData.CrossSectionList.Count();k++)
					{
						float outer;
						Interval v;
						boneData.CrossSectionList[k].OuterControl->GetValue(t,&outer,v);

						if (outer > l) l = outer;
					}
					checkBox.EnlargeBy(l+2.0f);  // this is a fudge since I am using large tick boxes
				}
			}
			if (newBounds)
				box = checkBox;
		}
	}
}

void BonesDefMod::SelectElement(BoneModData *bmd, BOOL alt)
{
	if (bmd->mesh)
	{
		//get our edge list
		AdjEdgeList *edgeList;

		edgeList = new AdjEdgeList(*bmd->mesh);
		int numberSelected = -1;

		//while not done
		while (numberSelected != bmd->selected.NumberSet())
		{
			numberSelected = bmd->selected.NumberSet();
			//find all edges that have one selected and not
			for (int i = 0; i < edgeList->edges.Count(); i++)
			{
				//select those
				int a = edgeList->edges[i].v[0];
				int b = edgeList->edges[i].v[1];
				if (bmd->selected[a] && (!bmd->selected[b]))
				{
					if (alt)
						bmd->selected.Set(a,FALSE);
					else bmd->selected.Set(b,TRUE);
				}
				else if (bmd->selected[b] && (!bmd->selected[a]))
				{
					if	(alt)
						bmd->selected.Set(b,FALSE);
					else bmd->selected.Set(a,TRUE);
				}
			}
		}

		delete edgeList;
	}
	if (bmd->mnMesh)
	{
		int numberSelected = -1;

		//while not done
		while (numberSelected != bmd->selected.NumberSet())
		{
			numberSelected = bmd->selected.NumberSet();
			//find all edges that have one selected and not
			for (int i = 0; i < bmd->mnMesh->nume; i++)
			{
				//select those
				int a = bmd->mnMesh->e[i].v1;
				int b = bmd->mnMesh->e[i].v2;
				if (bmd->selected[a] && (!bmd->selected[b]))
				{
					if (alt)
						bmd->selected.Set(a,FALSE);
					else bmd->selected.Set(b,TRUE);
				}
				else if (bmd->selected[b] && (!bmd->selected[a]))
				{
					if	(alt)
						bmd->selected.Set(b,FALSE);
					else bmd->selected.Set(a,TRUE);
				}
			}
		}
	}
	if (bmd->patch)
	{
		int numberSelected = -1;

		//transfer handle selection to knot
		for (int i = 0; i < bmd->vecOwnerList.Count(); i++)
		{
			int knotIndex =  bmd->vecOwnerList[i];
			if (knotIndex != -1)
			{
				if (alt)
				{
					if ((knotIndex < bmd->selected.GetSize()) && (!bmd->selected[i]))
					{
						bmd->selected.Set(knotIndex,FALSE);
					}
				}
				else
				{
					if ((knotIndex < bmd->selected.GetSize()) && bmd->selected[i])
					{
						bmd->selected.Set(knotIndex,TRUE);
					}
				}
			}
		}
		//while not done
		while (numberSelected != bmd->selected.NumberSet())
		{
			numberSelected = bmd->selected.NumberSet();
			//find all edges that have one selected and not
			for (int i = 0; i < bmd->patch->numEdges; i++)
			{
				//select those
				int a = bmd->patch->edges[i].v1;
				int b = bmd->patch->edges[i].v2;
				if (bmd->selected[a] && (!bmd->selected[b]))
				{
					if (alt)
						bmd->selected.Set(a,FALSE);
					else bmd->selected.Set(b,TRUE);
				}
				else if (bmd->selected[b] && (!bmd->selected[a]))
				{
					if	(alt)
						bmd->selected.Set(b,FALSE);
					else bmd->selected.Set(a,TRUE);
				}
			}
		}

		for (int i = 0; i < bmd->vecOwnerList.Count(); i++)
		{
			int knotIndex =  bmd->vecOwnerList[i];
			if (knotIndex != -1)
			{
				if (alt)
				{
					if ((knotIndex < bmd->selected.GetSize()) && (!bmd->selected[knotIndex]))
					{
						bmd->selected.Set(i,FALSE);
					}
				}
				else
				{
					if ((knotIndex < bmd->selected.GetSize()) && bmd->selected[knotIndex])
					{
						bmd->selected.Set(i,TRUE);
					}
				}
			}
		}
	}
}

void BonesDefMod::RescaleWorldUnits(float f)
{
	if (TestAFlag(A_WORK1))
		return;
	SetAFlag(A_WORK1);

	Point3 scale(f,f,f);
	Point3 p(0.0f,0.0f,0.0f);
	for (int i = 0; i < BoneData.Count(); i++)
	{
		BoneDataClass &boneData = BoneData[i];
		if (boneData.Node)
		{
			Matrix3 tm = boneData.tm;
			tm = Inverse(tm);
			p = tm.GetTrans();
			p *= f;
			tm.SetTrans(p);
			tm = Inverse(tm);
			boneData.tm = tm;

			p = boneData.InitStretchTM.GetTrans();
			p *= f;
			boneData.InitStretchTM.SetTrans(p);

			p = boneData.InitNodeTM.GetTrans();
			p *= f;
			boneData.InitNodeTM.SetTrans(p);

			if (boneData.EndPoint1Control)
				boneData.EndPoint1Control->RescaleWorldUnits(f);
			if (boneData.EndPoint2Control)
				boneData.EndPoint2Control->RescaleWorldUnits(f);
			for (int crossID = 0; crossID < boneData.CrossSectionList.Count(); crossID++)
			{
				if (boneData.CrossSectionList[crossID].InnerControl)
					boneData.CrossSectionList[crossID].InnerControl->RescaleWorldUnits(f);
				if (boneData.CrossSectionList[crossID].OuterControl)
					boneData.CrossSectionList[crossID].OuterControl->RescaleWorldUnits(f);
			}
		}
	}

	MyEnumProc dep;
	DoEnumDependents(&dep);
	//this puts back the original state of the node vc mods and shade state
	int nodeCount = 0;

	for (int  i = 0; i < dep.Nodes.Count(); i++)
	{
		if (dep.Nodes[i])
		{
			BoneModData *bmd = GetBMD(dep.Nodes[i]);
			if (bmd)
			{
				Matrix3 itm = bmd->InverseBaseTM;
				itm = Inverse(itm);
				p = itm.GetTrans();
				p *= f;
				itm.SetTrans(p);
				itm = Inverse(itm);
				bmd->InverseBaseTM = itm;

				p = bmd->BaseNodeTM.GetTrans();
				p *= f;
				bmd->BaseNodeTM.SetTrans(p);

				p = bmd->BaseTM.GetTrans();
				p *= f;
				bmd->BaseTM.SetTrans(p);

				p = bmd->baseNodeOffsetTM.GetTrans();
				p *= f;
				bmd->baseNodeOffsetTM.SetTrans(p);

				bmd->reevaluate = TRUE;
			}
		}
	}
}

void BonesDefMod::SelectBoneByBoneIndex(int boneIndex)
{
	if (boneIndex < 0)
		return;
	ModeBoneIndex = boneIndex;
	int rsel = ConvertBoneIndexToListID(boneIndex);
	mParameterListSelection = rsel;
	SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_SETCURSEL,rsel,0);
}
//Select a bone based on its index in the UI list box
void BonesDefMod::SelectBoneByListBoxIndex(int selIndex)
{
	if (selIndex < 0)
		return;
	ModeBoneIndex = ConvertListIDToBoneIndex(selIndex);
	mParameterListSelection = selIndex;
	SendMessage(GetDlgItem(hParam,IDC_LIST1), LB_SETCURSEL,selIndex,0);
}
