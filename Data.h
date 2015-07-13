#ifndef DATA_H
#define DATA_H

class Data
{
public:
	Data();
	int foo;
	~Data();
};

Data::Data(){ this->foo = 1; };
Data::~Data(){};

#endif // DATA_H