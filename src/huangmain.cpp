/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/
#pragma execution_character_set("utf-8")
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QTextCodec> 
#include <QSplashScreen>
#include <QTime>
#include <QFileInfo>
#include <QGLFormat>
#include <QScreen>
#include <QDesktopWidget>
#include "log4qt\logger.h"
#include "log4qt\propertyconfigurator.h"
#include "log4qt\fileappender.h"

#include "huangmainwindow.h"

#include "huangglobalconfig.h"

//#include "gridpreprocess/huangzbPlot3Dcommon.h"
//#include "postprocessQGLV/huangzbPLTReader.h"
//
//#include "gridpreprocess/huangzbPlot3DFormattedReader.h"
#include <vtkAutoInit.h>
#include <vtkOutputWindow.h>
#include <QLoggingCategory>
VTK_MODULE_INIT(vtkRenderingOpenGL2)
extern QString appComDir;
void huangzbsleep(unsigned int msec) 
{ 
	
	QTime dieTime = QTime::currentTime().addMSecs(msec); 
	while( QTime::currentTime() < dieTime ) 
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100); 
}

int main(int argc, char *argv[])
{
	

	//QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
	vtkOutputWindow::SetGlobalWarningDisplay(0);
	Q_INIT_RESOURCE(Dev701QtNSPlatformBaseMain);
	/*QApplication::setColorSpec( QApplication::CustomColor );
	QFont font(QStringLiteral("Tahoma"));
	font.setPixelSize(14);
	qApp->setFont(font);*/

	QApplication::setColorSpec(QApplication::CustomColor);
	QFont font(QStringLiteral("Microsoft Yahei"));
	font.setPixelSize(14);
	qApp->setFont(font);

	QApplication app(argc, argv);
	//double stdWidth = 2880, stdHeight = 1800;
	//QScreen *screen = QGuiApplication::primaryScreen();
	//int currentScreenWid = screen->size().width();
	//int currentScreenHei = screen->size().height();
	//double factorX = currentScreenWid / stdWidth, factorY = currentScreenHei / stdHeight;
	//auto scaleWidgets=[&](auto&& scaleWidgets,QWidget *parent, qreal widthScaleFactor, qreal heightScaleFactor)
	//{
	//	// 遍历所有子控件
	//	foreach(QObject *child, parent->children()) {
	//		QWidget *widget = qobject_cast<QWidget *>(child);
	//		if (widget) {
	//			// 获取控件的原始大小
	//			QSize originalSize = widget->sizeHint();

	//			// 设置新的大小
	//			widget->setFixedSize(
	//				originalSize.width() * widthScaleFactor,
	//				originalSize.height() * heightScaleFactor
	//			);

	//			// 递归地调整子控件
	//			scaleWidgets(scaleWidgets,widget, widthScaleFactor, heightScaleFactor);
	//		}
	//	}
	//};
	
	/*if (screen) {
		
		qDebug() << "Screen Width:" << currentScreenWid;
		qDebug() << "Screen Height:" << currentScreenHei;
	}
	else {
		qDebug() << "Failed to get screen information.";
	}*/

	QStringList args = QCoreApplication::arguments(); 

	QTranslator oTranslator;
	oTranslator.load(":/qt_zh_CN"); 

	app.installTranslator(&oTranslator);


	QString workpath = QCoreApplication::applicationDirPath();
	QDir::setCurrent(workpath);

	Log4Qt::PropertyConfigurator::configure(app.applicationDirPath() + "/huanglog4qt.conf");
	Log4Qt::Logger * logger=Log4Qt::Logger::logger("main");
	logger->info("\n\n####################################################################\n");
	logger->info("Application Starting.....");
	//logger->info(QString("sizeof(int)=%1").arg(sizeof(int)));
	
	app.setOrganizationName("ZhidaoSC");
	app.setApplicationName("CAAA/SSPNS");
	

	QSplashScreen *splash = new QSplashScreen;
	splash->setPixmap(QPixmap("images/splash3.PNG"));//设置图片
	Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
	//splash->show();
	app.processEvents();

	splash->showMessage(QObject::tr("建立主窗体..."),topRight, Qt::red);//显示信息
	huangzbsleep(100);

	splash->showMessage(QObject::tr("设置中文环境..."),topRight, Qt::red);//显示信息
	huangzbsleep(100);
	
	splash->showMessage(QObject::tr("加载模块..."),topRight, Qt::red); //显示信息
	huangzbsleep(100);




    MainWindow mainWin;
	mainWin.setWindowIcon(QIcon(":/images/default.ico"));
	
	//double stdWidth = 2880, stdHeight = 1800;
	//stdWidth = 500, stdHeight = 500;
	//QScreen *screen = QGuiApplication::primaryScreen();
	//int currentScreenWid = screen->size().width();
	//int currentScreenHei = screen->size().height();
	//double factorX = currentScreenWid / stdWidth, factorY = currentScreenHei / stdHeight;
	//auto scaleWidgets = [&](auto&& scaleWidgets, QWidget *parent, qreal widthScaleFactor, qreal heightScaleFactor)->void
	//{
	//	// 遍历所有子控件
	//	foreach(QObject *child, parent->children()) {
	//		QWidget *widget = qobject_cast<QWidget *>(child);
	//		if (widget) {
	//			// 获取控件的原始大小
	//			QSize originalSize = widget->sizeHint();

	//			// 设置新的大小
	//			widget->setFixedSize(
	//				originalSize.width() * widthScaleFactor,
	//				originalSize.height() * heightScaleFactor
	//			);

	//			// 递归地调整子控件
	//			scaleWidgets(scaleWidgets, widget, widthScaleFactor, heightScaleFactor);
	//		}
	//	}
	//};
	//scaleWidgets(scaleWidgets,&mainWin, factorX, factorY);

	app.setWindowIcon(QIcon(":/images/default.ico"));
	QTextCodec*codec = QTextCodec::codecForName("GBK");

	
	//mainWin.setWindowTitle(u8"3D流场交互式可视化分析系统");
	mainWin.setWindowTitle(u8"高速粘性流场PNS计算软件平台开发");
    mainWin.showMaximized();
	//mainWin.loadStyleSheet(appComDir + "/images/SSPNS/CSS/mainwindow.css");
	splash->finish(&mainWin);//图片一直显示到mainWin加载完成
	delete splash;//释放空间，节省内存
    return app.exec();
}
