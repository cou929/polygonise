CISODIR=../rchandra/
WORKDIR=./
INCDIR=-I../rchandra/
LIBDIR=
CXX_FLAGS=-Wall -O2

all: isoinh

$(WORKDIR)Vectors.o: 
	g++ $(CXX_FLAGS) -c $(CISODIR)Vectors.cpp

$(WORKDIR)CIsoSurface.o: Vectors.o 
	g++ $(CXX_FLAGS) -c $(CISODIR)CIsoSurface.cpp

$(WORKDIR)isoinh: Vectors.o CIsoSurface.o
	g++ $(CXX_FLAGS) -o isoinh isoinh.cpp $(INCDIR) $(LIBDIR)

clean:
	rm *~ *.o