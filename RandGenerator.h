#ifndef RANDGENERATOR_H
#define RANDGENERATOR_H

#include <random>
#include <string>
#include <fstream>


class RandGenerator
{

public:
	RandGenerator( int , int , int , std::string , int );
	void writeOnAFile();
	~RandGenerator();

private:
	int min;
	int max;
	int q;
	std::string file_name;
	int col;

};


RandGenerator::RandGenerator( int min , int max , int q , std::string file_name , int col )	// col : N dimensiones
{
	this->min = min;
	this->max = max;
	this->q = q;
	this->file_name = file_name;
	this->col = col;
};

void RandGenerator::writeOnAFile()
{
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist( min , max );

    std::ofstream test_file ( file_name );

    for( int i=0 ; i < q-1 ; i++ )
    {
    	for( int j=0 ; j < col-1 ; j++ )
 			test_file << dist(mt) << " ";

 		test_file << dist(mt) << "\n";
    }

    for( int j=0 ; j < col-1 ; j++ )
 		test_file << dist(mt) << " ";

 	test_file << dist(mt);

};

RandGenerator::~RandGenerator() {};

#endif // RANDGENERATOR_H