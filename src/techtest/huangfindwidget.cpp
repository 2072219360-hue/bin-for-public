#include "huangfindwidget.h"
#include "huangmainwindow.h"

#include <QDebug>

FindWidget::FindWidget(QWidget * parent)
	:QWidget(parent)
{
	m_ui.setupUi(this);

	m_ui.nextButton->setShortcut(QString("F3"));
	m_ui.prevButton->setShortcut(QString("Shift+F3"));

	connect(m_ui.closeButton, SIGNAL(clicked()), SLOT(hide()));
	connect(m_ui.nextButton, SIGNAL(clicked()), SIGNAL(nextClicked()));
	connect(m_ui.prevButton, SIGNAL(clicked()), SIGNAL(prevClicked()));
	connect(m_ui.lineEdit, SIGNAL(textChanged(QString)), SIGNAL(searchedTextChanged(QString)));
}
