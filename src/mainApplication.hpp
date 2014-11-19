#ifndef _MAINAPPLICATION_HPP
#define _MAINAPPLICATION_HPP

#include "framework/application.hpp"
#include "framework/draw/mesh.hpp"
#include "framework/draw/glMesh.hpp"
#include "framework/draw/glCubemap.hpp"
#include "brick.hpp"
#include "brickMenu.hpp"
#include "vector.hpp"

#include <vector>
#include <algorithm>

class MainApplication : public Application {
    protected:
    bool isRunning;
    bool willScreenshot;
    bool willGenInstructions;
    bool showCursor;

    vec4 target;
    std::vector<Brick*> bricks;
    Camera *camera;
    Brick* cursor;
    BrickMenu *brickMenu;
    GLCubemap *skybox;

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

    virtual int save(std::string filenm);
    virtual int load(std::string filenm);

    virtual void screenshot(std::string filenm);
    virtual void genInstructions(std::string folder);

    bool tryPlaceBrick(Brick *b);

    Camera *getCamera() { return camera; }
    std::vector<Brick*>* getBricks() { return &bricks; }
};

#endif
