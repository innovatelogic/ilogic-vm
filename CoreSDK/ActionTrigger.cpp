#include "coresdkafx.h"

//REGISTER_CLASS_ACTION(ActionTrigger, ActionBase);

#define CAPTION_HEIGHT 35
#define SHIFT_HEIGHT 25

//----------------------------------------------------------------------------------------------
ActionTrigger::ActionTrigger(const CObjectAbstract * Parent)
: Super(Parent)
{
	Size = Vector2f(200.f, 50.f);
}

//----------------------------------------------------------------------------------------------
ActionTrigger::ActionTrigger(const ActionTrigger & Source)
: Super(Source)
{
	if (this != &Source)
	{
		;//
	}
}

//----------------------------------------------------------------------------------------------
ActionTrigger::~ActionTrigger()
{
	;//
}

//----------------------------------------------------------------------------------------------
// @TODO: Redesign
void ActionTrigger::RenderFuncZone(HDC hdc)
{
	HPEN hpen, hPenOld;
	hpen = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	hPenOld = (HPEN)SelectObject(hdc, hpen);

	POINT pntArray[7] = {
		{(int)(Position.x), (int)Position.y + SHIFT_HEIGHT + CAPTION_HEIGHT},
			{(int)(Position.x + Size.x * 0.5f), (int)Position.y + CAPTION_HEIGHT},
		{(int)(Position.x + Size.x), (int)Position.y + SHIFT_HEIGHT + CAPTION_HEIGHT},
		{(int)(Position.x + Size.x), (int)(Position.y + Size.y + SHIFT_HEIGHT + CAPTION_HEIGHT)},
			{(int)(Position.x + Size.x * 0.5f), (int)(Position.y + Size.y + 2 * SHIFT_HEIGHT + CAPTION_HEIGHT)},
		{(int)Position.x, (int)(Position.y + Size.y + SHIFT_HEIGHT + CAPTION_HEIGHT)},
		pntArray[0], // holy shirt!!
	};

	Polyline(hdc, pntArray, 7);

	SelectObject(hdc, hPenOld);
	DeleteObject(hpen);
}