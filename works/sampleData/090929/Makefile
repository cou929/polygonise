CC=g++
LIB=-lcv -lcxcore -lhighgui
LIBDIR=-L/opt/local/lib
INC=-I/opt/local/include
CFLAGS=-Wall -O2
TARGETS=twoDimView imageValFeature

all: $(TARGETS)

.cpp:
	$(CC) $(CFLAGS) -o $@ $< $(INC) $(LIBDIR) $(LIB)

stl:
	perl generate3DDataFromImage.pl

clean:
	rm -rf *.o *~ $(TARGETS)