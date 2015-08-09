#include "coresdkafx.h"

REGISTER_CLASS_A(LabelButton, UIScreenObject)
	new PropertyFLOAT("TextPositionX", (BYTE*)&((LabelButton*)NULL)->TextPositionX - (BYTE*)NULL, "LabelButton", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyFLOAT("TextPositionY", (BYTE*)&((LabelButton*)NULL)->TextPositionY - (BYTE*)NULL, "LabelButton", "Value", READ_WRITE, CTRL_COMBO, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyStringW("Text",	(BYTE*)&((LabelButton*)NULL)->Text - (BYTE*)NULL, "LabelButton", "Text", READ_WRITE, CTRL_EDIT,	SERIALIZABLE, COMMON_PROP, INT_PROP), // common uninterrupted value
END_REGISTER_CLASS(LabelButton, UIScreenObject)

//----------------------------------------------------------------------------------------------
LabelButton::LabelButton(const CObjectAbstract * Parent /*= NULL*/)
: Super(Parent)
, Text(L"Label text")
, FontComponentState(NULL)
, ImageComponents(NULL)
{	
	NEW_OBJECT_TRANSIENT_CHILD(ImageComponents, UIComp_DrawImageState, "ImageComponents", this);
	NEW_OBJECT_TRANSIENT_CHILD(FontComponentState, UIComp_DrawFontState, "FontComponentState", this);

	NEW_OBJECT_TRANSIENT(ActionPress, ActionHandler, "ActionPress");
	NEW_OBJECT_TRANSIENT(ActionRelease, ActionHandler, "ActionRelease");

	RegisterRenderEntity(ImageComponents);
	RegisterRenderEntity(ImageComponents->GetStaticMesh());
	RegisterRenderEntity(FontComponentState);
	
	ActionPress->SetOwnerActor(this);
	ActionRelease->SetOwnerActor(this);

	ActionPress->SetPosition2f(Vector2f(100, 100));
	ActionRelease->SetPosition2f(Vector2f(100, 150));

	ActionPress->SetAppMain(GetAppMain());
	ActionRelease->SetAppMain(GetAppMain());

	ScriptDriver * Driver = GetAppMain()->GetScriptDriver();

	Driver->RegisterAction(ActionPress);
	Driver->RegisterAction(ActionRelease);
}

//----------------------------------------------------------------------------------------------
LabelButton::LabelButton(const LabelButton & Source)
: Super(Source)
{
	if (&Source != this)
	{	
	}
}

//----------------------------------------------------------------------------------------------
LabelButton::~LabelButton()
{
	ScriptDriver * Driver = GetAppMain()->GetScriptDriver();
	
	Driver->UnRegisterAction(ActionPress);
	Driver->UnRegisterAction(ActionRelease);

	ActionPress->ProcessRelease(true);
	ActionRelease->ProcessRelease(true);

	ActionPress = NULL;
	ActionRelease = NULL;
}

//----------------------------------------------------------------------------------------------
void LabelButton::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "Text"))
	{
		FontComponentState->RebuildMesh(Text);
	}

	if (!strcmp(PropertyName, "FontFamily") ||
		!strcmp(PropertyName, "Color") ||
		!strcmp(PropertyName, "Height"))
	{
		FontComponentState->ReinitStyles();
		FontComponentState->RebuildMesh(Text);
	}
}

//----------------------------------------------------------------------------------------------
void LabelButton::Initialize()
{
	Super::Initialize();

	ResolveActiveState();
	
	FontComponentState->RebuildMesh(Text);
}

//----------------------------------------------------------------------------------------------
void LabelButton::PostLink()
{
	ActionPress->PostLink();
	ActionRelease->PostLink();

	Super::PostLink();
}

//----------------------------------------------------------------------------------------------
void LabelButton::DoDraw()
{
	ImageComponents->DrawImage();

	GetRenderComponent()->DrawObject(FontComponentState);
}

//----------------------------------------------------------------------------------------------
void LabelButton::SetTextPositionX(float x)
{
	TextPositionX = x;

	assert(FontComponentState != NULL);
	FontComponentState->SetTextPositionX(TextPositionX);
}

//----------------------------------------------------------------------------------------------
float LabelButton::GetTextPositionX() const
{
	return TextPositionX;
}

//----------------------------------------------------------------------------------------------
void LabelButton::SetTextPositionY(float y)
{
	TextPositionY = y;

	assert(FontComponentState != NULL);
	FontComponentState->SetTextPositionY(TextPositionY);
}

//----------------------------------------------------------------------------------------------
float LabelButton::GetTextPositionY() const
{
	return TextPositionY;
}

//----------------------------------------------------------------------------------------------
bool LabelButton::DoEventPressed(const MouseInputData& InputData)
{
	ActionPress->Execute(this);
	return Super::DoEventPressed(InputData);
}

//----------------------------------------------------------------------------------------------
bool LabelButton::DoEventReleased(const MouseInputData& InputData)
{
	ActionRelease->Execute(this);
	return Super::DoEventReleased(InputData);
}

//----------------------------------------------------------------------------------------------
void LabelButton::ResolveActiveState()
{
	bool bPressed = ((GetVisualStates() & AS_PRESSED) != AS_NONE);
	bool bMouseOver = ((GetVisualStates() & AS_MOUSE_OVER) != AS_NONE);

	EActiveState State = !GetEnabled() ? ActiveState_Disabled :
							(
								bPressed ? ActiveState_Pressed :
									(
										IsMouseOver() ? ActiveState_Active :
										(
											IsFocused() ? ActiveState_Focused : ActiveState_Normal
										)
									)
							);

	ImageComponents->SetActiveState(State);
	FontComponentState->SetActiveState(State);
}

//----------------------------------------------------------------------------------------------
void LabelButton::SuperDeserializerInternal(TiXmlElement *pTree)
{
	NObjectFactory::TClassFactory *classFactory = GetClassFactoryA();

	XML_FOR_EACH_TREE(pTree)
	{
		XML_DEF_ATTRIBUTES_NODE(Transient);
		XML_DEF_ATTRIBUTES_NODE(Value);
		ValueParser ValueStore(Value);

		if (ValueStore.GetValueString("Name") == "ActionPress")
		{
			ActionPress->SuperDeserializer(XML_CURRENT_NODE);
		}
		else if (ValueStore.GetValueString("Name") == "ActionRelease")
		{
			ActionRelease->SuperDeserializer(XML_CURRENT_NODE);
		}
	}
}

//----------------------------------------------------------------------------------------------
bool LabelButton::SuperSerializerInternal(std::ofstream &Stream) 
{
	ActionPress->SuperSerializer(Stream);
	ActionRelease->SuperSerializer(Stream);

	return false;
}

//----------------------------------------------------------------------------------------------
bool LabelButton::SuperSerializerInternal(std::stringstream &Stream) 
{
	return false;
}