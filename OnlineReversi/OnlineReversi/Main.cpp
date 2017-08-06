#include "NetworkManager.h"

using namespace std;

int main()
{
	NetworkManager networkManager;

	int sqN = 0;
	SquareType* sq = NULL;
	CPUType cpu = Other;

	string inputBuffer;

	string message;
	char receiveBuffer[256];

	string host;
	unsigned short port = 0;
	SOCKET soc;
	int rcv;

	while (cpu == Other)
	{
		cout << "1:server 2:client\n";
		getline(cin, inputBuffer);
		switch (atoi(inputBuffer.c_str()))
		{
		case 1:
			cpu = Server;
			break;
		case 2:
			cpu = Client;
			break;
		default:
			cout << "1 or 2 ����͂���\n";
		}
	}

	switch (cpu)
	{
	case Server:
		networkManager.InputSqN(&sqN);

		sq = new SquareType[sqN];
		networkManager.MakeMap(sq, sqN);

		networkManager.InputPort(&port);

		if (networkManager.ServerStart(port, &soc))
		{
			cout << "�\�P�b�g�ʐM�@�\������Ɏg�p�ł��Ȃ������̂ŒʐM�͍s���܂���\n";
		}
		else
		{
			message = networkManager.Encode(sq, sqN);

			cout << message << "���M\n";//���ۂɂǂ̂悤�ȕ����𑗂�̂��m�F
			int sendBytes = send(soc, message.c_str(), message.length(), 0);

			cout << sendBytes << "bytes�@���M����\n";
		}
		break;
	case Client:
		networkManager.InputHost(&host);
		networkManager.InputPort(&port);

		if (networkManager.ClientStart(host, port, &soc))
		{
			cout << "�\�P�b�g�ʐM�@�\������Ɏg�p�ł��Ȃ������̂ŒʐM�͍s���܂���\n";
		}
		else
		{
			cout << "��M�҂�\n";
			rcv = recv(soc, receiveBuffer, sizeof(receiveBuffer) - 1, 0);

			if (rcv == SOCKET_ERROR)
			{
				cout << "��M�o���܂���ł���\n";
			}
			else
			{
				receiveBuffer[rcv] = '\0';
				cout << "��M\n"; //���ۂɂǂ̂悤�ȕ�������M�������m�F

				cout << rcv << "bytes ��M����\n";

				sqN = networkManager.Decode(receiveBuffer);
				sq = new SquareType[sqN];

				networkManager.Decode(receiveBuffer, sq, sqN);
			}
		}
		break;
	default:
		break;
	}

	cout << "�}�b�v��\�����܂�\n";
	networkManager.DrawMap(sq, sqN);

	networkManager.SocketEnd(&soc);

	delete[] sq; //�{����sq�̏�Ԃ����āAsq���L���ł����delete[]�ɂ��ׂ�

	cout << "�v���O�����̏I��\n";

	return 0;
}
