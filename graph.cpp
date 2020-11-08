/*
 * File:    graph.cpp
 * Author:  Rachel Bood
 * Date:    2014/11/07 (?)
 * Version: 1.7
 *
 * Purpose:
 *
 * Modification history:
 * July 20, 2020 (IC V1.1)
 *  (a) Fixed setRotation to properly rotate graph items while taking into
 *	account their previous rotation value.
 * July 30, 2020 (IC & JD V1.2)
 *  (a) Fix getRootParent().
 *  (b) Clean up formatting and improve comments.
 * August 12, 2020 (IC V1.3)
 *  (a) Reversed the previous change to setRotation since it was only needed
 *	when graphs could be children of other graphs which can no longer
 *	happen.
 * Aug 14, 2020 (IC V1.4)
 *  (a) Initialize rotation in constructor.
 *  (b) Once again changed setRotation() back to the July 20 change.
 *	The issue was that the GraphicsItem rotation call at the end
 *	of the function wasn't using the additive rotation value but
 *	instead the passed value.
 * Aug 20, 2020 (IC V1.5)
 *  (a) Once again changed setRotation back to the July 20 change.  The issue
 *	was that the GraphicsItem rotation call at the end of the function
 *	wasn't using the additive rotation value but instead the passed value.
 * Sep 11, 2020 (IC V1.6)
 *  (a) Add code to draw outlines around a graph (purely for debugging).
 * Nov 7, 2020 (JD V1.7)
 *  (a) Rename the second param of setRotation() to make its purpose
 *	clearer (at least to me).
 *  (b) Change setRotation() so that the rotation of edges and nodes
 *	is the negative of the graph's rotation, so that the labels
 *	are shown in the correct orientation.  This works after IC's
 *	changes to how a join operation works (i.e, when two graphs
 *	are joined, all the edges and nodes are children of the one
 *	and only resulting graph).  With this change (and the current
 *	join code) the labels are finally all correctly oriented, even
 *	in the cases where there are a sequence of four-node joins and
 *	rotations done in the Edit Canvas Graph tab.
 */

#include "graph.h"
#include "canvasview.h"
#include "node.h"
#include "edge.h"
#include "graphmimedata.h"

#include <QMimeData>
#include <QDrag>
#include <QDebug>
#include <QByteArray>
#include <QGraphicsSceneMouseEvent>
#include <QtAlgorithms>
#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <QtMath>



/*
 * Name:	Graph()
 * Purpose:	Constructor for the graph object.
 * Arguments:	None.
 * Output:	Nothing.
 * Modifies:	Private variables of the graph object.
 * Returns:	Nothing.
 * Assumptions: None.
 * Bugs:	None known.
 * Notes:	None.
 */

Graph::Graph()
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemIsFocusable);
    setCacheMode(DeviceCoordinateCache);
    moved = 0;
    rotation = 0;
    setAcceptHoverEvents(true);
    setZValue(0);
}



/*
 * Name:	isMoved()
 * Purpose:	Set a flag used to determined if the graph was dropped
 *		onto the canvas scene.
 * Arguments:	None.
 * Output:	Nothing.
 * Modifies:	int moved
 * Returns:	Nothing.
 * Assumptions: None.
 * Bugs:	None known.
 * Notes:	None.
 */

void
Graph::isMoved()
{
    moved = 1;
    setHandlesChildEvents(false);
}



/*
 * Name:	mouseReleaseEvent()
 * Purpose:	Handle the event that is triggered after the user releases
 *		the mouse button.
 * Arguments:	QGraphicsSceneMouseEvent *
 * Output:	Nothing.
 * Modifies:	the Cursor icon
 * Returns:	Nothing.
 * Assumptions: None.
 * Bugs:	None known.
 * Notes:	None.
 */

void
Graph::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    setCursor(Qt::CrossCursor);
    QGraphicsItem::mouseReleaseEvent(event);
}



/*
 * Name:	paint()
 * Purpose:	None, really (required for custom QGraphicsItems).
 * Arguments:	QPainter *, QStyleOptionGraphicsItem *, QWidget *
 * Output:	Nothing.
 * Modifies:	Nothing.
 * Returns:	Nothing.
 * Assumptions: None.
 * Bugs:	None known.
 * Notes:	A Graph object is just a container to house the nodes
 *		and edges, therefore nothing is required to be drawn
 *		in a graph object.
 */

void
Graph::paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
	     QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

#ifdef DEBUG
    // Paints a border around graphs for debug purposes.
    QRectF border = boundingRect();

    QPen pen;
    pen.setColor(Qt::black);
    pen.setWidthF(1);

    painter->setPen(pen);
    painter->drawRect(border);
#else
    Q_UNUSED(painter);
#endif
}



/*
 * Name:	boundingRect()
 * Purpose:	Returns the bouding rectangle of the graph.
 * Arguments:	None.
 * Output:	Nothing.
 * Modifies:	Nothing.
 * Returns:	The graph's boundingRect().
 * Assumptions: None.
 * Bugs:	None known.
 * Notes:	Returns the bounding rectangle that surrounds the
 *		nodes and edges of the graph.
 */

QRectF
Graph::boundingRect() const
{
    return childrenBoundingRect();
}



/*
 * Name:	setRotation()
 * Purpose:	Sets the Rotation of the graph.
 * Arguments:	An amount to rotate, and a flag indicating whether the
 *		rotation amount is relative or absolute.
 * Output:	Nothing.
 * Modifies:	The graph object itself, as well as nodes and edges of
 *		the graph.
 * Returns:	Nothing.
 * Assumptions: None.
 * Bugs:	None known.
 * Notes:	The node and edge labels need to be rotated in the
 *		opposite direction of the graph rotation in order to
 *		keep them oriented horizontally, right side up.
 */

void
Graph::setRotation(qreal aRotation, bool rotationIsRelative)
{
    QList<QGraphicsItem *> list;

    qDeb() << "G::setRotation(" << aRotation << "," << rotationIsRelative
	   << ") called";

    foreach (QGraphicsItem * gItem, this->childItems())
	list.append(gItem);

    if (rotationIsRelative)
    {
	qDeb() <<"   changing 'rotation' from " << rotation
	       << " to " << getRotation() + aRotation;
	rotation = getRotation() + aRotation;
    }
    else
    {	
	qDeb() << "   changing 'rotation' from " << rotation
	       << " to " << aRotation;
	rotation = aRotation;
    }

    while (!list.isEmpty())
    {
	qDeb() << "   ! list.isEmpty()";
	foreach (QGraphicsItem * child, list)
	{
	    qDeb() << "      found a child of type" << child->type();
	    if (child != nullptr || child != 0)
	    {
		if (child->type() == Graph::Type)
		{
		    // Can this happen after IC's changes to the join operation?
		    qDeb() << "         found a GRAPH child (add to list)";
		    list.append(child->childItems());
		}
		else if (child->type() == Node::Type)
		{
		    Node * node = qgraphicsitem_cast<Node*>(child);
		    qDeb() << "       changing NODE "
			     << node->getLabel() << "'s rotation from "
			     << node->getRotation() << " to "
			     << -rotation;
		    node->setRotation(-rotation);
		}
		else if (child->type() == Edge::Type)
		{
		    Edge * edge = qgraphicsitem_cast<Edge*>(child);
		    qDeb() << "       changing EDGE "
			     << edge->getLabel() << "'s rotation from "
			     << edge->getRotation() << " to "
			     << -rotation;
		    edge->setRotation(-rotation);
		}

		list.removeOne(child);
	    }
	}
    }

    QGraphicsItem::setRotation(rotation);
}



/*
 * Name:	getRotation()
 * Purpose:	Getter for the graph's rotation.
 * Arguments:	None.
 * Output:	Nothing.
 * Modifies:	Nothing.
 * Returns:	The graph rotation.
 * Assumptions: None.
 * Bugs:	Hard to imagine.
 * Notes:	None.
 */

qreal
Graph::getRotation()
{
    return rotation;
}



/*
 * Name:	getRootParent()
 * Purpose:	Returns the root parent of the graph.
 * Arguments:	None.
 * Output:	Nothing.
 * Modifies:	Nothing.
 * Returns:	QGraphicsItem *
 * Assumptions: None.
 * Bugs:	None known.
 * Notes:	None.
 */

QGraphicsItem *
Graph::getRootParent()
{
    QGraphicsItem * parent = this;

    while (parent->parentItem() != nullptr || parent->parentItem() != 0)
	parent = parent->parentItem();

    return parent;
}
