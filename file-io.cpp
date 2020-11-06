/*
 * File:	file-io.cpp
 * Author:	Jim Diamond
 * Date:	2020-10-22
 * Version:	1.1
 *
 * Purpose:	Implement the functions which read .grphc files and
 *		the functions which write files	graph files (text or
 *		images).
 *
 * Modification history:
 * Oct 22, 2020 (JD V1.0)
 *  (a) Initial revision.  Functions were extracted from mainwindow.cpp
 *      and then modified to allow the use of these functions from
 *	another (i.e., this) class.
 *  (b) Many names which had *both* '_' and camel-case were changed to
 *	just use camel case.
 *  (c) Broken TikZ code (setting width of node line) was fixed.
 *  (d) When a graph is read in, the rotation of the nodes (and edges) is
 *	set to the value of the graph rotation widget.  I therefore see no
 *	point in keeping the rotation of the node in the .grphc files,
 *	so that code is removed both from the writing and the reading.
 *	(It is left in inputCustomGraphOriginal() because those graphs
 *	will still have it.)  This makes any V 1 .grphc files written
 *	before today invalid, but I am OK with that, since all of them
 *	should be in my or IC's possession.
 *  (e) Write out label sizes, even if there is no label.
 *      Write the label surrounded by '<' and '>' to help avoid issues
 *	with white spaces or commas.
 *	(And, of course, expect this format when reading in .grphc files.)
 *  (f) Improve the pop-up error messages to users with invalid .grphc
 *	files.
 * Oct 29, 2020 (JD V1.1)
 *  (a) Do not clear the promptSave (i.e., the graph has not been
 *	saved) flag for output file types other than .grphc.
 */

#include <QDate>
#include <QDir>
#include <QFileDialog>
#include <QImageWriter>
#include <QMessageBox>
#include <QPainter>
#include <QtSvg/QSvgGenerator>

#include <unordered_map>

#include "basicgraphs.h"
#include "defuns.h"
#include "edge.h"
#include "graph.h"
#include "file-io.h"

#define TIKZ_SAVE_FILE		"TikZ (*.tikz)"
#define EDGES_SAVE_FILE		"Edge list (*.edges)"
#define SVG_SAVE_FILE		"SVG (*.svg)"

// The precision (number of digits after the decimal place) with which
// vertex positions and edge thicknesses, respectively, are written in
// TikZ output:
#define VP_PREC_TIKZ  4
#define VT_PREC_TIKZ  4
#define ET_PREC_TIKZ  4
// Similar for vertex precision in .grphc output:
#define VP_PREC_GRPHC  4

static QString fileDirectory;



/*
 * Name:	saveTikZ()
 * Purpose:	Save the current graph as a (LaTeX) TikZ file.
 * Arguments:	A file pointer to write to and the node list.
 * Outputs:	A TikZ picture (in LaTeX syntax) which draws the graph.
 * Modifies:	Nothing.
 * Returns:	True on success.
 * Assumptions: Args are valid.
 * Bugs:	This is horrendously, grotesquely long.
 * Notes:	Currently always returns T, but maybe in the future ...
 *		Idea: to minimize the amount of TikZ code, the most
 *		common vertex and edge attributes are found and stored
 *		in the styles v/.style, e/.style and l/.style.	Then
 *		when drawing a particular vertex or edge, anything not
 *		matching the default is output, over-riding the style.
 */

bool
File_IO::saveTikZ(QTextStream &outfile, QVector<Node *> nodes)
{
    qDebu("saveTikZ() called!");
    // TODO: only define a given colour once.
    // (Hash the known colours, and use the name if already defined?)

    nodeInfo nodeDefaults;
    edgeInfo edgeDefaults;

    // It seems (Qt5.15.1, anyway) that a QHash can't use a QColor as
    // a key.  Go figure.  So store the .name() of the colour.  Note
    // that .name() without a format specification doesn't get the
    // alpha channel, so if we ever allow alpha channels in node or
    // edge colours, this will need updating.
    QHash<QString, QString> unnamedColours;

    // Output the boilerplate TikZ picture code:
    outfile << "\\begin{tikzpicture}[x=1in, y=1in, xscale=1, yscale=1,\n";

    // Find and output the default node and edge details:
    findDefaults(nodes, &nodeDefaults, &edgeDefaults);

    // Define the default styles.
    // If the line or fill colour is a TikZ "known" colour,
    // use the name.  Otherwise \define a colour.
    // Q: why did Rachel output in RGB, as opposed to rgb?
    // Note: drawings may need to be tweaked by hand if they are to be
    //	     printed, due to the RGB/rgb <-> cmyk conversion nightmare.
    // Note: TikZ for plain TeX does not support the cmyk colourspace
    //	     nor (before JD's addition) the RGB colourspace.
    bool defineDefNodeFillColour;
    QColor defNodeFillColour
	= QColor(nodeDefaults.fillR, nodeDefaults.fillG, nodeDefaults.fillB);
    QString defNodeFillColourName = lookupColour(defNodeFillColour);
    if (defNodeFillColourName == nullptr)
    {
	defineDefNodeFillColour = true;
	outfile << "	n/.style={fill=defNodeFillColour, ";
    }
    else
    {
	defineDefNodeFillColour = false;
	outfile << "	n/.style={fill=" << defNodeFillColourName << ", ";
    }

    bool defineDefNodeLineColour;
    QColor defNodeLineColour
	= QColor(nodeDefaults.lineR, nodeDefaults.lineG, nodeDefaults.lineB);
    QString defNodeLineColourName = lookupColour(defNodeLineColour);
    if (defNodeLineColourName == nullptr)
    {
	defineDefNodeLineColour = true;
	outfile << "draw=defNodeLineColour, shape=circle,\n";
    }
    else
    {
	defineDefNodeLineColour = false;
	outfile << "draw=" << defNodeLineColourName << ", shape=circle,\n";
    }

    outfile << "\tminimum size=" << nodeDefaults.nodeDiameter << "in, "
	    << "inner sep=0, "
	    << "font=\\fontsize{" << nodeDefaults.labelSize
	    << "}{1}\\selectfont,\n"
	    << "\tline width="
	    << QString::number(nodeDefaults.penSize / currentPhysicalDPI_X,
			       'f', VT_PREC_TIKZ) << "in},\n";


    // e style gets 'draw=<colour>' and 'line width=<stroke width>' options
    // l style gets 'font=<spec>' options.  If we want to change the
    // label text colour to something else, use 'text=<colour>';
    // if we want to draw a box around the label text, use 'draw=<colour>'
    // (and "circle" to get a circle instead of a box).
    bool defineDefEdgeLineColour;
    QColor defEdgeLineColour
	= QColor(edgeDefaults.lineR, edgeDefaults.lineG, edgeDefaults.lineB);
    QString defEdgeLineColourName = lookupColour(defEdgeLineColour);
    if (defEdgeLineColourName == nullptr)
    {
	defineDefEdgeLineColour = true;
	outfile << "	e/.style={draw=defEdgeLineColour";
    }
    else
    {
	defineDefEdgeLineColour = false;
	outfile << "	e/.style={draw=" << defEdgeLineColourName;
    }

    outfile << ", line width="
	    << QString::number(edgeDefaults.penSize / currentPhysicalDPI_X,
			       'f', ET_PREC_TIKZ) << "in},\n"
	    << "    l/.style={font=\\fontsize{" << edgeDefaults.labelSize
	    << "}{1}\\selectfont}]\n";

    // We have now finished the generic style.
    // Output default colours, if needed.
    if (defineDefNodeFillColour)
    {
	outfile << "\\definecolor{defNodeFillColour} {RGB} {"
		<< QString::number(defNodeFillColour.red())
		<< "," << QString::number(defNodeFillColour.green())
		<< "," << QString::number(defNodeFillColour.blue())
		<< "}\n";
    }
    if (defineDefNodeLineColour)
    {
	outfile << "\\definecolor{defNodeLineColour} {RGB} {"
		<< QString::number(defNodeLineColour.red())
		<< "," << QString::number(defNodeLineColour.green())
		<< "," << QString::number(defNodeLineColour.blue())
		<< "}\n";
    }
    if (defineDefEdgeLineColour)
    {
	outfile << "\\definecolor{defEdgeLineColour} {RGB} {"
		<< QString::number(defEdgeLineColour.red())
		<< "," << QString::number(defEdgeLineColour.green())
		<< "," << QString::number(defEdgeLineColour.blue())
		<< "}\n";
    }

    QString edgeStyles = "";

    // Nodes: find center of graph, output graph centered on (0, 0)
    qreal minx = 0, maxx = 0, miny = 0, maxy = 0;
    if (nodes.count() > 0)
    {
	Node * node = nodes.at(0);
	minx = maxx = node->scenePos().rx();
	miny = maxy = node->scenePos().ry();
    }
    for (int i = 1; i < nodes.count(); i++)
    {
	Node * node = nodes.at(i);
	qreal x = node->scenePos().rx();
	qreal y = node->scenePos().ry();
	if (x > maxx)
	    maxx = x;
	else if (x < minx)
	    minx = x;
	if (y > maxy)
	    maxy = y;
	else if (y < minx)
	    miny = y;
    }
    qreal midx = (maxx + minx) / 2.;
    qreal midy = (maxy + miny) / 2.;

    // Sample output for a node:
    //	\definecolor{n<n>lineClr} {RGB} {R,G,B}	  (if not default)
    //	\definecolor{n<n>fillClr} {RGB} {R,G,B}	  (if not default)
    //	\node (v<n>) at (x,y) [n ,diffs from defaults] {$<node label>$};
    // Note that to change the text colour we could add (e.g.) "text=red"
    // to the \node options (or to the 'n' style above).
    // Note that TikZ is OK with a spurious ',' at the end of the options;
    // this fact is used to simplify the code below.
    for (int i = 0; i < nodes.count(); i++)
    {
	QString fillColour = "";
	QString lineColour = "";
	Node * node = nodes.at(i);
	bool doNewLine = false;

	if (node->getFillColour() != defNodeFillColour)
	{
	    // Is this colour known to TikZ?
	    fillColour = lookupColour(node->getFillColour());
	    QString qtname = node->getFillColour().name();
	    if (fillColour == nullptr)
	    {
		// Not known to TikZ... have we seen it yet?
		if (unnamedColours.contains(qtname))
		{
		    qDeb() << "\thot diggity... found " << node->getFillColour()
			   << " == " << qtname;
		    fillColour = unnamedColours.value(qtname);
		}
	    }
	    if (fillColour == nullptr)
	    {
		// We have not seen this colour before.
		// \definecolor it for TikZ *and* add it to the hash
		// of known colour names.
		qDeb() << "\tdid not find " << node->getFillColour()
		       << ";\n\t\tadding it to hash as " << qtname;
		fillColour = "n" + QString::number(i) + "fillClr";
		unnamedColours[qtname] = fillColour;
		outfile << "\\definecolor{" << fillColour << "} {RGB} {"
			<< QString::number(node->getFillColour().red())
			<< "," << QString::number(node->getFillColour().green())
			<< "," << QString::number(node->getFillColour().blue())
			<< "}\n";
	    }
	    // Wrap the fillColour with the TikZ syntax for later consumption:
	    fillColour = ", fill=" + fillColour;
	    doNewLine = true;
	}
	if (node->getLineColour() != defNodeLineColour)
	{
	    // Is this colour known to TikZ?
	    lineColour = lookupColour(node->getLineColour());
	    QString qtname = node->getLineColour().name();
	    if (lineColour == nullptr)
	    {
		// Not known to TikZ... have we seen it yet?
		if (unnamedColours.contains(qtname))
		{
		    qDeb() << "\thot diggity... found " << node->getLineColour()
			   << " == " << qtname;
		    lineColour = unnamedColours.value(qtname);
		}
	    }
	    if (lineColour == nullptr)
	    {
		qDeb() << "\tdid not find " << node->getLineColour()
		       << ";\n\t\tadding it to hash as " << qtname;
		lineColour = "n" + QString::number(i) + "lineClr";
		unnamedColours[qtname] = lineColour;
		outfile << "\\definecolor{" << lineColour << "}{RGB}{"
			<< QString::number(node->getLineColour().red())
			<< "," << QString::number(node->getLineColour().green())
			<< "," << QString::number(node->getLineColour().blue())
			<< "}\n";
	    }
	    lineColour = ", draw=" + lineColour;
	    doNewLine = true;
	}

	// Use (x,y) coordinate system for node positions.
	outfile << "\\node (v" << QString::number(i) << ") at ("
		<< QString::number((node->scenePos().rx() - midx)
				   / currentPhysicalDPI_X,
				   'f', VP_PREC_TIKZ)
		<< ","
		<< QString::number((node->scenePos().ry() - midy)
				   / -currentPhysicalDPI_Y,
				   'f', VP_PREC_TIKZ)
		<< ") [n";
	outfile << fillColour << lineColour;
	if (node->getDiameter() != nodeDefaults.nodeDiameter)
	{
	    outfile << ", minimum size=" << QString::number(node->getDiameter())
		    << "in";
	    doNewLine = true;
	}

	if (node->getPenWidth() != nodeDefaults.penSize)
	{
	    outfile << ", line width="
		    << QString::number(node->getPenWidth()
				       / currentPhysicalDPI_X,
				       'f', VT_PREC_TIKZ)
		    << "in";
	    doNewLine = true;
	}

	// Output the node label and its font size if and only if
	// there is a node label.
	if (node->getLabel().length() > 0)
	{
	    if (node->getLabelSize() != nodeDefaults.labelSize)
	    {
		if (doNewLine)
		    outfile << ",\n\t";
		else
		    outfile << ", ";
		outfile << "font=\\fontsize{"
			<< QString::number(node->getLabelSize()) // Font size
			<< "}{1}\\selectfont";
	    }

	    QString thisLabel = node->getLabel();
	    // TODO: this check just checks for a '^', but
	    // if a subscript itself has a superscript
	    // and there is no (top-level) superscript, we would
	    // fail to add the "^{}" text.
	    if (thisLabel.indexOf('^') != -1 || thisLabel.indexOf('_') == -1)
		outfile << "] {$" << thisLabel << "$};\n";
	    else
		outfile << "] {$" << thisLabel << "^{}$};\n";
	}
	else
	    outfile << "] {$$};\n";
    }

    // Sample output for an edge:
    //	\definecolor{e<n>_<m>lineClr} {RGB} {R,G,B}   (if not default)
    //	\path (v<n>) edge[e, diff from defaults] node[l, diff from defaults]
    //		{$<edge label>} (v_<m>);
    for (int i = 0; i < nodes.count(); i++)
    {
	bool wroteExtra = false;
	qDebu("\tNode %d has %d edges", i, nodes.at(i)->edgeList.count());
	for (int j = 0; j < nodes.at(i)->edgeList.count(); j++)
	{
	    // TODO: is it possible that with various and sundry
	    // operations on graphs neither the sourceID nor the
	    // destID of an edge in nodes.at(i)'s list is equal to
	    // i, and thus some edge won't be printed?	If so,
	    // should we just test "sourceID < destID in the if
	    // test immediately below?
	    Edge * edge = nodes.at(i)->edgeList.at(j);
	    int sourceID = edge->sourceNode()->getID();
	    int destID = edge->destNode()->getID();
	    if ((sourceID == i && destID > i)
		|| (destID == i && sourceID > i))
	    {
		qDebu("\ti %d j %d srcID %d dstID %d", i, j, sourceID, destID);
		qDebu("\tlabel = /%s/", edge->getLabel().toLatin1().data());
		QString lineColour = "";
		if (edge->getColour() != defEdgeLineColour)
		{
		    qDebu("E %d,%d: colour non-default", sourceID, destID);
		    lineColour = lookupColour(edge->getColour());
		    QString qtname = edge->getColour().name();
		    if (lineColour == nullptr)
		    {
			if (unnamedColours.contains(qtname))
			{
			    qDeb() << "\thot diggity... found "
				   << edge->getColour()
				   << " == " << qtname;
			    lineColour = unnamedColours.value(qtname);
			}
		    }
		    if (lineColour == nullptr)
		    {
			qDeb() << "\tdid not find " << edge->getColour()
			       << ";\n\t\tadding it to hash as " << qtname;
			lineColour = "e" + QString::number(sourceID) + "_"
			    + QString::number(destID) + "lineClr";
			unnamedColours[qtname] = lineColour;
			outfile << "\\definecolor{" << lineColour << "}{RGB}{"
				<< QString::number(edge->getColour().red())
				<< ","
				<< QString::number(edge->getColour().green())
				<< ","
				<< QString::number(edge->getColour().blue())
				<< "}\n";
		    }
		    lineColour = ", draw=" + lineColour;
		    wroteExtra = true;
		    qDebu("\tSETTING lineColour = /%s/",
			  lineColour.toLatin1().data());
		}

		outfile << "\\path (v"
			<< QString::number(sourceID)
			<< ") edge[e" << lineColour;
		if (edge->getPenWidth() != edgeDefaults.penSize)
		{
		    outfile << ", line width="
			    << QString::number(edge->getPenWidth()
					       / currentPhysicalDPI_X,
					       'f', ET_PREC_TIKZ)
			    << "in";
		    wroteExtra = true;
		}

		// Output a \n iff we have both a non-default line
		// width and a non-default label size.
		if (edge->getLabel().length() > 0
		    && edge->getLabelSize() != edgeDefaults.labelSize
		    && wroteExtra)
		    outfile << "]\n\tnode[l";
		else
		    outfile << "] node[l";

		// Output edge label size (and the "select font" info)
		// if and only if the edge has a label.
		if (edge->getLabel().length() > 0)
		{
		    if (edge->getLabelSize() != edgeDefaults.labelSize)
		    {
			outfile << ", font=\\fontsize{"
				<< QString::number(edge->getLabelSize())
				<< "}{1}\\selectfont";
		    }
		    outfile << "] {$" << edge->getLabel() << "$}";
		}
		else
		    outfile << "] {$$}";

		// Finally, output the other end of the edge:
		outfile << " (v"
			<< QString::number(destID)
			<< ");\n";
	    }
	}
    }

    outfile << "\\end{tikzpicture}\n";

    return true;
}



/*
 * Name:	saveGraphIc()
 * Purpose:	Output the description of the graph in "graph-ic" format.
 * Arguments:	A file pointer to write to and the node list.
 * Outputs:	The graph-ic description of the current graph.
 * Modifies:	Nothing.
 * Returns:	True on success.
 * Assumptions: Args are valid; node IDs are meaningful.
 * Bugs:
 * Notes:	Currently always returns T, but maybe in the future ...
 *		Normally vertex and edge label info is not output if
 *		the label is empty, but if outputExtra = T these
 *		are output (this is a debugging aid), as well as some
 *		extra info.
 */

bool
File_IO::saveGraphIc(QTextStream &outfile, QVector<Node *> nodes,
		     bool outputExtra)
{
    qDeb() << "FI::saveGraphIc() called";
    // Use some painful Qt constructs to output the node and edge
    // information with a more readable format.
    // Note that tests with explicitly setting the format to 'g'
    // and the precision to 6 indicate that Qt5.9.8 (on S64-14.2)
    // will do whatever it damn well pleases, vis-a-vis the number
    // of chars printed.
    outfile << "# Version 1 graph-ic graph definition created ";
    QDateTime dateTime = dateTime.currentDateTime();
    outfile << dateTime.toString("yyyy-MM-dd hh:mm:ss") << "\n";
    outfile << "# Do NOT edit or delete the above line!" << "\n\n";

    outfile << "# The number of nodes in this graph:\n";
    outfile << nodes.count() << "\n\n";

    QString nodeInfo = QString::number(nodes.count()) + "\n\n";

    outfile << "# The node descriptions; the format is:\n";
    outfile << "# x,y, diameter, pen_width, fill r,g,b,\n";
    outfile << "#      outline r,g,b, label_font_size, <label>\n";

    // In some cases I have created a graph where all the
    // coordinates are negative and "large", and the graph is not
    // visible in the preview pane when I load it, which also
    // means (as of time of writing) that I can't drag it to the
    // canvas.	Thus the graph is effectively lost.  Avoid this by
    // centering the graph on (0, 0) when writing it out.
    qreal minx = 0, maxx = 0, miny = 0, maxy = 0;
    if (nodes.count() > 0)
    {
	Node * node = nodes.at(0);
	minx = maxx = node->scenePos().rx();
	miny = maxy = node->scenePos().ry();
    }
    for (int i = 1; i < nodes.count(); i++)
    {
	Node * node = nodes.at(i);
	qreal x = node->scenePos().rx();
	qreal y = node->scenePos().ry();
	if (x > maxx)
	    maxx = x;
	else if (x < minx)
	    minx = x;
	if (y > maxy)
	    maxy = y;
	else if (y < minx)
	    miny = y;
    }

    qreal midxInch = (maxx + minx) / (currentPhysicalDPI_X * 2.);
    qreal midyInch = (maxy + miny) / (currentPhysicalDPI_Y * 2.);
    for (int i = 0; i < nodes.count(); i++)
    {
	// TODO: s/,/\\/ before writing out label.  Undo this when reading.
	Node * node = nodes.at(i);
	outfile << "# Node " + QString::number(i) + ":\n";
	outfile << QString::number(node->scenePos().rx() / currentPhysicalDPI_X
				   - midxInch,
				   'f', VP_PREC_GRPHC) << ","
		<< QString::number(node->scenePos().ry() / currentPhysicalDPI_Y
				   - midyInch,
				   'f', VP_PREC_GRPHC) << ", "
		<< QString::number(node->getDiameter()) << ", "
		<< QString::number(node->getPenWidth()) << ", "
		<< QString::number(node->getFillColour().redF()) << ","
		<< QString::number(node->getFillColour().greenF()) << ","
		<< QString::number(node->getFillColour().blueF()) << ", "
		<< QString::number(node->getLineColour().redF()) << ","
		<< QString::number(node->getLineColour().greenF()) << ","
		<< QString::number(node->getLineColour().blueF()) << ", "
		<< QString::number(node->getLabelSize()) << ", <"
		<< node->getLabel() << ">\n";
    }

    outfile << "\n# The edge descriptions; the format is:\n"
	    << "# u, v, dest_radius, source_radius, pen_width,\n"
	    << "#	line r,g,b, label_font_size, <label>\n";

    for (int n = 0; n < nodes.count(); n++)
    {
	for (int e = 0; e < nodes.at(n)->edgeList.count(); e++)
	{
	    Edge * edge = nodes.at(n)->edgeList.at(e);
	    if (outputExtra)
	    {
		outfile << "# Looking at n, e = "
			<< QString::number(n) << ", " << QString::number(e)
			<< "  ->  src, dst = "
			<< QString::number(edge->sourceNode()->getID())
			<< ", "
			<< QString::number(edge->destNode()->getID())
			<< "\n";
	    }

	    int printThisOne = 0;
	    int sourceID = edge->sourceNode()->getID();
	    int destID = edge->destNode()->getID();
	    if (sourceID == n && destID > n)
	    {
		printThisOne++;
		outfile << QString("%1").arg(sourceID, 2, 10, QChar(' '))
			<<  ","
			<< QString("%1").arg(destID, 2, 10, QChar(' '));
	    }
	    else if (destID == n && sourceID > n)
	    {
		printThisOne++;
		outfile << QString("%1").arg(destID, 2, 10, QChar(' '))
			<<  ","
			<< QString("%1").arg(sourceID, 2, 10, QChar(' '));
	    }
	    if (printThisOne)
	    {
		outfile << ", " << QString::number(edge->getDestRadius())
			<< ", " << QString::number(edge->getSourceRadius())
			<< ", " << QString::number(edge->getPenWidth()) << ", "
			<< QString::number(edge->getColour().redF()) << ","
			<< QString::number(edge->getColour().greenF()) << ","
			<< QString::number(edge->getColour().blueF()) << ", "
			<< edge->getLabelSize() << ", <"
			<< edge->getLabel() << ">\n";
	    }
	}
    }

    return true;
}



/*
 * Name:	saveGraph()
 * Purpose:	Saves an image/tikz/grphc/edgelist version of the canvas.
 * Arguments:	None.
 * Output:	A file corresponding to the graph.
 * Modifies:	Nothing.
 * Returns:	True on file successfully saved, false otherwise.
 * Assumptions: ?
 * Bugs:	This function is too long.
 * Notes:	None.
 */

bool
File_IO::saveGraph(bool * promptSave, QWidget * parent, Ui::MainWindow * ui)
{
    QString fileTypes = "";

    fileTypes += GRAPHiCS_SAVE_FILE ";;"
	TIKZ_SAVE_FILE ";;"
	EDGES_SAVE_FILE ";;";

    foreach (QByteArray format, QImageWriter::supportedImageFormats())
    {
	// Remove offensive and redundant file types.
	// Even with these, there still may a confusing number of choices.
	if (QString(format).toUpper() == "BMP")	    // Winblows bitmap
	    continue;
	if (QString(format).toUpper() == "CUR")	    // Winblows cursor
	    continue;
	if (QString(format).toUpper() == "DDS")	    // Winblows directdraw sfc
	    continue;
	if (QString(format).toUpper() == "ICO")	    // Winblows icon
	    continue;
	if (QString(format).toUpper() == "ICNS")    // Apple icon
	    continue;
	if (QString(format).toUpper() == "PBM")	    // Portable bitmap
	    continue;
	if (QString(format).toUpper() == "PGM")	    // Portable gray map
	    continue;
	if (QString(format).toUpper() == "PPM")	    // Portable pixmap
	    continue;
	if (QString(format).toUpper() == "XBM")	    // X bitmap
	    continue;
	if (QString(format).toUpper() == "XBM")	    // X bitmap
	    continue;
	if (QString(format).toUpper() == "XPM")	    // X pixmap
	    continue;
	if (QString(format).toUpper() == "WBMP")    // wireless bitmap
	    continue;
	if (QString(format).toUpper() == "TIFF")    // Just list "tif"
	    continue;
	if (QString(format).toUpper() == "JPEG")    // Just list "jpg"
	    continue;
	fileTypes += QObject::tr("%1 (*.%2);;").arg(QString(format).toUpper(),
						    QString(format).toLower());
    }

    fileTypes += SVG_SAVE_FILE ";;"
	"All Files (*)";

    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(parent,
						    QObject::tr("Save graph"),
						    fileDirectory, fileTypes,
						    &selectedFilter);
    if (fileName.isNull())
	return false;

#ifdef __linux__
    // Stupid, stupid Qt file browser works differently on different OSes.
    // Append the extension if there isn't already one ...
    QFileInfo fi(fileName);
    QString ext = fi.suffix();
    if (ext.isNull())
    {
	int start = selectedFilter.indexOf("*") + 1;
	int end = selectedFilter.indexOf(")");

	if (start < 0 || end < 0)
	{
	    qDebug() << "?? FI::saveGraph() could not find extension in "
		     << selectedFilter;
	    return false;
	}

	QString extension = selectedFilter.mid(start, end - start);
	qDeb() << "saveGraph(): computed extension is" << extension;
	fileName += extension;
	qDeb() << "saveGraph(): computed filename is" << fileName;
    }
#endif

    // Handle all image (i.e., non-text) outputs here;
    // check for all known text-file types.
    // TODO: should we use QFileInfo(fileName).extension().lower(); ?

    // Turn off the grid (if necessary) before exporting an image!
    bool saveS2GStatus = ui->snapToGrid_checkBox->isChecked();
    if (saveS2GStatus)
	ui->canvas->snapToGrid(false);

    if (selectedFilter != GRAPHiCS_SAVE_FILE
	&& selectedFilter != TIKZ_SAVE_FILE
	&& selectedFilter != EDGES_SAVE_FILE
	&& selectedFilter != SVG_SAVE_FILE)
    {
	ui->canvas->scene()->clearSelection();
	ui->canvas->scene()->invalidate(ui->canvas->scene()->itemsBoundingRect(),
					ui->canvas->scene()->BackgroundLayer);

	QPixmap * image = new QPixmap(ui->canvas->scene()
				      ->itemsBoundingRect().size().toSize());
	if (selectedFilter == "JPG (*.jpg)")
	{
	    if (settings.contains("jpgBgColour"))
	    {
		QColor colour = settings.value("jpgBgColour").toString();
		image->fill(colour);
	    }
	    else
		image->fill(Qt::white);
	}
	else
	{
	    if (settings.contains("otherImageBgColour"))
	    {
		QColor colour = settings.value("otherImageBgColour").toString();
		image->fill(colour);
	    }
	    else
		image->fill(Qt::transparent);
	}
	QPainter painter(image);
	painter.setRenderHints(QPainter::Antialiasing
			       | QPainter::TextAntialiasing
			       | QPainter::HighQualityAntialiasing
			       | QPainter::NonCosmeticDefaultPen, true);
	ui->canvas->scene()->setBackgroundBrush(Qt::transparent);
	ui->canvas->
	    scene()->render(&painter,
			    QRectF(0, 0,
				   ui->canvas->scene()
				   ->itemsBoundingRect().width(),
				   ui->canvas->scene()
				   ->itemsBoundingRect().height()),
			    ui->canvas->scene()->itemsBoundingRect(),
			    Qt::IgnoreAspectRatio);
	image->save(fileName); // Requires file extension or it won't save :-/

	ui->canvas->snapToGrid(saveS2GStatus);
	ui->canvas->update();
	return true;
    }

    // Common code for text files:
    int numOfNodes = 0;
    QVector<Node *> nodes;
    QString edges = "";

    QFile outputFile(fileName);
    outputFile.open(QIODevice::WriteOnly);
    if (!outputFile.isOpen())
    {
	QMessageBox::information(0, "Error",
				 "Unable to open " + fileName + " for output!");
	return false;
    }

    // The output routines just take a list of nodes.  Create the list
    // and give each node a meaningful ID.
    foreach (QGraphicsItem * item, ui->canvas->scene()->items())
    {
	if (item->type() == Node::Type)
	{
	    Node * node = qgraphicsitem_cast<Node *>(item);
	    node->setID(numOfNodes++);
	    nodes.append(node);
	}
    }

    QTextStream outStream(&outputFile);

    if (selectedFilter == GRAPHiCS_SAVE_FILE)
    {
	bool success = saveGraphIc(outStream, nodes, false);
	outputFile.close();
	ui->canvas->snapToGrid(saveS2GStatus);
	ui->canvas->update();
	QFileInfo fi(fileName);
	ui->graphType_ComboBox->insertItem(ui->graphType_ComboBox->count(),
					   fi.baseName());
	*promptSave = false;
	return success;
    }

    if (selectedFilter == EDGES_SAVE_FILE)
    {
	bool success = saveEdgelist(outStream, nodes);
	outputFile.close();
	ui->canvas->snapToGrid(saveS2GStatus);
	ui->canvas->update();
	return success;
    }

    if (selectedFilter == TIKZ_SAVE_FILE)
    {
	bool success = saveTikZ(outStream, nodes);
	outputFile.close();
	ui->canvas->snapToGrid(saveS2GStatus);
	ui->canvas->update();
	return success;
    }

    if (selectedFilter == SVG_SAVE_FILE)
    {
	QSvgGenerator svgGen;

	svgGen.setFileName(fileName);
	svgGen.setSize(ui->canvas->scene()
		       ->itemsBoundingRect().size().toSize());
	QPainter painter(&svgGen);
	ui->canvas->scene()->render(&painter,
				    QRectF(0, 0, ui->canvas->scene()
					   ->itemsBoundingRect().width(),
					   ui->canvas->scene()
					   ->itemsBoundingRect().height()),
				    ui->canvas->scene()->itemsBoundingRect(),
				    Qt::IgnoreAspectRatio);
	ui->canvas->snapToGrid(saveS2GStatus);
	ui->canvas->update();
	return true;
    }

    // ? Should not get here!
    qDebug() << "saveGraph(): Unexpected output filter in "
	     << "fileio::saveGraph()!";
    return false;
}



/*
 * Name:	loadGraphicFile()
 * Purpose:	Ask the user for a file to load.  If we get a name,
 *		call a function to read in the alleged graph.
 * Arguments:	The parent widget and the ui.
 * Outputs:	Displays a file chooser.
 * Modifies:	Nothing.
 * Returns:	True.  (TODO: Why?)
 * Assumptions: None.
 * Bugs:
 * Notes:
 */

bool
File_IO::loadGraphicFile(QWidget * parent, Ui::MainWindow * ui)
{
    qDeb() << "FI:loadGraphicFile() called; fileDirectory = '"
	   << fileDirectory << "'; GRAPHiCS_SAVE_FILE is '"
	   << GRAPHiCS_SAVE_FILE << "'";

    QString fileName = QFileDialog::getOpenFileName(parent,
						    "Load Graph-ics File",
						    fileDirectory,
						    GRAPHiCS_SAVE_FILE);
    if (! fileName.isNull())
	File_IO::inputCustomGraph(false, fileName, ui);

    return true;
}



/*
 * Name:	loadGraphicLibrary()
 * Purpose:	Append all of the graph-ic files in the library
 *		directory to the graphType menu.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	ui->graphType_ComboBox
 * Returns:	Nothing.
 * Assumptions: fileDirectory has been initialized.
 *		This assumes that if a file has a GRAPHiCS_FILE_EXTENSION
 *		extension then it is a graph-ic file.
 * Bugs:
 * Notes:
 */

void
File_IO::loadGraphicLibrary(Ui::MainWindow * ui)
{
    QDirIterator dirIt(fileDirectory, QDirIterator::Subdirectories);
    while (dirIt.hasNext())
    {
	dirIt.next();
#ifdef DEBUG2
	if (QFileInfo(dirIt.filePath()).isFile())
	    qDeb() << "FI::loadGraphicLibrary(): suffix of"
		   << QFileInfo(dirIt.filePath()).fileName()
		   << "is"
		   << QFileInfo(dirIt.filePath()).suffix();
#endif

	if (QFileInfo(dirIt.filePath()).suffix() == GRAPHiCS_FILE_EXTENSION
	    && QFileInfo(dirIt.filePath()).isFile())
	{
	    QFileInfo fileInfo(dirIt.filePath());
	    ui->graphType_ComboBox->addItem(fileInfo.baseName());
	}
    }
}



/*
 * Name:	    inputCustomGraph()
 * Purpose:	    Read in a graph-ic file and display it in the preview.
 * Argument:	    The name of the file to read from and the ui.
 * Outputs:	    Nothing.
 * Modifies:	    Upon successfully reading in a graph-ic file, this
 *		    function clears the preview scene and then adds the
 *		    created graph to the preview.
 * Returns:	    Nothing.
 * Assumptions:	    The input file is readable and not deviously invalid.
 * Bugs:	    May crash and burn on invalid input.
 * Notes:	    Arguably this function should be in preview.cpp.
 */

void
File_IO::inputCustomGraph(bool prependDirPath, QString graphName,
			  Ui::MainWindow * ui)
{
    int lineNum = 0;

    if (graphName.isNull())
    {
	qDebug() << "FI::inputCustomGraph(): graphName is NULL!! ??";
	return;
    }

    if (prependDirPath)
	graphName = fileDirectory + "/" + graphName;

    qDeb() << "FI::inputCustomGraph(): graphName is\n\t" << graphName;

    QFile file(graphName);

    if (!file.open(QIODevice::ReadOnly))
    {
	QMessageBox::information(nullptr,
				 "Error",
				 "File: " + graphName
				 + ": " + file.errorString());
	// Reset the combo box to the "Select Graph Type" item (#0).
	ui->graphType_ComboBox->setCurrentIndex(BasicGraphs::Nothing);
	file.close();
	return;
    }

    QTextStream in(&file);

    if (in.atEnd())
    {
	QMessageBox::information(nullptr,
				 "Error",
				 "File: " + graphName
				 + ": no data in file");
	// Reset the combo box to the "Select Graph Type" item (#0).
	ui->graphType_ComboBox->setCurrentIndex(BasicGraphs::Nothing);
	file.close();
	return;
    }

    // Check the first line to see if there is a version number, and
    // then call the appropriate function to read data in.
    QString line = in.readLine().simplified();
    lineNum++;
    QStringList toks = line.split(" ");
    if (toks.count() < 3 || toks.at(1) != "Version")
    {
	// Either the "original" non-versioned file format, or a bogus file.
	// In either case, let the "read original format" function
	// deal with it.
	// I wanted to hand "in" to this function, but since it would not
	// let me do that, hand the filename.
	file.close();
	inputCustomGraphOriginal(graphName, ui);
	return;
    }

    // TODO: split this off into another function.

    int i = 0;
    QVector<Node *> nodes;
    int numOfNodes = -1;		// < 0 ==> haven't read numOfNodes yet
    Graph * graph = new Graph();
    // The following 4 variables hold the extremal positions actually drawn,
    // so they take into account both the node center location and the
    // node diameter.  (These are the two values stored in the .grphc file.)
    qreal minX = 1E10, maxX = -1E10, minY = 1E10, maxY = -1E10;
    // These 4 variables hold the radii of the vertices which give the
    // extremal positions stored above.
    qreal minXr = 0, maxXr = 0, minYr = 0, maxYr = 0;
    qreal radius_total = 0;

    while (!in.atEnd())
    {
	QString line = in.readLine();
	qDeb() << "  just read line /" << line << "/";
	lineNum++;
	QString simpLine = line.simplified();
	if (simpLine.isEmpty())
	{
	    // Allow visually blank lines
	    continue;
	}

	if (simpLine.at(0).toLatin1() == '#')
	{
	    // Allow comments where first non-white is '#'.
	    // TODO: Should we save these comments somewhere?
	    continue;
	}

	if (numOfNodes < 0)
	{
	    bool ok;
	    numOfNodes = line.toInt(&ok);
	    // TODO: do we want to allow 0-node graphs?
	    // Theoretically yes, but practically, no.
	    if (! ok || numOfNodes < 0)
	    {
		QMessageBox::information(0, "Error",
					 "The file " + graphName
					 + " has an invalid number of "
					 "nodes.  Thus I can not read "
					 "this file.");
		qDeb() << "  numOfNodes = " << numOfNodes;
		file.close();
		return;
	    }
	    continue;
	}
	
	if (i < numOfNodes)
	{
	    i++;
	    QStringList fields = line.split(",");

	    // Nodes may or may not have label info.  Accept both.
	    // Nominally, we want 11 or 13 (and this assumes we don't
	    // want to record the label size if there is no label,
	    // which is possibly not what we will eventually realize
	    // we want).  But to avoid complex quoting of commas in
	    // labels, we just glue all the fields past #12 into the
	    // label.
	    if (fields.count() < 12)
	    {
		QMessageBox::information(0, "Error",
					 "Node " + QString::number(i - 1)
					 + " on line "
					 + QString::number(lineNum)
					 + " of file "
					 + graphName
					 + " has too few fields.  Thus I "
					 "can not read this file.");
		// TODO: do I need to free any storage?
		file.close();
		return;
	    }

	    Node * node = new Node();
	    node->setID(i);
	    qreal x = fields.at(0).toDouble();
	    qreal y = fields.at(1).toDouble();
	    qreal d = fields.at(2).toDouble();
	    qreal r = d / 2.;
	    radius_total += r;
	    node->setPos(x * currentPhysicalDPI_X, y * currentPhysicalDPI_Y);
	    node->setDiameter(d);
	    node->setPenWidth(fields.at(3).toDouble());
	    // Record information about the extremal nodes for use below.
	    if (x - r < minX)
	    {
		minX = x - r;
		minXr = r;
	    }
	    if (x + r > maxX)
	    {
		maxX = x + r;
		maxXr = r;
	    }
	    if (y - r < minY)
	    {
		minY = y - r;
		minYr = r;
	    }
	    if (y + r > maxY)
	    {
		maxY = y + r;
		maxYr = r;
	    }
	    qDebu("  node id %d at (%.4f, %.4f)\n\tX [%.4f, %.4f], "
		  "Y [%.4f, %.4f]", i - 1, x, y, minX, maxX, minY, maxY);

	    QColor fillColour;
	    fillColour.setRedF(fields.at(4).toDouble());
	    fillColour.setGreenF(fields.at(5).toDouble());
	    fillColour.setBlueF(fields.at(6).toDouble());
	    node->setFillColour(fillColour);

	    QColor lineColour;
	    lineColour.setRedF(fields.at(7).toDouble());
	    lineColour.setGreenF(fields.at(8).toDouble());
	    lineColour.setBlueF(fields.at(9).toDouble());
	    node->setLineColour(lineColour);

	    node->setNodeLabelSize(fields.at(10).toFloat());
	    int labelPrefixLoc = line.indexOf(", <");
	    // The test < 0 is much weaker than possible, but if someone
	    // really wants to screw up their .grphc file, who am I to
	    // stand in their way?
	    if (labelPrefixLoc < 0 || ! line.endsWith(">"))
	    {
		QMessageBox::information(0, "Error",
					 "Node " + QString::number(i - 1)
					 + " on line "
					 + QString::number(lineNum)
					 + " of file "
					 + graphName
					 + " has an invalid label.  Thus I "
					 "can not read this file.");
		// TODO: do I need to free any storage?
		file.close();
		return;
	    }
	    
	    QString l = line.mid(labelPrefixLoc + 3,
				 line.length() - (labelPrefixLoc + 3) - 1);
	    
	    qDeb() << "    subs line, " << labelPrefixLoc + 3
		   << ", " << line.length() - (labelPrefixLoc + 3) - 1
		   << ") = |" << l << "|";
	    node->setNodeLabel(l);

	    nodes.append(node);
	    node->setParentItem(graph);
	}
	else	// Default case: looking at an edge
	{
	    i++;
	    QStringList fields = line.split(",");

	    // Edges may or may not have label info.  Accept both.
	    if (fields.count() < 8 || fields.count() == 9)
	    {
		QMessageBox::information(0, "Error",
					 "Edge "
					 + QString::number(i - numOfNodes)
					 + " on line "
					 + QString::number(lineNum)
					 + " of file "
					 + graphName
					 + " has an invalid number of "
					 "fields.  Thus I can not read "
					 "this file.");
		// TODO: do I need to free any storage?
		file.close();
		return;
	    }
	    int from = fields.at(0).toInt();
	    int to = fields.at(1).toInt();
	    Edge * edge = new Edge(nodes.at(from), nodes.at(to));
	    edge->setDestRadius(fields.at(2).toDouble());
	    edge->setSourceRadius(fields.at(3).toDouble());
	    edge->setPenWidth(fields.at(4).toDouble());
	    QColor lineColour;
	    lineColour.setRedF(fields.at(5).toDouble());
	    lineColour.setGreenF(fields.at(6).toDouble());
	    lineColour.setBlueF(fields.at(7).toDouble());
	    edge->setColour(lineColour);

	    edge->setEdgeLabelSize(fields.at(8).toFloat());
	    int labelPrefixLoc = line.indexOf(", <");
	    if (labelPrefixLoc < 0 || ! line.endsWith(">"))
	    {
		QMessageBox::information(0, "Error",
					 "Edge (" + QString::number(from)
					 + ", "  + QString::number(to)
					 + ") on line "
					 + QString::number(lineNum)
					 + " of file "
					 + graphName
					 + " has an invalid label.  Thus I "
					 "can not read this file.");
		// TODO: do I need to free any storage?
		file.close();
		return;
	    }

	    QString l = line.mid(labelPrefixLoc + 3,
				 line.length() - (labelPrefixLoc + 3) - 1);
	    qDeb() << "    subs line, " << labelPrefixLoc + 3
		   << ", " << line.length() - (labelPrefixLoc + 3) - 1
		   << ") = |" << l << "|";
	    edge->setEdgeLabel(l);

	    edge->setParentItem(graph);
	}
    }
    file.close();

    // Scale all the node CENTER positions to a 1"x1" square
    // so that it can be appropriately styled.
    // TODO(?): center it on (0,0).  (Graphs output by this
    // program should already be centered.)
    qreal width = (maxX - maxXr) - (minX + minXr);
    qreal height = (maxY - maxYr) - (minY + minYr);
    qDebu("    X: [%.4f, %.4f], Xr min %.4f, max %.4f, r avg %.4f",
	  minX, maxX, minXr, maxXr, radius_total / numOfNodes);
    qDebu("    Y: [%.4f, %.4f], Yr min %.4f, max %.4f",
	  minY, maxY, minYr, maxYr);
    qDebu("    width %.4f, height %.4f", width, height);
    qDeb() << "    minX = " << minX << ", maxX = "
	   << maxX << "\n\tminY = " << minY << ", maxY = " << maxY
	   << "; width = " << width << " and height = " << height;
    for (int i = 0; i < nodes.count(); i++)
    {
	Node * n = nodes.at(i);
	n->setPreviewCoords(width == 0. ? 0.
			    : n->x() / width / currentPhysicalDPI_X,
			    height == 0. ? 0.
			    : n->y() / height / currentPhysicalDPI_Y);
	qDebu("    nodes[%s] coords: screen (%.4f, %.4f); "
	      "preview set to (%.4f, %.4f)", n->getLabel().toLatin1().data(),
	      n->x(), n->y(), n->getPreviewX(), n->getPreviewY());
    }

    // Set the w & h widgets to the actual values for this graph, to make
    // the UI behave predictably.
    // Note that if these signals are not (temporarily) turned off,
    // generateGraph() will be called multiple times.  Duh.
    ui->graphWidth->blockSignals(true);
    ui->graphWidth->setValue(width + 2 * radius_total / numOfNodes);
    ui->graphWidth->blockSignals(false);
    ui->graphHeight->blockSignals(true);
    ui->graphHeight->setValue(height + 2 * radius_total / numOfNodes);
    ui->graphHeight->blockSignals(false);

    qDeb() << "FI::inputCustomGraph: graph->childItems().length() ="
	   << graph->childItems().length();

    // Apparently we have to center the graph in the viewport.
    // (Presumably this is because the node positions are relative to
    // their parent, the graph?)
    qDeb() << "    graph current position is " << graph->x() << ", "
	   << graph->y();
    // I'd like to use something like
    // graph->setPos(mapToScene(viewport()->rect().center()));
    // but I get
    // "viewport() is unknown in this context".  For now, kludge the
    // centering of the graph as follows.  Those are the numbers from
    // PV::Create_Graph (every time), presumably they come from the
    // fact that PV::PV sets the scene rectangle to (0, 0, 100, 30).
    // But 100 and 30 are this->width and this->height, and it is not
    // clear to me how those numbers get set.
    graph->setPos(49, 15);
    qDeb() << "    graph CENTERED position is " << graph->x() << ", "
	   << graph->y();
    graph->setRotation(-1 * ui->graphRotation->value(), false);

    ui->preview->scene()->clear();
    ui->preview->scene()->addItem(graph);
}



/*
 * Name:	saveEdgelist()
 * Purpose:	Save the current graph as an edgelist.
 * Arguments:	A file pointer to write to and the node list.
 * Outputs:	An edge list of the graph to the file.
 * Modifies:	Nothing.
 * Returns:	True on success.
 * Assumptions: Args are valid.
 * Bugs:	?!
 * Notes:	Currently always returns T, but maybe in the future ...
 */

bool
File_IO::saveEdgelist(QTextStream &outfile, QVector<Node *> nodes)
{
    QString edges = "";

    for (int i = 0; i < nodes.count(); i++)
    {
	for (int j = 0; j < nodes.at(i)->edgeList.count(); j++)
	{
	    Edge * edge = nodes.at(i)->edgeList.at(j);

	    if (edge->sourceNode()->getID() == i
		&& edge->destNode()->getID() > i)
	    {
		edges += QString::number(edge->sourceNode()->getID()) + ","
		    + QString::number(edge->destNode()->getID()) + "\n";
	    }
	    else if (edge->destNode()->getID() == i
		     && edge->sourceNode()->getID() > i)
	    {
		edges += QString::number(edge->destNode()->getID()) + ","
		    + QString::number(edge->sourceNode()->getID())
		    + "\n";
	    }
	}
    }
    outfile << nodes.count() << "\n";
    outfile << edges;

    return true;
}



/*
 * Name:	lookupColour()
 * Purpose:	Given an RGB colour, see if this is a colour known to
 *		TikZ by a human-friendly name; if so, return the name.
 * Arguments:	A QColor.
 * Outputs:	Nothing.
 * Modifies:	Nothing.
 * Returns:	A TikZ colour name (as a QString) or nullptr.
 * Assumptions: None.
 * Bugs:	This is shamefully unsophisticated.
 *		The colours known to TikZ may be a moving target.
 * Notes:	At time of writing (Oct 2019), the following are
 *		(allegedly) the known colours (in RGB):
 *		red, green, blue, cyan, magenta, yellow, black,
 *		gray (128,128, 128), darkgray (64,64,64),
 *		lightgray (191,191,191), brown (191,128,64), lime (191,255,0),
 *		olive (127,127,0), orange (255,128,0), pink (255,191,191),
 *		purple (191,0,64), teal (0,128,128), violet (128,0,128)
 *		and white (modulo the fact that 127~=128, 63~=64, and so on).
 *		To get the RGB values from a PDF file with cmyk colours, I used
 *		    gs -dUseFastColor file.pdf
 *		which does a direct mapping of cmyk to RGB without
 *		using any ICC colour profiles, and then used xmag.
 *		Not knowing what numbers to turn to what names, I will
 *		only map the subset of the above names found in
 *		.../texmf-dist/tex/generic/pgf/utilities/pgfutil-plain.def,
 *		as well as lightgray and darkgray.
 *		Note that some of these are quite different than X11's rgb.txt.
 */

// Allow a bit of slop in some cases (see noted examples below).
#define	    CLOSE(x, c)	    (((x) == (c)) || ((x) == ((c) + 1)))

QString
File_IO::lookupColour(QColor colour)
{
    int r = colour.red();
    int g = colour.green();
    int b = colour.blue();

    if (r == 0)
    {
	if (g == 0 && b == 0)
	    return "black";
	if (g == 255 && b == 0)
	    return "green";
	if (g == 0 && b == 255)
	    return "blue";
	if (g == 255 && b == 255)
	    return "cyan";
	if (CLOSE(g, 127) && CLOSE(b, 127))
	    return "teal";
	return nullptr;
    }

    if (CLOSE(r, 63) && CLOSE(g, 63) && CLOSE(b, 63))
	return "darkgray";

    if (CLOSE(r, 127))			    // 0.5 -> 127.5
    {
	if (CLOSE(g, 127) && CLOSE(b, 127))
	    return "gray";
	if (CLOSE(g, 127) && b == 0)
	    return "olive";
	if (g == 0 && CLOSE(b, 127))
	    return "violet";
	return nullptr;
    }

    if (CLOSE(r, 191))			    // 0.75 -> 191.25
    {
	if (g == 0 && CLOSE(b, 63))	    // 0.25 -> 63.75
	    return "purple";
	if (CLOSE(g, 127) && CLOSE(b, 63))
	    return "brown";
	if (g == 255 && b == 0)
	    return "lime";
	if (CLOSE(g, 191) && CLOSE(b, 191))
	    return "lightgray";
	return nullptr;
    }

    if (r == 255)
    {
	if (g == 255 && b == 255)
	    return "white";
	if (g == 0 && b == 0)
	    return "red";
	if (g == 0 && b == 255)
	    return "magenta";
	if (g == 255 && b == 0)
	    return "yellow";
	if (CLOSE(g, 127) && b == 0)
	    return "orange";
	if (CLOSE(g, 191) && CLOSE(b, 191))
	    return "pink";
	return nullptr;
    }
    return nullptr;
}



/*
 * Name:	findDefaults()
 * Purpose:	Find the most common line colours, fill colours, pen widths,
 *		and so on, of the set of nodes and edges in the graph.
 * Arguments:	The list of nodes and int *'s to hold the R, G and B values.
 * Outputs:	Nothing.
 * Modifies:	Nothing.
 * Returns:	The two sets of R, G and B.
 * Assumptions: None.
 * Bugs:	?
 * Notes:	Returns (0,0,0) in the case there are no edges or vertices.
 */

void
File_IO::findDefaults(QVector<Node *> nodes,
		      nodeInfo * nodeDefaults_p, edgeInfo * edgeDefaults_p)
{
    // Set the default defaults (sic).
    // TODO: These values should really be #defines somewhere.
    *nodeDefaults_p = {255, 255, 255, 0, 0, 0, (qreal)0.2, (qreal)1., 12};
    *edgeDefaults_p = {0, 0, 0, (qreal)1., (qreal)12.};

    if (nodes.count() == 0)
	return;

    int max_count, result, colour, R, G, B;
    qreal fresult;
    std::unordered_map<int, int> vFillColour;
    std::unordered_map<int, int> vLineColour;
    std::unordered_map<qreal, int> vNodeDiam;
    std::unordered_map<qreal, int> vPenSize;
    std::unordered_map<qreal, int> vLabelSize;
    std::unordered_map<int, int> eLineColour;
    std::unordered_map<qreal, int> ePenSize;
    std::unordered_map<qreal, int> eLabelSize;

    // Populate all the node hashes.
    for (int i = 0; i < nodes.count(); i++)
    {
	Node * node = nodes.at(i);
	R = node->getFillColour().red();
	G = node->getFillColour().green();
	B = node->getFillColour().blue();
	colour = R << 16 | G << 8 | B;
	vFillColour[colour]++;

	R = node->getLineColour().red();
	G = node->getLineColour().green();
	B = node->getLineColour().blue();
	colour = R << 16 | G << 8 | B;
	vLineColour[colour]++;

	vNodeDiam[node->getDiameter()]++;
	vPenSize[node->getPenWidth()]++;
	vLabelSize[node->getLabelSize()]++;
    }

    max_count = 0;
    result = nodeDefaults_p->fillR << 16
	| nodeDefaults_p->fillG << 8
	| nodeDefaults_p->fillB;
    for (auto item : vFillColour)
    {
	if (max_count < item.second)
	{
	    result = item.first;
	    max_count = item.second;
	}
    }
    nodeDefaults_p->fillR = result >> 16;
    nodeDefaults_p->fillG = (result >> 8) & 0xFF;
    nodeDefaults_p->fillB = result & 0xFF;

    max_count = 0;
    result = nodeDefaults_p->lineR << 16 | nodeDefaults_p->lineG << 8
	| nodeDefaults_p->lineB;
    for (auto item : vLineColour)
    {
	if (max_count < item.second)
	{
	    result = item.first;
	    max_count = item.second;
	}
    }
    nodeDefaults_p->lineR = result >> 16;
    nodeDefaults_p->lineG = (result >> 8) & 0xFF;
    nodeDefaults_p->lineB = result & 0xFF;

    max_count = 0;
    fresult = nodeDefaults_p->nodeDiameter;
    for (auto item : vNodeDiam)
    {
	if (max_count < item.second)
	{
	    fresult = item.first;
	    max_count = item.second;
	}
    }
    nodeDefaults_p->nodeDiameter = fresult;
    qDebu("nodeDiam: %.4f count = %d", fresult, max_count);

    max_count = 0;
    fresult = nodeDefaults_p->penSize;
    for (auto item : vPenSize)
    {
	if (max_count < item.second)
	{
	    fresult = item.first;
	    max_count = item.second;
	}
    }
    nodeDefaults_p->penSize = fresult;
    qDebu("nodePenSize: %.4f count = %d", fresult, max_count);

    max_count = 0;
    fresult = nodeDefaults_p->labelSize;
    for (auto item : vLabelSize)
    {
	if (max_count < item.second)
	{
	    fresult = item.first;
	    max_count = item.second;
	}
    }
    nodeDefaults_p->labelSize = fresult;
    qDebu("nodeLabelSize: %.4f count = %d", fresult, max_count);

    for (int i = 0; i < nodes.count(); i++)
    {
	for (int j = 0; j < nodes.at(i)->edgeList.count(); j++)
	{
	    // TODO: see TODO in Edge section of saveTikZ().
	    Edge * edge = nodes.at(i)->edgeList.at(j);
	    int sourceID = edge->sourceNode()->getID();
	    int destID = edge->destNode()->getID();
	    if ((sourceID == i && destID > i)
		|| (destID == i && sourceID > i))
	    {
		R = edge->getColour().red();
		G = edge->getColour().green();
		B = edge->getColour().blue();
		colour = R << 16 | G << 8 | B;
		eLineColour[colour]++;
		// Don't count 0's, they are likely bogus.
		if (edge->getPenWidth() > 0)
		    ePenSize[edge->getPenWidth()]++;
		// Only count the label sizes for edges which have a label.
		if (edge->getLabel().length() > 0)
		{
		    qDebu("i=%d, j=%d, label=/%s/, size=%.1f",
			  i, j, edge->getLabel().toLatin1().data(),
			  edge->getLabelSize());
		    if (edge->getLabelSize() >= 1)
			eLabelSize[edge->getLabelSize()]++;
		}
	    }
	}
    }

    max_count = 0;
    result = edgeDefaults_p->lineR << 16 | edgeDefaults_p->lineG << 8
	| edgeDefaults_p->lineB;
    for (auto item : eLineColour)
    {
	if (max_count < item.second)
	{
	    result = item.first;
	    max_count = item.second;
	}
    }
    edgeDefaults_p->lineR = result >> 16;
    edgeDefaults_p->lineG = (result >> 8) & 0xFF;
    edgeDefaults_p->lineB = result & 0xFF;
    qDebu("edgeColour: (%d,%d,%d) count = %d", edgeDefaults_p->lineR,
	  edgeDefaults_p->lineG, edgeDefaults_p->lineB, max_count);

    max_count = 0;
    fresult = edgeDefaults_p->penSize;
    for (auto item : ePenSize)
    {
	if (max_count < item.second)
	{
	    fresult = item.first;
	    max_count = item.second;
	}
    }
    edgeDefaults_p->penSize = fresult;
    qDebu("edgePenSize: %.4f count = %d", fresult, max_count);

    max_count = 0;
    fresult = edgeDefaults_p->labelSize;
    for (auto item : eLabelSize)
    {
	if (max_count < item.second)
	{
	    fresult = item.first;
	    max_count = item.second;
	}
    }
    edgeDefaults_p->labelSize = fresult;
    qDebu("edgeLabelSize: %.4f count = %d", fresult, max_count);
}



/*
 * Name:	setFileDirectory()
 * Purpose:	Set the fileDirectory variable.
 * Arguments:	Parent widget.
 * Outputs:	Nothing.
 * Modifies:	fileDirectory.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	If a constructor is ever created for this class,
 *		it probably makes sense to call it from there.
 *		On the other hand, if we make GRAPHiCS_SAVE_SUBDIR
 *		a settable option, we might want to change this during
 *		a run of the program.
 */

void
File_IO::setFileDirectory(QWidget * parent)
{
    fileDirectory = QDir::currentPath().append("/" GRAPHiCS_SAVE_SUBDIR);
    QDir dir(fileDirectory);

    if (!dir.exists())
	if (!dir.mkdir(fileDirectory))
	{
	    QMessageBox::information(parent, "Error",
				     "Unable to create the subdirectory ./"
				     GRAPHiCS_SAVE_SUBDIR
				     " (where the graphs you create are "
				     "stored); I will boldly carry on anyway.  "
				     "Perhaps you can fix that problem from "
				     "a terminal or file manager before you "
				     "try to save a graph.");
	}
}


/**************************************************************************
 * Everything below this line is for handling old versions of the .grphc
 * file format.
 *************************************************************************/

/*
 * Name:	inputCustomGraphOriginal()
 * Purpose:	Read in the "original" format of a .grphc file.
 * Arguments:	The file to read from.
 * Outputs:	Nothing.
 * Modifies:	The preview.
 * Returns:	Nothing.
 * Assumptions:	The file exists, is readable, and is non-empty.
 * Bugs:	This is not only too long, but it is so similar to
 *		inputCustomGraph() that maybe the two of them should be
 *		refactored differently than this.
 * Notes:	JD added "comment lines" capability Oct 2019.
 */

void
File_IO::inputCustomGraphOriginal(QString graphFileName, Ui::MainWindow * ui)
{
    qDeb() << "FI::inputCustomGraphOriginal(" << graphFileName
	   << ") called";

    QFile file(graphFileName);
    if (!file.open(QIODevice::ReadOnly))
    {
	QMessageBox::information(nullptr,
				 "Error",
				 "File: " + graphFileName
				 + ": " + file.errorString());
	// Reset the combo box to the "Select Graph Type" item (#0).
	ui->graphType_ComboBox->setCurrentIndex(BasicGraphs::Nothing);
	file.close();
	return;
    }

    QTextStream in(&file);
    int i = 0;
    QVector<Node *> nodes;
    int numOfNodes = -1;		// < 0 ==> haven't read numOfNodes yet
    Graph * graph = new Graph();
    // The following 4 variables hold the extremal positions actually drawn,
    // so they take into account both the node center location and the
    // node diameter.  (These are the two values stored in the .grphc file.)
    qreal minX = 1E10, maxX = -1E10, minY = 1E10, maxY = -1E10;
    // These 4 variables hold the radii of the vertices which give the 
    // extremal positions stored above.
    qreal minXr = 0, maxXr = 0, minYr = 0, maxYr = 0;
    qreal radius_total = 0;

    while (!in.atEnd())
    {
	QString line = in.readLine();
	QString simpLine = line.simplified();
	if (simpLine.isEmpty())
	{
	    // Allow visually blank lines
	}
	else if (simpLine.at(0).toLatin1() == '#')
	{
	    // Allow comments where first non-white is '#'.
	    // TODO: Should we save these comments somewhere?
	}
	else if (numOfNodes < 0)
	{
	    bool ok;
	    numOfNodes = line.toInt(&ok);
	    qDeb() << "   numOfNodes = " << numOfNodes;
	    // TODO: do we want to allow 0-node graphs?
	    // Theoretically yes, but practically, no.
	    if (! ok || numOfNodes < 0)
	    {
		QMessageBox::information(0, "Error",
					 "The file " + graphFileName
					 + " has an invalid number of "
					 "nodes.  Thus I can not read "
					 "this file.");
		file.close();
		return;
	    }
	}
	else if (i < numOfNodes)
	{
	    QStringList fields = line.split(",");

	    // Nodes may or may not have label info.  Accept both.
	    // Nominally, we want 10 or 12 (and this assumes we don't
	    // want to record the label size if there is no label,
	    // which is possibly not what we will eventually realize
	    // we want).  But to avoid complex quoting of commas in
	    // labels, we just glue all the fields past #11 into the
	    // label.
	    if (fields.count() < 10 || fields.count() == 11)
	    {
		QMessageBox::information(0, "Error",
					 "Node " + QString::number(i)
					 + " of file "
					 + graphFileName
					 + " has an invalid number of "
					 "fields.  Thus I can not read "
					 "this file.");
		// TODO: do I need to free any storage?
		file.close();
		return;
	    }

	    Node * node = new Node();
	    qreal x = fields.at(0).toDouble();
	    qreal y = fields.at(1).toDouble();
	    qreal d = fields.at(2).toDouble();
	    qreal r = d / 2.;
	    radius_total += r;
	    node->setPos(x * currentPhysicalDPI_X, y * currentPhysicalDPI_Y);
	    node->setDiameter(d);
	    node->setRotation(fields.at(3).toDouble());
	    node->setID(i++);
	    // Record information about the extremal nodes for use below.
	    if (x - r < minX)
	    {
		minX = x - r;
		minXr = r;
	    }
	    if (x + r > maxX)
	    {
		maxX = x + r;
		maxXr = r;
	    }
	    if (y - r < minY)
	    {
		minY = y - r;
		minYr = r;
	    }
	    if (y + r > maxY)
	    {
		maxY = y + r;
		maxYr = r;
	    }
	    qDebu("  node id %d at (%.4f, %.4f)\n\tX [%.4f, %.4f], "
		  "Y [%.4f, %.4f]", i - 1, x, y, minX, maxX, minY, maxY);

	    QColor fillColour;
	    fillColour.setRedF(fields.at(4).toDouble());
	    fillColour.setGreenF(fields.at(5).toDouble());
	    fillColour.setBlueF(fields.at(6).toDouble());
	    node->setFillColour(fillColour);

	    QColor lineColour;
	    lineColour.setRedF(fields.at(7).toDouble());
	    lineColour.setGreenF(fields.at(8).toDouble());
	    lineColour.setBlueF(fields.at(9).toDouble());
	    node->setLineColour(lineColour);
	    if (fields.count() >= 12)
	    {
		// If the label has one or more commas, we must glue
		// the fields back together.
		node->setNodeLabelSize(fields.at(10).toFloat());
		QString l = fields.at(11);
		for (int i = 12; i < fields.count(); i++)
		    l += "," + fields.at(i);
		node->setNodeLabel(l);
	    }
	    else
	    {
		// Avoid bizarre behaviour when this is not initialized.
		// 12 pt seems like a nice default.
		node->setNodeLabelSize(12);
	    }
	    nodes.append(node);
	    node->setParentItem(graph);
	}
	else	// Default case: looking at an edge
	{
	    QStringList fields = line.split(",");

	    // Edges may or may not have label info.  Accept both.
	    if (fields.count() < 9 || fields.count() == 10)
	    {
		QMessageBox::information(0, "Error",
					 "Edge "
					 + QString::number(i - numOfNodes)
					 + " of file "
					 + graphFileName
					 + " has an invalid number of "
					 "fields.  Thus I can not read "
					 "this file.");
		// TODO: do I need to free any storage?
		file.close();
		return;
	    }
	    Edge * edge = new Edge(nodes.at(fields.at(0).toInt()),
				   nodes.at(fields.at(1).toInt()));
	    edge->setDestRadius(fields.at(2).toDouble());
	    edge->setSourceRadius(fields.at(3).toDouble());
	    edge->setRotation(fields.at(4).toDouble());
	    edge->setPenWidth(fields.at(5).toDouble());
	    QColor lineColour;
	    lineColour.setRedF(fields.at(6).toDouble());
	    lineColour.setGreenF(fields.at(7).toDouble());
	    lineColour.setBlueF(fields.at(8).toDouble());
	    edge->setColour(lineColour);
	    if (fields.count() >= 11)
	    {
		edge->setEdgeLabelSize(fields.at(9).toFloat());
		// If the label has one or more commas, we must glue
		// the fields back together.
		QString l = fields.at(10);
		for (int i = 11; i < fields.count(); i++)
		    l += "," + fields.at(i);
		edge->setEdgeLabel(l);
	    }
	    else
	    {
		// Avoid bizarre behaviour when this is not initialized.
		// 12 pt seems like a nice default.
		edge->setEdgeLabelSize(12);
	    }
	    edge->setParentItem(graph);
	}
    }
    file.close();

    // Scale all the node CENTER positions to a 1"x1" square
    // so that it can be appropriately styled.
    // TODO(?): center it on (0,0).  (Graphs output by this
    // program should already be centered.)
    qreal width = (maxX - maxXr) - (minX + minXr);
    qreal height = (maxY - maxYr) - (minY + minYr);
    qDebu("    X: [%.4f, %.4f], Xr min %.4f, max %.4f, r avg %.4f",
	  minX, maxX, minXr, maxXr, radius_total / numOfNodes);
    qDebu("    Y: [%.4f, %.4f], Yr min %.4f, max %.4f",
	  minY, maxY, minYr, maxYr);
    qDebu("    width %.4f, height %.4f", width, height);
    qDeb() << "    minX = " << minX << ", maxX = "
	   << maxX << "\n\tminY = " << minY << ", maxY = " << maxY
	   << "; width = " << width << " and height = " << height;
    for (int i = 0; i < nodes.count(); i++)
    {
	Node * n = nodes.at(i);
	n->setPreviewCoords(width == 0. ? 0.
			    : n->x() / width / currentPhysicalDPI_X,
			    height == 0. ? 0.
			    : n->y() / height / currentPhysicalDPI_Y);
	qDebu("    nodes[%s] coords: screen (%.4f, %.4f); "
	      "preview set to (%.4f, %.4f)", n->getLabel().toLatin1().data(),
	      n->x(), n->y(), n->getPreviewX(), n->getPreviewY());
    }

    // Set the w & h widgets to the actual values for this graph, to make
    // the UI behave predictably.
    // Note that if these signals are not (temporarily) turned off,
    // generateGraph() will be called multiple times.  Duh.
    // See similar code+comment in MW:on_graphType_ComboBox_currentIndexChanged
    ui->graphWidth->blockSignals(true);
    ui->graphWidth->setValue(width + 2 * radius_total / numOfNodes);
    ui->graphWidth->blockSignals(false);
    ui->graphHeight->blockSignals(true);
    ui->graphHeight->setValue(height + 2 * radius_total / numOfNodes);
    ui->graphHeight->blockSignals(false);

    qDeb() << "FI::inputCustomGraphOriginal: graph->childItems().length() = "
	   << graph->childItems().length();

    // Apparently we have to center the graph in the viewport.
    // (Presumably this is because the node positions are relative to
    // their parent, the graph?)
    qDeb() << "    graph current position is " << graph->x() << ", "
	   << graph->y();
    // I'd like to use something like
    // graph->setPos(mapToScene(viewport()->rect().center()));
    // but I get
    // "viewport() is unknown in this context".  For now, kludge the
    // centering of the graph as follows.  Those are the numbers from
    // PV::Create_Graph (every time), presumably they come from the
    // fact that PV::PV sets the scene rectangle to (0, 0, 100, 30).
    // But 100 and 30 are this->width and this->height, and it is not
    // clear to me how those numbers get set.

    graph->setPos(49, 15);
    qDeb() << "    graph CENTERED position is " << graph->x() << ", "
	   << graph->y();
    graph->setRotation(-1 * ui->graphRotation->value(), false);

    ui->preview->scene()->clear();
    ui->preview->scene()->addItem(graph);
}
