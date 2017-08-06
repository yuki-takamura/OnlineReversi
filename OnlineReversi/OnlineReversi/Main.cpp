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