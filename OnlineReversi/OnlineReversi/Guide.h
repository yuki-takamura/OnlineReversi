#ifndef ___Class_Guide
#define ___Class_Guide

#include "DrawObject.h"

class Guide : public DrawObject
{
public:
	void drawHorizontal(bool isVersion7);
	void drawVertical(int numberOfVertical);
};

#endif