#include <iostream>
#include "Tank.h"

Tank::Tank()
{
}

Tank::Tank(int x1, int y1)
{
	x = x1;
	y = y1;
	life = 2;
	whichClient = false;
}

void Tank::setX(int x2)
{
	x = x2;

	if (x > 500){
		x = 500;
	}
	else if (x < 0){
		x = 0;
	}
}

void Tank::setY(int y2)
{
	y = y2;

	if (y > 500){
		y = 500;
	}
	else if (y < 0){
		y = 0;
	}
}

void Tank::setLife()
{
	life = life - 1;
}

int Tank::getX()
{
	return x;
}

int Tank::getY()
{
	return y;
}

int Tank::getLife()
{
	return life;
}