SRC=\
	src/main.cpp\
	src/matrix.cpp\
	src/vector.cpp\
	src/framework/drawDevice.cpp\
	src/framework/glDrawDevice.cpp\
	src/framework/window.cpp\
	src/framework/sdlWindow.cpp\

GLSL=

CFLAGS= -DGL_GLEXT_PROTOTYPES -g -std=gnu++11
LDFLAGS= -lSDL -lGL

%.glx: %.glsl
	xxd -i $< $@

all:
	g++ $(SRC) $(CFLAGS) $(LDFLAGS) -o brickgl
