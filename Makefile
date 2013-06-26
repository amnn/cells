CC         = g++-4.7 --std=c++0x
GLEW 	   = `pkg-config --libs glew`
GLFW 	   = `pkg-config --libs libglfw`
DEST 	   = bin/cells
INCLUDES   = -Iincludes
SRCFILES   = src/*

all:
	$(CC) $(GLEW) $(GLFW) $(INCLUDES) -o $(DEST) $(SRCFILES)