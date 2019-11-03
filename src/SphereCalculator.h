#pragma once
#include <iostream>
class SphereCalculator
{
private:
	float radius;
	float horizont_angle;
	float vertical_angle;
public:
	SphereCalculator();
	void setRadius(float r);
	void setHorizontal(float angle);
	void setVetrical(float angle);
	void changeRadius(float r);
	void changeHorizontal(float angle);
	void changeVetrical(float angle);
	float getRadius();
	float getHorizontal();
	float getVertical();
	float getX();
	float getY();
	float getZ();
};

