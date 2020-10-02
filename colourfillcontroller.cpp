/*
 * File:    colourfillcontroller.cpp
 * Author:  Rachel Bood 100088769
 * Date:    2014 (?)
 * Version: 1.1
 *
 * Purpose:
 *
 * Modification history:
 * Jul 9, 2020 (IC V1.1)
 *  (a) BUTTON_STYLE moved to defuns.h.
 */

#include "colourfillcontroller.h"
#include <QColorDialog>

ColorFillController::ColorFillController(Node *aNode, QPushButton *aButton)
{
        node = aNode;
        button = aButton;
        if (button != nullptr || button != 0)
        {
            QColor color = node->getFillColour();
            QString s("background: #"
                      + QString(color.red() < 16? "0" : "")
                      + QString::number(color.red(),16)
                      + QString(color.green() < 16? "0" : "")
                      + QString::number(color.green(),16)
                      + QString(color.blue() < 16? "0" : "")
                      + QString::number(color.blue(),16) + ";"
                      BUTTON_STYLE);
            button->setStyleSheet(s);

            connect(button, SIGNAL (clicked(bool)),
                    this, SLOT(setNodeFillColor()));
            connect(aNode, SIGNAL(destroyed(QObject*)),
                    this, SLOT(deleteButton()));
            connect(aNode, SIGNAL(destroyed(QObject*)),
                    this, SLOT(deleteLater()));
        }
}

void ColorFillController::setNodeFillColor()
{
    QColor color = QColorDialog::getColor();
    QString s("background: #"
              + QString(color.red() < 16? "0" : "")
              + QString::number(color.red(),16)
              + QString(color.green() < 16? "0" : "")
              + QString::number(color.green(),16)
              + QString(color.blue() < 16? "0" : "")
              + QString::number(color.blue(),16) + ";"
              BUTTON_STYLE);
    button->setStyleSheet(s);
    if (node != 0 || node != nullptr)
    {
        node->setFillColour(color);
    }
}

void ColorFillController::deleteButton()
{
    delete button;
}
