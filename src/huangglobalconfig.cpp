/****************************************************************************

 Copyright (C) 2002-2011 zhidaofangyuan.  All rights reserved.

*****************************************************************************/
#include <QMessageBox>
#include <QDir>
#include <QFileInfoList>

#include <string>
#include <qgl.h>
#include "huangglobalconfig.h"
//#include "huangglobaldomUtils.h"

#include <windows.h>
#include <TlHelp32.h>

using namespace std;

void HuangzbGlobalConfig::defaultConstructor()
{
	author="黄智濒";
	release="2012-06-01";
	copyrightStamp="2012-06-01";

	cparam.bFlatOrSmooth=true;
	cparam.bIDir=true;
	cparam.bJDir=true;
	cparam.bKDir=true;
	cparam.bShowGrid=true;
	cparam.bShowEdge=true;

	cparam.nShowMode=0;

	pparam.curVarIdx =0;
	pparam.curZoneIdx =-1;
	pparam.bShowBox = true;
	pparam.bShowLegend = true;
	pparam.bShowLight = false;
	pparam.bShowAxis = false;
	pparam.IJKindex =1;//1=I;2=J;3=K
	pparam.IJKvalue = 1;

	clusteripstr="10.1.5.20";
	clusteripprestr="10.1.5";
	clusterportstr="7011";
	clusterappdirstr="/nfs/agentCAAA/";
	clusterlogfile="res.dat";
}

HuangzbGlobalConfig::HuangzbGlobalConfig()
{

	//defaultConstructor();
	//setStateFileName("globalconfig.xml");

	//bFullGridShow=true;

	gridfilename="9999";
	gridInspfilename="9999";
}

void HuangzbGlobalConfig::initFromDOMElement(const QDomElement& element)
{
	const QString version = element.attribute("version");
	logger()->info("HuangzbGlobalConfig.....initFromDOMElement...");

	QDomElement child=element.firstChild().toElement();
	while (!child.isNull())
	{
		if (child.tagName() == "copyright")
		{
			// #CONNECTION# default values from defaultConstructor()
			// setMouseTracking(DomUtils::boolFromDom(child, "mouseTracking", false));
			//setStereoDisplay(DomUtils::boolFromDom(child, "stereo", false));
			//if ((child.attribute("cameraMode", "revolve") == "fly") && (cameraIsInRevolveMode()))
			//	toggleCameraMode();
			if (child.attribute("stamp","2000-01-01")!="2012-06-01")
				continue;

			copyrightStamp=child.attribute("stamp","2000-01-01");

			QDomElement ch=child.firstChild().toElement();
			while (!ch.isNull())
			{
				if (ch.tagName() == "author")
				{
					author=ch.attribute("value","黄智濒");
					logger()->info(author);
				}
				if (ch.tagName() == "release")
				{
					release=ch.attribute("value","2012-09-01");
					logger()->info(release);
				}
			//	if (ch.tagName() == "backgroundColor")
			//		setBackgroundColor(DomUtils::QColorFromDom(ch));
				ch = ch.nextSibling().toElement();
			}

		}

		if (child.tagName() == "mainui")
		{
			//if (child.attribute("stamp","2000-01-01")=="2012-06-01")
			//	continue;
			//else
			logger()->info("维护日期："+child.attribute("value"," "));
			QDomElement ch=child.firstChild().toElement();
			while (!ch.isNull())
			{
				if (ch.tagName() == "gridbgimage")
				{
					gridbgimage=ch.attribute("value","bg.jpg");
					logger()->info(ch.attribute("value","bg.jpg"));
				}

				if (ch.tagName() == "winwqexe")
				{
					winwqexeconfig=ch.attribute("value","wq.exe");
					logger()->info(ch.attribute("value","wq.exe"));
				}

				if (ch.tagName() == "winfphexe")
				{
					winfphexeconfig=ch.attribute("value","fph.exe");
					logger()->info(ch.attribute("value","fph.exe"));
				}

				ch = ch.nextSibling().toElement();
			}

		}

		if (child.tagName() == "cluster")
		{
			//if (child.attribute("stamp","2000-01-01")=="2012-06-01")
			//	continue;
			//else
			logger()->info("集群配置："+child.attribute("value"," "));
			QDomElement ch=child.firstChild().toElement();
			while (!ch.isNull())
			{
				if (ch.tagName() == "clusterip")
				{
					clusteripstr=ch.attribute("value","192.168.88.132");
					logger()->info(ch.attribute("value","192.168.88.132"));
				}

				if (ch.tagName() == "clusterippre")
				{
					clusteripprestr=ch.attribute("value","192.168.88.132");
					logger()->info(ch.attribute("value","192.168.88.132"));
				}

				if (ch.tagName() == "clusterport")
				{
					clusterportstr=ch.attribute("value","7011");
					logger()->info(ch.attribute("value","7011"));
				}

				if (ch.tagName() == "clusterappdir")
				{
					clusterappdirstr=ch.attribute("value","/nfs/agentCAAA/");
					logger()->info(ch.attribute("value","/nfs/agentCAAA/"));
				}

				if (ch.tagName() == "clusterwqexe")
				{
					clusterwqexeconfig=ch.attribute("value","wq");
					logger()->info(ch.attribute("value","wq.exe"));
				}

				if (ch.tagName() == "clusterfphexe")
				{
					clusterfphexeconfig=ch.attribute("value","fph");
					logger()->info(ch.attribute("value","fph.exe"));
				}

				if (ch.tagName() == "clusterlogfile")
				{
					clusterfphexeconfig=ch.attribute("value","res.dat");
					logger()->info(ch.attribute("value","res.dat"));
				}

				ch = ch.nextSibling().toElement();
			}

		}

/*
		if (child.tagName() == "Geometry")
		{
			setFullScreen(DomUtils::boolFromDom(child, "fullScreen", false));

			if (isFullScreen())
			{
				prevPos_.setX(DomUtils::intFromDom(child, "prevPosX", 0));
				prevPos_.setY(DomUtils::intFromDom(child, "prevPosY", 0));
			}
			else
			{
				int width  = DomUtils::intFromDom(child, "width",  600);
				int height = DomUtils::intFromDom(child, "height", 400);
				topLevelWidget()->resize(width, height);
				camera()->setScreenWidthAndHeight(this->width(), this->height());

				QPoint pos;
				pos.setX(DomUtils::intFromDom(child, "posX", 0));
				pos.setY(DomUtils::intFromDom(child, "posY", 0));
				topLevelWidget()->move(pos);
			}
		}

		if (child.tagName() == "Camera")
		{
			connectAllCameraKFIInterpolatedSignals(false);
			camera()->initFromDOMElement(child);
			connectAllCameraKFIInterpolatedSignals();
		}

		if ((child.tagName() == "ManipulatedFrame") && (manipulatedFrame()))
			manipulatedFrame()->initFromDOMElement(child);
*/

		child = child.nextSibling().toElement();
	}

}

QString HuangzbGlobalConfig::stateFileName() const
{
	QString name = stateFileName_;

	if (!name.isEmpty())
	{
		QFileInfo fi(name);
		name = fi.absolutePath() + '/' + fi.completeBaseName() + "." + fi.suffix();
	}
	return name;
}


void HuangzbGlobalConfig::saveStateToFile()
{
	QString name = stateFileName();

	if (name.isEmpty())
		return;

	QFileInfo fileInfo(name);

	if (fileInfo.isDir())
	{
		//QMessageBox::warning(this, tr("Save to file error", "Message box window title"), tr("State file name (%1) references a directory instead of a file.").arg(name));
		logger()->warn(tr("State file name (%1) references a directory instead of a file.").arg(name));
		return;
	}

#if QT_VERSION >= 0x040000
	const QString dirName = fileInfo.absolutePath();
#else
	const QString dirName = fileInfo.dirPath();
#endif
	if (!QFileInfo(dirName).exists())
	{
		QDir dir;
#if QT_VERSION >= 0x040000
		if (!(dir.mkdir(dirName)))
#else
		if (!(dir.mkdir(dirName, true)))
#endif
		{
			//QMessageBox::warning(this, tr("Save to file error", "Message box window title"), tr("Unable to create directory %1").arg(dirName));
			logger()->warn(tr("State file name (%1) references a directory instead of a file.").arg(name));
			return;
		}
	}

	// Write the DOM tree to file
	QFile f(name);
#if QT_VERSION >= 0x040000
	if (f.open(QIODevice::WriteOnly))
#else
	if (f.open(IO_WriteOnly))
#endif
	{
		QTextStream out(&f);

		QDomDocument doc("HuangZhiBinGlobalConfig");

		QDomProcessingInstruction instruction;  //添加处理指令
		//instruction = doc.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
		//doc.appendChild(instruction);
		doc.appendChild(domElement("HGBLCONFIG", doc));

		doc.save(out, 2);

		f.flush();
		f.close();
	}
	else
#if QT_VERSION < 0x030200
		QMessageBox::warning(this, tr("Save to file error", "Message box window title"), tr("Unable to save to file %1").arg(name));
#else
		logger()->warn(tr("Unable to save to file %1").arg(name) + ":\n" + f.errorString());
		//QMessageBox::warning(this, tr("Save to file error", "Message box window title"), tr("Unable to save to file %1").arg(name) + ":\n" + f.errorString());
#endif
}


bool HuangzbGlobalConfig::restoreStateFromFile()
{
	QString name = stateFileName();

	if (name.isEmpty())
		return false;

	QFileInfo fileInfo(name);

	if (!fileInfo.isFile())
		// No warning since it would be displayed at first start.
		return false;

	if (!fileInfo.isReadable())
	{
		//QMessageBox::warning(this, tr("Problem in state restoration", "Message box window title"), tr("File %1 is not readable.").arg(name));
		logger()->warn(tr("File %1 is not readable.").arg(name));
		return false;
	}

	// Read the DOM tree form file
	QFile f(name);
#if QT_VERSION >= 0x040000
	if (f.open(QIODevice::ReadOnly) == true)
#else
	if (f.open(IO_ReadOnly) == true)
#endif
	{
		QDomDocument doc;
		doc.setContent(&f);
		f.close();
		QDomElement main = doc.documentElement();
		initFromDOMElement(main);
	}
	else
	{
#if QT_VERSION < 0x030200
		QMessageBox::warning(this, tr("Open file error", "Message box window title"), tr("Unable to open file %1").arg(name));
#else
		logger()->warn(tr("Unable to open file %1").arg(name) + ":\n" + f.errorString());
		//QMessageBox::warning(this, tr("Open file error", "Message box window title"), tr("Unable to open file %1").arg(name) + ":\n" + f.errorString());
#endif
		return false;
	}

	return true;
}


QDomElement HuangzbGlobalConfig::domElement(const QString& name, QDomDocument& document) const
{
	QDomElement de = document.createElement(name);
	de.setAttribute("version", "0.8.19");

	QDomElement copyrightNode = document.createElement("copyright");
	//stateNode.appendChild(DomUtils::QColorDomElement(foregroundColor(), "foregroundColor", document));
	//stateNode.appendChild(DomUtils::QColorDomElement(backgroundColor(), "backgroundColor", document));
	//stateNode.setAttribute("stereo", (displaysInStereo()?"true":"false"));
	//stateNode.setAttribute("cameraMode", (cameraIsInRevolveMode()?"revolve":"fly"));
	copyrightNode.setAttribute("stamp",copyrightStamp);
	//copyrightNode.appendChild(DomUtils::QStringDomElement("HuangZhiBin(zhidaosc@gmail.com)黄智濒","value","author",document));
	//copyrightNode.appendChild(DomUtils::QStringDomElement(release,"value","release",document));
	//de.appendChild(copyrightNode);

	//QDomElement maiuiNode = document.createElement("mainui");
	////maiuiNode.setAttribute("stamp","2012-06-01");
	//QDateTime ctime,time; 
	//ctime = QDateTime::currentDateTime(); 
	//QString ss = ctime.toString(Qt::LocalDate);
	//maiuiNode.appendChild(DomUtils::QStringDomElement(ss,"value","lastrun",document));
	//maiuiNode.appendChild(DomUtils::QStringDomElement(gridbgimage,"value","gridbgimage",document));
	//maiuiNode.appendChild(DomUtils::QStringDomElement(winwqexeconfig,"value","winwqexe",document));
	//maiuiNode.appendChild(DomUtils::QStringDomElement(winfphexeconfig,"value","winfphexe",document));
	//de.appendChild(maiuiNode);

	//QDomElement clusterNode= document.createElement("cluster");
	//clusterNode.appendChild(DomUtils::QStringDomElement(clusteripstr,"value","clusterip",document));
	//clusterNode.appendChild(DomUtils::QStringDomElement(clusteripprestr,"value","clusterippre",document));
	//clusterNode.appendChild(DomUtils::QStringDomElement(clusterportstr,"value","clusterport",document));
	//clusterNode.appendChild(DomUtils::QStringDomElement(clusterappdirstr,"value","clusterappdir",document));
	//clusterNode.appendChild(DomUtils::QStringDomElement(clusterwqexeconfig,"value","clusterwqexe",document));
	//clusterNode.appendChild(DomUtils::QStringDomElement(clusterfphexeconfig,"value","clusterfphexe",document));
	//clusterNode.appendChild(DomUtils::QStringDomElement(clusterlogfile,"value","clusterlogfile",document));
	//de.appendChild(clusterNode);

/*
	QDomElement geometryNode = document.createElement("Geometry");
	geometryNode.setAttribute("fullScreen", (isFullScreen()?"true":"false"));
	if (isFullScreen())
	{
		geometryNode.setAttribute("prevPosX", QString::number(prevPos_.x()));
		geometryNode.setAttribute("prevPosY", QString::number(prevPos_.y()));
	}
	else
	{
		QWidget* tlw = topLevelWidget();
		geometryNode.setAttribute("width",  QString::number(tlw->width()));
		geometryNode.setAttribute("height", QString::number(tlw->height()));
		geometryNode.setAttribute("posX",   QString::number(tlw->pos().x()));
		geometryNode.setAttribute("posY",   QString::number(tlw->pos().y()));
	}
	de.appendChild(geometryNode);

	// Restore original Camera zClippingCoefficient before saving.
	if (cameraIsEdited())
		camera()->setZClippingCoefficient(previousCameraZClippingCoefficient_);
	de.appendChild(camera()->domElement("Camera", document));
	if (cameraIsEdited())
		// #CONNECTION# 5.0 from setCameraIsEdited()
		camera()->setZClippingCoefficient(5.0);

	if (manipulatedFrame())
		de.appendChild(manipulatedFrame()->domElement("ManipulatedFrame", document));
*/

	return de;
}

std::string HuangzbGlobalConfig::change_file_ext(const std::string &filename,const std::string& new_ext)
{
	int dot_pos = 0;
	std::string temp = filename;

	for( int i = 0; i < temp.length(); ++i)
	{
		if ( temp[i] == '.' )
		{            
			dot_pos = i;
		}
	}

	temp.erase(dot_pos);
	temp += new_ext;

	return temp;
}


QString HuangzbGlobalConfig::s2q(const std::string &s)
{
	return QString(QString::fromLocal8Bit(s.c_str()));
}

std::string HuangzbGlobalConfig::q2s(const QString &s)
{
	return std::string((const char *)s.toLocal8Bit());
}


int HuangzbGlobalConfig::GetProcessidFromName(char name[])
{
	PROCESSENTRY32 pe;
	DWORD id=0;
	HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	pe.dwSize=sizeof(PROCESSENTRY32);
	if(!Process32First(hSnapshot,&pe))
		return 0;
	while(1)
	{
		pe.dwSize=sizeof(PROCESSENTRY32);
		if(Process32Next(hSnapshot,&pe)==FALSE)
			break;
		//int ret=name.compare(QString((const QChar*)pe.szExeFile, strlen(pe.szExeFile)));
		//if(strcmp(pe.szExeFile,name)==0)
		////if (ret==0)
		//{
		//	id=pe.th32ProcessID;
		//	break;
		//}
	}
	CloseHandle(hSnapshot);
	return id;

}


/**
  qCopyDirectory -- 拷贝目录
  fromDir : 源目录
  toDir   : 目标目录
  bCoverIfFileExists : ture:同名时覆盖  false:同名时返回false,终止拷贝
  返回: ture拷贝成功 false:拷贝未完成
*/
bool HuangzbGlobalConfig::qCopyDirectory(const QDir& fromDir, const QDir& toDir, bool bCoverIfFileExists)
{
    QDir formDir_ = fromDir;
    QDir toDir_ = toDir;

    if(!toDir_.exists())
    {
        if(!toDir_.mkdir(toDir.absolutePath()))
            return false;
    }

    QFileInfoList fileInfoList = formDir_.entryInfoList();
    foreach(QFileInfo fileInfo, fileInfoList)
    {
        if(fileInfo.fileName() == "." || fileInfo.fileName() == "..")
            continue;

        //拷贝子目录
        if(fileInfo.isDir())
        {
            //递归调用拷贝
			if(!qCopyDirectory(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
                return false;
        }
        //拷贝子文件
        else
        {
            if(bCoverIfFileExists && toDir_.exists(fileInfo.fileName()))
            {
                toDir_.remove(fileInfo.fileName());
            }
            if(!QFile::copy(fileInfo.filePath(), toDir_.filePath(fileInfo.fileName())))
            {
                return false;
            }
        }
    }
    return true;

}

QString HuangzbGlobalConfig::getTempFileNameByTime(QString prefix)
{
	QDateTime ctime,time;
	ctime = QDateTime::currentDateTime();
	QString ss = ctime.toString("yyyyMMddhhmmsszzz");
	return prefix+ss;
}

void HuangzbGlobalConfig::huangzbsleep(unsigned int msec) 
{ 
	QTime dieTime = QTime::currentTime().addMSecs(msec); 
	while( QTime::currentTime() < dieTime ) 
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100); 
}

QString HuangzbGlobalConfig::ConvertDateTimeToString14(QDateTime& dt)
{
	int AYear, AMonth, ADay, AHour, AMinute, ASecond, AMilliSecond;
	QDate dd=dt.date();
	dd.getDate(&AYear,&AMonth,&ADay);

	QTime tt=dt.time();
	AHour=tt.hour();
	AMinute=tt.minute();
	ASecond=tt.second();
	AMilliSecond=tt.msec();

	QString result;
	char str[14];
	sprintf(str,"%4.4d%2.2d%2.2d%2.2d%2.2d%2.2d",AYear, AMonth, ADay, AHour, AMinute, ASecond);
	result=str;

	result = dt.toString("yyyyMMddhhmmss"); 
	return result;
}

void HuangzbGlobalConfig::deleteDirectory(QFileInfo fileList)
{
	if(fileList.isDir()){
		int childCount =0;
		QString dir = fileList.filePath();
		QDir thisDir(dir);
		childCount = thisDir.entryInfoList().count();
		QFileInfoList newFileList = thisDir.entryInfoList();
		if(childCount>2){
			for(int i=0;i<childCount;i++){
				if(newFileList.at(i).fileName().operator ==(".")|newFileList.at(i).fileName().operator ==("..")){
					continue;
				}
				deleteDirectory(newFileList.at(i));
			}
		}
		fileList.absoluteDir().rmpath(fileList.fileName());
	}else if(fileList.isFile()){
		fileList.absoluteDir().remove(fileList.fileName());
	}
}
