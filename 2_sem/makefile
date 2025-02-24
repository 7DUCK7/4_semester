CFLAGS = -Wall -g -O
OBJ = cat.o dog.o animal.o main.o
A = animals
$(A): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
clean:
	$(RM) $(OBJ)