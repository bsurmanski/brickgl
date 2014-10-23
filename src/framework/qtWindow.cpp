#include "qtWindow.hpp"

#include <qt/QtOpenGL/QtOpenGL>

#include "application.hpp"

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
        app->mouseMove(event->x(), event->y());
        printf("MOVE\n");
    }

    void mousePressEvent(QMouseEvent *event) {
    }

    void mouseReleaseEvent(QMouseEvent *event) {
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
