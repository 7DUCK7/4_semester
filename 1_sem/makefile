CFLAGS = -Wall -g -O
OBJ = dictionary.o parser.o word.o main.o
A = main
$(A): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^
clean:
	$(RM) $(OBJ)