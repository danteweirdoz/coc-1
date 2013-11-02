//
//  Messages.cpp
//  Coc
//
//  Created by Hector on 13-10-27.
//
//

#include "Messages.h"

Message::~Message()
{
    if (m_data!=NULL)
    {
		delete[] m_data;
	}
}

Message::Message(const char* pdata,int commandId)
{
    init();
    intToByte(strlen(pdata),this->m_contentLenght);
    intToByte(commandId, this->m_command); //功能号
    
    this->m_data = new char[Message::headLength()+this->bodyLength()];
    memcpy(this->m_data+0,&this->m_head,4);
    memcpy(this->m_data+4,&this->m_protoVersion,1);
    memcpy(this->m_data+5,&this->m_serverVersion,4);
    memcpy(this->m_data+9,&this->m_contentLenght,4);
    memcpy(this->m_data+13,&this->m_command,4);
    memcpy(this->m_data+17,pdata,this->contentLength());
}

Message::Message(ByteBuffer* buff)
{
    int pos = buff->getPosition();
    buff->getAsBytes(this->m_head);
    this->m_protoVersion = buff->getByte();
    buff->getAsBytes(this->m_serverVersion);
    buff->getAsBytes(this->m_contentLenght);
    buff->getAsBytes(this->m_command);
    //printf("data:%s",buff->getBuffer() + 17);
    this->m_data = new char[this->dataLength()];
    buff->setPosition(pos);
    buff->get(this->m_data,0,this->dataLength());
}

void Message::init()
{
    this->m_head[0]=78;
    this->m_head[1]=37;
    this->m_head[2]=38;
    this->m_head[3]=48;
    this->m_protoVersion=9;
    intToByte(0, this->m_serverVersion); //服务器版本
    
    this->m_data =NULL;
}

int Message::dataLength()   // 数据总长度，包括数据
{
    // 13位文件头+内容
    return Message::headLength() + bodyLength();
}

int Message::headLength()   // 文件头长度
{
    return 13;
}

int Message::contentLength()   // 内容长度
{
    return bytesToInt(this->m_contentLenght);
}

int Message::bodyLength()   // 正文长度
{
    return this->contentLength() + 4; // 加上4位功能号
}

const char* Message::getData()    // 数据
{
    return m_data;
}

const char* Message::getContent() // 内容 不包含功能号的
{
    return m_data + Message::headLength() +4 ; //4为功能号长度
}

int Message::getCommand()   // 功能号
{
    return bytesToInt(this->m_command);
}
