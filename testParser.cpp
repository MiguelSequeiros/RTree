#include <string>
#include <iostream>
#include "NumParser.h"
#include "RandGenerator.h"
#include "Mbr.h"
#include "Interval.h"

int main()
{
	int min = 1;
	int max = 1000;
	int N = 3;
	std::string fileName = "RandTest.txt";

	RandGenerator rg1( min , max , 1000 , fileName , N );
	rg1.writeOnAFile();
    std::cout << "Para el Mbr "  << std::endl;
	NumParser np1( N , fileName );
    std::cout << "Para el Mbr "  << std::endl;
	Mbr** setOfMbr = np1.readFromFile();
    std::cout << "Para el Mbr " << std::endl;
    for( int i=0 ; i<499 ; i++ )
    {
        Interval* tmp = setOfMbr[i]->getIntervals();
        std::cout << "Para el Mbr " << i << std::endl;
        for( int j=0 ; j<N ; j++ )
        {
            std::cout << tmp[j].getMin()  << " " << tmp[j].getMax() << std::endl;
        }
        std::cout << " end " << std::endl;
    }

	return 0;
};