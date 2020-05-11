/*
 * File:    node.cpp
 * Author:  Rachel Bood
 * Date:    2014/11/07
 * Version: 1.8
 *
 * Purpose: creates a node for the users graph
 *
 * Modification history:
 * Feb 8, 2016 (JD V1.1):
 *  (a) Modify setWeight(QString) so that it uses cmr10 if the string
 *      is numeric.  This function is (apparently) called by the
 *      callback in labelcontroller.cpp when a user edits a label.
 *      (Is it possible instead to test there, and, if so, should we?)
 *  (b) Drive-by typo and formatting corrections, general cleanup.
 *      Note that node.lSize is currently written to, never read.
 * Oct 13, 2019 (JD V1.2)
 *  (a) Remove unused lSize from here and node.h.
 *  (b) Add '_' and the subscript to label (when there is a subscript).
 *  (c) Format tweaking and comment improvements/additions.
 *      Deleted and/or commented out some spurious code.
 *  (d) Renamed "choose" to "penStyle".
 *  (e) Added code to parse vertex labels and create corresponding HTML
 *      for text->setHtml().  This code assumes (requires) that sub-
 *	and superscripts are enclosed in {}.
 *	TODO: the labels are now displayed on the canvas using cmmi10.
 *	      However, TeX outputs digits (at least sub and sup) in cmr,
 *	      so I need to go through this code and see if I can get
 *	      all the fonts correct in the HTML text.
 * Nov 13, 2019 (JD V1.3)
 *  (a) Move the strToHtml() code to html-label.{h,cpp} (where it was
 *	partially rewritten anyway).  Modify labelToHtml() accordingly.
 *	digits are rendered in cmr10, so that they look more like what
 *	TeX will create.  The other parts are rendered in cmmi10.
 *	The code could be better, but it seems to display OK and is
 *	never written out to a file.
 * Nov 13, 2019 (JD V1.4)
 *  (a) Rename HTML_Label text to HTML_Label htmlLabel.
 * Nov 30, 2019 (JD V1.5)
 *  (a) Remove setNodeLabel(qreal) and replace it with setNodeLabel(int).
 *	Ditto for setNodeLabel(QString, qreal).
 *  (b) Simplify all the various setNodeLabel()s to create a string and
 *	then call setNodeLabel(QString), so that they all do the same things.
 *  (c) Removed the redundant call to edge->adjust() from addEdge().
 *  (d) Added in the new and improved qDeb() / DEBUG stuff.
 * Dec 1, 2019 (JD V1.6)
 *  (a) When a node diameter is changed, also tell its edges about
 *	this change.  (It may be the case that this is totally
 *	irrelevant, but until such time that I am convinced the edge's
 *	sourceRadius and destRadius are meaningless, I will try to
 *	make them correct.)
 *  (b) Improved(?) the comment for itemChange().
 * Dec 1, 2019 (JD V1.7)
 *  (a) Add preview X and Y coords and setter/getters.
 *  (b) Remove edgeWeight, which is used nowhere.
 * Dec 13, 2019 (JD V1.8)
 *  (a) Added defuns.h, removed debug stuff.
 * May 11, 2020 (IC V1.9)
 *  (a) Changed logicalDotsPerInchX variable to physicalDotsPerInchX
 *  to correct scaling issues. (Only reliable with Qt V5.12.2 or higher)
 *  (b) Removed unused physicalDotsPerInchY variable as only one DPI
 *  value is needed for the node's radius.
 */

#include "defuns.h"
#include "edge.h"
#include "node.h"
#include "canvasview.h"
#include "preview.h"

#include <QTextDocument>
#include <QKeyEvent>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QMessageBox>
#include <QDebug>
#include <QMimeData>
#include <QDrag>
#include <QtCore>

/*
 * Name:        Node
 * Purpose:     Contructor for Node class.
 * Arguments:   canvas pointer, diameter of node, edge size
 * Output:      none
 * Modifies:    Private node variables.
 * Returns:     none
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

Node::Node()
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setZValue(2);
    nodeID = -1;
    penStyle = 0;	// What type of pen style to use when drawing outline.
    nodeDiameter = 1;
    rotation = 0;
    htmlLabel = new HTML_Label(this);
    setHandlesChildEvents(true);
    select = false;		    // TODO: is 'select' of any use?
    QScreen * screen = QGuiApplication::primaryScreen();
    physicalDotsPerInchX = screen->physicalDotsPerInchX();
}

/*
 * Name:        addEdge
 * Purpose:     adds an Edge to the pointer QList of edges
 * Arguments:   an Edge pointer
 * Output:      Nothing.
 * Modifies:    The node's edgeList.
 * Returns:     Nothing.
 * Assumptions: edgeList is valid.
 * Bugs:        none...so far
 * Notes:       none
 */

void
Node::addEdge(Edge * edge)
{
    edgeList << edge;
}



/*
 * Name:        removeEdge()
 * Purpose:     Remove an edge from the edgelist.
 * Arguments:   Edge *
 * Output:      Nothing.
 * Modifies:    edgeList
 * Returns:     True if edge was removed, otherwise false.
 * Assumptions: edgeList is valid.
 * Bugs:        none
 * Notes:       none
 */

bool Node::removeEdge(Edge * edge)
{
    for (int i = 0; i < edgeList.length(); i++)
    {
        if (edgeList.at(i) == edge)
        {
            edgeList.removeAt(i);
            return true;
        }
    }

    return false;
}



/*
 * Name:        setDiameter()
 * Purpose:     Sets the size of the diameter of the node in "physical DPI".
 *		Notifies its edges that one of their nodes changed.
 * Arguments:   qreal
 * Output:      Nothing.
 * Modifies:    nodeDiameter
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       The argument diameter is the diameter in inches, therefore
 *              the value must be converted back to pixels in order for the
 *              node to be drawn correctly.
 */

void
Node::setDiameter(qreal diameter)
{
    nodeDiameter = diameter * physicalDotsPerInchX;
    foreach (Edge * edge, edgeList)
	edge->adjust();
    update();
}



/*
 * Name:        getDiameter()
 * Purpose:     Returns diameter of the node (in inches)
 * Arguments:   None.
 * Output:      Nothing.
 * Modifies:    Nothing.
 * Returns:     The node diameter, in inches.
 * Assumptions: none
 * Bugs:        none
 * Notes:       nodeDiamter is stored in pixels, it needs to be converted back
 *              to inches before it is returned.
 *              FUTURE WORK: create multiple getDiameter() functions to return
 *              different values....or create one function that will return
 *              the diameter in the desired unit of measurement.
 */

qreal Node::getDiameter()
{
    return nodeDiameter / physicalDotsPerInchX;
}



/*
 * Name:        setRotation()
 * Purpose:     Sets the rotation of the node.
 * Arguments:   qreal
 * Output:      none
 * Modifies:    rotation
 * Returns:     none
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

void
Node::setRotation(qreal aRotation)
{
   rotation = aRotation;
   QGraphicsItem::setRotation(aRotation);
}



/*
 * Name:        getRotation()
 * Purpose:     returns the rotation of the node
 * Arguments:   none
 * Output:      qreal
 * Modifies:    none
 * Returns:     rotation
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

qreal
Node::getRotation()
{
    return rotation;
}



/*
 * Name:        setFillColour()
 * Purpose:     Sets the fill colour of the node.
 * Arguments:   QColor
 * Output:      Nothing.
 * Modifies:    nodeFill
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

void
Node::setFillColour(QColor fColor)
{
    nodeFill = fColor;
    update();
}



/*
 * Name:        getFillColor()
 * Purpose:     Returns the fill colour of the node.
 * Arguments:   None.
 * Output:      Nothing.
 * Modifies:    Nothing.
 * Returns:     nodeFill.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

QColor
Node::getFillColour()
{
    return nodeFill;
}



/*
 * Name:        setLineColour()
 * Purpose:     Sets the outline colour of the node.
 * Arguments:   QColor
 * Output:      Nothing.
 * Modifies:    nodeLine
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

void
Node::setLineColour(QColor lColor)
{
    nodeLine = lColor;
    update();
}



/*
 * Name:        getLineColor()
 * Purpose:     Returns the outline colour of the node.
 * Arguments:   None.
 * Output:      Nothing.
 * Modifies:    Nothing.
 * Returns:     nodeline
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

QColor
Node::getLineColour()
{
    return nodeLine;
}



/*
 * Name:        findRootParent()
 * Purpose:     Finds the root parent of this node.
 * Arguments:   None.
 * Output:      Nothing.
 * Modifies:    Nothing.
 * Returns:     QGraphicsItem * root.
 * Assumptions: None.
 * Bugs:        none
 * Notes:       none
 */

QGraphicsItem *
Node::findRootParent()
{
    QGraphicsItem * root = this->parentItem();
    while (root->parentItem() != 0 && root->parentItem() != nullptr)
        root = root->parentItem();

    return root;
}



/*
 * Name:        setID()
 * Purpose:     Sets the ID (i.e., the internal node number) of the node.
 * Arguments:   int
 * Output:      Nothing.
 * Modifies:    The nodeID of this node.
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

void
Node::setID(int id)
{
    nodeID = id;
}



/*
 * Name:        getID()
 * Purpose:     Returns the ID of this node.
 * Arguments:   None.
 * Output:      Nothing.
 * Modifies:    Nothing.
 * Returns:     The int nodeID
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

int
Node::getID()
{
    return nodeID;
}



/*
 * Name:        edges()
 * Purpose:     Returns the list of edges incident to this node.
 * Arguments:   None.
 * Output:      Nothing.
 * Modifies:    Nothing.
 * Returns:     QList<Edge *> edgeList
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

QList<Edge *>
Node::edges() const
{
    return edgeList;
}



/*
 * Name:        setNodeLabel()
 * Purpose:     Sets the label of the node to an integer.
 * Arguments:   An int, the node label.
 * Output:      Nothing.
 * Modifies:    The text in the node label (both "htmlLabel" and "label" fields).
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
 */

void
Node::setNodeLabel(int number)
{
    QString nlabel = QString::number(number);
    setNodeLabel(nlabel);
}



/*
 * Name:        setNodeLabel(QString, int)
 * Purpose:     Sets the label of the node in the case where the label
 *		has a numeric subscript.
 * Arguments:   QString, int
 * Output:      Nothing.
 * Modifies:    The text in the node label (both "htmlLabel" and "label" fields).
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

void
Node::setNodeLabel(QString aLabel, int number)
{
    setNodeLabel(aLabel, QString::number(number));
}



/*
 * Name:        setNodeLabel(QString, QString)
 * Purpose:     Sets the label of the node in the case where the label
 *		has a string subscript.
 * Arguments:   QString, QString
 * Output:      Nothing.
 * Modifies:    The text in the node label (both "text" and "label" fields).
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

void
Node::setNodeLabel(QString aLabel, QString subscript)
{
    QString newLabel = aLabel + "_{" + subscript + "}";
    setNodeLabel(newLabel);
}



/*
 * Name:        setNodeLabel(QString)
 * Purpose:     Sets the label of the node.
 * Arguments:   QString
 * Output:      Nothing.
 * Modifies:    The text in the node label (both "text" and "label" fields).
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       This is (apparently) called from the labelcontroller.cpp
 *		callback, which doesn't distinguish between integer and
 *		string.  So do a test to choose the correct font.
 *		TODO: eh??
 */

void
Node::setNodeLabel(QString aLabel)
{
    label = aLabel;
    labelToHtml();
}



/*
 * Name:	labelToHtml()
 * Purpose:	Call lbelToHtml2 to parse the label string, turn it
 *		into HTML, wrap it in cmmi10 font tags, and return that text.
 * Arguments:	None (uses this node's label).
 * Outputs:	Nothing.
 * Modifies:	This node's text field.
 * Returns:	Nothing.
 * Assumptions:
 * Bugs:	Should return a success indication.
 * Notes:	TeX outputs digits in math formula in cmr, and so if I
 *		want this to look extremely TeX-like I actually need to
 *		go around changing fonts depending on whether a char
 *		is a digit or a non-digit.  **sigh**
 *		TODO: something for another day; image exports will look
 *		
 */

void
Node::labelToHtml()
{
    qDeb() << "labelToHtml() looking at node " << nodeID
	   << " with label " << label;

    QString html = HTML_Label::strToHtml(label);
    htmlLabel->setHtml(html);

    qDeb() <<  "labelToHtml setting htmlLabel to /" << html
	   << "/ for /" << label << "/";
}



/*
 * Name:        setNodeLabel()
 * Purpose:     Sets the label of the node.
 * Arguments:   QString, qreal, QString
 * Output:      none
 * Modifies:    the htmlLabel in the node's label
 * Returns:     none
 * Assumptions: none
 * Bugs:        none
 * Notes:       IS THIS CALLED FROM ANYWHERE?   NO?
 *		IS IT UP-TO-DATE?
 */

// Defunct?
//void Node::setNodeLabel(QString htmltext, qreal labelSize, QString aLabel)
//{
//    qDebug() << "setNodeLabel(QString, qreal, QString) called!";
//
//    label = aLabel;
//    htmlLabel->setHtml(htmltext);
//    QFont font = htmlLabel->font();
//    font.setPointSize(labelSize);
//    htmlLabel->setFont(font);
//    lSize = labelSize;
//    update();
//}



/*
 * Name:        setNodeLabelSize()
 * Purpose:     Sets the font size of the node's label.
 * Arguments:   qreal
 * Output:      Nothing.
 * Modifies:    The node's text and fontsize.
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       Should this be renamed to be "setNodeFontSize()" ?
 */

void
Node::setNodeLabelSize(qreal labelSize)
{
    QFont font = htmlLabel->font();
    font.setPointSize(labelSize);
    htmlLabel->setFont(font);
}



/*
 * Name:        getLabel()
 * Purpose:     Returns the label string for this node.
 * Arguments:   None.
 * Output:      QString
 * Modifies:    Nothing.
 * Returns:     The string contained in the label.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

QString
Node::getLabel() const
{
    return label;
}



/*
 * Name:        getLableSize()
 * Purpose:     Returns the font size of the label.
 * Arguments:   None.
 * Output:      Nothing/
 * Modifies:    Nothing.
 * Returns:     The font size as a qreal.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

qreal
Node::getLabelSize() const
{
    return htmlLabel->font().pointSizeF();
}



/*
 * Name:        boundingRect()
 * Purpose:     Determines the bounding rectangle of the node.
 * Arguments:   None.
 * Output:      Nothing.
 * Modifies:    Nothing.
 * Returns:     QRectF
 * Assumptions: none
 * Bugs:        none
 * Notes:       TODO: Q: Is adjust some empirical fudge factor?
 */

QRectF
Node::boundingRect() const
{
    qreal adjust = 2;

    return QRectF( (-1 * nodeDiameter / 2) - adjust,
                   (-1 * nodeDiameter / 2) - adjust,
                   nodeDiameter + 3 + adjust,
                   nodeDiameter + 3 + adjust);
}



/*
 * Name:        chosen()
 * Purpose:     An int used to determine how to draw the outline of the node.
 * Arguments:   integer
 * Output:      Nothing.
 * Modifies:    penStyle: the integer used in the Node::paint() function.
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none.
 */

void
Node::chosen(int pen_style)
{
    penStyle = pen_style;
    update();
}



/*
 * Name:        editLabel()
 * Purpose:     Change edit flags to specify if the label is editable.
 * Arguments:   boolean
 * Output:      Nothing.
 * Modifies:    ItemIsFocusable, ItemIsSelectable, setHandlesChildEvents flags
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

void
Node::editLabel(bool edit)
{
    setHandlesChildEvents(!edit);
    htmlLabel->setFlag(QGraphicsItem::ItemIsFocusable, edit);
    htmlLabel->setFlag(ItemIsSelectable, edit);
}



/*
 * Name:        ~Node()
 * Purpose:     destructor function
 * Arguments:   none
 * Output:      none
 * Modifies:    none
 * Returns:     none
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */
//Node::~Node()
//{
//    emit nodeDeleted();
//    foreach(Edge * edge, edgeList)
//       delete edge;

//    htmlLabel->setParentItem(nullptr);
//    delete htmlLabel;
//    htmlLabel = nullptr;
//    setParentItem(nullptr);
//}



/*
 * Name:        nodeDeleted()
 * Purpose:     Originally: send a signal to controllers that the node
 *		is deleted.  Now, obviously none.
 * Arguments:   none
 * Output:      none
 * Modifies:    nothing
 * Returns:     none
 * Assumptions: none
 * Bugs:        none
 * Notes:       none
 */

void
Node::nodeDeleted()
{

}



/*
 * Name:        paint()
 * Purpose:     Paints a node.
 * Arguments:   QPainter *, QStyleOptionGraphicsItem *, QWidget *
 * Output:      A node drawing
 * Modifies:    The node drawing.
 * Returns:     Nothing.
 * Assumptions: none
 * Bugs:        none
 * Notes:       Currently only draws nodes as circles.
 */

void
Node::paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
	    QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QColor brushColor;

    brushColor = nodeFill;
    painter->setBrush(brushColor);

    QPen pen;

    if (penStyle == 1)
        pen.setStyle(Qt::DotLine);
    else if (penStyle == 2)
        pen.setStyle(Qt::DashLine);
    else
        pen.setStyle(Qt::SolidLine);

    pen.setColor(nodeLine);
    painter->setPen(pen);

    painter->drawEllipse(-1 * nodeDiameter / 2,
                         -1 * nodeDiameter / 2,
                         nodeDiameter, nodeDiameter);

    htmlLabel->setPos(this->boundingRect().center().x()
		      - htmlLabel->boundingRect().width() / 2.,
		      this->boundingRect().center().y()
		      - htmlLabel->boundingRect().height() / 2.);
}



/*
 * Name:        itemChange()
 * Purpose:     Send a signal to the node's edges to re-adjust their
 *		geometries when a node is moved or rotated.
 * Arguments:   GraphicsItemChange, QVariant value
 * Output:      Nothing.
 * Modifies:    The node's edges' geometries and selection boxes (indirectly).
 * Returns:     A QVariant
 * Assumptions: ?
 * Bugs:        ?
 * Notes:       TODO: what is going on in the parent == graph block?
 *		If I don't execute that code, I get the whinage from
 *		the "else qDeb() << does not have parent" message below,
 *		but in quick tests nothing else seemed to be a problem.
 */

QVariant
Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
    qDeb() << "N::itemChange(" << change << ") called; "
	   << "node label is /" << label << "/";

    switch (change)
    {
      case ItemPositionHasChanged:
        if (parentItem() != 0)
        {
            if (parentItem()->type() == Graph::Type)
            {
                Graph * graph = qgraphicsitem_cast<Graph*>(parentItem());
                Graph * tempGraph = graph;
                graph = qgraphicsitem_cast<Graph*>(graph->getRootParent());
                this->setParentItem(nullptr);
                this->setParentItem(tempGraph);
            }
	    else
		qDeb() << "itemChange(): node does not have a "
		       << "graph item parent; Very Bad!";
        }
        foreach (Edge * edge, edgeList)
            edge->adjust();
        break;

      case ItemRotationChange:
        foreach (Edge * edge, edgeList)
            edge->adjust();
        break;

      default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}



/*
 * Name:        mousePressEvent()
 * Purpose:
 * Arguments:	The event.
 * Output:
 * Modifies:
 * Returns:
 * Assumptions:
 */

void
Node::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
    qDeb() << "N::mousePressEvent() setting 'select' to t";
    select = true;
    QGraphicsItem::mousePressEvent(event);
}



/*
 * Name:       mouseReleaseEvent()
 * Purpose:
 * Arguments:
 * Output:
 * Modifies:
 * Returns:
 * Assumptions:
 */

void
Node::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
    qDeb() << "N::mouseReleaseEvent() setting 'select' to F";

    select = false;
    QGraphicsItem::mouseReleaseEvent(event);
}



/*
 * Name:	setPreviewCoords()
 * Purpose:	Record the location of this node as it would have been
 *		originally placed in a 1" square.
 * Arguments:	The X and Y coords.
 * Outputs:	Nothing.
 * Modifies:	The node's preview X and Y data.
 * Returns:	Nothing.
 * Assumptions:	None.
 * Bugs:	?
 * Notes:	These values are used so that when a graph is styled
 *		multiple times the nodes can be scaled with respect to
 *		the original coordinates, not wrt previously scaled
 *		coords.  The latter does not faithfully scale things
 *		in all circumstances (e.g., a cycle which is scaled in
 *		X independently of Y, then later vice-versa).
 */

void
Node::setPreviewCoords(qreal x, qreal y)
{
    previewX = x;
    previewY = y;
}

qreal
Node::getPreviewX()
{
    return previewX;
}

qreal
Node::getPreviewY()
{
    return previewY;
}
