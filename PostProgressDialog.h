#ifndef POSTPROGRESSDIALOG_H
#define POSTPROGRESSDIALOG_H

#include <QApplication>
#include <QIcon>
#include <QPointer>
#include <QProgressBar>
#include <QProgressDialog>
#include<QPushButton>
#include<QMessageBox>
/**
 * @class PostProgressDialog
 * Progress dialog to be shown when a long task is running
 */
class PostProgressDialog :public QObject {
	Q_OBJECT
public:

	/**
	 * Constructor
	 * @param title         Title of the dialog
	 * @param description   Text of the dialog
	 */
	PostProgressDialog(const QString &title, const QString &description);

	/**
	 * Destructor
	 */
	~PostProgressDialog();

	/**
	 * Shows the dialog
	 */
public slots:
	void show();
	void setTitle(const QString& title, const QString& des);
	/**
	 * Close the dialog
	 */
	void close();
	/**
	 * 强制刷新对话框UI（新增接口）
	 */
	void forceRefresh();  // 新增行
private:
	QPointer<QProgressBar> bar; /**< Bar pointer */
	QPointer<QProgressDialog> progressDialog; /**< Dialog pointer */
};
#endif // PROGRESSDIALOG_H
