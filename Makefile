PROGS = game
CXX = g++
CXXFLAGS = -O3 -Wall
CPPFLAGS = # includes etc
LDFLAGS = # linkers etc
LDLIBS = -ldl -lglfw
all: game

# Need glfw from here: https://github.com/glfw/glfw

all : $(PROGS)

game: src/game.cpp src/glad.c src/glad.h
	$(CXX) $(CXXFLAGS) -o $@  $^ $(CPPFLAGS) $(LDLIBS) $(LDFLAGS)

clean:
	@-rm -f $(PROGS) *.o *.so *.a *~

.PHONY: clean

