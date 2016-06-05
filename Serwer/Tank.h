#ifndef Tank_H
#define Tank_H

class Tank
{
private:
	int x;
	int y;
	int course; // 1- dol 2-gora 3- lewo 4- prawo
	int life;
	bool whichClient;

public:
	Tank();
	Tank(int x1, int y2, int c);
	void setX(int x2);
	void setY(int y2);
	void setLife();
	void setCourse(int c);
	int getX();
	int getY();
	int getLife();
	int getCourse();
};
#endif