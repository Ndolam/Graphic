/*
 * File:	mainwindow.h
 * Author:	Rachel Bood
 * Date:	January 25, 2015.
 * Version:	1.10
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
 *      This required changes to mainwindow.ui as well.
 *	(Also changed "Complete" to "Draw edges" there.)
 * Dec 6, 2019 (JD V1.6)
 *  (a) Rename generate_Freestyle_{Nodes,Edges} to {node,edge}ParamsUpdated
 *      to better reflect what those functions do.
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
 */


#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGridLayout>
#include <QScrollArea>

#include "defuns.h"

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
    void setKeyStatusLabel(QString text);

  private slots:
    bool save_Graph();
    bool load_Graphic_File();
    void load_Graphic_Library();
    void select_Custom_Graph(QString graphName);
    void generate_Graph(enum widget_ID changed_widget);
    void style_Graph(enum widget_ID changed_widget);
    void generate_Combobox_Titles();
    void dumpGraphIc();
    void dumpTikZ();

    void set_Font_Sizes();
    void on_NodeOutlineColor_clicked();
    void on_NodeFillColor_clicked();
    void on_EdgeLineColor_clicked();

    void on_NumLabelCheckBox_clicked(bool checked);
    void on_graphType_ComboBox_currentIndexChanged(int index);
    void on_numOfNodes1_valueChanged(int arg1);
    void on_numOfNodes2_valueChanged(int arg1);

    void nodeParamsUpdated();
    void edgeParamsUpdated();

    void on_deleteMode_radioButton_clicked();

    void on_joinMode_radioButton_clicked();

    void on_editMode_radioButton_clicked();

    void on_dragMode_radioButton_clicked();

    void on_freestyleMode_radioButton_clicked();

    void on_tabWidget_currentChanged(int index);

  private:
    Ui::MainWindow * ui;
    QDir dir;
    QString fileDirectory;
    QGridLayout * gridLayout;
    QScrollArea * scroll;
};

#endif // MAINWINDOW_H
