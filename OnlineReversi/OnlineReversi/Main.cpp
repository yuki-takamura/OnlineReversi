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

//参考
//http://tech2assist.web.fc2.com/wabisabi/cpp/cpp014.html
//http://inemaru.hatenablog.com/entry/2015/09/23/002412
//https://msdn.microsoft.com/en-us/library/ms724429(v=vs.85).aspx
//http://www.kumei.ne.jp/c_lang/intro/no_58.htm