/*
 * File:    settingsdialog.cpp
 * Author:  Ian Cathcart
 * Date:    2020/08/05
 * Version: 1.5
 *
 * Purpose: Implements the settings dialog.
 *
 * Modification history:
 * Aug 5, 2020 (IC V1.1)
 *  (a) Fix spelling of "colour".  Add "Bg" into background colour var names.
 *  (b) Lots of tweaks to saveSettings() and loadSettings() to make sure
 *      they properly save and load custom DPI settings.
 *  (c) Added a signal to tell mainWindow that the user OK'd the dialog.
 * Aug 7, 2020 (IC V1.2)
 *  (a) Added background colour buttons to settingsdialog.ui so their code
 *      is reflected here.  They should prompt the user to select a colour
 *      for saved graph backgrounds.  TODO: Allow a transparent background
 *      option for non-JPEG image types.
 * Aug 26, 2020 (IC V1.3)
 *  (a) Set the font of this UI to match mainwindow's font.
 * Aug 27, 2020 (IC V1.4)
 *  (a) Moved the gridCellSize widget from the mainwindow to here and added
 *      a settings value for it to be used in canvasscene.cpp.
 * Oct 15, 2020 (JD V1.5)
 *  (a) Allow users to set alpha channel for the non-JPEG image background.
 *      Show the percentage transparent on the "other image" button.
 *	Added setOtherImageButtonStyle() to encapsulate the actions
 *	required for that button to show the transparency level.
 *  (b) Improve the way getColor() is called so that the colour picker
 *	is initialized to the previous colour (or white, if none).
 *	Ditto for the colour of the buttons themselves on the very
 *	first call to the settings dialog.
 *  (c) Some code tidying; some new function comments.
 *  (d) Rename customSpinBox->customDpiSpinBox and customButton ->
 *	customDpiButton for clarity.
 */

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "defuns.h"
#include "mainwindow.h"

#include <QColorDialog>


SettingsDialog::SettingsDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Settings");

    QFont font;
    font.setFamily("Arimo");
    this->setFont(font);

    // Initialize colour buttons; these might be over-ridden when
    // loadSettings() is called.
    QString s("background: #ffffff;" BUTTON_STYLE);
    ui->jpgBgColour->setStyleSheet(s);
    ui->otherImageBgColour->setStyleSheet(s);

    loadSettings();

    connect(this, SIGNAL(accepted()), this, SLOT(saveSettings()));
}



SettingsDialog::~SettingsDialog()
{
    delete ui;
}



void
SettingsDialog::loadSettings()
{
    // Always set this label to defaultResolution
    ui->defaultLabel->setText(settings.value("defaultResolution").toString()
                              + " pixels/inch");

    // If no DPI settings founds, initialize to defaults.
    if (!settings.contains("useDefaultResolution"))
    {
        ui->customDpiSpinBox
	    ->setValue(settings.value("defaultResolution").toInt());
    }
    else
    {
	// Load saved DPI settings.
        if (settings.value("useDefaultResolution").toBool() == true)
            ui->defaultDpiButton->setChecked(true);
        else
            ui->customDpiButton->setChecked(true);

        ui->customDpiSpinBox
	    ->setValue(settings.value("customResolution").toInt());
    }

    if (settings.contains("gridCellSize"))
	ui->gridCellSize->setValue(settings.value("gridCellSize").toInt());

    if (settings.contains("jpgBgColour"))
    {
	qDeb() << "... settings contains jpgBgColour = "
	       << settings.value("jpgBgColour").toString();
	
	ui->jpgBgColour
	    ->setStyleSheet("background: "
			    + settings.value("jpgBgColour").toString()
			    + "; " + BUTTON_STYLE);
	ui->jpgBgColour->update();
    }

    setOtherImageButtonStyle();
}



void
SettingsDialog::saveSettings()
{
    settings.setValue("useDefaultResolution", ui->defaultDpiButton->isChecked());
    settings.setValue("customResolution", ui->customDpiSpinBox->value());
    settings.setValue("gridCellSize", ui->gridCellSize->value());

    emit saveDone();
}



/*
 * Name:	on_jpgBgColour_clicked()
 * Purpose:	Implement the actions required when the "JPEG image"
 *		background colour button is clicked.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The JPEG image background colour and the look of the button.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	None.
 */

void
SettingsDialog::on_jpgBgColour_clicked()
{
    QColor oldColour, newColour;

    if (settings.contains("jpgBgColour"))
	oldColour = settings.value("jpgBgColour").toString();
    else
	oldColour = Qt::white;

    newColour = QColorDialog::getColor(oldColour, nullptr,
				       "Select JPEG background colour");
    if (!newColour.isValid())
	return;

    QString newStyle("background: " + newColour.name() + "; " BUTTON_STYLE);
    settings.setValue("jpgBgColour", newColour.name());
    ui->jpgBgColour->setStyleSheet(newStyle);

    ui->jpgBgColour->update();
}



/*
 * Name:	on_otherImageBgColour_clicked()
 * Purpose:	Implement the actions required when the "other image"
 *		background colour button is clicked.
 * Arguments:	None.
 * Outputs:	Nothing.
 * Modifies:	The other image background colour and the look of the button.
 * Returns:	Nothing.
 * Assumptions:	?
 * Bugs:	None known.
 * Notes:	None.
 */

void
SettingsDialog::on_otherImageBgColour_clicked()
{
    QColor oldColour, newColour;

    if (settings.contains("otherImageBgColour"))
	oldColour = settings.value("otherImageBgColour").toString();
    else
	oldColour = Qt::white;

    newColour = QColorDialog::getColor(oldColour, nullptr,
				       "Select image background colour",
				       QColorDialog::ShowAlphaChannel);

    if (!newColour.isValid())
	return;

    settings.setValue("otherImageBgColour", newColour.name(QColor::HexArgb));

    setOtherImageButtonStyle();
}



/*
 * Name:	setOtherImageButtonStyle()
 * Purpose:	Set the background colour, as well as the foreground
 *		text and colour, for the "other image" colour button.
 * Arguments:	None (params gleaned from settings).
 * Outputs:	Nothing.
 * Modifies:	The style of the other image colour button.
 * Returns:	Nothing.
 * Assumptions:	The ui->otherImageBgColour has been initialized.
 * Bugs:	None known.
 * Notes:	At time of writing called from both loadSettings() and
 *		on_otherImageBgColour_clicked().
 *		TODO: The decision on when to write the transparency
 *		in black or when to use white probably needs harder
 *		thinking.
 */

void
SettingsDialog::setOtherImageButtonStyle()
{
    QColor currentColour;
    QString buttonFGColour;
    QString transparency;
    int totalColour;
    int alphaPercent;

    if (settings.contains("otherImageBgColour"))
	currentColour = settings.value("otherImageBgColour").toString();
    else
	currentColour = Qt::white;

    totalColour = currentColour.red() + currentColour.green()
	+ currentColour.blue();
    if (totalColour < 255 * 3 / 2 && currentColour.alpha() > 127)
	buttonFGColour = "color: #ffffff; ";
    else
	buttonFGColour = "color: #000000; ";

    QString buttonStyle("background: " + currentColour.name(QColor::HexArgb)
			+ "; " + buttonFGColour + BUTTON_STYLE);
    ui->otherImageBgColour->setStyleSheet(buttonStyle);

    alphaPercent = 100 * (255 - currentColour.alpha()) / 255;
    transparency = QString::number(alphaPercent) + "% transparent";
    ui->otherImageBgColour->setText(transparency);

    ui->otherImageBgColour->update();
}
