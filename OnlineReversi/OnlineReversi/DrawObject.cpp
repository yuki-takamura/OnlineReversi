#include "DrawObject.h"
#include <Windows.h>

DrawObject::DrawObject()
{
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	//hStdout�̃R���\�[���X�N���[���o�b�t�@����csbi�Ɏ擾
	GetConsoleScreenBufferInfo(hStdout, &csbi);
}

void DrawObject::initialize()
{
	return;
}

void DrawObject::draw()
{
	return;
}