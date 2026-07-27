/****************************************************************************
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtGui/QtGui"
#include <QCloseEvent>

#include "log4qt\logger.h"
#include "log4qt\propertyconfigurator.h"
#include <vtkDataSet.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <QVTKOpenGLNativeWidget.h>
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataSetReader.h>
#include <vtkPolyDataReader.h>
#include "InputSet.h"
#include"PostProcessProfiling.h"
#include "huangglobalconfig.h"
#include "qtcommon.h"
#include "scenewidget.h"
#include "Widget.h"
#include "postgridsimplify.h"
#include <Windows.h>
#include "TrTableShow.h"
#include"SSPNS.h"
#include"BatchSSPNS.h"
#include"ECHV.h"
#include"SignalInstance.h"
#include<unordered_map>
using namespace platforms;
using namespace sspns;
using platforms::sspns::reveal::SceneWidget;

using namespace platforms;
using namespace sspns;
using namespace echv;
using platforms::sspns::reveal::InputHeader;
using platforms::sspns::reveal::InputSet;
using platforms::sspns::reveal::TrTableShow;

extern HuangzbGlobalConfig gblcfg;
extern Widget* generateWindow;
//extern int indexPlt;
extern QString testDir;
extern SceneWidget* sceneWidget;
extern SceneWidget* gridshowviewer;
extern int addIndex;
extern bool boolisLoaded;


QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;

QT_END_NAMESPACE

//! [0]
class MyWidget : public QWidget {
	Q_OBJECT

public:
	MyWidget(QWidget *parent = nullptr) : QWidget(parent) {
		resize(parent->size());

	}

protected:
	void paintEvent(QPaintEvent *event) override {
		QWidget::paintEvent(event);
		QPainter painter(this);

		// 加载背景图片
		QPixmap backgroundPixmap("images/splash.PNG");

		// 获取窗口大小
		QSize windowSize = size();

		// 将图片缩放到窗口大小
		QPixmap scaledPixmap = backgroundPixmap.scaled(windowSize, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

		// 绘制背景图片
		painter.drawPixmap(0, 0, scaledPixmap);
	}

	void resizeEvent(QResizeEvent *event) override {
		// 触发重绘
		update();
		QWidget::resizeEvent(event);
	}
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
	LOG4QT_DECLARE_QCLASS_LOGGER
///////////////////////////////////////////////////// ///////////////////////////
private:
	bool bNetworkStatus;
	QStackedWidget *stackedWidget;
	SSPNS* sspns;
	SSPNS* pns;
	BatchSSPNS* batchSSPNS;
	std::unordered_map<string, int> widgetIndex;
	int currentWidgetIndex;
public:
    MainWindow();
	~MainWindow();
	//void setCentralWidget(QWidget *widget);
	//PostGridSimplify* postgridsimplify;
private slots:
	void changeStyle();
	void showRWMessage(QString messageInfo);
	//void preference();
	void inspParamDo();
	void runParamDo();
	void basedataDo();
	void gridLoad();
	void post2DCanvasDo();
	void slotShowPNS();
	//void Mesh3DPost();
	void PostBasicFrameShow();

	void runBrowserDo();
	void runLocalMPIDo();
	void runClusterMPIDo();
	void resultBrowserDo();
	void gridshowOptions();
	void slotComputeOp();
protected:
	void paintEvent (QPaintEvent * event);
	void closeEvent(QCloseEvent *event);

public:
	
	bool boolgridshowOptions;
	bool boolinspParamTable;

	bool boolPostBasicFrameShow;
	//THuangTableForInsp *pal;
	QDockWidget *dockRightGraphforpal;


	bool boolOrbiterForDBSqlite;
	bool boolNSAppRunParam;

	bool boolDataResultBrowser;

	bool boolMPILocalJobBrowser;
	bool boolMPILog;

	bool boolMPIclusterJobBrowser;
	bool boolMPIclusterBaseCfg;
	bool boolVtkBrowser;
	bool boolMPIclusterInfoBrowser;
	
	bool boolGenerateOp;
	bool boolMakeOp;
	bool boolArguOp;
	bool boolMeshPre;
	bool boolComputeOp;

    void createActions();

    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
	void SetBgImage(const QString &strPath);
	void loadStyleSheet(QString sheetName);
	//SceneWidget *gridshowviewer;
    QListWidget *customerList;
    QListWidget *paragraphsList;
	QListWidget *infoList;

    QString m_strPath;

    QMenu *fileMenu;
	QMenu *filesubStyleMenu;
	QMenu *filesubWINMPIMenu;
	QMenu *filesubClusterMenu;

    QMenu *editMenu;
	QMenu *gridMenu;
		QAction *gridLoadAct;
		QAction *inspParamAct;
		QAction *runParamAct;
		QAction *basedataAct;

	QMenu *calcMenu;
		QAction *runBrowserAct;
		QAction *runLocalMPIAct;
		QAction *runClusterMPIAct;
		//QAction *resultBrowserAct;
	/// 后处理
	QMenu *postProMenu;
		
	QMenu *postMenu;
		QAction *post3DAct;
		QAction *post2DCanvasAct;

    QMenu *viewMenu;
    QMenu *helpMenu;
	QMenu *vtkTestMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *aboutAct;
    QAction *aboutQtAct;
	QAction *vtkFileOpen;

	//////////////////////////////FOR FILE MENU//////////////////////////////////
    QAction *quitAct;
	QAction *fileSaveLayoutAct;
	QAction *fileLoadLayoutAct;
	QAction *gblconfigAct;

	QAction *mpiLocalJobBrowserAct;
	QAction *mpiClusterJobBrowserAct;
	
	//SceneWidget*sceneWidget;

	QWidget *centralwidget;
	PostProcessProfiling* profilingWindow;
	void setCentral(QWidget* widget);
private slots:
	void slotShowProfiling();

	void mpi00EnvSetup();
	void mpiO1CreateMPD();
	void mpiO2MPIRegister();
	void mpiO3MPIConfig();
	void mpiO4MPITest();
	void mpiO5MPILocalJobBrowser();
	void clusterMPIJobBrowser();

	void vtkShow();

	void clusternetwork();
	void clusterconfig();
	void clusterbrowser();
	void OndataresultBrowser();
	void generateOp();
	void slotMakeOp();
	void slotMeshPre();
	void inputFileSet();
	void slotShowSSPNS();
	void slotShowBatchSSPNS();
	void slotShowECHV();
	void slotShowPostProcessGridSimplify();
	void slotClearProcessRes();
//////////////////////FOR TEST SUPPORT//////////////////////////////////////////
public:
	//Widget* generateWindow;
private:
	
	QMenu *testMenu;
	QMenu *testQwtMenu;
	QMenu *testQwtPlot3DMenu;
	QMenu *testQGLVMenu;
	QMenu *testQtGLMenu;

	//QAction *newLetterAct;
	//QAction *saveAct;
	//QAction *printAct;
	//QAction *undoAct;
	QAction *diagModalTstAct;

	//dockwidget测试
	//FindWidget * m_findWidget;
	//ConfigDialog *faderdialog;
    //QTreeWidget *foldersTreeWidget;

	//对话框显示测试
	//DiagExtDemo *modalDiag;

private slots:
	void about();
protected:

///////////////////////////////////////////////////////////////////////////////////
};
//! [0]
class testQSplitterDialog:public QDialog
{
public:
	testQSplitterDialog();
};

#endif
