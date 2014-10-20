SOURCES = \
src/main.cpp\
src/brick.cpp\
src/matrix.cpp\
src/vector.cpp\
src/box.cpp\
src/framework/qtWindow.cpp\
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
src/framework/application.cpp\
src/brickMenu.cpp

HEADERS = \
src/brick.hpp\
src/matrix.hpp\
src/vector.hpp\
src/box.hpp\
src/framework/qtWindow.hpp\
src/framework/window.hpp\
src/framework/input/sdlInputDevice.hpp\
src/framework/input/inputDevice.hpp\
src/framework/draw/camera.hpp\
src/framework/draw/drawProgram.hpp\
src/framework/draw/drawDevice.hpp\
src/framework/draw/glTexture.hpp\
src/framework/draw/glFramebuffer.hpp\
src/framework/draw/image.hpp\
src/framework/draw/glDrawDevice.hpp\
src/framework/draw/glMesh.hpp\
src/framework/draw/glDrawShader.hpp\
src/framework/draw/glDrawProgram.hpp\
src/framework/draw/objFormat.hpp\
src/framework/draw/pngFormat.hpp\
src/framework/draw/mesh.hpp\
src/framework/application.hpp\
src/brickMenu.hpp

config += qt debug

QMAKE_CXXFLAGS += -g

QT = core gui opengl

OBJECTS_DIR=build

TARGET = brickgl

LIBS += -lSDL -lSDL_image
