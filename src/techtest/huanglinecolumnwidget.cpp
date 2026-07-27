#include "huanglinecolumnwidget.h"
#include "ui_linecolumnwidget.h"

LineColumnWidget::LineColumnWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::LineColumnWidget)
{
    m_ui->setupUi(this);
}

LineColumnWidget::~LineColumnWidget()
{
    delete m_ui;
}

void LineColumnWidget::setLine(int line)
{
	m_ui->lineLabel->setText(QString("%1").arg(line));
}

void LineColumnWidget::setColumn(int column)
{
	m_ui->columnLabel->setText(QString("%1").arg(column));
}

void LineColumnWidget::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
