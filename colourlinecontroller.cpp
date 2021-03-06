/*
 * File:    colourlinecontroller.cpp
 * Author:  Rachel Bood 100088769
 * Date:    2014 (?)
 * Version: 1.3
 *
 * Purpose:
 *
 * Modification history:
 * Jul 9, 2020 (IC V1.1)
 *  (a) BUTTON_STYLE moved to defuns.h.
 * Oct 18, 2020 (JD V1.2)
 *  (a) Code tidying, spelling corrections, code simplifications.
 * Nov 6, 2020 (JD V1.3)
 *  (a) Fix bug in setNodeOutlineColour() and setEdgeLineColour()
 *	which would clobber the old colour when getColor() was called
 *	and then cancelled, replacing the old colour with black.
 */

#include "colourlinecontroller.h"

#include <QColorDialog>
#include <QtCore>


ColourLineController::ColourLineController(Edge * anEdge, QPushButton * aButton)
{
    edge = anEdge;
    button = aButton;
    if (button != nullptr || button != 0)
    {
        QColor colour = edge->getColour();
	QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
        button->setStyleSheet(s);

        connect(button, SIGNAL (clicked(bool)),
		this, SLOT(setEdgeLineColour()));
        connect(anEdge, SIGNAL(destroyed(QObject*)),
                this, SLOT(deleteButton()));
        connect(anEdge, SIGNAL(destroyed(QObject*)),
                this, SLOT(deleteLater()));
    }
}



ColourLineController::ColourLineController(Node * aNode, QPushButton * aButton)
{
    node = aNode;
    button = aButton;
    if (button != nullptr || button != 0)
    {
        QColor colour = node->getLineColour();
	QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
        button->setStyleSheet(s);

        connect(button, SIGNAL (clicked(bool)),
                this, SLOT(setNodeOutlineColour()));
        connect(aNode, SIGNAL(destroyed(QObject*)),
                this, SLOT(deleteButton()));
        connect(aNode, SIGNAL(destroyed(QObject*)),
                this, SLOT(deleteLater()));
    }

}


// The original code always set the button stylesheet.  Do we want that?
void
ColourLineController::setEdgeLineColour()
{
    if (edge != 0 || edge != nullptr)
    {
	QColor colour = QColorDialog::getColor();
	if (colour.isValid())
	{
	    QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
	    button->setStyleSheet(s);
	    edge->setColour(colour);
	}
    }
}



// The original code always set the button stylesheet.  Do we want that?
void
ColourLineController::setNodeOutlineColour()
{
    if (node != 0 || node != nullptr)
    {
	QColor colour = QColorDialog::getColor();
	if (colour.isValid())
	{
	    QString s("background: " + colour.name() + "; " + BUTTON_STYLE);
	    button->setStyleSheet(s);
	    node->setLineColour(colour);
	}
    }
}



void
ColourLineController::deleteButton()
{
    delete button;
}
