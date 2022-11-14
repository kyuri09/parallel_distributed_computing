#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int tlog(int i)
{
	int res=0;
	while(i>>=1)
	{
		res++;
	}
	return res;
}
int tpow(int i)
{
	int res=1;
	while(i--)
	{
		res<<=1;
	}
	return res;
}
int main(int argc, char *argv[])
{
	int numtasks, rank, tag=0,namelen;
	double start, end;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Get_processor_name(processor_name, &namelen);

	MPI_Status Stat;

	if(rank==0)
		start=MPI_Wtime();	

	srand(rank+time(NULL));
	
	int val = rand()%100;
	int level=tlog(numtasks);
	int i, sum=val;
	int sadd;

	for(i=0;i<level;i++)
	{
		int next=rank+tpow(i);
		int prev=rank-tpow(i);
		if(next<numtasks){
			MPI_Send(&sum, 1,MPI_INT, next, tag, MPI_COMM_WORLD);
		}
		if(rank<tpow(i)) sum=sum;
		else{
			MPI_Recv(&sadd,1,MPI_INT,prev,tag,MPI_COMM_WORLD,&Stat);
			sum+=sadd;
		}
	}

	MPI_Barrier(MPI_COMM_WORLD);
	printf("Process %d out of %d\n", rank,numtasks);
	if(rank==0){
		end=MPI_Wtime();
		printf("Elasped time: %lf sec\n", end-start);
	}
	MPI_Finalize();

	return 0;
}
