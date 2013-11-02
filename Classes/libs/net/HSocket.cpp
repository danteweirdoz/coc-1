//
//  HSocket.cpp
//  Coc
//
//  Created by Hector on 13-10-26.
//
//

#include "HSocket.h"
#include "Common.h"
#include "ByteBuffer.h"
#include "AutoReleasePool.h"
#include "Messages.h"
#include "GameData.h"
using namespace cocos2d;

SocketClient::SocketClient(const char* ip, unsigned short port, Byte clientid,
                            Byte serverid):
m_Status(SocketClient_WAIT_CONNECT),
m_cbRecvBuf(1024*60),m_cbSendBuf(1024*60)
{
    pthread_mutex_init (&m_sendqueue_mutex,NULL);
	pthread_mutex_init(&m_recvqueue_mutex,NULL);
    pthread_mutex_init(&m_thread_cond_mutex,NULL);
	pthread_cond_init(&m_threadCond, NULL);
    
	PServerInfo info = new ServerInfo();
    info->ip = ip;
    info->port =port;

    m_servers.push_back(info);
    m_serverindex = -1;
	
    m_Status = SocketClient_WAIT_CONNECT;
    
    m_socket = new ODSocket(); //新建对象
    
	m_bThreadRecvCreated = false;
	m_bThreadSendCreated = false;
}


SocketClient::~SocketClient()
{
    this->stop();
    CC_SAFE_DELETE(m_socket);
    for(int i = 0;i<m_servers.size();i++)
    {
        delete (PServerInfo)m_servers.at(i);
    }
}

bool SocketClient::connect()
{
    bool rt = false;
    for(int i = 0;i<m_servers.size();i++)
    {
        PServerInfo info = m_servers.at(i);
        if( strlen(info->ip) < 1 || info->port == 0) continue;
        
        //创建套接字
        if(!m_socket->Create(AF_INET, SOCK_STREAM,0))
        {
            CCLOG("socket create error!");
            continue;
        }
        
        //连接
        if (!m_socket->Connect(info->ip,info->port))
        {
            m_socket->Close();
            CCLOG("socket connect error,host:%s,ip:%d",info->ip,info->port);
            continue;
        }
        
        //运行接收线程
        if( !m_bThreadRecvCreated )
        {
            if(pthread_create( &pthread_t_receive, NULL,threadReceiveMessage, this)!=0)
            {
                CCLOG("SocketClient::connect():run threadReceiveMessage error!");
                break;
            }
            m_bThreadRecvCreated = true;
        }
        
        m_serverindex = i;
        rt = true;
        break;
    }
    if(!rt)
    {
        m_socket->Close();
        m_serverindex = -1;
    }
    else
    {
        m_Status = SocketClient_OK;
    }
	
	return rt;
}

void SocketClient::reConnect()
{
    MyLock lock(&m_thread_cond_mutex);
    m_socket->Close(); //关闭之前的套接字
    m_Status=SocketClient_WAIT_CONNECT;
    m_bThreadRecvCreated = false;
	m_bThreadSendCreated = false;
    this->start();
}

void* SocketClient::threadReceiveMessage(void *p)
{
	fd_set fdRead;
	
	struct timeval	aTime;
    
	//最大多少秒，连接上收不到数据就提示用户，重新登录
	int maxIdleTimeInSeconds = 60*3;
	
	//最大多少秒，连接上收不到数据就提示用户，选择重连
	int hint2TimeInSeconds = 60;
	
	//多长时间没有收到任何数据，提示用户
	int hintTimeInSeconds = 30;
	
	struct timeval lastHintUserTime;
	struct timeval lastReceiveDataTime;
	struct timeval now;
	
	gettimeofday(&lastReceiveDataTime, NULL);
	lastHintUserTime = lastReceiveDataTime;
	
	SocketClient* this_ = static_cast<SocketClient*>(p) ;
	
	ByteBuffer* recvBuff =new ByteBuffer(1024);
    recvBuff= &this_->m_cbRecvBuf;
    
	while (this_->m_Status != SocketClient_DESTROY)
	{
		if( this_->m_Status != SocketClient_OK) //挂起一段时间，等待连接
        {
			usleep(1000);
			continue;
		}
        
        //每1s查看是否有数据可读
		FD_ZERO(&fdRead);
		FD_SET(SOCKET(*this_->m_socket),&fdRead);
		aTime.tv_sec = 1;
		aTime.tv_usec = 0;
		int ret = select(((SOCKET)*this_->m_socket)+1,&fdRead,NULL,NULL,&aTime);
        
		if (ret == -1 ) // -1 mean error
		{
			if(errno == EINTR)
            {
				printf("服务器中断，do nothing");
			}
            else //系统出错
            {
				this_->m_Status = SocketClient_DESTROY;
                printf("select error, receive thread stop! errno=%d, address=%p\n",errno,this_);
				return ((void *)0);
			}
		}
		else if(ret==0) // 0 mean connect timeout
		{
            
			gettimeofday(&now, NULL);
			
            if(now.tv_sec - lastReceiveDataTime.tv_sec > maxIdleTimeInSeconds && now.tv_sec - lastHintUserTime.tv_sec > hintTimeInSeconds)
            {
                lastHintUserTime = now;
                this_->clearRecvQueue();
                CCLog("SocketClient::threadReceiveMessage:您的网络已经出问题了,请重新登录~");
            }
            else if(now.tv_sec - lastReceiveDataTime.tv_sec > hint2TimeInSeconds && now.tv_sec - lastHintUserTime.tv_sec > hintTimeInSeconds)
            {
                lastHintUserTime = now;
                lastHintUserTime = now;
                this_->clearRecvQueue();
                CCLog("SocketClient::threadReceiveMessage:您的网络已经出问题了！请尝试重连~");
            }
            else if(now.tv_sec - lastReceiveDataTime.tv_sec > hintTimeInSeconds && now.tv_sec - lastHintUserTime.tv_sec > hintTimeInSeconds)
            {
                lastHintUserTime = now;
                lastHintUserTime = now;
                this_->clearRecvQueue();
                CCLog("SocketClient::threadReceiveMessage:您的网络貌似出问题了！");
            }
            else
            {
                lastHintUserTime = now;
                lastReceiveDataTime= now;
            }
		}
		else if (ret > 0) //find have data to read
		{
			if (FD_ISSET((SOCKET)*this_->m_socket,&fdRead))
			{
				int iRetCode = 0;
                //将数据读入缓冲区
				if(recvBuff->getRemain() > 0)
                {
                    iRetCode = this_->m_socket->Recv(recvBuff->getBuffer() + recvBuff->getPosition(),recvBuff->getRemain());
				}
                else //缓冲区满了~~
                {
                    this_->m_Status = SocketClient_DESTROY;
                    printf("SocketClient::threadReceiveMessage：receive buff is full, receive thread stop!\n");
					this_->clearRecvQueue(); //清空已经接收到的数据
					return ((void *)0);
                }
                
				if (iRetCode == -1) //结果没有读取到数据
				{
                    printf("SocketClient::threadReceiveMessage:网络中断！\n");
					this_->m_Status = SocketClient_DESTROY; //线程停止
                    
                    this_->clearRecvQueue(); //清空已经接收到的数据
                    this_->reConnect(); //线程重新运行
                    
					return ((void *)0);
				}
                else if(iRetCode == 0 && recvBuff->getRemain() > 0) //server 端关闭
                {
                    if(errno == 0) //服务器正常关闭连接
                    {
                        FD_ZERO(&fdRead);
                        this_->m_socket->reConnect();
                        continue;
                        //this_->m_socket->reConnect();
                    }
                    else
                    {
                        this_->m_Status = SocketClient_DESTROY;
                        printf("SocketClient::threadReceiveMessage：server closed connection, receive thread stop!errorno:%d \n",errno);
                        this_->clearRecvQueue(); //清空已经接收到的数据
                        return ((void *)0);
                    }
				}
				else
				{
					gettimeofday(&lastReceiveDataTime, NULL);
					
					recvBuff->setPosition(recvBuff->getPosition()+ iRetCode);
					recvBuff->flip();
					int tmpOffset = 17;
					while(recvBuff->getRemain() > tmpOffset) // 正常接收到数据
                    {
						int pos = recvBuff->getPosition();
						int length= recvBuff->getLength(9);
						
						if(recvBuff->getRemain() +tmpOffset >= length) //已经全部收到
                        {
                            Message *msg = new Message(recvBuff);
                            {
                                MyLock lock(&this_->m_sendqueue_mutex);
                                this_->m_receivedMessageQueue.push(msg);
                                GameData::shareGameData()->dic->setObject(msg, msg->getCommand());
                            }
						}
                        else if(length > recvBuff->getCapacity()) //包太大，缓冲区存放不小下
                        {
							this_->m_Status = SocketClient_DESTROY;
                            CCLog("receive error message size(%d) greate capacity(%d), receive thread stop!\n",length,recvBuff->getCapacity());
							this_->clearRecvQueue(); //清空
							return ((void *)0);
						}
                        else //没有接收完，继续接收
                        {
                            recvBuff->setPosition(pos);
							break;
						}
					}
					//
					recvBuff->compact();
				}
				
			}//end read
		}
        
	}
    
	return (void*)0;
}

void* SocketClient::threadSendMessage(void *p)
{
    SocketClient* this_ = static_cast<SocketClient*>(p) ;
    
    // 连接
	if(this_->m_Status == SocketClient_WAIT_CONNECT && !this_->connect())
    {
		CCLog("SocketClient::threadSendMessage:Connot connect server!");
        return ((void *)0);
	}
	
    ByteBuffer* sendBuff = &this_->m_cbSendBuf;
    
	while (this_->m_Status != SocketClient_DESTROY)
    {
		if( this_->m_Status == SocketClient_OK)
        {
            //发送缓冲器有遗留的数据要发送
            if(sendBuff->getPosition() > 0)
            {
                sendBuff->flip(); //
                int ret = this_->m_socket->Send(sendBuff->getBuffer(), sendBuff->getLimit());
                if(-1 == ret) //stop send and send error to server
                {
                    this_->m_Status = SocketClient_DESTROY ;
                    CCLog("发送数据，网络异常！");
                    return ((void *)0);
                }
                sendBuff->setPosition(sendBuff->getPosition()+ret);
                sendBuff->compact();
            }
            
            //循环发送消息队列的消息
            Message* msg = NULL;
            while( this_->m_Status != SocketClient_DESTROY && this_->m_sendMessageQueue.size()> 0)
            {
                //读取消息
                {
                    MyLock lock(&this_->m_sendqueue_mutex);
                    msg = this_->m_sendMessageQueue.front();
                    this_->m_sendMessageQueue.pop();
                }
                
                if(msg->dataLength() + sendBuff->getPosition() > sendBuff->getLimit())
                {
                    this_->m_Status = SocketClient_DESTROY;
                    printf("send buffer is full, send thread stop!"); //发送太慢,或者发送包太大，网络环境有问题
                    return ((void *)0);
                }
                //添加到缓冲区，并发送
                //printf("send data:%s",msg->getContent());
                sendBuff->put(msg->getData(),0,msg->dataLength());
                sendBuff->flip();
                int ret = this_->m_socket->Send(sendBuff->getBuffer(),sendBuff->getLimit(),0);
                if(ret == -1){
                    this_->m_Status = SocketClient_DESTROY;
                    CCLog("发送数据，网络异常！");
                    return ((void *)0);
                }
                sendBuff->setPosition(sendBuff->getPosition()+ret);
                sendBuff->compact();
                
                delete msg;
            }
		}
		
        //等待5s 或者被唤醒 继续发送
        if(this_->m_Status != SocketClient_DESTROY && this_->m_sendMessageQueue.size() == 0)
        {
			//sleep
			struct timeval tv;
			struct timespec ts;
			gettimeofday(&tv, NULL);
            
            //如果缓冲区还有数据没有发送完，则 1s 后继续发送
            if(this_->m_Status == SocketClient_OK && sendBuff->getPosition() > 0)
            {
                ts.tv_sec = tv.tv_sec;
                ts.tv_nsec = 0;
            }
            else //或者等5s自动后苏醒
            {
                ts.tv_sec = tv.tv_sec + 5;
                ts.tv_nsec = 0;
            }
			
			MyLock lock(&(this_->m_thread_cond_mutex));
			if(this_->m_Status != SocketClient_DESTROY && this_->m_sendMessageQueue.size() == 0)
            {
				pthread_cond_timedwait(&(this_->m_threadCond),&(this_->m_thread_cond_mutex),&ts);
			}
		}
		
	}
    
	return (void*)0;

}

void SocketClient::clearRecvQueue()
{
    MyLock lock(&m_recvqueue_mutex);
    while( m_receivedMessageQueue.size()>0)
    {
        Message* msg = m_receivedMessageQueue.front();
        m_receivedMessageQueue.pop();
        delete msg;
    }
}

void SocketClient::clearSendQueue()
{
    MyLock lock(&m_sendqueue_mutex);
    while( m_sendMessageQueue.size()>0)
    {
        Message* msg = m_sendMessageQueue.front();
        m_sendMessageQueue.pop();
        delete msg;
    }
}


void SocketClient::sendMessage(Message* msg)
{
	if(m_Status == SocketClient_DESTROY)
    {
        CCLog("SocketClient::sendMessage  发送队列没有正常运行！");
		delete msg;
		return;
	}
    
    {
        MyLock lock(&m_sendqueue_mutex); // 加发送队列锁
        m_sendMessageQueue.push(msg); //加入发送队列
    }
    
	if( m_Status == SocketClient_OK)
	{
		MyLock lock(&m_thread_cond_mutex); //加锁
		pthread_cond_signal(&m_threadCond); //如果发送线程在等待,则唤醒,自动发送
	}
	
}

void SocketClient::start()
{
	if(!m_bThreadSendCreated)
    {
		pthread_create( &pthread_t_send, NULL,threadSendMessage, this);
		m_bThreadSendCreated = true;
	}
}

void SocketClient:: stop()
{
	m_Status = SocketClient_DESTROY;
	
	{
		MyLock lock(&m_thread_cond_mutex); //发送锁
		pthread_cond_signal(&m_threadCond); //发送信号，让其发送完成
	}
    
    pthread_join(pthread_t_receive, NULL); //等待接收线程完成释放资源
	pthread_join(pthread_t_send, NULL);  //等待发送线程完成释放资源
    
    
    //释放所有的锁
    pthread_mutex_destroy(&m_sendqueue_mutex);
    pthread_mutex_destroy(&m_recvqueue_mutex);
	pthread_mutex_destroy(&m_thread_cond_mutex);
	pthread_cond_destroy(&m_threadCond);
    
	//清空接收的消息
	while (!m_receivedMessageQueue.empty())
    {
		Message* m = m_receivedMessageQueue.front();
		m_receivedMessageQueue.pop();
		CC_SAFE_DELETE(m);
	}
	
    //清空发送队列
	while (!m_sendMessageQueue.empty())
    {
		Message* m = m_sendMessageQueue.front();
		m_sendMessageQueue.pop();
		CC_SAFE_DELETE(m);
	}
    m_socket->Close();//关闭套接字
}
