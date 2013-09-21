CC         = g++-4.9 --std=c++11
GLEW 	   = `pkg-config --static --libs glew`
GLFW 	   = `pkg-config --static --libs glfw3`
DEST 	   = bin/cells
INCLUDES   = -Iincludes
SRCFILES   = src/*

all:
	$(CC) $(GLEW) $(GLFW) $(INCLUDES) -o $(DEST) $(SRCFILES)
