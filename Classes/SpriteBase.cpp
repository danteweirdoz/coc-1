//
//  SpriteBase.cpp
//  Coc
//
//  Created by Hector on 13-10-25.
//
//

#include "SpriteBase.h"

using namespace cocos2d;

SpriteBase* SpriteBase::createWithSprite(cocos2d::CCSprite *sp)
{
    SpriteBase *eowSp = new SpriteBase();
    eowSp->initData();
    eowSp->sprite = sp;
    eowSp->sprite->setUserData(eowSp);
    return eowSp;
}

void SpriteBase::initData()
{
    sprite = NULL;
    mapWidth = 0;
    mapHeight = 0;
    mapPosition = ccp(0, 0);
    bloodCur =0;
    bloodDefault =0;
    level = 0;
}