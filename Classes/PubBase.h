//
//  PubBase.h
//  Coc
//
//  公共资源引用
//  Created by Hector on 13-10-25.
//
//

#ifndef __Coc__PubBase__
#define __Coc__PubBase__

//Tile 大小
#define TILE_WIDTH 57
#define TILE_HEIGHT 42

//Map
#define MAP_WIDTH 46
#define MAP_HEIGHT 46
#define MAP_MARGIN 3

#include "cocos2d.h"
#include "HSocket.h"
using namespace cocos2d;

/*
 实现自定义多语言功能
 */

class UiLanguage
{
private:
    CCDictionary* _defaultDic;
    CCDictionary* _currentDic;
    const char* _defaultlang;  // 默认语言
    const char* _currentlang;  // 当前语言
    
public:
    ~UiLanguage();
    static UiLanguage* shareLanguage();
    void init();
    void setCurLanguage(const char *lang);
    const char* get(const char* name,const char* defaultvalue = "");
private:
    CCDictionary* getLanguage(const char *lang);
    
};





#endif /* defined(__Coc__PubBase__) */
