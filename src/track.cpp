#include "track.h"

#include <QPaintEvent>
#include <QPainter>

Track::Track(QWidget *parent)
    : QListView(parent)
{
    setFlow(QListView::LeftToRight);
    setMaximumHeight(50);

    setModel(nullptr);
    setSelectionMode(QAbstractItemView::SingleSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
}

Track::~Track()
{
    
}

/**
 * @brief Track::addMarker
 * @param ms
 * Add a time marker at ms on the track
 */
void Track::addMarker(int64_t ms)
{
    msMarker.append(ms);
}

/**
 * @brief Track::paintEvent
 * @param e
 * http://doc.qt.io/qt-5/qwidget.html#paintEvent
 */
void Track::paintEvent(QPaintEvent *e)
{
    QListView::paintEvent(e);

    /*int w = width(), h = height();
    QBrush markerBrush(Qt::red);
    QPen pen(markerBrush, 2);
    QPainter painter(viewport());
    painter.setPen(pen);
    for (int64_t mark : msMarker){
        painter.drawLine(mark, 0, mark, h);
    }*/
}
