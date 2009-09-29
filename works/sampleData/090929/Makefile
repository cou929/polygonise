CC=g++
LIB=-lcv -lcxcore -lhighgui
LIBDIR=-L/opt/local/lib
INC=-I/opt/local/include
CFLAGS=-Wall -O2
TARGETS=twoDimView

all: $(TARGETS)

twoDimView: twoDimView.cpp
	$(CC) $(CFLAGS) -o twoDimView twoDimView.cpp $(INC) $(LIBDIR) $(LIB)

clean:
	rm -rf *.o *~ $(TARGETS)