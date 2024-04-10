#pragma once
#include <QObject>
#include <QGraphicsView>
#include <qevent.h>


class MygraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit MygraphicsView(QWidget* parent = 0);
    ~MygraphicsView();
    void setmainwin(QWidget* widget);
    void clear();
    void resetScale();

protected:
    void wheelEvent(QWheelEvent* event);
    void magnify();
    void shrink();
    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void keyPressEvent(QKeyEvent* event);
    

private:
    QWidget* mainwin = nullptr;
    QWidget* parent = nullptr;
    qreal scale_offset;//缩放尺寸
    qreal m_scale;
    qreal rot_offset;//旋转角度
    qreal m_rotate;
    bool rotable;//旋转标识
    QPoint lastMousePos;//鼠标上一次记录的位置
    QTransform origin;

signals:
    void quit_pressed();
    void e_pressed();
};

