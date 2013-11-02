//
//  HSocket.h
//  Coc
//  封装Socket线程
//  Created by Hector on 13-10-26.
//
//

#ifndef __Coc__HSocket__
#define __Coc__HSocket__

#include <iostream>
#include <string.h>
#include <queue>

#include "cocos2d.h"
#include "ODSocket.h"
#include "Common.h"
#include "ByteBuffer.h"
using namespace std;

class Message;
class MessageQueue;

//客户端状态
enum SocketClientStatus {
    SocketClient_WAIT_CONNECT, // 等待连接
    SocketClient_OK,            // 正常
    SocketClient_DESTROY        //丢失
};

typedef struct ServerInfo {
    const char* ip;
    unsigned short port;
} *PServerInfo;

class SocketClient
{
public:
    ODSocket* m_socket;
    
    vector<PServerInfo> m_servers; //服务器列表
    int m_serverindex;            //服务器位置
  
    //客户端状态
    SocketClientStatus m_Status;
    
    queue<Message*> m_sendMessageQueue; //消息发送队列
    pthread_mutex_t m_sendqueue_mutex;//发送队列 锁,访问时加锁
    
	queue<Message*> m_receivedMessageQueue;//消息接收队列
    pthread_mutex_t m_recvqueue_mutex;//接收队列 锁,访问时加锁
	
    
    //发送和接收缓冲区，发送缓冲区满的时候，会断开连接，并提示信号不好
	ByteBuffer m_cbRecvBuf;
	ByteBuffer m_cbSendBuf;
    
    //send thread
	bool m_bThreadSendCreated; // 是否创建线程
    pthread_t pthread_t_send;
    
    //receive thread
	bool m_bThreadRecvCreated; // 是否创建接收线程
	pthread_t pthread_t_receive;

    //用来控制线程运行的锁，如计时，唤醒等
    pthread_mutex_t m_thread_cond_mutex;//互斥锁
	pthread_cond_t m_threadCond;
    
    
public:
    SocketClient(const char* ip, unsigned short port, Byte clientId,
				 Byte serverId);

    ~SocketClient();
    void sendMessage(Message* msg);//发送消息
    void stop();
    void start();  //create send thread
    void reConnect();
private:
	//连接服务器
	bool  connect();
    static void* threadReceiveMessage(void *p); //
	static void* threadSendMessage(void *p);
    void clearRecvQueue();
    void clearSendQueue();
};

#endif /* defined(__Coc__HSocket__) */
