#include <iostream>
#include <random>
#include "Enemy.h"

using namespace std;

void Enemy::update(Stone stone[VERTICAL][HORIZONTAL])
{
	isPutStone = false;
	
	while (true)
	{
		generateRandom();

		putStone(stone);

		if (isPutStone)
			break;
	}
}

void Enemy::generateRandom()
{
	place.alphabetOfV = rand() % VERTICAL + 1;
	place.numberOfH = rand() % HORIZONTAL + 1;
}