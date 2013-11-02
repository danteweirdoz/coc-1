//
//  SpHall.cpp
//  eow
//
//  Created by Hector on 13-10-22.
//
//

#include "SpHall.h"

using namespace cocos2d;

SpHall* SpHall::createWithLevel(int _level)
{
    SpHall* hall = new SpHall();
    CCSpriteFrameCache *spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteCache->addSpriteFramesWithFile("images/hall/s_hall_sprite.plist");
    hall->initData();
    hall->sprite = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("hall_lv%i.png", _level)->getCString());
    hall->sprite->setUserData(hall);
    hall->level = _level;
    hall->bloodDefault = 400+100*hall->level;
    hall->bloodCur = hall->bloodDefault;
    return hall;
}
