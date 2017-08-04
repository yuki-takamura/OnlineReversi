#include "NetworkManager.h"

using namespace std;

int NetworkManager::ServerStart(unsigned short portNum, SOCKET* soc)
{
	WSADATA wsaData;
	SOCKET listenSoc;
	SOCKADDR_IN saddr, from;
	int fromlen, ret;

	ret = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (ret)
	{
		cout << "ソケット通信機能の初期化に失敗しました。プログラムを終了します\n";
		return -1;
	}

	listenSoc = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSoc < 0)
	{
		cout << "ソケットをオープン出来ませんでした。プログラムを終了します\n";
		WSACleanup();
		return -1;
	}

	ZeroMemory(&saddr, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(portNum);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(listenSoc, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		cout << "bindのエラー\n";
		closesocket(listenSoc);
		WSACleanup();
		return -1;
	}

	if (listen(listenSoc, 0) == SOCKET_ERROR)
	{
		cout << "listen error\n";
		closesocket(listenSoc);
		WSACleanup();
		return -1;
	}
	else
	{
		cout << "クライアントの接続を待ちます\n";
		cout << "終了する場合はctrl + cで強制的に終了してください\n";
	}

	fromlen = sizeof(from);

	*soc = accept(listenSoc, (SOCKADDR *)&from, &fromlen);
	if (*soc == INVALID_SOCKET)
	{
		cout << "accept error\n";
		closesocket(listenSoc);
		WSACleanup();
		return -1;
	}
	else
	{
		cout << inet_ntoa(from.sin_addr) << "が接続してきました\n";
	}
	closesocket(listenSoc);

	return 0;
}

int NetworkManager::ClientStart(string serverName, unsigned short portNum, SOCKET* soc)
{
	WSADATA wsaData;
	HOSTENT* host;
	unsigned int addr;
	SOCKADDR_IN saddr;
	int ret;

	ret = WSAStartup(MAKEWORD(1, 1), &wsaData);
	if (ret)
	{
		cout << "ソケット通信機能の初期化に失敗しました\n";
		return -1;
	}

	*soc = socket(AF_INET, SOCK_STREAM, 0);
	if (*soc < 0)
	{
		cout << "ソケットをオープン出来ませんでした\n";
		WSACleanup();
		return -1;
	}

	host = gethostbyname(serverName.c_str());
	if (!host)
	{
		addr = inet_addr(serverName.c_str());
		host = gethostbyaddr((char*)&addr, 4, AF_INET);
	}
	if (!host)
	{
		cout << "ホスト名が無効です\n";
		closesocket(*soc);
		WSACleanup();
		return -1;
	}

	ZeroMemory(&saddr, sizeof(SOCKADDR_IN));
	saddr.sin_family = host->h_addrtype;
	saddr.sin_port = htons(portNum);
	saddr.sin_addr.s_addr = *((u_long*)host->h_addr);

	if (connect(*soc, (SOCKADDR*)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		cout << "connectのエラー\n";
		closesocket(*soc);
		WSACleanup();
		return -1;
	}

	return 0;
}

int NetworkManager::SocketEnd(SOCKET* soc)
{
	if (*soc < 0)
	{
		cout << "無効なソケットです\n";
		return -1;
	}

	int ret;

	shutdown(*soc, SD_BOTH);
	closesocket(*soc);

	ret = WSACleanup();
	if (ret)
	{
		cout << "WSACleanupの失敗\n";
		return -1;
	}

	return 0;
}

void MakeMap(SquareType* map, int sqN)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < sqN; i++)
	{
		int type = rand() % 3;
		switch (type)
		{
		case 0:
			map[i] = Normal;
			break;
		case 1:
			map[i] = Forward;
		case 2:
			map[i] = Back;
			break;
		default:
			map[i] = Pass;
			break;
		}
	}
}

void DrawMap(SquareType* map, int sqN)
{
	for (int i = 0; i < sqN; i++)
	{
		switch (map[i])
		{
		case Normal:
			cout << '.';
			break;
		case Forward:
			cout << '+';
			break;
		case Back:
			cout << '-';
			break;
		case Pass:
			cout << 'p';
			break;
		}
	}
	cout << '\n';
}