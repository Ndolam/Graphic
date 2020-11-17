/*
 * File:    canvasscene.cpp
 * Author:  Rachel Bood
 * Date:    2014/11/07
 * Version: 1.29
 *
 * Purpose: Initializes a QGraphicsScene to implement a drag and drop feature.
 *          still very much a WIP
 *
 * Modification history:
 * Oct 13, 2019 (JD V1.1)
 *  (a) no functional code changes: some formatting, some addition of comments,
 *      made some debug statements more verbose, deleted some
 *      long-commented-out-by-Rachel code, and deleted the setting of an
 *      unused variable.
 *      (Note that removing braces from some cases caused errors on
 *      stmts of the form "type var = value;", but, oddly, these go away
 *      if these are replaced with "type var; var = value;".  I took
 *      away the braces before I noticed this because it made the
 *      indentation a bit weird, but perhaps I should have left well
 *      enough alone in this case.)
 * Nov 13, 2019 (JD V1.2)
 *  (a) rename Label to HTML_Label, as per changes to the naming scheme.
 * Nov 13, 2019 (JD V1.3)
 *  (a) rename "Weight" to "Label" for edge function names.
 * Nov 29, 2019 (JD V1.4)
 *  (a) Introduce qDeb() macro to get better debug output control.
 *  (b) Rename "none" mode to "drag" mode, for less confusion.
 *  (c) Added some (incomplete) comments to some functions.
 *  (d) Added many, many debug outputs.
 * Nov 30, 2019 (JD V1.5)
 *  (a) Tidy up some debug outputs, add some others.
 *  (b) Add function comment for keyReleaseEvent() and a few other ones.
 * Dec 13, 2019 (JD V1.6)
 *  (a) Remove unused private var numOfNodes.
 *  (b) Removed qDeb stuff, now in defuns.h, which is now included here.
 *  (c) Added a debug stmt.
 * Mar 30, 2020 (JD V1.7)
 *  (a) Remove deprecated usage of setSortCacheEnabled() in CanvasScene().
 * Jun 17, 2020 (IC V1.8)
 *  (a) Corrected mousePressEvent to properly delete the graph (and parent
 *      graphs) if the last child node is deleted.
 * Jun 19, 2020 (IC V1.9)
 *  (a) Added graphDropped() signal to tell mainWindow to update the edit tab.
 * Jun 26, 2020 (IC V1.10)
 *  (a) Updated mouseMoveEvent and mouseReleaseEvent to only snapToGrid a node
 *      or graph if the item was actually moved.
 * Jul 9, 2020 (IC V1.11)
 *  (a) Added bools to mousePressEvent so we grab the first label/node clicked.
 *  (b) Labels should always receive keyboard focus immediately following a
 *      mouse press event.  Additionally, other graph's bounding rects
 *	should no longer block keyboard focus going to a label.
 *	Further [JD thinks, anyway] add code to avoid snapping a node
 *	to the grid if it wasn't actually dragged.
 *  (c) Added graphJoined() signal to tell mainWindow to update the edit tab.
 * Jul 17, 2020 (IC V1.12)
 *  (a) Corrected keyReleaseEvent to properly renumber the newly joined graph
 *      if the label of the initial selected node contained only a number.
 * Jul 23, 2020 (IC V1.13)
 *  (a) Added searchAndSeparate() function to determine if a graph needs to be
 *      split into individual graphs following a node/edge deletion.
 * Jul 30, 2020 (IC V1.14)
 *  (a) (Partially) Removed the graph recursion from keyReleaseEvent.
 *	Children of the two graphs are now reassigned to a single
 *	graph and the old graph objects are deleted, at least in the
 *	case of 2 nodes selected!
 *      4 nodes selected is still WIP, as it messes up the rotations somehow.
 *  (b) For 4-node joins, ensure that all 4 nodes are distinct.
 * Jul 31, 2020 (IC V1.15)
 *  (a) Added somethingChanged() signal to be used along with the other signals
 *      to tell mainWindow that something has changed on the canvas and thus a
 *      new save prompt is necessary.
 * Aug 12, 2020 (IC V1.16)
 *  (a) Fully removed graph recursion from keyReleaseEvent.  Graphs
 *	should no longer find themselves children of other graphs.
 *	Because of this, nodes and edges need to always have their
 *	rotation reset to 0 whenever they are transferred to a new
 *	parent graph.
 * Aug 14, 2020 (IC V1.17)
 *  (a) Fix two of the known "join" bugs in keyReleaseEvent(), note
 *	one more one. 
 * Aug 19, 2020 (IC V1.18)
 *  (a) Changed the way drag mode works slightly.  The drag will
 *	prioritize moving graphs that had items at the point of click
 *	before defaulting to moving the first graph bounding box found.
 * Aug 20, 2020 (IC V1.19)
 *  (a) Fixed the rotation issue, although the change was made in
 *	graph.cpp.  The rotation of root2 needs to take into account
 *	any previous rotation. 
 *  (b) Added code to the keyReleaseEvent that checks if both sets of nodes
 *      in a 4-node join were connected by an edge and, if so, removes one.
 * Aug 26, 2020 (IC V1.20)
 *  (a) Added updateCellSize which changes the size of mCellSize based on
 *      the user's input to the cellSize widget on the UI and redraws the
 *      cells accordingly.
 * Aug 30, 2020 (JD V1.21)
 *  (a) updateCellSize now uses the saved settings value to determine the size
 *      of grid cells.
 *  (b) Adjust the code for the four-node join operation so that,
 *	rather than co-locating the first nodes selected in each graph,
 *	the midpoint between the selected nodes of the second graph
 *	is translated to the midpoint between the selected nodes of
 *	the first graph.  This tends to maintain more symmetry of the
 *	second graph than co-locating the first vertices.
 * Aug 30, 2020 (IC V1.22)
 *  (a) Modify updateCellSize() as the snap-to-grid cell size is now
 *	in the settings.
 *  (b) Yet more improvements to the now-infamous 4-node join operation.
 * Sep 1, 2020 (IC + JD V1.23)
 *  (a) Added the GRID_DOT_DPI_THRESHOLD macro and updated
 *	drawBackground() to determine when the grid dots should be
 *	single pixels or 2x2 blocks.
 * Sep 11, 2020 (IC V1.24)
 *  (a) Whenever a graph is created, append it to the canvasGraphList and
 *      similarly, whenever a graph is deleted, remove it from the list.
 *  (b) JD: tidy up comments, formatting, ...
 * Nov 9, 2020 (JD V1.25)
 *  (a) Since V1.14 graphs are no longer recursive structures.
 *	Simplify code that looks for the root given a node.
 *	Removed some other redundant tests.
 *  (b) Adjust the four-node join code so that the graph is centered
 *	around its coordinate system origin.  Without this, a joined
 *	graph orbits around some "random" point when it is rotated via
 *	the "Edit Canvas Graph" tab.
 *  (c) The same as (b), but for the two-node join.
 * Nov 11, 2020 (JD V1.26)
 *  (a) Removed a now-bogus (due to updates to graph.cpp & node.cpp) comment.
 * Nov 14, 2020 (JD V1.27)
 *  (a) Update calls to Graph::boundingBox() as per change in args.
 * Nov 16, 2020 (JD V1.28)
 *  (a) Considerable simplification of 2-node and 4-node join code.
 *  (b) After a 2-node or 4-node join, call Graph::centerGraph() to
 *	put the origin of the graph's coordinate system in the
 *	geometric center of the graph.  This makes rotating the graph
 *	(via the Edit Canvas Graph tab) work a lot better.
 * Nov 16, 2020 (JD V1.29)
 *  (a) Animate the moving of the second graph for joins.  Without this,
 *	the joining can be very jarring, especially for the 4-node join.
 */

#include "canvasscene.h"
#include "canvasview.h"
#include "defuns.h"
#include "edge.h"
#include "graph.h"
#include "node.h"

#include <QtDebug>
#include <QGraphicsSceneMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QCursor>
#include <qmath.h>
#include <QApplication>
#include <QPainter>
#include <QtCore>
#include <QtGui>
#include <QThread>

// If the default resolution (DPI) is >= this value, draw each grid
// dot as a 2x2 block instead of a single pixel.
#define GRID_DOT_DPI_THRESHOLD 120

CanvasScene::CanvasScene()
    :  mCellSize(25, 25)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);

    connectNode1a = nullptr;
    connectNode2a = nullptr;
    connectNode1b = nullptr;
    connectNode2b = nullptr;

    modeType = CanvasView::drag;
    mDragged = nullptr;
    snapToGrid = true;
    undoPositions = QList<undo_Node_Pos*>();
}



/*
 * Name:	updateCellSize()
 * Purpose:	Update the size of the "snap-to" grid.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The snap-to grid.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	None.
 */

void
CanvasScene::updateCellSize()
{
    QSize newCellSize(settings.value("gridCellSize").toInt(),
		      settings.value("gridCellSize").toInt());
    mCellSize = newCellSize;
    update();
}



// We get many of these events when dragging the graph from the
// preview window to the main canvas.
// But we don't get any when dragging (existing) things around the canvas.

void
CanvasScene::dragMoveEvent(QGraphicsSceneDragDropEvent * event)
{
    // qDeb() << "CS::dragMoveEvent(" << event->screenPos() << ")";

    Q_UNUSED(event);
}



// We get this when we let go of the mouse after dragging something
// from the preview window to the main canvas.

void
CanvasScene::dropEvent(QGraphicsSceneDragDropEvent * event)
{
    qDeb() << "CS::dropEvent(" << event->screenPos() << ")";

    const GraphMimeData * mimeData
	= qobject_cast<const GraphMimeData *> (event->mimeData());
    if (mimeData)
    {
	Graph * graphItem = mimeData->graphItem();

	graphItem->setPos(event->scenePos().rx()
			  - graphItem->boundingRect().x(),
			  event->scenePos().ry()
			  - graphItem->boundingRect().y());
	addItem(graphItem);
	canvasGraphList.append(graphItem);
	graphItem->isMoved();
	clearSelection();
	emit graphDropped();
    }
}



void
CanvasScene::drawBackground(QPainter * painter, const QRectF &rect)
{
    if (snapToGrid)
    {
	qreal left = int(rect.left()) - (int(rect.left()) % mCellSize.width());
	qreal top = int(rect.top()) - (int(rect.top()) % mCellSize.height());

	for (qreal x = left; x < rect.right(); x += mCellSize.width())
	    for (qreal y = top; y < rect.bottom(); y += mCellSize.height())
	    {
		painter->drawPoint(QPointF(x, y));
		if (settings.value("defaultResolution").toInt()
		    > GRID_DOT_DPI_THRESHOLD)
		{
		    painter->drawPoint(QPointF(x+1, y));
		    painter->drawPoint(QPointF(x, y+1));
		    painter->drawPoint(QPointF(x+1, y+1));
		}
	    }
    }
    else
	QGraphicsScene::drawBackground(painter, rect);
}



/* Apparently this is not called in Freestyle mode, but is called in
 * the other modes */

void
CanvasScene::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    // qDeb() << "CS::mousePressEvent(" << event->screenPos() << ")";

    bool itemFound = false;
    bool nodeFound = false;
    bool labelFound = false;
    bool something_changed = false;

    if (itemAt(event->scenePos(), QTransform()) != nullptr)
    {
	QList<QGraphicsItem *> itemList
	    = items(event->scenePos(), Qt::IntersectsItemShape,
		    Qt::DescendingOrder, QTransform());

	switch (getMode())
	{
	  case CanvasView::join:
	    foreach (QGraphicsItem * item, itemList)
	    {
		QGraphicsItem * parent1 = nullptr;
		QGraphicsItem * parent2 = nullptr;
		if (item->type() == Node::Type)
		{
		    if (connectNode1a == nullptr)
		    {
			connectNode1a = qgraphicsitem_cast<Node*>(item);
			connectNode1a->chosen(1);
			break;
		    }
		    else if (connectNode1b == nullptr)
		    {
			connectNode1b = qgraphicsitem_cast<Node*>(item);
			parent1 = connectNode1a->findRootParent();
			parent2 = connectNode1b->findRootParent();

			if (parent2 == parent1
			    && connectNode1a != connectNode1b)
			{
			    connectNode1b->chosen(2);
			    break;
			}
			else
			    connectNode1b = nullptr;
		    }
		    if (connectNode2a == nullptr)
		    {
			connectNode2a = qgraphicsitem_cast<Node*>(item);
			parent1 = connectNode1a->findRootParent();
			parent2 = connectNode2a->findRootParent();

			if (parent1 != parent2)
			{
			    connectNode2a->chosen(1);
			    break;
			}
			else
			    connectNode2a = nullptr;
		    }
		    else if (connectNode2b == nullptr)
		    {
			connectNode2b = qgraphicsitem_cast<Node*>(item);

			parent1 = connectNode2a->findRootParent();
			parent2 = connectNode2b->findRootParent();

			if (parent2 == parent1
			    && connectNode2a != connectNode2b)
			{
			    connectNode2b->chosen(2);
			    break;
			}
			else
			    connectNode2b = nullptr;
		    }
		}
	    }
	    break;

	  case CanvasView::del:
	    foreach (QGraphicsItem * item, itemList)
	    {
		if (item != nullptr)
		{
		    if (item->type() == HTML_Label::Type)
		    {
			qDeb() << "    mousepress/Delete LABEL";
		    }
		    else if (item->type() == Node::Type)
		    {
			qDeb() << "    mousepress/Delete Node";

			Node * node = qgraphicsitem_cast<Node *>(item);

			// Removes node from undolist if deleted.
			// Safety precaution to avoid null pointers.
			for (int i = 0; i < undoPositions.length(); i++)
			{
			    if (undoPositions.at(i)->node == node)
				undoPositions.removeAt(i);
			}

			// Delete all edges incident to node to be deleted
			QList<Node *> adjacentNodes;
			foreach (Edge * edge, node->edgeList)
			{
			    if (edge != nullptr || edge != 0)
			    {
				if (!adjacentNodes.contains(edge->destNode())
				    && edge->destNode() != node)
				    adjacentNodes.append(edge->destNode());
				else if (!adjacentNodes.contains(edge->sourceNode())
					 && edge->sourceNode() != node)
				    adjacentNodes.append(edge->sourceNode());

				edge->destNode()->removeEdge(edge);
				edge->sourceNode()->removeEdge(edge);

				edge->setParentItem(nullptr);
				removeItem(edge);
				delete edge;
				edge = nullptr;
			    }
			}
			if (adjacentNodes.count() > 1)
			    searchAndSeparate(adjacentNodes);

			Graph * parent =
			    qgraphicsitem_cast<Graph*>(node->parentItem());
			Graph * tempParent;

			// Delete the node.
			node->setParentItem(nullptr);
			removeItem(node);
			delete node;
			node = nullptr;

			// Now delete Graph (and root graphs) if there
			// are no nodes left.
			while (parent != nullptr || parent != 0)
			{
			    tempParent = qgraphicsitem_cast<Graph*>(parent->parentItem());
			    if (parent->childItems().isEmpty())
			    {
				parent->setParentItem(nullptr);
				removeItem(parent);
				canvasGraphList.removeOne(parent);
				delete parent;
				parent = nullptr;
			    }
			    parent = tempParent;
			}
			something_changed = true;
			break;
		    }
		    else if (item->type() == Edge::Type)
		    {
			qDeb() << "    mousepress/Delete Edge";

			Edge * edge = qgraphicsitem_cast<Edge *>(item);
			edge->destNode()->removeEdge(edge);
			edge->sourceNode()->removeEdge(edge);

			edge->setParentItem(nullptr);
			removeItem(edge);

			QList<Node *> adjacentNodes;
			adjacentNodes.append(edge->destNode());
			adjacentNodes.append(edge->sourceNode());
			searchAndSeparate(adjacentNodes);

			delete edge;
			edge = nullptr;
			something_changed = true;
			break;
		    }
		}
	    }
	    if (something_changed)
		emit somethingChanged();
	    break;

	  case CanvasView::edit:
	    qDeb() << "    edit mode...";
	    undo_Node_Pos * undoPos;
	    undoPos = new undo_Node_Pos();

	    foreach (QGraphicsItem * item, itemList)
	    {
		qDeb() << "\titem type is " << item->type();
		if (event->button() == Qt::LeftButton)
		{
		    if (item->type() == HTML_Label::Type && !labelFound)
		    {
			labelFound = true;
			qDeb() << "\tLeft button over a label";
			item->setFocus();
		    }
		    else if (item->type() == Node::Type && !nodeFound)
		    {
			qDeb() << "\tLeft button over a node";
			nodeFound = true;
			mDragged = qgraphicsitem_cast<Node*>(item);
			undoPos->node = qgraphicsitem_cast<Node *>(mDragged);
			undoPos->pos = mDragged->pos();
			undoPositions.append(undoPos);
			if (snapToGrid)
			{
			    mDragOffset = event->scenePos() - mDragged->pos();
			    qDeb() << "    mousepress/edit/node/snap2grid "
				   << "offset = " << mDragOffset;
			}
		    }
		}
	    }
	    if (!labelFound)
		clearFocus();

	    // Crash ensues if this is left uncommented, not sure why.
	    /*if (mDragged != nullptr)
	      if (mDragged->type() == Node::Type)
	      QGraphicsScene::mousePressEvent(event);*/
	    break;

	  case CanvasView::drag:
	    foreach (QGraphicsItem * item, itemList)
	    {
		// First we search for any node/edge/label at point of click.
		if (item != nullptr || item != 0)
		{
		    if (item->type() == Node::Type
			|| item->type() == Edge::Type
			|| item->type() == HTML_Label::Type)
		    {
			itemFound = true;
			mDragged = item;
			while (mDragged->parentItem() != nullptr)
			    mDragged = mDragged->parentItem();

			mDragOffset = event->scenePos() - mDragged->pos();

			QGraphicsScene::mousePressEvent(event);
			break;
		    }
		}
	    }
	    if (!itemFound)
	    {
		// If no graph item was clicked, then find first graph in list
		foreach (QGraphicsItem * item, itemList)
		{
		    if (item != nullptr || item != 0)
		    {
			if (item->type() == Graph::Type)
			{
			    mDragged = item;
			    while (mDragged->parentItem() != nullptr)
				mDragged = mDragged->parentItem();

			    mDragOffset = event->scenePos() - mDragged->pos();

			    QGraphicsScene::mousePressEvent(event);
			    break;
			}
		    }
		}
	    }
	    break;

	  default:
	    break;
	}
    }
    else
    {
	mDragged = nullptr;
	QGraphicsScene::mousePressEvent(event);
    }
}



void
CanvasScene::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
    if (mDragged
	&& (getMode() == CanvasView::drag || getMode() == CanvasView::edit))
    {
	moved = true;
	qDeb() << "CS::mouseMoveEvent: mode is "
	       << CanvasView::getModeName(getMode());
	if (mDragged->type() == Graph::Type)
	{
	    // Ensure that the item's offset from the mouse cursor
	    // stays the same.
	    qDeb() << "    graph dragged "
		   << event->scenePos() - mDragOffset;
	    mDragged->setPos(event->scenePos() - mDragOffset);
	}
	else if (mDragged->type() == Node::Type)
	{
	    qDeb() << "    node drag; event->scenePos = " << event->scenePos();
	    qDeb() << "\tmDragged->mapFromScene(value above) = "
		   << mDragged->mapFromScene(event->scenePos());
	    qDeb() << "\tnode pos set to mDragged->mapToParent(above) = "
		   << mDragged->mapToParent(
		       mDragged->mapFromScene(event->scenePos()));
	    mDragged->setPos(mDragged->mapToParent(
				 mDragged->mapFromScene(event->scenePos())));
	}
    }
}



void
CanvasScene::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    // qDeb() << "CS::mouseReleaseEvent(" << event->screenPos() << ")";

    if (mDragged && snapToGrid && moved
	&& (getMode() == CanvasView::drag || getMode() == CanvasView::edit))
    {
	int x = 0;
	int y = 0;

	if (mDragged->type() == Graph::Type)
	{
	    
	    qDeb() << "\tsnapToGrid processing a graph";
	    x = floor(mDragged->scenePos().x()
		      / mCellSize.width()) * mCellSize.width();
	    y = floor(mDragged->scenePos().y()
		      / mCellSize.height()) * mCellSize.height();
	    mDragged->setPos(x, y);
	}
	else if (mDragged->type() == Node::Type)
	{
	    qDeb() << "\tsnapToGrid processing a node";
	    x = round(mDragged->pos().x() / mCellSize.width())
		* mCellSize.width();
	    y = round(mDragged->pos().y() / mCellSize.height())
		* mCellSize.height();
	    mDragged->setPos(x , y);
	}
	moved = false;

	if (getMode() == CanvasView::edit)
	    emit somethingChanged();
    }
    mDragged = nullptr;
    clearSelection();
    QGraphicsScene::mouseReleaseEvent(event);
}



void
CanvasScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
    qDeb() << "CS::mouseDoubleClickEvent(" << event->screenPos() << ")";

    switch (getMode())
    {
      case CanvasView::del:
	if (itemAt(event->scenePos(), QTransform()) != nullptr)
	{
	    QGraphicsItem * item = itemAt(event->scenePos(), QTransform());

	    Graph * graph = qgraphicsitem_cast<Graph*>(item);

	    if (!graph)
		graph =  qgraphicsitem_cast<Graph*>(item->parentItem());

	    if (graph)
	    {
		while (graph->parentItem() != nullptr
		       && graph->parentItem()->type() == Graph::Type)
		    graph = qgraphicsitem_cast<Graph*>(graph->parentItem());

		removeItem(graph);
		canvasGraphList.removeOne(graph);
		delete graph;
		graph = nullptr;

		emit somethingChanged();
	    }
	}
	break;

      default:
	; // do nothing
    }
    QGraphicsScene::mouseDoubleClickEvent(event);
}



// This was used for debugging join issues.
// Left here in case it is ever useful again.
#if 0
static void
printGraphInfo(Graph * g, QString title)
{
    qDeb() << "Graph info for " << title;
    qDeb() << "   bounding rect: " << g->boundingRect();
    qDeb() << "   bounding rect center: " << g->boundingRect().center();
    qDeb() << "   rotation: " << g->getRotation();
    QPointF center;
    QRectF bb = g->boundingBox(&center, false, nullptr);
    QRectF bb2 = g->boundingBox(&center, true, nullptr);
    qDeb() << "   center is " << center;
    qDeb() << "   bb(node diam ignored) is " << bb;
    qDeb() << "   bb2(node diam used) is   " << bb2;
}
#endif


/*
 * Name:	keyReleaseEvent()
 * Purpose:	When a key is released execute any known function for
 *		that key.  Currently "j" (join (identify) nodes) and
 *		"escape" (undo node move in Edit mode) are the
 *		possible functions.
 * Arguments:	The key event.
 * Outputs:	Nothing.
 * Modifies:	Possibly the graph in major ways.
 * Returns:	Nothing.
 * Assumptions:	connectNode[12][ab] are all meaningful if 'j' is pressed.
 * Bugs:	TODO:
 *		(3) If somehow connectedNode<x> doesn't have a parentItem,
 *		root<n> will be nullptr and eventually dereferenced.
 * Notes:	The four-node join has been a nightmare to get
 *		correct, because of the fact that multiple geometric
 *		transformations have been getting stacked on each
 *		other, and getting the rotation of vertices and edges
 *		(to ensure that labels are printed horizontally) has
 *		been a problem, when joined graphs are joined together.
 *		Since V1.16 it has become simpler (with that version a
 *		new graph is created, the nodes & edges are moved from
 *		the old graphs into the new one, and the old ones are
 *		deleted), but during the changes for V1.25 it was
 *		discovered that setting the rotation of nodes to 0
 *		when moving them from an old graph to a new graph was
 *		not having any effect, so the graph rotation function
 *		was changed (and simplified) in graph.cpp(V1.7) to
 *		take the incorrect rotation values into account.
 *		Having done this, the center of rotation was off in
 *		some "random" location, so the V1.25 changes here put
 *		the center of rotation within the graph.
 */

// Constants for the join animations.
// Without animations the join operation can be jarring and confusing.
#define ANIMATION_STEPS 10  // Pulled out of thin air
#define ANIMATION_DELAY	70  // Ditto

void
CanvasScene::keyReleaseEvent(QKeyEvent * event)
{
    QPointF itemPos;

    switch (event->key())
    {
      case Qt::Key_J:
	qDeb() << "CS:keyReleaseEvent('j')";

	Graph * newRoot;
	Graph * root1;
	Graph * root2;

	if (connectNode1a != nullptr && connectNode2a != nullptr
	    && connectNode1b != nullptr && connectNode2b != nullptr)
	{
	    qDeb() << "CS:keyReleaseEvent('j'); four selected nodes case";

	    // Following if shouldn't be needed... but just in case!
	    if (connectNode1a->parentItem() != connectNode2a->parentItem()
		&& connectNode1a->parentItem() != connectNode2b->parentItem()
		&& connectNode1b->parentItem() != connectNode2a->parentItem()
		&& connectNode1b->parentItem() != connectNode2b->parentItem())
	    {
		newRoot = new Graph();
// needed?	newRoot->isMoved();
		addItem(newRoot);

		root1 = qgraphicsitem_cast<Graph *>(connectNode1a->parentItem());
		QPointF root1Pos = root1->scenePos();
		root2 = qgraphicsitem_cast<Graph *>(connectNode2a->parentItem());
		qDeb() << "    root2  WAS at " << root2->pos();

		// Move the newRoot to where root1 is found.
		newRoot->setPos(root1Pos);

		// Moving root2 to "the right place" is a bit a song
		// and dance.  ***Surely this can be simplified.***
		QPointF cn1a(connectNode1a->scenePos());
		QPointF cn1b(connectNode1b->scenePos());
		QPointF cn2a(connectNode2a->scenePos());
		QPointF cn2b(connectNode2b->scenePos());

		qreal angle1 = qAtan2(cn1b.ry() - cn1a.ry(),
				      cn1b.rx() - cn1a.rx());
		qreal angle2 = qAtan2(cn2b.ry() - cn2a.ry(),
				      cn2b.rx() - cn2a.rx());
		qreal angle = angle1 - angle2;

		qDeb() << "\tcn1a " << cn1a; qDeb() << "\tcn1b " << cn1b;
		qDeb() << "\tcn2a " << cn2a; qDeb() << "\tcn2b " << cn2b;
		qDebu("\tmidpoint of G1 selected vertices: (%.2f, %.2f)",
		      (cn1a.rx() + cn1b.rx()) / 2, (cn1a.ry() + cn1b.ry()) / 2);
		qDebu("\tmidpoint of G2 selected vertices: (%.2f, %.2f)",
		      (cn2a.rx() + cn2b.rx()) / 2, (cn2a.ry() + cn2b.ry()) / 2);
		qDeb() << "\tangle G1 = " << qRadiansToDegrees(angle1);
		qDeb() << "\tangle G2 = " << qRadiansToDegrees(angle2);
		qDeb() << "\tdelta angle = " << qRadiansToDegrees(angle);

		// Rotate the second graph by the calculated angle.
		qreal animate_angle = angle / ANIMATION_STEPS;
		for (int i = 0; i < ANIMATION_STEPS; i++)
		{
		    root2->setRotation(qRadiansToDegrees(animate_angle), true);
		    QCoreApplication::processEvents(QEventLoop::AllEvents);
		    QThread::msleep(ANIMATION_DELAY);
		}

		// To preserve as much symmetry as possible (?),
		// calculate the offset that moves the middle
		// of edge (cn2a, cn2b) to the middle of edge (cn1a, cn1b)
		// Note that cn2a and cn2b have moved since the rotation!
		cn2a = connectNode2a->scenePos();
		cn2b = connectNode2b->scenePos();
		qreal midcn1X = (cn1a.rx() + cn1b.rx()) / 2;
		qreal midcn1Y = (cn1a.ry() + cn1b.ry()) / 2;
		qreal midcn2X = (cn2a.rx() + cn2b.rx()) / 2;
		qreal midcn2Y = (cn2a.ry() + cn2b.ry()) / 2;
		qreal deltaX = midcn1X - midcn2X;
		qreal deltaY = midcn1Y - midcn2Y;
		qDebu("\tmidcn1X = %.1f, midcn2X= %.1f", midcn1X, midcn2X);
		qDebu("\tmidcn1Y = %.1f, midcn2Y= %.1f", midcn1Y, midcn2Y);
		qDebu("\tdeltaX = %.1f, deltaY = %.1f", deltaX, deltaY);
		qreal animate_x = deltaX / ANIMATION_STEPS;
		qreal animate_y = deltaY / ANIMATION_STEPS;
		for (int i = 0; i < ANIMATION_STEPS; i++)
		{
		    root2->moveBy(animate_x, animate_y);
		    QCoreApplication::processEvents(QEventLoop::AllEvents);
		    QThread::msleep(ANIMATION_DELAY);
		}

		// Attached connectNode2a edges to connectNode1a
		foreach (Edge * edge, connectNode2a->edges())
		{
		    if (edge->sourceNode() == connectNode2a)
			edge->setSourceNode(connectNode1a);
		    else
			edge->setDestNode(connectNode1a);
		    connectNode1a->addEdge(edge);
		}

		// Attach connectNode2b edges to connectNode1b
		foreach (Edge * edge, connectNode2b->edges())
		{
		    if (edge->sourceNode() == connectNode2b)
			edge->setSourceNode(connectNode1b);
		    else
			edge->setDestNode(connectNode1b);
		    connectNode1b->addEdge(edge);
		}

		// Now we need to check if node1a and node1b have two edges
		// connecting them and, if so, delete one.
		Edge * existingEdge = nullptr;
		foreach (Edge * edge, connectNode1a->edges())
		{
		    if (edge->sourceNode() == connectNode1b
			|| edge->destNode() == connectNode1b)
		    {
			if (existingEdge == nullptr)
			    existingEdge = edge;
			else
			{
			    connectNode1a->removeEdge(edge);
			    connectNode1b->removeEdge(edge);
			    connectNode2a->removeEdge(edge);
			    connectNode2b->removeEdge(edge);
			    removeItem(edge);
			    delete(edge);
			    edge = nullptr;
			    break;
			}
		    }
		}

		// Move all nodes from root1 to newroot.
		// If the rotation of root1 was 0, we could just use
		// each node's pos(), since the roots are at the same place.
		// However, since root1 might have been rotated, we
		// need to map the scene coords.
		foreach (QGraphicsItem * item, root1->childItems())
		{
		    itemPos = item->scenePos();
		    item->setParentItem(newRoot);
		    item->setPos(itemPos - root1Pos);
		    item->setRotation(0);
		}

		// Move all nodes from root2 to newRoot.
		foreach (QGraphicsItem * item, root2->childItems())
		{
		    itemPos = item->scenePos();
		    item->setParentItem(newRoot);
		    item->setPos(itemPos - root1Pos);
		    item->setRotation(0);
		}

		// This block renumbers all the nodes in the new graph
		// iff the first node selected had a numeric label.
		// See comments in the corresponding block for 2-node
		// join below.
		bool isInt;
		connectNode1a->getLabel().toInt(&isInt);
		if (isInt)
		{
		    int count = 0;

		    foreach (QGraphicsItem * item, newRoot->childItems())
			if (item->type() == Node::Type
			    && item != connectNode2a && item != connectNode2b)
			{
			    Node * node = qgraphicsitem_cast<Node*>(item);
			    node->setNodeLabel(count++);
			}
		}

		// Adjust the joined graph so that its center of
		// rotation is at the geometric center of the nodes,
		// in case the user rotates this graph.
		newRoot->centerGraph();

		canvasGraphList.append(newRoot);

		// Dispose of the now-unneeded nodes.
		connectNode2a->setParentItem(nullptr);
		removeItem(connectNode2a);
		delete connectNode2a;
		connectNode2a = nullptr;

		connectNode2b->setParentItem(nullptr);
		removeItem(connectNode2b);
		delete connectNode2b;
		connectNode2b = nullptr;

		// Dispose of old roots
		removeItem(root1);
		canvasGraphList.removeOne(root1);
		delete root1;
		root1 = nullptr;

		removeItem(root2);
		canvasGraphList.removeOne(root2);
		delete root2;
		root2 = nullptr;

		emit graphJoined();
	    }
	}
	else if (connectNode1a != nullptr && connectNode2a != nullptr)
	{
	    qDeb() << "CS:keyReleaseEvent('j'); two selected nodes case";
	    qDeb() << "\tn1 label /" << connectNode1a->getLabel()
		   << "/; n2 label /" << connectNode2a->getLabel() << "/";

	    // Following if shouldn't be needed... but just in case!
	    if (connectNode1a->parentItem() != connectNode2a->parentItem())
	    {
		newRoot = new Graph();
// needed?	newRoot->isMoved();
		addItem(newRoot);

		root1 = qgraphicsitem_cast<Graph *>(connectNode1a->parentItem());
		QPointF root1Pos = root1->scenePos();
		root2 = qgraphicsitem_cast<Graph *>(connectNode2a->parentItem());

		// Move the newRoot to root1's location.
		newRoot->setPos(root1Pos);

		// Move root2 so that the two selected nodes are coincident.
		qreal animate_x = (connectNode1a->scenePos()
				    - connectNode2a->scenePos()).x()
		    / ANIMATION_STEPS;
		qreal animate_y = (connectNode1a->scenePos()
				    - connectNode2a->scenePos()).y()
		    / ANIMATION_STEPS;
		for (int i = 0; i < ANIMATION_STEPS; i++)
		{
		    root2->moveBy(animate_x, animate_y);
		    QCoreApplication::processEvents(QEventLoop::AllEvents);
		    QThread::msleep(ANIMATION_DELAY);
		}

		foreach (Edge * edge, connectNode2a->edges())
		{
		    qDeb() << "\tlooking at n2's edge ("
			   << edge->sourceNode()->getLabel() << ", "
			   << edge->destNode()->getLabel() << ")";
		    // Replace n2 in this edge with n1
		    if (edge->sourceNode() == connectNode2a)
			edge->setSourceNode(connectNode1a);
		    else
			edge->setDestNode(connectNode1a);
		    // ... and add this edge to n1's list of edges.
		    connectNode1a->addEdge(edge);
		    edge->setZValue(0);		    // TODO: archaic?
		    connectNode1a->setZValue(3);    // TODO: archaic?
		}

		// Move all nodes from root1 to newRoot.
		// If the rotation of root1 was 0, we could just use
		// each node's pos(), since the roots are at the same place.
		// However, since root1 might have been rotated, we
		// need to map the scene coords.
		foreach (QGraphicsItem * item, root1->childItems())
		{
		    itemPos = item->scenePos();
		    item->setParentItem(newRoot);
		    item->setPos(itemPos - root1Pos);
		    item->setRotation(0);
		}

		// Move all nodes from root2 to newRoot.
		foreach (QGraphicsItem * item, root2->childItems())
		{
		    itemPos = item->scenePos();
		    item->setParentItem(newRoot);
		    item->setPos(itemPos - root1Pos);
		    item->setRotation(0);
		}

		// If n1 has a numeric label, renumber all the nodes from
		// 0 on up.
		// TODO: something intelligent when n1's label is of the form
		//		<letter>{^<anything>}_<number>
		// Alternatively, since this functionality is now
		// available from the Edit Canvas Graph tab, it could go.
		bool isInt;
		connectNode1a->getLabel().toInt(&isInt);
		if (isInt)
		{
		    int count = 0;

		    foreach (QGraphicsItem * item, newRoot->childItems())
			if (item->type() == Node::Type
			    && item != connectNode2a)
			{
			    Node * node = qgraphicsitem_cast<Node*>(item);
			    node->setNodeLabel(count++);
			}
		}
		
		// Adjust the joined graph so that its center of
		// rotation is at the geometric center of the nodes,
		// in case the user rotates this graph.
		newRoot->centerGraph();

		canvasGraphList.append(newRoot);

		// Properly dispose of the now unneeded node.
		removeItem(connectNode2a);
		delete connectNode2a;
		connectNode2a = nullptr;

		// Dispose of old roots
		removeItem(root1);
		canvasGraphList.removeOne(root1);
		delete root1;
		root1 = nullptr;

		removeItem(root2);
		canvasGraphList.removeOne(root2);
		delete root2;
		root2 = nullptr;

		emit graphJoined();
	    }
	}

	// Un-select selected nodes after a join.
	if (connectNode1a)
	{
	    connectNode1a->chosen(0);
	    connectNode1a = nullptr;
	}

	if (connectNode2a)
	{
	    connectNode2a->chosen(0);
	    connectNode2a = nullptr;
	}

	if (connectNode1b)
	{
	    connectNode1b->chosen(0);
	    connectNode1b = nullptr;
	}

	if (connectNode2b)
	{
	    connectNode2b->chosen(0);
	    connectNode2b = nullptr;
	}

	clearSelection();
	break;

      case Qt::Key_Escape:
	if (undoPositions.length() > 0)
	{
	    undoPositions.last()->node->setPos(undoPositions.last()->pos);
	    undoPositions.removeLast();

	    emit somethingChanged();
	}

      default:
	break;
    }

    QGraphicsScene::keyReleaseEvent(event);
}



void
CanvasScene::setCanvasMode(int mode)
{
    qDeb()  << "CS::setCanvasMode(" << mode << ") called; "
	    << "previous mode was " << modeType;

    modeType = mode;

    if (connectNode1a)
    {
	connectNode1a->chosen(0);
	connectNode1a = nullptr;
    }
    if (connectNode2a)
    {
	connectNode2a->chosen(0);
	connectNode2a = nullptr;
    }
    if (connectNode1b)
    {
	connectNode1b->chosen(0);
	connectNode1b = nullptr;
    }
    if (connectNode2b)
    {
	connectNode2b->chosen(0);
	connectNode2b = nullptr;
    }
    undoPositions.clear();

    foreach (QGraphicsItem * item, items())
    {
	if (item->type() == Node::Type)
	{
	    Node * node = qgraphicsitem_cast<Node *>(item);
	    if (modeType == CanvasView::edit)
		node->editLabel(true);
	    else
		node->editLabel(false);
	}
	else if (item->type() == Edge::Type)
	{
	    Edge * edge = qgraphicsitem_cast<Edge *>(item);
	    if (modeType == CanvasView::edit)
		edge->editLabel(true);
	    else
		edge->editLabel(false);
	}
    }
}



void
CanvasScene::isSnappedToGrid(bool snap)
{
    snapToGrid = snap;
}



int
CanvasScene::getMode() const
{
    return modeType;
}



/*
 * Name:	searchAndSeparate()
 * Purpose:	Determines whether new graph items need to be made
 *              as a result of deleting an edge/node.
 * Arguments:	A list of nodes incident to the item being deleted.
 * Outputs:	Nothing.
 * Modifies:	Potentially the graph(s) on the canvas.
 * Returns:	Nothing.
 * Assumptions:	Atleast 2 nodes are in the list.
 * Bugs:	?
 * Notes:	None.
 */

void
CanvasScene::searchAndSeparate(QList<Node *> Nodes)
{
    QList<Node *> graphNodes; // Checks if nodes in passed list are reachable
    QList<QGraphicsItem *> graphItems; // Stores items for new graph
    QList<int> skipList;	    // Used to skip indexes of reachable nodes
    QPointF itemPos;
    Node * node1;
    Node * node2;
    int i = 0;
    int j = 1;
    bool graphAdded = false;

    while (i < Nodes.indexOf(Nodes.last()))
    {
	node1 = Nodes.at(i);
	graphNodes.append(node1);
	graphItems.append(node1);

	while (!graphNodes.isEmpty())
	{
	    for (Node * node : graphNodes)
	    {
		// Check if this node can reach any nodes in the passed list
		while (j <= Nodes.indexOf(Nodes.last()))
		{
		    node2 = Nodes.at(j);
		    if (node == node2)
			skipList.append(j);
		    j++;
		}
		j = i + 1; // Reset j
		node->checked = 1;
		// Add neighbor nodes to graphNodes and neighbor nodes/edges
		// to graphItems in case we need to make a new graph
		foreach (Edge * edge, node->edgeList)
		{
		    if (!graphNodes.contains(edge->destNode())
			&& edge->destNode()->checked == 0)
		    {
			graphNodes.append(edge->destNode());
			if (!graphItems.contains(edge->destNode()))
			    graphItems.append(edge->destNode());
		    }
		    else if (!graphNodes.contains(edge->sourceNode())
			     && edge->sourceNode()->checked == 0)
		    {
			graphNodes.append(edge->sourceNode());
			if (!graphItems.contains(edge->sourceNode()))
			    graphItems.append(edge->sourceNode());
		    }
		    if (!graphItems.contains(edge))
			graphItems.append(edge);

		    edge->checked = 1;
		}
		graphNodes.removeOne(node);
	    }
	}

	// Only make a new graph if at least one node from the passed list
	// is not reachable.
	if (skipList.count() != (Nodes.count() - i - 1))
	{
	    Graph * graph = new Graph;

	    graphAdded = true;
	    addItem(graph);
	    canvasGraphList.append(graph);

	    foreach (QGraphicsItem * item, graphItems)
	    {
		itemPos = item->scenePos(); // MUST BE scenePos(), NOT pos()
		item->setParentItem(graph);
		item->setPos(itemPos);
		item->setRotation(0); // Reset rotation to 0
	    }
	}

	// Reset all the checked items.
	foreach (QGraphicsItem * item, graphItems)
	{
	    if (item->type() == Node::Type)
	    {
		Node * node = qgraphicsitem_cast<Node *>(item);
		node->checked = 0;
	    }
	    else if (item->type() == Edge::Type)
	    {
		Edge * edge = qgraphicsitem_cast<Edge *>(item);
		edge->checked = 0;
	    }
	}
	graphItems.clear();

	// Skip any nodes reachable by a previous node
	i++;
	while (skipList.contains(i))
	    i++;
	skipList.clear();
	j = i + 1;
    }

    if (graphAdded)
	emit graphSeparated();
}
