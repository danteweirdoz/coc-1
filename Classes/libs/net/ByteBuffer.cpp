//
//  ByteBuffer.cpp
//  Coc
//
//  Created by Hector on 13-10-27.
//
//

#include "ByteBuffer.h"
#include "Common.h"
#include <string>

ByteBuffer::ByteBuffer(int capacity)
{
	buffer = new char[capacity];
	position = 0;
	this->capacity = capacity;
	this->limit = capacity;
}


ByteBuffer::ByteBuffer(char* data,int offset,int size)
{
	buffer = new char[size];
	position = 0;
	memcpy(buffer,data+offset,size);
	this->capacity = size;
	this->limit = this->capacity;
    
}

ByteBuffer::~ByteBuffer(){
	SAFE_DELETE_ARRAY(buffer);
}

int ByteBuffer::getRemain()
{
	return limit - position;
}

int ByteBuffer::getLimit()
{
    return limit;
}

int ByteBuffer::getPosition()
{
    return position;
}

void ByteBuffer::setPosition(int p)
{
    if(p > limit)
    {
		printf("error ByteBuffer::setPosition p > limit--------p=%d,limit=%d\n",p,limit);
	}
    position = p;
}

void ByteBuffer::flip()
{
	this->limit = position;
	position = 0;
}

void ByteBuffer::compact()
{
	if(position > 0)
    {
		for(int i = position; i < limit ; i++)
        {
			buffer[i-position] = buffer[i];
		}
	}
	position = limit - position;
	limit = this->capacity;
}

void ByteBuffer::readBegin()
{
	position = 0;
}

void ByteBuffer::clear()
{
    position = 0;
	this->limit = capacity;
}

void ByteBuffer::put(const char* buf,int offset,int len)
{
    if(position + len > capacity)
    {
        printf("error: put data is too long~");
        return;
    }
    memcpy(buffer + position,buf +offset,len);
    position += len;
}

void ByteBuffer::putByte(Byte b)
{
    if(position + 1 > capacity)
    {
        printf("error: putByte is too long~");
        return;
    }
    buffer[position] = b;
    position++;
}


void ByteBuffer::putChar(int n)
{
    if(position+1>capacity)
    {
        printf("error: putChar is too long~");
        return;
    }
    buffer[position] = (char)n;
    position++;
}

void ByteBuffer::putBoolean(bool b)
{
    if( position+1> capacity )
    {
		printf("error putBoolean position+len> limit------------position=%d,len=%d,capacity=%d\n",position,1,capacity);
		return;
	}
	
	if(b)
		buffer[position] =(1&0xff);
	else
		buffer[position]=(0&0xff);
	position+=1;
}

void ByteBuffer::putShort(short n)
{
    if(position + 2 > capacity){
		printf("error -ByteBuffer::putShort---position=%d,len=%d,capacity=%d\n",position,2,capacity);
		return;
		
	}
	for(int i = 0 ; i < 2 ; i++) //占两个字节,高位放在前面
    {
		buffer[position] = (char)((n >> (8 *(1 - i))) & 0xFF);
		position++;
	}
}

void ByteBuffer::putFloat(float n)
{
    if(position + 4 > capacity){
		printf("error -ByteBuffer::putFloat---position=%d,len=%d,capacity=%d\n",position,4,capacity);
		return;
	}
	unsigned char* pBytes;
	pBytes = (unsigned char*)&n;
    
	buffer[position++] = pBytes[3];
	buffer[position++] = pBytes[2];
	buffer[position++] = pBytes[1];
	buffer[position++] = pBytes[0];
}

void ByteBuffer::putInt(int n)
{
    if(position + 4 > capacity){
		printf("error -ByteBuffer::putInt---position=%d,len=%d,capacity=%d\n",position,4,capacity);
		return;
		
	}
	for(int i = 0 ; i < 4 ; i++) //高位放在前面
    {
		buffer[position] = (char)((n >> (8 *(3 - i))) & 0xFF);
		position++;
	}
}

void ByteBuffer::putLong(long long n)
{
    if(position + 8 > capacity){
		printf("error -ByteBuffer::putLong---position=%d,len=%d,capacity=%d\n",position,8,capacity);
		return;
		
	}
	for(int i = 0 ; i < 8 ; i++) //高位放在前面
    {
		buffer[position] = (char)((n >> (8 *(7 - i))) & 0xFF);
		position++;
	}
}

void ByteBuffer::putUTF(const char* str)
{
    short len = strlen(str);
	if(position + 2+len > capacity){
		printf("error ByteBuffer::putUTF----position=%d,len=%d,capacity=%d\n",position,2+len,capacity);
		return;
	}
	putShort(len);
	if( len>0){
		put(str,0,len);
	}
}

void ByteBuffer::putUTF(const string& str)
{
    putUTF(str.c_str());
}

void ByteBuffer::putArray(vector<Byte>& a)
{
    putInt(a.size());
	for(int i=0;i< a.size();++i){
		putByte(a[i]);
	}
}

void ByteBuffer::putArray(vector<bool>& a)
{
    putInt(a.size());
	for(int i=0;i< a.size();++i)
    {
		putBoolean(a[i]);
	}
}

void ByteBuffer::putArray(vector<short>& a)
{
    putInt(a.size());
	for(int i=0;i< a.size();++i){
		putShort(a[i]);
	}
}

void ByteBuffer::putArray(vector<int>& a)
{
    putInt(a.size());
	for(int i=0;i< a.size();++i){
		putInt(a[i]);
	}
}

void ByteBuffer::putArray(vector<long long>& a)
{
    putInt(a.size());
    
	for(int i=0;i< a.size();++i)
    {
		putLong(a[i]);
	}
}
void ByteBuffer::putArray(vector<string>& a)
{
    putInt(a.size());
	
	for(int i=0;i< a.size();++i){
		putUTF(a[i]);
	}
}
//put end

int ByteBuffer::getCapacity()
{
	return this->capacity;
}

char* ByteBuffer::getBuffer()
{
	return buffer;
}

char* ByteBuffer::toByteArray()
{
	char* tmp = new char[position];
	memcpy( tmp,buffer,position);
	return tmp;
}

char ByteBuffer::get(){
	if(position < limit){
		return buffer[position++];
	}
	printf("error ByteBuffer::get() position+1> limit------------position=%d,limit=%d\n",position,limit);
	return 0;
}

Byte ByteBuffer::getByte()
{
	if(position + 1 > limit){
		printf("error ByteBuffer::getByte() position+1> limit------------position=%d,limit=%d\n",position,limit);
		return 0;
	}
	return buffer[position++];
}
bool ByteBuffer::getBoolean(){
	if(position + 1 > limit){
		printf("error ByteBuffer::getBoolean() position+1> limit------------position=%d,limit=%d\n",position,limit);
		return false;
	}
	return buffer[position++]!=0;
}

int readIntFromBuffer(Byte* buffer,int position){
	int rt = 0;
	for(int i = 0 ; i < 4 ; i++){
		rt |=  ((buffer[position] & 0xFF) << (8 *(3 - i)));
		position++;
	}
	return rt;
}

int ByteBuffer::getLength(int offset)
{
    int lengthPos = position+ offset;
    Byte* l = new Byte[4];
    for(int i = 0;i < 4 ;i++)
    {
        l[i] = buffer[lengthPos+i];
    }
    int rt = bytesToInt(l);
    delete []l;
    return rt;
}

void ByteBuffer::getAsBytes(Byte* bytes)
{
    for(int i = 0 ; i < 4 ; i++){
		bytes[i]=  buffer[position];
		position++;
	}
    
}

int ByteBuffer::getInt(){
	if(position + 4 > limit){
		printf("error ByteBuffer::getInt() position+4> limit------------position=%d,limit=%d\n",position,limit);
		return 0;
	}
	int rt = 0;
	for(int i = 0 ; i < 4 ; i++){
		rt |=  ((buffer[position] & 0xFF) << (8 *(3 - i)));
		position++;
	}
	return rt;
}

float ByteBuffer::getFloat(){
	if(position + 4 > limit){
		printf("error ByteBuffer::getFloat() position+4> limit------------position=%d,limit=%d\n",position,limit);
		return 0;
	}
	float floatValue;
	unsigned char* pBytes;
	pBytes = (unsigned char*)&floatValue;
	pBytes[3] = buffer[position++];
	pBytes[2] = buffer[position++];
	pBytes[1] = buffer[position++];
	pBytes[0] = buffer[position++];
	return floatValue;
}

short ByteBuffer::getShort(){
	if(position + 2 > limit){
		printf("error ByteBuffer::getShort() position+2> limit------------position=%d,limit=%d\n",position,limit);
		return 0;
	}
	short n = 0;
	for(int i = 0 ; i < 2 ; i++){
		n |= ((buffer[position] & 0xFF) << (8 *(1 - i)));
		position++;
	}
	return n;
}

long long ByteBuffer::getLong(){
	if( position + 8 > limit ){
		
		
		printf("error ByteBuffer::getLong() position+8> limit------------position=%d,limit=%d\n",position,limit);
		return 0;
		
	}
	long long n = 0;
	for(int i = 0 ; i < 8 ; i++){
		long long tmp = (buffer[position] & 0xFF);
		n  |= (tmp << (8 *(7 - i)));
		
		position++;
	}
	return n;
}
void ByteBuffer::get(char* bytes,int size)
{
	get(bytes,0,size);
}

void ByteBuffer::get(char* bytes,int offset,int len){
	if(position + len > limit){
		memset(bytes+offset, 0, len );
		printf("error ByteBuffer::get(char* bytes,int offset,int len) position+len> limit------------position=%d,len=%d,limit=%d\n",position,len,limit);
		return;
	}
	memcpy(bytes+offset,buffer+position,len);
	position += len;
}

void ByteBuffer::getUTF(string& str){
	short len = getShort();
	if( len>0){
		str.append(buffer+position,len);
		position+=len;
	}else {
		str="";
	}
    
}

string ByteBuffer::getUTF(){
	string str;
	getUTF(str);
	return str;
}

//这个方法只能用于打开的是文本文件的时候
string ByteBuffer::getLine()
{
    if( position>=capacity) return "";
	string line;
	for (int i = position; i<capacity; i++)
    {
		if (buffer[i] == '\n')
        {
			line.append(buffer + position, i - position);
			position = i + 1;
			return line;
		}
	}
	line.append(buffer + position, capacity - position);
	position = capacity +1;
	return line;
}

void ByteBuffer::getArray(vector<Byte>& a)
{
	int size = getInt();
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = get();
	}
	
}
void ByteBuffer::getArray(vector<bool>& a)
{
	int size = getInt();
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getBoolean();
	}
	
}

void ByteBuffer::getArray(vector<short>& a)
{
	int size = getInt();
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getShort();
	}
}
void ByteBuffer::getArray(vector<int>& a)
{
	int size = getInt();
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getInt();
	}
}
void ByteBuffer::getArray(vector<long long>& a)
{
	int size = getInt();
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getLong();
	}
	
}
void ByteBuffer::getArray(vector<string>& a)
{
	int size =getInt();
	getArray(a,size);
}

void ByteBuffer::getArray(vector<Byte>& a,int size)
{
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getByte();
	}
}
void ByteBuffer::getArray(vector<bool>& a,int size)
{
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getBoolean();
	}
}
void ByteBuffer::getArray(vector<short>& a,int size)
{
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getShort();
	}
}
void ByteBuffer::getArray(vector<int>& a,int size)
{
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getInt();
	}
}
void ByteBuffer::getArray(vector<long long>& a,int size)
{
	a.resize(size);
	for(int i=0;i< size;++i){
		a[i] = getLong();
	}
}

void ByteBuffer::getArray(vector<string>& a,int size)
{
	a.resize(size);
	for(int i=0;i< size;++i){
		getUTF(a[i]);
	}
}

