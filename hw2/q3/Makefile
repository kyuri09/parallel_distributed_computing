all: parallel sequential

sequential:
	gcc ppm.c sequential.c -o sequential

parallel:
	mpicc ppm.c parallel.c -o parallel

clean:
	rm -f parallel sequential

