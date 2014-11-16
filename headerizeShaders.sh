#!/usr/bin/sh

FILES="src/glsl/light.fs\
       src/glsl/light.vs\
       src/glsl/mesh.fs\
       src/glsl/mesh.vs\
       src/glsl/deferred.fs\
       src/glsl/deferred.vs\
       src/glsl/skybox.fs\
       src/glsl/skybox.vs"


for FILENM in $FILES; do
	(cat "$FILENM"; printf "\0") | xxd -i > "$FILENM".h
done

#xxd -i "$FILENM" "$FILENM".h
