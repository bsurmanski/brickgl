SRC=\
src/main.cpp\
src/brick.cpp\
src/matrix.cpp\
src/vector.cpp\
src/framework/sdlWindow.cpp\
src/framework/window.cpp\
src/framework/input/sdlInputDevice.cpp\
src/framework/input/inputDevice.cpp\
src/framework/draw/camera.cpp\
src/framework/draw/drawProgram.cpp\
src/framework/draw/drawDevice.cpp\
src/framework/draw/glTexture.cpp\
src/framework/draw/glFramebuffer.cpp\
src/framework/draw/image.cpp\
src/framework/draw/glDrawDevice.cpp\
src/framework/draw/glMesh.cpp\
src/framework/draw/glDrawShader.cpp\
src/framework/draw/glDrawProgram.cpp\
src/framework/draw/objFormat.cpp\
src/framework/draw/pngFormat.cpp\
src/framework/draw/mesh.cpp\
src/framework/application.cpp

GLSL=\
	 src/glsl/light.fs\
	 src/glsl/light.vs\
	 src/glsl/mesh.fs\
	 src/glsl/mesh.vs\
	 src/glsl/deferred.fs\
	 src/glsl/deferred.vs

GLSLH = $(patsubst %.vs,%.vs.h,$(patsubst %.fs,%.fs.h,$(GLSL)))

CFLAGS= -g -std=gnu++11 -Wno-narrowing
LDFLAGS= -lSDL -lSDL_image -lGL -lm -lc

%.fs.h: %.fs
	(cat $<; printf "\0") | xxd -i > $@

%.vs.h: %.vs
	(cat $<; printf "\0") | xxd -i > $@

%.glsl.h: %.glsl
	xxd -i $< $@

all: $(SRC) $(GLSLH)
	ctags -R -o .git/ctags
	g++ $(SRC) $(CFLAGS) $(LDFLAGS) -o brickgl

clean:
	rm -f brickgl
	rm -f $(GLSLH)
