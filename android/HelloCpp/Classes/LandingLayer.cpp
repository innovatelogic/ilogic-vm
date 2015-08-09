
#include "LandingLayer.h"
#include "HelloWorldScene.h"

const float SLandingData::fFadeTime = 3.f;
const unsigned int tagFade = 0x0000FADE;

//----------------------------------------------------------------------------------------------
void SLandingData::Process(float dt)
{
	const float interval = fFadeTime  * fA1;
	const float interval_x2 = fFadeTime * fA2;

	float alpha = 0.f;

	if (fTimeElapsed <= interval)
	{
		alpha = fTimeElapsed / interval; // fade in
	}
	else if ((fTimeElapsed > interval && fTimeElapsed <= interval_x2) || bHold)
	{
		alpha = 1.f; // hold
	}
	else if (fTimeElapsed > interval_x2 && fTimeElapsed <= fFadeTime)
	{
		alpha = 1.f - (fTimeElapsed - interval_x2) / interval; // fade out
	}

	SetOpacity(alpha);

	fTimeElapsed += dt;
}

void SLandingData::SetOpacity(float alpha)
{
	const GLubyte Alpha = 255 * alpha;

	pSpriteLabel->setOpacity(Alpha);

	CCArray *pChilds = pSpriteLabel->getChildren();
	CCObject* pObject = NULL;

	CCARRAY_FOREACH(pChilds, pObject)
	{
		CCNode* pChild = dynamic_cast<CCNode*>(pObject);
		if (pChild)
		{
			if (pChild->getTag() == tagFade)
			{
				((CCSprite*)pChild)->setOpacity(Alpha);
			}
		}
	}

}

//----------------------------------------------------------------------------------------------
CLandingLayer::CLandingLayer()
	: m_pRootScene(0)
{

}

//----------------------------------------------------------------------------------------------
CLandingLayer::~CLandingLayer()
{
	ClearLayers();
}

//----------------------------------------------------------------------------------------------
bool CLandingLayer::init()
{
	if (!CCLayer::init())
	{
		return false;
	}

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	// init layers 0 - 3
	SLandingData *pLandingPage01 = new SLandingData(0.25f, 0.75f);
	pLandingPage01->pSpriteLabel = CCSprite::create("logo.png");
	pLandingPage01->pSpriteLabel->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(pLandingPage01->pSpriteLabel);
	pLandingPage01->SetOpacity(0.f);
	m_QueueLayers.push(pLandingPage01);

	SLandingData *pLandingPage02 = new SLandingData(0.25f, 0.75f);
	pLandingPage02->pSpriteLabel = CCSprite::create("Explosive_Barrel.png");
	pLandingPage02->pSpriteLabel->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	addChild(pLandingPage02->pSpriteLabel);
	pLandingPage02->SetOpacity(0.f);
	m_QueueLayers.push(pLandingPage02);

	SLandingData *pLandingPage03 = new SLandingData(0.25f, 0.75f, true);
	pLandingPage03->pSpriteLabel = CCSprite::create("leveltest_bk.png");
	pLandingPage03->pSpriteLabel->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));

	float FontSize = cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / 480 * 24;

	CCLabelTTF* pLabel = CCLabelTTF::create("Water War", "Arial", FontSize);
	pLabel->setPosition(ccp(pLandingPage03->pSpriteLabel->getContentSize().width / 2,
							pLandingPage03->pSpriteLabel->getContentSize().height - pLabel->getContentSize().height));
	pLandingPage03->pSpriteLabel->addChild(pLabel, 0, tagFade);
	addChild(pLandingPage03->pSpriteLabel);
	pLandingPage03->SetOpacity(0.f);
	m_QueueLayers.push(pLandingPage03);


	setTouchEnabled(true);

	return true;
}

//----------------------------------------------------------------------------------------------
void CLandingLayer::onEnter()
{
	CCLayer::onEnter();

	schedule(schedule_selector(CLandingLayer::tick), 0);
}

//----------------------------------------------------------------------------------------------
void CLandingLayer::tick(float dt)
{
	if (m_QueueLayers.size())
	{
		if (SLandingData *pLanding = m_QueueLayers.front())
		{
			pLanding->Process(dt);

			if (!pLanding->GetHold() && pLanding->IsPlayed())
			{
				delete pLanding;
				m_QueueLayers.pop();
			}
		}
	}
	
	if (!m_QueueLayers.size())
	{
		if (m_pRootScene)
		{
			// 'layer' is an autorelease object
			CMainMenu *pMainMenu = CMainMenu::create();

			// add layer as a child to scene
			m_pRootScene->addChild(pMainMenu);

			m_pRootScene->removeChild(this);
		}
	}
}

//----------------------------------------------------------------------------------------------
void CLandingLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
}

//----------------------------------------------------------------------------------------------
bool CLandingLayer::ccTouchBegan(CCTouch* touch, CCEvent* event)
{
	SwitchToNext();
	return true;
}

//----------------------------------------------------------------------------------------------
void CLandingLayer::SwitchToNext()
{
	if (SLandingData *pLanding = m_QueueLayers.front())
	{
		pLanding->pSpriteLabel->setVisible(false);
		delete pLanding;
		m_QueueLayers.pop();
	}
}

//----------------------------------------------------------------------------------------------
void CLandingLayer::ClearLayers()
{
	while (!m_QueueLayers.empty())
	{
		delete m_QueueLayers.front();
		m_QueueLayers.pop();
	}		
}
