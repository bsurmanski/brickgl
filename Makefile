SRC=\
	src/main.c\

CFLAGS= -DGL_GLEXT_PROTOTYPES -g
LDFLAGS= -lglfw -lGL

all:
	gcc $(SRC) $(CFLAGS) $(LDFLAGS) -o brickgl
