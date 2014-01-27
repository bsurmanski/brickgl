/**
 * main.c
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#include <stddef.h>
#include <stdbool.h>
#include "brick.h"
#include "GL/gl.h"
#include "GL/glext.h"
#include "GLFW/glfw3.h"

bool running = true;
GLFWwindow* mainWindow;

void init()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, false);
    mainWindow = glfwCreateWindow(640, 480, "BrickGL", NULL, NULL);
}

void update()
{

}

void draw()
{

}

int main(int argc, char **argv)
{
    init(); 
    while(running)
    {
        update();
        draw();
    }
    return 0;
}
