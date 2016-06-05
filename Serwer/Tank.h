#ifndef Tank_H
#define Tank_H

class Tank
{
private:
	int x;
	int y;
	int life;
	bool whichClient;

public:
	Tank();
	Tank(int x1, int y2);
	void setX(int x2);
	void setY(int y2);
	void setLife();
	int getX();
	int getY();
	int getLife();
};
#endif