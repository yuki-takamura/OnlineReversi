#include "NetworkManager.h"
#include "Reversi.h"
#include<locale>
#include <VersionHelpers.h>
#include <windows.h>
//↑この順番でインクルードする

using namespace std;

int __cdecl wmain(__in int argc, __in_ecount(argc) PCWSTR argv[])
{
	NetworkManager networkManager;
	
		int sqN = 0;
		CPUType cpu = Other;
	
		string inputBuffer;
	
		string message;
		char buffer[256];
		char receiveBuffer[256];
	
		string host;
		unsigned short port = 0;
		SOCKET soc;
		int rcv;
	
		bool isConnecting = false;
	
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
	
		while (1) {
			switch (cpu)
			{
			case Server:
				if (!isConnecting)
				{
					networkManager.InputSqN(&sqN);
	
					networkManager.MakeMap(sqN);
	
					networkManager.InputPort(&port);
	
					if (networkManager.ServerStart(port, &soc))
					{
						cout << "ソケット通信機能が正常に使用できなかったので通信は行いません\n";
					}
					else
					{
						message = networkManager.Encode(sqN);
	
						cout << message << "送信\n";//実際にどのような文字を送るのか確認
						int sendBytes = send(soc, message.c_str(), message.length(), 0);
	
						cout << sendBytes << "bytes　送信完了\n";
	
						isConnecting = true;
					}
				}
				else
				{
					cout << "受信待ち";
					rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
					if (rcv == SOCKET_ERROR)
					{
						cout << "error";
						break;
					}

					cout << "rcv = " << rcv << '\n';
					buffer[rcv] = '\0';

					if (strcmp(buffer, "c_end") == 0)
					{
						cout << "クライアントが切断\n";
						break;
					}
					cout << "受信 : " << buffer << '\n';
					cout << "送信 : ";
					cin >> buffer;
					if (strcmp(buffer, "s_end") == 0)
					{
						send(soc, buffer, int(strlen(buffer)), 0);
						break;
					}
					send(soc, buffer, int(strlen(buffer)), 0);
				}
				break;
	
			case Client:
				if (!isConnecting)
				{
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
	
							networkManager.Decode(receiveBuffer,sqN);
	
							isConnecting = true;
						}
					}
				}
				else
				{
					cout << "送信 : ";
					cin >> buffer;
					if (strcmp(buffer, "c_end") == 0)
					{
						send(soc, buffer, (int)strlen(buffer), 0);
						break;
					}
					send(soc, buffer, (int)strlen(buffer), 0);

					cout << "受信待ち\n";
					rcv = recv(soc, buffer, sizeof(buffer) - 1, 0);
					if (rcv == SOCKET_ERROR)
					{
						cout << "error\n";
						break;
					}

					cout << "rcv = " << rcv << '\n';
					buffer[rcv] = '\0';

					if (strcmp(buffer, "s_end") == 0)
					{
						cout << "サーバーが切断\n";
						break;
					}
					cout << "受信 : " << buffer << '\n';
				}
				break;
			default:
				break;
			}
			networkManager.DrawMap(sqN);
		}
	
		networkManager.SocketEnd(&soc);

	bool isVersion7;
	
	//Windowsのバージョンを確認する
	if (IsWindows8OrGreater())
	{
		//8以上
		isVersion7 = false;
	}
	else
	{
		//7以前
		isVersion7 = true;

		//ワイド型で全角文字を表示できるように設定
		locale::global(locale("japanese"));
	}

	Reversi reversi(isVersion7);
	reversi.run();

	return 0;
}

//参考
//http://tech2assist.web.fc2.com/wabisabi/cpp/cpp014.html
//http://inemaru.hatenablog.com/entry/2015/09/23/002412
//https://msdn.microsoft.com/en-us/library/ms724429(v=vs.85).aspx
//http://www.kumei.ne.jp/c_lang/intro/no_58.htm