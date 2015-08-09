#include "StdafxRender.h"

REGISTER_CLASS_A(UIComp_DrawFont, CRenderObject)
	new PropertyString("FontFamily", (BYTE*)&((UIComp_DrawFont*)NULL)->m_FontFamily - (BYTE*)NULL, "UIComp_DrawFont", "Value", READ_WRITE, CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyFLOAT("Height", (BYTE*)&((UIComp_DrawFont*)NULL)->m_Height - (BYTE*)NULL, "UIComp_DrawFont", "Value",	READ_WRITE,	CTRL_EDIT, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
	new PropertyUINT("Color", (BYTE*)&((UIComp_DrawFont*)NULL)->m_Color - (BYTE*)NULL, "UIComp_DrawFont", "Value", READ_WRITE, CTRL_COLOR, SERIALIZABLE, NON_COMMON_PROP, INT_PROP),
END_REGISTER_CLASS(UIComp_DrawFont, CRenderObject);

//----------------------------------------------------------------------------------
UIComp_DrawFont::UIComp_DrawFont(const CObjectAbstract * Parent/* = NULL*/)
: Super(Parent)
, m_Value(L"")
, m_FontFamily("Arial")
, m_Height(12)
, m_Color(0xff000000)
{
	m_pFontComponent = Alloc_Font2D(GetRenderComponent()->GetRenderDriver());
}

//----------------------------------------------------------------------------------
UIComp_DrawFont::UIComp_DrawFont(const UIComp_DrawFont& Source)
: Super(Source)
{
	if (this != &Source)
	{
	}
}

//----------------------------------------------------------------------------------
UIComp_DrawFont::~UIComp_DrawFont()
{
	Release_Font2D(m_pFontComponent);
	m_pFontComponent = NULL;
}

//----------------------------------------------------------------------------------
void UIComp_DrawFont::OnPropertyChanged(const char* PropertyName)
{
	Super::OnPropertyChanged(PropertyName);

	if (!strcmp(PropertyName, "FontFamily") ||
		!strcmp(PropertyName, "Height"))
	{
		m_pFontComponent->Release();
		m_pFontComponent->Load(m_FontFamily.c_str(), m_Height);
	}
}

//----------------------------------------------------------------------------------
void UIComp_DrawFont::SetStyle(const std::string& strStyleInfo)
{ 
	ValueParser ValueStore(strStyleInfo);

	m_FontFamily = ValueStore.GetValueString(std::string("FontFamily"));
	m_Height  = (size_t)ValueStore.GetValueFloat(std::string("Height"));
	m_Color	  = ValueStore.GetValueUINT(std::string("Color"));
}

//----------------------------------------------------------------------------------
void UIComp_DrawFont::Initialize()
{
	Super::Initialize();

	ReinitStyles();
}

//----------------------------------------------------------------------------------
void UIComp_DrawFont::Render() const
{
	m_pFontComponent->Draw(m_Value.c_str(), m_Color);
}

//----------------------------------------------------------------------------------
void UIComp_DrawFont::ReinitStyles()
{
	DWORD ThreadId = ::GetCurrentThreadId();

	if (ThreadId != GetRenderComponent()->ThreadRenderID)
	{
		GetRenderComponent()->PushToRenderStack( new Command<UIComp_DrawFont, UIComp_DrawFont>(&UIComp_DrawFont::ReinitStyles, this), true);
	}
	else
	{
		m_pFontComponent->Release();
		m_pFontComponent->Load(m_FontFamily.c_str(), m_Height);
	}
}