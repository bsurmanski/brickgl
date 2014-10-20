#include "qtWindow.hpp"

#include <qt/QtOpenGL/QtOpenGL>

class QtWindowWidget : public QGLWidget {
    public:
    QtWindowWidget(QWidget *parent) : QGLWidget(QGLFormat(QGL::SampleBuffers), parent) {
    }

    ~QtWindowWidget() {
    }

    void initializeGL() {
    }

    void resizeGL(int w, int h) {
    }

    void paintGL() {
    }

    void timerEvent(QTimerEvent *) {
        update();
    }

    void mousePressEvent(QMouseEvent *) {
    }

    void mouseReleaseEvent(QMouseEvent *) {
    }
};

QtWindow::QtWindow(uint32_t w, uint32_t h, std::string name) : Window(w, h, name) {
    widget = new QtWindowWidget(0);
}

QtWindow::~QtWindow() {
}


void QtWindow::swapBuffers() {
}

void QtWindow::clear() {
}
