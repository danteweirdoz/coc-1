//
//  SpHall.h
//  eow
//
//  Created by Hector on 13-10-22.
//
//

#ifndef __eow__SpHall__
#define __eow__SpHall__

#include "cocos2d.h"
#include "SpriteBase.h"

class SpHall:public SpriteBase
{
public:
    static SpHall* createWithLevel(int _level);
};

#endif /* defined(__eow__SpHall__) */
