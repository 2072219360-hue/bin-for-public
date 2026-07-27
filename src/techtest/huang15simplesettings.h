#ifndef SIMPLE_SETTINGS_H
#define SIMPLE_SETTINGS_H

#include <QDialog>

class QPushButton;
class QLabel;
class QTextEdit;

class SimpleSettings : public QWidget
{
    Q_OBJECT
public:
    SimpleSettings(QWidget *parent=0);
    
    QLabel *label;
	QLabel *lbltest;
    QPushButton *colorBtn;
    QTextEdit *edit;
   
    void readSettings();
    void writeSettings();
    
protected:
    //void closeEvent(QCloseEvent *);
	void paintEvent ( QPaintEvent * event );
    
public slots:
    void slotColor();
};

#endif // SETTINGS_H
