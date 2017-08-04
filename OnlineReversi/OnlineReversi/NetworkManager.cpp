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
		cout << "�\�P�b�g�ʐM�@�\�̏������Ɏ��s���܂����B�v���O�������I�����܂�\n";
		return -1;
	}

	listenSoc = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSoc < 0)
	{
		cout << "�\�P�b�g���I�[�v���o���܂���ł����B�v���O�������I�����܂�\n";
		WSACleanup();
		return -1;
	}

	ZeroMemory(&saddr, sizeof(SOCKADDR_IN));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(portNum);
	saddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(listenSoc, (struct sockaddr *)&saddr, sizeof(saddr)) == SOCKET_ERROR)
	{
		cout << "bind�̃G���[\n";
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
		cout << "�N���C�A���g�̐ڑ���҂��܂�\n";
		cout << "�I������ꍇ��ctrl + c�ŋ����I�ɏI�����Ă�������\n";
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
		cout << inet_ntoa(from.sin_addr) << "���ڑ����Ă��܂���\n";
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
		cout << "�\�P�b�g�ʐM�@�\�̏������Ɏ��s���܂���\n";
		return -1;
	}

	*soc = socket(AF_INET, SOCK_STREAM, 0);
	if (*soc < 0)
	{
		cout << "�\�P�b�g���I�[�v���o���܂���ł���\n";
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
		cout << "�z�X�g���������ł�\n";
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
		cout << "connect�̃G���[\n";
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
		cout << "�����ȃ\�P�b�g�ł�\n";
		return -1;
	}

	int ret;

	shutdown(*soc, SD_BOTH);
	closesocket(*soc);

	ret = WSACleanup();
	if (ret)
	{
		cout << "WSACleanup�̎��s\n";
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