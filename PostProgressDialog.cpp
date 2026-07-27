#include "PostProgressDialog.h"

// 构造函数优化：初始化时立即刷新
PostProgressDialog::PostProgressDialog(const QString &title, const QString &description) :
	bar(new QProgressBar()),
	progressDialog(new QProgressDialog())
{
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

	// 初始化时强制刷新一次
	forceRefresh();  // 新增行
}
PostProgressDialog::~PostProgressDialog() = default;
// 实现强制刷新逻辑
void PostProgressDialog::forceRefresh() {
	if (progressDialog) {
		// 强制更新布局和绘制
		progressDialog->updateGeometry();
		progressDialog->repaint();
		// 处理所有待处理事件（尤其是绘制事件）
		QApplication::processEvents(QEventLoop::AllEvents, 100);
	}
}

// 重写show方法，显示后立即刷新
void PostProgressDialog::show() {
	progressDialog->show();
	forceRefresh();  // 新增行：显示后强制刷新
}

// 优化setTitle方法，设置后立即刷新
void PostProgressDialog::setTitle(const QString& title, const QString& des) {
	progressDialog->setWindowTitle(title);
	progressDialog->setLabelText(des);
	forceRefresh();  // 新增行：设置文本后强制刷新
}

// 关闭对话框时也处理残留事件
void PostProgressDialog::close() {
	if (progressDialog) {
		progressDialog->close();
		QApplication::processEvents(QEventLoop::AllEvents, 50);
	}
}