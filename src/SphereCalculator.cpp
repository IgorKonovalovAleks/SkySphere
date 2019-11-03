#include "SphereCalculator.h"

SphereCalculator::SphereCalculator() {
	radius = 0.0f;
	horizont_angle = 0.0f;
	vertical_angle = 0.0f;
}


float SphereCalculator::getX()
{
	return radius * cos(vertical_angle) * sin(horizont_angle);
}

float SphereCalculator::getY()
{
	return radius * sin(vertical_angle);
}

float SphereCalculator::getZ()
{
	return radius * cos(vertical_angle) * cos(horizont_angle);
}


void SphereCalculator::setRadius(float r)
{
	radius = r;
}

void SphereCalculator::setHorizontal(float angle)
{
	horizont_angle = angle;
}

void SphereCalculator::setVetrical(float angle)
{
	vertical_angle = angle;
}

void SphereCalculator::changeRadius(float r)
{
	radius += r;
}

void SphereCalculator::changeHorizontal(float angle)
{
	horizont_angle += angle;
}

void SphereCalculator::changeVetrical(float angle)
{
	vertical_angle += angle;
}

float SphereCalculator::getRadius()
{
	return radius;
}

float SphereCalculator::getHorizontal()
{
	return horizont_angle;
}

float SphereCalculator::getVertical()
{
	return vertical_angle;
}


