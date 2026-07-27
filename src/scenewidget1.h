#ifndef SCENEWIDGET_H
#define SCENEWIDGET_H

#include <QVTKOpenGLNativeWidget.h>
#include <vtkDataSet.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <QFile>
#include <QMenu>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>
#include <vtkDoubleArray.h>
#include <vtkPolyData.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkPolyDataWriter.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyDataReader.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkNew.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkScalarBarActor.h>
#include <vtkTextProperty.h>
#include <vtkCellData.h>
#include <vtkOutlineFilter.h>

#include <QToolBar>
#include <QAction>
#include <QVTKWidget.h>
#include <QMainWindow>
#include <QSignalMapper>
#include <QToolButton>

#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <sstream>
#include <set>
#include <unordered_set>
#include <iostream>
#include <string>
#include <chrono>  

#include "log4qt\logger.h"
#include "log4qt\propertyconfigurator.h"
#include "log4qt\fileappender.h"
using namespace std;
LOG4QT_DECLARE_STATIC_LOGGER(logger, SceneWidget)
#if 0

class SceneWidget : public QMainWindow {
	Q_OBJECT
public:
	explicit SceneWidget(QWidget* parent = nullptr);

	//! Add a data set to the scene
	/*!
	\param[in] dataSet The data set to add
  */
	void addDataSet(vtkSmartPointer<vtkDataSet> dataSet);

	//! Remove the data set from the scene
	void removeDataSet();
	vtkSmartPointer<vtkPolyData> vtkPltReader(const char* plt_file);
	void initActions();
	void setViewDirectionToX();
	void setViewDirectionToNegativeX();
	void setViewDirectionToY();
	void setViewDirectionToNegativeY();
	void setViewDirectionToZ();
	void setViewDirectionToNegativeZ();
	void amplificationView();
	void narrowView();
	void initToolBars();

	void resetCamera(bool bCompulsion);
	void initVariables();
	void showVTKData();

public slots:
	//! Zoom to the extent of the data set in the scene
	void zoomToExtent();

	bool LoadPlot3DFile(QString filename);
	bool LoadVTKFile();

private:
	//int * m
	vtkSmartPointer<vtkRenderer> m_renderer;
	vtkNew<vtkPolyDataReader> vtk_reader;
	vtkSmartPointer<vtkPolyData> vtk_dataSet;
	vtkNew<vtkDataSetMapper> vtk_mapper;
	vtkNew<vtkActor> vtk_actor;

	QToolBar* m_mainToolBar;                                  //主工具栏
	QToolBar* m_attributeToolBar;                             //属性工具栏
	QToolButton* m_attributeSelToolBtn;                       //属性选择工具按钮
	QToolButton* m_displayModeToolBtn;                        //显示模式工具按钮
	QToolButton* m_legendToolBtn;                             //图例工具按钮
	QToolButton* m_pointPickerToolBtn;                        //点拾取工具按钮
	QToolButton* m_planeToolBtn;							  //面选择工具按钮
	QToolButton* m_partitionToolBtn;                          //切分工具按钮
	/// 工具栏按钮Action.
	QAction* m_setViewDirectionToXAct;                        //设置视角方向为X轴正向
	QAction* m_setViewDirectionToNegativeXAct;                //设置视角方向为X轴负向
	QAction* m_setViewDirectionToYAct;                        //设置视角方向为Y轴正向
	QAction* m_setViewDirectionToNegativeYAct;                //设置视角方向为Y轴负向
	QAction* m_setViewDirectionToZAct;                        //设置视角方向为Z轴正向
	QAction* m_setViewDirectionToNegativeZAct;                //设置视角方向为Z轴负向
	QAction* m_amplificationViewAct;                          //放大
	QAction* m_narrowViewAct;

	double m_curViewUp[3];
	double m_curPosition[3];
	double m_curFocalPoint[3];
	bool m_bResetCamera;

	QVTKWidget* m_pQVTKWidget;
};
#endif
class SceneWidget : public QMainWindow {
	Q_OBJECT
	//LOG4QT_DECLARE_QCLASS_LOGGER
public:
	explicit SceneWidget(QWidget* parent = nullptr);

	//! Add a data set to the scene
	/*!
	\param[in] dataSet The data set to add
  */
	void addDataSet(vtkSmartPointer<vtkDataSet> dataSet);

	//! Remove the data set from the scene
	void removeDataSet();
	vtkSmartPointer<vtkPolyData> vtkPltReader(const char* plt_file);

	void initActions();
	void setViewDirectionToX();
	void setViewDirectionToNegativeX();
	void setViewDirectionToY();
	void setViewDirectionToNegativeY();
	void setViewDirectionToZ();
	void setViewDirectionToNegativeZ();
	void amplificationView();
	void narrowView();
	void initToolBars();

	void resetCamera(bool bCompulsion);
	void initVariables();
	void showVTKData();
	void initArrowActor();
	void initAttributeSelToolBar();
	void SetActivateAttributeToMapperForVtk(vtkSmartPointer<vtkDataSetMapper> mapper);
	void addScalarBarActorForVtk(vtkSmartPointer<vtkDataSetMapper> mapper, string title, double * range);
	void initDisplayModeToolBar();
	void initUI();
	vtkSmartPointer<vtkActor> addOutlineActorForVtk(vtkSmartPointer<vtkOutlineFilter> outlineData);
	vtkSmartPointer<vtkActor> addOutlineActor(vtkSmartPointer<vtkOutlineFilter> outlineData);
	vtkSmartPointer<vtkActor> addPointsActorForVtk(vtkSmartPointer<vtkDataSet> dataSet);
	vtkSmartPointer<vtkActor> addWireFrameActorForVtk(vtkSmartPointer<vtkDataSet> dataSet, bool bBlack);
	void updateAttributeNameMap(vtkSmartPointer<vtkPolyData> mesh);
	void initPlaneChooseToolBar();
	void initLegendToolBar();

public slots:
	//! Zoom to the extent of the data set in the scene
	void zoomToExtent();

	bool LoadPlot3DFile(QString filename);
	void attributeSelSlot(QString attribute);
	void displayModeSlot(QString mode);
	bool LoadVTKFile();
	void planeChooseSlot(QString pickName);
	void legendSlot(QString legend);

private:
	//int * m
	vtkNew<vtkPolyDataReader> vtk_reader;
	vtkSmartPointer<vtkPolyData> vtk_dataSet;
	vtkNew<vtkDataSetMapper> vtk_mapper;
	vtkNew<vtkActor> vtk_actor;
	vtkSmartPointer<vtkPolyData> plt_dataSet;
	vtkNew<vtkDataSetMapper> plt_mapper;
	vtkNew<vtkActor> plt_actor;
	vtkNew<vtkOrientationMarkerWidget> m_widgetAxes;
	vtkNew<vtkAxesActor> axesActor;
	vector<vtkSmartPointer<vtkActor>> m_vecActorUi_show;
	vector<vtkSmartPointer<vtkActor>> m_vecActorUi_show_vtk;

	QToolBar* m_mainToolBar;                                  //主工具栏
	QToolBar* m_attributeToolBar;                             //属性工具栏
	QToolButton* m_attributeSelToolBtn;                       //属性选择工具按钮
	QToolButton* m_displayModeToolBtn;                        //显示模式工具按钮
	QToolButton* m_legendToolBtn;                             //图例工具按钮
	QToolButton* m_planeToolBtn;							  //面选择工具按钮
	/// 工具栏按钮Action.
	QAction* m_setViewDirectionToXAct;                        //设置视角方向为X轴正向
	QAction* m_setViewDirectionToNegativeXAct;                //设置视角方向为X轴负向
	QAction* m_setViewDirectionToYAct;                        //设置视角方向为Y轴正向
	QAction* m_setViewDirectionToNegativeYAct;                //设置视角方向为Y轴负向
	QAction* m_setViewDirectionToZAct;                        //设置视角方向为Z轴正向
	QAction* m_setViewDirectionToNegativeZAct;                //设置视角方向为Z轴负向
	QAction* m_amplificationViewAct;                          //放大
	QAction* m_narrowViewAct;

	double m_curViewUp[3];
	double m_curPosition[3];
	double m_curFocalPoint[3];
	bool m_bResetCamera;


	vtkSmartPointer<vtkRenderer> m_renderer;
	QVTKWidget* m_pQVTKWidget;
	QSignalMapper* m_attributeSelMapper;
	map<string, vector<string>> m_mapAttrNames;
	string m_curShowMeshMode;                                 //当前的显示mesh的模式
	string m_curShowAttrName;                                 //当前的显示属性的名称
	string m_curShowAttrType;                                 //当前的显示属性的类型（POINT CELL FIELD）
	vtkSmartPointer<vtkScalarBarActor> m_scalarBarActor;
	QSignalMapper* m_displayModeMapper;                       //显示模式
	QSignalMapper* m_legendMapper;                            //图例
	bool isVTKData;
	QSignalMapper* m_planeMapper;

};


#endif // SCENEWIDGET_H

