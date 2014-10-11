SRCFILES=\
main.cpp\
brick.cpp\
matrix.cpp\
vector.cpp\
box.cpp\
framework/sdlWindow.cpp\
framework/window.cpp\
framework/input/sdlInputDevice.cpp\
framework/input/inputDevice.cpp\
framework/draw/camera.cpp\
framework/draw/drawProgram.cpp\
framework/draw/drawDevice.cpp\
framework/draw/glTexture.cpp\
framework/draw/glFramebuffer.cpp\
framework/draw/image.cpp\
framework/draw/glDrawDevice.cpp\
framework/draw/glMesh.cpp\
framework/draw/glDrawShader.cpp\
framework/draw/glDrawProgram.cpp\
framework/draw/objFormat.cpp\
framework/draw/pngFormat.cpp\
framework/draw/mesh.cpp\
framework/application.cpp\
brickMenu.cpp

SRC:=$(foreach file, $(SRCFILES), src/$(file))
OBJ:=$(foreach file, $(SRCFILES), build/$(file:.cpp=.o))
DEP:=$(foreach file, $(SRCFILES), build/$(file:.cpp=.d))

GLSL=\
	 src/glsl/light.fs\
	 src/glsl/light.vs\
	 src/glsl/mesh.fs\
	 src/glsl/mesh.vs\
	 src/glsl/deferred.fs\
	 src/glsl/deferred.vs

GLSLH = $(patsubst %.vs,%.vs.h,$(patsubst %.fs,%.fs.h,$(GLSL)))

CXXFLAGS= -ggdb -O0 -DDEBUG -std=gnu++11 -Wno-narrowing
LDFLAGS= -lSDL -lSDL_image -lGL -lm -lc

%.fs.h: %.fs
	(cat $<; printf "\0") | xxd -i > $@

%.vs.h: %.vs
	(cat $<; printf "\0") | xxd -i > $@

%.glsl.h: %.glsl
	xxd -i $< $@

all: build $(GLSLH) brickgl

brickgl: $(OBJ)
	-ctags -R -o .git/ctags
	g++ $(OBJ) $(CXXFLAGS) $(LDFLAGS) -o brickgl

build/%.o: src/%.cpp
	mkdir -p $(dir $@)
	g++ $< -c $(CXXFLAGS) -o $@
	g++ $(CXXFLAGS) -MM -MT '$@' $< -MF build/$*.d

build:
	mkdir -p build

clean:
	rm -rf build
	rm -f brickgl
	rm -f $(GLSLH)

run: brickgl
	./brickgl

-include $(DEP)
