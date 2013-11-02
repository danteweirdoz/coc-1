//
//  PubBase.cpp
//  Coc
//
//  Created by Hector on 13-10-25.
//
//

#include "PubBase.h"
using namespace cocos2d;

static UiLanguage* _uiLanguage = NULL;

UiLanguage::~UiLanguage()
{
    CC_SAFE_RELEASE(_defaultDic);
    CC_SAFE_RELEASE(_currentDic);
    CC_SAFE_DELETE(_uiLanguage);
}

UiLanguage* UiLanguage::shareLanguage()
{
    if(!_uiLanguage)
    {
        _uiLanguage = new UiLanguage();
        _uiLanguage->init();
        _uiLanguage->setCurLanguage("zh");
    }
    return _uiLanguage;
}

void UiLanguage::init()
{
    _defaultlang = "en";
    _defaultDic = getLanguage(_defaultlang);
}

void UiLanguage::setCurLanguage(const char *lang)
{
    _currentlang = lang;
    if (strcmp(lang,_defaultlang) == 0)
    {
        _currentDic = _defaultDic;
    }
    else
    {
        _currentDic = getLanguage(_currentlang);
    }
}

CCDictionary* UiLanguage::getLanguage(const char *lang)
{
    CCDictionary *dict = NULL;
    if(strcmp(lang,"zh") == 0 )
    {
        dict = CCDictionary::createWithContentsOfFile("lang/zh.xml");
    }
    else if(strcmp(lang,"en") == 0 )
    {
        dict = CCDictionary::createWithContentsOfFile("lang/en.xml");
    }
    dict->retain();
    return dict;
}

const char* UiLanguage::get(const char* name,const char* defaultvalue)
{
    const char* v = _currentDic->valueForKey(name)->getCString();
    if((strlen(v) == 0) && _currentDic != _defaultDic)
    {
        v = _defaultDic->valueForKey(name)->getCString();
    }
    
    if(strlen(v) == 0)
        return defaultvalue;
    else
        return v;
}