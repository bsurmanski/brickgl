# BrickGL Design Documentation

## Design Choices

### Use of C++ as the chosen programming language
C++ was chosen because I am deeply familiar with C (which is C++ is closely
related to), object-oriented features would ease the design of the system, C++
is highly portable, and the Qt library API is available in C++.

#### Alternatives
Alternatives include:

C which was rejected due to it's lack of object system and lack of Qt bindings.

Java which has the benefit of portability and safety, but was rejected due to
its high overhead, and lack of direct support for OpenGL.

C# is similar to Java, but was rejected due to it's platform dependence on the
.NET framework and Windows platforms.


### Use of OpenGL for rendering
OpenGL was used for rendering due to it's portability. OpenGL is availible on
almost any modern computer system including desktop operating systems and
mobile.

#### Alternative: DirectX
DirectX was deemed insufficient since it is only available on Windows-based platforms


### Use OpenGL directly instead of an abstraction Library
Using OpenGL directly can sometimes be difficult. Despite this, I have previous
experience with OpenGL and I deemed the scope of this project not to outreach my
previous experience. This would allow me to sample previous code for most of the
required OpenGL components. Additionally, adding an abstraction library would
add an additional dependency to BrickGL that may provide version or platform
compatibility issues.

### Use of Qt for Windowing
Qt was chosen because it is cross platform, has support for providing an OpenGL
context, provides key listening, and I have had previous experience with Qt. 
