#ifndef ENTRY_H
#define ENTRY_H

#include "Mbr.h"
#include <iostream>


/*
	Cada entrada va a almacenar un mbr y un puntero vacio,
	hay dos casos:
		(1) la entrada pertenece a un nodo hoja: en este caso apuntara a un Objeto de la clase Data
		(2) la entrada pertenece a un nodo no-hoja: en este caso apuntara a otro nodo, su hijo
*/
		
class Entry
{

	friend class RTree;

private:
	Mbr* mbr;
	void* child;

public:
	Entry( Mbr , void*);
	~Entry();

};

Entry::Entry( Mbr mbr , void* child)
{
	std::cout << "Starting Entry creation ... " << std::endl;
	this -> mbr = &mbr;
	this -> child = child;
	std::cout << "Entry created " << std::endl;
};
Entry::~Entry() 
{
	std::cout << "Entry destroyed" << std::endl;
};
#endif // ENTRY_H
