#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ppm.h"

int main(int argc, char *argv[])
{
	int numtask=1, rank=0, color;
	Pixel *input, *result;
	int width,height;	
	FILE *ifp, *ofp;
	int mode;

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
		return 0;
	}
	time_t start, end;
	start=clock();
	ifp=fopen(argv[2],"r");
	fscanf(ifp,"P6\n%d %d\n%d\n",&width,&height,&color);
	input=(Pixel*)malloc(sizeof(Pixel)*width*height);

	for(int i=0;i<width*height;i++)
	{
		fscanf(ifp,"%c%c%c",&input[i].rgb[0],&input[i].rgb[1],&input[i].rgb[2]);
	}
	
	if(mode==0){
		result=flip(input,width,height,rank);
	}
	else if(mode==1){
		result=grayscale(input,width,height,rank);
	}
	else if(mode==2){
		result=smooth(input,NULL,width,height,rank,numtask);
	}
	ofp=fopen(argv[3],"w");
	fprintf(ofp,"P6\n%d %d\n%d\n",width,height,color);
	for(int i=0;i<width*height;i++)
	{
		fprintf(ofp,"%c%c%c",result[i].rgb[0],result[i].rgb[1],result[i].rgb[2]);
	}
	
	end=clock();
	printf("[Sequential Program] Execution time: %f\n",(float)(end-start)/CLOCKS_PER_SEC);
	
	fclose(ifp);
	fclose(ofp);

	return 0;
}
