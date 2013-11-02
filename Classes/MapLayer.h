//
//  MapLayer.h
//  eow
//
//  Created by Hector on 13-10-6.
//
//

#ifndef __eow__MapLayer__
#define __eow__MapLayer__

#define MOVESP 200

#define kEXZoomControllerHistoryCount 12

#include "cocos2d.h"
#include "SpriteBase.h"
USING_NS_CC;

class MapLayer : public cocos2d::CCLayer
{
public:
    cocos2d::CCSprite *bgPic;
    float minScale;
    float maxScale;
    float mapScale;
    CCPoint mapOffset;
    
    CCTMXTiledMap *tileMap;
    CCTMXLayer *tlBackGroud;
    
    cocos2d::CCArray *hallArray;
    cocos2d::CCArray *ArcherArray;


private:
    CCPoint winSize;

public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    void update(float dt);
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    /*
    bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
	void ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    */
    
    virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
    virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MapLayer);
    
    void addBackgroud();
    
public:
    //找到屏幕坐标最近的房子
    SpriteBase* findNearestHall(cocos2d::CCPoint mapPosition);
    
    void ArcherMoveFinished(cocos2d::CCSprite *sender);
    cocos2d::CCPoint SetLayerPosition(cocos2d::CCPoint mapPosition);
    //void SetLayerScale(float scale);
};



#endif /* defined(__eow__MapLayer__) */
