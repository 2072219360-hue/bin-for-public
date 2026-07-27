#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProcess>
#include "CConnectionForSshClient.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = 0);
    ~Widget();
	QString m_pMa;//马赫
	QString m_pAcc;//攻角
	QString m_pHigh;//高度
private:
    void createConnection();
    void disConnection();
    void initUI();
    void setConnectState(bool bState);
private slots:
    void slotSshConnect();
    void slotSshSendCmd();
    void slotClearEdit();
    void slotConnectStateChanged(bool bState,QString strIp,int nPort);
    void slotDataArrived(QString strMsg,QString strIp, int nPort);
    void slotDownLoad();
	void acSlotDownLoad();
	void processFinished(int exitCode, QProcess::ExitStatus exitStatus);
signals:
    void sigSend(QString strMsg);
    void sigDisconnected();
	void sigFlush();
public:
	QTextEdit *m_pTextEdit;
	QComboBox *m_pComBoxCmd;
	QPushButton *m_pBtnSend;
	QComboBox *m_pComBoxIp;
	QComboBox *m_pComBoxUser;
	QLineEdit *m_pLineEditPwd;
	QString m_pTestDir;
private:

    QPushButton *m_pBtnConnect;
    QLabel *m_pLabelState;
    
    
    QPushButton *m_pBtnClearEdit;
    QPushButton *m_pBtndownLoad;
	QLineEdit *m_pPathEdit;
    bool m_bConnectState;
	QString beforeCmd;
    CConnectionForSshClient *m_sshSocket;
	QSsh::SftpChannel::Ptr sftpMyChannel;
	QProcess* process;
	
};

#endif // WIDGET_H
