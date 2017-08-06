#include <iostream>
#include "Player.h"

using namespace std;

void Player::update(Stone stone[VERTICAL][HORIZONTAL])
{
	isPutStone = false;

	cout << "���Ȃ�(��)�̔Ԃł�" << endl;
	cout << "�u�������ꏊ����͂��Ă�������" << endl;
	cout << "�� : 1A�@e5 (�������啶�����Ԃ͋C�ɂ��Ȃ��Ă�OK)" << endl;

	while (true)
	{
		inputString.clear();

		input(stone);

		putStone(stone);

		if (isPutStone)
			break;
	}
}

void Player::input(Stone stone[VERTICAL][HORIZONTAL])
{
	while (true)
	{
		cin >> inputString;

		if (inputString.length() <= 1 || inputString.length() > MAX_STRING_NUM - 1)
		{
			cout << "���͒l���܂������Ă��܂�" << endl;
			continue;
		}

		convertInput();

		if (isInRange())
			break;
		else
			cout << "���̃}�X�͑��݂��܂���" << endl;
	}
}

void Player::convertInput()
{
	//������
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
///���͒l���{�[�h���ɂ��邩
///</summary>
bool Player::isInRange()
{
	if (place.numberOfH >= 0 && place.numberOfH < HORIZONTAL
		&& place.alphabetOfV >= 0 && place.alphabetOfV < VERTICAL)
		return true;
	else
		return false;
}