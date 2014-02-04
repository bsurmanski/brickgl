SRC=\
src/main.cpp\
src/matrix.cpp\
src/vector.cpp\
src/framework/sdlWindow.cpp\
src/framework/window.cpp\
src/framework/input/sdlInputDevice.cpp\
src/framework/input/inputDevice.cpp\
src/framework/draw/glDrawDevice.cpp\
src/framework/draw/glDrawShader.cpp\
src/framework/draw/drawProgram.cpp\
src/framework/draw/drawDevice.cpp\
src/framework/draw/glDrawProgram.cpp\
src/framework/draw/mesh.cpp\
src/framework/draw/glMesh.cpp\
src/framework/draw/objMesh.cpp\
src/framework/application.cpp

GLSL=src/glsl/default.fs\
	 src/glsl/default.vs\
	 src/glsl/test.fs\
	 src/glsl/test.vs\
	 src/glsl/cube.fs\
	 src/glsl/cube.vs

GLSLH = $(patsubst %.vs,%.vs.h,$(patsubst %.fs,%.fs.h,$(GLSL)))

CFLAGS= -g -std=gnu++11
LDFLAGS= -lSDL -lGL -lm -lc

%.fs.h: %.fs
	(cat $<; printf "\0") | xxd -i > $@

%.vs.h: %.vs
	(cat $<; printf "\0") | xxd -i > $@

%.glsl.h: %.glsl
	xxd -i $< $@

all: $(SRC) $(GLSLH)
	g++ $(SRC) $(CFLAGS) $(LDFLAGS) -o brickgl

clean:
	rm -f brickgl
	rm -f $(GLSLH)
