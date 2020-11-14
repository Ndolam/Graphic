/*
 * File:	mainwindow.h
 * Author:	Rachel Bood
 * Date:	January 25, 2015.
 * Version:	1.25
 *
 * Purpose:	Define the MainWindow class.
 *
 * Modification history:
 * Oct 13, 2019 (JD V1.2)
 *  (a) Minor comment & formatting changes.
 *  (b) Add lookupColour() (for TikZ output routine).
 * Nov 17, 2019 (JD V1.3)
 *  (a) Remove lookupColour() (now a non-class function).
 * Nov 28, 2019 (JD V1.4)
 *  (a) Add dumpGraphIc() and dumpTikZ().
 * Nov 29, 2019 (JD V1.5)
 *  (a) Rename "none" mode to "drag" mode, for less confusion.
 *	This required changes to mainwindow.ui as well.
 *	(Also changed "Complete" to "Draw edges" there.)
 * Dec 6, 2019 (JD V1.6)
 *  (a) Rename generate_Freestyle_{Nodes,Edges} to {node,edge}ParamsUpdated
 *	to better reflect what those functions do.
 *  (b) Modify generate_Graph() to take a parameter.
 *  (c) Add the enum widget_num as the parameter values for generate_Graph().
 * Dec 9, 2019 (JD V1.7)
 *  (a) Add on_numOfNodes1_valueChanged() to follow the actions of
 *	on_numOfNodes2_valueChanged().
 * Dec 12, 2019 (JD V1.8)
 *  (a) #include defuns.h.
 *  (b) Change "changed_widget" from an int to and enum widget_ID.
 *  (c) Move enum widget_ID to defuns.h.
 * May 15, 2020 (IC V1.9)
 *  (a) Renamed set_Font_Label_Sizes() to set_Font_Sizes() to better reflect
 *	what the function does.
 * May 25, 2020 (IC V1.10)
 *  (a) Removed setKeyStatusLabel() in favour of tooltips for each mode.
 * Jun 6, 2020 (IC V1.11)
 *  (a) Added set_Interface_Sizes() to fix sizing issues on monitors with
 *	different DPIs.
 * Jun 10, 2020 (IC V1.12)
 *  (a) Added loadSettings(), saveSettings(), and reimplemented closeEvent().
 * Jun 19, 2020 (IC V1.13)
 *  (a) Added multiple slots for updating edit tab when graphs/nodes/edges are
 *	created.
 * Jun 26, 2020 (IC V1.14)
 *  (a) Rename on_tabWidget_currentChanged(int) to updateEditTab(int).
 *  (b) Add params to add<X>ToEditTab().
 *  (c) Add graphList to the mainwindow object.
 * Jul 7, 2020 (IC V1.15)
 *  (a) Add generate_Graph() function.
 * Aug 6, 2020 (IC V1.16)
 *  (a) Add #includes for settingsdialog.h and ui_settingsdialog.h.
 *  (b) Add somethingChanged(), promptSave and settingsDialog.
 * Aug 11, 2020 (IC V1.17)
 *  (a) Add updateDpiAndPreview().
 *  (b) Added settingsDialog variable to be used in conjunction with
 *	the new settingsDialog window which allows the user to use a
 *	custom DPI value instead of the system default.
 * Aug 13, 2020 (IC V1.18)
 *  (a) Removed addGraphToEditTab(), addNodeToEditTab() and addEdgeToEditTab().
 * Aug 21, 2020 (IC V1.19)
 *  (a) Added the ability to number edge labels similar to nodes so
 *	on_EdgeNumLabelCheckBox_clicked was added as well.
 *	Some related renaming of functions was required.
 * Aug 26, 2020 (IC V1.20)
 *  (a) Added a QLineEdit * offsets field, since the offsets widget is
 *	now created in mainwindow.cpp, non in mainwindow.ui.
 * Oct 16, 2020 (JD V1.21)
 *  (a) Rename saveSettings() -> saveWinSizeSettings() to clarify.
 * Sep 4, 2020 (IC V1.22)
 *  (a) Lots of infrastructure added for the new canvas graph editing tab.
 *	Numerous on_clicked() functions were added for the new colour
 *	widgets and checkboxes.	 set_Font_Sizes() will now set the
 *	font for all widgets on the canvas graph tab.
 *	style_Canvas_Graph() passes the changed_widget ID to the
 *	overloaded functions of the same name along with all relevant
 *	canvas graph tab widgets to style the selected canvas items.
 * Oct 18, 2020 (JD V1.23)
 *  (a) Fix spelling.
 *  (b) Modify names (see comment in mainwindow.cpp for details).
 *  (c) Removed spurious private variable.
 * Oct 22, 2020 (JD V1.24)
 *  (a) Not all of the actions above were checked in.  Now they are.
 *  (b) Modify the collection of function and variable names to
 *	reflect the fact that many functions were moved to file-io.
 * Nov 12, 2020 (JD V1.25)
 *  (a) Rename resetCanvasGraphTab() to resetEditCanvasGraphTabWidgets()
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGridLayout>
#include <QScrollArea>

#include "defuns.h"
#include "graph.h"
#include "settingsdialog.h"
#include "ui_settingsdialog.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget * parent = 0);
    ~MainWindow();
    void set_Interface_Sizes();

  protected:
    virtual void closeEvent (QCloseEvent * event);

  private slots:
    bool saveGraph();
    bool loadGraphicFile();
    void regenerateGraph();
    void generateGraph(enum widget_ID changed_widget);
    void styleGraph(enum widget_ID changed_widget);
    void generateComboboxTitles();
    void dumpGraphIc();
    void dumpTikZ();

    void setFontSizes();

    void on_NodeOutlineColour_clicked();
    void on_NodeFillColour_clicked();
    void on_EdgeLineColour_clicked();

    void on_NodeNumLabelCheckBox_clicked(bool checked);
    void on_EdgeNumLabelCheckBox_clicked(bool checked);

    void on_graphType_ComboBox_currentIndexChanged(int index);
    void on_numOfNodes1_valueChanged(int arg1);
    void on_numOfNodes2_valueChanged(int arg1);

    void nodeParamsUpdated();
    void edgeParamsUpdated();

    void on_cNodeNumLabelCheckBox_clicked(bool checked);
    void on_cEdgeNumLabelCheckBox_clicked(bool checked);

    void on_cNodeOutlineColour_clicked();
    void on_cNodeFillColour_clicked();
    void on_cEdgeLineColour_clicked();

    void on_deleteMode_radioButton_clicked();
    void on_joinMode_radioButton_clicked();
    void on_editMode_radioButton_clicked();
    void on_dragMode_radioButton_clicked();
    void on_freestyleMode_radioButton_clicked();
    void on_selectMode_radioButton_clicked();
    void on_tabWidget_currentChanged(int index);

    void updateEditTab();
    void scheduleUpdate();

    void somethingChanged();
    void updateDpiAndPreview();

    void updateCanvasGraphList();
    void resetEditCanvasGraphTabWidgets();

    void style_Canvas_Graph(enum canvas_widget_ID what_changed);
    void style_Canvas_Graph(enum canvas_widget_ID what_changed,
			    qreal nodeDiameter,	    QString nodeLabel,
			    bool labelsAreNumbered, qreal nodeLabelSize,
			    QColor nodeFillColour,  QColor nodeOutlineColour,
			    qreal edgeSize,	    QString edgeLabel,
			    qreal edgeLabelSize,    QColor edgeLineColour,
			    qreal totalWidth,	    qreal totalHeight,
			    qreal rotation,	    qreal numStart,
			    qreal nodeThickness,    bool edgeLabelsNumbered,
			    qreal edgeNumStart);

  private:
    void loadWinSizeSettings();
    void saveWinSizeSettings();

    Ui::MainWindow * ui;
    QGridLayout * gridLayout;
    QScrollArea * scroll;
    bool promptSave = false;
    SettingsDialog * settingsDialog;
    QLineEdit * offsets;
};

#endif // MAINWINDOW_H
