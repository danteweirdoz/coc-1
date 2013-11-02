//
//  LoginScene.h
//  Coc
//
//  Created by Hector on 13-10-26.
//
//

#ifndef __Coc__LoginScene__
#define __Coc__LoginScene__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "HSocket.h"

using namespace cocos2d;
using namespace extension;

class LoginScene:public CCLayer
{
public:
    SocketClient * socket;
    CCEditBox* edtUserName;
    CCEditBox* edtPassword;
public:
    virtual bool init();
    static CCScene* scene();
    CREATE_FUNC(LoginScene);
    
    void sendLoginData();
    void recvLoginData();
    void registers();
};

#endif /* defined(__Coc__LoginScene__) */
