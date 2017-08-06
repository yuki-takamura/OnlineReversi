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
			cout << "1 or 2 を入力せよ\n";
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
			cout << "ソケット通信機能が正常に使用できなかったので通信は行いません\n";
		}
		else
		{
			message = networkManager.Encode(sq, sqN);

			cout << message << "送信\n";//実際にどのような文字を送るのか確認
			int sendBytes = send(soc, message.c_str(), message.length(), 0);

			cout << sendBytes << "bytes　送信完了\n";
		}
		break;
	case Client:
		networkManager.InputHost(&host);
		networkManager.InputPort(&port);

		if (networkManager.ClientStart(host, port, &soc))
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

				sqN = networkManager.Decode(receiveBuffer);
				sq = new SquareType[sqN];

				networkManager.Decode(receiveBuffer, sq, sqN);
			}
		}
		break;
	default:
		break;
	}

	cout << "マップを表示します\n";
	networkManager.DrawMap(sq, sqN);

	networkManager.SocketEnd(&soc);

	delete[] sq; //本来はsqの状態を見て、sqが有効であればdelete[]にすべき

	cout << "プログラムの終了\n";

	return 0;
}
