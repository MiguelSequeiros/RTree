#ifndef MBR_H
#define MBR_H

#include "Interval.h"
#include <GL/glut.h>
#include <iostream>

/* 	Un Mbr ( Minimun Bounding Rectangle) o hiperrectangulo es una coleccion de intervalos,
	específicamente, una coleccíon de N intervalos(Interval), donde N es el numero de dimensiones*/

class Mbr
{
private:
	Interval *intervals; //la coleccion de N intervalos

public:
	Mbr( int , float** );
	Mbr( Mbr* );

	Interval* getIntervals();
	float findArea( int );
	float findNewArea( Mbr , int );
	bool checkOverlap( Mbr , int );
	void refreshIntervals( Mbr , int );
	void draw( bool );
	void drawMbrLines( bool );

	~Mbr();
	
};

Mbr::Mbr( int N , float** m )
{
	std::cout << "Starting Mbr creation ..." << std::endl;
	this->intervals = new Interval[N];
	for( int i=0 ; i<N ; i++)
	{
		this->intervals[i].setMin(m[1][i]);
		this->intervals[i].setMax(m[2][i]);
	}
	std::cout << "Mbr created..."<< std::endl;
};

Mbr::Mbr( Mbr* mbr_n )
{	
	std::cout << "Starting Mbr copy ..." << std::endl;
	this->intervals = mbr_n->getIntervals();
	std::cout << "Min " << this->intervals[0].getMin() << " max " <<  this->intervals[0].getMax() << std::endl;
	std::cout << "Min " << this->intervals[1].getMin() << " max " <<  this->intervals[1].getMax() << std::endl;
	std::cout << "Min " << this->intervals[2].getMin() << " max " <<  this->intervals[2].getMax() << std::endl;
	std::cout << "Mbr copied..."<< std::endl;
};

Interval* Mbr::getIntervals()
{
	return this->intervals; 
};

float Mbr::findArea( int N )
{
	float area = 1;

	for(int i=0 ; i<N ; i++)
		area *= intervals[i].getMax() - intervals[i].getMin();

	return area;
};

float Mbr::findNewArea( Mbr newMbr , int N )
{
	float area = 1;
	Interval *intervals1 = newMbr.getIntervals();

	for(int i=0 ; i<N ; i++)
	{
		float min = intervals1[i].getMin() < this->intervals[i].getMin() ? intervals1[i].getMin() : intervals[i].getMin(); 
		float max = intervals1[i].getMax() > this->intervals[i].getMax() ? intervals1[i].getMax() : intervals[i].getMax(); 
		area *= min - max;
	}

	return area;
};

bool Mbr::checkOverlap( Mbr mbr1 , int N )
{
	Interval *intervals1 = mbr1.getIntervals();

	for(int i=0 ; i<N ; i++)
	{
		if( intervals[i].getMin() > this->intervals[i].getMax() || intervals[i].getMax() < this->intervals[i].getMin() )
			return false;
	}

	return true;
};

void Mbr::refreshIntervals( Mbr mbr , int N )
{
	for( int i=0 ; i<N ; i++ )
	{
		intervals[i].setMax( intervals[i].getMax() >= mbr.getIntervals()[i].getMax() ? intervals[i].getMax() : mbr.getIntervals()[i].getMax() );
		intervals[i].setMin( intervals[i].getMin() >= mbr.getIntervals()[i].getMin() ? intervals[i].getMin() : mbr.getIntervals()[i].getMin() );
	}
	std::cout << "Mbr refreshed..." << std::endl;
};

void Mbr::draw( bool is3D ) // esto va a cambiar a un booleano
{
		// Z max = constante
		glBegin( GL_LINE_LOOP );
			glVertex3f( this->intervals[0].getMin() , this->intervals[1].getMin() , this->intervals[2].getMax() );
			glVertex3f( this->intervals[0].getMin() , this->intervals[1].getMax() , this->intervals[2].getMax() );
			glVertex3f( this->intervals[0].getMax() , this->intervals[1].getMax() , this->intervals[2].getMax() );
			glVertex3f( this->intervals[0].getMax() , this->intervals[1].getMin() , this->intervals[2].getMax() );
		glEnd();
		glFlush();
		// Z min = constante
		glBegin( GL_LINE_LOOP );
			glVertex3f( this->intervals[0].getMin() , this->intervals[1].getMin() , this->intervals[2].getMin() );
			glVertex3f( this->intervals[0].getMin() , this->intervals[1].getMax() , this->intervals[2].getMin() );
			glVertex3f( this->intervals[0].getMax() , this->intervals[1].getMax() , this->intervals[2].getMin() );
			glVertex3f( this->intervals[0].getMax() , this->intervals[1].getMin() , this->intervals[2].getMin() );
		glEnd();
		glFlush();

}

void Mbr::drawMbrLines( bool is3D ) // esto va a cambiar a un booleano bool is3D
{
			glVertex3f( this->intervals[0].getMin() , this->intervals[1].getMin() , this->intervals[2].getMax() );
			glVertex3f( this->intervals[0].getMin() , this->intervals[1].getMin() , this->intervals[2].getMin() );

			glVertex3f( this->intervals[0].getMin() , this->intervals[1].getMax() , this->intervals[2].getMax() );
			glVertex3f( this->intervals[0].getMin() , this->intervals[1].getMax() , this->intervals[2].getMin() );

			glVertex3f( this->intervals[0].getMax() , this->intervals[1].getMax() , this->intervals[2].getMax() );
			glVertex3f( this->intervals[0].getMax() , this->intervals[1].getMax() , this->intervals[2].getMin() );

			glVertex3f( this->intervals[0].getMax() , this->intervals[1].getMin() , this->intervals[2].getMax() );
			glVertex3f( this->intervals[0].getMax() , this->intervals[1].getMin() , this->intervals[2].getMin() );
};

Mbr::~Mbr(){};
#endif // MBR_H
