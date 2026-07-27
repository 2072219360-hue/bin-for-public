#ifndef FONTDIALOG_H
#define FONTDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class FontDialog;
}

class FontDialog : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(FontDialog)
public:
    explicit FontDialog(QWidget *parent = 0);
    virtual ~FontDialog();

	void setEditSize(int size);
	void setTreeSize(int size);
	int editSize() const;
	int treeSize() const;

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::FontDialog *m_ui;
	int m_editSize;
	int m_treeSize;

private slots:
	void on_textEditFontSpin_valueChanged(int value);
	void on_treeFontSpin_valueChanged(int value);
};

#endif // FONTDIALOG_H
