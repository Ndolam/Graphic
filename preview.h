/*
 * File:    preview.h
 * Author:  Rachel Bood 100088769
 * Date:    2014/11/07 (?)
 * Version: 1.3
 *
 * Purpose: define the fields of the preview class.
 *
 * Modification history:
 * Dec 7, 2019 (JD V1.1)
 *  (a) Add the currentGraphTypeIndex field (& 3 similar friends).
 *  (b) Remove the return type of Create_Graph() since it wasn't used.
 * Dec 10, 2019 (JD V1.2)
 *  (a) #include "defuns.h".
 * Dec 12, 2019
 *  (a) Add nodeDiameter param to Create_Graph() (for future use).
 *  (b) Remove currentGraphTypeIndex & friends (now in mainwindow.cpp).
 *  (c) Rename Create_Graph() to Create_Basic_Graph() for specificity.
 *      Other minor identifier renaming as well.
 *  (d) Removed unused private vars QColor edgeLine, nodeLine, nodeFill;
 *      and bool modified; and QImage image; as well as undefined
 *	function bool isComplete(); also unnecessary (it seems)
 *	class Node; and class Edge; statements.
 * May 25, 2020 (IC V1.3)
 *  (a) Added numStart param to Style_Graph() to allow numbering of nodes
 *	to start at a specified value instead of only 0.
 */

#ifndef PREVIEW_H
#define PREVIEW_H

#include "defuns.h"
#include "graph.h"

#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>


class PreView: public QGraphicsView
{
    Q_OBJECT
  public:
    PreView(QWidget * parent = 0);

    public slots:
      void zoomIn();
      void zoomOut();
      void Create_Basic_Graph(int graphType, int numOfNodes1, int numOfNodes2,
			      qreal nodeDiameter, bool drawEdges);

      void Style_Graph(Graph * graph, int graphType,
		       enum widget_ID what_changed, qreal nodeDiameter,
		       QString topNodeLabels,	    QString bottomNodeLabels,
		       bool labelsAreNumbered,	    qreal nodeLabelSize,
		       QColor nodeFillColor,	    QColor nodeOutlineColor,
		       qreal edgeSize,		    QString edgeLabel,
		       qreal edgeLabelSize,	    QColor edgeLineColor,
		       qreal totalWidth,	    qreal totalHeight,
		       qreal rotation,		    qreal numStart);

  protected:
    virtual void keyPressEvent(QKeyEvent * event);
    virtual void scaleView(qreal scaleFactor);
    virtual void mousePressEvent(QMouseEvent * event);

  private:
    QGraphicsScene * PV_Scene;
};

#endif // PREVIEW_H
