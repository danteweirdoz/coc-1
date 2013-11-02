#ifndef __EOW_MAINLAYER_H__
#define __EOW_MAINLAYER_H__

#include "cocos2d.h"
#include "MapLayer.h"

class MainLayer : public cocos2d::CCLayer
{
public:
    MapLayer *mapLayer;

public:
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();

    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainLayer);
    
    
};

#endif // __EOW_MAINLAYER_H__
