CC = g++
#CPPFLAGS = -Wall -O2 -march=native -ggdb -lGL -lGLU `sdl-config --cflags --libs`
CPPFLAGS = -Wall -ggdb -lGL -lGLU `sdl-config --cflags --libs`

all_: vec3.o character.o utils.o path.o config.hpp edge.o node.o graph_algorithms.o state.o transition.o
	$(CC) $(CPPFLAGS) character.o vec3.o utils.o path.o node.o edge.o world_main.cpp graph_algorithms.o state.o transition.o -o run

transition: transition.o transition.h transition.cpp state.o
	$(CC) $(CPPFLAGS) character.o state.o state.cpp

state: character.o state.h state.cpp
	$(CC) $(CPPFLAGS) character.o state.cpp

a_star: vec3.o character.o utils.o path.o config.hpp edge.o node.o graph_algorithms.o graph_algorithms.o
	$(CC) $(CPPFLAGS) character.o vec3.o utils.o path.o node.o edge.o main.cpp graph_algorithms.o graph_algorithms.o -o run

graph_algorithms: graph_algorithm.hpp graph_algorithm.cpp
	$(CC) $(CPPFLAGS) graph_algorithm.cpp

path: path.h path.cpp config.hpp
	$(CC) $(CPPFLAGS) path.cpp

vec3: vec3.cpp vec3.hpp config.hpp
	$(CC) $(CPPFLAGS) vec3.cpp
	
edge: edge.h edge.cpp node.o character.o vec3.o
	$(CC) $(CPPFLAGS) edge.cpp
	
node: node.h node.cpp character.o vec3.o
	$(CC) $(CPPFLAGS) node.cpp
	
character: utils.o character.cpp character.h config.hpp
	$(CC) $(CPPFLAGS) character.cpp
	
utils: utils.cpp utils.hpp config.hpp
	$(CC) $(CPPFLAGS) utils.cpp

clean:
	@echo Cleaning up...
	@rm -f *.o run
	@echo Done.
