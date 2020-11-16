/*
 * File:	mainwindow.cpp
 * Author:	Rachel Bood
 * Date:	January 25, 2015.
 * Version:	1.68
 *
 * Purpose:	Implement the main window and functions called from there.
 *
 * Modification history:
 * Jan 25, 2016 (JD V1.2):
 *  (a) Don't include TIFF and JPEG as file types to make the list shorter;
 *	assume (!) that the list also includes TIF and JPG.
 *  (b) TikZ: only output edge label font size if there is an edge label.
 *  (c) TikZ: only output edge thickness to ET_PREC_TIKZ digits.
 *  (d) TikZ: Only output vertex positions to VP_PREC_TIKZ digits.
 *  (e) TikZ: Merge the two (currently identical) code branches for drawing
 *	edges.
 *  (f) Factored out the "default" features in
 *	on_graphType_ComboBox_currentIndexChanged()) while fixing a
 *	bug where one or two weren't set when they should be.
 *  (g) Changed the minimum number of nodes to 1 for known graph types;
 *	tweaked some other similar parameters as well.
 *	For graphs read in from .grphc files, only display the UI control
 *	that (currently) do anything.
 *  (h) Changed some of the widget labels for the different types of
 *	graphs in on_graphType_ComboBox_currentIndexChanged().
 *  (i) Re-ordered file type drop-down list so to put the "text" outputs
 *	at the top.
 * Feb 3, 2016 (JD V1.3)
 *  (a) Removed "GraphSettings.h" since it is not used.
 *  (b) Minor formatting cleanups.
 *  (c) Changed "Grapha" to "Graphic".
 * Oct 11, 2019 (JD V1.4)
 *  (a) Allow blank lines and comments ([ \t]*#.*) in .grphc files.
 *  (b) Add some comments.  Reformat a bit.  Improve debug stmts.
 *  (c) Output a more informative and human-readable .grhpc file.
 *	Output the information as we compute it, rather than slowly
 *	constructing two big honkin' strings piece by piece and
 *	then outputting them.
 *  (d) Check for some error conditions and pop up a QErrorMessage in
 *	such cases.
 * Oct 13, 2019 (JD V1.5)
 *  (a) Before this version, node labels and corresponding font sizes
 *	were not stored in the .grphc file.  Go figure.
 *  (b) Look up TikZ-known colours (for TikZ output) and use those
 *	names rather than defining new colours for every coloured item.
 *  (c) When saving the graph, only do Linux-specific filename checks
 *	*after* confirming the selected filename is non-null.
 *  (d) Added some error checking when selecting files as possible
 *	graph-ic files, as well as checking contents of a graph-ic file.
 *  (e) Since node.{h,cpp} have been updated to allow superscripts as
 *	well as subscripts, and this allows the "edit" function to change
 *	vertex labels with subs/supers, the TikZ export has been changed
 *	to add '^{}' iff there is no '^' in the label.	This adds a
 *	spurious (but apparently harmless) empty superscript to labels
 *	which have neither sub nor super.
 *  (f) Added a number of function comments.
 * Oct 16, 2019 (JD V1.6)
 *  (a) When creating .grphc file, center graph on (0,0) so that when
 *	it is read back in it is centered (and thus accessible) in the
 *	preview pane.  Ditto
 *  (b) Output coords (to .grphc file) using a %.<VP_PREC_GRPHC>f
 *	format, rather than the default %6g format, to avoid numbers
 *	like -5.68434e-14 (DAMHIKT) in the output file.
 * Oct 16, 2019 (JD V1.7)
 *  (a) The original code was outputting (TikZ case) too many edge
 *	colour definitions, and in some cases they could be wrong.
 *	Fix that, and also add in the "well known colour" stuff from
 *	the nodes, as described in V1.5(b) above.
 *  (b) Only output the edge label and size if there is one.
 *  (c) Read edges from .grphc file with and without label info.
 * Nov 10, 2019 (JD V1.8)
 *  (a) Output the label font size before label so that a space before the
 *	label doesn't need to be trimmed.
 * Nov 10, 2019 (JD V1.9)
 *  (a) Fix apparently-erroneous use of logicalDotsPerInchX in the Y
 *	part of Create_Graph() (called in generate_Graph()).
 *  (b) Add some comments and some qDebug() statements.
 *  (c) Rename getWeightLabelSize() -> getLabelSize().
 *  (d) Rename "Weight" to "Label" for edge function names.
 *  (e) Do not call style_Graph() in generate_Graph() for "library"
 *	graphs, since doing so removes colour and size info.  This is
 *	only a partial fix to the problem, since this mod takes away
 *	the ability to style a library graph.  style_Graph() needs to
 *	be entirely rethought.
 *  (f) Add the rest of the "named" colours to lookupColour().
 *  (g) Remove a bunch of uninteresting output file types from the
 *	save dialog menu.
 * Nov 16, 2019 (JD V1.10)
 *  (a) Refactor save_graph() because it is unwieldy large
 *	into itself + saveEdgelist() (step 1 of many!).
 *  (b) Formatting tweaks.
 * Nov 17, 2019 (JD V1.11)
 *  (a) Move lookupColour() above where it is used and make it a
 *	non-class function.
 * Nov 18, 2019 (JD V1.12)
 *  (a) Fixed the edit window so that it looks nicer and behaves
 *	(vis-a-vis vertical spacing) much nicer (changes in
 *	on_tabWidget_currentChanged()).
 *	Modify code which outputs "Edit Graph" tab to make the Label
 *	box wider, since that is the one which needs the space (at
 *	least when using sub- or superscripts).
 *	Also put stretch at the bottom so that the layout manager
 *	doesn't distribute extra space in ugly places.
 *  (b) Add connections between a node (or edge) and its label in the
 *	edit tab, so that when a node (or edge) is deleted (in delete
 *	mode) the label in its row in the Edit Graph tab is also
 *	deleted.  (Everything else used to go, may as well do that
 *	too.)
 *  (c) Change "weight" to "label" in .grphc output edge comment.
 *  (d) Comment & formatting tweaking.
 * Nov 19, 2019 (JD V1.13)
 *  (a) Factor out saveTikZ() from save_Graph().
 *  (b) Add findDefaults() as a first step to improving the TikZ
 *	output (which will set default styles at the top of each graph
 *	and only output differences for nodes/edges that are different).
 * Nov 24, 2019 (JD V1.14)
 *  (a) Add a call on_graphType_ComboBox_currentIndexChanged(-1) the
 *	mainwindow constructor to initialize the "Create Graph" pane
 *	to a self-consistent shape.  Modify on_graphType_...() to not
 *	whine when its arg is < 0.
 * Nov 28, 2019 (JD V1.15)
 *  (a) Add dumpTikZ() and dumpGraphIc() functions and shortcuts (^T
 *	and ^G) to call them.  dumpGraphIc() not yet implemented.
 *  (b) findDefaults(): changed default node diameter from 0.2 to
 *	(qreal)0.2 to correctly match the data type.  Changed the
 *	floats to qreals in the hashes for the same reason.
 *	Also only change struct values from the initial defaults if
 *	there were actually observations in the corresponding hash table.
 *  (c) Improve saveTikZ() so that "nicer" TikZ code is output.
 *	Specifically, output default styles for nodes, edge lines and
 *	edge labels and when outputting individual nodes and edges
 *	only specify differences from the defaults.
 *	Also output things as we go, rather than creating a big
 *	honkin' string and outputting it at the end (can you say O(n^2)?).
 * Nov 28, 2019 (JD V1.16)
 *  (a) Factor out saveGraphIc() from save_Graph().
 *	Use this to implement dumpGraphIc().
 * Nov 29, 2019 (JD V1.17)
 *  (a) Rename "none" mode to "drag" mode, for less confusion.
 * Dec 1, 2019 (JD V1.18)
 *  (a) Add qDeb() / DEBUG stuff.
 *  (b) Print out more screen resolution & size info.
 *  (c) Add comments, minor code clean-ups.
 * Dec 6, 2019 (JD V1.19)
 *  (a) Rename generate_Freestyle_{Nodes,Edges} to {node,edge}ParamsUpdated
 *	to better reflect what those functions do.
 *  (b) Modify generate_Graph(), and the related connect() calls, to
 *	hand a parameter to generate_Graph() so that it knows which
 *	widget's change caused it to be called.	 This is the first
 *	step of fixing things so that specific features of library
 *	graphs can be styled without applying all styles, which
 *	otherwise destroys much of the content of a library graph.
 *  (c) Bug fix: nodeParamsUpdated() now passes the NodeLabelSize to
 *	ui->canvas->setUpNodeParams() where it should, not the nodeSize.
 *  (d) Clean up debug outputs a bit.  Improve some comments.
 * Dec 9, 2019 (JD V1.20)
 *  (a) Various comment and debug changes.
 *  (b) Change generate_Graph() to not pass height and width into to
 *	PreView::Create_Graph().  This is part of a large set of
 *	changes which will eventually allow library graphs to be styled.
 *  (c) Update call sequence to PV::Style_graph() as per changes there.
 *  (d) Obsessively re-order the cases in
 *	on_graphType_ComboBox_currentIndexChanged().
 *	Also set more constraints on the spinboxes so that we can't
 *	call generate_...() functions with meaningless parameters.
 *	Show both width and height for Dutch Windmill, in case someone
 *	wants one with non-unit aspect ratio.
 *  (e) Add on_numOfNodes1_valueChanged() to follow the actions of
 *	on_numOfNodes2_valueChanged() to avoid inconsistent parameters
 *	when numOfNodes1 is changed.
 * Dec 12, 2019 (JD V1.21)
 *  (a) Save the screen DPI in a couple of static global vars & get it
 *	over with, rather than repeatedly calling the Qt functions.
 *  (b) Modify saveGraphIc() so that the coords in the .grphc file are
 *	in inches, not in pixels.  (It should have been this way from
 *	day 1!)
 *  (c) Allow commas in labels.
 *  (d) Close the file descriptor upon discovering an error when
 *	reading a .grphc file.
 *  (e) (Re-)Implement the ability to style library graphs (with
 *	changes to preview.cpp).  As of this version, the graph
 *	drawing widgets visible when a library graph is chosen do not
 *	have values related to the graph in the preview window.	 This
 *	needs to be dealt with in the fullness of time.
 *  (f) Show nodeLabel2 iff showing numOfNodes2.
 *  (g) The usual collection of debug statement improvements.
 * Dec 15, 2019 (JD V1.22)
 *  (a) Modify on_graphType_ComboBox_currentIndexChanged() so that
 *	Prism gets the same treatment as Antiprism, vis-a-vis the
 *	behaviour of ui->numOfNodes1.
 * Dec 15, 2019 (JD V1.23)
 *  (a) Replace font.setPixelSize() (which is a device-dependent
 *	thing) with font.setPointSize() (which is device-INdependent).
 *	This makes the fonts on Linux show up at a reasonable size
 *	even without env QT_AUTO_SCREEN_SCALE_FACTOR=1 and things look
 *	fine on macos as well.
 *  (b) Replace a bunch of printf()s with qDebu(); delete a number of others.
 * May 11, 2020 (IC V1.24)
 *  (a) Changed the logical DPI variables to use physical DPI to correct
 *	scaling issues. (Only reliable with Qt V5.14.2 or higher)
 * May 12, 2020 (IC V1.25)
 *  (a) Removed certain labels that were unnecessary after redesigning the ui.
 * May 15, 2020 (IC V1.26)
 *  (a) Modified set_Font_Sizes() to be more readable and flexible if we decide
 *	to change font (sizes) at a later date.
 * May 19, 2020 (IC V1.27)
 *  (a) Updated set_Font_Sizes() to use embeded font "arimo" as default font.
 * May 25, 2020 (IC V1.28)
 *  (a) Removed setKeyStatusLabel() in favour of tooltips for each mode.
 *  (b) Added widget NumLabelStart which allows the user to start numbering
 *	nodes at a specified value instead of always 0.
 * May 28, 2020 (IC V1.29)
 *  (a) Modified save_Graph() to use a white background for JPEG files.
 * Jun 6, 2020 (IC V1.30)
 *  (a) Added set_Interface_Sizes() to fix sizing issues on monitors with
 *	different DPIs.
 * Jun 9, 2020 (IC V1.31)
 *  (a) Converted the node and edge label size doublespinboxes into
 *	regular spinboxes and updated any relevant connect statements.
 * Jun 10, 2020 (IC V1.32)
 *  (a) Added QSettings to save the window size on exit and load the size
 *	on startup. See saveSettings() and loadSettings()
 *  (b) Reimplemented closeEvent() to accommodate QSettings and prompt user to
 *	save graph if any exists on the canvas.
 *  (c) Added code to saveGraph() that supports saving default background
 *	colour of saved images. WIP
 * Jun 17, 2020 (IC V1.33)
 *  (a) Updated on_tabWidget_currentChanged() to display merged graphs under a
 *	single set of headers as well as delete those headers if the graph is
 *	deleted.
 * Jun 19, 2020 (IC V1.34)
 *  (a) Added multiple slots and appropriate connections for updating edit tab
 *	when graphs/nodes/edges are created.
 * Jun 26, 2020 (IC V1.35)
 *  (a) Update some connections to take a node or edge param.
 *  (b) Fixed a bug that would set the colour to black if user exited the
 *	colour select window without selecting a colour.
 *  (c) Rename on_tabWidget_currentChanged(int) to updateEditTab(int).
 *	Do some UI tweaking there.
 *  (d) Implement addGraphToEditTab(), addNodeToEditTab() and
 *	addEdgeToEditTab().
 * Jun 30, 2020 (IC V1.36)
 *  (a) Added another connection to refresh the preview pane with a new graph
 *	when the previous is dropped onto the canvas.
 * Jul 3, 2020 (IC V1.37)
 *  (a) Added code so that the thickness of a node (the circle) can be
 *	changed.  Added another connection to update the preview and
 *	params when the node thickness is adjusted.
 *  (b) Updated set_Font_Sizes() to include the new thickness widgets.
 * Jul 7, 2020 (IC V1.38)
 *  (a) Added another connection to update the zoomDisplay after a zoom change.
 * Jul 9, 2020 (IC V1.39)
 *  (a) Added another connection to update the edit tab after two graphs were
 *	joined.
 *  (b) Fixed a bug that allowed labels to be focusable on the preview pane.
 * Jul 14, 2020 (IC V1.40)
 *  (a) Corrected an issue that was preventing custom graphs from being
 *	refreshed on the preview after being dropped onto the canvas.
 * Jul 15, 2020 (IC V1.41)
 *  (a) Added node thickness widgets to the edit tab so that both node penwidth
 *	and diameter can be changed after leaving the preview pane.
 * Jul 22, 2020 (IC V1.42)
 *  (b) Set the font of the zoomDisplay.
 *  (c) Add "N width" label and widget to edit tab; rearrange edit tab widgets.
 * Jul 23, 2020 (IC V1.43)
 *  (a) Added another connection to update the edit tab after a graph is
 *	separated.  (Replace itemDeleted() with graphSeparated().)
 * Jul 24, 2020 (IC V1.44)
 *  (a) Added another connection to call clearCanvas when the clearCanvas
 *	button is pressed.
 * Jul 29, 2020 (IC V1.45)
 *  (a) Installed event filters in updateEditTab(int) to send event
 *	handling to node.cpp and edge.cpp.
 * Jul 31, 2020 (IC V1.46)
 *  (a) Added connections to somethingChanged() slot and bool promptSave that
 *	detects if any change has been made on the canvas since the last save
 *	and thus a new save prompt is needed on exit.
 *  (b) Add setting to allow the user to specify their physicalDPI
 *	setting, a la acroread.
 * Aug 5, 2020 (IC V1.47)
 *  (a) Node thickness is now be included in the save code and is passed
 *	to nodeParam functions.
 *	THIS DEPRECATES PREVIOUS .grphc FILES and changes .tikz output.
 *  (b) Added updateDpiAndPreview slot and settingsDialog variable to be used
 *	in conjunction with the new settingsDialog window which allows the user
 *	to use a custom DPI value instead of the system default.
 *  (c) Renamed nodeSize widget to nodeDiameter and edgeSize widget to
 *	edgeThickness for clarity.  Corresponding changes also in UI.
 *  (d) On exit, don't ask the user if the graph should be saved if it
 *	has not been changed since the last save.
 *  (e) In set_Font_Sizes() set the font of the clearCanvas button.
 * Aug 7, 2020 (IC V1.48)
 *  (a) save_Graph() now uses the saved background colours from settingsDialog
 *	to colour saved graphs.
 * Aug 11, 2020 (IC V1.49)
 *  (a) A zoom function was added to the canvas similar to the one for the
 *	preview so C_ZoomDisplay needs to be scaled in set_Interface_Sizes().
 *  (b) New connection for zoomChanged() signal.
 *  (c) Update preview when settings DPI is changed.  Add
 *	updateDpiAndPreview() function.
 *  (d) Handraulically scale clearCanvas and zoomDisplay{,_2} widgets.
 * Aug 12, 2020 (IC V1.50)
 *  (a) Cleaned up set_Interface_Sizes() to make the default scale code more
 *	readable.  Currently, the scale is based on logicalDPI / 72 for apple
 *	and logicalDPI / 96 for any other machine.
 *  (b) Major code removal: addEdgeToEditTab(), addNodeToEditTab() and
 *	addGraphToEditTab() all went away.
 * Aug 14, 2020 (IC V1.51)
 *  (a) Update call to setRotation() with new param.
 *  (b) Remove a now-bogus comment.
 * Aug 21, 2020 (IC V1.52)
 *  (a) Added the ability to number edge labels similar to nodes.  Edge labels
 *	can now have numbered subscripts or simply numbered labels and the user
 *	can specify the start number with EdgeNumLabelStart.
 *  (b) Widgets related to numbering slightly renamed to indicate whether they
 *	are related to an edge or a node for clarity.
 * Aug 24, 2020 (IC V1.51)
 *  (a) Changed the wording on some edit tab labels for clarity.
 * Aug 25, 2020 (IC V1.52)
 *  (a) Added a new basicGraphs category, circulant, so a new offsets widget
 *	was added. It only accepts input in the format "d,d,d" or "d d d"
 *	and occupies the same space as numOfNodes2.
 *  (c) For circulant graph, added connection for offsets widget and
 *	pass the offsets text to Create_Basic_Graph().	Set the
 *	offsets widget's font.	Show or hide that widget as desired.
 *  (d) Deleted some old commented-out code.
 * Aug 25, 2020 (IC V1.54)
 *  (a) Restrict the input for the offsets widget so users can't even
 *	enter invalid data.
 *  (b) When the offsets text is changed, a new graph is generated on
 *	the preview instead of simply styling the graph as new edges
 *	need to be added.
 * Aug 26, 2020 (IC V1.55)
 *  (a) Removed offsets from the mainwindow.ui and instead initialize it in
 *	the constructor before moving it to the location of numOfNodes2.
 *  (b) Restructured updateEditTab slightly so that nodes are added to
 *	the edit tab before edges.
 *  (c) Added a new setting underneath the snapToGrid checkbox that lets the
 *	user specify the size of the grid cells when snapToGrid is on.
 *	A signal is sent to the canvas scene when the value changes.
 * Aug 27, 2020 (IC + JD V1.56)
 *  (a) In select_custom_graph() fix the positioning of graphs whose width
 *	or height is equal to 0.
 *  (b) When a custom graph is read in, set the height and width widgets,
 *	as well as the node size widget, to values from the custom graph.
 *	(The node size is computed as the average of the actual node
 *	sizes, which is a so-so approximation to what we want.)
 *	This keeps the custom graph from magically changing size when, for
 *	example, the node fill colour is changed.
 *  (b) Removed redundant for loop from updateEditTab.
 *  (c) Moved the gridCellSize widget to settingsdialog as it made more sense
 *	there.
 *  (d) Moved clearCanvas to the layout above the canvas so it is out of the
 *	way of canvas graphs and  adheres to a layout.
 * Aug 28, 2020 (IC V1.57)
 *  (a) Update the connection for updateCellSize().
 *  (b) Improve the code which populates the edit tab.
 * Oct 16, 2020 (JD V1.58)
 *  (a) Rename saveSettings() -> saveWinSizeSettings() to clarify.
 *  (b) Add some code to make the use of settings more robust.
 *  (c) Tidy white-space.
 * Sep 4, 2020 (IC V1.59)
 *  (a) Lots of infrastructure added for the new canvas graph editing tab.
 *	There are now connections that call style_Canvas_Graph() with the
 *	appropriate ID of the widget that changed and all relevant widget
 *	information. Numerous on_clicked functions were added for the new
 *	colour widgets and checkboxes and set_Font_Sizes will now set the
 *	font for all widgets on the canvas graph tab.
 * Sep 8, 2020 (IC V1.60)
 *  (a) Fixed a rotation issue related to the new select mode.
 * Sep 9, 2020 (IC V1.61)
 *  (a) Changed the canvas change signals to slot into scheduleUpdate()
 *	instead of updateEditTab(). Currently, the edit tab should ONLY update
 *	when the user switches to it AND either a signal has been sent by
 *	canvasscene/view or a change was made on the canvas graph tab.
 *	(i.e. when ui->tabWidget->currentIndex() == 2 && updateNeeded == true)
 *  (b) Reintroduce the on_tabWidget_currentChanged() function, but
 *	with a much smaller job to do.
 * Sep 10, 2020 (IC V1.62)
 *  (a) resetCanvasGraphTab() added to reset the widgets on the canvas graph
 *	to their defaults whenever the selectedList is cleared.  It also resets
 *	any static variables used in style_Canvas_Graph().
 * Sep 11, 2020 (IC V1.63)
 *  (a) somethingChanged() function now also calls updateCanvasGraphList()
 *	to populate the graph list on the canvas graph tab.
 * Oct 17, 2020 (JD V1.64)
 *  (a) Replace a lot of code with colour.name().
 *  (b) Generic code tidying.
 * Oct 18, 2020 (JD V1.65)
 *  (a) Change all possible "color"s to "colour"s.
 *  (b) Add an enum tab_IDs and fix missed change from last commit (where
 *	the edit nodes and edges tab changed from tab 1 to tab 2).
 *  (c) Clarify some names, such as there was formerly both edgeLabel
 *	(a QLabel) and EdgeLabel (a QLineEdit); "EdgeLabel" is now
 *	"edgeLabelEdit".
 *  (d) zoomDisplay_2 is now C_ZoomDisplay ('C' for canvas, the other
 *	zoom display is on the preview pane).
 *  (e) The changes to add a new tab with many widgets similar to the
 *	preview pane introduced many widgets whose names were the same
 *	as the preview pane widgets, but with _2 appended.  All of
 *	these names were changed along the lines of this:
 *	   nodeDiameter_2 -> cNodeDiameter
 *	where (unlike (d)) 'c' is from 'edit Canvas graph tab'.
 *	This required many analogous changes in mainwindow.ui.
 *	Even with all the name changes things are still not as clear
 *	as I would like.
 * Oct 22, 2020 (JD V1.66)
 *  (a) Remove all functions that do I/O and put them in file-io.c.
 *      This required changing things a bit since the ui and parent
 *	widget are needed by some functions, and they are no longer in
 *	this class.  Some related functions (e.g., dumpTikZ() and
 *	dumpGraphIc()) are still here to avoid the potential nuisance
 *	of mapping functions in another file to QShortcuts.
 *  (b) Update many comments.
 *  (c) Modify on_numOfNodes[12]_valueChanged() to (i) use the
 *	argument instead of a getter, and (ii) allow the second
 *	parameter to be 1/2 the first one (possibly not useful, but it
 *	doesn't hurt anything).
 *  (d) Fix closeEvent() so that if the user cancels from the "save"
 *	dialog the program does not exit.
 *  (e) Many names which had both '_' and camel-case were changed to
 *	just use camel case.
 *  (f) Rearranged the code which decides when to update the edit tab,
 *      eliminating one function.
 *  (g) Default to select mode for "edit canvas graph" and disable
 *	that mode elsewhere, since it is otherwise useless as of now.
 *  (h) Blocked many signals when widgets are programmatically
 *	changed, since these signals may cause generateGraph() to be
 *	called multiple times, and this may cause anomalous behaviour.
 * Nov 14, 2020 (JD V1.67)
 *  (a) Use Graph::boundingBox() instead to boundingRect() to get a
 *	more accurate size of the graph for the Edit Canvas Graph tab.
 *  (b) Rename resetCanvasGraphTab() to resetEditCanvasGraphTabWidgets(),
 *	because it doesn't reset anything else there.
 *	Rename editCanvasTab to editCanvasGraphTab for added specificity.
 *  (c) Modify resetEditCanvasGraphTabWidgets() to (i) disable all the
 *	widgets (except colour pickers) in the Edit Canvas Graph tab
 *	if selectedList is empty, and (ii) to otherwise enable the
 *	desired widgets.
 *  (d) Improve the H & W values in the Edit Canvas Tab graph list by
 *	using graph->boundingBox() rather than boundingRect(), and by
 *	outputting them with g4 formats.
 *  (e) Add setEditCanvasGraphTabWidgets() so that canvasview() can
 *	change the widgets in this tab.
 *  (g) Only call updateCanvasGraphList() from somethingChanged() if
 *	the Edit Canvas Graph tab is visible.  But now call
 *	updateCanvasGraphList() from on_tabWidget_currentChanged()
 *	when switching to the Edit Canvas Graph tab.
 *	See the comments in those functions for why this was needed.
 *  (h) Changed style_Canvas_Graph() so that (i) H and W don't both
 *	change when one is modified, (ii) joined and freestyle
 *	graphs scale properly.
 *  (i) Improve some comments.
 * Nov 16, 2020 (JD V1.68)
 *  (a) Remove a now-bogus comment that was misleading enough to
 *	deserve a commit.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "file-io.h"
#include "edge.h"
#include "basicgraphs.h"
#include "colourlinecontroller.h"
#include "sizecontroller.h"
#include "labelcontroller.h"
#include "labelsizecontroller.h"
#include "colourfillcontroller.h"

#include <QDesktopWidget>
#include <QColorDialog>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QShortcut>
#include <qmath.h>
#include <QErrorMessage>
#include <QCloseEvent>

// The tab order is set in mainwindow.ui.  If it changes, so must this:
enum tab_IDs { previewTab = 0, editCanvasGraphTab, editNodesAndEdgesTab };

// The unit of these is points:
#define TITLE_SIZE	    20
#define SUB_TITLE_SIZE	    18
#define SUB_SUB_TITLE_SIZE  12


QSettings settings("Acadia", "Graphic");
qreal currentPhysicalDPI, currentPhysicalDPI_X, currentPhysicalDPI_Y;

static qreal screenLogicalDPI_X;
static bool updateNeeded = false;
static int previousRotation;



/*
 * Name:	saveGraph()
 * Purpose:	Map from a (parameterless) slot connected to an
 *		accelerator to a File_IO function which takes params.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	Everything File_IO::saveGraph() modifies.
 * Returns:	Return value of File_IO::save_Graph().
 * Assumptions:	None(?).
 * Bugs:	None known.
 * Notes:	Possibly a complex connect statement would obviate
 *		this function.
 */

bool
MainWindow::saveGraph()
{
    return File_IO::saveGraph(&promptSave, this, ui);
}



/*
 * Name:	loadGraphicFile()
 * Purpose:	Map from a (parameterless) slot connected to an
 *		accelerator to a File_IO function which takes params.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	Everything File_IO::loadGraphicFile() modifies.
 * Returns:	Return value of File_IO::loadGraphicFile()..
 * Assumptions:	None(?).
 * Bugs:	None known.
 * Notes:	Possibly a complex connect statement would obviate
 *		this function.
 */

bool
MainWindow::loadGraphicFile()
{
    return File_IO::loadGraphicFile(this, ui);
}



/*
 * Name:	MainWindow
 * Purpose:	Main window constructor
 * Arguments:	QWidget *
 * Output:	Nothing.
 * Modifies:	private MainWindow variables
 * Returns:	Nothing.
 * Assumptions: ?
 * Bugs:	None known... so far.
 * Notes:	This is a cpp file used with the mainwindow.ui file
 */

MainWindow::MainWindow(QWidget * parent) :
QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    File_IO::setFileDirectory(this);
    ui->setupUi(this);
    this->generateComboboxTitles();

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(saveGraph()));
    connect(ui->actionOpen_File, SIGNAL(triggered()),
	    this, SLOT(loadGraphicFile()));

    // Ctrl-Q quits.
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));

    // DEBUG HELP:
    // Dump TikZ to stdout
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_T), this, SLOT(dumpTikZ()));
    // Dump graph-ic code to stdout
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_G), this,
		  SLOT(dumpGraphIc()));

    // Create an offsets widget to be used with the circulant graph type.
    offsets = new QLineEdit;
    offsets->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    offsets->setPlaceholderText("offsets");
    offsets->setAlignment(Qt::AlignHCenter);

    // Restrict the input for offsets lineEdit to the format "d,d,d" or "d d d"
    // and move it to the same layout position as numOfNodes2.
    QRegExp re = QRegExp("([1-9]\\d{0,1}(, ?| ))+");
    QRegExpValidator * validator = new QRegExpValidator(re);
    offsets->setValidator(validator);

    // We want the offsets widget to be in the same row/column position as
    // numOfNodes2.
    int index = ui->gridLayout->indexOf(ui->numOfNodes2);
    int row, col, rSpan, cSpan;
    ui->gridLayout->getItemPosition(index, &row, &col, &rSpan, &cSpan);
    ui->gridLayout->addWidget(offsets, row, col, Qt::AlignHCenter);

    // The horrendous calls to connect() below were the simplest ones
    // I (JD) could find which allow passing information about which
    // UI widget was changed.  I could have had a separate function for
    // every widget, which then had just one line to call generateGraph()
    // with an appropriate argument, but that is perhaps even more
    // grotesque.

    // Redraw the preview pane graph (if any) when these NODE
    // parameters are modified:
    connect(ui->nodeDiameter,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { generateGraph(nodeDiam_WGT); });
    connect(ui->nodeThickness,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { generateGraph(nodeThickness_WGT); });
    connect(ui->NodeLabel1,
	    (void(QLineEdit::*)(const QString &))&QLineEdit::textChanged,
	    this, [this]() { generateGraph(nodeLabel1_WGT); });
    connect(ui->NodeLabel2,
	    (void(QLineEdit::*)(const QString &))&QLineEdit::textChanged,
	    this, [this]() { generateGraph(nodeLabel2_WGT); });
    connect(ui->NodeLabelSize,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { generateGraph(nodeLabelSize_WGT); });
    connect(ui->NodeNumLabelCheckBox,
	    (void(QCheckBox::*)(bool))&QCheckBox::clicked,
	    this, [this]() { generateGraph(nodeNumLabelCheckBox_WGT); });
    connect(ui->NodeNumLabelStart,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { generateGraph(nodeNumLabelStart_WGT); });
    connect(ui->NodeFillColour,
	    (void(QPushButton::*)(bool))&QPushButton::clicked,
	    this, [this]() { generateGraph(nodeFillColour_WGT); });
    connect(ui->NodeOutlineColour,
	    (void(QPushButton::*)(bool))&QPushButton::clicked,
	    this, [this]() { generateGraph(nodeOutlineColour_WGT); });

    // Redraw the preview pane graph (if any) when these EDGE
    // parameters are modified:
    connect(ui->edgeThickness,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { generateGraph(edgeThickness_WGT); });
    connect(ui->edgeLabelEdit,
	    (void(QLineEdit::*)(const QString &))&QLineEdit::textChanged,
	    this, [this]() { generateGraph(edgeLabel_WGT); });
    connect(ui->EdgeLabelSize,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { generateGraph(edgeLabelSize_WGT); });
    connect(ui->EdgeNumLabelCheckBox,
	    (void(QCheckBox::*)(bool))&QCheckBox::clicked,
	    this, [this]() { generateGraph(edgeNumLabelCheckBox_WGT); });
    connect(ui->EdgeNumLabelStart,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { generateGraph(edgeNumLabelStart_WGT); });
    connect(ui->EdgeLineColour,
	    (void(QPushButton::*)(bool))&QPushButton::clicked,
	    this, [this]() { generateGraph(edgeLineColour_WGT); });

    // Redraw the preview pane graph (if any) when these GRAPH
    // parameters are modified:
    connect(ui->graphRotation,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { generateGraph(graphRotation_WGT); });
    connect(ui->complete_checkBox,
	    (void(QCheckBox::*)(bool))&QCheckBox::clicked,
	    this, [this]() { generateGraph(completeCheckBox_WGT); });
    connect(ui->graphHeight,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { generateGraph(graphHeight_WGT); });
    connect(ui->graphWidth,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { generateGraph(graphWidth_WGT); });
    connect(ui->numOfNodes1,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { generateGraph(numOfNodes1_WGT); });
    connect(ui->numOfNodes2,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { generateGraph(numOfNodes2_WGT); });
    connect(ui->graphType_ComboBox,
	    (void(QComboBox::*)(int))&QComboBox::activated,
	    this, [this]() { generateGraph(graphTypeComboBox_WGT); });
    connect(offsets,
	    (void(QLineEdit::*)(const QString &))&QLineEdit::textChanged,
	    this, [this]() { generateGraph(offsets_WGT); });

    // When these NODE and EDGE parameters are changed, the updated
    // values are passed to the canvas view, so that nodes and edges
    // drawn in "Freestyle" mode are styled as per the settings in the
    // "Create Graph" tab.
    connect(ui->nodeDiameter, SIGNAL(valueChanged(double)),
	    this, SLOT(nodeParamsUpdated()));
    connect(ui->nodeThickness, SIGNAL(valueChanged(double)),
	    this, SLOT(nodeParamsUpdated()));
    connect(ui->NodeLabel1, SIGNAL(textChanged(QString)),
	    this, SLOT(nodeParamsUpdated()));
    connect(ui->NodeLabel2, SIGNAL(textChanged(QString)),
	    this, SLOT(nodeParamsUpdated()));
    connect(ui->NodeLabelSize, SIGNAL(valueChanged(int)),
	    this, SLOT(nodeParamsUpdated()));
    connect(ui->NodeNumLabelCheckBox, SIGNAL(clicked(bool)),
	    this, SLOT(nodeParamsUpdated()));
    connect(ui->NodeFillColour, SIGNAL(clicked(bool)),
	    this, SLOT(nodeParamsUpdated()));
    connect(ui->NodeOutlineColour, SIGNAL(clicked(bool)),
	    this, SLOT(nodeParamsUpdated()));

    connect(ui->edgeThickness, SIGNAL(valueChanged(double)),
	    this, SLOT(edgeParamsUpdated()));
    connect(ui->edgeLabelEdit, SIGNAL(textChanged(QString)),
	    this, SLOT(edgeParamsUpdated()));
    connect(ui->EdgeLabelSize, SIGNAL(valueChanged(int)),
	    this, SLOT(edgeParamsUpdated()));
    connect(ui->EdgeNumLabelCheckBox, SIGNAL(clicked(bool)),
	    this, SLOT(edgeParamsUpdated()));
    connect(ui->EdgeLineColour, SIGNAL(clicked(bool)),
	    this, SLOT(edgeParamsUpdated()));

    // Yet more connections...
    connect(ui->snapToGrid_checkBox, SIGNAL(clicked(bool)),
	    ui->canvas, SLOT(snapToGrid(bool)));
    connect(ui->canvas, SIGNAL(resetDragMode()),
	    ui->dragMode_radioButton, SLOT(click()));

    // These connects update the edit nodes and edges tab when the
    // number of items on the canvas changes.
    connect(ui->canvas->scene(), SIGNAL(graphDropped()),
	    this, SLOT(scheduleUpdate()));
    connect(ui->canvas->scene(), SIGNAL(graphJoined()),
	    this, SLOT(scheduleUpdate()));
    connect(ui->canvas->scene(), SIGNAL(graphSeparated()),
	    this, SLOT(scheduleUpdate()));
    connect(ui->canvas, SIGNAL(nodeCreated()),
	    this, SLOT(scheduleUpdate()));
    connect(ui->canvas, SIGNAL(edgeCreated()),
	    this, SLOT(scheduleUpdate()));

    // This adds a new graph to the preview pane when the previous is
    // dropped onto the canvas.
    connect(ui->canvas->scene(), SIGNAL(graphDropped()),
	    this, SLOT(regenerateGraph()));

    // Updates the zoomDisplays after zoomIn/zoomOut is called.
    connect(ui->preview, SIGNAL(zoomChanged(QString)),
	    ui->zoomDisplay, SLOT(setText(QString)));
    connect(ui->canvas, SIGNAL(zoomChanged(QString)),
	    ui->C_ZoomDisplay, SLOT(setText(QString)));

    // Clears all items from the canvas:
    connect(ui->clearCanvas, SIGNAL(clicked()),
	    ui->canvas, SLOT(clearCanvas()));

    // Ask to save on exit if any changes were made on the canvas since
    // last save and update the list of graphs on the canvas graph tab.
    connect(ui->canvas->scene(), SIGNAL(somethingChanged()),
	    this, SLOT(somethingChanged()));
    connect(ui->canvas, SIGNAL(nodeCreated()), this, SLOT(somethingChanged()));
    connect(ui->canvas, SIGNAL(edgeCreated()), this, SLOT(somethingChanged()));
    connect(ui->canvas->scene(), SIGNAL(graphDropped()),
	    this, SLOT(somethingChanged()));
    connect(ui->canvas->scene(), SIGNAL(graphJoined()),
	    this, SLOT(somethingChanged()));

    // The following connects relate to the Canvas Graph tab...
    connect(ui->cNodeDiameter,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cNodeDiam_WGT); });
    connect(ui->cNodeThickness,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cNodeThickness_WGT); });
    connect(ui->cNodeLabel1,
	    (void(QLineEdit::*)(const QString &))&QLineEdit::textChanged,
	    this, [this]() { style_Canvas_Graph(cNodeLabel1_WGT); });
    connect(ui->cNodeLabelSize,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cNodeLabelSize_WGT); });
    connect(ui->cNodeNumLabelCheckBox,
	    (void(QCheckBox::*)(bool))&QCheckBox::clicked,
	    this, [this]() { style_Canvas_Graph(cNodeNumLabelCheckBox_WGT); });
    connect(ui->cNodeNumLabelStart,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cNodeNumLabelStart_WGT); });
    connect(ui->cNodeFillColour,
	    (void(QPushButton::*)(bool))&QPushButton::clicked,
	    this, [this]() { style_Canvas_Graph(cNodeFillColour_WGT); });
    connect(ui->cNodeOutlineColour,
	    (void(QPushButton::*)(bool))&QPushButton::clicked,
	    this, [this]() { style_Canvas_Graph(cNodeOutlineColour_WGT); });

    connect(ui->cEdgeThickness,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cEdgeThickness_WGT); });
    connect(ui->cEdgeLabelEdit,
	    (void(QLineEdit::*)(const QString &))&QLineEdit::textChanged,
	    this, [this]() { style_Canvas_Graph(cEdgeLabel_WGT); });
    connect(ui->cEdgeLabelSize,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cEdgeLabelSize_WGT); });
    connect(ui->cEdgeNumLabelCheckBox,
	    (void(QCheckBox::*)(bool))&QCheckBox::clicked,
	    this, [this]() { style_Canvas_Graph(cEdgeNumLabelCheckBox_WGT); });
    connect(ui->cEdgeNumLabelStart,
	    (void(QSpinBox::*)(int))&QSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cEdgeNumLabelStart_WGT); });
    connect(ui->cEdgeLineColour,
	    (void(QPushButton::*)(bool))&QPushButton::clicked,
	    this, [this]() { style_Canvas_Graph(cEdgeLineColour_WGT); });

    connect(ui->cGraphRotation,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cGraphRotation_WGT); });
    connect(ui->cGraphHeight,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cGraphHeight_WGT); });
    connect(ui->cGraphWidth,
	    (void(QDoubleSpinBox::*)(double))&QDoubleSpinBox::valueChanged,
	    this, [this]() { style_Canvas_Graph(cGraphWidth_WGT); });

    // Reset appropriate widgets and variables whenever selectedList
    // is changed.  Note that this signal is emitted by various
    // functions in canvasview.cpp.
    connect(ui->canvas, SIGNAL(selectedListChanged()),
	    this, SLOT(resetEditCanvasGraphTabWidgets()));

    // Initialize the canvas to be in "drag" mode.
    ui->dragMode_radioButton->click();

    // Initialize colour buttons.
    QString s("background: #000000;" BUTTON_STYLE);
    ui->EdgeLineColour->setStyleSheet(s);
    ui->NodeOutlineColour->setStyleSheet(s);
    ui->cEdgeLineColour->setStyleSheet(s);
    ui->cNodeOutlineColour->setStyleSheet(s);

    s = "background: #ffffff;" BUTTON_STYLE;
    ui->NodeFillColour->setStyleSheet(s);
    ui->cNodeFillColour->setStyleSheet(s);

    edgeParamsUpdated();
    nodeParamsUpdated();

    // Initialize the canvas to enable snapToGrid feature when loaded.
    ui->canvas->snapToGrid(ui->snapToGrid_checkBox->isChecked());

    // Initialize font sizes for ui labels/widgets.
    setFontSizes();

    gridLayout = new QGridLayout();
    ui->scrollAreaWidgetContents->setLayout(gridLayout);

    // Initialize Create Graph pane to default values:
    on_graphType_ComboBox_currentIndexChanged(-1);

    QScreen * screen = QGuiApplication::primaryScreen();
    if (settings.value("useDefaultResolution") == false)
    {
	currentPhysicalDPI = settings.value("customResolution").toReal();
	currentPhysicalDPI_X = settings.value("customResolution").toReal();
	currentPhysicalDPI_Y = settings.value("customResolution").toReal();
    }
    else
    {
	currentPhysicalDPI = screen->physicalDotsPerInch();
	currentPhysicalDPI_X = screen->physicalDotsPerInchX();
	currentPhysicalDPI_Y = screen->physicalDotsPerInchY();
    }
    screenLogicalDPI_X = screen->logicalDotsPerInchX();

    loadWinSizeSettings();

    // Unfortunately qreal QVariants can't convert... so we store an int...
    int defaultDPI = screen->physicalDotsPerInch();
    settings.setValue("defaultResolution", defaultDPI);

    settingsDialog = new SettingsDialog(this);

    connect(ui->actionGraph_settings, SIGNAL(triggered()),
	    settingsDialog, SLOT(open()));
    connect(settingsDialog, SIGNAL(saveDone()),
	    this, SLOT(updateDpiAndPreview()));
    connect(settingsDialog, SIGNAL(saveDone()),
	    ui->canvas->scene(), SLOT(updateCellSize()));

#ifdef DEBUG
    // Info to help with dealing with HiDPI issues
    printf("MW::MW: Logical DPI: (%.3f, %.3f)\nPhysical DPI: (%.3f, %.3f)\n",
	   screen->logicalDotsPerInchX(), screen->logicalDotsPerInchY(),
	   screen->physicalDotsPerInchX(), screen->physicalDotsPerInchY());
    printf("      Physical size (mm): ht %.1f, wd %.3f\n",
	   screen->physicalSize().height(), screen->physicalSize().width());
    printf("      Pixel resolution:  %d, %d\n",
	   screen->size().height(), screen->size().width());
    printf("     screen->devicePixelRatio: %.3f\n", screen->devicePixelRatio());
    fflush(stdout);
#endif
}



/*
 * Name:	~MainWindow
 * Purpose:	frees the memory of mainwindow
 * Arguments:	none
 * Output:	none
 * Modifies:	mainwindow
 * Returns:	none
 * Assumptions: none
 * Bugs:	none...so far
 * Notes:	none
 */

MainWindow::~MainWindow()
{
    delete ui;
}



/*
 * Name:	generateComboboxTitles()
 * Purpose:	Populate the list of graph types with the defined
 *		basic types, then add a separator, then call
 *		loadGraphicLibrary() to load the local graph library
 *		(if any).
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The ui->graphType_ComboBox
 * Returns:	Nothing.
 * Assumptions: ui->graphType_ComboBox is set up.
 * Bugs:	?
 * Notes:	None.
 */

void
MainWindow::generateComboboxTitles()
{
    BasicGraphs * basicG = new BasicGraphs();
    int i = 1;

    while (i < BasicGraphs::Count)
	ui->graphType_ComboBox->addItem(basicG->getGraphName(i++));

    ui->graphType_ComboBox->insertSeparator(BasicGraphs::Count);
    File_IO::loadGraphicLibrary(ui);
}



/*
 * Name:	somethingChanged()
 * Purpose:	Record the fact that something on the canvas changed.
 *		This is used so that we know whether or not a save
 *		dialog should be presented when the user quits the
 *		program.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The promptSave flag and the list of canvas graphs.
 * Returns:	Nothing.
 * Assumptions:	None.
 * Bugs:	None, surely.
 * Notes:	
 */

void
MainWindow::somethingChanged()
{
    promptSave = true;

    // If we are not in this tab, no use updating it, since it is
    // updated by on_tabWidget_currentChanged() when we switch to the
    // edit canvas graph tab.
    if (ui->tabWidget->currentIndex() == editCanvasGraphTab)
	updateCanvasGraphList();
}



/*
 * Name:	styleGraph()
 * Purpose:	Update a basic graph when a preview tab widget changes.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The graph in the preview scene.
 * Returns:	Nothing.
 * Assumptions: ?
 * Bugs:	?
 * Notes:	Do not call this on a non-basic graph, otherwise the
 *		colours, line thicknesses and node sizes are lost,
 *		since everything will be set to the current values of
 *		the UI boxes/sliders.
 */

void
MainWindow::styleGraph(enum widget_ID whatChanged)
{
    qDeb() << "MW::styleGraph(WID " << whatChanged << ") called";

    foreach (QGraphicsItem * item, ui->preview->scene()->items())
    {
	if (item->type() == Graph::Type)
	{
	    Graph * graphItem =	 qgraphicsitem_cast<Graph *>(item);
	    ui->preview->Style_Graph(
		graphItem,
		ui->graphType_ComboBox->currentIndex(),
		whatChanged,
		ui->nodeDiameter->value(),
		ui->NodeLabel1->text(),
		ui->NodeLabel2->text(),
		ui->NodeNumLabelCheckBox->isChecked(),
		ui->NodeLabelSize->value(),
		ui->NodeFillColour->palette().window().color(),
		ui->NodeOutlineColour->palette().window().color(),
		ui->edgeThickness->value(),
		ui->edgeLabelEdit->text(),
		ui->EdgeLabelSize->value(),
		ui->EdgeLineColour->palette().window().color(),
		ui->graphWidth->value(),
		ui->graphHeight->value(),
		ui->graphRotation->value(),
		ui->NodeNumLabelStart->value(),
		ui->nodeThickness->value(),
		ui->EdgeNumLabelCheckBox->isChecked(),
		ui->EdgeNumLabelStart->value());
	}
    }
}



/*
 * Name:	regenerateGraph()
 * Purpose:	When a graph is dragged from the preview pane to the
 *		main canvas, this function is called and recreates the
 *		same graph in the preview pane.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The preview pane.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	None.
 */

void
MainWindow::regenerateGraph()
{
    generateGraph(NO_WGT);
}



/*
 * Name:	generateGraph()
 * Purpose:	Load a new graph into the preview pane.
 * Arguments:	A value indicating which "New Graph" ui element was changed.
 * Outputs:	Nothing.
 * Modifies:	The graph drawing in the preview pane.
 * Returns:	Nothing.
 * Assumptions: There is only one MainWindow object per invocation of
 *		this program; otherwise the static vars below will
 *		need to be object variables.
 * Bugs:	?
 * Notes:	In the case of a non-"basicGraph", only UI items
 *		specifically modified should be applied to the graph.
 *		The tortuous connect() statements in MW's constructor
 *		call this function with an identifier for the changed
 *		UI item.  This information is only needed for
 *		"basic" graphs.
 *		Use static variables to remember the last graph type seen,
 *		and only (re-)load a library graph when the changed
 *		widget is the graphType_ComboBox.
 *		Only (re-)load a basic graph when a parameter which
 *		(might) affect the layout of the nodes has changed.
 * TODO:	Could we not just re-style the current graph when
 *		currentDrawEdges != drawEdges ?  This should not
 *		adjust the geometry of the nodes, nor should offsets.
 */

void
MainWindow::generateGraph(enum widget_ID changed_widget)
{
    static int currentGraphIndex = -1;	    // -1 does not exist
    static int currentNumOfNodes1 = -1;
    static int currentNumOfNodes2 = -1;
    static qreal currentNodeDiameter = -1;
    static bool currentDrawEdges = false;

    int graphIndex = ui->graphType_ComboBox->currentIndex();

    qDeb() << "\nMW::generateGraph(widget " << changed_widget << ") called.";

    if (ui->preview->items().count() == 0)
    {
	qDeb() << "\tpreview is empty, resetting currentGraphIndex to -1";
	currentGraphIndex = -1;
    }

    if (graphIndex < BasicGraphs::Count)
    {
	int numOfNodes1 = ui->numOfNodes1->value();
	int numOfNodes2 = ui->numOfNodes2->value();
	qreal nodeDiameter = ui->nodeDiameter->value();
	bool drawEdges = ui->complete_checkBox->isChecked();
	QString offsetsText = offsets->text();

	if (currentGraphIndex != graphIndex
	    || currentNumOfNodes1 != numOfNodes1
	    || currentNumOfNodes2 != numOfNodes2
	    || currentNodeDiameter != nodeDiameter
	    || currentDrawEdges != drawEdges
	    || changed_widget == offsets_WGT)
	{
	    qDeb() << "\tmaking a basic graph ("
		   << ui->graphType_ComboBox->currentText() << ")";
	    ui->preview->Create_Basic_Graph(graphIndex,
					    numOfNodes1, numOfNodes2,
					    nodeDiameter, drawEdges,
					    offsetsText);
	    this->styleGraph(ALL_WGT);
	    currentNumOfNodes1 = numOfNodes1;
	    currentNumOfNodes2 = numOfNodes2;
	    currentNodeDiameter = nodeDiameter;
	    currentDrawEdges = drawEdges;
	}
	else
	{
	    qDeb() << "\tredrawing the current basic graph ("
		   << ui->graphType_ComboBox->currentText() << ")";
	    this->styleGraph(changed_widget);
	}
    }
    else
    {
	if (graphIndex != currentGraphIndex)
	{
	    qDeb() << "\tmaking a '"
		   << ui->graphType_ComboBox->currentText()
		   << "' graph";
	    File_IO::inputCustomGraph(true,
				      ui->graphType_ComboBox->currentText()
				      + "." + GRAPHiCS_FILE_EXTENSION, ui);
	}
	else
	{
	    qDeb() << "\tsame library graph as last time, just style it.";
	    this->styleGraph(changed_widget);
	}
    }
    currentGraphIndex = graphIndex;

    // Node and edge labels are focusable (but not editable) so lets fix that
    if (!ui->editMode_radioButton->isChecked()) //Unnecessary but good practice
    {
	foreach (QGraphicsItem * item, ui->preview->scene()->items())
	{
	    if (item->type() == HTML_Label::Type)
	    {
		item->setFlag(QGraphicsItem::ItemIsFocusable, false);
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);//Useless?
	    }
	}
    }
}



/*
 * Name:	on_NodeOutlineColour_clicked()
 * Purpose:	Set the node outline colour for the preview tab.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	ui->NodeOutlineColour.
 * Returns:	Nothing.
 * Assumptions: ???
 * Bugs:	Setting the style sheet shrinks the button size.
 * Notes:	???
 */

void
MainWindow::on_NodeOutlineColour_clicked()
{
    QColor colour = QColorDialog::getColor();

    if (!colour.isValid())
	return;

    QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
    qDeb() << "MW::on_NodeOutlineColour_clicked(): outline colour set to" << s;
    ui->NodeOutlineColour->setStyleSheet(s);
    ui->NodeOutlineColour->update();
}



/*
 * Name:	on_NodeFillColour_clicked()
 * Purpose:	Set the node fill colour for the preview tab.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	ui->NodeFillColour
 * Returns:	Nothing.
 * Assumptions: ???
 * Bugs:	Setting the style sheet shrinks the button size.
 * Notes:	???
 */

void
MainWindow::on_NodeFillColour_clicked()
{
    QColor colour = QColorDialog::getColor();

    if (!colour.isValid())
	return;

    QString s("background: " + colour.name() + ";" + BUTTON_STYLE);
    qDeb() << "MW::on_NodeFillColour_clicked(): fill colour set to " << s;
    ui->NodeFillColour->setStyleSheet(s);
    ui->NodeFillColour->update();
}



/*
 * Name:	on_EdgeLineColour_clicked()
 * Purpose:	Set the edge line colour for the preview tab.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	ui->EdgeLineColour
 * Returns:	Nothing.
 * Assumptions: ???
 * Bugs:	Setting the style sheet shrinks the button size.
 * Notes:	???
 */

void
MainWindow::on_EdgeLineColour_clicked()
{
    QColor colour = QColorDialog::getColor();

    if (!colour.isValid())
	return;

    QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
    qDeb() << "MW::on_EdgeLineColour_clicked(): edge line colour set to" << s;
    ui->EdgeLineColour->setStyleSheet(s);
    ui->EdgeLineColour->update();
}



/*
 * Name:	on_cNodeOutlineColour_clicked()
 * Purpose:	Set the node outline colour for the edit canvas graph tab.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	ui->cNodeOutlineColour.
 * Returns:	Nothing.
 * Assumptions: ???
 * Bugs:	Setting the style sheet shrinks the button size.
 * Notes:	???
 */

void
MainWindow::on_cNodeOutlineColour_clicked()
{
    QColor colour = QColorDialog::getColor();

    if (!colour.isValid())
	return;

    QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
    qDeb() << "MW::on_cNodeOutlineColour_clicked(): outline colour set to" << s;
    ui->cNodeOutlineColour->setStyleSheet(s);
    ui->cNodeOutlineColour->update();
}



/*
 * Name:	on_cNodeFillColour_clicked()
 * Purpose:	Set the node fill colour for the edit canvas graph tab.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	ui->cNodeFillColour
 * Returns:	Nothing.
 * Assumptions: ???
 * Bugs:	Setting the style sheet shrinks the button size.
 * Notes:	???
 */

void
MainWindow::on_cNodeFillColour_clicked()
{
    QColor colour = QColorDialog::getColor();

    if (!colour.isValid())
	return;

    QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
    qDeb() << "MW::on_cNodeFillColour_clicked(): fill colour set to " << s;
    ui->cNodeFillColour->setStyleSheet(s);
    ui->cNodeFillColour->update();
}



/*
 * Name:	on_cEdgeLineColour_clicked()
 * Purpose:	Set the edge line colour for the edit canvas graph tab.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	ui->cEdgeLineColour
 * Returns:	Nothing.
 * Assumptions: ???
 * Bugs:	Setting the style sheet shrinks the button size.
 * Notes:	???
 */

void
MainWindow::on_cEdgeLineColour_clicked()
{
    QColor colour = QColorDialog::getColor();

    if (!colour.isValid())
	return;

    QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
    qDeb() << "MW::on_cEdgeLineColour_clicked(): edge line colour set to" << s;
    ui->cEdgeLineColour->setStyleSheet(s);
    ui->cEdgeLineColour->update();
}



/*
 * Name:	on_NodeNumLabelCheckBox_clicked()
 * Purpose:	Activate or deactivate the preview tab node label widgets.
 * Arguments:	A boolean.
 * Outputs:	Nothing.
 * Modifies:	The active/inactive status of the node label widgets.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	?!
 * Notes:	Simplified by JD on Jan 25/2016 to use less lines of code.
 *		(In honour of Robbie Burns?)
 */

void
MainWindow::on_NodeNumLabelCheckBox_clicked(bool checked)
{
    ui->NodeLabel1->setDisabled(checked);
    ui->NodeLabel2->setDisabled(checked);
}



/*
 * Name:	on_EdgeNumLabelCheckBox_clicked()
 * Purpose:	Activate or deactivate the preview tab edge label widget.
 * Arguments:	A boolean.
 * Outputs:	Nothing.
 * Modifies:	The active/inactive status of the edge label widget.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	?!
 * Notes:	None.
 */

void
MainWindow::on_EdgeNumLabelCheckBox_clicked(bool checked)
{
    ui->edgeLabelEdit->setDisabled(checked);
}



/*
 * Name:	on_cNodeNumLabelCheckBox_clicked()
 * Purpose:	Enable or disable the edit canvas node label text box.
 * Arguments:	A boolean.
 * Outputs:	Nothing.
 * Modifies:	The active/inactive status of the canvas tab node label widget.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	?!
 * Notes:	None.
 */

void
MainWindow::on_cNodeNumLabelCheckBox_clicked(bool checked)
{
    ui->cNodeLabel1->setDisabled(checked);
}



/*
 * Name:	on_cEdgeNumLabelCheckBox_clicked()
 * Purpose:	Enable or disable the edit canvas edge label text box.
 * Arguments:	A boolean.
 * Outputs:	Nothing.
 * Modifies:	The active/inactive status of the canvas tab node label widget.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	?!
 * Notes:	None.
 */

void
MainWindow::on_cEdgeNumLabelCheckBox_clicked(bool checked)
{
    ui->cEdgeLabelEdit->setDisabled(checked);
}



/*
 * Name:	MainWindow::set_Font_Sizes()
 * Purpose:	Set the font sizes of almost everything.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	Many, many fonts.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	?!
 * Notes:	It is possibly the case that almost all of these could
 *		be set by setting the application default font, followed
 *		then by a small number of exceptions.
 */

void
MainWindow::setFontSizes()
{
    QFont font;
    font.setFamily("Arimo");
    font.setPointSize(10);
    this->setFont(font);

    font.setPointSize(TITLE_SIZE);
    ui->graphLabel->setFont(font);

    ui->cGraphLabel->setFont(font);

    font.setPointSize(TITLE_SIZE - 1);
    ui->edgeLabel->setFont(font);
    ui->nodeLabel->setFont(font);

    ui->cEdgeLabel->setFont(font);
    ui->cNodeLabel->setFont(font);

    font.setPointSize(SUB_TITLE_SIZE);
    ui->partitionLabel->setFont(font);
    ui->colourLabel->setFont(font);

    ui->cColourLabel->setFont(font);

    font.setPointSize(SUB_SUB_TITLE_SIZE);
    ui->edgeThicknessLabel->setFont(font);
    ui->rotationLabel->setFont(font);
    ui->widthLabel->setFont(font);
    ui->heightLabel->setFont(font);
    ui->textInputLabel->setFont(font);
    ui->cTextInputLabel->setFont(font);
    ui->textSizeLabel->setFont(font);
    ui->cTextSizeLabel->setFont(font);
    ui->fillLabel->setFont(font);
    ui->outlineLabel->setFont(font);
    ui->nodeThicknessLabel->setFont(font);
    ui->nodeDiameterLabel->setFont(font);
    ui->numLabel->setFont(font);

    ui->cEdgeThicknessLabel->setFont(font);
    ui->cRotationLabel->setFont(font);
    ui->cWidthLabel->setFont(font);
    ui->cHeightLabel->setFont(font);
    ui->textInputLabel_3->setFont(font);
    ui->textInputLabel_4->setFont(font);
    ui->textSizeLabel_3->setFont(font);
    ui->textSizeLabel_4->setFont(font);
    ui->cFillLabel->setFont(font);
    ui->cOutlineLabel->setFont(font);
    ui->cNodeThicknessLabel->setFont(font);
    ui->cNodeDiameterLabel->setFont(font);
    ui->cNumLabel->setFont(font);

    ui->zoomDisplay->setFont(font);
    ui->C_ZoomDisplay->setFont(font);
    ui->clearCanvas->setFont(font);

    font.setPointSize(SUB_SUB_TITLE_SIZE - 1);
    ui->graphType_ComboBox->setFont(font);
    ui->complete_checkBox->setFont(font);
    ui->NodeNumLabelCheckBox->setFont(font);
    ui->EdgeNumLabelCheckBox->setFont(font);
    ui->edgeLabelEdit->setFont(font);
    ui->NodeLabel1->setFont(font);
    ui->NodeLabel2->setFont(font);

    ui->cNodeNumLabelCheckBox->setFont(font);
    ui->cEdgeNumLabelCheckBox->setFont(font);
    ui->cEdgeLabelEdit->setFont(font);
    ui->cNodeLabel1->setFont(font);

    font.setPointSize(SUB_SUB_TITLE_SIZE - 2);
    ui->graphHeight->setFont(font);
    ui->graphWidth->setFont(font);
    ui->numOfNodes1->setFont(font);
    ui->numOfNodes2->setFont(font);
    ui->nodeThickness->setFont(font);
    ui->graphRotation->setFont(font);
    ui->EdgeLabelSize->setFont(font);
    ui->edgeThickness->setFont(font);
    ui->NodeLabelSize->setFont(font);
    ui->nodeDiameter->setFont(font);
    ui->NodeNumLabelStart->setFont(font);
    ui->EdgeNumLabelStart->setFont(font);
    offsets->setFont(font);

    ui->cGraphHeight->setFont(font);
    ui->cGraphWidth->setFont(font);
    ui->cNodeThickness->setFont(font);
    ui->cGraphRotation->setFont(font);
    ui->cEdgeLabelSize->setFont(font);
    ui->cEdgeThickness->setFont(font);
    ui->cNodeLabelSize->setFont(font);
    ui->cNodeDiameter->setFont(font);
    ui->cNodeNumLabelStart->setFont(font);
    ui->cEdgeNumLabelStart->setFont(font);
}



/*
 * Name:	MainWindow::set_Interface_Sizes()
 * Purpose:	Resize the UI and correct widget minimum sizes
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The size of a number of widgets; see notes below.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	Qt will scale fonts automatically according to logicalDPI
 *		so we must handraulically scale some of the widgets that don't
 *		scale well (or at all).
 */

void
MainWindow::set_Interface_Sizes()
{
#ifdef __APPLE__
    #define SYSTEM_DEFAULT_LOGICAL_DPI 72
#else
    #define SYSTEM_DEFAULT_LOGICAL_DPI 96
#endif

    qreal scale = screenLogicalDPI_X / SYSTEM_DEFAULT_LOGICAL_DPI;

    // Total width of tabWidget borders:
    int borderWidth1 = 50 * scale;

    // Total width of mainWindow borders:
    int borderWidth2 = 30 * scale;

    // These three widgets need a max width or they misbehave, so we scale them
    ui->edgeLabelEdit->setMaximumWidth(ui->edgeLabelEdit->maximumWidth()
				       * scale);
    ui->NodeLabel1->setMaximumWidth(ui->NodeLabel1->maximumWidth() * scale);
    ui->NodeLabel2->setMaximumWidth(ui->NodeLabel2->maximumWidth() * scale);

    // These widgets don't belong to any layout, so they won't scale
    // automatically.
    ui->clearCanvas->resize(ui->clearCanvas->sizeHint());
    ui->zoomDisplay->resize(ui->zoomDisplay->sizeHint());
    ui->C_ZoomDisplay->resize(ui->C_ZoomDisplay->sizeHint());

    // Set the tabWidget to the first tab and fix the minimum width:
    ui->tabWidget->setCurrentIndex(previewTab);
    ui->tabWidget->setMinimumWidth(
	ui->scrollAreaWidgetContents_2->sizeHint().width() + borderWidth1);

    // Fix mainWindow's minimum width:
    this->setMinimumWidth(ui->tabWidget->minimumWidth()
			  + ui->gridLayout_3->sizeHint().width()
			  + borderWidth2);

    // Resize the initial window size for high dpi screens:
    if (!settings.contains("windowSize"))
    {
	this->resize(this->width() * scale, this->height() * scale);
	settings.setValue("windowSize", this->size());
    }
}



/*
 * Name:	on_graphType_ComboBox_currentIndexChanged()
 * Purpose:	Set up the Create Graph widgets in a sensible state
 *		(either for the default or for a particular graph type).
 * Arguments:	The index of the selected graph from the drop-down list
 *		(note that the list title, currently "Select Graph Type",
 *		is index 0).
 *		Any arg <= 0 produces the "default" setup and returns.
 * Outputs:	Nothing.
 * Modifies:	Various and sundry UI parameters.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:
 * Notes:	Doesn't know very much about what to do with graphs
 *		loaded from .grphc files.
 *		DO NOT RETURN FROM THE MIDDLE OF THIS FUNCTION!
 */

void
MainWindow::on_graphType_ComboBox_currentIndexChanged(int index)
{
    qDeb() << "\nMW::on_graphType_ComboBox_currentIndexChanged("
	   << index << ") called";

    // Here are the default settings.  Over-ride as needed below.
    ui->numOfNodes1->setSingleStep(1);
    ui->numOfNodes1->setMinimum(1);
    ui->numOfNodes1->show();

    ui->numOfNodes2->setSingleStep(1);
    ui->numOfNodes2->setMinimum(1);
    ui->numOfNodes2->hide();
    ui->NodeLabel2->hide();

    ui->partitionLabel->setText("Nodes");

    ui->graphHeight->show();
    ui->heightLabel->show();
    ui->graphWidth->show();
    ui->widthLabel->show();

    // If we don't block these signals before changing them,
    // generateGraph() will get called once for every time that the
    // value actually changes (which is only if the widget has a
    // different value than those used below).  I don't like that
    // (anomalous results have been observed), so tediously block and
    // (at the end of the function) unblock all the signals which
    // might be generated below.
    ui->graphHeight->blockSignals(true);
    ui->graphWidth->blockSignals(true);
    ui->graphRotation->blockSignals(true);
    ui->numOfNodes1->blockSignals(true);
    ui->numOfNodes2->blockSignals(true);

    ui->graphHeight->setValue(2.50);
    ui->graphWidth->setValue(2.50);
    ui->graphRotation->setValue(0);

    ui->complete_checkBox->show();

    offsets->hide();

    if (index <= 0)
	return;

    switch (index)
    {
      case BasicGraphs::Antiprism:
      case BasicGraphs::Prism:
	ui->numOfNodes1->setMinimum(6);
	if (ui->numOfNodes1->value() % 2 == 1)
	    ui->numOfNodes1->setValue(ui->numOfNodes1->value() - 1);
	ui->numOfNodes1->setSingleStep(2);
	break;

      case BasicGraphs::BBTree:
      case BasicGraphs::Complete:
	break;

      case BasicGraphs::Bipartite:
	ui->partitionLabel->setText("Partitions");
	ui->numOfNodes2->show();
	ui->NodeLabel2->show();
	break;

      case BasicGraphs::Circulant:
	ui->numOfNodes2->hide();
	offsets->show();
	break;

      case BasicGraphs::Cycle:
      case BasicGraphs::Crown:
      case BasicGraphs::Helm:
	ui->numOfNodes1->setMinimum(3);
	break;

      case BasicGraphs::Dutch_Windmill:
	ui->partitionLabel->setText("Blades & Nodes");
	ui->numOfNodes1->setMinimum(2);
	ui->numOfNodes2->show();
	ui->numOfNodes2->setMinimum(3);
	if (ui->numOfNodes2->value() < 3)
	    ui->numOfNodes2->setValue(3);

	// If someone really wants to scale this, why not?
	// ui->graphWidth->hide();
	// ui->widthLabel->hide();
	// But start them off with a square drawing area:
	ui->graphWidth->setValue(ui->graphHeight->value());
	break;

      case BasicGraphs::Gear:
	ui->numOfNodes1->setMinimum(6);
	break;

      case BasicGraphs::Grid:
	ui->partitionLabel->setText("Columns & Rows");
	ui->numOfNodes2->show();
	break;

      case BasicGraphs::Path:
	ui->graphHeight->hide();
	ui->heightLabel->hide();
	break;

      case BasicGraphs::Petersen:
	ui->partitionLabel->setText("Nodes & Step");
	ui->numOfNodes1->setMinimum(3);
	ui->numOfNodes2->setValue(2);
	ui->numOfNodes2->show();
	// If someone really wants to scale this, why not?
	// But start them off with a square drawing area:
	ui->graphWidth->setValue(ui->graphHeight->value());
	break;

      case BasicGraphs::Star:
      case BasicGraphs::Wheel:
	ui->numOfNodes1->setMinimum(4);
	break;

      default:
	// Should only get here if the graph is a library graph.
	// In that case, hide the numOfNodes1 widget, since we can't
	// change the number of nodes in a library graph from the
	// preview pane.
	qDeb() << "   Not the index of a basic graph, assuming a library graph";
	ui->numOfNodes1->hide();
    }

    // Unblock all signals that have been blocked above.
    ui->graphHeight->blockSignals(false);
    ui->graphWidth->blockSignals(false);
    ui->graphRotation->blockSignals(false);
    ui->numOfNodes1->blockSignals(false);
    ui->numOfNodes2->blockSignals(false);
}



/*
 * Name:	on_numOfNodes1_valueChanged()
 * Purpose:	Ensure that the new value of the numOfNodes1 spinbox
 *		does not cause some non-meaningful combination of
 *		parameters.
 * Arguments:	The value of the numOfNodes1.
 * Outputs:	Nothing.
 * Modifies:	Possibly ui->numOfNodes2.
 * Returns:	Nothing.
 * Assumptions: ???
 * Bugs:	???
 * Notes:	At time of writing, this function is magically
 *		connected to the ui->numOfNodes1 QSpinBox.
 */

void
MainWindow::on_numOfNodes1_valueChanged(int numOfNodes1)
{
    qDebu("MW::on_numOfNodes1_valueChanged(%d) called", numOfNodes1);
    qDebu("\t and ui->numOfNodes1->value() is %d", ui->numOfNodes1->value());

    if (ui->graphType_ComboBox->currentIndex() == BasicGraphs::Petersen)
    {
	if (ui->numOfNodes2->value() > numOfNodes1 / 2)
	{
	    qDeb() << "\tchanging ui->numOfNodes2 to 1 from "
		   << ui->numOfNodes2->value();
	    ui->numOfNodes2->blockSignals(true);
	    ui->numOfNodes2->setValue(1);
	    ui->numOfNodes2->blockSignals(false);
	}
    }
}



/*
 * Name:	on_numOfNodes2_valueChanged()
 * Purpose:	Ensure that the new value of the numOfNodes2 spinbox
 *		does not cause some non-meaningful combination of
 *		parameters.
 * Arguments:	(Unused)
 * Outputs:	Nothing.
 * Modifies:	Possibly ui->numOfNodes2.
 * Returns:	Nothing.
 * Assumptions: ???
 * Bugs:	???
 * Notes:	At time of writing, this function is magically
 *		connected to the ui->numOfNodes2 QSpinBox.
 */

void
MainWindow::on_numOfNodes2_valueChanged(int numOfNodes2)
{
    qDebu("MW::on_numOfNodes2_valueChanged(%d) called", numOfNodes2);

    if (ui->graphType_ComboBox->currentIndex() == BasicGraphs::Petersen)
    {
	if (numOfNodes2 > ui->numOfNodes1->value() / 2)
	{
	    qDeb() << "\tchanging ui->numOfNodes2 to 1 from " << numOfNodes2;
	    ui->numOfNodes2->blockSignals(true);
	    ui->numOfNodes2->setValue(1);
	    ui->numOfNodes2->blockSignals(false);
	}
    }
}



/*
 * Name:	nodeParamsUpdated()
 * Purpose:	Tell the canvas that a node param setting has changed.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	Nothing.
 * Returns:	Nothing.
 * Assumptions: There are no other node params to tell the canvas about.
 * Bugs:	?
 * Notes:	Should start # be added to this?
 */

void
MainWindow::nodeParamsUpdated()
{
    qDeb() << "MW::nodeParamsUpdated() called.";

    ui->canvas->setUpNodeParams(
	ui->nodeDiameter->value(),
	ui->NodeNumLabelCheckBox->isChecked(),	// Useful?
	ui->NodeLabel1->text(),		    // Useful?
	ui->NodeLabelSize->value(),
	ui->NodeFillColour->palette().window().color(),
	ui->NodeOutlineColour->palette().window().color(),
	ui->nodeThickness->value());
}



/*
 * Name:	edgeParamsUpdated()
 * Purpose:	Tell the canvas that an edge param setting has changed.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	Nothing.
 * Returns:	Nothing.
 * Assumptions: There are no other edge params to tell the canvas about.
 * Bugs:	?
 * Notes:	?
 */

void
MainWindow::edgeParamsUpdated()
{
    qDeb() << "MW::edgeParamsUpdated() called; EdgeLabelSize is "
	   << ui->EdgeLabelSize->value();

    ui->canvas->setUpEdgeParams(
	ui->edgeThickness->value(),
	ui->edgeLabelEdit->text(),
	ui->EdgeLabelSize->value(),
	ui->EdgeLineColour->palette().window().color(),
	ui->EdgeNumLabelCheckBox->isChecked());	 // Useful?
}



void
MainWindow::on_deleteMode_radioButton_clicked()
{
    ui->canvas->setMode(CanvasView::del);
}



void
MainWindow::on_joinMode_radioButton_clicked()
{
    ui->canvas->setMode(CanvasView::join);

}



void
MainWindow::on_editMode_radioButton_clicked()
{
    ui->canvas->setMode(CanvasView::edit);
}



void
MainWindow::on_dragMode_radioButton_clicked()
{
    ui->canvas->setMode(CanvasView::drag);
}



void
MainWindow::on_freestyleMode_radioButton_clicked()
{
    ui->canvas->setMode(CanvasView::freestyle);
}



void
MainWindow::on_selectMode_radioButton_clicked()
{
    ui->canvas->setMode(CanvasView::select);
}



/*
 * Name:	on_tabWidget_currentChanged()
 * Purpose:	Do any desired updates when the user changes from one
 *		tab to another.
 * Arguments:	The tab index.
 * Outputs:	Nothing.
 * Modifies:	The user view.
 * Returns:	Nothing.
 * Assumptions: ?
 * Bugs:	?
 * Notes:	At time of writing the select button is useless except
 *		on the edit canvas tab, but on that tab it seems like
 *		the best default.
 */

void
MainWindow::on_tabWidget_currentChanged(int index)
{
    qDebu("MW::on_tabWidget_currentChanged(%d) called", index);
    switch (index)
    {
      case previewTab:
	ui->selectMode_radioButton->setEnabled(false);
	ui->dragMode_radioButton->click();
	break;

      case editCanvasGraphTab:
	ui->selectMode_radioButton->setEnabled(true);
	ui->selectMode_radioButton->click();
	resetEditCanvasGraphTabWidgets();
	// TODO: As of Nov 2020 not all conditions that change the
	// canvas are caught, and so we can't be sure somethingChanged
	// is called, so we call it from here when switching in.
	updateCanvasGraphList();
	break;

      case editNodesAndEdgesTab:
	if (updateNeeded)
	    updateEditTab();
	ui->selectMode_radioButton->setEnabled(false);
	ui->dragMode_radioButton->click();
	break;

      default:
	qDebug() << "on_tabWidget_currentChanged() called with bogus index "
		 << index;
	break;
    }
}



void
MainWindow::scheduleUpdate()
{
    if (ui->tabWidget->currentIndex() == editNodesAndEdgesTab)
	updateEditTab();
    else
	updateNeeded = true;
}



/*
 * Name:	updateEditTab (formerly on_tabWidget_currentChanged())
 * Purpose:	Recreate the UI for the edit nodes and edges tab.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The user view of the edit nodes and edges tab.
 * Returns:	Nothing.
 * Assumptions: This is only called when the edit nodes and edges tab
 *		is visible.
 * Bugs:	This erases all the widgets on the tab, then recreates
 *		all the needed ones.
 *		TODO: fix it so that only the required changes are done.
 * Notes:	The UI for the preview tab is drawn by ui_mainwindow.
 */

void
MainWindow::updateEditTab()
{
    int row;

    QLayoutItem * wItem;
    QLayout * layout = ui->scrollAreaWidgetContents->layout();
    while ((wItem = layout->takeAt(0)) != nullptr)
    {
	if (wItem->widget())
	{
	    wItem->widget()->setParent(NULL);
	    // https://doc.qt.io/qt-5/qlayout.html#takeAt (Qt 5.15.1)
	    // suggests to do this, but it causes core dumps in
	    // SizeController::deletedNodeBoxes() ("delete box1;").
	    // So don't do it.
	    // delete wItem->widget();
	}
	delete wItem;
    }

    row = 0;
    foreach (QGraphicsItem * item, ui->canvas->scene()->items())
    {
	// Q: when would item be a 0 or nullptr?
	if (item != 0 || item != nullptr)
	{	// Only creates headers for "root" graphs
	    if (item->type() == Graph::Type
		&& item->parentItem() == nullptr
		&& !item->childItems().isEmpty())
	    {
		Graph * graph = qgraphicsitem_cast<Graph*>(item);

		QLabel * label = new QLabel("Graph");
		gridLayout->addWidget(label, row, 0);
		row++;

		QLabel * label2 = new QLabel("Line");
		gridLayout->addWidget(label2, row, 2);
		QLabel * label3 = new QLabel("Width");
		gridLayout->addWidget(label3, row+1, 2);
		QLabel * label4 = new QLabel("Node");
		gridLayout->addWidget(label4, row, 3);
		QLabel * label5 = new QLabel("Diam");
		gridLayout->addWidget(label5, row+1, 3);
		QLabel * label6 = new QLabel("Label");
		gridLayout->addWidget(label6, row, 4);
		QLabel * label7 = new QLabel("Text");
		gridLayout->addWidget(label7, row, 5);
		QLabel * label8 = new QLabel("Size");
		gridLayout->addWidget(label8, row+1, 5);
		QLabel * label9 = new QLabel("Line");
		gridLayout->addWidget(label9, row, 6);
		QLabel * label10 = new QLabel("Colour");
		gridLayout->addWidget(label10, row+1, 6);
		QLabel * label11 = new QLabel("Fill");
		gridLayout->addWidget(label11, row, 7);
		QLabel * label12 = new QLabel("Colour");
		gridLayout->addWidget(label12, row+1, 7);
		row += 2;

		// Horrible, ugly connects....
		connect(graph, SIGNAL(destroyed(QObject*)),
			label, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label2, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label3, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label4, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label5, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label6, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label7, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label8, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label9, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label10, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label11, SLOT(deleteLater()));
		connect(graph, SIGNAL(destroyed(QObject*)),
			label12, SLOT(deleteLater()));

		// Make two lists for nodes and edges:
		QList<QGraphicsItem *> nodeList, edgeList;
		foreach (QGraphicsItem * gItem, graph->childItems())
		{
		    if (gItem->type() == Node::Type)
			nodeList.append(gItem);
		    else if (gItem->type() == Edge::Type)
			edgeList.append(gItem);
		}

		// First add all nodes to the edit tab:
		while (!nodeList.isEmpty())
		{
		    QGraphicsItem * gItem = nodeList.at(0);
		    Node * node = qgraphicsitem_cast<Node*>(gItem);
		    QLineEdit * nodeEdit = new QLineEdit();

		    QLabel * label = new QLabel("Node");
		    // When this node is deleted, also
		    // delete its label in the edit tab.
		    connect(node, SIGNAL(destroyed(QObject*)),
			    label, SLOT(deleteLater()));

		    node->htmlLabel->editTabLabel = label;

		    QDoubleSpinBox * diamBox = new QDoubleSpinBox();
		    QDoubleSpinBox * thicknessBox = new QDoubleSpinBox();
		    QPushButton * lineColourButton = new QPushButton();
		    QPushButton * fillColourButton = new QPushButton();
		    QSpinBox * fontSizeBox = new QSpinBox();

		    nodeEdit->installEventFilter(node);
		    diamBox->installEventFilter(node);
		    thicknessBox->installEventFilter(node);
		    fontSizeBox->installEventFilter(node);

		    // All controllers handle deleting of widgets
		    SizeController * sizeController
			= new SizeController(node, diamBox, thicknessBox);
		    ColourLineController * colourLineController
			= new ColourLineController(node, lineColourButton);
		    LabelController * weightController
			= new LabelController(node, nodeEdit);
		    LabelSizeController * weightSizeController
			= new LabelSizeController(node, fontSizeBox);
		    ColourFillController * colourFillController
			= new ColourFillController(node, fillColourButton);

		    gridLayout->addWidget(label, row, 1);
		    gridLayout->addWidget(thicknessBox, row, 2);
		    gridLayout->addWidget(diamBox, row, 3);
		    gridLayout->addWidget(nodeEdit,	 row, 4);
		    gridLayout->addWidget(fontSizeBox, row, 5);
		    gridLayout->addWidget(lineColourButton, row, 6);
		    gridLayout->addWidget(fillColourButton, row, 7);
		    Q_UNUSED(sizeController);
		    Q_UNUSED(colourLineController);
		    Q_UNUSED(colourFillController);
		    Q_UNUSED(weightController);
		    Q_UNUSED(weightSizeController);
		    row++;
		    nodeList.removeFirst();
		}

		// Now add all edges to the edit tab:
		while (!edgeList.isEmpty())
		{
		    QGraphicsItem * gItem = edgeList.at(0);
		    Edge * edge = qgraphicsitem_cast<Edge*>(gItem);
		    QLineEdit * edgeEdit = new QLineEdit();
		    // Q: what were these for??
		    // editEdge->setText("Edge\n");
		    // gridLayout->addWidget(editEdge);

		    QLabel * label = new QLabel("Edge");
		    // When this edge is deleted, also
		    // delete its label in the edit tab.
		    connect(edge, SIGNAL(destroyed(QObject*)),
			    label, SLOT(deleteLater()));

		    edge->htmlLabel->editTabLabel = label;

		    QPushButton * button = new QPushButton();

		    QDoubleSpinBox * sizeBox = new QDoubleSpinBox();

		    QSpinBox * fontSizeBox = new QSpinBox();

		    edgeEdit->installEventFilter(edge);
		    sizeBox->installEventFilter(edge);
		    fontSizeBox->installEventFilter(edge);

		    // All controllers handle deleting of widgets
		    SizeController * sizeController
			= new SizeController(edge, sizeBox);
		    ColourLineController * colourController
			= new ColourLineController(edge, button);
		    LabelController * weightController
			= new LabelController(edge, edgeEdit);
		    LabelSizeController * weightSizeController
			= new LabelSizeController(edge, fontSizeBox);

		    gridLayout->addWidget(label, row, 1);
		    gridLayout->addWidget(sizeBox, row, 2);
		    gridLayout->addWidget(edgeEdit, row, 4);
		    gridLayout->addWidget(fontSizeBox, row, 5);
		    gridLayout->addWidget(button, row, 6);
		    Q_UNUSED(sizeController);
		    Q_UNUSED(colourController);
		    Q_UNUSED(weightController);
		    Q_UNUSED(weightSizeController);
		    row++;
		    edgeList.removeFirst();
		}
	    }
	}
	else
	    qDebug() << "MW::updateEditTab(): the elusive nullptr "
		     << "has been seen!!  Alert the media!";
    }

    if (row > 0)
    {
	QLabel * label = new QLabel(" ");
	gridLayout->addWidget(label, 1000, 1);
	gridLayout->setRowStretch(1000, 40);
    }
}



/*
 * Name:	dumpTikZ()
 * Purpose:	(Mainly for debugging.)  Dump the TikZ for the canvas
 *		contents on the terminal.
 * Arguments:	None.
 * Outputs:	The TikZ code for the graph(s) on the canvas.
 * Modifies:	Stdout.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	None.
 */

void
MainWindow::dumpTikZ()
{
    QVector<Node *> nodes;
    int numOfNodes = 0;

    foreach (QGraphicsItem * item, ui->canvas->scene()->items())
    {
	if (item->type() == Node::Type)
	{
	    Node * node = qgraphicsitem_cast<Node *>(item);
	    node->setID(numOfNodes++);
	    nodes.append(node);
	}
    }

    qDeb() << "%%========== TikZ dump of current graph follows: ============";
    QTextStream tty(stdout);
    File_IO::saveTikZ(tty, nodes);
}



/*
 * Name:	dumpGraphIc()
 * Purpose:	(Mainly for debugging.)  Dump the .grphc for the canvas
 *		contents on the terminal.
 * Arguments:	None.
 * Outputs:	The .grhpc code for the graph(s) on the canvas.
 * Modifies:	Stdout.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	saveGraphIc() assumes the node IDs have been set to
 *		meaningful values, so 
 *		extra info is output.  This extra output includes the
 *		ID of each node, which is why it is set below.
 */

void
MainWindow::dumpGraphIc()
{
    qDeb() << "MW::dumpGraphIc() called";
    QVector<Node *> nodes;
    int numOfNodes = 0;

    foreach (QGraphicsItem * item, ui->canvas->scene()->items())
    {
	if (item->type() == Node::Type)
	{
	    Node * node = qgraphicsitem_cast<Node *>(item);
	    node->setID(numOfNodes++);
	    nodes.append(node);
	}
    }

    qDeb() << "%%========= graphIc dump of current graph follows: ===========";
    QTextStream tty(stdout);
    File_IO::saveGraphIc(tty, nodes, true);
}



void
MainWindow::loadWinSizeSettings()
{
    if (settings.contains("windowSize"))
	this->resize(settings.value("windowSize").toSize());

    if (settings.contains("windowMaxed")
	&& settings.value("windowMaxed") == true)
	this->showMaximized();
}



void
MainWindow::saveWinSizeSettings()
{
    if (this->isMaximized())
	settings.setValue("windowMaxed", true);
    else
    {
	settings.setValue("windowMaxed", false);
	settings.setValue("windowSize", this->size());
    }
}



void
MainWindow::updateDpiAndPreview()
{
    QScreen * screen = QGuiApplication::primaryScreen();
    if (settings.value("useDefaultResolution").toBool() == true
	|| ! settings.contains("customResolution"))
    {
	currentPhysicalDPI = screen->physicalDotsPerInch();
	currentPhysicalDPI_X = screen->physicalDotsPerInchX();
	currentPhysicalDPI_Y = screen->physicalDotsPerInchY();
    }
    else
    {
	currentPhysicalDPI = settings.value("customResolution").toReal();
	currentPhysicalDPI_X = settings.value("customResolution").toReal();
	currentPhysicalDPI_Y = settings.value("customResolution").toReal();
    }

    // Need to redraw the preview graph if the DPI changed.
    // Pretending this widget changed is good enough for generateGraph().
    generateGraph(nodeDiam_WGT);
}


/*
 * Name:	closeEvent()
 * Purpose:	Deal with the user closing the main window.
 * Arguments:	A QCloseEvent.
 * Outputs:	Possibly a new file.
 * Modifies:	If the current canvas is non-empty but unsaved, and
 *		the user wishes to save it, the canvas contents will
 *		be written to a file.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	If the user is asked to save, saving any kind of
 *		output satisfies the following function.  Arguably the
 *		user should have to save a .grphc file.
 */

void
MainWindow::closeEvent(QCloseEvent * event)
{
    if (!ui->canvas->scene()->itemsBoundingRect().isEmpty()
	&& promptSave == true)
    {
	QMessageBox::StandardButton closeBtn
	    = QMessageBox::question(this, "Graphic",
				    tr("Save graph before quitting?"),
				    QMessageBox::Cancel | QMessageBox::No
				    | QMessageBox::Yes);
	if (closeBtn == QMessageBox::Cancel)
	    event->ignore();
	else
	{
	    if (closeBtn == QMessageBox::Yes)
	    {
		bool success = File_IO::saveGraph(&promptSave, this, ui);
		qDeb() << "MW:closeEvent(): FI:saveGraph() returns " << success;
		if (success)
		{
		    saveWinSizeSettings();
		    event->accept();
		}
		else
		    event->ignore();
	    }
	}
    }
    else
    {
	saveWinSizeSettings();
	event->accept();
    }
}



/*
 * Name:	style_Canvas_Graph()
 * Purpose:	Update the selected items on the canvas when a widget on the
 *		canvas graph tab is updated. This function simply passes the
 *		relevant information to the following overloaded function.
 * Arguments:	The changed widget ID.
 * Outputs:	Nothing.
 * Modifies:	The drawing of the canvas graph (indirectly).
 * Returns:	Nothing.
 * Assumptions: ?
 * Bugs:	?
 * Notes:	Closely mimics the style_graph() function.  Sort of.
 */

void
MainWindow::style_Canvas_Graph(enum canvas_widget_ID what_changed)
{
    qDeb() << "MW::style_Canvas_Graph(CWID " << what_changed << ") called";

    if (selectedList.isEmpty())
	return;

    style_Canvas_Graph(
	what_changed,
	ui->cNodeDiameter->value(),
	ui->cNodeLabel1->text(),
	ui->cNodeNumLabelCheckBox->isChecked(),
	ui->cNodeLabelSize->value(),
	ui->cNodeFillColour->palette().window().color(),
	ui->cNodeOutlineColour->palette().window().color(),
	ui->cEdgeThickness->value(),
	ui->cEdgeLabelEdit->text(),
	ui->cEdgeLabelSize->value(),
	ui->cEdgeLineColour->palette().window().color(),
	ui->cGraphWidth->value(),
	ui->cGraphHeight->value(),
	ui->cGraphRotation->value(),
	ui->cNodeNumLabelStart->value(),
	ui->cNodeThickness->value(),
	ui->cEdgeNumLabelCheckBox->isChecked(),
	ui->cEdgeNumLabelStart->value());
}



/*
 * Name:	style_Canvas_Graph()
 * Purpose:	Update the selected items on the canvas when a widget on the
 *		canvas graph tab is updated.
 * Arguments:	All relevant graph drawing info.
 * Outputs:	Nothing.
 * Modifies:	The drawing of the graph.
 * Returns:	Nothing.
 * Assumptions: ?
 * Bugs:	So many bugs, so little time...
 *		TODO: For example, changing the width or height of a rotated
 *		graph scales the graph in the rotated coordinate system.
 *		TODO: the height and width widgets could arguably be
 *		set to the right size when a single graph is selected.
 * Notes:	Rotation only works if an entire graph is selected.
 */

#define GUARD(x) if (x == what_changed)

void
MainWindow::style_Canvas_Graph(enum canvas_widget_ID what_changed,
			       qreal nodeDiameter,	QString nodeLabel,
			       bool nodeLabelsNumbered, qreal nodeLabelSize,
			       QColor nodeFillColour,	QColor nodeOutlineColour,
			       qreal edgeSize,		QString edgeLabel,
			       qreal edgeLabelSize,	QColor edgeLineColour,
			       qreal totalWidth,	qreal totalHeight,
			       qreal rotation,		qreal nodeNumStart,
			       qreal nodeThickness,	bool edgeLabelsNumbered,
			       qreal edgeNumStart)
{
    qDeb() << "MW::style_Canvas_Graph(........) called";
    int i = nodeNumStart;
    int j = edgeNumStart;

    foreach (QGraphicsItem * item, selectedList)
    {
	if (item->type() == Node::Type)
	{
	    Node * node = qgraphicsitem_cast<Node *>(item);

	    qDeb() << "   looking at node with label " << node->getLabel();

	    node->physicalDotsPerInchX = currentPhysicalDPI_X;

	    GUARD(cNodeThickness_WGT) node->setPenWidth(nodeThickness);
	    GUARD(cNodeDiam_WGT) node->setDiameter(nodeDiameter);
	    GUARD(cNodeFillColour_WGT) node->setFillColour(nodeFillColour);
	    GUARD(cNodeOutlineColour_WGT) node->setLineColour(nodeOutlineColour);
	    GUARD(cNodeLabelSize_WGT) node->setNodeLabelSize(nodeLabelSize);

	    if (what_changed == cNodeLabel1_WGT
		|| what_changed == cNodeNumLabelCheckBox_WGT
		|| what_changed == cNodeNumLabelStart_WGT)
	    {
		// Clear the node label, in case it was set previously.
		node->setNodeLabel("");
		if (nodeLabelsNumbered)
		    node->setNodeLabel(i++);
		else if (nodeLabel.length() != 0)
		    node->setNodeLabel(nodeLabel, i++);
	    }
	}
	else if (item->type() == Edge::Type)
	{
	    Edge * edge = qgraphicsitem_cast<Edge *>(item);

	    qDeb() << "   looking at edge with label " << edge->getLabel();

	    GUARD(cEdgeThickness_WGT) edge->setPenWidth(edgeSize);
	    GUARD(cEdgeLineColour_WGT) edge->setColour(edgeLineColour);
	    GUARD(cEdgeLabelSize_WGT)
		edge->setEdgeLabelSize((edgeLabelSize > 0) ? edgeLabelSize : 1);
	    if (what_changed == cEdgeLabel_WGT
		|| what_changed == cEdgeNumLabelCheckBox_WGT
		|| what_changed == cEdgeNumLabelStart_WGT)
	    {
		// Clear the edge label, in case it was set previously.
		edge->setEdgeLabel("");
		if (edgeLabelsNumbered)
		    edge->setEdgeLabel(j++);
		else if (edgeLabel.length() != 0)
		    edge->setEdgeLabel(edgeLabel, j++);
	    }
	    GUARD(cNodeDiam_WGT) edge->setDestRadius(nodeDiameter / 2.);
	    GUARD(cNodeDiam_WGT) edge->setSourceRadius(nodeDiameter / 2.);
	}
	else if (item->type() == Graph::Type)
	{
	    Graph * graph = qgraphicsitem_cast<Graph *>(item);
	    qDeb() << "   graph currently located at " << graph->x() << ", "
		   << graph->y();

	    GUARD(cGraphRotation_WGT)
	    {
		qreal netRotation = rotation - previousRotation;
		graph->setRotation(-1 * netRotation, true);
	    }

	    if (what_changed == cGraphWidth_WGT
		|| what_changed == cGraphHeight_WGT)
	    {
		// While the rotation widget is relative to the
		// previous rotation, the H and W widgets are current
		// values (averages of all graphs selected).  To make
		// them relative, we would need to allow the widgets
		// to be negative.  Do we want that?

		// In the preview, a graph might not fill the
		// requested bounding box in order to maintain symmetry.
		// However, here we only know the current actual size
		// of a graph, and so we scale according to its current
		// bounding box, not any information left over from
		// the preview (which is inaccurate for joined graphs
		// and non-existent for freestyle graphs anyway).

		QPointF center, RGcenter;
		QRectF bb = graph->boundingBox(&center, true, nullptr);
		QRectF bb2 = graph->boundingBox(nullptr, false, &RGcenter);
		qDeb() << "    bb is " << bb;
		qDeb() << "    center is " << center;
		qDeb() << "    bb2 is " << bb2;

		// Since we are not scaling the node sizes, we must
		// subtract their contribution to the overall size
		// from the desired size before computing the scale
		// factors.
		qreal nodeDiamWidthSlop = bb.width() - bb2.width();
		qreal nodeDiamHeightSlop = bb.height() - bb2.height();
		
		qreal widthScaleFactor = 1, heightScaleFactor = 1;
		GUARD(cGraphWidth_WGT) widthScaleFactor
		    = (totalWidth * currentPhysicalDPI_X - nodeDiamWidthSlop)
		    / bb2.width();
		GUARD(cGraphHeight_WGT) heightScaleFactor
		    = (totalHeight *  currentPhysicalDPI_Y - nodeDiamHeightSlop)
		    / bb2.height();

		qDeb() << "    Desired total width: " << totalWidth
		       << "; width = " << bb.width() / currentPhysicalDPI_X
		       << "; widthScaleFactor = " << widthScaleFactor;
		qDeb() << "    Desired total height: " << totalHeight
		       << "; height = " << bb.height() / currentPhysicalDPI_Y
		       << "; heightScaleFactor = " << heightScaleFactor;

		qreal xmid = RGcenter.x();
		qreal ymid = RGcenter.y();
		qDebu("    Center in graph coords is (%.4f, %.4f)\n",
		      xmid, ymid);
		qDebu("    Center in scene coords is (%.4f, %.4f)\n",
		      center.x(), center.y());

		foreach (QGraphicsItem * child, graph->childItems())
		{
		    if (child->type() == Node::Type)
		    {
			// We want to scale wrt the center of the graph.
			Node * node = qgraphicsitem_cast<Node *>(child);
			qreal newx = (child->pos().x() - xmid)
			    * widthScaleFactor + xmid;
			qreal newy = (child->pos().y() - ymid)
			    * heightScaleFactor + ymid;
			qDeb() << "   Moving node '" << node->getLabel()
			       << "' from " << child->pos() << " to ("
			       << newx << ", " << newy << ")";
			child->setPos(newx, newy);
			qDeb() << "    NOW node.pos() is " << child->pos();
		    }
		}		
		qDeb() << "   END: graph now located at "
		       << graph->x() << ", " << graph->y();
	    }
	}
    }

    // If ever the pen width is taken into account for the boundingBox(),
    // that widget should be included here.
    // TODO: Should we take (large) labels into account as well?
    if (what_changed == cNodeDiam_WGT
	|| what_changed == cGraphWidth_WGT
	|| what_changed == cGraphHeight_WGT
	|| what_changed == cGraphRotation_WGT)
	updateCanvasGraphList();

    previousRotation = ui->cGraphRotation->value();
    updateNeeded = true;
}



/*
 * Name:	updateCanvasGraphList()
 * Purpose:	Similar in methodology to updateEditTab, this clears the list
 *		of graphs on the canvas graph tab and repopulates it with each
 *		graph in the canvasGraphList and their respective widths and
 *		heights.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The graph list on the canvas graph tab.
 * Returns:	Nothing.
 * Assumptions: ?
 * Bugs:	None known.
 * Notes:	The Create Graph tab H & W specify the size of a bounding
 *		box into which a maximally-large graph is drawn *symmetrically*.
 *		The graph may not occupy the entire box.
 *		Contrariwise, the H & W displayed here give the actual
 *		size (except the pen size is not taken into account,
 *		which is also currently the case when using the Create
 *		Graph tab).
 */

void
MainWindow::updateCanvasGraphList()
{
    qDeb() << "MW::updateCanvasGraphList() called";
    // Clear the list, sadly...
    QLayoutItem * wItem;
    while ((wItem = ui->graphListLayout->layout()->takeAt(1)) != nullptr)
    {
	if (wItem->widget())
	    wItem->widget()->setParent(NULL);
	delete wItem;
    }

    // Now populate the list with the graphs and their dimensions.
    int i = 1;
    foreach (QGraphicsItem * item, canvasGraphList)
    {
	Graph * graph = qgraphicsitem_cast<Graph *>(item);

	QLabel * graphLabel = new QLabel("Graph " + QString::number(i));
	ui->graphListLayout->addWidget(graphLabel, i, 0);

	QRectF bb = graph->boundingBox(nullptr, true, nullptr);

	qreal height = bb.height() / currentPhysicalDPI_Y;
	QLabel * heightLabel = new QLabel("Height: "
					  + QString::number(height, 'g', 4));
	ui->graphListLayout->addWidget(heightLabel, i, 1);

	qreal width = bb.width() / currentPhysicalDPI_X;
	QLabel * widthLabel = new QLabel("Width: "
					 + QString::number(width, 'g', 4));
	ui->graphListLayout->addWidget(widthLabel, i, 2);

	connect(graph, SIGNAL(destroyed(QObject*)),
		graphLabel, SLOT(deleteLater()));
	connect(graph, SIGNAL(destroyed(QObject*)),
		heightLabel, SLOT(deleteLater()));
	connect(graph, SIGNAL(destroyed(QObject*)),
		widthLabel, SLOT(deleteLater()));

	i++;
    }
}



/*
 * Name:	resetEditCanvasGraphTabWidgets()
 * Purpose:	Ensures that the widgets on the canvas graph tab are reset
 *		to their defaults and disabled whenever the
 *		selectedList is emptied.
 *		When selectedList is set to something non-empty,
 *		enable and set appropriate widgets.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The widgets on the canvas graph tab.
 * Returns:	Nothing.
 * Assumptions: ?
 * Bugs:	?
 * Notes:	Arguably, these widgets should have their signals
 *		blocked before they are set.  However, since this is
 *		called when there is no selected graph on the canvas,
 *		no adjustments are made to any graph.
 */

void
MainWindow::resetEditCanvasGraphTabWidgets()
{
    if (selectedList.isEmpty())
    {
	qDeb() << "MW::resetEditCanvasGraphTabWidgets() called when "
	       << "selectedList is empty";

	ui->cGraphHeight->setValue(2.50);
	ui->cGraphHeight->setDisabled(true);
	ui->cGraphWidth->setValue(2.50);
	ui->cGraphWidth->setDisabled(true);

	previousRotation = 0;
	ui->cGraphRotation->setValue(0);
	ui->cGraphRotation->setDisabled(true);

	ui->cNodeLabel1->setText("");
	ui->cNodeLabel1->setDisabled(true);
	ui->cNodeNumLabelStart->setValue(0);
	ui->cNodeNumLabelStart->setDisabled(true);
	ui->cNodeNumLabelCheckBox->setChecked(false);
	ui->cNodeNumLabelCheckBox->setDisabled(true);

	ui->cNodeThickness->setValue(1.0);
	ui->cNodeThickness->setDisabled(true);
	ui->cNodeLabelSize->setValue(12);
	ui->cNodeLabelSize->setDisabled(true);
	ui->cNodeDiameter->setValue(0.20);
	ui->cNodeDiameter->setDisabled(true);

	ui->cEdgeLabelEdit->setText("");
	ui->cEdgeLabelEdit->setDisabled(true);
	ui->cEdgeNumLabelStart->setValue(0);
	ui->cEdgeNumLabelStart->setDisabled(true);
	ui->cEdgeNumLabelCheckBox->setChecked(false);
	ui->cEdgeNumLabelCheckBox->setDisabled(true);

	ui->cEdgeThickness->setValue(1.0);
	ui->cEdgeThickness->setDisabled(true);
	ui->cEdgeLabelSize->setValue(12);
	ui->cEdgeLabelSize->setDisabled(true);
    }
    else
    {
	qDeb() << "MW::resetEditCanvasGraphTabWidgets() called when "
	       << "selectedList is NOT empty";
	int num_graphs = 0, num_edges = 0, num_nodes = 0;
	qreal total_ht = 0, total_wd = 0;
	qreal total_e_lsize = 0, total_e_thick = 0;
	qreal total_n_lsize = 0, total_n_thick = 0, total_n_diam = 0;
	foreach (QGraphicsItem * item, selectedList)
	{
	    if (item->type() == Node::Type)
	    {
		Node * node = qgraphicsitem_cast<Node *>(item);
		num_nodes++;
		total_n_thick += node->getPenWidth();
		total_n_lsize += node->getLabelSize();
		total_n_diam += node->getDiameter();
	    }
	    else if (item->type() == Edge::Type)
	    {
		Edge * edge = qgraphicsitem_cast<Edge *>(item);
		num_edges++;
		total_e_thick += edge->getPenWidth();
		total_e_lsize += edge->getLabelSize();
	    }
	    else if (item->type() == Graph::Type)
	    {
		Graph * graph = qgraphicsitem_cast<Graph *>(item);
		num_graphs++;
		QRectF bbox = graph->boundingBox(nullptr, true, nullptr);
		total_wd += bbox.width();
		total_ht += bbox.height();
	    }
	}

	// Changing widgets causes style_Canvas_Graph() to be called.
	// The Right Way to do it is to call
	//     ui->[widget]->blockSignals(true);
	//     ui->[widget]->setValue(...);
	//     ui->[widget]->blockSignals(false);
	// How tedious.
	// Instead, since style_Canvas_Graph() returns immediately if
	// selectedList is empty, empty it before changing widgets.
	// Restore it when done tweaking widgets.
	QList<QGraphicsItem *> selectedListHold = selectedList;
	selectedList.clear();

	if (num_graphs > 0)
	{
	    ui->cGraphHeight->setValue(total_ht
				       / num_graphs / currentPhysicalDPI_Y);
	    ui->cGraphHeight->setDisabled(false);

	    ui->cGraphWidth->setValue(total_wd
				      / num_graphs / currentPhysicalDPI_X);
	    ui->cGraphWidth->setDisabled(false);

	    ui->cGraphRotation->setValue(0);
	    ui->cGraphRotation->setDisabled(false);
	}

	if (num_nodes > 0)
	{
	    ui->cNodeLabel1->setDisabled(false);
	    ui->cNodeNumLabelStart->setDisabled(false);
	    ui->cNodeNumLabelCheckBox->setDisabled(false);

	    ui->cNodeThickness->setValue(total_n_thick / num_nodes);
	    ui->cNodeThickness->setDisabled(false);
	    ui->cNodeLabelSize->setValue(total_n_lsize / num_nodes);
	    ui->cNodeLabelSize->setDisabled(false);
	    ui->cNodeDiameter->setValue(total_n_diam / num_nodes);
	    ui->cNodeDiameter->setDisabled(false);
	}

	if (num_edges > 0)
	{
	    ui->cEdgeLabelEdit->setDisabled(false);
	    ui->cEdgeNumLabelStart->setDisabled(false);
	    ui->cEdgeNumLabelCheckBox->setDisabled(false);

	    ui->cEdgeThickness->setValue(total_e_thick / num_edges);
	    ui->cEdgeThickness->setDisabled(false);
	    ui->cEdgeLabelSize->setValue(total_e_lsize / num_edges);
	    ui->cEdgeLabelSize->setDisabled(false);
	}

	// Restore:
	selectedList = selectedListHold;
    }
}
