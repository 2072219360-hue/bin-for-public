#include "QtGui/QtGui"

#include "huang01Configdialog.h"
#include "huang01Baseinfo.h"
#include "huang01Detail.h"
#include "huang01Contact.h"

ConfigDialog::ConfigDialog(QWidget *parent):QWidget(parent)
{
	contentsWidget=new QListWidget;
	contentsWidget->addItem(tr("base info"));
	contentsWidget->addItem(tr("contact"));
	contentsWidget->addItem(tr("detail"));
	contentsWidget->setMaximumWidth(128);
	connect(contentsWidget,SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)),this,SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));

	pagesWidget=new QStackedWidget;
	pagesWidget->addWidget(new BaseInfo);
	pagesWidget->addWidget(new Contact);
	pagesWidget->addWidget(new Detail);

	connect(pagesWidget,SIGNAL(currentChanged(int)),this,SLOT(faderInWidget(int)));

	QPushButton *closeButton = new QPushButton(tr("Close"));

	contentsWidget->setCurrentRow(0);
	connect(closeButton,SIGNAL(clicked()),this,SLOT(close()));

	QHBoxLayout *HorizontalLayout=new QHBoxLayout;
	HorizontalLayout->addWidget(contentsWidget);
	HorizontalLayout->addWidget(pagesWidget,1);

	QHBoxLayout *buttonsLayout=new QHBoxLayout;
	buttonsLayout->addStretch(1);
	buttonsLayout->addWidget(closeButton);

	QVBoxLayout *mainLayout=new QVBoxLayout;
	mainLayout->addLayout(HorizontalLayout);
	mainLayout->addStretch(1);
	mainLayout->addSpacing(12);
	mainLayout->addLayout(buttonsLayout);
	setLayout(mainLayout);

	setWindowTitle(tr("Fader in Fader out Example"));

	faderWidget=new FaderWidget(this);
	faderWidget->start();

}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
	if (!current)
		current=previous;
	pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void ConfigDialog::faderInWidget(int index)
{
	if (faderWidget)
		faderWidget->close();
	faderWidget=new FaderWidget(pagesWidget->widget(index));
	faderWidget->start();
}