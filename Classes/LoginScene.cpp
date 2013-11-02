//
//  LoginScene.cpp
//  Coc
//
//  Created by Hector on 13-10-26.
//
//

#include "LoginScene.h"
#include "HSocket.h"
#include "GameData.h"
#include "Json.h"
#include "Messages.h"
#include "PubBase.h"
#include "MainLayer.h"

CCScene * LoginScene::scene()
{
    CCScene * scene=CCScene::create();
    CCLayer * layer=LoginScene::create();
    scene->addChild(layer);
    return scene;
}

bool LoginScene::init()
{
    if (!CCLayer::init()) {
        return false;
    }
    CCSpriteFrameCache *spriteCache = CCSpriteFrameCache::sharedSpriteFrameCache();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    spriteCache->addSpriteFramesWithFile("images/GUI/Loading background.plist");
    CCSprite* bg = CCSprite::createWithSpriteFrameName("coccover.png");
    bg->setPosition(ccp(winSize.width/2,winSize.height/2));
    this->addChild(bg,1);
    
    socket =GameData::shareSocket();
    socket->start();
    
    CCMenuItemImage *itemLogin = CCMenuItemImage::create("extra/login_up.png", "extra/login_down.png", this, menu_selector(LoginScene::sendLoginData));
    itemLogin->setPosition(ccp(240, -80));
    CCMenuItemImage * itemRegister=CCMenuItemImage::create("extra/register_up.png", "extra/register_down.png",this,menu_selector(LoginScene::registers));
    itemRegister->setPosition(ccp(240, -160));
    CCMenu *menu = CCMenu::create(itemLogin,itemRegister,NULL);
    this->addChild(menu,2);

    edtUserName = CCEditBox::create(CCSizeMake(204, 60), CCScale9Sprite::create("extra/login_kong.png"));
    edtUserName->setFontSize(15);
    edtUserName->setPosition(ccp(winSize.width/2 -100,winSize.height/2-70));
    edtUserName->setFontColor(ccc3(255,255,255));
    edtUserName->setReturnType(kKeyboardReturnTypeDone);
    this->addChild(edtUserName,3);
    
    edtPassword = CCEditBox::create(CCSizeMake(204, 60), CCScale9Sprite::create("extra/login_kong.png"));
    edtPassword->setInputFlag(kEditBoxInputFlagPassword);
    edtPassword->setFontSize(15);
    edtPassword->setPosition(ccp(winSize.width/2 -100,winSize.height/2-160));
    edtPassword->setFontColor(ccc3(255,255,255));
    edtPassword->setReturnType(kKeyboardReturnTypeDone);
    this->addChild(edtPassword,3);

    return true;
}

void LoginScene::sendLoginData()
{
    if(strlen(edtUserName->getText())==0 || strlen(edtPassword->getText())==0)
    {
        CCMessageBox(UiLanguage::shareLanguage()->get("login_check"), UiLanguage::shareLanguage()->get("msgbox_notice"));
        return;
    }
    CSJson::Value root;
    CSJson::FastWriter writer;
    root["username"] = edtUserName->getText();
    root["password"] = edtPassword->getText();
    std:string data = writer.write(root);
    Message* msg = new Message(data.c_str(), 1001);
    socket->sendMessage(msg);
    this->schedule(schedule_selector(LoginScene::recvLoginData));
}

void LoginScene::recvLoginData()
{
    Message * msg= (Message *)GameData::shareGameData()->dic->objectForKey(1001);
    if(msg){
        this->unschedule(schedule_selector(LoginScene::recvLoginData));
        GameData::shareGameData()->dic->removeObjectForKey(1001);
        CSJson::Reader reader;
        CSJson::Value root;
        if (reader.parse(msg->getContent(), root))
        {
            int uid = root["uid"].asInt();
            if (uid > 0)
            {
                GameData::shareGameData()->userID=uid;
                CCScene * scene= MainLayer::scene();
                //  this->addChild(scene,10);
                CCDirector::sharedDirector()->replaceScene(scene);
            }
            else
            {
                CCMessageBox(UiLanguage::shareLanguage()->get("password_error"), UiLanguage::shareLanguage()->get("msgbox_notice"));
            }
        }
    }

}

void LoginScene::registers()
{
    
}
