#include "coresdkafx.h"

REGISTER_CLASS(UIImage, UIScreenObject);

//----------------------------------------------------------------------------------------------
UIImage::UIImage(const CObjectAbstract *pParent)
: Super(pParent)
{
	// add transient object:
	NEW_OBJECT_TRANSIENT_CHILD(m_pImageComponent, UIComp_DrawImage, "UIImage_ImageComponent", this);

	RegisterRenderEntity(m_pImageComponent);
	RegisterRenderEntity(m_pImageComponent->GetStaticMesh());
}

//----------------------------------------------------------------------------------------------
UIImage::UIImage(const UIImage& Source)
: Super(Source)
{
}

//----------------------------------------------------------------------------------------------
UIImage::~UIImage()
{
}

//----------------------------------------------------------------------------------------------
void UIImage::DoDraw()
{
	m_pImageComponent->DrawImage();
}

//----------------------------------------------------------------------------------------------
void UIImage::LoadImage(const wchar_t * filename, IListener * pListener /*= 0*/)
{
	m_pImageComponent->LoadImage(filename, pListener);
}

//----------------------------------------------------------------------------------------------
bool UIImage::GetInfo(unsigned int &Width, unsigned int &Height) const
{
	return m_pImageComponent->GetInfo(Width, Height); 
}

//----------------------------------------------------------------------------------------------
bool UIImage::GetDIB(BYTE** pData, bool bResample /*= false*/, unsigned int NewWidth /*= 0*/, unsigned int NewHeight /*= 0*/) const
{ 
	return m_pImageComponent->GetDIB(pData, bResample, NewWidth, NewHeight); 
}