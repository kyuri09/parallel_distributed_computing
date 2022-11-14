#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>


int palindrome(char **words,char *word,int cnt)
{
	int len=strlen(word);
	char *reverse;
	reverse=(char*)malloc(sizeof(char)*len);

	int i;
	for(i=0;i<len;i++)
	{
		reverse[i]=word[len-i-1];
	}
	if(strcmp(reverse,word)==0){
	//	printf("%s\n",reverse);	
	 	return 1;
	}
	for(i=0;i<cnt;i++)
	{
		if(strcmp(reverse,words[i])==0)
		{
	//	printf("%s\n",reverse);
			free(reverse);
			return 1;
		}
	}
	return 0;
}
int main(int argc, char *argv[])
{

	if(argc!=4)
	{
		printf("Usage: palindrome [num of threads] [input-file] [output-file]\n");
		return 0;
	}

	int thread_count=atoi(argv[1]);
	
	FILE *ifp=fopen(argv[2],"r");
	FILE *ofp=fopen(argv[3],"w");
	
	char tmp[30];
	int word_count=0;
	double start, end;
	start=omp_get_wtime();
	while(1)
	{
		if(fscanf(ifp,"%s",tmp)==EOF) break;
		word_count++;
	}
//	printf("%d\n",word_count);

	fseek(ifp,0,SEEK_SET);
	
	char **words;
	words=(char**)malloc(sizeof(char*)*word_count);
	int i;
	#pragma omp parallel num_threads(thread_count)
	{
		#pragma omp for
		for(i=0;i<word_count;i++)
		{
			words[i]=(char*)malloc(sizeof(char)*30);
			fscanf(ifp,"%s",words[i]);
		}

		#pragma omp for
		for(i=0;i<word_count;i++)
		{
			if(palindrome(words,words[i],word_count)){
					fprintf(ofp,"%s\n",words[i]);
			}
		}
		#pragma omp for
		for(i=0;i<word_count;i++)
		{
			free(words[i]);
		}
		
	}
	end=omp_get_wtime();
	fclose(ifp);
	fclose(ofp);

	printf("elapsed time: %f\n", end-start);
	return 0;
}

