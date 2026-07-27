#ifndef PALETTE_DEMO_H

#include "QtGui/QDialog"
#include "QtGui/QLabel"
#include "QtGui/QTextEdit"
#include "QtGui/QPushButton"
#include "QtGui/QComboBox"
#include "QtGui/QFrame"


class PaletteDemo:public QWidget
{
	Q_OBJECT
public:
	PaletteDemo(QWidget *parent=0);

	void createCtrlFrame();
	void createContentFrame();

	void fillColorList(QComboBox *);

public slots:
	void slotWindow();
	void slotWindowText();
	void slotButton();
	void slotButtonText();
	void slotBase();

private:
	QFrame *ctrlFrame;
	QFrame *contentFrame;

	QComboBox *windowComboBox;
	QComboBox *windowTextComboBox;
	QComboBox *buttonComboBox;
	QComboBox *buttonTextComboBox;
	QComboBox *baseComboBox;
};




#endif