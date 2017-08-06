#include "NetworkManager.h"
#include <string>

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

void NetworkManager::MakeMap(int sqN)
{
	srand((unsigned int)time(NULL));
	for (int i = 0; i < sqN; i++)
	{
		int type = rand() % 3;
		switch (type)
		{
		default:
			break;
		}
	}
}

void NetworkManager::DrawMap(int sqN)
{
	for (int i = 0; i < sqN; i++)
	{
		cout << "a";
	}
	cout << '\n';
}

void NetworkManager::InputSqN(int* sqN)
{
	const int sqNMax = 80;
	string buffer;

	while (!(*sqN))
	{
		cout << "�v�f������͂��Ă�������>";
		getline(cin, buffer);
		*sqN = atoi(buffer.c_str());
		if (*sqN <= 0 || *sqN > sqNMax)
		{
			cout << "�v�f���͂P�`" << sqNMax << "�͈̔͂ɂ��Ă�������\n\n";
			*sqN = 0;
		}
	}
}

void NetworkManager::InputHost(string* host)
{
	cout << "�z�X�g���܂���IP�A�h���X����͂��Ă������� > ";
	getline(cin, *host);
	if (*host == "")
	{
		cout << "���[�J���}�V���̃A�h���X���g�p���܂�\n";
		*host = "127.0.0.1";
	}
}

void NetworkManager::InputPort(unsigned short* port)
{
	const unsigned short privatePortL = 49152;
	const unsigned short privatePortU = 65535;
	const unsigned short defaultPort = 55555;
	string buffer;

	while (!(*port))
	{
		cout << "�|�[�g�ԍ�(" << privatePortL << "~" << privatePortU << ")����͂��Ă������� > ";
		getline(cin, buffer);
		if (buffer == "")
		{
			cout << "�f�t�H���g�l�F" << defaultPort << "���g�p���܂�\n";
			*port = defaultPort;
		}
		else
		{
			*port = atoi(buffer.c_str());
			if (*port < privatePortL || *port> privatePortU)
			{
				cout << "�|�[�g�ԍ���" << privatePortL << "~" << privatePortU << "�͈̔͂ɂ��Ă�������\n\n";

				*port = 0;
			}
		}
	}
}

string NetworkManager::Encode(int sqN)
{
	stringstream text;

	text.fill('0');
	text << setw(2) << showbase << sqN;
	for (int i = 0; i < sqN; i++)
	{
		text << "a";
	}

	return text.str();
}

int NetworkManager::Decode(char* receiveData)
{
	char numChar[3];

	numChar[0] = receiveData[0];
	numChar[1] = receiveData[1];
	numChar[2] = '\0';

	return atoi(numChar);
}

void NetworkManager::Decode(char* receiveData, int sqN)
{
	for (int i = 0; i < sqN; i++)
	{
		switch (receiveData[2 + i] - '0')
		{
		default :
			break;
		}
	}
}