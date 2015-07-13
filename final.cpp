#include "RTree.h"
#include "NumParser.h"
#include <iostream>
int main()
{
	std::cout << "Testing ... "  << std::endl;
  	std::string fileName = "RandTest.txt";

	NumParser np1( 3 , fileName );

	Mbr** setOfMbr = np1.readFromFile();

	RTree rt1 = RTree( 3 , 3 );

	for ( int i=0 ; i<10 ; i++ )
		rt1.insert( *setOfMbr[i] , Data() );

	return 0;
}