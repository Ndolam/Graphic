/*
 * File:    graph.cpp
 * Author:  Rachel Bood
 * Date:    2014/11/07 (?)
 * Version: 1.11
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
 * Nov 9, 2020 (JD V1.8)
 *  (a) Add the boundingBox() function to return a tight bounding box
 *      for a graph, and, optionally the center of this box.
 *	The Qt boundingRect() function returns a larger box which is
 *	not suitable for all purposes.
 * Nov 11, 2020 (JD V1.9)
 *  (a) Removed rotation as a graph attribute, since a graph is a
 *	qgraphicsitem, which has a rotation (don't store the same info
 *	in two places).  Modified code accordingly.
 * Nov 14, 2020 (JD V1.10)
 *  (a) Add a third arg to boundingBox() which can be used to return
 *	the geometric center of the nodes in the graph coordinate system.
 *  (b) Fix bug in boundingBox() which didn't deal with the first node
 *	correctly.
 * Nov 16, 2020 (JD V1.11)
 *  (a) Add centerGraph(), which adjusts the graph so that the center
 *	of the graph is at the geometric center of the node centers.
 *	This is useful (particularly for joined graphs and freestyle
 *	graphs, but even for some basicgraphs) so that when the graph
 *	is rotated via the Edit Canvas Graph tab it appears to rotate
 *	around its center, rather than orbiting around some apparently
 *	arbitrary point on the canvas.
 */

#include "graph.h"
#include "defuns.h"
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
 * Outputs:	Nothing.
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
    setAcceptHoverEvents(true);
    setZValue(0);
}



/*
 * Name:	boundingBox()
 * Purpose:	Return information about the graph, as computed from
 *		the nodes' screen coords, and optionally the node diameters.
 *		Also calculate and return the center of the graph,
 *		using only the node screen coords.
 *		Optionally also return the center of all the nodes
 *		relative to the center of the graph.
 * Arguments:	A "copy out" QPointF for the center of the graph
 *		(relative to the scene) (or nullptr if this
 *		information is not desired), and a Boolean which
 *		indicates whether the individual node diameters
 *		should be taken into account.
 *		Another (Relative to Graph) center argument for the
 *		center of the node locations relative to the graph
 *		coordinate system.
 * Outputs:	Nothing.
 * Modifies:	The pointer parameter.
 * Returns:	The QRectF as described above.
 * Assumptions:	The graph is non-empty.  If it is empty, (0, 0, 0, 0)
 *		and (0, 0) are returned.
 * Bugs:	None known.
 * Notes:	This ignores labels; thus if there is a "very large"
 *		edge or node label the label may be outside the
 *		returned QRectF.
 */

QRectF
Graph::boundingBox(QPointF * center, bool useNodeSizes, QPointF * RGcenter)
{
    qreal minx = 0, maxx = 0, miny = 0, maxy = 0;
    qreal RGminx = 0, RGmaxx = 0, RGminy = 0, RGmaxy = 0;
    bool firstNode = true;
    qreal r, x, y, RGx, RGy;

    qDebu("G:bB(%p, %c) called on graph at %p", center,
	  useNodeSizes ? 'T' : 'F', this);

    foreach (QGraphicsItem * item, this->childItems())
    {
	switch (item->type())
	{
	  case Node::Type:
	    x = item->scenePos().x();
	    y = item->scenePos().y();
	    RGx = item->pos().x();
	    RGy = item->pos().y();

	    if (useNodeSizes)
	    {
		// If we wish to take the node diameter into account we
		// must convert from inches to screen coords.
		r = (qgraphicsitem_cast<Node *>(item))->getDiameter() / 2
		    * currentPhysicalDPI;
	    }
	    else
		r = 0;

	    if (firstNode)
	    {
		firstNode = false;
		minx = x - r;
		maxx = x + r;
		miny = y - r;
		maxy = y + r;
		RGminx = RGx;
		RGmaxx = RGx;
		RGminy = RGy;
		RGmaxy = RGy;
	    }
	    else
	    {
		if (x + r > maxx)
		    maxx = x + r;
		else if (x - r < minx)
		    minx = x - r;
		if (y + r > maxy)
		    maxy = y + r;
		else if (y - r < miny)
		    miny = y - r;

		if (RGx > RGmaxx)
		    RGmaxx = RGx;
		else if (RGx < RGminx)
		    RGminx = RGx;
		if (RGy > RGmaxy)
		    RGmaxy = RGy;
		else if (RGy < RGminy)
		    RGminy = RGy;
	    }
	    qDeb() << "    scene: x = " << x << ", y = " << y << ", r = " << r;
	    qDebu("        scene x's: [%.4f, %.4f], y's: [%.4f, %.4f]",
		  minx, maxx, miny, maxy);
	    qDeb() << "    RG:    x = " << RGx << ", y = " << RGy;
	    qDebu("        RG:   x's: [%.4f, %.4f], y's: [%.4f, %.4f]",
		  RGminx, RGmaxx, RGminy, RGmaxy);
	    break;

	  case Graph::Type:
	    // This should not happen since V1.16 of canvasscene.cpp
	    qDebug() << "UNEXPECTED GRAPH FOUND INSIDE GRAPH!!!!";
	    break;
	}
    }

    QRectF rect (minx, miny, maxx - minx, maxy - miny);
    if (center != nullptr)
    {
	center->setX((maxx + minx) / 2.);
	center->setY((maxy + miny) / 2.);
	qDeb() << "G::bB: center is " << *center << " and BB rect is " << rect;
    }
    else
	qDeb() << "G::bB: center is [" << (maxx + minx) / 2. << ", "
	       <<  (maxy + miny) / 2. << "] and BB rect is " << rect;

    if (RGcenter != nullptr)
    {
	RGcenter->setX((RGmaxx + RGminx) / 2.);
	RGcenter->setY((RGmaxy + RGminy) / 2.);
	qDeb() << "G::bB: RGcenter is " << *RGcenter;
    }
    else
	qDeb() << "G::bB: RGcenter is [" << (RGmaxx + RGminx) / 2. << ", "
	       <<  (RGmaxy + RGminy) / 2. << "]";

    return rect;
}




/*
 * Name:	isMoved()
 * Purpose:	Set a flag used to determined if the graph was dropped
 *		onto the canvas scene.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The int moved.
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
 * Outputs:	Nothing.
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
 * Outputs:	Nothing.
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
 * Outputs:	Nothing.
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
 * Outputs:	Nothing.
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
Graph::setRotation(qreal rotationAmount, bool rotationIsRelative)
{
    QList<QGraphicsItem *> list;
    qreal newRotation;

    qDeb() << "G::setRotation(" << rotationAmount << ", "
	   << rotationIsRelative << ") called";

    foreach (QGraphicsItem * gItem, this->childItems())
	list.append(gItem);

    if (rotationIsRelative)
	newRotation = getRotation() + rotationAmount;
    else
	newRotation = rotationAmount;

    qDeb() << "   changing 'rotation' from " << this->rotation()
	   << " to " << newRotation;

    while (!list.isEmpty())
    {
	qDeb() << "   ! list.isEmpty()";
	foreach (QGraphicsItem * child, list)
	{
	    qDeb() << "      found a child of type " << child->type();
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
		    qDeb() << "       changing NODE " << node->getLabel()
			   << "'s rotation from " << node->getRotation()
			   << " to " << -newRotation;
		    node->setRotation(-newRotation);
		}
		else if (child->type() == Edge::Type)
		{
		    Edge * edge = qgraphicsitem_cast<Edge*>(child);
		    qDeb() << "       changing EDGE " << edge->getLabel()
			   << "'s rotation from " << edge->getRotation()
			   << " to " << -newRotation;
		    edge->setRotation(-newRotation);
		}

		list.removeOne(child);
	    }
	}
    }

    QGraphicsItem::setRotation(newRotation);
}



/*
 * Name:	getRotation()
 * Purpose:	Getter for the graph's rotation.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	Nothing.
 * Returns:	The graph rotation.
 * Assumptions: None.
 * Bugs:	Hard to imagine.
 * Notes:	None.
 */

qreal
Graph::getRotation()
{
    return this->rotation();
}



/*
 * Name:	getRootParent()
 * Purpose:	Returns the root parent of the graph.
 * Arguments:	None.
 * Outputs:	Nothing.
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



/*
 * Name:	centerGraph()
 * Purpose:	Adjust the graph (without moving it on the canvas) so
 *		that its coordinate system is at its geometric center.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The graph.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	If seems to work on rotated graphs, but this has been
 *		very lightly tested when the function was initially
 *		added.
 */

void
Graph::centerGraph()
{
    QPointF RGcenter; // Relative to the graph's coordinate system
    QRectF bb = this->boundingBox(nullptr, false, &RGcenter);
    qDeb() << "G::centerGraph() centering a graph";
    qDeb() << "     bbox:   " << bb;
    qDeb() << "     center: " << RGcenter;
    qDeb() << "     pos:    " << this->pos();

    foreach (QGraphicsItem * item, this->childItems())
    {
	if (item->type() == Node::Type)
	{
	    Node * node = qgraphicsitem_cast<Node *>(item);
	    qDeb() << "move node '" << node->getLabel()
		   << "': " << node->pos()
		   << " -> " << node->pos() - RGcenter;
	    node->setPos(node->pos() - RGcenter);
	}
    }

    qDeb() << "    moving graph from " << this->pos()
	   << " to " << this->pos() + RGcenter;
    this->setPos(this->pos() + RGcenter);
    update();
}
