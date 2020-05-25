#include "serial.h"

using namespace std;

Serial::Serial()
{
	callbackcount = 0;
	recvdata[0] = 'Z';
	hcomx = NULL;
}

Serial::Serial(const char* portname, int baudrate)
{
	callbackcount = 0;
	recvdata[0] = 'Z';
	hcomx = NULL;
	open(portname, baudrate);
}

Serial::~Serial()
{
	close();
}

void Serial::open(const char* portname, int baudrate)
{
	hcomx = CreateFileA(
		portname,
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		0,
		0);
	if (hcomx == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hcomx);
		cout << "open serial port failed" << endl;
	}
	else
	{
		cout << "open serial port success" << endl;
		SetupComm(hcomx, 4096, 4096);
		COMMTIMEOUTS timeouts;
		timeouts.ReadIntervalTimeout = 1;
		timeouts.ReadTotalTimeoutConstant = 1;
		timeouts.ReadTotalTimeoutMultiplier = 1;
		timeouts.WriteTotalTimeoutConstant = 1;
		timeouts.WriteTotalTimeoutMultiplier = 1;
		SetCommTimeouts(hcomx, &timeouts);

		DCB dcb;
		GetCommState(hcomx, &dcb);
		dcb.BaudRate = baudrate;
		dcb.ByteSize = 8;
		dcb.Parity = NOPARITY;
		dcb.StopBits = ONESTOPBIT;
		SetCommState(hcomx, &dcb);

		PurgeComm(hcomx, PURGE_TXCLEAR | PURGE_RXCLEAR);
	}
}

void Serial::close()
{
	CloseHandle(hcomx);
}

void Serial::send(int id, int num, int time)
{
	cmdmsg cmd;
	DWORD Write_count;
	cmd = dataanalysis(id, num, time);
	WriteFile(hcomx, cmd.data, cmd.len + 2, &Write_count, NULL);
	for (int i = 0; i < cmd.len + 1; i++)
	{
		cout << hex << int(cmd.data[i]) << " ";
	}
	cout << endl;
	delete[]cmd.data;
}

void Serial::sendx(int cmdnum, int* id, int* num, int time)
{
	DWORD Write_count;
	cmdmsg msg;
	stringstream stream;
	string str;
	for (int i = 0; i < cmdnum; i++)
	{
		stream << "#" << id[i] << "P" << num[i] << "T" << time;
	}
	stream >> str;
	char* data = new char[str.length() + 2];
	for (int i = 0; i < str.length() + 1; i++)
	{
		if (str[i] == '#')data[i] = 0x23;
		if (str[i] == '0')data[i] = 0x30;
		if (str[i] == '1')data[i] = 0x31;
		if (str[i] == '2')data[i] = 0x32;
		if (str[i] == '3')data[i] = 0x33;
		if (str[i] == '4')data[i] = 0x34;
		if (str[i] == '5')data[i] = 0x35;
		if (str[i] == '6')data[i] = 0x36;
		if (str[i] == '7')data[i] = 0x37;
		if (str[i] == '8')data[i] = 0x38;
		if (str[i] == '9')data[i] = 0x39;
		if (str[i] == 'P')data[i] = 0x50;
		if (str[i] == 'T')data[i] = 0x54;
	}
	data[str.length()] = 0x0d;
	data[str.length() + 1] = 0x0a;

	msg.data = data;
	msg.len = str.length() + 1;
	WriteFile(hcomx, msg.data, msg.len + 2, &Write_count, NULL);
	for (int i = 0; i < msg.len + 1; i++)
	{
		cout << hex << int(msg.data[i]) << " ";
	}
	cout << endl;
	delete[]msg.data;
}

int Serial::receive(int count)
{
	int i = 0;
	DWORD Read_count = 0;
	ReadFile(hcomx, recvdata, 4096, &Read_count, 0);
	if (recvdata[0] == 'L')callbackcount++;
	recvdata[0] = 'Z';
	if (callbackcount == count)
	{
		i = 1;
		callbackcount = 0;
	}
	return i;
}

cmdmsg dataanalysis(int id, int num, int time)
{
	cmdmsg msg;
	stringstream stream;
	string str;
	stream << "#" << id << "P" << num << "T" << time;
	stream >> str;
	char* data = new char[str.length()+2];
	data[0] = 0x23;
	for (int i = 1; i < str.length()+1; i++)
	{
		if (str[i] == '0')data[i] = 0x30;
		if (str[i] == '1')data[i] = 0x31;
		if (str[i] == '2')data[i] = 0x32;
		if (str[i] == '3')data[i] = 0x33;
		if (str[i] == '4')data[i] = 0x34;
		if (str[i] == '5')data[i] = 0x35;
		if (str[i] == '6')data[i] = 0x36;
		if (str[i] == '7')data[i] = 0x37;
		if (str[i] == '8')data[i] = 0x38;
		if (str[i] == '9')data[i] = 0x39;
		if (str[i] == 'P')data[i] = 0x50;
		if (str[i] == 'T')data[i] = 0x54;
	}
	data[str.length()] = 0x0d;
	data[str.length()+1] = 0x0a;

	msg.data = data;
	msg.len = str.length()+1;

	return msg;
}
