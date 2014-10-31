#include "qtWindow.hpp"

#include <qt/QtOpenGL/QtOpenGL>

#include "application.hpp"
#include "input/qtInputDevice.hpp"

static unsigned qtToInputKey(unsigned key) {
        printf("%c\n", key);
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
        setMinimumSize(640, 480);
        setMaximumSize(640, 480);
        setMouseTracking(true);
        setFocusPolicy(Qt::StrongFocus);
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

    void wheelEvent(QWheelEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) {
            QWidget::wheelEvent(event);
            return;
        }

        if(event->orientation() == Qt::Horizontal) {
            qidevice->setWheelDX(event->delta());
        } else {
            qidevice->setWheelDY(event->delta());
        }
    }

    void mouseMoveEvent(QMouseEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) {
            QWidget::mouseMoveEvent(event);
            return;
        }

        qidevice->setMouseX(event->x());
        qidevice->setMouseY(event->y());
    }

    void mousePressEvent(QMouseEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) {
            QWidget::mousePressEvent(event);
            return;
        }

        unsigned key = qtToInputMouse(event->button());
        if(key == KEY_UNKNOWN) {
            QWidget::mousePressEvent(event);
            return;
        }

        qidevice->setKey(key, 1);
    }

    void mouseReleaseEvent(QMouseEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) {
            QWidget::mouseReleaseEvent(event);
            return;
        }

        unsigned key = qtToInputMouse(event->button());
        if(key == KEY_UNKNOWN) {
            QWidget::mouseReleaseEvent(event);
            return;
        }

        qidevice->setKey(key, 0);
    }

    void keyPressEvent(QKeyEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) {
            QWidget::keyPressEvent(event);
            return;
        }

        unsigned key = qtToInputKey(event->key());
        if(key == KEY_UNKNOWN) {
            QWidget::keyPressEvent(event);
            return;
        }

        qidevice->setKey(key, 1);
    }

    void keyReleaseEvent(QKeyEvent *event) {
        QtInputDevice *qidevice = dynamic_cast<QtInputDevice*>(app->getInputDevice());
        if(!qidevice) {
            QWidget::keyReleaseEvent(event);
            return;
        }

        unsigned key = qtToInputKey(event->key());
        if(key == KEY_UNKNOWN) {
            QWidget::keyReleaseEvent(event);
            return;
        }

        qidevice->setKey(key, 0);
    }
};

QtWindow::QtWindow(Application *app, uint32_t w, uint32_t h, std::string name)
    : Window(app, w, h, name)
{
    //setWindowTitle(name);
    qwindow = new QMainWindow(0);
    menubar = new QMenuBar(qwindow);
    fileMenu = new QMenu("File", menubar);
    saveAction = new QAction("Save", fileMenu);
    loadAction = new QAction("Load", fileMenu);
    quitAction = new QAction("Quit", fileMenu);
    QObject::connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    QObject::connect(loadAction, SIGNAL(triggered()), this, SLOT(load()));
    QObject::connect(quitAction, SIGNAL(triggered()), this, SLOT(quit()));
    fileMenu->addAction(saveAction);
    fileMenu->addAction(loadAction);
    fileMenu->addAction(quitAction);
    menubar->addMenu(fileMenu);
    qwindow->setMenuBar(menubar);
    //qwindow->resize(QSize(w, h));

    widget = new GLFrame(app, qwindow);
    widget->resize(QSize(640, 480));
    qwindow->setCentralWidget(widget);
    qwindow->show();
}

void QtWindow::quit() {
    app->quit();
}

void QtWindow::save() {
    QString filter = "Brick Project (*.bpj)";
    QString filenm = QFileDialog::getSaveFileName(qwindow, "Save File", "", "Brick Projects (*.bpj)", &filter);
    int err = app->save(filenm.toUtf8().constData());
    if(err) {
        //TODO: error dialog
    }
}

void QtWindow::load() {
    QString filenm = QFileDialog::getOpenFileName(qwindow, "Load File", "", "Brick Projects (*.bpj)");
    int err = app->load(filenm.toUtf8().constData());
    if(err) {
        //TODO: error dialog
    }
}

size_t QtWindow::frameHeight() {
    return widget->height();
}
size_t QtWindow::frameWidth() {
    return widget->width();
}

QtWindow::~QtWindow() {
    delete widget;
    delete menubar;
    delete qwindow;
}

void QtWindow::swapBuffers() {
    widget->swapBuffers();
}

void QtWindow::clear() {
}
