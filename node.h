/*
 * File:    node.cpp
 * Author:  Rachel Bood
 * Date:    2014/11/07
 * Version: 1.5
 *
 * Purpose: Declare the node class.
 * 
 * Modification history:
 * Oct 13, 2019 (JD V1.1)
 *  (a) Remove unused lSize from here and node.cpp.
 *  (b) Minor formatting changes.
 *  (c) renamed "choose" to "penStyle".
 * Nov 13, 2019 (JD V1.2)
 *  (a) rename Label -> HTML_Label, label-h -> html-label.h,
 *      remove strToHtml() decl.
 * Nov 13, 2019 (JD V1.3)
 *  (a) rename HTML_Label text to HTML_Label htmlLabel.
 *  (b) delete cruft.
 * Nov 30, 2019 (JD V1.4)
 *  (a) Remove setNodeLabel(qreal) and replace it with setNodeLabel(int).
 *	Ditto for setNodeLabel(QString, qreal).
 * Dec 8, 2019 (JD V1.5)
 *  (a) Add preview X and Y coords and setter/getters.
 *  (b) Remove edgeWeight, which is used nowhere.
 * May 11, 2020 (IC V1.6)
 *  (a) Changed logicalDotsPerInchX variable to physicalDotsPerInchX
 *  to correct scaling issues. (Only reliable with Qt V5.12.2 or higher)
 *  (b) Removed unused physicalDotsPerInchY variable as only one DPI
 *  value is needed for the node's radius.
 */


#ifndef NODE_H
#define NODE_H

#include "html-label.h"
#include <QGraphicsItem>
#include <QList>
#include <QGraphicsSceneMouseEvent>

class Edge;
class CanvasView;
class PreView;

class Node : public QGraphicsObject
{
  public:
    Node();

    void addEdge(Edge * edge);

    bool removeEdge(Edge * edge);

    void setDiameter(qreal diameter);
    qreal getDiameter();

    void setRotation(qreal aRotation);
    qreal getRotation();

    void setFillColour(QColor fColor);
    QColor getFillColour();

    void setLineColour(QColor lColor);
    QColor getLineColour();
    QGraphicsItem * findRootParent();
    void setID(int id);
    int getID();

    void setNodeLabel(int number);
    void setNodeLabel(QString aLabel);
    void setNodeLabel(QString aLabel, int number);
    void setNodeLabel(QString aLabel, QString subscript);
    void setNodeLabelSize(qreal labelSize);

    void setPreviewCoords(qreal x, qreal y);
    qreal getPreviewX();
    qreal getPreviewY();
    
    QString getLabel() const;
    qreal getLabelSize() const;

    QRectF boundingRect() const;

    enum { Type = UserType + 1 };
    int type() const { return Type; }

    QList<Edge *> edgeList;

    QList<Edge *> edges() const;
    void chosen(int group1);

    void editLabel(bool edit);
    // ~Node();

  protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent * event);  
    void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
	       QWidget * widget);

  signals:
    void nodeDeleted();

  private:
    QPointF	newPos;
    qreal	nodeDiameter, rotation;
    QString	label;
    HTML_Label	* htmlLabel;
    QColor	nodeLine, nodeFill;
    int		nodeID;		    // The (internal) number of the node.
    int		penStyle;
    bool	select;
    qreal	physicalDotsPerInchX;
    void	labelToHtml();
    qreal	previewX;
    qreal	previewY;
};

#endif // NODE_H
