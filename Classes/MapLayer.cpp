//
//  MapLayer.cpp
//  eow
//  游戏层 1 背景层
//  Created by Hector on 13-10-6.
//
//

#include "MapLayer.h"
#include "SimpleAudioEngine.h"
#include "PubBase.h"
#include "SpriteBase.h"
#include "EXZoomController.h"
#include "SpHall.h"
#include "SpArcher.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* MapLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    MapLayer *layer = MapLayer::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MapLayer::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    winSize = CCDirector::sharedDirector()->getWinSize();
    
    this->addBackgroud();
    //加载地图
    tileMap = CCTMXTiledMap::create("images/isometric2.tmx");
    tlBackGroud = tileMap->layerNamed("backgroud");
    tlBackGroud->setVisible(false);
    tileMap->setAnchorPoint(ccp(0,0));
    
    //计算地图缩放比例
    mapScale = winSize.y/(TILE_HEIGHT*MAP_HEIGHT);
    tileMap->setScale(winSize.y/(TILE_HEIGHT*MAP_HEIGHT));
    
    //将地图稍微偏移,对准背景
    float iLeft = winSize.x - mapScale*MAP_WIDTH*TILE_WIDTH;
    float iTop = mapScale*TILE_HEIGHT * MAP_MARGIN;
    mapOffset = ccp(iLeft, iTop);
    tileMap->setPosition(iLeft, iTop);
    bgPic->addChild(tileMap);
    
    //加载拖动和缩放层
    EXZoomController* controller = EXZoomController::controllerWithNode(bgPic);
    controller->setWindowRect(CCRectMake(0, 0, winSize.x, winSize.y));
    CCRect r = bgPic->getTextureRect();
    controller->setBoundingRect(r);
    controller->zoomOutLimit = controller->getOptimalZoomOutLimit();
    controller->zoomInLimit = 1.0/mapScale *20;
    addChild(controller);
    controller->enableWithTouchPriority(0,true);
    
    //划出地图的边界
    /*
    spriteCache->addSpriteFramesWithFile("images/groundTiles.plist", "images/groundTiles.png");
    
    for(int i = 0;i<46;i++)
        for(int j = 0;j<46;j++)
        {
            if(i<3 | j<3 || i>42 || j > 42)
            {
                CCSprite *sp = CCSprite::createWithSpriteFrame(spriteCache->spriteFrameByName("groundTiles2.png"));
                sp->setPosition(tlBackGroud->positionAt(ccp(i,j)));
                tileMap->addChild(sp,1);
            }
                
        }
     */
    
    
    hallArray = new CCArray;
    ArcherArray = CCArray::create();
    //随机一些房子
    for(int i = 1;i<10;i++)
    {
        SpHall *hall = SpHall::createWithLevel(i);
        hall->mapPosition = ccp(CCRANDOM_0_1()* (MAP_WIDTH-2*MAP_MARGIN)+3,CCRANDOM_0_1()* (MAP_HEIGHT-2*MAP_MARGIN)+3);
        hall->sprite->setPosition(tlBackGroud->positionAt(hall->mapPosition));
        tileMap->addChild(hall->sprite,1);
        hallArray->addObject(hall->sprite);
    }
    
    minScale = 1.0;
    maxScale = 1/mapScale;
    //this->setScale(maxScale);
    
    
    
    // 开启计时器
    this->scheduleUpdate();
    this->setTouchEnabled(true);
    //单点触摸
    //CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 1, true);
    return true;
}

void MapLayer::addBackgroud()
{
    // 先加载背景
    CCSpriteFrameCache *spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteCache->addSpriteFramesWithFile("images/Beijing.plist", "images/Beijing.png");
    
    bgPic = CCSprite::createWithSpriteFrameName("beijing.png");
    bgPic->setAnchorPoint(ccp(0,0));
    this->addChild(bgPic);
    
    CCSprite *pubu1 = CCSprite::createWithSpriteFrameName("sequence/pubu1/pubu1_00.png");
    CCArray *animFrames = CCArray::create();
    for(int i=0; i<=12; i++)
    {
        CCAnimationFrame *animationFrame= new CCAnimationFrame();
        animationFrame->initWithSpriteFrame(spriteCache->spriteFrameByName(CCString::createWithFormat("sequence/pubu1/pubu1_%02d.png",i)->getCString()), 1, NULL);
        animFrames->addObject(animationFrame);
    }
    CCAnimation *animation = CCAnimation::create(animFrames, 0.1f);
    CCAnimate *animate = CCAnimate::create(animation);
    CCSequence *seq = CCSequence::create(animate,NULL);
    CCRepeatForever *repeat = CCRepeatForever::create(seq);
    
    pubu1->setPosition(ccp(160,60));
    pubu1->runAction(repeat);
    
    bgPic->addChild(pubu1);
    
    for(int i=0; i<=12; i++)
    {
        CCAnimationFrame *animationFrame= new CCAnimationFrame();
        animationFrame->initWithSpriteFrame(spriteCache->spriteFrameByName(CCString::createWithFormat("sequence/pubu2/pubu2_%02d.png",i)->getCString()), 1, NULL);
        animFrames->addObject(animationFrame);
    }
    
    CCSprite *pubu2 = CCSprite::createWithSpriteFrameName("sequence/pubu2/pubu2_00.png");

    animation = CCAnimation::create(animFrames, 0.1f);
    animate = CCAnimate::create(animation);
    seq = CCSequence::create(animate,NULL);
    repeat = CCRepeatForever::create(seq);
    
    pubu2->setPosition(ccp(800,20));
    pubu2->runAction(repeat);
    
    bgPic->addChild(pubu2);


}

void MapLayer::update(float dt)
{
}


void MapLayer::ccTouchesBegan(cocos2d::CCSet *pTouches,cocos2d::CCEvent *pEvent)
{
    CCLog("ccTouchesBegan count = %i",pTouches->count());
    //单指移动
    if(pTouches->count() == 1)
    {
        
    } //双指缩放
    else if(pTouches->count() == 20)
    {
        int i = 0;
        CCTouch *pCurTouch1;
        CCTouch *pCurTouch2;
        for(CCSetIterator iterTouch = pTouches->begin(); iterTouch != pTouches->end(); iterTouch++)
        {
            if (i == 0)
            {
                pCurTouch1 =  (cocos2d::CCTouch*)(*iterTouch);//获得第一个触摸点
            }
            else if(i == 1)
            {
                pCurTouch2 =  (cocos2d::CCTouch*)(*iterTouch);
                break;
            }
            i++;
        }
    }
    
}

void MapLayer::ccTouchesMoved(cocos2d::CCSet *pTouches,cocos2d::CCEvent *pEvent)
{
    CCLog("ccTouchesMoved count = %i",pTouches->count());

    //单指移动
    if(pTouches->count() == 1)
    {
        
    }
    
    if(pTouches->count() == 20)
    {
          }

    
}


void MapLayer::ccTouchesEnded(cocos2d::CCSet *pTouches,cocos2d::CCEvent *PEvent)
{
    //单指添加小人
    if(pTouches->count() == 1)
    {
        CCTouch *pTouch = (CCTouch *)pTouches->anyObject();
    
        //Touch坐标转化为GL坐标
        CCPoint touchLocation = pTouch->getLocationInView();
        touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
        
        CCPoint oldLocation = pTouch->getPreviousLocationInView();
        oldLocation = CCDirector::sharedDirector()->convertToGL(oldLocation);
        
        CCPoint change = ccpSub(touchLocation,oldLocation);
        if(ccpLength(change)<20)
        {
            SpArcher* Archer = SpArcher::createWithLevel(1);
            Archer->sprite->setPosition(tileMap->convertToNodeSpace(touchLocation));
            tileMap->addChild(Archer->sprite,1);
    
    
            //ArcherArray->addObject(Archer->sprite);
            SpriteBase* hall = findNearestHall(tileMap->convertToNodeSpace(touchLocation));
    
            float duration =ccpDistance(hall->sprite->getPosition(),touchLocation)/MOVESP ;
    
    
            CCMoveTo *move = CCMoveTo::create(duration,hall->sprite->getPosition());
    
            CCCallFuncN *MoveDone = CCCallFuncN::create(this, callfuncN_selector(MapLayer::ArcherMoveFinished));
    
            Archer->sprite->runAction(CCSequence::create(move,MoveDone,NULL));
            
            Archer->sprite->runAction(Archer->getRunLeft());
            
        }
    }
    
}

void MapLayer::ArcherMoveFinished(cocos2d::CCSprite *sender)
{
    CCLog("move ok!");
    CCParticleSystem *system;
    static int aaa= 1;
    switch (aaa) {
        case 1:
            system = CCParticleExplosion::create();
            break;
        case 2:
            system = CCParticleFire::create();
            break;
        case 3:
            system= CCParticleFireworks::create();
            break;
        case 4:
            system = CCParticleFlower::create();
            break;
        case 5:
            system = CCParticleGalaxy::create();
            break;
        case 6:
            system = CCParticleMeteor::create();
            break;
        case 7:
            system = CCParticleRain::create();
            break;
        case 8:
            system = CCParticleSmoke::create();
            break;
        case 9:
            system = CCParticleSnow::create();
            break;
        case 10:
            system = CCParticleSpiral::create();
            break;
        case  11:
            system = CCParticleSun::create();
            break;
        default:
            break;
    }
    
    system->setPosition(sender->getPosition());
    tileMap->addChild(system);
    if(aaa<12)
        aaa++;
}

//获取最近的屋子
SpriteBase* MapLayer::findNearestHall(CCPoint pos)
{
    CCObject *pObj;
    CCSprite *sp;
    int min = INT32_MAX;
    CCPoint des;
    CCARRAY_FOREACH(hallArray, pObj)
    {
        CCSprite *temp = (CCSprite*)pObj;
        int t = ccpDistance(temp->getPosition(),pos);
        if( t<min )
        {
            min = t;
            sp = temp;
        }
    }
    return (SpriteBase*)sp->getUserData();
}





