//
//  SpArcher.cpp
//  eow
//
//  Created by Hector on 13-10-22.
//
//

#include "SpArcher.h"

using namespace cocos2d;

SpArcher* SpArcher::createWithLevel(int _level)
{
    SpArcher* hall = new SpArcher();
    CCSpriteFrameCache *spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    spriteCache->addSpriteFramesWithFile("animation/Archer.plist");
    hall->initData();
    hall->sprite = CCSprite::createWithSpriteFrameName("walk/archer1_run_0/unitshadow.png");
    //hall->sprite = CCSprite::createWithSpriteFrameName("walk/archer1_run_0/archer1_run_0_0.png");
    hall->sprite->setUserData(hall);
    hall->level = _level;
    hall->bloodDefault = 40+10*hall->level;
    hall->bloodCur = hall->bloodDefault;
    return hall;
}

void SpArcher::initData()
{
    this->SpriteBase::initData(); //调用父类的初始化函数
    m_runLeft = NULL;
    m_runRight = NULL;
}

CCAction* SpArcher::getRunLeft()
{
    if(!m_runLeft)
    {
        CCSpriteFrameCache *spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
        
        CCArray *animFrames = CCArray::create();
        for(int i=0; i<8; i++)
        {
            CCAnimationFrame *animationFrame= new CCAnimationFrame();
            animationFrame->initWithSpriteFrame(spriteCache->spriteFrameByName(CCString::createWithFormat("walk/archer1_run_0/archer1_run_0_%i.png",i)->getCString()), 1, NULL);
            animFrames->addObject(animationFrame);
        }
        CCAnimation *animation = CCAnimation::create(animFrames, 0.1f);
        CCAnimate *animate = CCAnimate::create(animation);
        CCSequence *seq = CCSequence::create(animate,NULL);
        m_runLeft = CCRepeatForever::create(seq);
    }
    return  m_runLeft;
}