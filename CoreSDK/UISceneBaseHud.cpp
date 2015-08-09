#include "coresdkafx.h"

REGISTER_CLASS(UISceneBaseHud, UIScene);

//----------------------------------------------------------------------------------------------
UISceneBaseHud::UISceneBaseHud(const CObjectAbstract * Parent)
: Super(Parent)
, bBlink_LabelMessageLT(false)
, bBlink_LabelMessage1C(false)
, bBlink_LabelMessage2C(false)
, AnimTimeLT(0.f)
, AnimTime1C(0.f)
, AnimTime2C(0.f)
{
	NEW_OBJECT_TRANSIENT_CHILD(LabelMessageLT, Label, "LabelMessageLT", this);
	NEW_OBJECT_TRANSIENT_CHILD(LabelMessage1C, Label, "LabelMessage1C", this);
	NEW_OBJECT_TRANSIENT_CHILD(LabelMessage2C, Label, "LabelMessage2C", this);
}

//----------------------------------------------------------------------------------------------
UISceneBaseHud::UISceneBaseHud(const UISceneBaseHud & Source)
: Super(Source)
, bBlink_LabelMessageLT(false)
, bBlink_LabelMessage1C(false)
, bBlink_LabelMessage2C(false)
, AnimTimeLT(0.f)
, AnimTime1C(0.f)
, AnimTime2C(0.f)
{
	if (this != &Source)
	{
		LabelMessageLT = CLONE_OBJECT(Label, Source.LabelMessageLT, "LabelMessageLT", this);
		LabelMessage1C = CLONE_OBJECT(Label, Source.LabelMessage1C, "LabelMessage1C", this);
		LabelMessage2C = CLONE_OBJECT(Label, Source.LabelMessage2C, "LabelMessage2C", this);
	}
}

//----------------------------------------------------------------------------------------------
UISceneBaseHud::~UISceneBaseHud()
{

}

//----------------------------------------------------------------------------------------------
void UISceneBaseHud::Initialize()
{
	Super::Initialize();

	LabelMessageLT->SetVisible(false);
	LabelMessage1C->SetVisible(false);
	LabelMessage2C->SetVisible(false);

	BlinkLabelMessageLT("");
}

//----------------------------------------------------------------------------------------------
void UISceneBaseHud::Update(float DeltaTime)
{
	Super::Update(DeltaTime);

	float BlinkPeriod = 6.f;
	float HalfBlinkPeriod = BlinkPeriod * 0.5f;

	if (bBlink_LabelMessageLT)
	{
/*		if (LabelMessageLT->GetOpacity()  == 0){ // trap
			LabelMessageLT->SetOpacity(m_eps);
		}
		else
		{
			AnimTimeLT += DeltaTime;

			float LabelOpacity = 0.f;

			if (AnimTimeLT <= HalfBlinkPeriod)
			{
				LabelOpacity = AnimTimeLT / HalfBlinkPeriod;
			}
			else
			{
				LabelOpacity = 1.f - ((AnimTimeLT - HalfBlinkPeriod) / HalfBlinkPeriod);

				if (AnimTimeLT > BlinkPeriod)
				{
					LabelOpacity = 0.f;
					LabelMessageLT->SetVisible(false);
					bBlink_LabelMessageLT = false;
				}
			}
			LabelMessageLT->SetOpacity(LabelOpacity);
		}*/
	}
}

//----------------------------------------------------------------------------------------------
void UISceneBaseHud::BlinkLabelMessageLT(const std::string &Value, float Time /*= 2.5f*/)
{
	//LabelMessageLT->SetText(Value);
//	LabelMessageLT->SetOpacity(0.f);
	LabelMessageLT->SetVisible(true);
	bBlink_LabelMessageLT = true;
	AnimTimeLT = 0.f;
}

//----------------------------------------------------------------------------------------------
void UISceneBaseHud::BlinkLabelMessage1C(const std::string &Value, float Time /*= 2.5f*/)
{

}

//----------------------------------------------------------------------------------------------
void UISceneBaseHud::BlinkLabelMessage2C(const std::string &Value, float Time /*= 2.5f*/)
{

}

//----------------------------------------------------------------------------------------------
bool UISceneBaseHud::ProcessButtonUI(const EventInput &InputData)
{
	bool bResult = false;
	switch (InputData.Key)
	{
	case KEY_ESCAPE:
		{
			const char *pLPauseMenuName = "ScenePauseMenu.xml";

			CUISceneManager * pUISceneMgr = GetAppMain()->GetUISceneManager();
						
			//if (!pUISceneMgr->FindScene(pLPauseMenuName))
			{
				pUISceneMgr->Command_LoadScene(pLPauseMenuName);
				pUISceneMgr->Command_InitializeScene(pLPauseMenuName);
			}
			bResult = true;
		}break;
	default:
		break;
	};
	return bResult;
}