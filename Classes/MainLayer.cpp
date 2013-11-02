/*
 图层说明
 游戏进入层
    1 游戏层(包括背景和地图)
    2 控制层
 */
#include "MainLayer.h"
#include "SimpleAudioEngine.h"
#include "PubBase.h"
#include "MapLayer.h"
#include "PubBase.h"
#include "LoginScene.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MainLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MainLayer *layer = MainLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCScene* psence = MapLayer::scene();
    
    psence->setAnchorPoint(ccp(0,0));
    
    this->addChild(psence,1);
    
    this->setTouchEnabled(true);
    

    return true;
}


void MainLayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
