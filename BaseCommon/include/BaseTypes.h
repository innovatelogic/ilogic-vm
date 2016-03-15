#pragma once

/** position states for UI Controls */
enum EPixelTransform
{
	PixelOwner_Transform        = 0,
	PixelViewport_Transform		= 1,
	PersentageOwner_Transform   = 2,
	PersentageViewport_Transform = 3,
};

enum EHandleInputKey
{
	HIK_HandleInputKey = 0,
	HIK_IgnoreInputKey,
	EHandleInputKey_MAX,
};

enum EHandleInputMouse
{
	HIM_HandleInputMouse = 0,
	HIM_IgnoreInputMouse,
	EHandleInputMouse_MAX,
};

enum ESceneInputmode
{
	INPUTMODE_None			= 0,
	INPUTMODE_ActiveOnly	= 1,
	ESceneInputmode_MAX,
};