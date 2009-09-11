CISODIR=../rchandra/
WORKDIR=./
INCDIR=-I../rchandra/
LIBDIR=
CXX_FLAGS=-Wall -O2
OBJECTS= Vectors.o CIsoSurface.o

all: isoinh gen

$(WORKDIR)Vectors.o: $(CISODIR)Vectors.cpp $(CISODIR)Vectors.h
	g++ $(CXX_FLAGS) -c $(CISODIR)Vectors.cpp $(INCDIR) $(LIBDIR)

$(WORKDIR)CIsoSurface.o: Vectors.o $(CISODIR)CIsoSurface.cpp $(CISODIR)CIsoSurface.h
	g++ $(CXX_FLAGS) -c $(CISODIR)CIsoSurface.cpp $(INCDIR) $(LIBDIR)

$(WORKDIR)isoinh: Vectors.o CIsoSurface.o isoinh.cpp
	g++ $(CXX_FLAGS) -o isoinh isoinh.cpp $(INCDIR) $(LIBDIR) $(OBJECTS)

$(WORKDIR)gen: Vectors.o CIsoSurface.o gen.cpp
	g++ $(CXX_FLAGS) -o gen gen.cpp $(INCDIR) $(LIBDIR) $(OBJECTS)

clean:
	rm *~ *.o