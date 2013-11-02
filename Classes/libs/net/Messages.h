//
//  Messages.h
//  Coc
//
//  Created by Hector on 13-10-27.
//
//

#ifndef __Coc__Messages__
#define __Coc__Messages__

#include <iostream>
#include "Common.h"
#include "HSocket.h"
using namespace cocos2d;

class ByteBuffer;
//消息类
class Message:public CCObject
{
private:
    //文件头开始，包按照此顺序排列
    Byte m_head[4];           //自定义四位文件头,避免粘包
    Byte m_protoVersion;      //版本
    Byte m_serverVersion[4];      //服务器版本
    Byte m_contentLenght[4];         //数据内容长度
    
    //body开始，功能号+数据
    Byte m_command[4]; //功能号
    char* m_data;         //消息内容,包括文件头
public:
    Message(const char* pdata,int commandId);
    Message(ByteBuffer* buff); //会更改buff的position
	~Message();
    
    int dataLength();   // 数据总长度，包括数据
    static int headLength();   // 文件头长度
    int bodyLength();   // 正文长度
    int contentLength();   // 内容长度
    const char* getData();    // 数据
    const char* getContent(); // 内容 不包含功能号的
    int getCommand();   // 功能号
private:
    //初始化文件头等
    void init();
};


#endif /* defined(__Coc__Messages__) */
