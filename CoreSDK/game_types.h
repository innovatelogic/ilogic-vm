#pragma once

#include "StdafxFoundation.h"
#include "D3DDriver.h"


//----------------------------------------------------------------------------------------------
struct MouseMoveInputData
{
    Vector2f MousePos;
    Vector2f DeltaPos;
    int	ModifKey;
    bool bMiddleButtonPressed;
    oes::d3d::SRenderContext *pRenderContext;

    MouseMoveInputData()
        : ModifKey(0)
        , bMiddleButtonPressed(false)
        , pRenderContext(0)
    {
        MousePos = DeltaPos = Vector2f(0.f, 0.f);
    }

    MouseMoveInputData(const MouseMoveInputData& Source)
    {
        if (&Source != this)
        {
            MousePos = Source.MousePos;
            DeltaPos = Source.DeltaPos;
            ModifKey = Source.ModifKey;
            bMiddleButtonPressed = Source.bMiddleButtonPressed;
            pRenderContext = Source.pRenderContext;
        }
    }
};

//----------------------------------------------------------------------------------------------
struct MouseInputData
{
    Event		event;
    MouseCode	Code;
    int			ModifKey;
    Vector2f	MousePos;
    oes::d3d::SRenderContext *pRenderContext;

    MouseInputData()
        : ModifKey(0)
        , pRenderContext(0)
    {
        MousePos = Vector2f(0.f, 0.f);
    }
};

#ifndef CORESDK_EXPORTS
#define CORESDK_API __declspec(dllexport)
#else
#define CORESDK_API __declspec(dllimport)
#endif