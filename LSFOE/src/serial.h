#pragma once
#ifndef _SERIAL_H

#include <Windows.h>
#include <iostream>
#include <sstream>

typedef struct
{
	char* data = NULL;
	int len = 0;
}cmdmsg;

/*
*@bref: ����ͨ���֧࣬��PC���е������IC��ͨ��
*@author: ColdDewStudio
*@version: 0.0.1
*/
class Serial
{
private:
	int callbackcount;
	char recvdata[1];
	HANDLE hcomx;
public:
	Serial();
	Serial(const char* portname, int baudrate);
	~Serial();

	void open(const char* portname, int baudrate);
	void close(void);
	void send(int id, int num, int time);
	void sendx(int cmdnum, int* id, int* num, int time);
	int receive(int count);
};

cmdmsg dataanalysis(int id, int num, int time);

#endif // !_SERIAL_H
