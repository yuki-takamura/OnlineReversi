#include <iostream>
#include <Windows.h>
#include "Stone.h"

using namespace std;

void Stone::initialize(int colorNumber)
{
	switch (colorNumber)
	{
	case 0:
		myColor = GREEN;
		break;
	case 1:
		myColor = BLACK;
		break;
	case 2:
		myColor = WHITE;
		break;
	default:
		myColor = GREEN;
		break;
	}
}

void Stone::update()
{

}

void Stone::draw()
{
	switch (myColor)
	{
	case GREEN:
		wAttributes = FOREGROUND_GREEN | BACKGROUND_GREEN;
		break;

	case BLACK:
		wAttributes = COLOR_BLACK | BACKGROUND_GREEN;
		break;

	case WHITE:
		wAttributes = COLOR_WHITE | FOREGROUND_INTENSITY | BACKGROUND_GREEN;
		break;

	default:
		wAttributes = FOREGROUND_GREEN | BACKGROUND_GREEN;
		break;
	}
	SetConsoleTextAttribute(hStdout, wAttributes);

	cout << "●";

	//元のテキスト状態に戻す
	SetConsoleTextAttribute(hStdout, csbi.wAttributes);
}