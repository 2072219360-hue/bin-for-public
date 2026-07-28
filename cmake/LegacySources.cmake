# Source inventory mirrored from Dev701QtNSPlatformBase.vcxproj.
# Keep this explicit: the repository contains many examples, backups, generated
# files, and additional main() functions that must not enter the application.
set(DEV701_APP_SOURCES
    argData.cpp
    ArgWindow.cpp
    BatchConfigWidget.cpp
    batchProcess.cpp
    BatchSSPNS.cpp
    bodyPathChecker.cpp
    btnObserver.cpp
    btnWiget.cpp
    computeLPCWSTR.cpp
    fileParamChecker.cpp
    checkProgramStrategy.cpp
    clearStrategy.cpp
    ColorChooseDialog.cpp
    computeLiutex.cpp
    createBoundata.cpp
    CreateFullDataset.cpp
    CreateTrees.cpp
    CusButton.cpp
    dataServer.cpp
    ECHV.cpp
    ECHVSignalInstance.cpp
    ECHVStrategy.cpp
    echvVTKWin.cpp
    fileOp.cpp
    FileOpUtils.cpp
    filePathComputer.cpp
    GraphWindow.cpp
    inputFileEditor.cpp
    InputHeader.cpp
    InputSet.cpp
    inp_process.cpp
    ISA_Calc.cpp
    logWidget.cpp
    myColorTable.cpp
    myDBSCAN.cpp
    myObserver.cpp
    mySubject.cpp
    nsStrategy.cpp
    Operator.cpp
    OutPutLogger.cpp
    ParamEditDialog.cpp
    ParamSetDialog.cpp
    pnsStrategy.cpp
    postgridsimplify.cpp
    PostProcessProfiling.cpp
    PostProgressDialog.cpp
    processPool.cpp
    ProgressDialog.cpp
    progressTable.cpp
    projectWidget.cpp
    PTGeneration.cpp
    publicArgWindow.cpp
    readFileStrategy.cpp
    relevance.cpp
    runData.cpp
    RWThread.cpp
    SectInfoDialog.cpp
    sectStrategy.cpp
    SignalInstance.cpp
    SingletonBase.cpp
    src/CConnectionForSshClient.cpp
    src/huangglobalconfig.cpp
    src/huangmain.cpp
    src/huangmainwindow.cpp
    src/scenewidget.cpp
    src/Widget.cpp
    SSPNS.cpp
    statusSubject.cpp
    StyleWidgetFactory.cpp
    SubZoneWindow.cpp
    SwitchButton.cpp
    sync_trData_input.cpp
    transGrid.cpp
    TrTableShow.cpp
    unionFindCT.cpp
    updateBatchArgs.cpp
    updateStrategy.cpp
    USStandardAtmosphere.cpp
    workContext.cpp
    workerHandler.cpp
    workFlow.cpp
    WorkSpaceOp.cpp
    WorkSpaceOpDialog.cpp
    workSpaceStarter.cpp
    workStrategy.cpp
)

set(DEV701_LOG4QT_SOURCES
    src/log4qt/appenderskeleton.cpp
    src/log4qt/basicconfigurator.cpp
    src/log4qt/consoleappender.cpp
    src/log4qt/dailyrollingfileappender.cpp
    src/log4qt/fileappender.cpp
    src/log4qt/helpers/classlogger.cpp
    src/log4qt/helpers/configuratorhelper.cpp
    src/log4qt/helpers/datetime.cpp
    src/log4qt/helpers/factory.cpp
    src/log4qt/helpers/initialisationhelper.cpp
    src/log4qt/helpers/logerror.cpp
    src/log4qt/helpers/logobject.cpp
    src/log4qt/helpers/logobjectptr.cpp
    src/log4qt/helpers/optionconverter.cpp
    src/log4qt/helpers/patternformatter.cpp
    src/log4qt/helpers/properties.cpp
    src/log4qt/hierarchy.cpp
    src/log4qt/layout.cpp
    src/log4qt/level.cpp
    src/log4qt/log4qt.cpp
    src/log4qt/logger.cpp
    src/log4qt/loggerrepository.cpp
    src/log4qt/loggingevent.cpp
    src/log4qt/logmanager.cpp
    src/log4qt/mdc.cpp
    src/log4qt/ndc.cpp
    src/log4qt/patternlayout.cpp
    src/log4qt/propertyconfigurator.cpp
    src/log4qt/rollingfileappender.cpp
    src/log4qt/simplelayout.cpp
    src/log4qt/spi/filter.cpp
    src/log4qt/ttcclayout.cpp
    src/log4qt/varia/debugappender.cpp
    src/log4qt/varia/denyallfilter.cpp
    src/log4qt/varia/levelmatchfilter.cpp
    src/log4qt/varia/levelrangefilter.cpp
    src/log4qt/varia/listappender.cpp
    src/log4qt/varia/nullappender.cpp
    src/log4qt/varia/stringmatchfilter.cpp
    src/log4qt/writerappender.cpp
)

# Headers explicitly processed by Qt/MSBuild in the working legacy project.
set(DEV701_MOC_HEADERS
    ParamEditDialog.h projectWidget.h PostProgressDialog.h SubZoneWindow.h
    BatchSSPNS.h batchProcess.h btnWiget.h logWidget.h ParamSetDialog.h
    PostProcessProfiling.h ProgressDialog.h progressTable.h publicArgWindow.h
    SwitchButton.h WorkSpaceOpDialog.h WorkSpaceOp.h SSPNS.h TrTableShow.h
    InputSet.h CusButton.h BatchConfigWidget.h ArgWindow.h InputHeader.h
    OutPutLogger.h ECHV.h ECHVSignalInstance.h dataServer.h echvVTKWin.h
    GraphWindow.h Operator.h postgridsimplify.h src/Widget.h
    src/scenewidget.h SignalInstance.h RWThread.h SectInfoDialog.h
    src/CConnectionForSshClient.h src/huangmainwindow.h SingletonBase.h
    src/log4qt/appenderskeleton.h src/log4qt/appender.h
    src/log4qt/consoleappender.h src/log4qt/dailyrollingfileappender.h
    src/log4qt/fileappender.h src/log4qt/helpers/configuratorhelper.h
    src/log4qt/helpers/logobject.h src/log4qt/layout.h src/log4qt/logger.h
    src/log4qt/patternlayout.h src/log4qt/rollingfileappender.h
    src/log4qt/simplelayout.h src/log4qt/spi/filter.h src/log4qt/ttcclayout.h
    src/log4qt/varia/debugappender.h src/log4qt/varia/denyallfilter.h
    src/log4qt/varia/levelmatchfilter.h src/log4qt/varia/levelrangefilter.h
    src/log4qt/varia/listappender.h src/log4qt/varia/nullappender.h
    src/log4qt/varia/stringmatchfilter.h src/log4qt/writerappender.h
)

set(DEV701_QT_RESOURCES
    ccconfigdialog.qrc
    classwizard.qrc
    Dev701QtNSPlatformBaseMain.qrc
    GLWidget.qrc
    mesh2.qrc
    cmake/Resource2.cmake.qrc
    resources/resources.qrc
    stylesheet.qrc
    uiloader.qrc
)

set(DEV701_UI_FORMS
    src/postprocessQWT3D/lightingdlgbase4.ui
    src/postprocessQWT3D/mesh2mainwindowbase4.ui
    src/testQGLViewer/viewerInterfacedialog.Qt4.ui
    src/testQGLViewer/viewerInterfacewidget.Qt4.ui
    ui/findwidget.ui
    ui/first.ui
    ui/fontdialog.ui
    ui/linecolumnwidget.ui
    ui/mainwindowOfstylesheet.ui
    ui/second.ui
    ui/stylesheeteditor.ui
)
