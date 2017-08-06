#include "Reversi.h"
#include<locale>
#include <VersionHelpers.h>
#include <windows.h>
//↑この順番でインクルードする

using namespace std;

int __cdecl wmain(__in int argc, __in_ecount(argc) PCWSTR argv[])
{
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