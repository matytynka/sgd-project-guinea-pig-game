IDIR = include
LDIR = lib
ODIR = build
SDIR = src

CC=g++
CFLAGS=-I$(IDIR) -L$(LDIR) -w -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -std=c++14 -o $@

_DEPS = 
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = world.o test.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.cpp $(DEPS)
	$(CC) -c $< $(CFLAGS)

build/game: $(OBJ)
	$(CC) $^ $(CFLAGS)