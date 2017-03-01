#ifndef __wtlplacementwidget_h__
#define __wtlplacementwidget_h__

#pragma once

#define Rad_To_Deg(RAD) ((RAD * 180.f) / PI_CONST)
#define Deg_To_Rad(DEG) ((DEG * PI_CONST) / 180.f)

//----------------------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------------------
template<class T>
class CWTLPlacementWidget : public CPaneContainer
{
public:
	BEGIN_MSG_MAP(CWTLPlacementWidget)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_USER_DELETE_ALL, OnDeleteAll)
		MESSAGE_HANDLER(WM_USER_FILL_PROPS, OnFillProps)
		MESSAGE_HANDLER(WM_USER_UPDATE_PROPS, OnUpdateProps)
		COMMAND_CODE_HANDLER(EN_CHANGE, OnTextChange)
		CHAIN_MSG_MAP(CPaneContainer)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()

	//----------------------------------------------------------------------------------------------
	CWTLPlacementWidget()
		: m_pSelectedActor(0)
		, m_EMode(ECM_Move)
		, m_pBaseProp(0)
		, m_nByteShift(0)
		, m_PointSize(90)
		, m_TypeFace(_T("MS Shell Dlg 2"))
		, m_bProgrammaticallyUpdateEditbox(0)
		, m_pSDK(0)
	{
		m_pEditX = new CEdit();
		m_pEditY = new CEdit();
		m_pEditZ = new CEdit();
	}

	//----------------------------------------------------------------------------------------------
	~CWTLPlacementWidget()
	{
		SAFE_DELETE(m_pEditX);
		SAFE_DELETE(m_pEditY);
		SAFE_DELETE(m_pEditZ);
	}

	//----------------------------------------------------------------------------------------------
	T*		 GetSelected() const { return m_pSelectedActor; }
	void	 SetSelected(T *pActor) { m_pSelectedActor = pActor; }

	//----------------------------------------------------------------------------------------------
	void Update(const T *pSender, ESystemEventID EventId)
	{
		if (EventId == Event_PreRelease) // release selected
		{
		}
		else
		{
			if (EventId == Event_OnSelected)
			{
				SetSelected(const_cast<T*>(pSender));

				PostMessage(WM_USER_DELETE_ALL, 0, 0);
				PostMessage(WM_USER_FILL_PROPS, 0, 0);
			}
			else if (EventId == Event_PropertyChanged || EventId == Event_OnChangePivot)
			{
				PostMessage(WM_USER_UPDATE_PROPS, 0, 0);
			}
		}
	}

	//----------------------------------------------------------------------------------------------
	void SetMode(EObjEditControlMode mode)
	{
		m_EMode = mode;
		UpdatePreview();
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnDeleteAll(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		ClearAllEdit();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnFillProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		FillProperties();
		UpdatePreview();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnUpdateProps(UINT msg, WPARAM wParam, LPARAM lParam, BOOL&)
	{
		UpdatePreview();
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnLVEndLabelEdit(WPARAM wParam)
	{
		if (m_pBaseProp)
		{
			::GetWindowTextW(m_pEditX->m_hWnd, __wbufX, 32);
			::GetWindowTextW(m_pEditY->m_hWnd, __wbufY, 32);
			::GetWindowTextW(m_pEditZ->m_hWnd, __wbufZ, 32);

			WideCharToMultiByte(CP_ACP, 0, __wbufX, -1, __asciiX, sizeof(__asciiX), NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, __wbufY, -1, __asciiY, sizeof(__asciiY), NULL, NULL);
			WideCharToMultiByte(CP_ACP, 0, __wbufZ, -1, __asciiZ, sizeof(__asciiZ), NULL, NULL);

			if (m_EMode == ECM_Move)
			{
				for (std::vector<IBaseInterface*>::iterator Iter = m_pSelectedActor->m_VecInterfacesImp.begin(); Iter != m_pSelectedActor->m_VecInterfacesImp.end(); ++Iter)
				{
					if (IDrawInterface *pInterface = dynamic_cast<IDrawInterface*>(*Iter))
					{
						Matrix LTM = pInterface->GetLTM_();
						LTM.t = Vector((float) atof(__asciiX), (float) atof(__asciiY), (float) atof(__asciiZ));
						pInterface->SetLTM_(LTM);

						pInterface->GetCoreSDK()->GetViewportManager()->RebuildTransform(m_pSelectedActor);
						break;
					}
				}

			}
			else if (m_EMode == ECM_Rotate)
			{
				for (std::vector<IBaseInterface*>::iterator Iter = m_pSelectedActor->m_VecInterfacesImp.begin(); Iter != m_pSelectedActor->m_VecInterfacesImp.end(); ++Iter)
				{
					if (IDrawInterface *pInterface = dynamic_cast<IDrawInterface*>(*Iter))
					{
						Vector &vRot = pInterface->GetRotator();

						vRot[0] = Deg_To_Rad((float) atof(__asciiX));
						vRot[1] = Deg_To_Rad((float) atof(__asciiY));
						vRot[2] = Deg_To_Rad((float) atof(__asciiZ));

						Quaternion rot(0.f, 0.f, 0.f, 1.f);
						rot.set_rot(vRot[0], vRot[1], vRot[2]);

						Matrix M;
						rot.Normalize();
						rot.ToMatrix(&M);

						M.t =  pInterface->GetPosition_();
						pInterface->SetLTM_(M);

						pInterface->GetCoreSDK()->GetViewportManager()->RebuildTransform(m_pSelectedActor);
						break;
					}
				}
			}
			else if (m_EMode == ECM_Scale)
			{
				for (std::vector<IBaseInterface*>::iterator Iter = m_pSelectedActor->m_VecInterfacesImp.begin(); Iter != m_pSelectedActor->m_VecInterfacesImp.end(); ++Iter)
				{
					if (IDrawInterface *pInterface = dynamic_cast<IDrawInterface*>(*Iter))
					{
						Matrix3f STM = pInterface->GetSTM_();

						STM[0] = (float) atof(__asciiX);
						STM[4] = (float) atof(__asciiY);
						STM[8] = (float) atof(__asciiZ);

						pInterface->SetSTM_(STM);

						pInterface->GetCoreSDK()->GetViewportManager()->RebuildTransform(m_pSelectedActor);
						break;
					}
				}
			}
		}

		return 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL& bHandled)
	{
		const int Shift = 1500;
		const int EditWidth = 60;
		const int EditHeight = 20;
		const int WDist = 5;

		RECT rect;
		GetClientRect(&rect);

		int Width = rect.right - rect.left;
		int Height = rect.bottom - rect.top;

		Width = 1000; // HACK

		// Create the edit's control font
		m_Font.CreatePointFont(m_PointSize, m_TypeFace);

		m_pEditX->Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
		m_pEditY->Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
		m_pEditZ->Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL, WS_EX_STATICEDGE);

		m_pEditX->SetFont(m_Font);
		m_pEditY->SetFont(m_Font);
		m_pEditZ->SetFont(m_Font);

		unsigned int wdth = EditWidth + WDist;
		unsigned int right = Width - (wdth * 3);

		m_pEditX->MoveWindow(right, EditHeight, EditWidth, EditHeight, TRUE);
		m_pEditY->MoveWindow(right + wdth, EditHeight, EditWidth, EditHeight, TRUE);
		m_pEditZ->MoveWindow(right + wdth * 2, EditHeight, EditWidth, EditHeight, TRUE);

		bHandled = false;
		return 0;
	}

	//----------------------------------------------------------------------------------------------
	void FillProperties()
	{
		oes::rflex::AppClassTree &classTree = oes::rflex::GetClassTree();

		if (oes::rflex::ClassNode *pClassNode = classTree.Find(m_pSelectedActor->GetType()))
		{
			bool bBreak = false;

			while (pClassNode && !bBreak)
			{
				// add interface properties
                oes::rflex::ClassNode::TVecInterfaceIter IterIntf = pClassNode->m_VecInterfaces.begin();

				while (IterIntf != pClassNode->m_VecInterfaces.end() && !bBreak)
				{
					if (oes::rflex::ClassNode *pNodeInterface = classTree.FindInterface((*IterIntf)->strType))
					{
						TVecPropertyConstIterator IterIntfProp = pNodeInterface->PropertyMap.begin();

						while (IterIntfProp != pNodeInterface->PropertyMap.end() && !bBreak)
						{
							if (!strcmp((*IterIntfProp)->GetName(), "LTM"))
							{
								m_pBaseProp = *IterIntfProp;
								m_nByteShift = (*IterIntf)->byteShift;
								bBreak = true;
							}
							++IterIntfProp;
						}
					}
					++IterIntf;
				}

				pClassNode = pClassNode->GetRootNode();
			}
		}
	}

	//----------------------------------------------------------------------------------------------
	void UpdatePreview()
	{
		m_bProgrammaticallyUpdateEditbox = 1;

		if (m_pBaseProp)
		{
			char Buff[1024] = {0};

			int MemoryOffsetOverride = 0;
			if (m_nByteShift != -1){ // interface relative shift
				MemoryOffsetOverride = m_nByteShift;// m_PropertyClass->nOverrideByteShift;
			}

			m_pBaseProp->GetProperty((BYTE*)((void*)m_pSelectedActor) + MemoryOffsetOverride, Buff);

			CStringUtility<float> ParseString(Buff, ";");

			if (ParseString.m_vector.size() == 16)
			{
				char BuffDigit[32] = {0};

				if (m_EMode == ECM_Move)
				{
					for (std::vector<IBaseInterface*>::iterator Iter = m_pSelectedActor->m_VecInterfacesImp.begin(); Iter != m_pSelectedActor->m_VecInterfacesImp.end(); ++Iter)
					{
						if (IDrawInterface *pInterface = dynamic_cast<IDrawInterface*>(*Iter))
						{
							Matrix LTM = pInterface->GetLTM_();

							sprintf(BuffDigit, "%0.4f", LTM.t.x);
							m_pEditX->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());

							sprintf(BuffDigit, "%0.4f", LTM.t.y);
							m_pEditY->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());

							sprintf(BuffDigit, "%0.4f", LTM.t.z);
							m_pEditZ->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());
							break;
						}
					}
				}
				else if (m_EMode == ECM_Rotate)
				{
					for (std::vector<IBaseInterface*>::iterator Iter = m_pSelectedActor->m_VecInterfacesImp.begin(); Iter != m_pSelectedActor->m_VecInterfacesImp.end(); ++Iter)
					{
						if (IDrawInterface *pInterface = dynamic_cast<IDrawInterface*>(*Iter))
						{
							Vector &VRot = pInterface->GetRotator();

							sprintf(BuffDigit, "%0.4f", Rad_To_Deg(VRot.x));
							m_pEditX->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());

							sprintf(BuffDigit, "%0.4f", Rad_To_Deg(VRot.y));
							m_pEditY->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());

							sprintf(BuffDigit, "%0.4f", Rad_To_Deg(VRot.z));
							m_pEditZ->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());
							break;
						}
					}
				}
				else if (m_EMode == ECM_Scale)
				{
					for (std::vector<IBaseInterface*>::iterator Iter = m_pSelectedActor->m_VecInterfacesImp.begin(); Iter != m_pSelectedActor->m_VecInterfacesImp.end(); ++Iter)
					{
						if (IDrawInterface *pInterface = dynamic_cast<IDrawInterface*>(*Iter))
						{
							Matrix3f mScale = pInterface->GetSTM_();
							sprintf(BuffDigit, "%0.4f", mScale[0]);
							m_pEditX->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());

							sprintf(BuffDigit, "%0.4f", mScale[4]);
							m_pEditY->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());

							sprintf(BuffDigit, "%0.4f", mScale[8]);
							m_pEditZ->SetWindowText(ConvertStringToWideString(BuffDigit).c_str());
							break;
						}
					}
				}
			}
		}

		m_bProgrammaticallyUpdateEditbox = 0;
	}

	//----------------------------------------------------------------------------------------------
	void ClearAllEdit()
	{
		m_pBaseProp = 0;

		m_bProgrammaticallyUpdateEditbox = 1;

		m_pEditX->SetWindowText(L"");
		m_pEditY->SetWindowText(L"");
		m_pEditZ->SetWindowText(L"");

		m_bProgrammaticallyUpdateEditbox = 0;
	}

	//----------------------------------------------------------------------------------------------
	LRESULT OnTextChange(WORD /*wNotifyCode*/, WORD wID, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		if (m_bProgrammaticallyUpdateEditbox == 0)
		{
			OnLVEndLabelEdit(0);
		}
		return 0;
	}

	//----------------------------------------------------------------------------------------------
private:
	T *m_pSelectedActor;

	EObjEditControlMode m_EMode;

    oes::rflex::Property_Base	*m_pBaseProp;
	int				m_nByteShift;
	//SPropertyClass	*m_PropertyClass;

	CEdit	*m_pEditX;
	CEdit	*m_pEditY;
	CEdit	*m_pEditZ;

	CFont			m_Font;
	LPCTSTR			m_TypeFace;
	int				m_PointSize;

	BOOL m_bProgrammaticallyUpdateEditbox;

	wchar_t __wbufX[32];
	wchar_t __wbufY[32];
	wchar_t __wbufZ[32];

	char __asciiX[32];
	char __asciiY[32];
	char __asciiZ[32];

	CCoreSDK *m_pSDK;
};

#endif//__wtlplacementwidget_h__