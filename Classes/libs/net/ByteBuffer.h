//
//  ByteBuffer.h
//  Coc
//
//  Created by Hector on 13-10-27.
//
//

#ifndef __Coc__ByteBuffer__
#define __Coc__ByteBuffer__

#include <iostream>
#include "Common.h"

class ByteBuffer
{
private:
	char* buffer; //data
	int position;//read position
	int capacity; // datalength
	int limit;  //content max limit
public:
	
	ByteBuffer(int capacity); //set datalength
	ByteBuffer(char* data,int offset,int length); //create by data
	~ByteBuffer();
    
    int getLength(int offset=0);
	int getRemain(); //remain space
    int getLimit();
    int getCapacity();
    char* getBuffer();
    
    int getPosition();
    void setPosition(int p);
    
    void flip(); //set limit to position postion and set position to zero
    void compact(); // move the data after postion from 0,and set postion to the datalength and reset right limit
    void readBegin(); //reset position at 0
    void clear(); //do nothing
	
	char* toByteArray();
    
    void put(const char* buf,int offset,int len);
    
    void putByte(Byte b);
    void putChar(int n);
	void putBoolean(bool b);
	void putShort(short n);
	void putFloat(float n);
	void putInt(int n);
	void putLong(long long n);
	void putUTF(const char* str);
	void putUTF(const std::string& str);
    //every vector contain a size (short)
	void putArray(vector<Byte>& a);
	void putArray(vector<bool>& a);
	void putArray(vector<short>& a);
	void putArray(vector<int>& a);
	void putArray(vector<long long>& a);
	void putArray(vector<string>& a);
    
    //every read opreation will increase position
    char get();
    Byte getByte();
	bool getBoolean();
	int getInt();
	float getFloat();
	short getShort();
	void get(char* bytes,int size);
	void get(char* bytes,int offset,int len);
    string getLine(); //just for read text file
	long long getLong();
    void getAsBytes(Byte* bytes);
    void getUTF(string& str);
	string getUTF();
    
    void getArray(vector<Byte>& a);
	void getArray(vector<bool>& a);
	void getArray(vector<short>& a);
	void getArray(vector<int>& a);
	void getArray(vector<long long>& a);
	void getArray(vector<string>& a);
    
    void getArray(vector<Byte>& a,int len);
	void getArray(vector<bool>& a,int len);
	void getArray(vector<short>& a,int len);
	void getArray(vector<int>& a,int len);
	void getArray(vector<long long>& a,int len);
	void getArray(vector<string>& a,int len);
};


#endif /* defined(__Coc__ByteBuffer__) */
