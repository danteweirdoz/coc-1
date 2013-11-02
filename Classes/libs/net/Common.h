//
//  Common.h
//  Coc
//  通用单元
//  Created by Hector on 13-10-27.
//
//

#ifndef __Coc__Common__
#define __Coc__Common__

#include "cocos2d.h"

#include "pthread.h"
#include <string>
using namespace std;

typedef unsigned char Byte;

#define SAFE_DELETE_ARRAY( ptr )if (ptr != NULL){delete[] ptr;ptr = NULL;}

//字符串转化为16进制字符串
string byteToHexStr(unsigned char *byte_arr, int arr_len);

//convert method
int bytesToInt(Byte* b);
void intToByte(int i,Byte* b);

//创建互斥量,析构时自动解锁
class MyLock{
	pthread_mutex_t* mutex_t;
public:
	MyLock(pthread_mutex_t* _mutex_t);
	~MyLock();
	
};


#endif /* defined(__Coc__Common__) */
