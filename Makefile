SRC=\
	src/main.c\
	src/matrix.cpp\
	src/vector.cpp

CFLAGS= -DGL_GLEXT_PROTOTYPES -g
LDFLAGS= -lglfw -lGL

all:
	g++ $(SRC) $(CFLAGS) $(LDFLAGS) -o brickgl
