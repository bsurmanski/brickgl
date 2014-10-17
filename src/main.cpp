/**
 * main.c
 * brickgl
 * January 21, 2014
 * Brandon Surmanski
 */

#include <stddef.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <SDL/SDL.h>

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>

#include "framework/application.hpp"
#include "framework/draw/glDrawDevice.hpp"
#include "framework/draw/glDrawProgram.hpp"
#include "framework/sdlWindow.hpp"
#include "matrix.hpp"
#include "vector.hpp"
#include "brick.hpp"
#include "brickMenu.hpp"

#include "framework/draw/camera.hpp"
#include "framework/draw/mesh.hpp"
#include "framework/draw/glMesh.hpp"
#include "framework/draw/objFormat.hpp"
#include "framework/draw/pngFormat.hpp"

#include <vector>

vec4 target;

static unsigned WIDTH = 640;
static unsigned HEIGHT = 480;

int mousex;
int mousey;

bool placeActive = false;

class MainApplication : public Application
{
    bool isRunning;
    std::vector<Brick*> bricks;
    Camera *camera;
    Brick* cursor;
    BrickMenu *brickMenu;

    public:

    MainApplication()
    {
        window = new SDLWindow(WIDTH, HEIGHT, "BrickSim");
        drawDevice = new GLDrawDevice();
        cursor = new ANDBrick(vec4(0,0,0,1));
        isRunning = true;

        camera = &((GLDrawDevice*)drawDevice)->camera;
        brickMenu = new BrickMenu;

        Brick::init();
    }

    ~MainApplication()
    {
    }

    bool tryPlaceBrick()
    {
        Brick *b = cursor;
        b->position = target;
        bool collision = false;
        int i;
        for(i = 0; i < bricks.size(); i++)
        {
            if(!b->connect(bricks[i])) {
                printf("invalid connection\n");
                goto ERR;
            }

            if(b->collides(bricks[i]))
            {
                printf("collision\n");
                b->getBox().print();
                bricks[i]->getBox().print();
                collision = true;
                goto ERR;
            }
        }

        if(!collision)
        {
            //if(placeActive) b->value = 1.0f;
            bricks.push_back(b);
            cursor = cursor->copy();
            return true;
        }

ERR:
    for(int j = i; j >= 0; j--) {
        b->disconnect(bricks[j]);
        return false;
    }

        return false;
    }

    void input()
    {
        uint8_t *keystate;
        SDL_Event event;

        static bool rclick = 0;
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                    isRunning = false;
                    break;
                case SDL_MOUSEMOTION:
                    if(rclick)
                    {
                        camera->addRotation(vec4( event.motion.yrel / 100.0f,
                                                 -event.motion.xrel / 100.0f, 0, 0));
                    }
                    SDL_GetMouseState(&mousex, &mousey);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        tryPlaceBrick();
                    }
                    if(event.button.button == SDL_BUTTON_RIGHT)
                        rclick = true;

                    if(event.button.button == 4)
                        camera->addOffset(vec4(0,0,5,0));

                    if(event.button.button == 5)
                        camera->addOffset(vec4(0,0,-5,0));

                    break;
                case SDL_MOUSEBUTTONUP:
                    if(event.button.button == SDL_BUTTON_RIGHT)
                        rclick = false;
                    break;
            }
        }

        keystate = SDL_GetKeyState(0);
        if(keystate[SDLK_a])
        {
            camera->addPosition(vec4(-1,0,0,0));
        }

        if(keystate[SDLK_d])
        {
            camera->addPosition(vec4(1,0,0,0));
        }

        if(keystate[SDLK_w])
        {
            camera->addPosition(vec4(0,0,-1,0));
        }

        if(keystate[SDLK_s])
        {
            camera->addPosition(vec4(0,0,1,0));
        }

        if(keystate[SDLK_1])
        {
            delete cursor;
            cursor = new ANDBrick();
        }

        if(keystate[SDLK_2])
        {
            delete cursor;
            cursor = new Wire8Brick();
        }

        if(keystate[SDLK_3])
        {
            delete cursor;
            cursor = new LEDBrick();
        }

        if(keystate[SDLK_3])
        {
            //cursor = Brick(Brick::BRICK_PLATE2x4, vec4(0,0,0,1));
        }

        if(keystate[SDLK_e])
        {
            placeActive = !placeActive;
        }

        static int left = 0;
        static int right = 0;
        if(keystate[SDLK_LEFT] && !left)
        {
            cursor->rotate(vec4(0,M_PI / 2.0f,0,0));
        }

        if(keystate[SDLK_RIGHT] && !right)
        {
            cursor->rotate(vec4(0,M_PI / 2.0f,0,0));
        }
        left = keystate[SDLK_LEFT];
        right = keystate[SDLK_RIGHT];

        cursor->position = cursor->position + ((target - cursor->position) * 0.25f);
        if(cursor->position.distanceSq(target) > 1000 * 1000)
        {
            cursor->position = target;
        }
    }

    void drawBrick(GLMesh *mesh, GLTexture *tex, vec4 pos, unsigned w, unsigned h)
    {
        for(int j = 0; j < h; j++)
        {
            for(int i = 0; i < w; i++)
            {
                mat4 mMat = mat4::getTranslation(pos + vec4(i * 8, 0, j * 8, 0));
                ((GLDrawDevice*)drawDevice)->drawMesh(mesh, tex, mMat);
            }
        }
    }

    Brick *findClosestBrick(vec4 p)
    {
        Brick *b = NULL;
        float dist = -1.0f;
        for(int i = 0; i < bricks.size(); i++)
        {
            if(!b || dist < bricks[i]->getBox().offsetOf(p).length())
            {
                b = bricks[i];
                dist = bricks[i]->getBox().offsetOf(p).length();
            }
        }

        return b;
    }

    void draw()
    {
        ((GLDrawDevice*)drawDevice)->lightBuffer->clear();
        ((GLDrawDevice*)drawDevice)->mainBuffer->clear();

        drawBrick(Brick::flatMesh, Brick::plateTexture, vec4(-16 * 8,-8.0, -16 * 8,1), 32, 32);
        for(int i = 0; i < bricks.size(); i++)
        {
            Brick *b = bricks[i];
            b->draw(drawDevice);
        }

#define MOUSESUPPORT
#ifdef MOUSESUPPORT
        vec4 MOUSE;
        glReadBuffer(GL_COLOR_ATTACHMENT2);
        glReadPixels(mousex, HEIGHT-mousey, 1, 1, GL_RGBA, GL_FLOAT, &MOUSE.v);
        MOUSE.x = round(MOUSE.x / 8.0f) * 8.0f;
        MOUSE.y = ceil(MOUSE.y / 9.6f) * 9.6f;
        MOUSE.z = round(MOUSE.z / 8.0f) * 8.0f;
        if(MOUSE.x != NAN && MOUSE.y != NAN && MOUSE.z != NAN)
            target = MOUSE;

        Brick *br = findClosestBrick(MOUSE);

#endif
        //target.print();
        //printf("\n");
        cursor->draw(drawDevice);

        ((GLDrawDevice*)drawDevice)->applyLighting();

        for(int i = 0; i < bricks.size(); i++)
        {
            Brick *b = bricks[i];
            b->light(drawDevice);
        }

        ((GLDrawDevice*)drawDevice)->drawToScreen();

        brickMenu->draw(drawDevice);
        drawBrick(Brick::flatMesh, Brick::plateTexture, vec4(-16 * 8,-8.0, -16 * 8,1), 32, 32);

        SDL_GL_SwapBuffers();
        window->swapBuffers();
    }

    void update()
    {
        for(int i = 0; i < bricks.size(); i++)
        {
            bricks[i]->update();
        }

        // once values are updated, we can flip value buffers
        for(int i = 0; i < bricks.size(); i++) {
            bricks[i]->flip();
        }

        SDL_Delay(32);
    }

    void run()
    {
        while(isRunning)
        {
            input();
            update();
            draw();
        }
    }
};

int main(int argc, char **argv)
{
    MainApplication app;
    app.run();
    return 0;
}
