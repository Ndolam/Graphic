/*
 * File:	graph.h
 * Author:	Rachel Bood
 * Date:	2014 or 2015?
 * Version:	1.8
 *
 * Purpose:	Define the graph class.
 *
 * Modification history:
 * Oct 10, 2019 (JD)
 *  (a) Add this header comment.
 *  (b) Remove unused field "point_star" from the graph struct.
 *  (c) Minor reformatting.
 * Jun 17, 2020 (IC V1.1)
 *  (a) Changed class type from QGraphicsItem to QGraphicsObject for access to
 *      destroyed() signal in connect statements for graphs.
 * Aug 14, 2020 (IC V1.2)
 *  (a) Add "keepRotation" param to setRotation().
 *  (b) Add a private "rotation" variable and its "getRotation()" getter.
 * Oct 18, 2020 (JD V1.3)
 *  (a) Remove the apparently-unused cornergrabber.h.
 * Nov 7, 2020 (JD V1.4)
 *  (a) Rename the second param of setRotation() to make its purpose
 *	clearer (at least to me).
 * Nov 9, 2020 (JD V1.5)
 *  (a) Add the boundingBox() function to return a tight bounding box
 *      for a graph, and, optionally the center of this box.
 * Nov 11, 2020 (JD V1.6)
 *  (a) Removed rotation as a graph attribute.
 * Nov 14, 2020 (JD V1.7)
 *  (a) Added the third arg to boundingBox().
 * Nov 16, 2020 (JD V1.8)
 *  (a) Added centerGraph() function.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <QGraphicsItemGroup>

class CanvasView;
class Node;
class Edge;

class Graph : public QGraphicsObject
{
  public:
    typedef struct graph
    {
        QList<Node *> cycle;
        QList<QList <Node *>> double_cycle;
        QList<QList <Node *>> list_of_cycles;
        QList<Node *> bipartite_top;
        QList<Node *> bipartite_bottom;
        QList<Node *> grid;
        QList<Node *> path;
        QVector<Node *> binaryHeap;
        Node * center;
    } Nodes;

    Graph();
    void isMoved();
    enum {Type = UserType + 3};
    int type() const {return Type;}
    Nodes nodes;

    QRectF boundingRect() const;
    void setRotation(qreal rotationAmount, bool rotationIsRelative);
    qreal getRotation();
    QGraphicsItem * getRootParent();
    QRectF boundingBox(QPointF * center, bool useNodeSizes, QPointF * RGcenter);
    void centerGraph();

  protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void paint(QPainter * painter,
	       const QStyleOptionGraphicsItem * option,
	       QWidget * widget);

  private:
    int moved;		// 1 means the graph was dropped onto the canvas.
};

#endif // GRAPH_H
