/****************************************************************************

Copyright (C) 2002-2011 zhidaofangyuan.  All rights reserved.

*****************************************************************************/

#ifndef HUANGZB_GLOBAL_OBJECT_H_
#define HUANGZB_GLOBAL_OBJECT_H_

#include <QMap>
#include <QClipboard>
#include <QObject>
#include <QTimer> 
#include <QString>
#include <QDateTime>
#include <QEvent>
#include <math.h>
#include <iostream> 
#include <QDomElement>
#include <QFile> 

#include <QDomDocument>
#include <QtXml>
#include <QXmlStreamReader>
#include <string>

#include "log4qt\logger.h"
#include "log4qt\propertyconfigurator.h"
#include "qtcommon.h"
using namespace std;

#ifdef UNICODE

#define QStringToTCHAR(x)     (wchar_t*) x.utf16()
#define PQStringToTCHAR(x)    (wchar_t*) x->utf16()
#define TCHARToQString(x)     QString::fromUtf16((x))
#define TCHARToQStringN(x,y)  QString::fromUtf16((x),(y))

#else

#define QStringToTCHAR(x)     x.local8Bit().constData()
#define PQStringToTCHAR(x)    x->local8Bit().constData()
#define TCHARToQString(x)     QString::fromLocal8Bit((x))
#define TCHARToQStringN(x,y)  QString::fromLocal8Bit((x),(y))

#endif

struct GlobalVariableForGrid{
	double nRange;
	bool bFlatOrSmooth;
	bool bKDir;
	bool bIDir;
	bool bJDir;
	bool bShowShader;
	bool bShowEdge;
	bool bShowGrid;

	int nShowMode;//0=DrawLod,1=DrawGrid....	
};

struct GlobalVariableForPost{
	double nRange;
	bool bFlatOrSmooth;
	bool bKDir;
	bool bIDir;
	bool bJDir;
	bool bShowShader;
	bool bShowEdge;
	bool bShowGrid;

	int nShowMode;//0=DrawLod,1=DrawGrid....

	int curVarIdx;
	int curZoneIdx;//-1:All Zone;0,1,2....
	bool bShowBox;
	bool bShowLegend;
	bool bShowLight;

	bool bShowAxis;

	int IJKindex;
	int IJKvalue;

};

/*
struct GlobalVariableForPostProcess{
	double nRange;

	
	bool bFlatOrSmooth;
};
*/

struct GlobalVariableForRunParam{
	bool bFlag;
	QString gridfilename;  //网格文件

	QString gridInspfilename; //网格边界文件
	int gridtype;
	double scaleEdit;

	double maEdit;
	double reEdit;
	double tfEdit;
	double alEdit;
	double btEdit;
	double twallEdit;

	int ideal_nonequ_Radio;
	int none_full_finiteRadio;
	double cataEdit;

	int lam_tur_Radio;
	int sa_kw_sstRadio;

	double mutEdit;
	double tuEdit;

	int adi_tem_Radio;

	double wallEdit;

	int new_old_Radio;
	int irst;
	int mcyc;
	double eps;

	int ivis;
	int iturb;
	int ilimit;
	int iflux;
	int itransit;
	int iblank;

	int stepEdit;
	int tempEdit;

	int schemeBox;

	int first_second_Radio;

	int limiterBox;

	double cflEdit;

	int ZF579;//

};


class HuangzbGlobalConfig:public QObject
{
	LOG4QT_DECLARE_QCLASS_LOGGER
private:
	QString copyrightStamp;

public:
	QString author;
	QString release;

	QString stateFileName_;
	QString gridbgimage;
	QString lastrun;

	QString clusteripstr;
	QString clusteripprestr;
	QString clusterportstr;
	QString clusterappdirstr;

	QString clusterwqexeconfig;
	QString clusterfphexeconfig;
	QString clusterexetype;
	QString clusterlogfile;


public:
	//全局变量
	double nRange;
	struct GlobalVariableForGrid cparam; //用于网格显示时的配置
	struct GlobalVariableForPost pparam; //用于后处理器的参数配置
	bool bFullGridShow;

	QStringList GlobalListForInsp;
	//QMap<int, int> inspShowMapByFenQuAndLineNo;
	//struct GlobalVariableForPostProcess cpostparam; //用于后处理时的配置

	//所有的操作都要先设置一个网格文件和一个边界文件.
	QString gridfilename;  //网格文件
	QString gridfilenameBasePath;
	QString gridInspfilename; //网格边界文件

	struct GlobalVariableForRunParam runparam;

	//QString exeFileName;
	QString winwqexeconfig;
	QString winfphexeconfig;
public :
	QString stateFileName() const;
	virtual QDomElement domElement(const QString& name, QDomDocument& document) const;
	void setStateFileName(const QString& name) { stateFileName_ = name; };

	HuangzbGlobalConfig();
	void defaultConstructor();
	void saveStateToFile();
	bool restoreStateFromFile();
	void initFromDOMElement(const QDomElement& element);

	QString s2q(const std::string &s);
	std::string q2s(const QString &s); 

	std::string change_file_ext(const std::string &filename,const std::string& new_ext);

	static int GetProcessidFromName(char name[]);


	static bool qCopyDirectory(const QDir& fromDir, const QDir& toDir, bool bCoverIfFileExists=false);

	static QString getTempFileNameByTime(QString prefix);

	static void huangzbsleep(unsigned int msec); 

	static QString ConvertDateTimeToString14(QDateTime& dt);

	static void deleteDirectory(QFileInfo fileList);

};


#endif // OBJECT_H_
