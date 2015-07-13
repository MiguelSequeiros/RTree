#include "RandGenerator.h"

int main()
{
	RandGenerator test( 1 , 1000 , 1000 , "RandTest.txt" , 3 );
	test.writeOnAFile();
	
	return 0;
}

