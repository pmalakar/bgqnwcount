#include <stdio.h>
#include <error.h>
#include <mpi.h>
#include "nwcnt.h"
#include "send.h"

int myrank, commsize, mode, fileSize;
int oneKB = 1024;
int totalBytes[2] = {0};

MPI_Status status;
//const int oneKB = 1024 * 1024;

int main (int argc, char **argv) {

  int count, rcount;
  int result;
  double tStart, tEnd; 

    MPI_Status status;
  MPI_Request *send_request, *recv_request;
  MPI_Status *send_status, *recv_status;

  if (argc > 1){
    int exp = atoi (argv[1]);
    count = exp * oneKB; 
  }

  MPI_Init (&argc, &argv);
  MPI_Comm_rank (MPI_COMM_WORLD, &myrank);
  MPI_Comm_size (MPI_COMM_WORLD, &commsize);

    bgpminit();

    send_status = (MPI_Status *) malloc(commsize/2 * sizeof(MPI_Status));
    recv_status = (MPI_Status *) malloc(commsize/2 * sizeof(MPI_Status));

    send_request = (MPI_Request *) malloc(commsize/2 * sizeof(MPI_Request));
    recv_request = (MPI_Request *) malloc(commsize/2 * sizeof(MPI_Request));

  dataBlock *datum = new dataBlock(count);   //initializes alpha array to random double values

  if (datum == NULL)
   return 1;

  datum->allocElement(1);

  tStart = MPI_Wtime();

    int j=-1;
    for (int dest=(myrank+1)%commsize ; dest!=(myrank+1+commsize/2)%commsize ; dest=(dest+1)%commsize) {
   ++j;
   result = MPI_Isend (datum->getAlphaBuffer(), count, MPI_DOUBLE, dest, myrank+1, MPI_COMM_WORLD, &send_request[j]);
  }

    int k=-1;
    for (int source=(myrank+commsize/2)%commsize ; source!=myrank ; source=(source+1)%commsize) {
    ++k;
   MPI_Recv (datum->getAlphaBuffer(), count, MPI_DOUBLE, source, source+1, MPI_COMM_WORLD, &status);
  }
  tEnd = MPI_Wtime();

   MPI_Waitall(commsize/2, send_request, send_status);

  if (myrank == 0)
  printf("Perf %d %d %lf\n", commsize, count, tEnd-tStart);
  
    bgpmfinalize();
    PrintCounts("Network", hNWSet, myrank);

  MPI_Finalize ();

  return 0;

}

