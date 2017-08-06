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

enum CPUType
{
	Server, Client, Other
};

class NetworkManager
{
public :
	int serverStart(unsigned short portNum, SOCKET* soc);
	int clientStart(std::string serverName, unsigned short portNum, SOCKET* soc);
	int socketEnd(SOCKET* soc);

	void inputHost(std::string* host);
	void inputPort(unsigned short* port);

	std::string encode(int sqN);
	int decode(char* receiveData);
};

#endif