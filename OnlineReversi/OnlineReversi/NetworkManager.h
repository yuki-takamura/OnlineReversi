#ifndef ___Class_NetworkManager
#define ___Class_NetworkManager

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <string>
#include <iomanip>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <winsock2.h>

enum SquareType
{
	Normal, Forward, Back, Pass
};

enum CPUType
{
	Server, Client, Other
};

class NetworkManager
{
public :
	int ServerStart(unsigned short portNum, SOCKET* soc);
	int ClientStart(std::string serverName, unsigned short portNum, SOCKET* soc);
	int SocketEnd(SOCKET* soc);

	void MakeMap(SquareType* map, int sqN);
	void DrawMap(SquareType* map, int sqN);

	void InputSqN(int* sqN);
	void InputHost(std::string* host);
	void InputPort(unsigned short* port);

	std::string Encode(SquareType* map, int sqN);
	int Decode(char* receiveData);
	void Decode(char* receiveData, SquareType* map, int sqN);
};

#endif