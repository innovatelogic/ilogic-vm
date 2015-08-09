#include "coresdkafx.h"

static const EHandleInputKey DefInputKeyMode = HIK_HandleInputKey;
static const EHandleInputMouse DefInputMouseMode = HIM_HandleInputMouse;

REGISTER_CLASS_A(UIScene, UIScreenObject)
	new PropertySceneInputmode("InputMode",	(BYTE*)&((UIScene*)NULL)->m_eInputMode - (BYTE*)NULL, "UIScene", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyEHandleInputKey("HandleInputKey", (BYTE*)&((UIScene*)NULL)->m_eHandleInputKey - (BYTE*)NULL, "UIScene", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, ((const int*)&DefInputKeyMode)),
	new PropertyEHandleInputMouse("HandleInputMouse", (BYTE*)&((UIScene*)NULL)->m_eHandleInputMouse - (BYTE*)NULL, "UIScene", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP, 0, 0, ((const int*)&DefInputMouseMode)),
END_REGISTER_CLASS(UIScene, UIScreenObject)

//----------------------------------------------------------------------------------------------
UIScene::UIScene(const CObjectAbstract * Parent)
: Super(Parent)
, IUIInputController(Parent)
, m_eInputMode(INPUTMODE_ActiveOnly)
, m_eHandleInputKey(DefInputKeyMode)
, m_eHandleInputMouse(DefInputMouseMode)
{
	RegisterUIInputInterface(this);
}

//----------------------------------------------------------------------------------------------
UIScene::UIScene(const UIScene &Source)
: Super(Source)
, IUIInputController(Source)
{
	if (this != &Source)
	{

	}
}

//----------------------------------------------------------------------------------------------
UIScene::~UIScene()
{

}

//----------------------------------------------------------------------------------------------
void UIScene::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);
}

//----------------------------------------------------------------------------------------------
void UIScene::Initialize()
{
	Super::Initialize();

	GetAppMain()->GetViewportManager()->RebuildTransform(this);
}

//----------------------------------------------------------------------------------------------
void UIScene::PreRelease()
{
	Super::PreRelease();

	GetAppMain()->GetUISceneManager()->RemoveScene(this);
}

//----------------------------------------------------------------------------------------------
void UIScene::DoDraw()
{
	//Super::Draw();
}

//----------------------------------------------------------------------------------------------
void UIScene::Update(float DeltaTime)
{

}

//----------------------------------------------------------------------------------------------
/*bool UIScene::OnEventPressed(const MouseInputData &InputData)
{
	return (InputMode == INPUTMODE_None) ? false : Super::OnEventPressed(InputData);
}

//----------------------------------------------------------------------------------------------
bool UIScene::OnMouseMove(const MouseMoveInputData &InputData)
{
	return (InputMode == INPUTMODE_None) ? false : Super::OnMouseMove(InputData);
}*/

//----------------------------------------------------------------------------------------------
bool UIScene::HitTest(const Vector2f &Position)
{
	return false;
}

//----------------------------------------------------------------------------------------------
bool UIScene::IsHandleMouseInput() const
{
	return true;
}

//----------------------------------------------------------------------------------------------
bool UIScene::IsHandleKeyInput() const
{
	return m_eHandleInputKey == HIK_HandleInputKey;
}

//----------------------------------------------------------------------------------------------
bool UIScene::ProcessButtonUI(const EventInput &InputData)
{
	return m_eHandleInputMouse == HIM_HandleInputMouse;
}