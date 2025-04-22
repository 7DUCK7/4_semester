CFLAGS =
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system
OBJ = GUI.o sprites.o main.o map.o block.o
A = tank
$(A): $(OBJ)
	$(CXX) $(CFLAGS) -o $@ $^ $(SFML_FLAGS)
clean:
	$(RM) $(OBJ)
