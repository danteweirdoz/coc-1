//
//  Common.cpp
//  Coc
//
//  Created by Hector on 13-10-27.
//
//

#include "Common.h"
#include <stdlib.h>
#include "cocos2d.h"

//字符串转化为16进制字符串
string byteToHexStr(unsigned char *byte_arr, int arr_len)
{
	string  hexstr;
	for (int i=0;i<arr_len;i++)
	{
		char hex1;
		char hex2;
		int value=byte_arr[i];
		int v1=value/16;
		int v2=value % 16;
		
		if (v1>=0&v1<=9)
			hex1=(char)(48+v1);
		else
			hex1=(char)(55+v1);
        
		
		if (v2>=0&&v2<=9)
			hex2=(char)(48+v2);
		else
			hex2=(char)(55+v2);
        
		
		hexstr+= hex1;
		hexstr+= hex2;
		
	}
	return hexstr;
    
}

int bytesToInt(Byte* b)
{
    
    int addr = b[3] & 0xFF;
    addr |= ((b[2] << 8) & 0xFF00);
    addr |= ((b[1] << 16) & 0xFF0000);
    addr |= ((b[0] << 24) & 0xFF000000);
    return addr;
    
}

void intToByte(int i,Byte* b)
{
    b[3] = (Byte) (0xff & i);
    b[2] = (Byte) ((0xff00 & i) >> 8);
    b[1] = (Byte) ((0xff0000 & i) >> 16);
    b[0] = (Byte) ((0xff000000 & i) >> 24);
}


MyLock::MyLock(pthread_mutex_t* _mutex_t)
{
	this->mutex_t = _mutex_t;
	pthread_mutex_lock(mutex_t);
}
MyLock::~MyLock(){
	pthread_mutex_unlock(mutex_t);
}