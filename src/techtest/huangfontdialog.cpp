#include "huangfontdialog.h"
#include "ui_fontdialog.h"

FontDialog::FontDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::FontDialog)
{
    m_ui->setupUi(this);
}

FontDialog::~FontDialog()
{
    delete m_ui;
}

void FontDialog::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void FontDialog::setEditSize(int size)
{
	m_editSize = size;
	m_ui->textEditFontSpin->setValue(size);
}

void FontDialog::setTreeSize(int size)
{
	m_treeSize = size;
	m_ui->treeFontSpin->setValue(size);
}

int FontDialog::editSize() const
{
	return m_editSize;
}

int FontDialog::treeSize() const
{
	return m_treeSize;
}

void FontDialog::on_textEditFontSpin_valueChanged(int value)
{
	m_editSize = value;
}

void FontDialog::on_treeFontSpin_valueChanged(int value)
{
	m_treeSize = value;
}
