/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#include "sftptest.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>
#include <iostream>
#include <QApplication>
#include <QTimer>
using namespace QSsh;

SftpTest::SftpTest(Parameters &params)
    : m_parameters(params), m_state(Inactive), m_error(false), m_connection(0),
      m_bigFileUploadJob(SftpInvalidJob),
      m_bigFileDownloadJob(SftpInvalidJob),
      m_bigFileRemovalJob(SftpInvalidJob),
      m_mkdirJob(SftpInvalidJob),
      m_statDirJob(SftpInvalidJob),
      m_lsDirJob(SftpInvalidJob),
      m_rmDirJob(SftpInvalidJob),
      inistate(false),
      isDownTwo(0)
{


}

SftpTest::~SftpTest()
{
    removeFiles(true);
    delete m_connection;
}

void SftpTest::run()
{
    //建立一个新的连接m_connection，并初始化sshConnectionPrivare
    //将sshConnectionPrivare和sshconnect进行信号与槽的连接
    m_connection = new SshConnection(m_parameters.sshParams);
    //信号与槽的连接（连接、出错以及断开连接）
    connect(m_connection, SIGNAL(connected()), SLOT(handleConnected()));
    connect(m_connection, SIGNAL(error(QSsh::SshError)), SLOT(handleError()));
    connect(m_connection, SIGNAL(disconnected()), SLOT(handleDisconnected()));

    qDebug()<<"Connecting to host '"<< qPrintable(m_parameters.sshParams.host) << "'...";
    qDebug()<<"Connecting to port '"<< qPrintable(QString::number(m_parameters.sshParams.port)) << "'...";
    qDebug()<<"Connecting to user '"<< qPrintable(m_parameters.sshParams.userName) << "'...";
    qDebug()<<"Connecting to pwd '"<< qPrintable(m_parameters.sshParams.password) << "'...";
    qDebug()<<qPrintable(m_parameters.sshParams.host);
    qDebug()<<qPrintable(QString::number(m_parameters.sshParams.port));
    qDebug()<<qPrintable(m_parameters.sshParams.userName);
    qDebug()<<qPrintable(m_parameters.sshParams.password);

    m_state = Connecting;
    //正式进行连接conncetToHost
    m_connection->connectToHost();


}

bool SftpTest::getState()
{
    return inistate;
}

void SftpTest::handleConnected()
{
    if (m_state != Connecting) {
        std::cerr << "Unexpected state " << m_state << " in function "
            << Q_FUNC_INFO << "." << std::endl;
        earlyDisconnectFromHost();
    } else {
        //std::cout << "Connected. Initializing SFTP channel..." << std::endl;
        qDebug()<<"Connected. Initializing SFTP channel...";
        m_channel = m_connection->createSftpChannel();
        connect(m_channel.data(), SIGNAL(initialized()), this,
           SLOT(handleChannelInitialized()));
        connect(m_channel.data(), SIGNAL(channelError(QString)), this,
            SLOT(handleChannelInitializationFailure(QString)));
        connect(m_channel.data(), SIGNAL(finished(QSsh::SftpJobId,QString)),
            this, SLOT(handleJobFinished(QSsh::SftpJobId,QString)));
        connect(m_channel.data(),
            SIGNAL(fileInfoAvailable(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)),
            SLOT(handleFileInfo(QSsh::SftpJobId,QList<QSsh::SftpFileInfo>)));
        connect(m_channel.data(), SIGNAL(closed()), this,
            SLOT(handleChannelClosed()));
        m_state = InitializingChannel;
        m_channel->initialize();


    }
}

void SftpTest::handleDisconnected()
{
    if (m_state != Disconnecting) {
        std::cerr << "Unexpected state " << m_state << " in function "
            << Q_FUNC_INFO << std::endl;
        m_error = true;
    } else {
        std::cout << "Connection closed." << std::endl;
    }
    std::cout << "Test finished. ";
    if (m_error)
        std::cout << "There were errors.";
    else
        std::cout << "No errors encountered.";
    std::cout << std::endl;
    qApp->exit(m_error ? EXIT_FAILURE : EXIT_SUCCESS);
}

void SftpTest::handleError()
{
    std::cerr << "Encountered SSH error: "
        << qPrintable(m_connection->errorString()) << "." << std::endl;
    m_error = true;
    m_state = Disconnecting;
    qApp->exit(EXIT_FAILURE);
}

void SftpTest::handleChannelInitialized()
{
//    if (m_state != InitializingChannel) {
//        std::cerr << "Unexpected state " << m_state << "in function "
//            << Q_FUNC_INFO << "." << std::endl;
//        earlyDisconnectFromHost();
//        return;
//    }

//    std::cout << "Creating " << m_parameters.smallFileCount
//        << " files of 1 KB each ..." << std::endl;
//    qsrand(QDateTime::currentDateTime().toTime_t());
//    for (int i = 0; i < m_parameters.smallFileCount; ++i) {
//        const QString fileName
//            = QLatin1String("sftptestfile") + QString::number(i + 1);
//        const FilePtr file(new QFile(QDir::tempPath() + QLatin1Char('/')
//            + fileName));
//        bool success = true;
//        if (!file->open(QIODevice::WriteOnly | QIODevice::Truncate))
//            success = false;
//        if (success) {
//            int content[1024/sizeof(int)];
//            for (size_t j = 0; j < sizeof content / sizeof content[0]; ++j)
//                content[j] = qrand();
//            file->write(reinterpret_cast<char *>(content), sizeof content);
//            file->close();
//        }
//        success = success && file->error() == QFile::NoError;
//        if (!success) {
//            std::cerr << "Error creating local file "
//                << qPrintable(file->fileName()) << "." << std::endl;
//            earlyDisconnectFromHost();
//            return;
//        }
//        m_localSmallFiles << file;
//    }
//    std::cout << "Files created. Now uploading..." << std::endl;
//    foreach (const FilePtr &file, m_localSmallFiles) {
//        const QString localFilePath = file->fileName();
//        const QString remoteFp
//            = remoteFilePath(QFileInfo(localFilePath).fileName());
//        const SftpJobId uploadJob = m_channel->uploadFile(file->fileName(),
//            remoteFp, SftpOverwriteExisting);
//        if (uploadJob == SftpInvalidJob) {
//            std::cerr << "Error uploading local file "
//                << qPrintable(localFilePath) << " to remote file "
//                << qPrintable(remoteFp) << "." << std::endl;
//            earlyDisconnectFromHost();
//            return;
//        }
//        m_smallFilesUploadJobs.insert(uploadJob, remoteFp);
//    }
    qDebug()<<"ready to downLoad!!!!!!!!!!!!!!!!!!!!!!!";
    qDebug()<<"m_parameters.isDownLoad:"<<m_parameters.isDownLoad<<" m_parameters.isUpload:"<<m_parameters.isUpload;
    //system("pause");
    if(m_parameters.isDownLoad)
        const SftpJobId downloadJob=m_channel->downloadFile(QString("%1").arg(m_parameters.Path),"../pltDemo/test3.tar.gz",SftpOverwriteExisting);
    else if(m_parameters.isUpload)
        const SftpJobId uploadJob=m_channel->uploadFile(m_parameters.localPath,m_parameters.Path,SftpOverwriteExisting);
    else
    {
        timer = new QTimer(this);
        timer->setInterval(1000);
        connect(timer,SIGNAL(timeout()),this,SLOT(handleChannelClosed()));
        timer->start();
        const SftpJobId stateJob = m_channel->statFile(m_parameters.Path);
    }
}

void SftpTest::handleChannelInitializationFailure(const QString &reason)
{
    std::cerr << "Could not initialize SFTP channel: " << qPrintable(reason)
        << "." << std::endl;
    earlyDisconnectFromHost();
}

void SftpTest::handleChannelClosed()
{
    if (m_state != ChannelClosing) {
        std::cerr << "Unexpected state " << m_state << " in function "
            << Q_FUNC_INFO << "." << std::endl;
    } else {
        std::cout << "SFTP channel closed. Now disconnecting..." << std::endl;
    }
    m_state = Disconnecting;
    m_connection->disconnectFromHost();
    /*************************/
    //QApplication::closeAllWindows();
    exit(0);
    /*************************/
}

void SftpTest::handleJobFinished(SftpJobId job, const QString &error)
{
    isDownTwo++;
    if(m_parameters.isUpload||m_parameters.isDownLoad){
        //QApplication::closeAllWindows();
        exit(0);
    }else
    {
        if(isDownTwo>=3)
        {
            //QApplication::closeAllWindows();
            exit(0);
        }
    }
}

void SftpTest::handleFileInfo(SftpJobId job, const QList<SftpFileInfo> &fileInfoList)
{
//    system("pause");
//    m_state = CheckingDirAttributes;
//    switch (m_state) {
//    case CheckingDirAttributes: {
//        static int count = 0;
//        if (!checkJobId(job, m_statDirJob, "checking directory attributes"))
//            return;
//        if (++count > 1) {
//            std::cerr << "Error: More than one reply for directory attributes check." << std::endl;
//            earlyDisconnectFromHost();
//            return;
//        }
//        m_dirInfo = fileInfoList.first();
//        break;
//    }
//    case CheckingDirContents:
//        if (!checkJobId(job, m_lsDirJob, "checking directory contents"))
//            return;
//        m_dirContents << fileInfoList;
//        break;
//    default:
//        std::cerr << "Error: Unexpected file info in state " << m_state << "." << std::endl;
//        earlyDisconnectFromHost();
//    }
//    if(job == QSsh::SftpInvalidJob)
//        qDebug()<<"job无效！！！";
    //qDebug()<<"hhhhhh";
    //system("pause");
    //-----------------------------------------------------------------------//
    timer->stop();
    //-----------------------------------------------

    //system("pause");
    //-----------------------------------------------
    QString filePath = m_parameters.Path.mid(m_parameters.Path.size()-21,21);
    qDebug()<<"filePath:"<<filePath;
    //将modes_check.txt同步进行下载
    QString model_check = m_parameters.Path.mid(0,m_parameters.Path.size()-21);
    model_check+="modes_check.txt";
    qDebug()<<"model_check:"<<model_check;
    //system("pause");

    /*************************************2023_2_20***************************/
    const SftpJobId downloadJob=m_channel->downloadFile(m_parameters.Path,
                                                        QString("%1/pltDemo/%2").arg(QDir::currentPath())
                                                        .arg(filePath),SftpOverwriteExisting);


    const SftpJobId downloadModelJob=m_channel->downloadFile(model_check,
                                                             QString("%1/pltDemo/modes_check.txt").arg(QDir::currentPath())
                                                             ,SftpOverwriteExisting);

//    const SftpJobId downloadJob=m_channel->downloadFile(m_parameters.Path,
//                                                        QString("D:/QtProject/testDev701_10_30/Dev701QtNSPlatformBase/binForPublic/pltDemo/%1")
//                                                        .arg(filePath),SftpOverwriteExisting);


//    const SftpJobId downloadModelJob=m_channel->downloadFile(model_check,
//                                                             QString("D:/QtProject/testDev701_10_30/Dev701QtNSPlatformBase/binForPublic/pltDemo/modes_check.txt")
//                                                             ,SftpOverwriteExisting);


    //const SftpJobId downloadJobModel = m_channel->downloadFile
}

void SftpTest::removeFile(const FilePtr &file, bool remoteToo)
{
    if (!file)
        return;
    const QString localFilePath = file->fileName();
    file->remove();
    QFile::remove(cmpFileName(localFilePath));
    if (remoteToo && m_channel
            && m_channel->state() == SftpChannel::Initialized)
        m_channel->removeFile(remoteFilePath(QFileInfo(localFilePath).fileName()));
}

QString SftpTest::cmpFileName(const QString &fileName) const
{
    return fileName + QLatin1String(".cmp");
}

QString SftpTest::remoteFilePath(const QString &localFileName) const
{
    return QLatin1String("/tmp/") + localFileName + QLatin1String(".upload");
}

void SftpTest::earlyDisconnectFromHost()
{
    m_error = true;
    removeFiles(true);
    if (m_channel)
        disconnect(m_channel.data(), 0, this, 0);
    m_state = Disconnecting;
    m_connection->disconnectFromHost();
    /*************************/
    //QApplication::closeAllWindows();
    exit(0);
    /*************************/
}

bool SftpTest::checkJobId(SftpJobId job, SftpJobId expectedJob, const char *activity)
{
    if (job != expectedJob) {
        std::cerr << "Error " << activity << ": Expected job id " << expectedJob
           << ", got job id " << job << '.' << std::endl;
        earlyDisconnectFromHost();
        return false;
    }
    return true;
}

void SftpTest::removeFiles(bool remoteToo)
{
    foreach (const FilePtr &file, m_localSmallFiles)
        removeFile(file, remoteToo);
    removeFile(m_localBigFile, remoteToo);
}

bool SftpTest::handleJobFinished(SftpJobId job, JobMap &jobMap,
    const QString &error, const char *activity)
{
    JobMap::Iterator it = jobMap.find(job);
    if (it == jobMap.end()) {
        std::cerr << "Error: Unknown job " << job << "finished."
            << std::endl;
        earlyDisconnectFromHost();
        return false;
    }
    if (!error.isEmpty()) {
        std::cerr << "Error " << activity << " file " << qPrintable(it.value())
            << ": " << qPrintable(error) << "." << std::endl;
        earlyDisconnectFromHost();
        return false;
    }
    jobMap.erase(it);
    return true;
}

bool SftpTest::handleJobFinished(SftpJobId job, SftpJobId expectedJob, const QString &error,
    const char *activity)
{
    if (!checkJobId(job, expectedJob, activity))
        return false;
    if (!error.isEmpty()) {
        std::cerr << "Error " << activity << ": " << qPrintable(error) << "." << std::endl;
        earlyDisconnectFromHost();
        return false;
    }
    return true;
}

bool SftpTest::handleBigJobFinished(SftpJobId job, SftpJobId expectedJob,
    const QString &error, const char *activity)
{
    if (job != expectedJob) {
        std::cerr << "Error " << activity << " file '"
           << qPrintable(m_localBigFile->fileName())
           << "': Expected job id " << expectedJob
           << ", got job id " << job << '.' << std::endl;
        earlyDisconnectFromHost();
        return false;
    }
    if (!error.isEmpty()) {
        std::cerr << "Error " << activity << " file '"
            << qPrintable(m_localBigFile->fileName()) << "': "
            << qPrintable(error) << std::endl;
        earlyDisconnectFromHost();
        return false;
    }
    return true;
}

bool SftpTest::compareFiles(QFile *orig, QFile *copy)
{
    bool success = orig->size() == copy->size();
    qint64 bytesLeft = orig->size();
    orig->seek(0);
    while (success && bytesLeft > 0) {
        const qint64 bytesToRead = qMin(bytesLeft, Q_INT64_C(1024*1024));
        const QByteArray origBlock = orig->read(bytesToRead);
        const QByteArray copyBlock = copy->read(bytesToRead);
        if (origBlock.size() != bytesToRead || origBlock != copyBlock)
            success = false;
        bytesLeft -= bytesToRead;
    }
    orig->close();
    success = success && orig->error() == QFile::NoError
        && copy->error() == QFile::NoError;
    if (!success) {
        std::cerr << "Error: Original file '" << qPrintable(orig->fileName())
            << "'' differs from downloaded file '"
            << qPrintable(copy->fileName()) << "'." << std::endl;
        earlyDisconnectFromHost();
    }
    return success;
}
