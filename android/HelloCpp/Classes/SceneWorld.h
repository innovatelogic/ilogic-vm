#ifndef __sceneworld_h__
#define __sceneworld_h__

#include "cocos2d.h"

USING_NS_CC;
using namespace std;

//----------------------------------------------------------------------------------------------
class CCLevel : public CCScene
{
public:
	CCLevel();
	virtual ~CCLevel();
};

//----------------------------------------------------------------------------------------------
class CParallax : public CCLayer
{
public:
	CParallax();
	virtual std::string title();

	virtual void onEnter();

protected:
	CCNode*					m_root;
	CCNode*					m_target;
	CCMotionStreak*			m_streak;
protected:
	CCTextureAtlas* m_atlas;
};

//----------------------------------------------------------------------------------------------
class CSceneWorld : public CCScene
{
public:
	CSceneWorld();
	virtual ~CSceneWorld();

	// implement the "static node()" method manually
	CREATE_FUNC(CSceneWorld);

protected:
private:
};


#endif//__sceneworld_h__