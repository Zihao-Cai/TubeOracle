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
    qreal scale_offset;//���ųߴ�
    qreal m_scale;
    qreal rot_offset;//��ת�Ƕ�
    qreal m_rotate;
    bool rotable;//��ת��ʶ
    QPoint lastMousePos;//�����һ�μ�¼��λ��
    QTransform origin;

signals:
    void quit_pressed();
    void e_pressed();
};

