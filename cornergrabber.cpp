/*
 * File:    cornergrabber.cpp
 * Author:  Rachel Bood
 * Date:    2014 (?)
 * Version: 1.1
 *
 * Purpose: If only we knew.
 *
 * Modification history:
 * Oct 18, 2020 (JD V1.1):
 *  (a) Add header comment.  Fix spelling.  Tidy code.
 */

#include "cornergrabber.h"

CornerGrabber::CornerGrabber(QGraphicsItem * parent, int corner) :
QGraphicsItem(parent),
    mouseDownX(0),
    mouseDownY(0),
    _outerborderColour(Qt::black),
    _outerborderPen(),
    _width(6),
    _height(6),
    _corner(corner),
    _mouseButtonState(kMouseReleased)
{
    // setParentItem(parent);
    _outerborderPen.setWidth(1);
    _outerborderPen.setColor(_outerborderColour);

    this->setAcceptHoverEvents(true);
}


void
CornerGrabber::setMouseState(int s)
{
    _mouseButtonState = s;
}


int
CornerGrabber::getMouseState()
{
    return _mouseButtonState;
}


int
CornerGrabber::getCorner()
{
    return _corner;
}


// We have to implement the mouse events to keep the linker happy,
// but just set accepted to false since are not actually handling them.

void
CornerGrabber::mouseMoveEvent(QGraphicsSceneDragDropEvent * event)
{
    event->setAccepted(false);
}


void
CornerGrabber::mousePressEvent(QGraphicsSceneDragDropEvent * event)
{
    event->setAccepted(false);
}


void
CornerGrabber::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    event->setAccepted(true);
}


void
CornerGrabber::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    event->setAccepted(false);
}


void
CornerGrabber::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    event->setAccepted(false);
}



// Change the colour on hover events to indicate to the use the object
// has been captured by the mouse.

void
CornerGrabber::hoverLeaveEvent(QGraphicsSceneHoverEvent *)
{
    _outerborderColour = Qt::black;
    this->update(0,0,_width,_height);
}


void
CornerGrabber::hoverEnterEvent(QGraphicsSceneHoverEvent *)
{
    _outerborderColour = Qt::red;
    this->update(0,0,_width,_height);
}


QRectF
CornerGrabber::boundingRect() const
{
    return QRectF(0,0,_width,_height);
}


void
CornerGrabber::paint(QPainter * painter,
		     const QStyleOptionGraphicsItem *, QWidget *)
{

    // Fill the box with solid colour, use sharp corners.

    _outerborderPen.setCapStyle(Qt::SquareCap);
    _outerborderPen.setStyle(Qt::SolidLine);
    painter->setPen(_outerborderPen);

    QPointF topLeft(0, 0);
    QPointF bottomRight(_width, _height);

    QRectF rect(topLeft, bottomRight);

    QBrush brush(Qt::SolidPattern);
    brush.setColor(_outerborderColour);
    painter->fillRect(rect, brush);
}
