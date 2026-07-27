#include "Widget.h"
#include <QDebug>
#include <QGridLayout>

//nohup ./test.exe >testLog.txt 2>&1 &
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    initUI();
}

Widget::~Widget()
{

}

void Widget::initUI()
{
    int nItemHeight = 30;
    //...
    m_bConnectState = false;
    //窗口大小
    //resize(400,250);

    QLabel *pLabelIp = new QLabel(this);

    pLabelIp->setText(u8"IP地址");
    pLabelIp->setMinimumHeight(nItemHeight);
    QLabel *pLabelUser = new QLabel(this);
    pLabelUser->setText(u8"用户名");
    pLabelUser->setMinimumHeight(nItemHeight);
    QLabel *pLabelPort = new QLabel(this);
    pLabelPort->setText(u8"密 码");
    pLabelPort->setMinimumHeight(nItemHeight);
    //下拉框
    m_pComBoxIp = new QComboBox(this);
    //m_pComBoxIp->addItem("101.42.159.79");
    m_pComBoxIp->setMinimumHeight(nItemHeight);
    m_pComBoxIp->setEditable(true);

    m_pComBoxUser = new QComboBox(this);
    //m_pComBoxUser->addItem("sqh");
    m_pComBoxUser->setEditable(true);

    m_pLineEditPwd = new QLineEdit(this);
    m_pLineEditPwd->setEchoMode(QLineEdit::Password);
    //m_pLineEditPwd->setText("sqh");

    m_pLabelState = new QLabel(this);
    m_pLabelState->setFixedSize(10,10);
    setConnectState(m_bConnectState);

    m_pBtnConnect = new QPushButton(this);
    m_pBtnConnect->setText(u8"连接");
    connect(m_pBtnConnect,SIGNAL(clicked()),this,SLOT(slotSshConnect()));

    m_pComBoxCmd = new QComboBox(this);
    m_pComBoxCmd->addItem("astparam dump");
    m_pComBoxCmd->addItem("ls -la");
    m_pComBoxCmd->addItem("cd /tmp");
    m_pComBoxCmd->setEditable(true);

    m_pBtnSend = new QPushButton(this);
    m_pBtnSend->setText(u8"发送");
    connect(m_pBtnSend,SIGNAL(clicked()),this,SLOT(slotSshSendCmd()));

    m_pTextEdit = new QTextEdit(this);

	m_pPathEdit = new QLineEdit(this);

    m_pBtnClearEdit = new QPushButton(this);
    m_pBtnClearEdit->setText(u8"清除");
    connect(m_pBtnClearEdit,SIGNAL(clicked()),this,SLOT(slotClearEdit()));

    m_pBtndownLoad = new QPushButton(this);
    m_pBtndownLoad->setText(u8"下载");
    connect(m_pBtndownLoad,SIGNAL(clicked()),this,SLOT(slotDownLoad()));

    //格线布局
    QGridLayout *layout = new QGridLayout;
    layout->addWidget(pLabelIp,     0,0,1,4); //IP地址Label  坐标第0行0列  所占空间大小 1行4列
    layout->addWidget(pLabelUser,   1,0,1,4);
    layout->addWidget(pLabelPort,   2,0,1,4);
    layout->addWidget(m_pLabelState,3,0,1,4);//连接状态Label  坐标第3行0列  所占空间大小 1行4列

    layout->addWidget(m_pComBoxIp,   0,1,1,4);
    layout->addWidget(m_pComBoxUser, 1,1,1,4);
    layout->addWidget(m_pLineEditPwd,2,1,1,4);
    layout->addWidget(m_pBtnConnect, 3,1,1,4);

    layout->addWidget(m_pComBoxCmd,8,0,1,2);
    layout->addWidget(m_pBtnSend,  8,2,1,1);

    layout->addWidget(m_pTextEdit,    4,0,1,8);
    layout->addWidget(m_pBtnClearEdit,8,3,1,5);
	layout->addWidget(m_pPathEdit,9,0,1,2);
	layout->addWidget(m_pBtndownLoad, 9, 2, 1,1);

	m_pTextEdit->setReadOnly(true);
    this->setLayout(layout);
}

void Widget::createConnection()
{
    QString strIp = m_pComBoxIp->currentText();
    QString strUser = m_pComBoxUser->currentText();
    QString strPwd = m_pLineEditPwd->text();
    m_sshSocket = new CConnectionForSshClient(strIp,22,strPwd,strUser);
    m_sshSocket->init();
    connect(m_sshSocket,SIGNAL(sigConnectStateChanged(bool,QString,int)),
            this,SLOT(slotConnectStateChanged(bool,QString,int)));
    connect(m_sshSocket,SIGNAL(sigDataArrived(QString ,QString , int )),
            this,SLOT(slotDataArrived(QString ,QString , int )));
    connect(this,SIGNAL(sigSend(QString)),m_sshSocket,SLOT(slotSend(QString)));
    connect(this,SIGNAL(sigDisconnected()),m_sshSocket,SLOT(slotDisconnected()));

}

void Widget::disConnection()
{
    emit sigDisconnected();
}

void Widget::setConnectState(bool bState)
{
    if(!bState)
        m_pLabelState->setStyleSheet("QLabel{background-color:#ff0000;border-radius:5px;}");
    else
        m_pLabelState->setStyleSheet("QLabel{background-color:#00ff00;border-radius:5px;}");
}

void Widget::slotSshConnect()
{
    //我没链接
    if(!m_bConnectState){
        m_pBtnConnect->setText(u8"连接中...");
        createConnection();  //发起连接
    }else{
        m_pBtnConnect->setText(u8"连接");
        m_bConnectState = false;
        emit sigDisconnected();//断开连接
        setConnectState(m_bConnectState);
    }
}

void Widget::slotSshSendCmd()
{
    if(m_bConnectState){
        QString strCmd = m_pComBoxCmd->currentText();
        strCmd += "\n"; //添加回车
        emit sigSend(strCmd);
    }
	else {
		slotSshConnect();
		QString strCmd = m_pComBoxCmd->currentText();
		strCmd += "\n"; //添加回车
		emit sigSend(strCmd);
	}
	m_pComBoxCmd->clearEditText();
	//emit sigFlush();
	connect(m_pTextEdit, SIGNAL(textChanged()), this, SIGNAL(sigFlush()));
}

void Widget::slotClearEdit()
{
    m_pTextEdit->clear();
}

void Widget::slotConnectStateChanged(bool bState, QString strIp, int nPort)
{
    Q_UNUSED(strIp)
    Q_UNUSED(nPort)

    m_bConnectState = bState;
    setConnectState(m_bConnectState);
    if(m_bConnectState)
        m_pBtnConnect->setText(u8"断开");
    else
        m_pBtnConnect->setText(u8"连接");
}

void Widget::slotDataArrived(QString strMsg, QString strIp, int nPort)
{
    Q_UNUSED(strIp)
    Q_UNUSED(nPort)

    m_pTextEdit->append(strMsg);
    m_pTextEdit->moveCursor(QTextCursor::End);
}
void Widget::slotDownLoad()
{
	/***************************************************************/
	//QProcess process(this);
	////process.setProgram("D:/CPlus/Qssh-master/examples_bin/sftp.exe");
	//QStringList argument;
	//argument  << "-h"<< m_pComBoxIp->currentText() <<"-u"<<m_pComBoxUser->currentText()<<"-pwd"<< m_pLineEditPwd->text()<<"-path"<<"1"<< m_pPathEdit->text();
	////process.setArguments(argument);
	//process.setProcessChannelMode(QProcess::MergedChannels);
	//process.start("D:/CPlus/Qssh-master/examples_bin/sftp.exe",argument);
	//
	//process.waitForStarted(); //等待程序启动
	//process.waitForFinished();//等待程序关闭
	/**************************************************************/
	//while(!process.waitForFinished());	

	/*if (!process.waitForFinished(1)) {
		process.kill();
		process.waitForFinished(1);
	}*/
	//QString temp = QString::fromLocal8Bit(process.readAllStandardOutput()); //程序输出信息
	//QMessageBox::information(this, u8"提示", temp);
	//----------------------------------------------------
	//process = new QProcess(this);
	////process.setProgram("D:/CPlus/Qssh-master/examples_bin/sftp.exe");
	//QStringList argument;
	//argument << "-h" << m_pComBoxIp->currentText() << "-u" << m_pComBoxUser->currentText() << "-pwd" << m_pLineEditPwd->text() << "-path" << "1" << m_pPathEdit->text();
	//connect(process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(processFinished(int, QProcess::ExitStatus)));
	//process->start("sftp.exe", argument);
	

}

void Widget::acSlotDownLoad()
{
	//m_sshSocket->sftpChannel->downloadFile("/home/sqh/test.log", "D:/QtProject/test.log", QSsh::SftpOverwriteMode::SftpOverwriteExisting);
	sftpMyChannel->downloadFile("/home/sqh/test.log", "D:/QtProject/test.log", QSsh::SftpOverwriteMode::SftpOverwriteExisting);

}

void Widget::processFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
	if (exitStatus == QProcess::NormalExit)
	{
		//程序正常结束
		
	}
	else
	{
		//程序异常结束
	}
	//QMessageBox::information(this, u8"提示", u8"下载完成");
}
