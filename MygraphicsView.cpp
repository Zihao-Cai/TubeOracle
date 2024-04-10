#include "MygraphicsView.h"
#include <qline.h>
#include <qgraphicsitem.h>


MygraphicsView::MygraphicsView(QWidget* parent)
    : QGraphicsView(parent)
{
    this->parent = parent;
    scale_offset = 1;
    m_scale = 1;
    rot_offset = 0;
    m_rotate = 0;
    rotable = false;
    mainwin = parent;
    origin = transform();
}

MygraphicsView::~MygraphicsView()
{

}

void MygraphicsView::setmainwin(QWidget* widget)
{
    mainwin = widget;
    connect(this, SIGNAL(quit_pressed()), mainwin, SLOT(quit_sel()));
    connect(this, SIGNAL(e_pressed()), mainwin, SLOT(on_addLoop()));
    connect(this, SIGNAL(e_pressed()), mainwin, SLOT(on_addSurface()));
    connect(this, SIGNAL(e_pressed()), mainwin, SLOT(on_settransfinite()));
    connect(this, SIGNAL(e_pressed()), mainwin, SLOT(on_setf()));
    connect(this, SIGNAL(e_pressed()), mainwin, SLOT(on_setq()));
    connect(this, SIGNAL(e_pressed()), mainwin, SLOT(on_setdis()));
}

void MygraphicsView::clear()
{
    QGraphicsScene* currentScene = this->scene();
    if (currentScene != nullptr) {
        for (auto it : currentScene->items()) {
            it->~QGraphicsItem();
        }
    }
}

void MygraphicsView::resetScale()
{
    scale_offset = 1;
    m_scale = 1;
    rot_offset = 0;
    m_rotate = 0;
    setTransform(origin);
}

void MygraphicsView::magnify()
{
    scale_offset += 0.05;
    scale(scale_offset, scale_offset);
    m_scale *= scale_offset;
    scale_offset = 1;
}

void MygraphicsView::shrink()
{
    scale_offset -= 0.05;
    scale(scale_offset, scale_offset);
    m_scale *= scale_offset;
    scale_offset = 1;
}

void MygraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (rotable) {
        //确定旋转方向
        QLineF line1(QPoint(this->width() / 2, this->height() / 2), lastMousePos);
        QLineF line2(QPoint(this->width() / 2, this->height() / 2), event->pos());
        rot_offset = 360 - line1.angleTo(line2);
        rot_offset < 180 ? rot_offset = 1 : rot_offset = -1;
        m_rotate += rot_offset;
        rotate(rot_offset);
        lastMousePos = event->pos();
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MygraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        QPointF startpnt = mapToScene(event->pos());
        if (scene()->itemAt(startpnt,transform()) == NULL) {
            rotable = true;
            lastMousePos = event->pos();
        }
    }
    lastMousePos = event->pos();
    QGraphicsView::mousePressEvent(event);
}

void MygraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton) {
        rotable = false;
    }
    lastMousePos = event->pos();
    QGraphicsView::mouseReleaseEvent(event);
}

void MygraphicsView::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Q) {
        emit quit_pressed();
    }
    else if (event->key() == Qt::Key_E) {
        emit e_pressed();
    }
    if (event->modifiers() == Qt::AltModifier) {
        if (event->key() == Qt::Key_R) {
            resetScale();
            return;
        }
    }

}

void MygraphicsView::wheelEvent(QWheelEvent* event)
{
    QPoint sroll = event->angleDelta();
    sroll.y() > 0 ? magnify() : shrink();

}
