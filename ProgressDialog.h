#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QApplication>
#include <QIcon>
#include <QPointer>
#include <QProgressBar>
#include <QProgressDialog>
#include<QPushButton>
#include<QMessageBox>
/**
 * @class ProgressDialog
 * Progress dialog to be shown when a long task is running
 */
class ProgressDialog:public QObject {
	Q_OBJECT
public:
	
    /**
     * Constructor
     * @param title         Title of the dialog
     * @param description   Text of the dialog
     */
    ProgressDialog(const QString &title, const QString &description);

    /**
     * Destructor
     */
    ~ProgressDialog();

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
private:
    QPointer<QProgressBar> bar; /**< Bar pointer */
    QPointer<QProgressDialog> progressDialog; /**< Dialog pointer */
};

#endif // PROGRESSDIALOG_H
