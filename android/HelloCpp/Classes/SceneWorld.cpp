#include "SceneWorld.h"

using namespace cocos2d;


//----------------------------------------------------------------------------------------------
CCLevel::CCLevel()
{

}

//----------------------------------------------------------------------------------------------
CCLevel::~CCLevel()
{

}

//----------------------------------------------------------------------------------------------
CParallax::CParallax()
{
	// Top Layer, a simple image
	CCSprite *pBackgroundImage = CCSprite::create("leveltest_bk.png");

	// scale the image (optional)
	pBackgroundImage->setScale( 1.f );

	// change the transform anchor point to 0,0 (optional)
	pBackgroundImage->setAnchorPoint(ccp(0, 0));

	// Top Layer, a simple image
	CCSprite* barrelImage = CCSprite::create("Explosive_Barrel.png");
	// scale the image (optional)
	barrelImage->setScale( 0.5f );
	// change the transform anchor point to 0,0 (optional)
	barrelImage->setAnchorPoint( ccp(0, 0) );
	
	// create a void node, a parent node
	CCParallaxNode* voidNode = CCParallaxNode::create();

	// top image is moved at a ratio of 3.0x, 2.5y
	voidNode->addChild(pBackgroundImage, -1, ccp(1.4f, 1.5f), ccp(0,0));

	// top image is moved at a ratio of 3.0x, 2.5y
	voidNode->addChild(barrelImage, 2, ccp(3.0f, 2.5f), ccp(20, 0) );
	
	CCActionInterval* goR = CCMoveBy::create(4, ccp(400, 0));
	CCActionInterval* goL = goR->reverse();
	//CCActionInterval* go = CCMoveBy::create(8, ccp(-1000,0) );
	//CCActionInterval* goBack = go->reverse();
	CCSequence* seq = CCSequence::create(goR, goL, NULL, NULL, NULL);
	voidNode->runAction( (CCRepeatForever::create(seq) ));

	addChild(voidNode);

	CCSprite *wpnimage = CCSprite::create("syringes01.png");
	wpnimage->setScale( 1.5f );
	wpnimage->setAnchorPoint( ccp(0.f,0.f) );
	wpnimage->setPosition(ccp(-120, -120));
	addChild(wpnimage, 1);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		this,
		/*menu_selector(HelloWorld::menuCloseCallback)*/0);

	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
		origin.y + pCloseItem->getContentSize().height/2));

	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
	pMenu->setPosition(CCPointZero);
	addChild(pMenu, 1);

}

//----------------------------------------------------------------------------------------------
void CParallax::onEnter()
{
	CCLayer::onEnter();

	CCSize s = CCDirector::sharedDirector()->getWinSize();
/*
	CCLabelTTF* label = CCLabelTTF::create(title().c_str(), "Arial", 28);
	addChild(label, 1);
	label->setPosition( ccp(s.width/2, s.height-50) );

	CCMenuItemImage *item1 = CCMenuItemImage::create(s_pPathB1, s_pPathB2, this, menu_selector(ParallaxDemo::backCallback) );
	CCMenuItemImage *item2 = CCMenuItemImage::create(s_pPathR1, s_pPathR2, this, menu_selector(ParallaxDemo::restartCallback) );
	CCMenuItemImage *item3 = CCMenuItemImage::create(s_pPathF1, s_pPathF2, this, menu_selector(ParallaxDemo::nextCallback) );

	CCMenu *menu = CCMenu::create(item1, item2, item3, NULL);

	menu->setPosition( CCPointZero );
	item1->setPosition(ccp(VisibleRect::center().x - item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));
	item2->setPosition(ccp(VisibleRect::center().x, VisibleRect::bottom().y+item2->getContentSize().height/2));
	item3->setPosition(ccp(VisibleRect::center().x + item2->getContentSize().width*2, VisibleRect::bottom().y+item2->getContentSize().height/2));

	addChild(menu, 1);  */  
}
//----------------------------------------------------------------------------------------------
std::string CParallax::title()
{
	return "Scene";
}

//----------------------------------------------------------------------------------------------
CSceneWorld::CSceneWorld()
{
	CCScene::init();

	CCLayer* pLayer = new CParallax();

	addChild(pLayer);
}

//----------------------------------------------------------------------------------------------
CSceneWorld::~CSceneWorld()
{

}