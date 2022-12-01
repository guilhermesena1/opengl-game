PROGS = game
CXX = g++
CC = g++
CXXFLAGS = -O3 -Wall
CPPFLAGS = # includes etc
LDFLAGS = # linkers etc
LDLIBS = -ldl -lglfw3

# Need glfw from here: https://github.com/glfw/glfw
all : $(PROGS)

%.o: %.cpp %.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< $(CPPFLAGS)

%.o: %.c %.h
	$(CC) $(CXXFLAGS) -c -o $@ $< $(CPPFLAGS)

$(PROGS) : game.o glad.o
	$(CXX) $(CXXFLAGS) -o $@  $^ $(CPPFLAGS) $(LDLIBS) $(LDFLAGS)

install: $(PROGS)
	@install -m 755 $(PROGS) $(SRC_ROOT)

clean:
	@-rm -f $(PROGS) *.o *.so *.a *~

.PHONY: clean

