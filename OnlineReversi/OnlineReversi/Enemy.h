#ifndef ___Class_Enemy
#define ___Class_Enemy

#include "PlayerObject.h"

class Enemy : public PlayerObject
{
public :
	Enemy() : PlayerObject(false) {}
	void update(Stone stone[VERTICAL][HORIZONTAL]);

private:
	void generateRandom();
};
#endif