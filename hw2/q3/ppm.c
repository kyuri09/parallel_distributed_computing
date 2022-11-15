#include <stdio.h>
#include <stdlib.h>
#include "ppm.h"

Pixel *flip(Pixel *image, int w, int h, int rank)
{
	int r,c;

	Pixel *res = (Pixel*)malloc(sizeof(Pixel)*w*h);

	for(r=0;r<h;r++)
	{
		for(c=0;c<w;c++)
		{
			int tmp=(r+1)*w-c;
			int idx=r*w+c;
			res[idx]=image[tmp];
		}
	}
	printf("Process %d finished flipping\n",rank);

	return res;
}
Pixel *grayscale(Pixel *image, int w, int h, int rank)
{
	int i;
	unsigned char average;

	Pixel *res = (Pixel*)malloc(sizeof(Pixel)*w*h);

	for(i=0;i<w*h;i++)
	{
		average=(image[i].rgb[0]+image[i].rgb[1]+image[i].rgb[2])/3;
		res[i].rgb[0]=average;
		res[i].rgb[1]=average;
		res[i].rgb[2]=average;
	}
	printf("Process %d finished grayscaling\n",rank);

	return res;
}
Pixel *smooth(Pixel *image, Pixel *up_low_row, int w, int h, int rank, int numtasks)
{
    int i, j, r, c, count, index = 0;
    double r_sum, g_sum, b_sum;
    int idx;
    Pixel *res = (Pixel *)malloc(sizeof(Pixel)*w*h);

    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {   
            count = 9;
            r_sum = 0;
            g_sum = 0;
            b_sum = 0;
            for (r = 0; r < 3; r++)
            {
                for (c = 0; c < 3; c++)
                {
                    if (up_low_row != NULL)
                    {
                        if ((i - 1 + r) < 0 && rank != 0 && (j - 1 + c) >= 0 && (j - 1 + c) < w)
                        {
							idx=j-1+c;
                            r_sum += up_low_row[idx].rgb[0];
                            g_sum += up_low_row[idx].rgb[1];
                            b_sum += up_low_row[idx].rgb[2];
                        }
                        else if ((i - 1 + r) >= h && rank != (numtasks - 1) && (j - 1 + c) >= 0 && (j - 1 + c) < w)
                        {
                            if (rank != 0)
                            {
								idx=w+j-1+c;
                                r_sum += up_low_row[idx].rgb[0];
                                g_sum += up_low_row[idx].rgb[1];
                                b_sum += up_low_row[idx].rgb[2];                            
                            }
                            else {
								idx=j-1+c;
                                r_sum += up_low_row[idx].rgb[0];
                                g_sum += up_low_row[idx].rgb[1];
                                b_sum += up_low_row[idx].rgb[2];                            
                            }
                        }
                        else if ((i - 1 + r) >= 0 && (i - 1 + r) < h && (j - 1 + c) >= 0 && (j - 1 + c) < w)
                        {
							idx=(i-1+r)*w + (j-1+c);
                            r_sum += image[idx].rgb[0];
                            g_sum += image[idx].rgb[1];
                            b_sum += image[idx].rgb[2];
                        }
                        else {
                            count--;
                        }
                    }
                    else {
                        if ((i - 1 + r) >= 0 && (i - 1 + r) < h && (j - 1 + c) >= 0 && (j - 1 + c) < w)
                        {
							idx=(i-1+r)*w + (j-1+c);
                            r_sum += image[idx].rgb[0];
                            g_sum += image[idx].rgb[1];
                            b_sum += image[idx].rgb[2];
                        }
                        else {
                            count--;
                        }
                    }
                }
            }
            res[index].rgb[0] = (unsigned char) (r_sum / count);
            res[index].rgb[1] = (unsigned char) (g_sum / count);
            res[index].rgb[2] = (unsigned char) (b_sum / count);
            index++;
        }
    }
    printf("Process %d finished smoothing\n", rank);
    return res;
}

