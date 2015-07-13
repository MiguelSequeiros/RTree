#ifndef NUMPARSER_H
#define NUMPARSER_H

#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>

#include "Mbr.h"

class NumParser
{

public:
	NumParser( int , std::string );
	Mbr** readFromFile();
	~NumParser();

private:
	int N;
	std::string fileName;

};

NumParser::NumParser( int N , std::string fileName )
{
	this->N = N;
	this->fileName = fileName;
};

Mbr** NumParser::readFromFile()
{
	std::vector<Mbr*> rep;

	std::ifstream source ( this->fileName );

	if(!source)
		std::cerr << " No se puede abrir ese archivo ... ";

	else
	{
		int i = 1;
		float** m;
		for(std::string line ; std::getline(source, line); )
		{
		    std::istringstream in(line);

		    if( i%2 != 0 )
		    {
				m = new float*[2];
			    m[1] = new float[this->N];
			    m[2] = new float[this->N];

			    for( int i=0 ; i < this->N ; i++ )
			    	in >> m[1][i];
			}

			else
			{

			    for( int i=0 ; i < this->N ; i++ )
			    	in >> m[2][i];

			    Mbr* tmp = new Mbr( this->N , m );			    
			    rep.push_back(tmp);

			    delete m;
			}

		    i++;
		}
	}

	Mbr** res = new Mbr*[rep.size()];

	for(int i=0 ; i<rep.size() ; i++ )
		res[i] = rep[i];

	return res;
};

NumParser::~NumParser() {}; 

#endif // NUMPARSER_H