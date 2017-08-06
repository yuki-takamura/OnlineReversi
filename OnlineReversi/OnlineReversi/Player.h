#ifndef ___Class_Player
#define ___Class_Player

#include <iostream>
#include <string>
#include "PlayerObject.h"

class Player: public PlayerObject
{
public :
	Player(): PlayerObject(true) { }
	void update(Stone stone[VERTICAL][HORIZONTAL], char buffer[256]);

private:
	std :: string inputString;

	void input(Stone stone[VERTICAL][HORIZONTAL], char buffer[256]);
	void convertInput();
	bool isInRange();

protected:
	bool isPlayer = true;
};

#endif
