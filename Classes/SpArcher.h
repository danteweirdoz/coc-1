//
//  SpArcher.h
//  eow
//
//  Created by Hector on 13-10-22.
//
//

#ifndef __eow__SpArcher__
#define __eow__SpArcher__

#include "SpriteBase.h"
using namespace cocos2d;

class SpArcher:public SpriteBase
{
public:
    static SpArcher* createWithLevel(int _level);
    void initData();
    CCAction* getRunLeft();
private:
    CCAction *m_runLeft;
    CCAction *m_runRight;
};

#endif /* defined(__eow__SpArcher__) */
