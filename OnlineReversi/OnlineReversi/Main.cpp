#include "NetworkManager.h"
#include "Reversi.h"
#include<locale>
#include <VersionHelpers.h>
#include <windows.h>
//�����̏��ԂŃC���N���[�h����

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
				cout << "1 or 2 ����͂���\n";
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
				}
				else
				{
					cout << "��M�҂�";
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
						cout << "�N���C�A���g���ؒf\n";
						break;
					}
					cout << "��M : " << buffer << '\n';
					cout << "���M : ";
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
	
							networkManager.Decode(receiveBuffer,sqN);
	
							isConnecting = true;
						}
					}
				}
				else
				{
					cout << "���M : ";
					cin >> buffer;
					if (strcmp(buffer, "c_end") == 0)
					{
						send(soc, buffer, (int)strlen(buffer), 0);
						break;
					}
					send(soc, buffer, (int)strlen(buffer), 0);

					cout << "��M�҂�\n";
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
						cout << "�T�[�o�[���ؒf\n";
						break;
					}
					cout << "��M : " << buffer << '\n';
				}
				break;
			default:
				break;
			}
			networkManager.DrawMap(sqN);
		}
	
		networkManager.SocketEnd(&soc);

	bool isVersion7;
	
	//Windows�̃o�[�W�������m�F����
	if (IsWindows8OrGreater())
	{
		//8�ȏ�
		isVersion7 = false;
	}
	else
	{
		//7�ȑO
		isVersion7 = true;

		//���C�h�^�őS�p������\���ł���悤�ɐݒ�
		locale::global(locale("japanese"));
	}

	Reversi reversi(isVersion7);
	reversi.run();

	return 0;
}

//�Q�l
//http://tech2assist.web.fc2.com/wabisabi/cpp/cpp014.html
//http://inemaru.hatenablog.com/entry/2015/09/23/002412
//https://msdn.microsoft.com/en-us/library/ms724429(v=vs.85).aspx
//http://www.kumei.ne.jp/c_lang/intro/no_58.htm