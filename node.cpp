/*
 * File:    node.cpp
 * Author:  Rachel Bood
 * Date:    2014/11/07
 * Version: 1.20
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
 *	to correct scaling issues. (Only reliable with Qt V5.14.2 or higher)
 *  (b) Removed unused physicalDotsPerInchY variable as only one DPI
 *	value is needed for the node's radius.
 * Jun 18, 2020 (IC V1.10)
 *  (a) Added setNodeLabel() and appropriate connect in the contructor to
 *      update label when changes are made on the canvas in edit mode.
 * Jun 26, 2020 (IC V1.11)
 *  (a) Comment out setFlag(QGraphicsItem::ItemClipsChildrenToShape); (Why?)
 * Jul 3, 2020 (IC V1.12)
 *  (a) Added setter and getter for node pen width and updated the painter
 *      to allow user to change thickness of a node.
 * Jul 22, 2020 (IC V1.13)
 *  (a) Initialize 'checked' in node constructor.
 * Jul 29, 2020 (IC V1.14)
 *  (a) Added eventFilter() to receive edit tab events so we can identify
 *      the node being edited/looked at.
 *  (b) Fixed findRootParent().
 * Aug 7, 2020 (IC V1.15)
 *  (a) Use settings custom resolution (if desired) for physicalDotsPerInchX.
 * Aug 12, 2020 (IC V1.16)
 *  (a) Updated the constructor to use global physicalDPI variable for node
 *      DPI.
 * Aug 19, 2020 (IC V1.17)
 *  (a) Removed the June 18th change and replaced the connection with one
 *      that updates the label when the user is done editting it from the
 *      canvas.
 * Aug 26, 2020 (IC V1.18)
 *  (a) Save the current penstyle when an associated edit tab widget sends
 *      a focusIn event and restore the penstyle during the focusOut event.
 * Oct 18, 2020 (JD V1.19)
 *  (a) Fix spelling; tweak comments.
 * Nov 11, 2020 (JD V1.20)
 *  (a) Removed rotation attribute, since a node is a qgraphicsitem
 *	which has a rotation (don't store the same info twice!).
 *	This alleviates some (former) confusion about what the
 *	rotation of a node is, which contributed to bugs related to
 *	the four-node join code in canvasscene.cpp.
 *  (b) Renamed tempPenStyle to savedPenStyle.
 *  (c) Removed mousePressEvent() and mouseReleaseEvent(), which are
 *	not needed.  All they did were set and clear "select", which
 *	is never accessed.  Removed select at the same time.
 *  (d) Cleaned up comments.
 *  (e) Removed the long-commented-out setNodeLabel(QString, qreal, QString)
 *      and nodeDeleted() functions.  (The latter had no code anyway.)
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
 * Purpose:     Constructor for Node class.
 * Arguments:   None.
 * Outputs:     Nothing.
 * Modifies:    Private node variables.
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None known.
 * Notes:       None.
 */

Node::Node()
{
    setFlag(ItemIsMovable);
    setFlag(ItemIsSelectable);
    setFlag(ItemSendsGeometryChanges);
    //setFlag(QGraphicsItem::ItemClipsChildrenToShape);
    setZValue(2);
    nodeID = -1;
    penStyle = 0;	// What type of pen style to use when drawing outline.
    penSize = 1;        // Size of node outline
    nodeDiameter = 1;
    htmlLabel = new HTML_Label(this);
    setHandlesChildEvents(true);
    physicalDotsPerInchX = currentPhysicalDPI_X;
    checked = 0;

    connect(htmlLabel, SIGNAL(editDone(QString)),
            this, SLOT(setNodeLabel(QString)));
}



/*
 * Name:        addEdge
 * Purpose:     Adds an Edge to the pointer QList of edges.
 * Arguments:   An Edge pointer.
 * Outputs:     Nothing.
 * Modifies:    The node's edgeList.
 * Returns:     Nothing.
 * Assumptions: edgeList is valid.
 * Bugs:        None...so far.
 * Notes:       None.
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
 * Outputs:     Nothing.
 * Modifies:    edgeList
 * Returns:     True if edge was removed, otherwise false.
 * Assumptions: edgeList is valid.
 * Bugs:        None.
 * Notes:       None.
 */

bool
Node::removeEdge(Edge * edge)
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
 * Outputs:     Nothing.
 * Modifies:    nodeDiameter
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
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
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     The node diameter, in inches.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       nodeDiamter is stored in pixels, it needs to be converted back
 *              to inches before it is returned.
 *              FUTURE WORK: create multiple getDiameter() functions to return
 *              different values....or create one function that will return
 *              the diameter in the desired unit of measurement.
 */

qreal
Node::getDiameter()
{
    return nodeDiameter / physicalDotsPerInchX;
}



/*
 * Name:        setRotation()
 * Purpose:     Sets the rotation of the node.
 * Arguments:	the new rotation amount.
 * Outputs:     Nothing.
 * Modifies:    The node's rotation.
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:	The node and edge labels need to be rotated in the
 *		opposite direction of the graph rotation in order to
 *		keep them oriented horizontally, right side up.
 */

void
Node::setRotation(qreal rotationAmount)
{
   QGraphicsItem::setRotation(rotationAmount);
}



/*
 * Name:        getRotation()
 * Purpose:     Getter for the node's rotation.
 * Arguments:   None.
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     The node's rotation.
 * Assumptions: None.
 * Bugs:	Hard to imagine.
 * Notes:	None.
 */

qreal
Node::getRotation()
{
    return QGraphicsItem::rotation();
}



/*
 * Name:        setFillColour()
 * Purpose:     Sets the fill colour of the node.
 * Arguments:   A QColor.
 * Outputs:     Nothing.
 * Modifies:    nodeFill
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
 */

void
Node::setFillColour(QColor fillColour)
{
    nodeFill = fillColour;
    update();
}



/*
 * Name:        getFillColour()
 * Purpose:     Returns the fill colour of the node.
 * Arguments:   None.
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     nodeFill.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
 */

QColor
Node::getFillColour()
{
    return nodeFill;
}



/*
 * Name:        setLineColour()
 * Purpose:     Sets the outline colour of the node.
 * Arguments:   A QColor.
 * Outputs:     Nothing.
 * Modifies:    nodeLine
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
 */

void
Node::setLineColour(QColor lineColour)
{
    nodeLine = lineColour;
    update();
}



/*
 * Name:        getLineColour()
 * Purpose:     Getter for the outline colour of the node.
 * Arguments:   None.
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     The node line colour.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
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
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     QGraphicsItem * root.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       Having removed the idea of graphs containing graphs
 *		(canvasscene.cpp V1.16), this is probably more complex
 *		than necessary.
 */

QGraphicsItem *
Node::findRootParent()
{
    QGraphicsItem * root = this;
    while (root->parentItem() != 0 && root->parentItem() != nullptr)
        root = root->parentItem();

    return root;
}



/*
 * Name:        setID()
 * Purpose:     Sets the ID (i.e., the internal node number) of the node.
 * Arguments:   An int, the ID.
 * Outputs:     Nothing.
 * Modifies:    The nodeID of this node.
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
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
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     The int nodeID
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
 */

int
Node::getID()
{
    return nodeID;
}



/*
 * Name:        edges()
 * Purpose:     Returns the list of edges incident with this node.
 * Arguments:   None.
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     QList<Edge *> edgeList
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
 */

QList<Edge *>
Node::edges() const
{
    return edgeList;
}



/*
 * Name:        setNodeLabel(int)
 * Purpose:     Sets the label of the node to an integer.
 * Arguments:   An int, the node label.
 * Outputs:     Nothing.
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
 * Outputs:     Nothing.
 * Modifies:    The text in the node label (both "htmlLabel" and "label" fields).
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
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
 * Outputs:     Nothing.
 * Modifies:    The text in the node label (both "text" and "label" fields).
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
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
 * Outputs:     Nothing.
 * Modifies:    The text in the node label (both "text" and "label" fields).
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
 */

void
Node::setNodeLabel(QString aLabel)
{
    label = aLabel;
    htmlLabel->texLabelText = aLabel;
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
 * Assumptions:	?
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
 * Name:        setNodeLabelSize()
 * Purpose:     Sets the font size of the node's label.
 * Arguments:   qreal
 * Outputs:     Nothing.
 * Modifies:    The node's text and fontsize.
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
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
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     The string contained in the label.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
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
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     The font size as a qreal.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       None.
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
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     QRectF
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       TODO: Q: Is adjust some empirical fudge factor???
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
 * Arguments:   The pen style.
 * Outputs:     Nothing.
 * Modifies:    penStyle: the integer used in the Node::paint() function.
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
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
 * Arguments:	A boolean specifying whether the label is editable.
 * Outputs:     Nothing.
 * Modifies:    ItemIsFocusable, ItemIsSelectable, setHandlesChildEvents flags
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       Is the setHandlesChildEvents flag actually modified??
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
 * Arguments:   None.
 * Outputs:     Nothing.
 * Modifies:    ?
 * Returns:     None.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       TODO: Why is this here but commented out?
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
 * Name:        setPenWidth()
 * Purpose:     Sets the width (penSize) of the node.
 * Arguments:   The new width.
 * Outputs:     Nothing.
 * Modifies:    The node's penSize.
 * Returns:     Nothing.
 * Assumptions: ?
 * Bugs:        ?
 * Notes:       The method is labeled setPenWidth and not setNodeWidth because
 *              penWidth is the naming convention used in Qt to draw a line.
 *              See paint() function for further details and implementation.
 */

void
Node::setPenWidth(qreal aPenWidth)
{
    penSize = aPenWidth;
    update();
}



/*
 * Name:        getPenWidth()
 * Purpose:     Returns the width (penSize) of the node.
 * Arguments:   None.
 * Outputs:     Nothing.
 * Modifies:    Nothing.
 * Returns:     A qreal, the penSize.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       The method is labeled getPenWidth and not getNodeWidth because
 *              penWidth is the naming convention used in Qt to draw a line.
 *              See paint() function for further details and implementation.
 */

qreal
Node::getPenWidth()
{
    return penSize;
}



/*
 * Name:        paint()
 * Purpose:     Paints a node.
 * Arguments:   QPainter *, QStyleOptionGraphicsItem *, QWidget *
 * Outputs:     A node drawing.
 * Modifies:    The scene.
 * Returns:     Nothing.
 * Assumptions: None.
 * Bugs:        None.
 * Notes:       Currently only draws nodes as circles.
 */

void
Node::paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
	    QWidget * widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QColor brushColour;

    brushColour = nodeFill;
    painter->setBrush(brushColour);

    QPen pen;

    if (penStyle == 1)
        pen.setStyle(Qt::DotLine);
    else if (penStyle == 2)
        pen.setStyle(Qt::DashLine);
    else
        pen.setStyle(Qt::SolidLine);

    pen.setColor(nodeLine);
    pen.setWidthF(penSize);
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
 * Outputs:     Nothing.
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
                this->setParentItem(nullptr);  // ???????????
                this->setParentItem(tempGraph);// Whats the point of this?
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
 * Name:        eventFilter()
 * Purpose:     Intercepts events related to edit tab widgets so
 *              we can identify the node being edited.
 * Arguments:	The object and the event.
 * Outputs:	Nothing.
 * Modifies:    The penstyle of the node outline.
 * Returns:	The result of QObject::eventFilter(obj, event).
 * Assumptions: The focusIn events pertain to edit tab widgets, not the
 *              node itself.
 * Bugs:	?
 * Notes:       Try using QEvent::HoverEnter and QEvent::HoverLeave?
 */

bool
Node::eventFilter(QObject * obj, QEvent * event)
{
    if (event->type() == QEvent::FocusIn)
    {
        savedPenStyle = penStyle;
        chosen(2);
    }
    else if (event->type() == QEvent::FocusOut)
        chosen(savedPenStyle);

    return QObject::eventFilter(obj, event);
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
