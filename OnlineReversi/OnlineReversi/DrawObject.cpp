#include "DrawObject.h"
#include <Windows.h>

DrawObject::DrawObject()
{
	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	//hStdoutのコンソールスクリーンバッファ情報をcsbiに取得
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