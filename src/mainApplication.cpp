#include "mainApplication.hpp"

#include "framework/draw/objFormat.hpp"
#include "matrix.hpp"

MainApplication::MainApplication(int argc, char **argv) {
    drawDevice = NULL;
    inputDevice = NULL;
    cursor = NULL;
    isRunning = false;
    camera = NULL;
    brickMenu = NULL;
}

void MainApplication::init() {
    drawDevice = new GLDrawDevice();
    cursor = new ANDBrick(vec4(0,0,0,1));
    isRunning = true;

    camera = &((GLDrawDevice*)drawDevice)->camera;
    brickMenu = new BrickMenu;

    Brick::init();
}

bool MainApplication::tryPlaceBrick() {
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

void MainApplication::input() {
    //TODO: scroll zoom in/out
    //camera->addOffset(vec4(0,0,5,0));
    //camera->addOffset(vec4(0,0,-5,0));

    if(inputDevice->keyReleased(MOUSE_LEFT)) {
        tryPlaceBrick();
    }

    if(inputDevice->isKeyDown(MOUSE_RIGHT)) {
        camera->addRotation(vec4( inputDevice->mouseDY() / 100.0f,
                                 -inputDevice->mouseDX() / 100.0f, 0, 0));
    }

    if(inputDevice->isKeyDown(KEY_A))
    {
        camera->addPosition(vec4(-1,0,0,0));
    }

    if(inputDevice->isKeyDown(KEY_D))
    {
        camera->addPosition(vec4(1,0,0,0));
    }

    if(inputDevice->isKeyDown(KEY_W))
    {
        camera->addPosition(vec4(0,0,-1,0));
    }

    if(inputDevice->isKeyDown(KEY_S))
    {
        camera->addPosition(vec4(0,0,1,0));
    }

    if(inputDevice->isKeyDown(KEY_1))
    {
        delete cursor;
        cursor = new ANDBrick();
    }

    if(inputDevice->isKeyDown(KEY_2))
    {
        delete cursor;
        cursor = new Wire8Brick();
    }

    if(inputDevice->isKeyDown(KEY_3))
    {
        delete cursor;
        cursor = new LEDBrick();
    }

    if(inputDevice->isKeyDown(KEY_4))
    {
        //cursor = Brick(Brick::BRICK_PLATE2x4, vec4(0,0,0,1));
    }

    if(inputDevice->keyPressed(KEY_LEFT))
    {
        cursor->rotate(vec4(0,M_PI / 2.0f,0,0));
    }

    if(inputDevice->keyPressed(KEY_RIGHT))
    {
        cursor->rotate(vec4(0,-M_PI / 2.0f,0,0));
    }

    cursor->position = cursor->position + ((target - cursor->position) * 0.25f);
    if(cursor->position.distanceSq(target) > 1000 * 1000)
    {
        cursor->position = target;
    }
}

void MainApplication::drawBrick(GLMesh *mesh, GLTexture *tex, vec4 pos, unsigned w, unsigned h) {
    for(int j = 0; j < h; j++)
    {
        for(int i = 0; i < w; i++)
        {
            mat4 mMat = mat4::getTranslation(pos + vec4(i * 8, 0, j * 8, 0));
            ((GLDrawDevice*)drawDevice)->drawMesh(mesh, tex, mMat);
        }
    }
}

Brick *MainApplication::findClosestBrick(vec4 p) {
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

void MainApplication::draw() {
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
    glReadPixels(inputDevice->mouseX(), window->frameHeight()-inputDevice->mouseY(), 1, 1, GL_RGBA, GL_FLOAT, &MOUSE.v);
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

    window->swapBuffers();
}

void MainApplication::update(float dt) {
    Application::update(dt);
    for(int i = 0; i < bricks.size(); i++)
    {
        bricks[i]->update();
    }

    // once values are updated, we can flip value buffers
    for(int i = 0; i < bricks.size(); i++) {
        bricks[i]->flip();
    }
}

void MainApplication::run() {
    while(isRunning)
    {
        input();
        update(32);
        draw();
    }
}