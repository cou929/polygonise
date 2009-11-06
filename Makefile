TARGETS=stlGenerator sphereGenerator
LDFLAGS += -lcv -lhighgui -lcxcore
CXXFLAGS += -Wall -Wextra -O2

all: $(TARGETS)

$(TARGETS): Vectors.o CIsoSurface.o

clean:e
	rm -rf *~ *.o $(TARGETS)
