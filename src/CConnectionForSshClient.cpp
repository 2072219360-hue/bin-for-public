#include "CConnectionForSshClient.h"
#include <QDebug>


CConnectionForSshClient::CConnectionForSshClient(QString strIp, int nPort, QString strPwd, QString strUser)
{
    m_strIp = strIp;
    m_nPort = nPort;
    m_strUser = strUser;
    m_strPwd = strPwd;
    m_strIpPort = m_strIp + ":" + QString::number(m_nPort);
}

void CConnectionForSshClient::init()
{

    m_pThread = new QThread();
    connect(m_pThread,SIGNAL(finished()),this,SLOT(slotThreadFinished()));
    this->moveToThread(m_pThread);

    m_pThread->start();

    //之后的逻辑都得通过信号和槽接通
    connect(this,SIGNAL(sigInitForClild()),this,SLOT(slotInitForClild()));
    emit sigInitForClild();
    //connect(btn1,clicked())
}

void CConnectionForSshClient::unInit()
{
    m_pThread->quit();
}

int CConnectionForSshClient::send(QString strMessage)
{
    qDebug()<<"CConnectionForSshClient ssh send "<<strMessage;

    int nSize = 0;
    if(m_bConnected && m_bSendAble){
       nSize = m_shell->write(strMessage.toLatin1().data());
    }else{
       qDebug()<<"CConnectionForSshClient::send() ssh未连接 或 shell未连接:"<<getIpPort();
    }

    return nSize;
}

CConnectionForSshClient::~CConnectionForSshClient()
{
    if(nullptr != m_pSshSocket){
        delete m_pSshSocket;
        m_pSshSocket = nullptr;
    }
}

void CConnectionForSshClient::slotResetConnection(QString strIpPort)
{
    if(this->getIpPort() == strIpPort){
        this->slotDisconnected();
    }
}

void CConnectionForSshClient::slotSend(QString strIpPort, QString strMessage)
{
    if(0 != m_strIpPort.compare(strIpPort)){
        return;
    }

    send(strMessage);
}

void CConnectionForSshClient::slotSendByQByteArray(QString strIpPort, QByteArray arrMsg)
{
    if(0 != m_strIpPort.compare(strIpPort)){
        return;
    }

    if(m_bConnected){
       m_shell->write(arrMsg);
    }else{
       qDebug()<<"CConnectionForSshClient::send(QString strMessage) 发送失败 未建立连接:"<<getIpPort();
    }
}

void CConnectionForSshClient::slotInitForClild()
{
	//对parameters进行赋值
    m_argParameters.port = m_nPort;
    m_argParameters.userName = m_strUser;
    m_argParameters.password = m_strPwd;
    m_argParameters.host = m_strIp;
    m_argParameters.timeout = 10000;
    m_argParameters.authenticationType =
            QSsh::SshConnectionParameters::AuthenticationTypePassword; //密码方式连接

    slotCreateConnection(); //连接

    m_pTimer = new QTimer(this);
    //每10s触发一次timeout
    m_pTimer->setInterval(RECONNET_SPAN_TIME);
    connect(m_pTimer,SIGNAL(timeout()),this,SLOT(slotCreateConnection()));
    m_pTimer->start();//启动定时器，每隔一段时间进入slotCreateConnection判断是否需要重连
}

void CConnectionForSshClient::slotCreateConnection()
{

    qDebug()<<"CConnectionForSshClient::slotCreateConnection检查连接" ;

    if(true == m_bConnected)
        return;

    if(nullptr == m_pSshSocket){
        //QSsh::SshConnection *m_pSshSocket
        m_pSshSocket = new QSsh::SshConnection(m_argParameters);
        //m_pSshSocket = QSsh::acquireConnection(m_argParameters);
        connect(m_pSshSocket,SIGNAL(connected()),SLOT(slotConnected()));
        connect(m_pSshSocket,SIGNAL(error(QSsh::SshError)),SLOT(slotSshConnectError(QSsh::SshError)));
    }
    m_pSshSocket->connectToHost();
    qDebug()<<"CConnectionForSshClient::slotCreateConnection() 以ssh方式 尝试连接:"<<getIpPort();
}

void CConnectionForSshClient::slotConnected()
{
    qDebug()<<"CConnectionForSshClient::slotConnected ssh已连接到:"<<getIpPort();
    m_pTimer->stop();

    m_shell = m_pSshSocket->createRemoteShell();
    connect(m_shell.data(), SIGNAL(started()), SLOT(slotShellStart()));
    connect(m_shell.data(), SIGNAL(readyReadStandardOutput()), SLOT(slotDataReceived()));
    connect(m_shell.data(), SIGNAL(readyReadStandardError()), SLOT(slotShellError()));
    m_shell.data()->start();
	
    m_bConnected = true;
    emit sigConnectStateChanged(m_bConnected,m_strIp,m_nPort);
}

void CConnectionForSshClient::slotDisconnected()
{
    m_pSshSocket->disconnectFromHost();
}

void CConnectionForSshClient::slotThreadFinished()
{
    //延时析构
    m_pThread->deleteLater();
    this->deleteLater();
}

void CConnectionForSshClient::slotSshConnectError(QSsh::SshError sshError)
{
    m_bSendAble = false;
    m_bConnected = false;
    emit sigConnectStateChanged(m_bConnected,m_strIp,m_nPort);

    m_pTimer->start();

    switch(sshError){
    case QSsh::SshNoError:
        qDebug()<<"slotSshConnectError SshNoError"<<getIpPort();
        break;
    case QSsh::SshSocketError:
        qDebug()<<"slotSshConnectError SshSocketError"<<getIpPort(); //拔掉网线是这种错误
        break;
    case QSsh::SshTimeoutError:
        qDebug()<<"slotSshConnectError SshTimeoutError"<<getIpPort();
        break;
    case QSsh::SshProtocolError:
        qDebug()<<"slotSshConnectError SshProtocolError"<<getIpPort();
        break;
    case QSsh::SshHostKeyError:
        qDebug()<<"slotSshConnectError SshHostKeyError"<<getIpPort();
        break;
    case QSsh::SshKeyFileError:
        qDebug()<<"slotSshConnectError SshKeyFileError"<<getIpPort();
        break;
    case QSsh::SshAuthenticationError:
        qDebug()<<"slotSshConnectError SshAuthenticationError"<<getIpPort();
        break;
    case QSsh::SshClosedByServerError:
        qDebug()<<"slotSshConnectError SshClosedByServerError"<<getIpPort();
        break;
    case QSsh::SshInternalError:
        qDebug()<<"slotSshConnectError SshInternalError"<<getIpPort();
        break;
    default:
        break;
    }

}

void CConnectionForSshClient::slotShellStart()
{
    m_bSendAble = true;
    qDebug()<<"CConnectionForSshClient::slotShellStart Shell已连接:"<<getIpPort();
}

void CConnectionForSshClient::slotShellError()
{
    qDebug()<<"CConnectionForSshClient::slotShellError Shell发生错误:"<<getIpPort();
}

void CConnectionForSshClient::slotSend(QString strMessage)
{
    send(strMessage);
}

void CConnectionForSshClient::slotDataReceived()
{
    QByteArray byteRecv = m_shell->readAllStandardOutput();
    QString strRecv = QString::fromUtf8(byteRecv);

//    if(strRecv.contains("password for")){
//        m_shell->write(m_strPwd.toLatin1().data());
//    }
	qDebug() << u8"remote收到消息了!!";
    if(!strRecv.isEmpty()) //过滤空行
        emit sigDataArrived(strRecv, m_strIp, m_nPort);

}
