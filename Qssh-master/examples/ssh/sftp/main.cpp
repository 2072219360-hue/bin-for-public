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

#include "argumentscollector.h"
#include "sftptest.h"

#include <ssh/sftpchannel.h>
#include <ssh/sshconnection.h>

#include <QCoreApplication>
#include <QObject>
#include <QStringList>

#include <cstdlib>
#include <iostream>
#include <QLoggingCategory>
#include <QApplication>
#include <QDir>
int main(int argc, char *argv[])
{
    //.\sftp.exe -h 192.168.72.128 -u sqh -pwd sqh -path 2 D:\QtProject\demo\testProperty\main.cpp /home/sqh/inp.d
    //.\sftp.exe -h 101.42.159.79 -u sqh -pwd sqh -path  1 /home/sqh/test/day_mesh.tar.gz
    //.\sftp.exe -h 101.42.159.79 -u sqh -pwd sqh -path  2 D:\QtProject\testDev701_10_30\Dev701QtNSPlatformBase\binForPublic\INP.D /home/sqh/inp.d
    //.\sftp.exe -h 101.42.159.79 -u sqh -pwd sqh -path  3 /home/sqh/Aeroelastic_software_demon/double_body_demo/test_Ma0.50_aoa0_H0/dyn_mesh/bnd_morph00000001.plt
    //设置过滤信息，debug类型的信息不打印
    QLoggingCategory::setFilterRules("*.debug=false");
    QCoreApplication app(argc, argv);
    bool parseSuccess;
    //获取参数并进行解析返回值为parseSuccess
    Parameters parameters = ArgumentsCollector(app.arguments()).collect(parseSuccess);
    //qCDebug(sshLog, "sftp subsystem initialized");
    if (!parseSuccess)
        return EXIT_FAILURE;
    SftpTest sftpTest(parameters);
    sftpTest.run();
    return app.exec();
}
