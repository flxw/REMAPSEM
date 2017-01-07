ifndef CC
  CC = gcc-4.9
endif

ifndef FC
  FC = gfortran-4.9
endif

ifndef STREAM_ARRAY_SIZE
  CFLAGS = -O2 -fopenmp
else
  CFLAGS = -O2 -fopenmp -mcmodel=large -DSTREAM_ARRAY_SIZE=$(STREAM_ARRAY_SIZE)
endif

ifndef STREAM_ARRAY_SIZE
  FFLAGS = -O2 -fopenmp
else
  FFLAGS = -O2 -fopenmp -DSTREAM_ARRAY_SIZE=$(STREAM_ARRAY_SIZE)
endif

all: stream_f.exe stream_c.exe

stream_f.exe: stream.f mysecond.o
	$(CC) $(CFLAGS) -c mysecond.c
	$(FC) $(FFLAGS) -c stream.f
	$(FC) $(FFLAGS) stream.o mysecond.o -o stream_f.exe

stream_c.exe: stream.c
	$(CC) $(CFLAGS)  stream.c -o stream_c.exe

clean:
	rm -f stream_f.exe stream_c.exe *.o

# an example of a more complex build line for the Intel icc compiler
stream.icc: stream.c
	icc -O3 -xCORE-AVX2 -ffreestanding -qopenmp -DSTREAM_ARRAY_SIZE=80000000 -DNTIMES=20 stream.c -o stream.omp.AVX2.80M.20x.icc
