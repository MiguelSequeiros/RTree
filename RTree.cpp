#include "RTree.h"
#include <vector>

RTree::RTree( int N , int M ) : N(N) , M(M) , m(M/2)
{
	/*
		Inicializamos nuestra raiz, esta apunta a un nodo sin padre
	*/
	this->root = new Node( 0 , true );
};

void RTree::insert( Mbr mbr_n , Data data_object )
{
	/*	
		Insertamos una nueva entrada, tenemos que considerar que una entrada 
		esta conformada por un Mbr y un Data
	*/
	Node* selectedLeaf = chooseLeaf( mbr_n );

	/* Ahora vamos a verificar si no se ocasiona un oveflow */
	if( selectedLeaf->entries.size() < M )
	{
		putEntry( selectedLeaf , mbr_n , data_object );		// introducimos la entrada
		Node *LL = 0;										// un comodin para invocar adjustTree
		Mbr *mbr_LL = 0;									// un comodin para invocar adjustTree
		adjustTree( *selectedLeaf , *LL , mbr_n , *mbr_LL );
		return;
	}
	else
	{
		/* En caso haya un overflow procedemos a hacer splitNode*/
		Node *L = selectedLeaf;			
		Node *LL = new Node( 0 , true ); // aun no sabemos si compartiran padres por eso pasamos 0 como parametro

		/* Creamos una nueva entrada */
		Entry e = Entry( mbr_n , (void*)&data_object );		

		/* Declaramos para inicializar los mbr dentro de splitNode y asi no tener que hacer dos veces el mismo trabajo*/
		Mbr *mbr_L , *mbr_LL;

		/* LLamamos a la funcion splitNode */
		splitNode( *L , *LL , &e , *mbr_L , *mbr_LL);

		/* Llamamos a la funcion adjustTree y si nos dice que hubo un split en la raiz (rootSplitted) creamos otra raiz*/
		if( adjustTree( *L , *LL , *mbr_L , *mbr_LL ) )		
		{
			this->root = new Node( 0 , false );
			this->root->entries.push_back( Entry (  *mbr_L , (void*)  &L ));
			this->root->entries.push_back( Entry ( *mbr_LL , (void*) &LL ));
		}

	}
};

void RTree::putEntry( Node* selectedLeaf , Mbr mbr_n , Data data_object )
{
	/* KISS */
	selectedLeaf->entries.push_back(Entry( mbr_n , (void*) &data_object ));
};

Node* RTree::chooseLeaf( Mbr mbr_i )
{
	/*
		Como su nombre describe, esta funcion escoge una hoja en la que aniadiremos la nueva entrada
	*/

	Node* tmp = this->root;

	while( !tmp->isLeaf )
	{

		float minMbrArea = tmp->entries[0].mbr->findArea( this->N );
		float minEnlargementArea = tmp->entries[0].mbr->findNewArea( mbr_i , this->N ) - minMbrArea;
		int index = 0;


		for(int i=1 ; i<tmp->entries.size() ; i++) /* O (M) */
		{
			float tmpMbrArea = tmp->entries[i].mbr->findArea( this->N );
			float tmpEnlargementArea = tmp->entries[i].mbr->findNewArea( mbr_i , this->N );

			if( tmpEnlargementArea < minEnlargementArea )
				index = i;

			else if( tmpEnlargementArea == minEnlargementArea )
			{
				if( tmpMbrArea < minMbrArea )
					index = i;
			}
		}
		/*
			Sabemos que cada entrada se compone por un
		*/
		tmp = static_cast<Node*>( tmp->entries[index].child );
	}

	return tmp;
};

void RTree::splitNode( Node& L , Node& LL , Entry *e , Mbr& mbr_L , Mbr& mbr_LL ) /*Quadratic split node*/
{
	/*
		Recibimos como parametros dos nodos:
			L y LL (segun la notacion del paper original) ambos son nodos hoja:
			
		Con los nodos vacios y las antiguas entradas de L en el vector rep,
		procedemos a escoger dos semillas de rep...
	*/

	/* Ahora vamos a limpiar el nodo que apunta L y pondremos esas entradas junto con la entrada a insertar	*/
	
	std::vector<Entry> rep;

	for(int i=0 ; i<L.entries.size() ; i++)  	// aqui aniadimos las entradas de L a un repositorio
		rep.push_back( L.entries[i] );

	L.entries.clear();						// aqui limpiamos el nodo L

	rep.push_back( *e ); 					// aqui aniadimos la nueva entrada a insertar

	int i_s1 , i_s2; 						// aqui guardaremos el indice de cada semilla

	/*Invocamos pickSeeds y asignaremos las entradas a un nodo cada una, luego las eliminaremos de nuestro repositorio */

	pickSeeds( &i_s1 , &i_s2 , rep );

	L.entries.push_back( rep[i_s1] );	// asignamos la entrada al nodo L
	LL.entries.push_back( rep[i_s2] );	// asignamos la entrada al nodo LL

	mbr_L = Mbr( *rep[i_s1].mbr );	// estamos definiendo un MBR para L
	mbr_LL = Mbr( *rep[i_s2].mbr );	// lo mismo para LL

	/* 	El porque eliminamos primero la entrada rep[i_s2] es porque i_s2 siempre sera mayor que i_s1, 
		al eliminar una entrada en un vector todo se refresca, entonces eliminar una entrada en i_s1 
		movería todo el vector :O */
	rep.erase( rep.begin() + i_s2 );
	rep.erase( rep.begin() + i_s1 );

	/* Ahora vamos a asignar cada entrada restante a un nodo segun corresponda */
	while( rep.size()!=0 ) 				// "If all entries have been assigned" es decir cuando el rep este vacio
	{

		if( L.entries.size() + rep.size() == this->M )
		{
			for( int i=0 ; i<rep.size() ; i++ )
				L.entries.push_back( rep[i] );

			rep.clear();
		}
		else if( LL.entries.size() + rep.size() == this->M )
		{
			for( int i=0 ; i<rep.size() ; i++ )
				LL.entries.push_back( rep[i] );

			rep.clear();
		}
		else
		{
			int i_n; 						// este es sera el indice de la siguiente entrada
			pickNext( &mbr_L , &mbr_LL , &i_n , rep );

			/* En primera instancia seleccionaremos el nodo aniadir de acuerdo al area de enlargue que se necesitara */
			float EnlargementArea_L  = mbr_L.findNewArea( rep[i_n].mbr , this->N ) - mbr_L.findArea( this->N );
			float EnlargementArea_LL = mbr_LL.findNewArea( rep[i_n].mbr , this->N ) - mbr_LL.findArea( this->N );

			if( EnlargementArea_L < EnlargementArea_LL )
			{
				L.entries.push_back( rep[i_n] );
				mbr_L.refreshIntervals( rep[i_n].mbr , this->N );
			}

			else if( EnlargementArea_LL < EnlargementArea_L)
			{
				LL.entries.push_back( rep[i_n] );
				mbr_LL.refreshIntervals( rep[i_n].mbr , this->N );
			}

			else // ( EnlargementArea_LL == EnlargementArea_L )
			{
				/* En caso haya una igualdad pasaremos a elegir el nodo de acuerdo al area de cada uno*/
				if( mbr_L.findArea( this->N ) < mbr_LL.findArea( this->N ) )
				{
					L.entries.push_back( rep[i_n] );
					mbr_L.refreshIntervals( rep[i_n].mbr , this->N );
				}

				else if( mbr_LL.findArea( this->N ) < mbr_L.findArea( this->N ) )
				{
					LL.entries.push_back( rep[i_n] );
					mbr_LL.refreshIntervals( rep[i_n].mbr , this->N );
				}

				else // ( mbr_LL.findArea( this->N ) == mbr_L.findArea( this->N ) )
				{
					/* Por ultimo, escogeremos de acuerdo al numero de entradas de cada nodo */
					if( L.entries.size() < LL.entries.size() )
					{
						L.entries.push_back( rep[i_n] );
						mbr_L.refreshIntervals( rep[i_n].mbr , this->N );
					}

					else
					{
						LL.entries.push_back( rep[i_n] );
						mbr_LL.refreshIntervals( rep[i_n].mbr , this->N );
					}
				}
			}

			rep.erase( rep.begin() + i_n );
		}
	}


	return;
};

void RTree::pickSeeds( int *i_s1 , int *i_s2 , std::vector<Entry> rep )
{
	/*
		En pickSeeds comparamos el costo de cada emparejamiento, por lo que: O(M+1)^2,
		retornaremos los indices de los pares mas costosos
	*/

	float maxEnlargementArea = rep[0].mbr->findNewArea( *rep[1].mbr , this->N ) - rep[0].mbr->findArea( this->N );
	*i_s1 = 0;
	*i_s2 = 1;

	for( int i=0 ; i<rep.size() ; i++ )
	{
		for( int j=i+1 ; i < rep.size() ; i++ )
		{
			float tmpEnlargementArea = rep[i].mbr->findNewArea( *rep[j].mbr , this->N ) - rep[i].mbr->findArea( this->N );

			if( tmpEnlargementArea > maxEnlargementArea )
			{
				maxEnlargementArea = tmpEnlargementArea;
				*i_s1 = i;
				*i_s2 = j;				
			}
		}
	}
};

void RTree::pickNext( Mbr *mbr_L , Mbr *mbr_LL , int* i_n , std::vector<Entry> rep)
{
	/*
		En pickNext seleccionamos de nuestro repositorio la siguiente entrada a ser asignada,
		nos basaremos en el area de enlarge que se necesitaria para aniadirla en cada nodo,
		seleccionaremos a aquel que tenga la maxima diferencia entre tales areas de enlarge
	*/
	float d1 = mbr_L->findNewArea( *rep[0].mbr , this->N ) - mbr_L->findArea( this->N );
	float d2 = mbr_LL->findNewArea( *rep[0].mbr , this->N ) - mbr_LL->findArea( this->N );

	float max_d = d2>d1 ? d2-d1 : d1-d2;

	for( int i=1 ; i<rep.size() ; i++ )
	{
		d1 = mbr_L->findNewArea( *rep[i].mbr , this->N ) - mbr_L->findArea( this->N );
		d2 = mbr_LL->findNewArea( *rep[i].mbr , this->N ) - mbr_LL->findArea( this->N );

		float tmp_d = d2>d1 ? d2-d1 : d1-d2;

		if( tmp_d > max_d )
		{
			max_d = tmp_d;
			*i_n = i;
		}
	}
};

bool RTree::adjustTree( Node& L , Node& LL , Mbr& mbr_L , Mbr& mbr_LL )
{
	bool rootSplitted = false;

	Node* PP;
	Node* N  = &L;
	Node* NN = &LL;
	Mbr* mbr_N = &mbr_L;
	Mbr* mbr_NN = &mbr_LL;

	while( this->root != N )
	{
		Node* P = N->parent; 	// "Let P be the parent of N "

		/* En este bucle copiamos el indice de de la entrada que contiene a N */
		int i_e;
		for( int i=0 ; i<P->entries.size() ; i++ )
		{
			if( P->entries[i].child == &N )
			{
				i_e = i;
				break;
			}
		}

		P->entries[i_e].mbr->refreshIntervals( *mbr_N , this->N );

		/* En caso exista una entrada NN producto de un splitNode */
		if( NN ) 
		{
			Entry* e_NN = new Entry( *mbr_NN , (void*)NN);	// Creamos una entrada que contiene a NN

			if( P->entries.size() < M )
			{
				/* Si hay espacio para al menos una entrada */
				P->entries.push_back( *e_NN );
				N=P;
				NN=0;
				rootSplitted = false;
			}
			
			else
			{
				/* Caso contrario invocamos un splitNode */
				splitNode( *N , *NN , e_NN , *mbr_N , *mbr_NN );
				P = N;
				PP = NN;
				rootSplitted = true;
			}
		}

		else
			N = P;
	}

	/* "Ingenieria" inversa */
	L  = *N;
	LL = *NN;
	mbr_L  = *mbr_N;
	mbr_LL = *mbr_NN;

	return rootSplitted;
};

void RTree::Delete( Entry e )
{
	Node* L = findLeaf( e );
	/* Verificamos si el nodo que retorno findLeaf nos */
	if( L )
	{	
		/* Aqui borramos la entrada*/
		for( int i=0 ; i<L->entries.size() ; i++ )
		{
			if( L->entries[i].mbr == e.mbr )
			{
				L->entries.erase( L->entries.begin() + i );
				break;
			}
		}
		/* Invocamos condenseTree */
	}
	else
		return; 	//stop
};

Node* RTree::findLeaf( Entry e )
{
	Node* T = this->root;
	std::vector<Entry*> rep; // repositorio con las entradas que pasaron el checkOverlap

	while( !T->isLeaf )
	{
		for( int i=0 ; i < T->entries.size() ; i++ )
		{
			if( T->entries[i].mbr->checkOverlap( e.mbr , this->N ) )
				rep.push_back( &T->entries[i] );
		}

		T = static_cast<Node*>(rep[0]->child);
		rep.erase(rep.begin());
	}

	for( int i=0 ; T->entries.size() ; i++ )
	{
		if( T->entries[i].mbr == e.mbr )
			return T;
	}

	return 0;
};

void RTree::condenseTree( Node& L )
{
	Node* N 	= &L;
	//Mbr* mbr_N 	= &mbr_L;			// "Set N = L"
	std::vector<Node*> Q;	// "Set Q, the set of eliminated nodes, to be empty"

	while( this->root != N )
	{
		Node* P = N->parent;
		int i_e;
		for( int i ; i<P->entries.size() ; i++ )
		{
			if( P->entries[i].child == &L )
			{
				i_e = i;
				break;
			}
		}
		Entry* e_N = &P->entries[i_e];
		Mbr* mbr_N = P->entries[i_e].mbr;

		/* CT3 : "If N has fewer than m entries ... " */
		if( N->entries.size() < m ) // Hurra! por fin usamos m =)
		{
			P->entries.erase( P->entries.begin() + i_e );	// delete e_N from P
			Q.push_back( N );								// add N to set Q
		}
		/* CT4 : "IF N has not been eliminated , adjust EnI (mbr_N) to tightly contain all entries in N" */
	//	else
	//	{
			//P->entries[i_e].mbr->refreshIntervals( )
	//	}
	}
};

RTree::~RTree(){};
