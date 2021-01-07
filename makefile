CC=gcc
IDIR=./include
SDIR=./src
ODIR=./build
CFLAGS=-I. \
       -I$(IDIR) \
       -I./release \
       -Wall \

DEP=$(wildcard $(IDIR)/*.h)
SRC=$(wildcard $(SDIR)/*.c)
OBJ = $(patsubst %.c, %.o, $(patsubst $(SDIR)/%, $(ODIR)/%, $(SRC)))

# Rebuild the executable if any underlying object file changes or does not
# exist.
all: $(OBJ)
	$(CC) -pthread $(OBJ) -L./release/build -lftd2xx -o $(ODIR)/main.exe

# Rebuild object files if an underlying source or header file changes.
$(ODIR)/%.o: $(SDIR)/%.c $(DEP) $(ODIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(ODIR):
	$(shell mkdir -p $(ODIR))

doc: $(SRC)
	$(shell doxygen)

clean:
	rm -rf $(ODIR) ./doc/html ./doc/latex
