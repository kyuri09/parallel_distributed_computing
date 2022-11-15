#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>
#include "ppm.h"

int main(int argc, char *argv[])
{
	int numtasks, rank, color;
	Pixel *input, *result, *up_low_row, *tmpImage;
	int width,height;
	FILE *ifp, *ofp;
	int mode, worksize,i;
	MPI_Status Stat;
	MPI_Datatype type;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Type_contiguous(3,MPI_UNSIGNED_CHAR, &type);
	MPI_Type_commit(&type);

	double start, end,elapsed;
		start=MPI_Wtime();
	if(rank==0){		
		if(strcmp("flip", argv[1])==0){
			mode=0;
		}
		else if(strcmp("grayscale",argv[1])==0){
			mode=1;
		}
		else if(strcmp("smooth",argv[1])==0){
			mode=2;
		}
		else{
			printf("mode: flip / graysacle / smooth\n");
			MPI_Finalize();
			return 0;
		}

		ifp=fopen(argv[2],"r");
		
		fscanf(ifp,"P6\n%d %d\n%d\n",&width,&height,&color);
		input=(Pixel*)malloc(sizeof(Pixel)*width*height);
		
		for(i=0;i<width*height;i++)
		{
			fscanf(ifp,"%c%c%c",&input[i].rgb[0],&input[i].rgb[1],&input[i].rgb[2]);
		}
	}
	MPI_Bcast(&mode,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&width,1,MPI_INT,0,MPI_COMM_WORLD);
	MPI_Bcast(&height,1,MPI_INT,0,MPI_COMM_WORLD);

	worksize=height/numtasks;
	int tmp=worksize*width;
	tmpImage = (Pixel*)malloc(sizeof(Pixel)*tmp);
	MPI_Scatter(input, tmp, type, tmpImage,tmp, type, 0, MPI_COMM_WORLD);
  
	if (mode == 2)
    {
        if (rank == 0)
        {
            int up_offset = width * worksize - width;
            int low_offset = width * worksize * 2 + width;
            
            int num;
            for (num = 1; num < numtasks - 1; num++)
            {               
                up_low_row = (Pixel *)malloc(sizeof(Pixel) * width * 2); 
                for (int index = 0; index < width; index++)
                {
                    up_low_row[index] = input[up_offset + index];
                    up_low_row[index + width] = input[low_offset + index];
                }
                MPI_Send(up_low_row, width * 2, type, num, 0, MPI_COMM_WORLD);
                up_offset += width * worksize;
                low_offset += width * worksize; 
                free(up_low_row);
            }
            
            if (num == numtasks - 1)
            {
                up_low_row = (Pixel *)malloc(sizeof(Pixel) * width);
                for (int index = 0; index < width; index++)
                {
                    up_low_row[index] = input[up_offset + index];
                }
                MPI_Send(up_low_row, width, type, num, 0, MPI_COMM_WORLD);
                free(up_low_row);
            }
            int offset = width * worksize + width;
            up_low_row = (Pixel *)malloc(sizeof(Pixel) * width);
            for (int index = 0; index < width; index++)
            {
                up_low_row[index] = input[offset + index];
            }

        }
        else if (rank == numtasks - 1)
        {
            up_low_row = (Pixel *)malloc(sizeof(Pixel) * width);
            MPI_Recv(up_low_row, width, type, 0, 0, MPI_COMM_WORLD, &Stat);    
        }
        else {
            up_low_row = (Pixel *)malloc(sizeof(Pixel) * width * 2);
            MPI_Recv(up_low_row, width * 2, type, 0, 0, MPI_COMM_WORLD, &Stat);
        }
//        MPI_Barrier(MPI_COMM_WORLD);
    }


	if(mode==0){
		result=flip(tmpImage,width,height/numtasks,rank);
	}
	else if(mode==1){
		result=grayscale(tmpImage,width,height/numtasks,rank);
	}
	else if(mode==2){
		result=smooth(tmpImage,up_low_row,width,height/numtasks,rank,numtasks);
	}

	MPI_Gather(result, width*(height/numtasks), type, input, width*(height/numtasks),type,0,MPI_COMM_WORLD);
	
	if(rank==0)
	{
		ofp=fopen(argv[3],"w");
		fprintf(ofp,"P6\n%d %d\n%d\n",width,height,255);
		for(i=0;i<width*height;i++)
		{
			fprintf(ofp,"%c%c%c",input[i].rgb[0],input[i].rgb[1],input[i].rgb[2]);
		}
	}
	
	end=MPI_Wtime();
	//end=MPI_Wtime();
//	MPI_Reduce(&end,&elapsed,1,MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);
	printf("[Parallel Program] Process %d Execution time: %lf\n",rank, end-start);

	if(rank==0){
		fclose(ifp);
		fclose(ofp);
	}
	MPI_Finalize();
	return 0;
}
