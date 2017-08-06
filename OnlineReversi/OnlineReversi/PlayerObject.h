#ifndef ___Class_PlayerObject
#define ___Class_PlayerObject

#include "Stone.h"
#include "ConstantMacro.h"

struct Place
{
	int alphabetOfV;
	int numberOfH;
};

class PlayerObject
{
private:
	Color myColor;

public:
	PlayerObject(bool isTurnFirst);
	void virtual update(Stone stone[VERTICAL][HORIZONTAL]);
	bool checkEnd(Stone stone[VERTICAL][HORIZONTAL]);

protected :
	bool isPutStone;
	bool isPlayer = false;
	bool canFlip[VECTOR_NUM];

	void putStone(Stone stone[VERTICAL][HORIZONTAL]);
	bool checkCanPut(int x, int y, int vec, Stone stone[VERTICAL][HORIZONTAL]);
	void flip(int x, int y, int vec, Stone stone[VERTICAL][HORIZONTAL]);

	Place place;
};

#endif