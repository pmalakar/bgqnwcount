#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <string.h>
#include <new>
#define MAXBUF (1024*32)
#define xMicroSec 1000000

using namespace std; 

// Functions

//adapted verbatim from https://wiki.alcf.anl.gov/parts/index.php/Blue_Gene/Q#Allocating_Memory
void * bgq_malloc(size_t n)
{
    void * ptr;
    size_t alignment = 32; /* 128 might be better since that ensures every heap allocation 
                            * starts on a cache-line boundary */
    posix_memalign( &ptr , alignment , n );
    return ptr;
}


//data used for computation, analysis, IO etc..

class dataBlock {

 private:	
	double *alpha, *beta, *gamma;
//    double U[1024][1024], V[1024][1024], W[1024][1024];
	double **U, **V, **W;
	int numElem;
	int latsize, lonsize;

 public:
	dataBlock() {}
	dataBlock(int count) {

		numElem = count;

	}

	void allocElement (int type) {
		
		if (type == 1) {
		  try {
			alpha = new double[numElem];
			for (int i=0; i<numElem ; i++) {
				alpha[i] = rand() % 100;
			}
		  }
		  catch (bad_alloc& ba) {
				cerr << "Bad allocation for alpha\n" << ba.what() << endl;
		  }
		}
	}

	void freeElement (int type) {

		if (type == 1) delete [] alpha;
	}

	double *getAlphaBuffer() {
		return &alpha[0];
	}

};


/* Allocate and free memory before and after use */ //Though this is bit of overhead but .. Just to ensure there is no spatial locality interference affecting the statistics
    
void alloc_free (dataBlock *datum, int type) {
    
    if (type == 1) {

		datum->allocElement(1);
		datum->allocElement(2);
		datum->allocElement(3);

    }
    else {

		datum->freeElement(1);
		datum->freeElement(2);
		datum->freeElement(3);

    }

}


void prnerror (int error, char *string)
{
    fprintf(stderr, "Error %d in %s\n", error, string);
    MPI_Finalize();
    exit(-1);
}

