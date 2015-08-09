#include "coresdkafx.h"

REGISTER_CLASS_A(Label, UIScreenObject)
	new PropertyString("Text", (BYTE*)&((Label*)NULL)->Text - (BYTE*)NULL, "Label",	"Value", READ_WRITE, CTRL_EDIT,	SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(Label, UIScreenObject);

//----------------------------------------------------------------------------------
Label::Label(const CObjectAbstract * Parent /*= NULL*/)
: Super(Parent)
, Text(L"Label text")
, FontComponent(NULL)
{
	NEW_OBJECT_TRANSIENT_CHILD(FontComponent, UIComp_DrawFont, "FontComponent", this);
}

//----------------------------------------------------------------------------------
Label::Label(const Label & Source)
: Super(Source)
{
	if (&Source != this)
	{
		FontComponent = CLONE_OBJECT(UIComp_DrawFont, Source.FontComponent, Source.FontComponent->GetName(), this);

		Text = Source.Text;
	}
}

//----------------------------------------------------------------------------------
Label::~Label()
{
	;//
}

//----------------------------------------------------------------------------------
void Label::DoDraw()
{
	FontComponent->SetText(Text);
	GetRenderComponent()->PushToRenderStack(new RenderChunk<UIComp_DrawFont, CRenderObject>(&CRenderObject::Render, FontComponent));


	//RenderChunk<UIComp_DrawFont, CRenderObject> *pChunk = ()
}