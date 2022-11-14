all: 
	mpicc psum_scan.c -o scan
	mpicc psum_blocking.c -o blocking
	mpicc psum_nblocking.c -o nonblocking

clean:
	rm scan blocking nonblocking 
