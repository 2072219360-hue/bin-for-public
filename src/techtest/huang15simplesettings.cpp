#include "huang15simplesettings.h"
#include <QtGui>

SimpleSettings::SimpleSettings(QWidget *parent):QWidget(parent)
{
    setWindowTitle("Settings");
    
    label = new QLabel;
    label->setText("Setting Test");  
	
	lbltest=new QLabel;
	lbltest->setText("Another Comparor");
     
    colorBtn = new QPushButton;
    colorBtn->setText("Select Color");
    
    edit = new QTextEdit;
        
    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(label,0,0);
	layout->addWidget(lbltest,0,2);
    layout->addWidget(colorBtn,0,1);
    layout->addWidget(edit,1,0,1,3);
    
    readSettings();
    
    connect(colorBtn, SIGNAL(clicked()), this, SLOT(slotColor()));
}

void
SimpleSettings::slotColor()
{
    QColor color = QColorDialog::getColor (Qt::blue);    
    
    if(color.isValid())
    {
    	QPalette p = label->palette();
	p.setColor(QPalette::Normal,QPalette::WindowText,color);
	label->setPalette(p);
    }
}

void 
SimpleSettings::readSettings()
{
    QSettings setting("MyPro","settings");
    setting.beginGroup("Dialog");
    QPoint pos = setting.value("position").toPoint();
    QSize size = setting.value("size").toSize();    
    setting.endGroup();
    
    setting.beginGroup("Content");
    QColor color = setting.value("color").value<QColor>();
    QString text = setting.value("text").toString();
    setting.endGroup();
    
    move(pos);
    resize(size);
    QPalette p = label->palette();
    p.setColor(QPalette::Normal,QPalette::WindowText,color);
    label->setPalette(p);
    edit->setPlainText(text);
}

void
SimpleSettings::writeSettings()
{
    QSettings setting("MyPro","settings");
    setting.beginGroup("Dialog");
    setting.setValue("position",pos());
    setting.setValue("size",size());
    setting.endGroup();
    
    setting.beginGroup("Content");
    setting.setValue("color",label->palette().color(QPalette::WindowText));
    setting.setValue("text",edit->toPlainText());
    setting.endGroup();
}

void
SimpleSettings::paintEvent ( QPaintEvent * event )
{
    writeSettings();
}


