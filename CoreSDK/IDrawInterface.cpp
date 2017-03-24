#include "IDrawInterface.h"
#include "reflx.h"
#include "coresdkafx.h"

#define COLOR_DARK_RED		0xff8b1818
#define COLOR_DARK_GREEN	0xff117900
#define COLOR_DARK_BLUE		0xff000079

#define ROT_N0 1

static const bool V_TRUE = true;
static const bool V_FALSE = false;
static const Matrix DefMatrixTransform(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f);
static const Matrix3f DefMatrixScale(1.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 1.f);
static const Vector DefVec(0.f, 0.f, 0.f);


BEGIN_AUTO_CLASS(IDrawInterface)
	new oes::rflex::PropertyBOOL("bVisible", (BYTE*)&((IDrawInterface*)NULL)->m_bVisible - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_TRUE),
	new oes::rflex::PropertyBOOL("RenderDebug",	(BYTE*)&((IDrawInterface*)NULL)->m_bRenderDebug - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &V_FALSE),
    new oes::rflex::TProperty<Matrix, IDrawInterface>("LTM", "IDrawInterface", "Value",
    [&](const void *ptr, const char *v)
    {
        void *nc_ptr = const_cast<void*>(ptr);
        IDrawInterface *act = reinterpret_cast<IDrawInterface*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));
        CStringUtility<float> ParseString(v, ";");
        assert(ParseString.m_vector.size() == 16);
       
        Matrix &Dest = act->m_LTM_;
        Dest._11 = ParseString.m_vector[0];
        Dest._12 = ParseString.m_vector[1];
        Dest._13 = ParseString.m_vector[2];
        Dest._14 = ParseString.m_vector[3];
        Dest._21 = ParseString.m_vector[4];
        Dest._22 = ParseString.m_vector[5];
        Dest._23 = ParseString.m_vector[6];
        Dest._24 = ParseString.m_vector[7];
        Dest._31 = ParseString.m_vector[8];
        Dest._32 = ParseString.m_vector[9];
        Dest._33 = ParseString.m_vector[10];
        Dest._34 = ParseString.m_vector[11];
        Dest._41 = ParseString.m_vector[12];
        Dest._42 = ParseString.m_vector[13];
        Dest._43 = ParseString.m_vector[14];
        Dest._44 = ParseString.m_vector[15];

    },
    [&](const void *ptr, const char **out)
    {
    }),
    //new oes::rflex::PropertyMatrix3x3("STM", (BYTE*)&((IDrawInterface*)NULL)->m_STM_ - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_ONLY, CTRL_MATRIX9, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &DefMatrixScale),
        new oes::rflex::TProperty<Matrix, IDrawInterface>("STM", "IDrawInterface", "Value",
            [&](const void *ptr, const char *v)
            {
                void *nc_ptr = const_cast<void*>(ptr);
                IDrawInterface *act = reinterpret_cast<IDrawInterface*>(reinterpret_cast<CObjectAbstract*>(nc_ptr));
                CStringUtility<float> ParseString(v, ";");
                assert(ParseString.m_vector.size() == 9);

                Matrix3f &Dest = act->m_STM_;
                Dest._11 = ParseString.m_vector[0];
                Dest._12 = ParseString.m_vector[1];
                Dest._13 = ParseString.m_vector[2];
                Dest._21 = ParseString.m_vector[3];
                Dest._22 = ParseString.m_vector[4];
                Dest._23 = ParseString.m_vector[5];
                Dest._31 = ParseString.m_vector[6];
                Dest._32 = ParseString.m_vector[7];
                Dest._33 = ParseString.m_vector[8];

            },
            [&](const void *ptr, const char **out)
            {
            }),
    //new oes::rflex::PropertyVector("Rotator", (BYTE*)&((IDrawInterface*)NULL)->m_YawPitchRoll - (BYTE*)&((IDrawInterface*)NULL)->___startObjectMarkerIDrawInterface, "IDrawInterface", "Value", READ_ONLY, CTRL_VECTOR, SERIALIZABLE, NON_COMMON_PROP, EXT_PROP, 0, 0, &DefVec),
END_AUTO_CLASS(IDrawInterface)
CLASS_INSTANCE_INTERFACE(IDrawInterface);

//----------------------------------------------------------------------------------------------
IDrawInterface::IDrawInterface(const CObjectAbstract *pParent)
: m_pNode(0)
, m_VisualStates(AS_NONE)
, m_bVisible(V_TRUE)
, m_bRenderDebug(V_FALSE)
, m_Bounds(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX)
, m_CompositeBounds(V_FLT_MAX, V_FLT_MAX, V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX, -V_FLT_MAX)
, m_LTM_(DefMatrixTransform)
, m_STM_(DefMatrixScale)
{
	if (pParent)
	{
 		m_pCoreSDK = (CCoreSDK*)pParent->GetUserData();
	}
}

//----------------------------------------------------------------------------------------------
IDrawInterface::IDrawInterface(const IDrawInterface &Source)
: m_pNode(0)
, m_VisualStates(AS_NONE)
{
	if (this != &Source)
	{
		m_pCoreSDK = Source.m_pCoreSDK;
		m_bVisible = Source.m_bVisible;
		m_bRenderDebug = Source.m_bRenderDebug;
		m_LTM_ = Source.m_LTM_;
		m_STM_ = Source.m_STM_;
		m_Bounds = Source.m_Bounds;
		m_CompositeBounds = Source.m_CompositeBounds;
	}
}

//----------------------------------------------------------------------------------------------
IDrawInterface::~IDrawInterface()
{
	UnregisterDrawInterface();
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::RegisterDrawInterface(const CActor *pSrc, int SrcParent /*= 1*/)
{
	const_cast<CActor*>(pSrc)->RegisterInterfaceImpl(this);

	if (m_pCoreSDK)
    {
        core_sdk_api::CViewportManager *manager = m_pCoreSDK->GetViewportManager();

        assert(manager);

		m_pNode = manager->Register(this, pSrc, (SrcParent == 1) ? pSrc->GetParent() : nullptr );
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::UnregisterDrawInterface()
{
	if (m_pCoreSDK && m_pNode)
	{
        core_sdk_api::CViewportManager *manager = m_pCoreSDK->GetViewportManager();

        assert(manager);

        manager->Unregister(m_pNode);
	}
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::DoVisibilityTest_() const
{
	return true;
}

//----------------------------------------------------------------------------------------------
bool IDrawInterface::PrePropertyChangeIntf(const char *PropertyName)
{
	return true;
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::OnPropertyChangedIntf(const char *PropertyName)
{
	if (!strcmp(PropertyName, "Position") || 
		!strcmp(PropertyName, "PositionX") ||
		!strcmp(PropertyName, "PositionY") || 
		!strcmp(PropertyName, "PositionZ") ||
		!strcmp(PropertyName, "LTM") || 
		!strcmp(PropertyName, "STM"))
	{
		m_pCoreSDK->GetViewportManager()->RebuildTransform(const_cast<CActor*>(m_pNode->m_pKey));
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::Draw()
{
	DoDraw();

	if (m_pCoreSDK->IsAEditor() && !m_pCoreSDK->IsAGame())
	{
		DrawBounds();
		//DrawController();
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::DrawBounds() const
{
#ifndef FINAL_RELEASE
 	if (m_pCoreSDK->GetObjectBoundsVisible() && m_Bounds.IsValid())
 	{
 		m_pCoreSDK->GetRenderSDK()->DrawBounds(m_WorldMatrixTransform, m_Bounds, COLOR_BLUE);
 	}
 
// 	if (m_pCoreSDK->GetSparitalSubdivisionVisible() && CompositeBounds.IsValid())
// 	{
// 		Matrix I;
// 		GetRenderComponent()->DrawBounds(I, CompositeBounds, COLOR_GREEN);
// 	}
#endif//FINAL_RELEASE
}

//----------------------------------------------------------------------------------------------
/*bool IDrawInterface::ProcessPress(const MouseInputData &InputData)
{
	bool bResult = false;

	if (!m_pNode->m_pKey->GetEnabled() || !GetVisible()){
		return bResult;
	}*/

/*	if (!m_pNode->m_pValue->GetCompositeBounds().HitTest(InputData.MousePos)){
		return false;
	}
		
	switch (InputData.event)
	{
	case MOUSE_Pressed:
	case MOUSE_DoubleClick:
		{
			if (m_pNode->m_pKey->GetControlState() == ActorState_None)
			{
				if (!m_pNode->m_pValue->GetCompositeBounds().HitTest(InputData.MousePos)){
					return false;
				}

				if (m_pNode->m_pKey->HitTest(InputData.MousePos)) // top most widget hit test
				{
					unsigned int State = m_pNode->m_pKey->GetStates();
					m_pNode->m_pValue->SetStates(State |= AS_PRESSED);

					//SetControlState(ActorState_Locked, true);	// set mouse input lock
					m_pNode->m_pValue->GainFocus();							// set active state focused

					m_pNode->m_pValue->DoEventPressed(InputData); 
					bResult = true;
				}
			}
		}break;

		case MOUSE_Released:
		{
			// iterate to low level locked object
			if (GetControlState() == ActorState_Locked)
			{
				for (TVecActorChildRevIterator Iter = ChildNodes.rbegin(); Iter != ChildNodes.rend(); ++Iter)
				{
					if ((*Iter)->GetControlState() == ActorState_Locked && 
						(*Iter)->OnEventPressed(InputData)) 
					{
						return true;
					}
				}

				// here lowest locked object 
				if (DoEventReleased(InputData)) // process deactivate widget
				{
					unsigned int State = GetStates();
					SetStates(State &= ~AS_PRESSED);

					SetControlState(ActorState_None, true); // drop mouse input lock upper to root
					bResult = true;
				}
			}
		}break;
	};
	*/
/*	return bResult;
}*/

//----------------------------------------------------------------------------------------------
/*bool IDrawInterface::ProcessRelease(const MouseInputData &InputData)
{
	bool bResult = false;

	unsigned int State = m_pNode->m_pValue->GetVisualStates();

	if (State & AS_PRESSED)
	{
		CActor *pActor = const_cast<CActor*>(m_pNode->m_pKey);

		m_pCoreSDK->GetViewportManager()->SetFocus(this, false);

		const_cast<IDrawInterface*>(m_pNode->m_pValue)->SetVisualStates(State &= ~AS_PRESSED);
		
		pActor->DoEventReleased(InputData);

		bResult = true;
	}
	return bResult;
}*/

//----------------------------------------------------------------------------------------------
/*void IDrawInterface::SetFocus(bool bFlag)
{
	if (bFlag)
	{
		if (m_VisualStates & AS_FOCUSED){ // already focused
			return;
		}
		SetVisualStates(m_VisualStates |= AS_FOCUSED);
	}else
	{
		SetVisualStates(m_VisualStates &= ~AS_FOCUSED);
	}
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::SetMouseOver(bool bFlag )
{
	if (bFlag)
	{
		SetVisualStates(m_VisualStates |= AS_MOUSE_OVER);
	}else{
		SetVisualStates(m_VisualStates &= ~AS_MOUSE_OVER);
	}
}*/

//----------------------------------------------------------------------------------------------
void IDrawInterface::DoBuildWorldTransform_(const Matrix &WTM)
{
	m_WorldMatrixTransform = GetLTM_() * WTM * GetSTM_();

    m_Bounds.SetUnvalid(); // invalidate

    std::vector<oes::common_base::IRenderInterface*> &vecRenderEntities = const_cast<CActor*>(m_pNode->m_pKey)->m_VecRenderEntities;
	for (std::vector<oes::common_base::IRenderInterface*>::const_iterator iter = vecRenderEntities.begin(); iter != vecRenderEntities.end(); ++iter)
	{
		(*iter)->SetRWTM(m_WorldMatrixTransform);
	
		Bounds3f BBox = (*iter)->GetRBounds_();

		if (BBox.IsValid())
		{
            const unsigned int BBOX_POINTS = 8;

			Vector BoxPoints[BBOX_POINTS] =
			{ 
				/*Vector(BBox.bound_min.x, BBox.bound_min.y, BBox.bound_min.z),
				Vector(BBox.bound_min.x, BBox.bound_min.y, BBox.bound_max.z),
				Vector(BBox.bound_max.x, BBox.bound_min.y, BBox.bound_max.z),
				Vector(BBox.bound_max.x, BBox.bound_min.y, BBox.bound_min.z),

				Vector(BBox.bound_min.x, BBox.bound_max.y, BBox.bound_min.z),
				Vector(BBox.bound_min.x, BBox.bound_max.y, BBox.bound_max.z),
				Vector(BBox.bound_max.x, BBox.bound_max.y, BBox.bound_max.z),
				Vector(BBox.bound_max.x, BBox.bound_max.y, BBox.bound_min.z),*/

                // TODO make homogeneous with exporter tool
                Vector(BBox.bound_min.z, BBox.bound_min.y, BBox.bound_min.x),
                Vector(BBox.bound_max.z, BBox.bound_min.y, BBox.bound_min.x),
                Vector(BBox.bound_max.z, BBox.bound_min.y, BBox.bound_max.x),
                Vector(BBox.bound_min.z, BBox.bound_min.y, BBox.bound_max.x),

                Vector(BBox.bound_min.z, BBox.bound_max.y, BBox.bound_min.x),
                Vector(BBox.bound_max.z, BBox.bound_max.y, BBox.bound_min.x),
                Vector(BBox.bound_max.z, BBox.bound_max.y, BBox.bound_max.x),
                Vector(BBox.bound_min.z, BBox.bound_max.y, BBox.bound_max.x),
			}; 

			Matrix MT = m_WorldMatrixTransform;
			MT.t = Vector(0.f, 0.f, 0.f);

            Bounds3f tmpBound;
			for (int Index = 0; Index < BBOX_POINTS; ++Index)
			{
				Vector point = transform_coord(BoxPoints[Index], MT);
                
                if (Index == 0)
                {
                    tmpBound.SetBounds(point, point);
                    continue;
                }
                tmpBound.Add(point);
			}

            const Vector bound_min = m_WorldMatrixTransform.t + tmpBound.bound_min;
            const Vector bound_max = m_WorldMatrixTransform.t + tmpBound.bound_max;

            (*iter)->SetWBounds(Bounds3f(bound_min, bound_max));

            if (iter == vecRenderEntities.begin())
            {
                m_Bounds.SetBounds(bound_min, bound_max);
                continue;
            }
            else
            {
                m_Bounds.Add(bound_min);
                m_Bounds.Add(bound_max);
            }
		}
	}

    // invalidate composite bounds
    m_CompositeBounds = m_Bounds;
}

//----------------------------------------------------------------------------------------------
void IDrawInterface::DoBuildCompounds()
{

}

//----------------------------------------------------------------------------------------------
/** Transforms local space position in to global space point */
 Vector& IDrawInterface::LocalToGlobalTransform(Vector& OutGlobalPoint, const Vector& InLocalPoint) const
 {
	 transform_coord(OutGlobalPoint, InLocalPoint, m_WorldMatrixTransform);

	 return OutGlobalPoint;
 }

 //----------------------------------------------------------------------------------------------
 Vector& IDrawInterface::GlobalToLocalTransform(Vector& OutLocalPoint, const Vector& InGlobalPoint) const
 {
	 // build global space matrix
	 Matrix WTM = CalcWorldTransform();

	 Vector UnTransformedLocalPosition = InGlobalPoint - WTM.t;

	 // calc projection to local axis
	 Vector AxisX = WTM._row0;
	 Vector AxisY = WTM._row1;
	 Vector AxisZ = WTM._row2;

	 AxisX.normalize();
	 AxisY.normalize();
	 AxisZ.normalize();

	 Matrix IWTM;	
	 invert(IWTM, WTM);

	 OutLocalPoint = Vector(UnTransformedLocalPosition.Dot(AxisX) * IWTM._row0.Length(),
							UnTransformedLocalPosition.Dot(AxisY) * IWTM._row1.Length(),
							UnTransformedLocalPosition.Dot(AxisZ) * IWTM._row2.Length());
	 return OutLocalPoint;
 }

 //----------------------------------------------------------------------------------------------
 Matrix IDrawInterface::CalcWorldTransform() const
 {
	 Matrix OutMatrix;

	 std::vector<const IDrawInterface*> TmpActorList;

	 TNodeMap<class CActor, IDrawInterface> *IterActor = m_pNode->GetRootNode();

	 while (IterActor) // do not allow top level root processing
	 {				   // use it as origin for child nodes
		 TmpActorList.push_back(IterActor->m_pValue);
		 IterActor = IterActor->GetRootNode();
	 }

	 // build global space matrix
	 for (std::vector<const IDrawInterface*>::reverse_iterator Iter = TmpActorList.rbegin(); Iter != TmpActorList.rend(); ++Iter)
	 {
		 OutMatrix = OutMatrix * (*Iter)->GetPivot_();
	 }
	 return OutMatrix;
 }


 //----------------------------------------------------------------------------------------------
 bool IDrawInterface::TransformObjectAToObjectBLocalPos(CActor * ObjectA, const CActor * ObjectB, Vector& LocalBPos/* = Vector(0.f, 0.f, 0.f)*/, bool bKeepAOrientation /*= true*/)
 {
	 bool bResult = false;
	 /*
	 if (ObjectA && ObjectB)
	 {
		 Vector GlobalPoint;
		 ObjectB->LocalToGlobalTransform(GlobalPoint, LocalBPos);

		 Vector LocalPoint;
		 ObjectA->GlobalToLocalTransform(LocalPoint, GlobalPoint);
		 ObjectA->SetPosition(LocalPoint);

		 // common parent object
		 // 		CActor * ObjectC = NULL;
		 // 
		 // 		// try to find common root node
		 // 		CActor * ParentB = ObjectB->GetParent();
		 // 		while (ParentB && !ObjectC)
		 // 		{
		 // 			CActor * ParentA = const_cast<CActor*>(ObjectA);
		 // 			while (ParentA != NULL)
		 // 			{
		 // 				if (ParentB == ParentA)
		 // 				{
		 // 					ObjectC = ParentA;
		 // 					break;
		 // 				}
		 // 				ParentA = ParentA->GetParent();
		 // 			}
		 // 			ParentB = ParentB->GetParent();
		 // 		}
		 // 
		 // 		// common root node found
		 // 		if (ObjectC != NULL)
		 // 		{		
		 // 			std::vector<const CActor*> TmpActorListA, TmpActorListB;
		 // 
		 // 			// build B-way array 
		 // 			CActor * IterActor = const_cast<CActor*>(ObjectB);
		 // 			while (IterActor != NULL) 
		 // 			{
		 // 				TmpActorListB.push_back(IterActor);
		 // 				IterActor = IterActor->GetParent();
		 // 			}
		 // 
		 // 			// build A array
		 // 			IterActor = const_cast<CActor*>(ObjectA);
		 // 			while (IterActor != NULL) 
		 // 			{
		 // 				TmpActorListA.push_back(IterActor);
		 // 				IterActor = IterActor->GetParent();
		 // 			}
		 // 
		 // 			// build [Parent-ObjectB] space matrix
		 // 			Matrix CA_LTM, CB_LTM;
		 // 			for (std::vector<const CActor*>::reverse_iterator Iter = TmpActorListB.rbegin(); Iter != TmpActorListB.rend(); ++Iter)
		 // 			{
		 // 				CB_LTM = CB_LTM * (*Iter)->GetPivot();
		 // 			}
		 // 
		 // 			// build [Parent-ObjectA] local transform
		 // 			for (std::vector<const CActor*>::reverse_iterator Iter = TmpActorListA.rbegin(); Iter != TmpActorListA.rend(); ++Iter)
		 // 			{
		 // 				CA_LTM = CA_LTM * (*Iter)->GetPivot();
		 // 			}
		 // 
		 // 			// add local shift
		 //  			Matrix LTM_LBP = ObjectB->GetPivot();
		 //  			LTM_LBP.t = LocalBPos;
		 //  			CB_LTM = CB_LTM * LTM_LBP;
		 // 
		 //  			Vector V = CB_LTM.t - CA_LTM.t;
		 // 
		 // 			// calc projection to local axis
		 //  			Vector AxisX = CA_LTM._row0;
		 //  			Vector AxisY = CA_LTM._row1;
		 //  			Vector AxisZ = CA_LTM._row2;
		 //  
		 //  			AxisX.normalize();
		 //  			AxisY.normalize();
		 //  			AxisZ.normalize();
		 // 
		 // 			Vector LocalPoint = ObjectA->m_LTM.t + Vector(V.Dot(AxisX), V.Dot(AxisY), V.Dot(AxisZ));
		 //  			ObjectA->SetPosition(LocalPoint);
		 bResult = true;
		 //		}
	 }*/
	 return bResult;
 }

 //----------------------------------------------------------------------------------------------
 bool IDrawInterface::GetObjectAInLocalSpaceB(Matrix &outLTM, const IDrawInterface *pObjectA, const IDrawInterface *pObjectB)
 {
	 bool bResult = false;

	 if (pObjectA && pObjectB)
	 {
		 CCoreSDK *pApp =  pObjectA->GetCoreSDK();
		 assert(pApp);

		 // common parent object
		 CActor *pObjectC = nullptr;
		 CActor *pActorA = const_cast<CActor*>(pObjectA->m_pNode->m_pKey);
		 CActor *pActorB = const_cast<CActor*>(pObjectB->m_pNode->m_pKey);

		 // common root node found
		 if (CActor::FindCommonParent(&pObjectC, pActorA, pActorB))
		 {
			 std::vector<const CActor*> tmpActorListA, tmpActorListB;

			 // build B-way array 
			 CActor *pIterActor = const_cast<CActor*>(pActorB);
			 while (pIterActor != pObjectC) 
			 {
				 tmpActorListB.push_back(pIterActor);
				 pIterActor = pIterActor->GetParent();
			 }

			 // build A array
			 pIterActor = const_cast<CActor*>(pActorA);
			 while (pIterActor != pObjectC) 
			 {
				 tmpActorListA.push_back(pIterActor);
				 pIterActor = pIterActor->GetParent();
			 }

			 // build [Parent - ObjectB] space matrix
			 Matrix CA_LTM, CB_LTM; 
			 for (std::vector<const CActor*>::reverse_iterator Iter = tmpActorListB.rbegin(); Iter != tmpActorListB.rend(); ++Iter)
			 {
				 IDrawInterface *pIActor = pApp->GetViewportManager()->GetByActor(const_cast<CActor*>(*Iter));
				 if (pIActor){
					CB_LTM = CB_LTM * (pIActor->GetLTM_());
				 }
				 
			 }
			 // [Parent - ObjectA] local transform
			 for (std::vector<const CActor*>::reverse_iterator Iter = tmpActorListA.rbegin(); Iter != tmpActorListA.rend(); ++Iter)
			 {
				 IDrawInterface *pIActor = pApp->GetViewportManager()->GetByActor(const_cast<CActor*>(*Iter));
				 if (pIActor){
					CA_LTM = CA_LTM * (pIActor->GetLTM_());
				 }
			 }

			 Matrix ICA_LTM;	
			 outLTM = invert(ICA_LTM, CA_LTM) * CB_LTM;
			 bResult = true;
		 }
	 }
	 return bResult;
 }

 //----------------------------------------------------------------------------------------------
 /** Calculates object world matrix.*/
 Matrix IDrawInterface::GetParentWorldTransform(const CActor* Object)
 {
	 Matrix WTM; 
	/* std::vector<const CActor*> TmpActorList;

	 CActor * IterActor = Object->GetParent();
	 while (IterActor)
	 {
		 TmpActorList.push_back(IterActor);
		 IterActor = IterActor->GetParent();
	 }

	 // build global space matrix
	 
	 for (std::vector<const CActor*>::reverse_iterator Iter = TmpActorList.rbegin(); Iter != TmpActorList.rend(); ++Iter)
	 {
		 WTM = WTM * (*Iter)->GetPivot() * (*Iter)->GetSTM();
	 }*/
	 return WTM;
 }