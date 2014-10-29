#ifndef _MAINAPPLICATION_HPP
#define _MAINAPPLICATION_HPP

#include "framework/application.hpp"
#include "framework/draw/mesh.hpp"
#include "framework/draw/glMesh.hpp"
#include "brick.hpp"
#include "brickMenu.hpp"
#include "vector.hpp"

#include <vector>

class MainApplication : public Application {
    bool isRunning;
    vec4 target;
    std::vector<Brick*> bricks;
    Camera *camera;
    Brick* cursor;
    BrickMenu *brickMenu;

    protected:
    bool tryPlaceBrick();
    void drawBrick(GLMesh *mesh, GLTexture *tex, vec4 pos, unsigned w, unsigned h);
    Brick *findClosestBrick(vec4 p);

    public:
    MainApplication(int argc, char **argv);

    virtual void init();

    virtual ~MainApplication() {
    }

    virtual void input();
    virtual void draw();
    virtual void update(float dt);
    virtual void run();

    Camera *getCamera() { return camera; }
    std::vector<Brick*>* getBricks() { return &bricks; }
};

#endif
