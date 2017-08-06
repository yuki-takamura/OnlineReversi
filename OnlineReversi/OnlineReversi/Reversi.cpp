#include <iostream>
#include <time.h>
#include <random>
#include "DrawObject.h"
#include "Reversi.h"

using namespace std;

Reversi::Reversi(bool isVersion7)
{
	this->isVersion7 = isVersion7;
	srand((unsigned)time(NULL));
}

void Reversi::run()
{
	Reversi::initialize();
	Reversi::draw();

	while (true)
	{
		Reversi::update();

		Reversi::draw();

		if (canNotPut[0] && canNotPut[1])
		{
			cout << "�I�����܂�" << endl;
			break;
		}
	}
}

void Reversi::initialize()
{
	board.initialize();

	const int GREEN = 0;
	const int BLACK = 1;
	const int WHITE = 2;

	for (int i = 0; i < VERTICAL; i++)
	{
		for (int j = 0; j < HORIZONTAL; j++)
		{
			//�����̍���
			if ((j + 1) * 2 == HORIZONTAL && (i + 1) * 2 == VERTICAL)
			{
				stone[i][j].initialize(BLACK);
				continue;
			}
			//�����̉E��
			if (j * 2 == HORIZONTAL && (i + 1) * 2 == VERTICAL)
			{
				stone[i][j].initialize(WHITE);
				continue;
			}
			//�����̍���
			if ((j + 1) * 2 == HORIZONTAL && i * 2 == VERTICAL)
			{
				stone[i][j].initialize(WHITE);
				continue;
			}
			//�����̉E��
			if (j * 2 == HORIZONTAL && i * 2 == VERTICAL)
			{
				stone[i][j].initialize(BLACK);
				continue;
			}

			stone[i][j].initialize(GREEN);
		}
	}
}

void Reversi::update()
{
	cout << endl;

	if (player.checkEnd(stone))
	{
		canNotPut[0] = false;
		player.update(stone);
	}
	else
		canNotPut[0] = true;

	draw();

	if (enemy.checkEnd(stone))
	{
		canNotPut[1] = false;
		enemy.update(stone);
	}
	else
		canNotPut[1] = true;

	//�`��X�V
	system("cls");

	if (canNotPut[0] && canNotPut[1])
	{
		for (int i = 0; i < VERTICAL; i++)
		{
			for (int j = 0; j < HORIZONTAL;j++)
			{
				if (stone[i][j].myColor == Color::BLACK)
					stoneCounter[0]++;
				else if (stone[i][j].myColor == Color::WHITE)
					stoneCounter[1]++;
			}
		}

		if (stoneCounter[0] > stoneCounter[1])
		{
			cout << "���Ȃ��̏����ł�" << endl;
		}
		else if (stoneCounter[0] < stoneCounter[1])
		{
			cout << "�R���s���[�^�̏����ł�" << endl;
		}
		else
		{
			cout << "�Ђ��킯" << endl;
		}
		cout << "���� : " << stoneCounter[0] << "��" << endl;
		cout << "���� : " << stoneCounter[1] << "��" << endl;

	}
}

void Reversi::draw()
{
	cout << endl;
	guide.drawHorizontal(isVersion7);
	board.draw(stone, guide);
}