#include <iostream>
#include <string>
#include "Player.h"

using namespace std;

void Player::update(Stone stone[VERTICAL][HORIZONTAL], char buffer[256])
{
	isPutStone = false;

	while (true)
	{
		inputString.clear();

		input(stone, buffer);

		putStone(stone);

		if (isPutStone)
			break;
	}
}

void Player::input(Stone stone[VERTICAL][HORIZONTAL], char buffer[256])
{
	while (true)
	{
		inputString = buffer;

		if (inputString.length() <= 1 || inputString.length() > MAX_STRING_NUM - 1)
		{
			cout << "入力値がまちがっています" << endl;
			continue;
		}

		convertInput();

		if (isInRange())
			break;
		else
			cout << "そのマスは存在しません" << endl;
	}
}

void Player::convertInput()
{
	//初期化
	place.numberOfH = EMPTY_NUM;
	place.alphabetOfV = EMPTY_NUM;

	for (int i = 0; i < inputString.length(); i++)
	{
		if (inputString[i] >= '1' && inputString[i] <= '1' + HORIZONTAL)
		{
			place.numberOfH = inputString[i] - '1';
			continue;
		}

		if (inputString[i] >= 'A' && inputString[i] <= 'A' + VERTICAL) 
		{
			place.alphabetOfV = inputString[i] - 'A';
			continue;
		}

		if (inputString[i] >= 'a' && inputString[i] <= 'a' + VERTICAL)
		{
			place.alphabetOfV = inputString[i] - 'a';
			continue;
		}
	}
}

///<summary>
///入力値がボード内にあるか
///</summary>
bool Player::isInRange()
{
	if (place.numberOfH >= 0 && place.numberOfH < HORIZONTAL
		&& place.alphabetOfV >= 0 && place.alphabetOfV < VERTICAL)
		return true;
	else
		return false;
}