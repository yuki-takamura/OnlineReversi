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

	//設定
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
			cout << "1 or 2 を入力せよ\n";
		}
	}

	//接続
	while (!isConnecting) {
		switch (cpu)
		{
		case Server:
			networkManager.inputPort(&port);

			if (networkManager.serverStart(port, &soc))
			{
				cout << "ソケット通信機能が正常に使用できなかったので通信は行いません\n";
			}
			else
			{
				message = networkManager.encode(sqN);

				cout << message << "送信\n";//実際にどのような文字を送るのか確認
				int sendBytes = send(soc, message.c_str(), message.length(), 0);

				cout << sendBytes << "bytes　送信完了\n";

				isConnecting = true;
			}
			break;

		case Client:
			networkManager.inputHost(&host);
			networkManager.inputPort(&port);

			if (networkManager.clientStart(host, port, &soc))
			{
				cout << "ソケット通信機能が正常に使用できなかったので通信は行いません\n";
			}
			else
			{
				cout << "受信待ち\n";
				rcv = recv(soc, receiveBuffer, sizeof(receiveBuffer) - 1, 0);

				if (rcv == SOCKET_ERROR)
				{
					cout << "受信出来ませんでした\n";
				}
				else
				{
					receiveBuffer[rcv] = '\0';
					cout << "受信\n"; //実際にどのような文字を受信したか確認

					cout << rcv << "bytes 受信完了\n";

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

	//実行
	while (true)
	{
		switch (cpu)
		{
		case Server:
			cout << "相手(黒)の番です";
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
				cout << "クライアントが切断\n";
				break;
			}
			cout << "受信 : " << buffer << "に置かれました\n";
			cout << "あなた(白)の番です : ";
			cin >> buffer;
			if (strcmp(buffer, "s_end") == 0)
			{
				send(soc, buffer, int(strlen(buffer)), 0);
				break;
			}

			//置けるかどうかの判定処理
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
			
			cout << "あなた(黒)の番です : ";
			cin >> buffer;
			if (strcmp(buffer, "c_end") == 0)
			{
				send(soc, buffer, (int)strlen(buffer), 0);
				break;
			}

			//置けるかどうかの判定
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

			cout << "相手(白)の番です\n";
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
				cout << "サーバーが切断\n";
				break;
			}
			cout << "受信 : " << buffer << "に置かれました\n";
			break;
		default:
			break;
		}

		if (canNotPut[0] && canNotPut[1])
		{
			cout << "終了します" << endl;
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
			//中央の左上
			if ((j + 1) * 2 == HORIZONTAL && (i + 1) * 2 == VERTICAL)
			{
				stone[i][j].initialize(BLACK);
				continue;
			}
			//中央の右上
			if (j * 2 == HORIZONTAL && (i + 1) * 2 == VERTICAL)
			{
				stone[i][j].initialize(WHITE);
				continue;
			}
			//中央の左下
			if ((j + 1) * 2 == HORIZONTAL && i * 2 == VERTICAL)
			{
				stone[i][j].initialize(WHITE);
				continue;
			}
			//中央の右下
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
	//描画更新
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
			cout << "あなたの勝ちです" << endl;
		}
		else if (stoneCounter[0] < stoneCounter[1])
		{
			cout << "相手の勝ちです" << endl;
		}
		else
		{
			cout << "ひきわけ" << endl;
		}
		cout << "くろ : " << stoneCounter[0] << "個" << endl;
		cout << "しろ : " << stoneCounter[1] << "個" << endl;
	}
}

void Reversi::draw()
{
	cout << endl;
	guide.drawHorizontal(isVersion7);
	board.draw(stone, guide);
}