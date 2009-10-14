CISODIR=../rchandra/
LDFLAGS += -lcv -lhighgui -lcxcore
CXXFLAGS += -Wall -Wextra -O2 -I../rchandra/ -g
OBJECTS= Vectors.o CIsoSurface.o
TARGETS=isoinh gen morph/morph

all: $(TARGETS) tags

$(TARGETS): Vectors.o CIsoSurface.o

Vectors.o: $(CISODIR)Vectors.cpp $(CISODIR)Vectors.h
	g++ $(CXXFLAGS) -c $(CISODIR)Vectors.cpp $(LDFLAGS)

CIsoSurface.o: Vectors.o $(CISODIR)CIsoSurface.cpp $(CISODIR)CIsoSurface.h
	g++ $(CXXFLAGS) -c $(CISODIR)CIsoSurface.cpp $(LDFLAGS)

tags: $(TARGETS)
	gtags -v

clean:
	rm -rf *~ *.o $(TARGETS) 

