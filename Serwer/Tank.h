#ifndef Tank_H
#define Tank_H

class Tank
{
private:
	int x;
	int y;
	bool whichClient;

public:
	Tank();
	Tank(int x1, int y2);
	int getX();
	void setX(int x2);
	int getY();
	void setY(int y2);

};
#endif