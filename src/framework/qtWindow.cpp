#include "qtWindow.hpp"

#include <qt/QtOpenGL/QtOpenGL>

#include "application.hpp"
#include "input/qtInputDevice.hpp"

static unsigned qtToInputKey(unsigned key) {
        switch(key) {
            case Qt::Key_Escape: return KEY_ESC;
            case Qt::Key_Space: return KEY_SPACE;
            case Qt::Key_Enter: return KEY_ENTER;
            case Qt::Key_Left: return KEY_LEFT;
            case Qt::Key_Right: return KEY_RIGHT;
            case Qt::Key_Up: return KEY_UP;
            case Qt::Key_Down: return KEY_DOWN;
            default:
                if((key >= '0' && key <= '9')) {
                    return key;
                } else if(key >= 'A' && key <= 'Z') {
                    return key + 32;
                }
        }
        return KEY_UNKNOWN;
}

static unsigned qtToInputMouse(unsigned mouse) {
        switch(mouse) {
            case Qt::LeftButton: return MOUSE_LEFT; break;
            case Qt::RightButton: return MOUSE_RIGHT; break;
            case Qt::MiddleButton: return MOUSE_MIDDLE; break;
            default: return KEY_UNKNOWN;
        }
}

class GLFrame : public QGLWidget, protected QGLFunctions {
    Application *app;
    QBasicTimer timer;

    public:
    GLFrame(Application *_app, QWidget *parent) : QGLWidget(parent) {
        app = _app;
        setMinimumSize(320, 240);
        setMouseTracking(true);
    }

    virtual ~GLFrame() {
    }

    QSize sizeHint() {
        return QSize(640, 480);
    }

    void initializeGL() {
        initializeGLFunctions();
        makeCurrent();
        app->init();
        timer.start(32, this);
    }

    void resizeGL(int w, int h) {
        glViewport(0, 0, w, h);
        glScissor(0, 0, w, h);
    }

    void paintGL() {
        app->draw();
    }

    void timerEvent(QTimerEvent *event) {
        update();
        app->input();
        app->update(32);
    }

    void mouseMoveEvent(QMouseEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) return;

        qidevice->setMouseX(event->x());
        qidevice->setMouseY(event->y());
    }

    void mousePressEvent(QMouseEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) return;

        unsigned key = qtToInputMouse(event->button());
        if(key == KEY_UNKNOWN) return;

        qidevice->setKey(key, 1);
    }

    void mouseReleaseEvent(QMouseEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) return;

        unsigned key = qtToInputMouse(event->button());
        if(key == KEY_UNKNOWN) return;

        qidevice->setKey(key, 0);
    }

    void keyPressEvent(QKeyEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) return;

        unsigned key = qtToInputKey(event->key());
        if(key == KEY_UNKNOWN) return;

        qidevice->setKey(key, 1);
    }

    void keyReleaseEvent(QKeyEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) return;

        unsigned key = qtToInputKey(event->key());
        if(key == KEY_UNKNOWN) return;

        qidevice->setKey(key, 0);
    }
};

QtWindow::QtWindow(Application *app, uint32_t w, uint32_t h, std::string name)
    : Window(app, w, h, name)
{
    //setWindowTitle(name);
    widget = new GLFrame(app, 0);
    widget->resize(QSize(640, 480));
    widget->show();
}

QtWindow::~QtWindow() {
}

void QtWindow::swapBuffers() {
    widget->swapBuffers();
}

void QtWindow::clear() {
}
