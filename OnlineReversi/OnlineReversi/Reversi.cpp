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

	player[0].myColor = Color::WHITE;
	player[1].myColor = Color::BLACK;
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
			networkManager.inputPort(&port);

			if (networkManager.serverStart(port, &soc))
			{
				cout << "�\�P�b�g�ʐM�@�\������Ɏg�p�ł��Ȃ������̂ŒʐM�͍s���܂���\n";
			}
			else
			{
				message = networkManager.encode(sqN);

				cout << message << "���M\n";//���ۂɂǂ̂悤�ȕ����𑗂�̂��m�F
				int sendBytes = send(soc, message.c_str(), message.length(), 0);

				cout << sendBytes << "bytes�@���M����\n";

				isConnecting = true;
			}
			break;

		case Client:
			networkManager.inputHost(&host);
			networkManager.inputPort(&port);

			if (networkManager.clientStart(host, port, &soc))
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

					sqN = networkManager.decode(receiveBuffer);

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
			cout << "����(��)�̔Ԃł�";
			rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
			if (rcv == SOCKET_ERROR)
			{
				cout << "error";
				break;
			}

			buffer[rcv] = '\0';

			if (player[1].checkEnd(stone))
			{
				canNotPut[1] = false;
				player[1].update(stone, buffer);
			}
			else
				canNotPut[1] = true;

			Reversi::update();
			Reversi::draw();

			if (strcmp(buffer, "c_end") == 0)
			{
				cout << "�N���C�A���g���ؒf\n";
				break;
			}
			cout << "��M : " << buffer << "�ɒu����܂���\n";
			cout << "���Ȃ�(��)�̔Ԃł� : ";
			cin >> buffer;
			if (strcmp(buffer, "s_end") == 0)
			{
				send(soc, buffer, int(strlen(buffer)), 0);
				break;
			}

			//�u���邩�ǂ����̔��菈��
			if (player[0].checkEnd(stone))
			{
				canNotPut[0] = false;
				player[0].update(stone, buffer);
			}
			else
				canNotPut[0] = true;

			send(soc, buffer, int(strlen(buffer)), 0);
			
			Reversi::update();
			Reversi::draw();
			break;

		case Client:
			
			cout << "���Ȃ�(��)�̔Ԃł� : ";
			cin >> buffer;
			if (strcmp(buffer, "c_end") == 0)
			{
				send(soc, buffer, (int)strlen(buffer), 0);
				break;
			}

			//�u���邩�ǂ����̔���
			if (player[1].checkEnd(stone))
			{
				canNotPut[1] = false;
				player[1].update(stone, buffer);
			}
			else
				canNotPut[1] = true;

			send(soc, buffer, (int)strlen(buffer), 0);

			Reversi::update();
			Reversi::draw();

			cout << "����(��)�̔Ԃł�\n";
			rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
			if (rcv == SOCKET_ERROR)
			{
				cout << "error\n";
				break;
			}

			buffer[rcv] = '\0';

			if (player[0].checkEnd(stone))
			{
				canNotPut[0] = false;
				player[0].update(stone, buffer);
			}
			else
				canNotPut[0] = true;

			Reversi::update();
			Reversi::draw();

			if (strcmp(buffer, "s_end") == 0)
			{
				cout << "�T�[�o�[���ؒf\n";
				break;
			}
			cout << "��M : " << buffer << "�ɒu����܂���\n";
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

	networkManager.socketEnd(&soc);
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
			cout << "����̏����ł�" << endl;
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