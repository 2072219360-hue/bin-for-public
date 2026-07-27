#pragma once

#include <QWidget>


#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include<vtkSmartPointer.h>
#include<QVTKWidget.h>
#include<vtkRendererCollection.h>
#include<vtkActorCollection.h>
#include<vtkUnstructuredGridReader.h>
#include<vtkDataSetMapper.h>
#include<vtkDataSet.h>
#include<vtkUnstructuredGrid.h>
#include <array>
#include <QVBoxLayout>
#include<qthread.h>
#include<qsignalmapper.h>
#include<vtkScalarBarActor.h>
#include<vtkCellData.h>
#include<vtkTextProperty.h>
#include<vtkOutlineFilter.h>
#include<QDebug>
#include<QIcon>
#include<QToolButton>
#include<QAction>
#include<QMenu>
#include<QToolBar>
#include<vtkPointData.h>
#include<vtkOutlineFilter.h>
#include"argData.h"
#include"runData.h"
#include "ECHVSignalInstance.h"
#define ECHVSIGNAL platforms::echv::ECHVSignalInstance
#define ECHVINSTANCE platforms::echv::ECHVSignalInstance::getInstance()

namespace platforms {
	namespace echv {
		
		//该类用来显示vtk
		class echvVTKWin : public QWidget
		{
			Q_OBJECT

		public:
			echvVTKWin(QWidget *parent = nullptr);
			~echvVTKWin();
			void initVTK();
			void removeActors();
			void removeRenders();
			void connectAll();
			void initDisplayModeToolBar();
			void initToolBars();
			void initUI();
			void SetActivateAttributeToMapperForVtk(vtkSmartPointer<vtkDataSetMapper> mapper);
			void addScalarBarActorForVtk(vtkSmartPointer<vtkDataSetMapper> mapper, string title, double * range);
			void initVariables();

			void addActorToShow(vtkSmartPointer<vtkActor> actor);
			vtkSmartPointer<vtkActor> addOutlineActor(vtkSmartPointer<vtkOutlineFilter> outlineData);
			vtkSmartPointer<vtkActor> addPointsActor(vtkSmartPointer<vtkDataSet> dataSet);
			vtkSmartPointer<vtkActor> addSurfaceActor(vtkSmartPointer<vtkDataSet> dataSet);
			vtkSmartPointer<vtkActor> addWireFrameActor(vtkSmartPointer<vtkDataSet> dataSet);
		public slots:
			//根据参数的路径打开文件加载VTK
			void reloadVTK(argData *arg);
			//根据runData中的数据加载VTK
			void loadVTK(argData *arg,runData *data);
			void displayModeSlot(QString mode);

			
		private:
			//QVTKOpenGLNativeWidget *vtkWin;
			QVTKWidget *vtkWin;
			vtkSmartPointer<vtkStructuredGrid> m_grid;
			vtkSmartPointer<vtkRenderer> m_renderer;
			vtkNew<vtkDataSetMapper> vtk_mapper;
			vector<vtkSmartPointer<vtkActor>> m_vecActors;
			//vtkSmartPointer<vtkStructuredGrid> vtk_dataSet;

			vtkSmartPointer<vtkScalarBarActor> m_scalarBarActor;

			QSignalMapper* m_displayModeMapper;                       //显示模式
			QToolButton* m_displayModeToolBtn;                        //显示模式工具按钮
			QToolBar* m_attributeToolBar;                             //属性工具栏

			string m_curShowMeshMode;                                 //当前的显示mesh的模式
			string m_curShowAttrName;                                 //当前的显示属性的名称
			string m_curShowAttrType;                                 //当前的显示属性的类型（POINT CELL FIELD）

			//QVTKOpenGLWidget *vtkWin;

			/*vtkNew<vtkUnstructuredGridReader> reader;
			vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid;
			vtkNew<vtkDataSetMapper> mapper;
			vtkNew<vtkActor> actor;
			vtkNew<vtkRenderer> renderer;*/
		};

	}
}



