#include "ProgressDialog.h"

ProgressDialog::ProgressDialog(const QString &title, const QString &description) :
    bar(new QProgressBar()),
    progressDialog(new QProgressDialog())
{
	//progressDialog->setFixedSize(30,30);
	progressDialog->setLabelText(description);
    progressDialog->setWindowTitle(title);
    
	QString style = "QLabel{font-family: \"Microsoft YaHei\";font-style: normal;font-size:20px}";
	progressDialog->setStyleSheet(style);
    progressDialog->setWindowIcon(QIcon(":/img/PNG/VistaICO_Toolbar_Icons/Find.png"));
    progressDialog->setWindowFlags(progressDialog->windowFlags() & ~Qt::WindowCloseButtonHint);
    progressDialog->setCancelButton(nullptr);
	progressDialog->setWindowFlags(Qt::WindowCloseButtonHint);
	
    progressDialog->setModal(true);
    progressDialog->setMinimumWidth(360);
    progressDialog->setBar(bar);
    bar->close();
}


ProgressDialog::~ProgressDialog() = default;
void ProgressDialog::setTitle(const QString& title, const QString& des) {
	progressDialog->setWindowTitle(title);
	progressDialog->setLabelText(des);
}
void ProgressDialog::show() {
    progressDialog->show();
    //QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

}

void ProgressDialog::close() {
    progressDialog->close();
}
