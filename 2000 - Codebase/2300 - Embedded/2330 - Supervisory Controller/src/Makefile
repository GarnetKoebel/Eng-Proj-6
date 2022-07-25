IDIR =../include
LDIR =-L/usr/lib/arm-linux-gnueabihf
LDIR2 = -L/usr/include
LDIR3 = -L/usr/include/cppconn
CC=g++
CFLAGS=-I$(IDIR)
ODIR=obj

LIBS=-l:libmysqlcppconn.so -lpcan

_OBJ = main.o CAN.o Supervisor.o linkedList.o databaseFunctions.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp
	$(CC) -g -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	g++ -g -o $@ $^ $(CFLAGS) $(LDIR) $(LDIR2) $(LDIR3) $(LIBS)

.PHONY: clean

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~
