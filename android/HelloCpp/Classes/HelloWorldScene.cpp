#include "HelloWorldScene.h"
#include "SceneWorld.h"
#include "LandingLayer.h"

using namespace cocos2d;

//----------------------------------------------------------------------------------------------
CCScene* CMainMenu::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    CLandingLayer *layer = CLandingLayer::create();
	layer->SetRootScene(scene);

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

//----------------------------------------------------------------------------------------------
// on "init" you need to initialize your instance
bool CMainMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
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
                                        menu_selector(CMainMenu::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

	// create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    addChild(pMenu, 1);

	CCMenuItemImage *pPlayItem = CCMenuItemImage::create("play_button.png",
		"play_button_selected.png",
		this,
		menu_selector(CMainMenu::menuPlayCallback));

	pPlayItem->setPosition(ccp(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	
	// create menu, it's an autorelease object
	CCMenu* pMenuPlay = CCMenu::create(pPlayItem, NULL);
	pMenuPlay->setPosition(CCPointZero);
	addChild(pMenuPlay, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
	float FontSize = cocos2d::CCEGLView::sharedOpenGLView()->getDesignResolutionSize().width / 480 * 24;

    CCLabelTTF* pLabel = CCLabelTTF::create("H2O", "Arial", FontSize);
    
    // position the label on the center of the screen
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(pLabel, 1);

    // add "HelloWorld" splash screen"
    CCSprite* pSprite = CCSprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(pSprite, 0);
    
    return true;
}

//----------------------------------------------------------------------------------------------
void CMainMenu::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

//----------------------------------------------------------------------------------------------
void CMainMenu::menuPlayCallback(CCObject* pSender)
{
	// create a scene. it's an autorelease object
	CCScene *pScene = new CSceneWorld();

	CCDirector::sharedDirector()->replaceScene(pScene);
}