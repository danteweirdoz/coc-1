//
//  SpriteBase.h
//  Coc
//  精灵类基类
//  Created by Hector on 13-10-25.
//
//

#ifndef __Coc__SpriteBase__
#define __Coc__SpriteBase__

#include "cocos2d.h"

class SpriteBase
{
public:
    cocos2d::CCSprite *sprite;
    int mapWidth;
    int mapHeight;
    cocos2d::CCPoint mapPosition;
    
    int bloodCur; //当前血量
    int bloodDefault; //默认血量
    int level; // 级别
public:
    static SpriteBase* createWithSprite(cocos2d::CCSprite *sp);
    void initData();
};


#endif /* defined(__Coc__SpriteBase__) */
