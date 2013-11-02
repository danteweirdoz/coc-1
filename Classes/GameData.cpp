//
//  GameData.cpp
//  Coc
//
//  Created by Hector on 13-10-26.
//
//

#include "GameData.h"
#include "HSocket.h"

static GameData * game=NULL;

GameData::GameData(){
    roleDic=CCDictionary::create();
    roleDic->retain();
    dic=CCDictionary::create();
    dic->retain();
    
}
GameData::~GameData(){
    
}
GameData * GameData::shareGameData(){
    if(!game){
        game=new GameData();
    }
    return game;
}

static SocketClient* _socketclient = NULL;
SocketClient* GameData::shareSocket()
{
    if(!_socketclient)
    {
        _socketclient = new SocketClient("192.168.1.113", 11008, 1, 1);
    }
    return _socketclient;
}