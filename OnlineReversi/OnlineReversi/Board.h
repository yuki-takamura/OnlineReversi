#ifndef ___Class_Board
#define ___Class_Board

#include "DrawObject.h"
#include "Guide.h"
#include "Stone.h"
#include "ConstantMacro.h"

class Board : public DrawObject
{
public:
	void setBoardColor();
	void update();
	void draw(Stone stone[VERTICAL][HORIZONTAL], Guide guide);
};
#endif