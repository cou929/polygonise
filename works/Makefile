CISODIR=../rchandra/
WORKDIR=./
INCDIR=-I../rchandra/ -I/opt/local/include
LIBDIR=-L/opt/local/lib
CXX_FLAGS=-Wall -Wextra -O2
OBJECTS= Vectors.o CIsoSurface.o
OPENCVLIB=-lcv -lhighgui -lcxcore

all: isoinh gen morph/morph

$(WORKDIR)Vectors.o: $(CISODIR)Vectors.cpp $(CISODIR)Vectors.h
	g++ $(CXX_FLAGS) -c $(CISODIR)Vectors.cpp $(INCDIR) $(LIBDIR)

$(WORKDIR)CIsoSurface.o: Vectors.o $(CISODIR)CIsoSurface.cpp $(CISODIR)CIsoSurface.h
	g++ $(CXX_FLAGS) -c $(CISODIR)CIsoSurface.cpp $(INCDIR) $(LIBDIR)

$(WORKDIR)isoinh: Vectors.o CIsoSurface.o isoinh.cpp
	g++ $(CXX_FLAGS) -o isoinh isoinh.cpp $(INCDIR) $(LIBDIR) $(OBJECTS)

$(WORKDIR)gen: Vectors.o CIsoSurface.o gen.cpp
	g++ $(CXX_FLAGS) -o gen gen.cpp $(INCDIR) $(LIBDIR) $(OBJECTS)

morph/morph: morph/morph.cpp
	g++ $(CXX_FLAGS) -o morph/morph morph/morph.cpp $(INCDIR) $(LIBDIR) $(OPENCVLIB)

clean:
	rm -rf *~ *.o
