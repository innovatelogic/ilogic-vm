#ifndef __landinglayer_h__
#define __landinglayer_h__

#include "cocos2d.h"
//#include "cocos-ext.h"
#include <queue>

using namespace cocos2d;
//using namespace cocos2d::extension;

//----------------------------------------------------------------------------------------------
struct SLandingData
{
	CCSprite	*pSpriteLabel;
	float		fTimeElapsed;
	float		fA1;
	float		fA2;
	bool		bHold;
	
	static const float fFadeTime;

	SLandingData(float a1, float a2, bool hold = false)
		: pSpriteLabel(0)
		, fTimeElapsed(0.f)
		, fA1(a1)
		, fA2(a2)
		, bHold(hold)
	{}

	void Process(float dt);

	bool IsPlayed() const { return fTimeElapsed >= fFadeTime; }

	bool GetHold() const { return bHold; }

	void SetOpacity(float alpha);
};

//----------------------------------------------------------------------------------------------
class CLandingLayer : public CCLayer
{
	typedef std::queue<SLandingData*>	TQueueLayer;

public:
	CLandingLayer();
	virtual ~CLandingLayer();

	virtual bool init();

	virtual void onEnter();

	void tick(float dt);

	virtual void registerWithTouchDispatcher();

	virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);

	void SetRootScene(CCScene *pScene) { m_pRootScene = pScene; }

	// implement the "static node()" method manually
	CREATE_FUNC(CLandingLayer);

protected:
	void SwitchToNext();
	void ClearLayers();

private:
	TQueueLayer	m_QueueLayers;

	CCScene *m_pRootScene;	
};

#endif//__landinglayer_h__