//
//  GameData.h
//  Coc
//
//  Created by Hector on 13-10-26.
//
//

#ifndef __Coc__GameData__
#define __Coc__GameData__

#include <iostream>
#include "cocos2d.h"
#include "HSocket.h"
using namespace cocos2d;
class GameData{
public:
    CCDictionary * dic;
    std::string scoredata;
    int userID;
    std::string  str;
    CCDictionary * roleDic;
public:
    GameData();
    ~GameData();
    static GameData * shareGameData();
    static SocketClient* shareSocket();
};


#endif /* defined(__Coc__GameData__) */
