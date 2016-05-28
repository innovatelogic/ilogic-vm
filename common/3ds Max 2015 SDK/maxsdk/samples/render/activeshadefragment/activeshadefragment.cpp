//
// Copyright 2014 Autodesk, Inc.  All rights reserved. 
//
// This computer source code and related instructions and comments are the
// unpublished confidential and proprietary information of Autodesk, Inc. and
// are protected under applicable copyright and trade secret law.  They may
// not be disclosed to, copied or used by any third party without the prior
// written consent of Autodesk, Inc.
//
//

#include "activeshadefragment.h"
#include <iparamb2.h>
#include <Graphics/ViewSystem/EvaluationContext.h>
#include <IColorCorrectionMgr.h>
#include "Resource.h"

using namespace MaxSDK::Graphics;

#define ACTIVE_SHADE_FRAGMENT_CLASS_ID Class_ID(0x12c31f85, 0xe40a704e)

class ActiveShadeFragmentDesc:public ClassDesc2
{
public:
	int 			IsPublic() {return TRUE;}
	void *			Create(BOOL loading)
	{
		loading;
		return new ::MaxGraphics::ActiveShadeFragment;
	}
	const MCHAR *	ClassName() { return _M("ActiveShadeFragment"); }
	SClass_ID		SuperClassID() { return Fragment_CLASS_ID; }
	Class_ID		ClassID() { return ACTIVE_SHADE_FRAGMENT_CLASS_ID;}
	const MCHAR* 	Category() { return _M("Fragment"); }
};
static ActiveShadeFragmentDesc oneActiveShadeFragmentDesc;
ClassDesc2* GetActiveShadeFragmentDesc() { return &oneActiveShadeFragmentDesc; }

namespace MaxGraphics 
{
	enum ActiveShadeFragmentInput
	{
		ActiveShadeFragmentInput_ColorTarget, 
		ActiveShadeFragmentInput_Count, 
	};

	enum ActiveShadeFragmentOutput
	{
		ActiveShadeFragmentOutput_ColorTarget, 
		ActiveShadeFragmentOutput_Count, 
	};

	ActiveShadeFragment::ActiveShadeFragment()
	{
		mpBitmap = NULL;
		mpRenderer = NULL;
		mIRenderInterface = NULL;

		mHwnd = NULL;
		mpViewExp = NULL;
		memset(mDefaultLights, 0, sizeof(mDefaultLights));
		mNumDefaultLights = 0;

		mBitmapWidth				= 1;
		mBitmapHeight				= 1;

		Class_ID inputIDs[ActiveShadeFragmentInput_Count];
		inputIDs[ActiveShadeFragmentInput_ColorTarget] = TargetHandle::ClassID();
		InitializeInputs(ActiveShadeFragmentInput_Count, inputIDs);

		Class_ID outputIDs[ActiveShadeFragmentOutput_Count];
		outputIDs[ActiveShadeFragmentOutput_ColorTarget] = TargetHandle::ClassID();
		InitializeOutputs(ActiveShadeFragmentOutput_Count, outputIDs);

		SetTimerTicks(100);
		RegisterMessage(FragmentMessageIDEndFrame);
	}

	ActiveShadeFragment::~ActiveShadeFragment()
	{
		Cleanup();
	}

	bool ActiveShadeFragment::DoEvaluate(EvaluationContext* evaluationContext)
	{
		if(NULL == evaluationContext)
		{
			return false;
		}
		ViewParameter* pViewportParameter = evaluationContext->pViewParameter.GetPointer();
		
		if(pViewportParameter->IsRenderRegionVisible())
		{
			mBitmapWidth	= size_t(pViewportParameter->GetWidth() *pViewportParameter->GetRenderRegion().mScreenSpaceScale.x);
			mBitmapHeight	= size_t(pViewportParameter->GetHeight()*pViewportParameter->GetRenderRegion().mScreenSpaceScale.y);

			mRenderRegion.SetXY(
				int(pViewportParameter->GetClippedSourceRegion().pmin.x),
				int(pViewportParameter->GetClippedSourceRegion().pmin.y));
			//If there is no +1 ,the black outline of the rendered bitmap will be displayed in the  target.
			mRenderRegion.SetWH(
				int(pViewportParameter->GetClippedSourceRegion().pmax.x - pViewportParameter->GetClippedSourceRegion().pmin.x + 1), 
				int(pViewportParameter->GetClippedSourceRegion().pmax.y - pViewportParameter->GetClippedSourceRegion().pmin.y + 1));

			TargetHandle targetHandle;
			if (!GetSmartHandleInput(targetHandle,ActiveShadeFragmentInput_ColorTarget) ||
				!targetHandle.IsValid())
			{
				return false;
			}

			if(SynchronizeToTarget(targetHandle,evaluationContext))
			{
				return SetSmartHandleOutput(targetHandle,ActiveShadeFragmentOutput_ColorTarget);
			}
		}

		return false;	
	}

	bool ActiveShadeFragment::SynchronizeToTarget(BaseRasterHandle& targetHandle,
		EvaluationContext* evaluationContext)
	{
		IColorCorrectionMgr* idispGamMgr = (IColorCorrectionMgr*)GetCOREInterface(COLORCORRECTIONMGR_INTERFACE);
		if( idispGamMgr==NULL )
			return false; // Need the display gamma manager, but it isn't available

		if (NULL == mpRenderer)
		{
			Startup(evaluationContext, mBitmapWidth, mBitmapHeight);
		}

		if (NULL == mIRenderInterface)
		{
			return false;
		}

		if (mpBitmap == NULL || 
			mpBitmap->Width() != mBitmapWidth || 
			mpBitmap->Height() != mBitmapHeight)
		{
			Cleanup();
			Startup(evaluationContext, mBitmapWidth, mBitmapHeight);
		}

		if (targetHandle.GetWidth() != mBitmapWidth ||
			targetHandle.GetHeight() != mBitmapHeight)
		{
			targetHandle.SetSize(mBitmapWidth,mBitmapHeight);
		}
		if(targetHandle.GetFormat() != TargetFormatA8R8G8B8)
		{
			targetHandle.SetFormat(TargetFormatA8R8G8B8);
		}

		mLineColors.resize(mpBitmap->Width());

		// update color target!
		size_t destRowPitch = 0, destSlicePitch = 0;
		byte* pDestData = (byte*)targetHandle.Map(WriteAcess,destRowPitch,destSlicePitch);
		for (int y = 0; y < mpBitmap->Height(); ++y)
		{
			mpBitmap->GetPixels(0, y, mpBitmap->Width(), &mLineColors[0]);
			for (int x = 0; x < mpBitmap->Width(); ++x)
			{
				DWORD color = 
					(((DWORD)(mLineColors[x].a >> 8)) << 24) | 
					(((DWORD)(idispGamMgr->ColorCorrect16(mLineColors[x].r, IColorCorrectionMgr::kBLUE_C))) << 16) | 
					(((DWORD)(idispGamMgr->ColorCorrect16(mLineColors[x].g, IColorCorrectionMgr::kGREEN_C))) << 8) | 
					(((DWORD)(idispGamMgr->ColorCorrect16(mLineColors[x].b, IColorCorrectionMgr::kRED_C))));
				*((DWORD*)pDestData + x) = GetDeviceCompatibleARGBColor(color);
			}
			pDestData += destRowPitch;
		}
		targetHandle.UnMap();

		return true;
	}

	void ActiveShadeFragment::Startup(EvaluationContext* evaluationContext, size_t width, size_t height)
	{
		if (NULL != mpRenderer)
		{
			return;
		}

		Interface* pInterface = GetCOREInterface();
		Renderer *pRenderer = pInterface->GetRenderer(RS_IReshade);
		mpRenderer = (pRenderer) ? (Renderer*)CloneRefHierarchy(pRenderer) : NULL;
		if (!mpRenderer)
		{
			return;
		}
		mIRenderInterface = reinterpret_cast<IInteractiveRender *>( mpRenderer->GetInterface(I_RENDER_ID) );

		// create the bitmap
		BitmapInfo bi;
		bi.SetWidth((int)width);
		bi.SetHeight((int)height);
		bi.SetType(BMM_TRUE_64); // 64-bit color: 16 bits each for Red, Green, Blue, and Alpha.
		bi.SetFlags(MAP_HAS_ALPHA);
		bi.SetAspect(1.0f);
		mpBitmap = TheManager->Create(&bi);

		ViewParameter* pViewportParameter = evaluationContext->pViewParameter.GetPointer();
		
		mpViewExp = pViewportParameter->GetViewExp();
		mHwnd = mpViewExp->GetHWnd();		
		mNumDefaultLights = GetCOREInterface7()->InitDefaultLights(mDefaultLights, 2, FALSE, mpViewExp, TRUE);

		mIRenderInterface->SetOwnerWnd( mpViewExp->GetHWnd() );
		mIRenderInterface->SetIIRenderMgr(this);
		mIRenderInterface->SetBitmap( mpBitmap );
		mIRenderInterface->SetSceneINode( pInterface->GetRootNode() );
		mIRenderInterface->SetUseViewINode(false);
		mIRenderInterface->SetViewINode(NULL);
		mIRenderInterface->SetViewExp(mpViewExp);	
		mIRenderInterface->SetRegion(mRenderRegion);
		mIRenderInterface->SetDefaultLights(mDefaultLights, mNumDefaultLights);
		mIRenderInterface->SetProgressCallback( static_cast<IRenderProgressCallback *>(this) );
		mIRenderInterface->BeginSession();
	}

	void ActiveShadeFragment::Cleanup()
	{
		if( mIRenderInterface )
		{
			mIRenderInterface->EndSession();
		}

		if (NULL != mpRenderer)
		{
			if (NULL != mIRenderInterface)
			{
				// we're done with the reshading interface
				mpRenderer->ReleaseInterface(I_RENDER_ID, mIRenderInterface);
				mIRenderInterface = NULL;
			}
			mpRenderer->MaybeAutoDelete();
			mpRenderer = NULL;
		}

		if (NULL != mpBitmap)
		{
			mpBitmap->DeleteThis();
			mpBitmap = NULL;
		}
	}

	bool ActiveShadeFragment::OnMessage(const MaxSDK::Graphics::FragmentMessageParameter& messageParameter)
	{
		switch(messageParameter.messageID)
		{
		case FragmentMessageIDTimer:
			{
				if (IsRendering())
				{
					SetFlag(FragmentFlagsDirty,true);
				}
			}
			break;
		default:
			break;
		}

		return true;
	}
}


