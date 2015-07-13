#ifndef INTERVAL_H
#define INTERVAL_H

#include <iostream>

/*
	" 	Here n is the number of dimension and Ii is a closed bounded Interval[a,b] describing
		the extent of the object along dimension i. Alternatively Ii may have one or both endpoints
		equal to infinity, idicating that the object extends outward indefinitely Non-leaf nodes ..."
*/

class Interval 
{

private:

	float min; //a
	float max; //b

public:

	Interval ( float , float );
	Interval ();
	float getMin ();
	float getMax ();	
	void setMin (float);
	void setMax (float);
	~Interval();

};


Interval::Interval( float min , float max )
{
	this->min = min;
	this->max = max;
	std::cout << "Interval created, min: " << min << "  max:" << max << std::endl;
};

Interval::Interval()
{
	this->min = 0;
	this->max = 0;
	std::cout << "Interval created" << std::endl;
};

float Interval::getMin () { return this->min; };
float Interval::getMax () { return this->max; };
void Interval::setMin (float min) 
{ 
	this->min = min;
	std::cout << " Interval minValue: " << this->min << std::endl;
};
void Interval::setMax (float max) 
{ 
	this->max = max; 
	std::cout << " Interval maxValue: " << this->max << std::endl;
};

Interval::~Interval(){};
#endif //INTERVAL_H
