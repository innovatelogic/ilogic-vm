// #ifndef __uislider_h__
// #define __uislider_h__
// 
// #ifdef _WIN32
// #pragma once
// #endif
// 
// #include "UIScreenObject.h"
// 
// enum ESliderOrientation
// {
//   SLIDER_Vertical = 0,
//   SLIDER_Horizontal,
// };
// 
// class UISlider: public UIScreenObject
// {
// 
// public:
//    UISlider(const CObjectAbstract * Parent);
//    virtual ~UISlider();
// 
//    virtual void SetOrientation(ESliderOrientation orientation ) { Orientation = orientation; } 
//    virtual ESliderOrientation GetOrientation() { return Orientation; }
//    virtual void Initialize();
// 
//    virtual Vector2f GetTransformedSize();
//   
//    inline_ virtual void OnDraw(const Matrix &WTM);
//    
//    virtual void ScrollNext(ESliderOrientation orientation);
//    virtual void ScrollPrev(ESliderOrientation orientation);
// 
//    static float GetSize() { return ButtonHeight; }
// 
// protected:
// private:
// 	ESliderOrientation Orientation;
// 	bool bHorizontal;
// 
// 	static float ButtonHeight;
// 
// 	int SCROLL_PX_SPEED;
// };
// 
// 
// #endif//__uislider_h__