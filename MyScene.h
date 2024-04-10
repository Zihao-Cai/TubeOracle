#pragma once
#include <qgraphicsscene.h>
#include <qstyleoption.h>

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    MyScene(QObject* parent = nullptr) : QGraphicsScene(parent), m_manualUpdate(false) {}

    void setManualUpdate(bool manualUpdate) {
        m_manualUpdate = manualUpdate;
        if (!m_manualUpdate) {
            update();
        }
    }

protected:
    void drawItems(QPainter* painter, int numItems, QGraphicsItem* items[], const QStyleOptionGraphicsItem* options) {
        if (!m_manualUpdate) {
            QGraphicsScene::drawItems(painter, numItems, items, options);
        }
    }
    

private:
    bool m_manualUpdate;
};

