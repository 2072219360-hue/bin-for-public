#ifndef MESSAGEBOX_TEST_H
#define MESSAGEBOX_TEST_H

#include "QtGui/QDialog"

class QLabel;
class MessageBoxTest:public QDialog
{
	Q_OBJECT
public:
	MessageBoxTest(QWidget *parent=0);

public slots:
	void slotQuestion();
    void slotInformation();
    void slotWarning();
    void slotCritical();
    void slotAbout();
    void slotAboutQt();
    void slotCustom();

private:
	QLabel *label;
};

#endif
