#include "NetworkManager.h"
#include <iostream>
#include <time.h>
#include <random>
#include "DrawObject.h"
#include "Reversi.h"

using namespace std;

Reversi::Reversi(bool isVersion7)
{
	this->isVersion7 = isVersion7;
	srand((unsigned)time(NULL));
}

void Reversi::run()
{
	int sqN = 0;

	string inputBuffer;

	string message;
	char buffer[256];
	char receiveBuffer[256];

	string host;
	unsigned short port = 0;
	SOCKET soc;
	int rcv;

	bool isConnecting = false;

	//�ݒ�
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

	//�ڑ�
	while (!isConnecting) {
		switch (cpu)
		{
		case Server:
			networkManager.InputSqN(&sqN);

			networkManager.InputPort(&port);

			if (networkManager.ServerStart(port, &soc))
			{
				cout << "�\�P�b�g�ʐM�@�\������Ɏg�p�ł��Ȃ������̂ŒʐM�͍s���܂���\n";
			}
			else
			{
				message = networkManager.Encode(sqN);

				cout << message << "���M\n";//���ۂɂǂ̂悤�ȕ����𑗂�̂��m�F
				int sendBytes = send(soc, message.c_str(), message.length(), 0);

				cout << sendBytes << "bytes�@���M����\n";

				isConnecting = true;
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

					networkManager.Decode(receiveBuffer, sqN);

					isConnecting = true;
				}
			}
			break;
		default:
			break;
		}
	}

	Reversi::initialize();
	Reversi::draw();

	//���s
	while (true)
	{
		switch (cpu)
		{
		case Server:
			cout << "����̔Ԃł�";
			rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
			if (rcv == SOCKET_ERROR)
			{
				cout << "error";
				break;
			}

			//�����ŕ`��
			Reversi::draw();

			buffer[rcv] = '\0';

			if (strcmp(buffer, "c_end") == 0)
			{
				cout << "�N���C�A���g���ؒf\n";
				break;
			}
			cout << "��M : " << buffer << '\n';
			cout << "���Ȃ��̔Ԃł� : ";
			cin >> buffer;
			if (strcmp(buffer, "s_end") == 0)
			{
				send(soc, buffer, int(strlen(buffer)), 0);
				break;
			}
			//�u���邩�ǂ����̔��菈��
			send(soc, buffer, int(strlen(buffer)), 0);
			
			//�`��
			Reversi::draw();
			break;

		case Client:
			
			cout << "���Ȃ��̔Ԃł� : ";
			cin >> buffer;
			if (strcmp(buffer, "c_end") == 0)
			{
				send(soc, buffer, (int)strlen(buffer), 0);
				break;
			}

			//�u���邩�ǂ����̔���

			send(soc, buffer, (int)strlen(buffer), 0);

			//�����ŕ`��
			Reversi::draw();

			cout << "����̔Ԃł�\n";
			rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
			if (rcv == SOCKET_ERROR)
			{
				cout << "error\n";
				break;
			}

			//�����ŕ`��
			Reversi::draw();

			buffer[rcv] = '\0';

			if (strcmp(buffer, "s_end") == 0)
			{
				cout << "�T�[�o�[���ؒf\n";
				break;
			}
			cout << "��M : " << buffer << '\n';
			break;
		default:
			break;
		}

		if (canNotPut[0] && canNotPut[1])
		{
			cout << "�I�����܂�" << endl;
			break;
		}
	}

	networkManager.SocketEnd(&soc);

	while (true)
	{
		Reversi::update();

		Reversi::draw();


	}
}

void Reversi::initialize()
{
	board.initialize();

	const int GREEN = 0;
	const int BLACK = 1;
	const int WHITE = 2;

	for (int i = 0; i < VERTICAL; i++)
	{
		for (int j = 0; j < HORIZONTAL; j++)
		{
			//�����̍���
			if ((j + 1) * 2 == HORIZONTAL && (i + 1) * 2 == VERTICAL)
			{
				stone[i][j].initialize(BLACK);
				continue;
			}
			//�����̉E��
			if (j * 2 == HORIZONTAL && (i + 1) * 2 == VERTICAL)
			{
				stone[i][j].initialize(WHITE);
				continue;
			}
			//�����̍���
			if ((j + 1) * 2 == HORIZONTAL && i * 2 == VERTICAL)
			{
				stone[i][j].initialize(WHITE);
				continue;
			}
			//�����̉E��
			if (j * 2 == HORIZONTAL && i * 2 == VERTICAL)
			{
				stone[i][j].initialize(BLACK);
				continue;
			}

			stone[i][j].initialize(GREEN);
		}
	}
}

void Reversi::update()
{
	cout << endl;

	if (player.checkEnd(stone))
	{
		canNotPut[0] = false;
		player.update(stone);
	}
	else
		canNotPut[0] = true;

	draw();

	if (enemy.checkEnd(stone))
	{
		canNotPut[1] = false;
		enemy.update(stone);
	}
	else
		canNotPut[1] = true;

	//�`��X�V
	system("cls");

	if (canNotPut[0] && canNotPut[1])
	{
		for (int i = 0; i < VERTICAL; i++)
		{
			for (int j = 0; j < HORIZONTAL;j++)
			{
				if (stone[i][j].myColor == Color::BLACK)
					stoneCounter[0]++;
				else if (stone[i][j].myColor == Color::WHITE)
					stoneCounter[1]++;
			}
		}

		if (stoneCounter[0] > stoneCounter[1])
		{
			cout << "���Ȃ��̏����ł�" << endl;
		}
		else if (stoneCounter[0] < stoneCounter[1])
		{
			cout << "�R���s���[�^�̏����ł�" << endl;
		}
		else
		{
			cout << "�Ђ��킯" << endl;
		}
		cout << "���� : " << stoneCounter[0] << "��" << endl;
		cout << "���� : " << stoneCounter[1] << "��" << endl;

	}
}

void Reversi::draw()
{
	cout << endl;
	guide.drawHorizontal(isVersion7);
	board.draw(stone, guide);
}