/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <preview.h>
#include "canvasview.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew_File;
    QAction *actionOpen_File;
    QAction *actionImport_File;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionSelectAll;
    QAction *actionNew_Graph;
    QAction *actionGraph_settings;
    QAction *actionSave;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *newGraph;
    QVBoxLayout *verticalLayout_2;
    QScrollArea *scrollArea_2;
    QWidget *scrollAreaWidgetContents_2;
    QVBoxLayout *verticalLayout_3;
    QGridLayout *gridLayout_4;
    QFrame *line_2;
    QSpinBox *numOfNodes1;
    QLabel *graphLabel;
    QSpinBox *numOfNodes2;
    QCheckBox *complete_checkBox;
    QLabel *rotationLabel;
    QDoubleSpinBox *graphWidth;
    QLabel *partitionLabel;
    QFrame *line_5;
    QLabel *widthLabel;
    QFrame *line_3;
    QLabel *heightLabel;
    QDoubleSpinBox *graphHeight;
    QComboBox *graphType_ComboBox;
    QDoubleSpinBox *graphRotation;
    QGridLayout *gridLayout;
    QDoubleSpinBox *EdgeLabelSize;
    QPushButton *NodeFillColor;
    QLabel *textSizeLabel_2;
    QPushButton *NodeOutlineColor;
    QLineEdit *EdgeLabel;
    QDoubleSpinBox *edgeSize;
    QLabel *textSizeLabel;
    QDoubleSpinBox *nodeSize;
    QFrame *line_4;
    QLabel *colorLabel;
    QLabel *textInputLabel_2;
    QLineEdit *NodeLabel1;
    QFrame *line;
    QLabel *inchesLabel;
    QDoubleSpinBox *NodeLabelSize;
    QLabel *outlineLabel;
    QLineEdit *NodeLabel2;
    QLabel *fillLabel;
    QLabel *nodeLabel;
    QFrame *line_6;
    QPushButton *EdgeLineColor;
    QLabel *ptLabel;
    QLabel *edgeLabel;
    QLabel *textInputLabel;
    QCheckBox *NumLabelCheckBox;
    QSpinBox *NumLabelStart;
    QLabel *numLabel;
    PreView *preview;
    QWidget *editGraph;
    QVBoxLayout *verticalLayout;
    QScrollArea *scrollArea;
    QWidget *scrollAreaWidgetContents;
    QVBoxLayout *verticalLayout_4;
    QHBoxLayout *horizontalLayout;
    QLabel *modeLabel;
    QRadioButton *joinMode_radioButton;
    QRadioButton *editMode_radioButton;
    QRadioButton *deleteMode_radioButton;
    QRadioButton *freestyleMode_radioButton;
    QRadioButton *dragMode_radioButton;
    QCheckBox *snapToGrid_checkBox;
    CanvasView *canvas;
    QMenuBar *menuBar;
    QMenu *menuEdit;
    QMenu *menuFile;
    QMenu *menuSettings;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1400, 920);
        QSizePolicy sizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindow->sizePolicy().hasHeightForWidth());
        MainWindow->setSizePolicy(sizePolicy);
        MainWindow->setMinimumSize(QSize(1200, 650));
        QFont font;
        font.setStrikeOut(false);
        MainWindow->setFont(font);
        MainWindow->setMouseTracking(false);
        MainWindow->setWindowTitle(QString::fromUtf8("Graphic"));
        MainWindow->setAutoFillBackground(false);
        actionNew_File = new QAction(MainWindow);
        actionNew_File->setObjectName(QString::fromUtf8("actionNew_File"));
        actionOpen_File = new QAction(MainWindow);
        actionOpen_File->setObjectName(QString::fromUtf8("actionOpen_File"));
        actionImport_File = new QAction(MainWindow);
        actionImport_File->setObjectName(QString::fromUtf8("actionImport_File"));
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        actionSelectAll = new QAction(MainWindow);
        actionSelectAll->setObjectName(QString::fromUtf8("actionSelectAll"));
        actionNew_Graph = new QAction(MainWindow);
        actionNew_Graph->setObjectName(QString::fromUtf8("actionNew_Graph"));
        actionGraph_settings = new QAction(MainWindow);
        actionGraph_settings->setObjectName(QString::fromUtf8("actionGraph_settings"));
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(centralWidget->sizePolicy().hasHeightForWidth());
        centralWidget->setSizePolicy(sizePolicy1);
        verticalLayout_5 = new QVBoxLayout(centralWidget);
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setContentsMargins(11, 11, 11, 11);
        verticalLayout_5->setObjectName(QString::fromUtf8("verticalLayout_5"));
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
        QSizePolicy sizePolicy2(QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(tabWidget->sizePolicy().hasHeightForWidth());
        tabWidget->setSizePolicy(sizePolicy2);
        tabWidget->setMinimumSize(QSize(650, 0));
        newGraph = new QWidget();
        newGraph->setObjectName(QString::fromUtf8("newGraph"));
        QSizePolicy sizePolicy3(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(newGraph->sizePolicy().hasHeightForWidth());
        newGraph->setSizePolicy(sizePolicy3);
        verticalLayout_2 = new QVBoxLayout(newGraph);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        scrollArea_2 = new QScrollArea(newGraph);
        scrollArea_2->setObjectName(QString::fromUtf8("scrollArea_2"));
        sizePolicy1.setHeightForWidth(scrollArea_2->sizePolicy().hasHeightForWidth());
        scrollArea_2->setSizePolicy(sizePolicy1);
        scrollArea_2->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        scrollArea_2->setWidgetResizable(true);
        scrollAreaWidgetContents_2 = new QWidget();
        scrollAreaWidgetContents_2->setObjectName(QString::fromUtf8("scrollAreaWidgetContents_2"));
        scrollAreaWidgetContents_2->setGeometry(QRect(0, 0, 624, 801));
        sizePolicy2.setHeightForWidth(scrollAreaWidgetContents_2->sizePolicy().hasHeightForWidth());
        scrollAreaWidgetContents_2->setSizePolicy(sizePolicy2);
        verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents_2);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
        gridLayout_4 = new QGridLayout();
        gridLayout_4->setSpacing(6);
        gridLayout_4->setObjectName(QString::fromUtf8("gridLayout_4"));
        line_2 = new QFrame(scrollAreaWidgetContents_2);
        line_2->setObjectName(QString::fromUtf8("line_2"));
        QSizePolicy sizePolicy4(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(line_2->sizePolicy().hasHeightForWidth());
        line_2->setSizePolicy(sizePolicy4);
        line_2->setFrameShape(QFrame::VLine);
        line_2->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line_2, 2, 3, 5, 1);

        numOfNodes1 = new QSpinBox(scrollAreaWidgetContents_2);
        numOfNodes1->setObjectName(QString::fromUtf8("numOfNodes1"));
        QSizePolicy sizePolicy5(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy5.setHorizontalStretch(0);
        sizePolicy5.setVerticalStretch(0);
        sizePolicy5.setHeightForWidth(numOfNodes1->sizePolicy().hasHeightForWidth());
        numOfNodes1->setSizePolicy(sizePolicy5);
        numOfNodes1->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        numOfNodes1->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        numOfNodes1->setMaximum(100);
        numOfNodes1->setValue(5);

        gridLayout_4->addWidget(numOfNodes1, 5, 4, 2, 1, Qt::AlignHCenter);

        graphLabel = new QLabel(scrollAreaWidgetContents_2);
        graphLabel->setObjectName(QString::fromUtf8("graphLabel"));
        QSizePolicy sizePolicy6(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed);
        sizePolicy6.setHorizontalStretch(0);
        sizePolicy6.setVerticalStretch(0);
        sizePolicy6.setHeightForWidth(graphLabel->sizePolicy().hasHeightForWidth());
        graphLabel->setSizePolicy(sizePolicy6);
        QPalette palette;
        QBrush brush(QColor(188, 188, 188, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(180, 180, 180, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush1);
        QBrush brush2(QColor(163, 163, 163, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
        QBrush brush3(QColor(180, 180, 180, 128));
        brush3.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush3);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
        QBrush brush4(QColor(180, 180, 180, 128));
        brush4.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush4);
#endif
        QBrush brush5(QColor(127, 127, 127, 255));
        brush5.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush5);
        QBrush brush6(QColor(148, 148, 148, 255));
        brush6.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush6);
        QBrush brush7(QColor(180, 180, 180, 128));
        brush7.setStyle(Qt::NoBrush);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush7);
#endif
        graphLabel->setPalette(palette);
        QFont font1;
        font1.setPointSize(20);
        font1.setBold(false);
        font1.setWeight(50);
        font1.setStrikeOut(false);
        graphLabel->setFont(font1);
        graphLabel->setScaledContents(false);
        graphLabel->setAlignment(Qt::AlignCenter);
        graphLabel->setTextInteractionFlags(Qt::NoTextInteraction);

        gridLayout_4->addWidget(graphLabel, 0, 1, 2, 8, Qt::AlignHCenter);

        numOfNodes2 = new QSpinBox(scrollAreaWidgetContents_2);
        numOfNodes2->setObjectName(QString::fromUtf8("numOfNodes2"));
        sizePolicy5.setHeightForWidth(numOfNodes2->sizePolicy().hasHeightForWidth());
        numOfNodes2->setSizePolicy(sizePolicy5);
        numOfNodes2->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        numOfNodes2->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        numOfNodes2->setMaximum(100);
        numOfNodes2->setValue(5);

        gridLayout_4->addWidget(numOfNodes2, 5, 5, 2, 1, Qt::AlignHCenter);

        complete_checkBox = new QCheckBox(scrollAreaWidgetContents_2);
        complete_checkBox->setObjectName(QString::fromUtf8("complete_checkBox"));
        sizePolicy6.setHeightForWidth(complete_checkBox->sizePolicy().hasHeightForWidth());
        complete_checkBox->setSizePolicy(sizePolicy6);
        QPalette palette1;
        QBrush brush8(QColor(103, 103, 103, 255));
        brush8.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        QBrush brush9(QColor(120, 120, 120, 255));
        brush9.setStyle(Qt::SolidPattern);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        complete_checkBox->setPalette(palette1);
        QFont font2;
        font2.setPointSize(12);
        font2.setStrikeOut(false);
        complete_checkBox->setFont(font2);
        complete_checkBox->setMouseTracking(false);
        complete_checkBox->setLayoutDirection(Qt::LeftToRight);
        complete_checkBox->setChecked(true);
        complete_checkBox->setTristate(false);

        gridLayout_4->addWidget(complete_checkBox, 4, 4, 1, 2, Qt::AlignHCenter);

        rotationLabel = new QLabel(scrollAreaWidgetContents_2);
        rotationLabel->setObjectName(QString::fromUtf8("rotationLabel"));
        sizePolicy6.setHeightForWidth(rotationLabel->sizePolicy().hasHeightForWidth());
        rotationLabel->setSizePolicy(sizePolicy6);
        QPalette palette2;
        QBrush brush10(QColor(149, 149, 149, 255));
        brush10.setStyle(Qt::SolidPattern);
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        rotationLabel->setPalette(palette2);
        QFont font3;
        font3.setPointSize(18);
        font3.setStrikeOut(false);
        rotationLabel->setFont(font3);
        rotationLabel->setScaledContents(true);
        rotationLabel->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(rotationLabel, 2, 7, 2, 2, Qt::AlignHCenter);

        graphWidth = new QDoubleSpinBox(scrollAreaWidgetContents_2);
        graphWidth->setObjectName(QString::fromUtf8("graphWidth"));
        sizePolicy5.setHeightForWidth(graphWidth->sizePolicy().hasHeightForWidth());
        graphWidth->setSizePolicy(sizePolicy5);
        graphWidth->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        graphWidth->setDecimals(2);
        graphWidth->setMaximum(100.000000000000000);
        graphWidth->setSingleStep(0.500000000000000);
        graphWidth->setValue(2.500000000000000);

        gridLayout_4->addWidget(graphWidth, 5, 2, 2, 1, Qt::AlignHCenter);

        partitionLabel = new QLabel(scrollAreaWidgetContents_2);
        partitionLabel->setObjectName(QString::fromUtf8("partitionLabel"));
        sizePolicy6.setHeightForWidth(partitionLabel->sizePolicy().hasHeightForWidth());
        partitionLabel->setSizePolicy(sizePolicy6);
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        partitionLabel->setPalette(palette3);
        partitionLabel->setFont(font3);
        partitionLabel->setFrameShape(QFrame::NoFrame);
        partitionLabel->setLineWidth(0);
        partitionLabel->setScaledContents(true);
        partitionLabel->setAlignment(Qt::AlignCenter);
        partitionLabel->setWordWrap(true);
        partitionLabel->setMargin(0);

        gridLayout_4->addWidget(partitionLabel, 2, 4, 2, 2, Qt::AlignHCenter);

        line_5 = new QFrame(scrollAreaWidgetContents_2);
        line_5->setObjectName(QString::fromUtf8("line_5"));
        sizePolicy6.setHeightForWidth(line_5->sizePolicy().hasHeightForWidth());
        line_5->setSizePolicy(sizePolicy6);
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line_5, 7, 1, 1, 8);

        widthLabel = new QLabel(scrollAreaWidgetContents_2);
        widthLabel->setObjectName(QString::fromUtf8("widthLabel"));
        sizePolicy6.setHeightForWidth(widthLabel->sizePolicy().hasHeightForWidth());
        widthLabel->setSizePolicy(sizePolicy6);
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        widthLabel->setPalette(palette4);
        widthLabel->setFont(font2);
        widthLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_4->addWidget(widthLabel, 4, 2, 1, 1, Qt::AlignHCenter);

        line_3 = new QFrame(scrollAreaWidgetContents_2);
        line_3->setObjectName(QString::fromUtf8("line_3"));
        sizePolicy4.setHeightForWidth(line_3->sizePolicy().hasHeightForWidth());
        line_3->setSizePolicy(sizePolicy4);
        line_3->setFrameShape(QFrame::VLine);
        line_3->setFrameShadow(QFrame::Sunken);

        gridLayout_4->addWidget(line_3, 2, 6, 5, 1);

        heightLabel = new QLabel(scrollAreaWidgetContents_2);
        heightLabel->setObjectName(QString::fromUtf8("heightLabel"));
        sizePolicy6.setHeightForWidth(heightLabel->sizePolicy().hasHeightForWidth());
        heightLabel->setSizePolicy(sizePolicy6);
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        heightLabel->setPalette(palette5);
        QFont font4;
        font4.setPointSize(12);
        font4.setBold(false);
        font4.setWeight(50);
        font4.setStrikeOut(false);
        heightLabel->setFont(font4);
        heightLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_4->addWidget(heightLabel, 4, 1, 1, 1, Qt::AlignHCenter);

        graphHeight = new QDoubleSpinBox(scrollAreaWidgetContents_2);
        graphHeight->setObjectName(QString::fromUtf8("graphHeight"));
        sizePolicy5.setHeightForWidth(graphHeight->sizePolicy().hasHeightForWidth());
        graphHeight->setSizePolicy(sizePolicy5);
        graphHeight->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        graphHeight->setDecimals(2);
        graphHeight->setMaximum(100.000000000000000);
        graphHeight->setSingleStep(0.500000000000000);
        graphHeight->setValue(2.500000000000000);

        gridLayout_4->addWidget(graphHeight, 5, 1, 2, 1, Qt::AlignHCenter);

        graphType_ComboBox = new QComboBox(scrollAreaWidgetContents_2);
        graphType_ComboBox->addItem(QString());
        graphType_ComboBox->setObjectName(QString::fromUtf8("graphType_ComboBox"));
        QSizePolicy sizePolicy7(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy7.setHorizontalStretch(0);
        sizePolicy7.setVerticalStretch(0);
        sizePolicy7.setHeightForWidth(graphType_ComboBox->sizePolicy().hasHeightForWidth());
        graphType_ComboBox->setSizePolicy(sizePolicy7);
        graphType_ComboBox->setMinimumSize(QSize(250, 0));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        graphType_ComboBox->setPalette(palette6);
        graphType_ComboBox->setFont(font2);
        graphType_ComboBox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLength);
        graphType_ComboBox->setFrame(false);
        graphType_ComboBox->setModelColumn(0);

        gridLayout_4->addWidget(graphType_ComboBox, 2, 1, 2, 2, Qt::AlignHCenter);

        graphRotation = new QDoubleSpinBox(scrollAreaWidgetContents_2);
        graphRotation->setObjectName(QString::fromUtf8("graphRotation"));
        sizePolicy5.setHeightForWidth(graphRotation->sizePolicy().hasHeightForWidth());
        graphRotation->setSizePolicy(sizePolicy5);
        graphRotation->setFont(font);
        graphRotation->setWrapping(true);
        graphRotation->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        graphRotation->setDecimals(1);
        graphRotation->setMaximum(360.000000000000000);

        gridLayout_4->addWidget(graphRotation, 5, 7, 2, 1);


        verticalLayout_3->addLayout(gridLayout_4);

        gridLayout = new QGridLayout();
        gridLayout->setSpacing(6);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        EdgeLabelSize = new QDoubleSpinBox(scrollAreaWidgetContents_2);
        EdgeLabelSize->setObjectName(QString::fromUtf8("EdgeLabelSize"));
        sizePolicy5.setHeightForWidth(EdgeLabelSize->sizePolicy().hasHeightForWidth());
        EdgeLabelSize->setSizePolicy(sizePolicy5);
        EdgeLabelSize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        EdgeLabelSize->setDecimals(0);
        EdgeLabelSize->setMinimum(1.000000000000000);
        EdgeLabelSize->setValue(12.000000000000000);

        gridLayout->addWidget(EdgeLabelSize, 2, 2, 1, 1, Qt::AlignHCenter);

        NodeFillColor = new QPushButton(scrollAreaWidgetContents_2);
        NodeFillColor->setObjectName(QString::fromUtf8("NodeFillColor"));
        sizePolicy5.setHeightForWidth(NodeFillColor->sizePolicy().hasHeightForWidth());
        NodeFillColor->setSizePolicy(sizePolicy5);
        NodeFillColor->setMinimumSize(QSize(50, 0));

        gridLayout->addWidget(NodeFillColor, 3, 7, 1, 1, Qt::AlignHCenter);

        textSizeLabel_2 = new QLabel(scrollAreaWidgetContents_2);
        textSizeLabel_2->setObjectName(QString::fromUtf8("textSizeLabel_2"));
        sizePolicy6.setHeightForWidth(textSizeLabel_2->sizePolicy().hasHeightForWidth());
        textSizeLabel_2->setSizePolicy(sizePolicy6);
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        textSizeLabel_2->setPalette(palette7);
        textSizeLabel_2->setFont(font2);

        gridLayout->addWidget(textSizeLabel_2, 2, 9, 1, 2, Qt::AlignHCenter);

        NodeOutlineColor = new QPushButton(scrollAreaWidgetContents_2);
        NodeOutlineColor->setObjectName(QString::fromUtf8("NodeOutlineColor"));
        sizePolicy5.setHeightForWidth(NodeOutlineColor->sizePolicy().hasHeightForWidth());
        NodeOutlineColor->setSizePolicy(sizePolicy5);
        NodeOutlineColor->setMinimumSize(QSize(50, 0));

        gridLayout->addWidget(NodeOutlineColor, 2, 7, 1, 1, Qt::AlignHCenter);

        EdgeLabel = new QLineEdit(scrollAreaWidgetContents_2);
        EdgeLabel->setObjectName(QString::fromUtf8("EdgeLabel"));
        sizePolicy5.setHeightForWidth(EdgeLabel->sizePolicy().hasHeightForWidth());
        EdgeLabel->setSizePolicy(sizePolicy5);
        EdgeLabel->setMaximumSize(QSize(65, 16777215));
        EdgeLabel->setMouseTracking(false);
        EdgeLabel->setFrame(false);
        EdgeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(EdgeLabel, 1, 2, 1, 1, Qt::AlignHCenter);

        edgeSize = new QDoubleSpinBox(scrollAreaWidgetContents_2);
        edgeSize->setObjectName(QString::fromUtf8("edgeSize"));
        sizePolicy5.setHeightForWidth(edgeSize->sizePolicy().hasHeightForWidth());
        edgeSize->setSizePolicy(sizePolicy5);
        edgeSize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        edgeSize->setDecimals(1);
        edgeSize->setSingleStep(0.500000000000000);
        edgeSize->setValue(1.000000000000000);

        gridLayout->addWidget(edgeSize, 3, 2, 1, 1, Qt::AlignHCenter);

        textSizeLabel = new QLabel(scrollAreaWidgetContents_2);
        textSizeLabel->setObjectName(QString::fromUtf8("textSizeLabel"));
        sizePolicy6.setHeightForWidth(textSizeLabel->sizePolicy().hasHeightForWidth());
        textSizeLabel->setSizePolicy(sizePolicy6);
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        textSizeLabel->setPalette(palette8);
        textSizeLabel->setFont(font2);
        textSizeLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(textSizeLabel, 2, 0, 1, 2, Qt::AlignHCenter);

        nodeSize = new QDoubleSpinBox(scrollAreaWidgetContents_2);
        nodeSize->setObjectName(QString::fromUtf8("nodeSize"));
        sizePolicy5.setHeightForWidth(nodeSize->sizePolicy().hasHeightForWidth());
        nodeSize->setSizePolicy(sizePolicy5);
        nodeSize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        nodeSize->setProperty("showGroupSeparator", QVariant(true));
        nodeSize->setDecimals(2);
        nodeSize->setSingleStep(0.050000000000000);
        nodeSize->setValue(0.200000000000000);

        gridLayout->addWidget(nodeSize, 3, 11, 1, 1, Qt::AlignHCenter);

        line_4 = new QFrame(scrollAreaWidgetContents_2);
        line_4->setObjectName(QString::fromUtf8("line_4"));
        line_4->setFrameShape(QFrame::VLine);
        line_4->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_4, 0, 3, 4, 1);

        colorLabel = new QLabel(scrollAreaWidgetContents_2);
        colorLabel->setObjectName(QString::fromUtf8("colorLabel"));
        sizePolicy6.setHeightForWidth(colorLabel->sizePolicy().hasHeightForWidth());
        colorLabel->setSizePolicy(sizePolicy6);
        QPalette palette9;
        palette9.setBrush(QPalette::Active, QPalette::WindowText, brush10);
        palette9.setBrush(QPalette::Inactive, QPalette::WindowText, brush10);
        palette9.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        colorLabel->setPalette(palette9);
        colorLabel->setFont(font3);
        colorLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(colorLabel, 1, 4, 1, 4, Qt::AlignHCenter);

        textInputLabel_2 = new QLabel(scrollAreaWidgetContents_2);
        textInputLabel_2->setObjectName(QString::fromUtf8("textInputLabel_2"));
        sizePolicy6.setHeightForWidth(textInputLabel_2->sizePolicy().hasHeightForWidth());
        textInputLabel_2->setSizePolicy(sizePolicy6);
        QPalette palette10;
        palette10.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette10.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette10.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        textInputLabel_2->setPalette(palette10);
        textInputLabel_2->setFont(font2);

        gridLayout->addWidget(textInputLabel_2, 1, 9, 1, 2, Qt::AlignHCenter);

        NodeLabel1 = new QLineEdit(scrollAreaWidgetContents_2);
        NodeLabel1->setObjectName(QString::fromUtf8("NodeLabel1"));
        sizePolicy5.setHeightForWidth(NodeLabel1->sizePolicy().hasHeightForWidth());
        NodeLabel1->setSizePolicy(sizePolicy5);
        NodeLabel1->setMaximumSize(QSize(53, 16777215));
        NodeLabel1->setMouseTracking(false);
        NodeLabel1->setMaxLength(1);
        NodeLabel1->setFrame(false);
        NodeLabel1->setAlignment(Qt::AlignCenter);
        NodeLabel1->setClearButtonEnabled(false);

        gridLayout->addWidget(NodeLabel1, 1, 11, 1, 1, Qt::AlignHCenter);

        line = new QFrame(scrollAreaWidgetContents_2);
        line->setObjectName(QString::fromUtf8("line"));
        sizePolicy7.setHeightForWidth(line->sizePolicy().hasHeightForWidth());
        line->setSizePolicy(sizePolicy7);
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 4, 0, 1, 14);

        inchesLabel = new QLabel(scrollAreaWidgetContents_2);
        inchesLabel->setObjectName(QString::fromUtf8("inchesLabel"));
        sizePolicy6.setHeightForWidth(inchesLabel->sizePolicy().hasHeightForWidth());
        inchesLabel->setSizePolicy(sizePolicy6);
        QPalette palette11;
        palette11.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette11.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette11.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        inchesLabel->setPalette(palette11);
        inchesLabel->setFont(font2);
        inchesLabel->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout->addWidget(inchesLabel, 3, 9, 1, 2, Qt::AlignHCenter);

        NodeLabelSize = new QDoubleSpinBox(scrollAreaWidgetContents_2);
        NodeLabelSize->setObjectName(QString::fromUtf8("NodeLabelSize"));
        sizePolicy5.setHeightForWidth(NodeLabelSize->sizePolicy().hasHeightForWidth());
        NodeLabelSize->setSizePolicy(sizePolicy5);
        NodeLabelSize->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        NodeLabelSize->setDecimals(0);
        NodeLabelSize->setMinimum(1.000000000000000);
        NodeLabelSize->setValue(12.000000000000000);

        gridLayout->addWidget(NodeLabelSize, 2, 11, 1, 1, Qt::AlignHCenter);

        outlineLabel = new QLabel(scrollAreaWidgetContents_2);
        outlineLabel->setObjectName(QString::fromUtf8("outlineLabel"));
        sizePolicy6.setHeightForWidth(outlineLabel->sizePolicy().hasHeightForWidth());
        outlineLabel->setSizePolicy(sizePolicy6);
        QPalette palette12;
        palette12.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette12.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette12.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        outlineLabel->setPalette(palette12);
        outlineLabel->setFont(font2);
        outlineLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(outlineLabel, 2, 5, 1, 2, Qt::AlignHCenter);

        NodeLabel2 = new QLineEdit(scrollAreaWidgetContents_2);
        NodeLabel2->setObjectName(QString::fromUtf8("NodeLabel2"));
        sizePolicy5.setHeightForWidth(NodeLabel2->sizePolicy().hasHeightForWidth());
        NodeLabel2->setSizePolicy(sizePolicy5);
        NodeLabel2->setMaximumSize(QSize(53, 16777215));
        NodeLabel2->setMouseTracking(false);
        NodeLabel2->setMaxLength(1);
        NodeLabel2->setFrame(false);
        NodeLabel2->setAlignment(Qt::AlignCenter);
        NodeLabel2->setClearButtonEnabled(false);

        gridLayout->addWidget(NodeLabel2, 1, 12, 1, 1, Qt::AlignHCenter);

        fillLabel = new QLabel(scrollAreaWidgetContents_2);
        fillLabel->setObjectName(QString::fromUtf8("fillLabel"));
        sizePolicy6.setHeightForWidth(fillLabel->sizePolicy().hasHeightForWidth());
        fillLabel->setSizePolicy(sizePolicy6);
        QPalette palette13;
        palette13.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette13.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette13.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        fillLabel->setPalette(palette13);
        fillLabel->setFont(font2);
        fillLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(fillLabel, 3, 5, 1, 2, Qt::AlignHCenter);

        nodeLabel = new QLabel(scrollAreaWidgetContents_2);
        nodeLabel->setObjectName(QString::fromUtf8("nodeLabel"));
        sizePolicy6.setHeightForWidth(nodeLabel->sizePolicy().hasHeightForWidth());
        nodeLabel->setSizePolicy(sizePolicy6);
        QPalette palette14;
        palette14.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette14.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        nodeLabel->setPalette(palette14);
        QFont font5;
        font5.setPointSize(20);
        font5.setStrikeOut(false);
        nodeLabel->setFont(font5);
        nodeLabel->setLayoutDirection(Qt::LeftToRight);
        nodeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(nodeLabel, 0, 9, 1, 5);

        line_6 = new QFrame(scrollAreaWidgetContents_2);
        line_6->setObjectName(QString::fromUtf8("line_6"));
        line_6->setFrameShape(QFrame::VLine);
        line_6->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line_6, 0, 8, 4, 1);

        EdgeLineColor = new QPushButton(scrollAreaWidgetContents_2);
        EdgeLineColor->setObjectName(QString::fromUtf8("EdgeLineColor"));
        sizePolicy5.setHeightForWidth(EdgeLineColor->sizePolicy().hasHeightForWidth());
        EdgeLineColor->setSizePolicy(sizePolicy5);
        EdgeLineColor->setMinimumSize(QSize(50, 0));

        gridLayout->addWidget(EdgeLineColor, 2, 4, 1, 1, Qt::AlignHCenter);

        ptLabel = new QLabel(scrollAreaWidgetContents_2);
        ptLabel->setObjectName(QString::fromUtf8("ptLabel"));
        sizePolicy6.setHeightForWidth(ptLabel->sizePolicy().hasHeightForWidth());
        ptLabel->setSizePolicy(sizePolicy6);
        QPalette palette15;
        palette15.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette15.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette15.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        ptLabel->setPalette(palette15);
        ptLabel->setFont(font2);

        gridLayout->addWidget(ptLabel, 3, 0, 1, 2, Qt::AlignHCenter);

        edgeLabel = new QLabel(scrollAreaWidgetContents_2);
        edgeLabel->setObjectName(QString::fromUtf8("edgeLabel"));
        sizePolicy6.setHeightForWidth(edgeLabel->sizePolicy().hasHeightForWidth());
        edgeLabel->setSizePolicy(sizePolicy6);
        QPalette palette16;
        palette16.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette16.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette16.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        edgeLabel->setPalette(palette16);
        edgeLabel->setFont(font5);
        edgeLabel->setAlignment(Qt::AlignCenter);

        gridLayout->addWidget(edgeLabel, 0, 0, 1, 3, Qt::AlignHCenter);

        textInputLabel = new QLabel(scrollAreaWidgetContents_2);
        textInputLabel->setObjectName(QString::fromUtf8("textInputLabel"));
        sizePolicy6.setHeightForWidth(textInputLabel->sizePolicy().hasHeightForWidth());
        textInputLabel->setSizePolicy(sizePolicy6);
        QPalette palette17;
        palette17.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette17.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette17.setBrush(QPalette::Disabled, QPalette::WindowText, brush5);
        textInputLabel->setPalette(palette17);
        textInputLabel->setFont(font2);

        gridLayout->addWidget(textInputLabel, 1, 0, 1, 2, Qt::AlignHCenter);

        NumLabelCheckBox = new QCheckBox(scrollAreaWidgetContents_2);
        NumLabelCheckBox->setObjectName(QString::fromUtf8("NumLabelCheckBox"));
        sizePolicy6.setHeightForWidth(NumLabelCheckBox->sizePolicy().hasHeightForWidth());
        NumLabelCheckBox->setSizePolicy(sizePolicy6);
        QPalette palette18;
        palette18.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette18.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette18.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        NumLabelCheckBox->setPalette(palette18);
        QFont font6;
        font6.setPointSize(10);
        font6.setStrikeOut(false);
        NumLabelCheckBox->setFont(font6);
        NumLabelCheckBox->setFocusPolicy(Qt::TabFocus);
        NumLabelCheckBox->setLayoutDirection(Qt::LeftToRight);
        NumLabelCheckBox->setTristate(false);

        gridLayout->addWidget(NumLabelCheckBox, 2, 12, 1, 2, Qt::AlignHCenter);

        NumLabelStart = new QSpinBox(scrollAreaWidgetContents_2);
        NumLabelStart->setObjectName(QString::fromUtf8("NumLabelStart"));
        sizePolicy5.setHeightForWidth(NumLabelStart->sizePolicy().hasHeightForWidth());
        NumLabelStart->setSizePolicy(sizePolicy5);
        NumLabelStart->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        NumLabelStart->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
        NumLabelStart->setMaximum(100);

        gridLayout->addWidget(NumLabelStart, 3, 13, 1, 1, Qt::AlignHCenter);

        numLabel = new QLabel(scrollAreaWidgetContents_2);
        numLabel->setObjectName(QString::fromUtf8("numLabel"));
        sizePolicy6.setHeightForWidth(numLabel->sizePolicy().hasHeightForWidth());
        numLabel->setSizePolicy(sizePolicy6);
        QPalette palette19;
        palette19.setBrush(QPalette::Active, QPalette::WindowText, brush8);
        palette19.setBrush(QPalette::Inactive, QPalette::WindowText, brush8);
        palette19.setBrush(QPalette::Disabled, QPalette::WindowText, brush9);
        numLabel->setPalette(palette19);
        numLabel->setFont(font2);

        gridLayout->addWidget(numLabel, 3, 12, 1, 1, Qt::AlignHCenter);


        verticalLayout_3->addLayout(gridLayout);

        preview = new PreView(scrollAreaWidgetContents_2);
        preview->setObjectName(QString::fromUtf8("preview"));
        sizePolicy.setHeightForWidth(preview->sizePolicy().hasHeightForWidth());
        preview->setSizePolicy(sizePolicy);
        preview->setMinimumSize(QSize(500, 400));
        preview->setCursor(QCursor(Qt::CrossCursor));
        preview->setMouseTracking(false);
        preview->setAutoFillBackground(false);

        verticalLayout_3->addWidget(preview);

        scrollArea_2->setWidget(scrollAreaWidgetContents_2);

        verticalLayout_2->addWidget(scrollArea_2);

        tabWidget->addTab(newGraph, QString());
        editGraph = new QWidget();
        editGraph->setObjectName(QString::fromUtf8("editGraph"));
        verticalLayout = new QVBoxLayout(editGraph);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        scrollArea = new QScrollArea(editGraph);
        scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
        scrollArea->setWidgetResizable(true);
        scrollAreaWidgetContents = new QWidget();
        scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
        scrollAreaWidgetContents->setGeometry(QRect(0, 0, 624, 801));
        scrollArea->setWidget(scrollAreaWidgetContents);

        verticalLayout->addWidget(scrollArea);

        tabWidget->addTab(editGraph, QString());

        horizontalLayout_2->addWidget(tabWidget);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QString::fromUtf8("verticalLayout_4"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        modeLabel = new QLabel(centralWidget);
        modeLabel->setObjectName(QString::fromUtf8("modeLabel"));
        QSizePolicy sizePolicy8(QSizePolicy::Maximum, QSizePolicy::Maximum);
        sizePolicy8.setHorizontalStretch(0);
        sizePolicy8.setVerticalStretch(0);
        sizePolicy8.setHeightForWidth(modeLabel->sizePolicy().hasHeightForWidth());
        modeLabel->setSizePolicy(sizePolicy8);

        horizontalLayout->addWidget(modeLabel);

        joinMode_radioButton = new QRadioButton(centralWidget);
        joinMode_radioButton->setObjectName(QString::fromUtf8("joinMode_radioButton"));
        sizePolicy8.setHeightForWidth(joinMode_radioButton->sizePolicy().hasHeightForWidth());
        joinMode_radioButton->setSizePolicy(sizePolicy8);
        joinMode_radioButton->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(joinMode_radioButton);

        editMode_radioButton = new QRadioButton(centralWidget);
        editMode_radioButton->setObjectName(QString::fromUtf8("editMode_radioButton"));
        sizePolicy8.setHeightForWidth(editMode_radioButton->sizePolicy().hasHeightForWidth());
        editMode_radioButton->setSizePolicy(sizePolicy8);
        editMode_radioButton->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(editMode_radioButton);

        deleteMode_radioButton = new QRadioButton(centralWidget);
        deleteMode_radioButton->setObjectName(QString::fromUtf8("deleteMode_radioButton"));
        sizePolicy8.setHeightForWidth(deleteMode_radioButton->sizePolicy().hasHeightForWidth());
        deleteMode_radioButton->setSizePolicy(sizePolicy8);
        deleteMode_radioButton->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(deleteMode_radioButton);

        freestyleMode_radioButton = new QRadioButton(centralWidget);
        freestyleMode_radioButton->setObjectName(QString::fromUtf8("freestyleMode_radioButton"));
        sizePolicy8.setHeightForWidth(freestyleMode_radioButton->sizePolicy().hasHeightForWidth());
        freestyleMode_radioButton->setSizePolicy(sizePolicy8);
        freestyleMode_radioButton->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(freestyleMode_radioButton);

        dragMode_radioButton = new QRadioButton(centralWidget);
        dragMode_radioButton->setObjectName(QString::fromUtf8("dragMode_radioButton"));
        sizePolicy8.setHeightForWidth(dragMode_radioButton->sizePolicy().hasHeightForWidth());
        dragMode_radioButton->setSizePolicy(sizePolicy8);
        dragMode_radioButton->setLayoutDirection(Qt::LeftToRight);

        horizontalLayout->addWidget(dragMode_radioButton);

        snapToGrid_checkBox = new QCheckBox(centralWidget);
        snapToGrid_checkBox->setObjectName(QString::fromUtf8("snapToGrid_checkBox"));
        QSizePolicy sizePolicy9(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy9.setHorizontalStretch(0);
        sizePolicy9.setVerticalStretch(0);
        sizePolicy9.setHeightForWidth(snapToGrid_checkBox->sizePolicy().hasHeightForWidth());
        snapToGrid_checkBox->setSizePolicy(sizePolicy9);
        snapToGrid_checkBox->setLayoutDirection(Qt::LeftToRight);
        snapToGrid_checkBox->setChecked(true);

        horizontalLayout->addWidget(snapToGrid_checkBox, 0, Qt::AlignRight);


        verticalLayout_4->addLayout(horizontalLayout);

        canvas = new CanvasView(centralWidget);
        canvas->setObjectName(QString::fromUtf8("canvas"));
        sizePolicy.setHeightForWidth(canvas->sizePolicy().hasHeightForWidth());
        canvas->setSizePolicy(sizePolicy);
        canvas->setMinimumSize(QSize(400, 500));
        canvas->setCursor(QCursor(Qt::CrossCursor));
        canvas->setMouseTracking(false);
        canvas->setAcceptDrops(true);
        canvas->setLayoutDirection(Qt::RightToLeft);

        verticalLayout_4->addWidget(canvas);


        horizontalLayout_2->addLayout(verticalLayout_4);


        verticalLayout_5->addLayout(horizontalLayout_2);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1400, 21));
        sizePolicy.setHeightForWidth(menuBar->sizePolicy().hasHeightForWidth());
        menuBar->setSizePolicy(sizePolicy);
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuSettings = new QMenu(menuBar);
        menuSettings->setObjectName(QString::fromUtf8("menuSettings"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        sizePolicy.setHeightForWidth(mainToolBar->sizePolicy().hasHeightForWidth());
        mainToolBar->setSizePolicy(sizePolicy);
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        sizePolicy.setHeightForWidth(statusBar->sizePolicy().hasHeightForWidth());
        statusBar->setSizePolicy(sizePolicy);
        MainWindow->setStatusBar(statusBar);
#if QT_CONFIG(shortcut)
        widthLabel->setBuddy(graphWidth);
        heightLabel->setBuddy(graphHeight);
        textSizeLabel_2->setBuddy(NodeLabelSize);
        textSizeLabel->setBuddy(EdgeLabelSize);
        textInputLabel_2->setBuddy(NodeLabel1);
        inchesLabel->setBuddy(nodeSize);
        outlineLabel->setBuddy(NodeOutlineColor);
        fillLabel->setBuddy(NodeFillColor);
        ptLabel->setBuddy(edgeSize);
        textInputLabel->setBuddy(EdgeLabel);
        numLabel->setBuddy(NumLabelStart);
#endif // QT_CONFIG(shortcut)

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuSettings->menuAction());
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addAction(actionSelectAll);
        menuFile->addAction(actionNew_File);
        menuFile->addAction(actionOpen_File);
        menuFile->addAction(actionSave);
        menuSettings->addAction(actionGraph_settings);

        retranslateUi(MainWindow);
        QObject::connect(complete_checkBox, SIGNAL(stateChanged(int)), dragMode_radioButton, SLOT(click()));
        QObject::connect(NodeFillColor, SIGNAL(clicked()), dragMode_radioButton, SLOT(click()));
        QObject::connect(EdgeLabel, SIGNAL(textChanged(QString)), dragMode_radioButton, SLOT(click()));
        QObject::connect(NodeLabel2, SIGNAL(textChanged(QString)), dragMode_radioButton, SLOT(click()));
        QObject::connect(graphType_ComboBox, SIGNAL(currentIndexChanged(int)), dragMode_radioButton, SLOT(click()));
        QObject::connect(EdgeLineColor, SIGNAL(clicked()), dragMode_radioButton, SLOT(click()));
        QObject::connect(numOfNodes1, SIGNAL(valueChanged(int)), dragMode_radioButton, SLOT(click()));
        QObject::connect(numOfNodes2, SIGNAL(valueChanged(int)), dragMode_radioButton, SLOT(click()));
        QObject::connect(EdgeLabelSize, SIGNAL(valueChanged(double)), dragMode_radioButton, SLOT(click()));
        QObject::connect(nodeSize, SIGNAL(valueChanged(double)), dragMode_radioButton, SLOT(click()));
        QObject::connect(NodeLabelSize, SIGNAL(valueChanged(double)), dragMode_radioButton, SLOT(click()));
        QObject::connect(NodeOutlineColor, SIGNAL(clicked()), dragMode_radioButton, SLOT(click()));
        QObject::connect(NodeLabel1, SIGNAL(textChanged(QString)), dragMode_radioButton, SLOT(click()));
        QObject::connect(graphWidth, SIGNAL(valueChanged(double)), dragMode_radioButton, SLOT(click()));
        QObject::connect(NumLabelCheckBox, SIGNAL(stateChanged(int)), dragMode_radioButton, SLOT(click()));
        QObject::connect(graphHeight, SIGNAL(valueChanged(double)), dragMode_radioButton, SLOT(click()));
        QObject::connect(graphRotation, SIGNAL(valueChanged(double)), dragMode_radioButton, SLOT(click()));
        QObject::connect(edgeSize, SIGNAL(valueChanged(double)), dragMode_radioButton, SLOT(click()));

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        actionNew_File->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
        actionOpen_File->setText(QCoreApplication::translate("MainWindow", "Open ", nullptr));
        actionImport_File->setText(QCoreApplication::translate("MainWindow", "Import ", nullptr));
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
        actionSelectAll->setText(QCoreApplication::translate("MainWindow", "Select All", nullptr));
        actionNew_Graph->setText(QCoreApplication::translate("MainWindow", "New Graph", nullptr));
        actionGraph_settings->setText(QCoreApplication::translate("MainWindow", "Graph settings", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(tooltip)
        numOfNodes1->setToolTip(QCoreApplication::translate("MainWindow", "For most graph types, the number of nodes.", nullptr));
#endif // QT_CONFIG(tooltip)
        graphLabel->setText(QCoreApplication::translate("MainWindow", "GRAPH", nullptr));
#if QT_CONFIG(tooltip)
        numOfNodes2->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(tooltip)
        complete_checkBox->setToolTip(QCoreApplication::translate("MainWindow", "Uncheck this to only draw the nodes.", nullptr));
#endif // QT_CONFIG(tooltip)
        complete_checkBox->setText(QCoreApplication::translate("MainWindow", "Draw edges", nullptr));
        rotationLabel->setText(QCoreApplication::translate("MainWindow", "Rotation", nullptr));
        partitionLabel->setText(QCoreApplication::translate("MainWindow", "Partitions", nullptr));
        widthLabel->setText(QCoreApplication::translate("MainWindow", "Width", nullptr));
        heightLabel->setText(QCoreApplication::translate("MainWindow", "Height", nullptr));
        graphType_ComboBox->setItemText(0, QCoreApplication::translate("MainWindow", "Select Graph Type", nullptr));

        NodeFillColor->setText(QString());
        textSizeLabel_2->setText(QCoreApplication::translate("MainWindow", "Label size", nullptr));
        NodeOutlineColor->setText(QString());
        EdgeLabel->setInputMask(QString());
        EdgeLabel->setText(QString());
        EdgeLabel->setPlaceholderText(QCoreApplication::translate("MainWindow", "weight", nullptr));
        textSizeLabel->setText(QCoreApplication::translate("MainWindow", "Label size", nullptr));
        nodeSize->setSpecialValueText(QString());
        colorLabel->setText(QCoreApplication::translate("MainWindow", "Colour", nullptr));
        textInputLabel_2->setText(QCoreApplication::translate("MainWindow", "Label text", nullptr));
        NodeLabel1->setPlaceholderText(QCoreApplication::translate("MainWindow", "u", nullptr));
        inchesLabel->setText(QCoreApplication::translate("MainWindow", "Node size", nullptr));
        outlineLabel->setText(QCoreApplication::translate("MainWindow", "Outline", nullptr));
        NodeLabel2->setPlaceholderText(QCoreApplication::translate("MainWindow", "v", nullptr));
        fillLabel->setText(QCoreApplication::translate("MainWindow", "Fill", nullptr));
        nodeLabel->setText(QCoreApplication::translate("MainWindow", "NODE", nullptr));
        EdgeLineColor->setText(QString());
        ptLabel->setText(QCoreApplication::translate("MainWindow", "Edge size", nullptr));
        edgeLabel->setText(QCoreApplication::translate("MainWindow", "EDGE", nullptr));
        textInputLabel->setText(QCoreApplication::translate("MainWindow", "Label text", nullptr));
        NumLabelCheckBox->setText(QCoreApplication::translate("MainWindow", "Numbers", nullptr));
#if QT_CONFIG(tooltip)
        NumLabelStart->setToolTip(QString());
#endif // QT_CONFIG(tooltip)
        numLabel->setText(QCoreApplication::translate("MainWindow", "Start #", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(newGraph), QCoreApplication::translate("MainWindow", "Create Graph", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(editGraph), QCoreApplication::translate("MainWindow", "Edit Graph", nullptr));
        modeLabel->setText(QCoreApplication::translate("MainWindow", "Modes:", nullptr));
#if QT_CONFIG(tooltip)
        joinMode_radioButton->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Select up to 2 nodes from 2 different graph components and press 'J' to join.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        joinMode_radioButton->setText(QCoreApplication::translate("MainWindow", "Join", nullptr));
#if QT_CONFIG(tooltip)
        editMode_radioButton->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Drag individual nodes around the canvas. Use 'Esc' key to undo one or more node drags. Click on a node to give it a label.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        editMode_radioButton->setText(QCoreApplication::translate("MainWindow", "Edit", nullptr));
#if QT_CONFIG(tooltip)
        deleteMode_radioButton->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Click on any node or edge to be deleted. Double click on a graph to delete it entirely.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        deleteMode_radioButton->setText(QCoreApplication::translate("MainWindow", "Delete", nullptr));
#if QT_CONFIG(tooltip)
        freestyleMode_radioButton->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Add nodes by double clicking at the desired location. Add a path between existing nodes by clicking on two or more nodes consecutively.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        freestyleMode_radioButton->setText(QCoreApplication::translate("MainWindow", "Freestyle", nullptr));
#if QT_CONFIG(tooltip)
        dragMode_radioButton->setToolTip(QCoreApplication::translate("MainWindow", "<html><head/><body><p>Hold the mouse down over any part of a graph and move the mouse to drag that graph around the canvas.</p></body></html>", nullptr));
#endif // QT_CONFIG(tooltip)
        dragMode_radioButton->setText(QCoreApplication::translate("MainWindow", "Drag", nullptr));
        snapToGrid_checkBox->setText(QCoreApplication::translate("MainWindow", "Snap to Grid", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuSettings->setTitle(QCoreApplication::translate("MainWindow", "Settings", nullptr));
        (void)MainWindow;
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
