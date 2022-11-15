#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>


int main(int argc, char **argv){

	int rank,numprocs, namelen;
	double start, end;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name,&namelen);

	if(rank == 0) 
    	start = MPI_Wtime();
  

	// int my_data = rank;
	srand ((rank+1) * time(NULL));
	int my_data = rand() % 100;

	int my_sum;
    MPI_Scan(&my_data, &my_sum, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	// printf("rank %d : my_sum is %d\n", rank, my_sum);

  	MPI_Barrier(MPI_COMM_WORLD);
    printf("Process %d out of %d\n", rank, numprocs);
  	if(rank == 0) {
    	end = MPI_Wtime();
    	printf("Elapsed time = %lf sec\n", end - start);
  	}

	MPI_Finalize();

  	return 0;
}
