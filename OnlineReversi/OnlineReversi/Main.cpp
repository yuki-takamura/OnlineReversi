#include "Reversi.h"
#include<locale>
#include <VersionHelpers.h>
#include <windows.h>
//�����̏��ԂŃC���N���[�h����

using namespace std;

int __cdecl wmain(__in int argc, __in_ecount(argc) PCWSTR argv[])
{
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