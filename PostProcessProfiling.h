#pragma once
#include <QVTKOpenGLNativeWidget.h>
#include <QPushButton.h>
#include <vtkDataSet.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkStructuredGrid.h>
#include <vtkAppendPolyData.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkBoxRepresentation.h>
#include <vtkPlaneWidget.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkTransform.h>
#include <vtkUnstructuredGrid.h>
#include <vtkPlanes.h>
#include <QDebug>
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
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkBoxRepresentation.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkPolyDataWriter.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyDataReader.h>
#include <vtkDataSet.h>
#include <vtkProperty.h>
#include <vtkDataSetMapper.h>
#include <vtkNew.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkScalarBarActor.h>
#include <vtkTextProperty.h>
#include <vtkCellData.h>
#include <vtkOutlineFilter.h>
#include <vtkClipPolyData.h>
#include <vtkPlane.h>
#include <vtkConeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkBoxWidget.h>
#include <QToolBar>
#include <QCloseEvent>
#include <vtkCutter.h>
#include <QAction>
#include <QVTKWidget.h>
#include <QMainWindow>
#include <QSignalMapper>
#include <QToolButton>
#include "PostProgressDialog.h"
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
#include <vtkPolyDataToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkImageReslice.h>
#include <vtkRendererCollection.h>
#include <vtkPointDataToCellData.h>
#include <vtkUnstructuredGridWriter.h>
#include <vtkLookupTable.h>
#include <vtkScalarBarActor.h>
#include "SubZoneWindow.h"
#include "FileOpUtils.h"
#include<vtkContourTriangulator.h>
#include <regex> 

using namespace std;
#define Create(type,name)\
    vtkNew<type> name;

namespace {
	
	// Callback for the interaction
	// This does the actual work: updates the vtkPlane implicit function.
	// This in turn causes the pipeline to update and clip the object.
	class vtkIPWCallback2 : public vtkCommand
	{
	public:
		static vtkIPWCallback2* New()
		{
			return new vtkIPWCallback2;
		}

		virtual void Execute(vtkObject* caller, unsigned long, void*)
		{
			vtkImplicitPlaneWidget2* planeWidget =
				reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
			vtkImplicitPlaneRepresentation* rep =
				reinterpret_cast<vtkImplicitPlaneRepresentation*>(
					planeWidget->GetRepresentation());
			rep->GetPlane(this->plane);
		}

		vtkIPWCallback2() = default;

		vtkPlane* plane{ nullptr };
	};


	class vtkIPWCallback1 : public vtkCommand
	{
	public:
		static vtkIPWCallback1* New()
		{
			return new vtkIPWCallback1;
		}
		virtual void Execute(vtkObject* caller, unsigned long, void*)
		{
			vtkImplicitPlaneWidget2* planeWidget =
				reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
			vtkImplicitPlaneRepresentation* rep =
				reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
			//rep->GetPlane(this->Plane);
			//-----------------------
			//if (rep) {
			//	// 锁定平面法线方向（例如，始终保持法线沿 x 轴）
			//	rep->SetNormal(1.0, 0.0, 0.0);  // 法线方向固定为 (1, 0, 0)

			//	// 如果平面位置发生了变化，强制将其返回到固定位置
			//	double origin[3];
			//	rep->GetOrigin(origin);

			//	// 锁定 y 和 z 坐标，仅允许 x 坐标变化
			//	origin[1] = 0.0;  // 锁定 y 轴
			//	origin[2] = 0.0;  // 锁定 z 轴
			//	rep->SetOrigin(origin);  // 设置新的固定原点
			//}
			rep->GetPlane(this->Plane);
			//----------------------


		}
		vtkIPWCallback1() :Plane(0), Actor(0) {}
		vtkSmartPointer<vtkPlane> Plane;
		vtkSmartPointer<vtkActor> Actor;

	};

	class vtkRightPressCallBack : public vtkCommand
	{
	public:
		vtkRightPressCallBack() :zoneWindow_(nullptr) {}
		static vtkRightPressCallBack *New() {
			return new vtkRightPressCallBack;
		}

		virtual void Execute(vtkObject *caller, unsigned long, void*) {
			if (zoneWindow_)
			{
				zoneWindow_->show();
			}
		}
		SubZoneWindow* zoneWindow_;
	};

	class vtkBoxWidgetCallback : public vtkCommand
	{
	public:
		static vtkBoxWidgetCallback *New() {
			return new vtkBoxWidgetCallback;
		}

		void RemoveAllActor()
		{
			//取得最近的一个actor
			vtkActor* actor = Renderer_->GetActors()->GetLastActor();
			//删除已经有的actor
			if (actor != nullptr) {
				Renderer_->RemoveActor(actor);
			}

			//渲染窗口,对窗口进行初始化
			m_pQVTKWidget->update();

		}

		vtkBoxWidgetCallback() : ClipPolyData(nullptr), Plane_(nullptr) {}

		void SetClipPolyData(vtkDataSet *clipPolyData) {
			this->ClipPolyData = clipPolyData;
		}

		void SetClipPlanes(vtkSmartPointer<vtkPlanes> clipPlanes) {
			this->Plane_ = clipPlanes;
		}

		virtual void Execute(vtkObject *caller, unsigned long, void*) {
			vtkBoxWidget *boxWidget = reinterpret_cast<vtkBoxWidget*>(caller);
			vtkSmartPointer<vtkBoxRepresentation> boxRep = vtkSmartPointer<vtkBoxRepresentation>::New();
			boxWidget->GetPlanes(this->Plane_);
			//vtkRenderWindowInteractor* interactor = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
			//std::string keySym = interactor->GetKeySym();
			//if (keySym == "c" || keySym == "C") { // 检查是否按下了'C'键
			//	RemoveAllActor();
			//	m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(Renderer_);
			//	std::cout << "Key 'C' pressed." << std::endl;
			//	boxWidget->Off();
			//	//planeWidget_->SetEnabled(false);
			//	Vtk_latestData_->DeepCopy(Clipper_->GetOutput());
			//	Mapper_->SetInputData(Vtk_latestData_);
			//	Actor_->SetMapper(Mapper_);
			//	Renderer_->AddActor(Actor_);
			//	m_pQVTKWidget->GetRenderWindow()->AddRenderer(Renderer_);
			//	m_pQVTKWidget->GetRenderWindow()->Render();
			//	interactor->GetRenderWindow()->Render(); // 重绘渲染窗口
			//}
			//if (keySym == "z" || keySym == "Z") { // 检查是否按下了'Z'键
			//	
			//}

		}
		vtkDataSet *ClipPolyData;
		vtkSmartPointer<vtkPlanes> Plane_{ nullptr };
		vtkSmartPointer<vtkDataSet> Vtk_latestData_{ nullptr };
		vtkSmartPointer<vtkClipPolyData> Clipper_{ nullptr };
		vtkSmartPointer<vtkRenderer> Renderer_{ nullptr };
		vtkSmartPointer<vtkDataSetMapper> Mapper_{ nullptr };
		vtkSmartPointer<vtkActor> Actor_{ nullptr };
		QVTKWidget* m_pQVTKWidget;
	};

	class vtkSliceCallback : public vtkCommand
	{
	public:
		static vtkSliceCallback *New() {
			return new vtkSliceCallback;
		}

		void Execute(vtkObject *caller, unsigned long eventId, void *callData) override
		{
			vtkPlaneWidget *widget = (vtkPlaneWidget *)caller;
			widget->GetPlane(m_plane);
		}
		vtkPlane *m_plane;
	private:
		vtkSliceCallback() {
			m_plane = nullptr;
		}
		~vtkSliceCallback() {}
	};

	class vtkKeyPressCallback : public vtkCommand
	{
	public:
		static vtkKeyPressCallback* New() {
			return new vtkKeyPressCallback;
		}

		void ClipExecute()
		{
			RemoveAllActor();
			m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(Renderer_);
			//std::cout << "Key 'C' pressed." << std::endl;
			planeWidget_->Off();
			//planeWidget_->SetEnabled(false);
			Vtk_latestData_->DeepCopy(Clipper_->GetOutput());

			Mapper_->SetInputData(Vtk_latestData_);
			Actor_->SetMapper(Mapper_);
			Renderer_->AddActor(Actor_);
			m_pQVTKWidget->GetRenderWindow()->AddRenderer(Renderer_);
			m_pQVTKWidget->GetRenderWindow()->Render();
			m_clipBtn->setEnabled(false);
		}

		void showClipExecute()
		{
			// Remove resource
			RemoveAllActor();
			m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(Renderer_);

			//------------------------------------
			// Setup a visualization pipeline
			Clipper_->SetClipFunction(plane_);
			Clipper_->InsideOutOn();
			Clipper_->SetInputData(Vtk_latestData_);
			Clipper_->Update();
			// Checkout output
			vtkSmartPointer<vtkDataSet> clippedData = vtkDataSet::SafeDownCast(Clipper_->GetOutput());
			if (clippedData && clippedData->GetNumberOfPoints() > 0) {
				qDebug() << "Clipper output has data.";
			}
			else {
				qDebug() << "Clipper output is empty.";
			}

			// Set Mapper input
			Mapper_->SetInputConnection(Clipper_->GetOutputPort());
			//Mapper_->SetInputData(Vtk_latestData_);
			Mapper_->SetScalarVisibility(true);

			// Actor property
			Actor_->SetMapper(Mapper_);
			vtkSmartPointer<vtkProperty> backFaces =
				vtkSmartPointer<vtkProperty>::New();
			backFaces->SetDiffuseColor(.8, .8, .4);
			Actor_->SetBackfaceProperty(backFaces);
			m_pQVTKWidget->GetRenderWindow()->AddRenderer(Renderer_);
			Renderer_->AddActor(Actor_);

			// The callback will do the work
			vtkSmartPointer<vtkIPWCallback1> myCallback =
				vtkSmartPointer<vtkIPWCallback1>::New();
			myCallback->Plane = plane_;
			myCallback->Actor = Actor_;

			// Set rep used to planeWidget
			vtkSmartPointer<vtkImplicitPlaneRepresentation> rep =
				vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
			rep->SetPlaceFactor(1.25); // This must be set prior to placing the widget
			rep->SetNormal(plane_->GetNormal());
			double bounds[6];
			Vtk_latestData_->GetBounds(bounds);
			double center[3] = {
				(bounds[0] + bounds[1]) / 2.0,
				(bounds[2] + bounds[3]) / 2.0,
				(bounds[4] + bounds[5]) / 2.0
			};
			rep->SetOrigin(center);
			rep->PlaceWidget(bounds); // 使用数据集的边界来放置小部件
			rep->OutsideBoundsOff();
			rep->OutlineTranslationOff();

			// Set planeWidget property
			planeWidget_->RemoveAllObservers();
			planeWidget_->SetInteractor(m_pQVTKWidget->GetRenderWindow()->GetInteractor());
			planeWidget_->SetRepresentation(rep);
			planeWidget_->AddObserver(vtkCommand::InteractionEvent, myCallback);

			//-----------------------
			// Render
			m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Initialize();
			m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Start();
			m_pQVTKWidget->GetRenderWindow()->Render();
			planeWidget_->On();
			m_pQVTKWidget->update();
			m_clipBtn->setEnabled(true);
		}

		void RemoveAllActor()
		{
			//取得最近的一个actor
			vtkActor* actor = Renderer_->GetActors()->GetLastActor();
			//删除已经有的actor
			if (actor != nullptr) {
				Renderer_->RemoveActor(actor);
			}

			//渲染窗口,对窗口进行初始化
			m_pQVTKWidget->update();

		}
		virtual void Execute(vtkObject* caller, unsigned long, void*) {
			vtkRenderWindowInteractor* interactor = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
			std::string keySym = interactor->GetKeySym();


		}
		void setImplicitPlaneWidget(vtkImplicitPlaneWidget2* planeWidget)
		{
			planeWidget_ = planeWidget;
		}

		vtkKeyPressCallback() = default;
		vtkSmartPointer<vtkPlane> plane_{ nullptr };
		vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidget_{ nullptr };

		vtkSmartPointer<vtkRenderer> Renderer_{ nullptr };

		vtkSmartPointer<vtkClipPolyData> Clipper_{ nullptr };
		vtkSmartPointer<vtkDataSetMapper> Mapper_{ nullptr };
		vtkSmartPointer<vtkActor> Actor_{ nullptr };
		vtkSmartPointer<vtkDataSet> Vtk_latestData_{ nullptr };
		QVTKWidget* m_pQVTKWidget;
		QPushButton* m_clipBtn;
	};



	class vtkKeyPressBoxClipCallback : public vtkCommand
	{
	public:
		static vtkKeyPressBoxClipCallback* New() {
			return new vtkKeyPressBoxClipCallback;
		}

		void BoxClipExecute()
		{
			RemoveAllActor();
			m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(Renderer_);
			std::cout << "Key Press Box 'C' pressed." << std::endl;
			boxWidget_->Off();
			Vtk_latestData_->DeepCopy(Clipper_->GetOutput());
			Mapper_->SetInputData(Vtk_latestData_);
			Actor_->SetMapper(Mapper_);
			Renderer_->AddActor(Actor_);
			m_pQVTKWidget->GetRenderWindow()->AddRenderer(Renderer_);
			m_pQVTKWidget->GetRenderWindow()->Render();
			m_boxClipBtn->setEnabled(false);
		}

		void showBoxClipExecute()
		{
			// Remove resource
			RemoveAllActor();
			m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(Renderer_);

			//------------------------------------
			// Setup a visualization pipeline
			Clipper_->SetClipFunction(Plane_);
			Clipper_->InsideOutOn();
			Clipper_->SetInputData(Vtk_latestData_);
			Clipper_->Update();
			// Checkout output
			vtkSmartPointer<vtkDataSet> clippedData = vtkDataSet::SafeDownCast(Clipper_->GetOutput());
			if (clippedData && clippedData->GetNumberOfPoints() > 0) {
				qDebug() << "Clipper output has data.";
			}
			else {
				qDebug() << "Clipper output is empty.";
			}

			// Set Mapper input
			Mapper_->SetInputConnection(Clipper_->GetOutputPort());
			//Mapper_->SetInputData(Vtk_latestData_);
			Mapper_->SetScalarVisibility(true);

			// Actor property
			Actor_->SetMapper(Mapper_);
			vtkSmartPointer<vtkProperty> backFaces =
				vtkSmartPointer<vtkProperty>::New();
			backFaces->SetDiffuseColor(.8, .8, .4);
			Actor_->SetBackfaceProperty(backFaces);
			m_pQVTKWidget->GetRenderWindow()->AddRenderer(Renderer_);
			Renderer_->AddActor(Actor_);

			// The callback will do the work
			//vtkSmartPointer<vtkBoxWidgetCallback> myCallback =
			//	vtkSmartPointer<vtkBoxWidgetCallback>::New();
			//myCallback->Plane_ = Plane_;
			//myCallback->Actor_ = Actor_;
			// Set rep used to planeWidget
			double bounds[6];
			Vtk_latestData_->GetBounds(bounds);
			double center[3] = {
				(bounds[0] + bounds[1]) / 2.0,
				(bounds[2] + bounds[3]) / 2.0,
				(bounds[4] + bounds[5]) / 2.0
			};
			boxWidget_->PlaceWidget(bounds);
			boxWidget_->SetPlaceFactor(1.0); // 设置BoxWidget的大小与actor相同

			//-----------------------
			// Render
			m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Initialize();
			m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Start();
			m_pQVTKWidget->GetRenderWindow()->Render();
			boxWidget_->On();
			m_pQVTKWidget->update();
			m_boxClipBtn->setEnabled(true);
		}

		void RemoveAllActor()
		{
			//取得最近的一个actor
			vtkActor* actor = Renderer_->GetActors()->GetLastActor();
			//删除已经有的actor
			if (actor != nullptr) {
				Renderer_->RemoveActor(actor);
			}

			//渲染窗口,对窗口进行初始化
			m_pQVTKWidget->update();

		}
		virtual void Execute(vtkObject* caller, unsigned long, void*) {
			vtkRenderWindowInteractor* interactor = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
			std::string keySym = interactor->GetKeySym();


		}

		vtkKeyPressBoxClipCallback() = default;
		vtkDataSet *ClipPolyData;
		vtkSmartPointer<vtkPlanes> Plane_{ nullptr };
		vtkSmartPointer<vtkDataSet> Vtk_latestData_{ nullptr };
		vtkSmartPointer<vtkClipPolyData> Clipper_{ nullptr };
		vtkSmartPointer<vtkRenderer> Renderer_{ nullptr };
		vtkSmartPointer<vtkDataSetMapper> Mapper_{ nullptr };
		vtkSmartPointer<vtkActor> Actor_{ nullptr };
		vtkSmartPointer<vtkBoxWidget> boxWidget_;
		QVTKWidget* m_pQVTKWidget;
		QPushButton* m_boxClipBtn;

	};
	class vtkKeyPressSliceCallback : public vtkCommand
	{
	public:
		static vtkKeyPressSliceCallback* New() {
			return new vtkKeyPressSliceCallback;
		}

		void RemoveAllActor()
		{
			//取得最近的一个actor
			vtkActor* actor = Renderer_->GetActors()->GetLastActor();
			//删除已经有的actor
			if (actor != nullptr) {
				Renderer_->RemoveActor(actor);
			}

			//渲染窗口,对窗口进行初始化
			m_pQVTKWidget->update();

		}
		virtual void Execute(vtkObject* caller, unsigned long, void*) {
			vtkRenderWindowInteractor* interactor = reinterpret_cast<vtkRenderWindowInteractor*>(caller);
			std::string keySym = interactor->GetKeySym();
			if (keySym == "c" || keySym == "C") { // 检查是否按下了'C'键
				//RemoveAllActor();
				//m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(Renderer_);
				//std::cout << "Key Press Box 'C' pressed." << std::endl;
				//boxWidget_->Off();
				//Vtk_latestData_->DeepCopy(Clipper_->GetOutput());
				//Mapper_->SetInputData(Vtk_latestData_);
				//Actor_->SetMapper(Mapper_);
				//Renderer_->AddActor(Actor_);
				//m_pQVTKWidget->GetRenderWindow()->AddRenderer(Renderer_);
				//m_pQVTKWidget->GetRenderWindow()->Render();
				//interactor->GetRenderWindow()->Render(); // 重绘渲染窗口
			}
			if (keySym == "z" || keySym == "Z") { // 检查是否按下了'Z'键
				//// Remove resource
				//RemoveAllActor();
				//m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(Renderer_);

				////------------------------------------
				//// Setup a visualization pipeline
				//Clipper_->SetClipFunction(Plane_);
				//Clipper_->InsideOutOn();
				//Clipper_->SetInputData(Vtk_latestData_);
				//Clipper_->Update();
				//// Checkout output
				//vtkSmartPointer<vtkDataSet> clippedData = vtkDataSet::SafeDownCast(Clipper_->GetOutput());
				//if (clippedData && clippedData->GetNumberOfPoints() > 0) {
				//	qDebug() << "Clipper output has data.";
				//}
				//else {
				//	qDebug() << "Clipper output is empty.";
				//}

				//// Set Mapper input
				//Mapper_->SetInputConnection(Clipper_->GetOutputPort());
				////Mapper_->SetInputData(Vtk_latestData_);
				//Mapper_->SetScalarVisibility(true);

				//// Actor property
				//Actor_->SetMapper(Mapper_);
				//vtkSmartPointer<vtkProperty> backFaces =
				//	vtkSmartPointer<vtkProperty>::New();
				//backFaces->SetDiffuseColor(.8, .8, .4);
				//Actor_->SetBackfaceProperty(backFaces);
				//m_pQVTKWidget->GetRenderWindow()->AddRenderer(Renderer_);
				//Renderer_->AddActor(Actor_);

				//// The callback will do the work
				//vtkSmartPointer<vtkBoxWidgetCallback> myCallback =
				//	vtkSmartPointer<vtkBoxWidgetCallback>::New();
				//myCallback->Plane_ = Plane_;
				//myCallback->Actor_ = Actor_;
				//// Set rep used to planeWidget
				//double bounds[6];
				//Vtk_latestData_->GetBounds(bounds);
				//double center[3] = {
				//	(bounds[0] + bounds[1]) / 2.0,
				//	(bounds[2] + bounds[3]) / 2.0,
				//	(bounds[4] + bounds[5]) / 2.0
				//};
				//boxWidget_->PlaceWidget(bounds);
				//boxWidget_->SetPlaceFactor(1.0); // 设置BoxWidget的大小与actor相同

				////-----------------------
				//// Render
				//m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Initialize();
				//m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Start();
				//m_pQVTKWidget->GetRenderWindow()->Render();
				//boxWidget_->On();
				//m_pQVTKWidget->update();
			}

		}

		vtkKeyPressSliceCallback() = default;
		vtkDataSet *ClipPolyData;
		vtkSmartPointer<vtkPlanes> Plane_{ nullptr };
		vtkSmartPointer<vtkDataSet> Vtk_latestData_{ nullptr };
		vtkSmartPointer<vtkClipPolyData> Clipper_{ nullptr };
		vtkSmartPointer<vtkRenderer> Renderer_{ nullptr };
		vtkSmartPointer<vtkDataSetMapper> Mapper_{ nullptr };
		vtkSmartPointer<vtkActor> Actor_{ nullptr };
		vtkSmartPointer<vtkBoxWidget> boxWidget_;
		QVTKWidget* m_pQVTKWidget;

	};




}

class PostProcessProfiling : public QMainWindow {
	Q_OBJECT
public:

	explicit PostProcessProfiling(QWidget* parent = nullptr);

	//! Add a data set to the scene
	/*!
	\param[in] dataSet The data set to add
	*/
	void addDataSet(vtkSmartPointer<vtkDataSet> dataSet);

	//! Remove the data set from the scene
	void removeDataSet();
	vtkSmartPointer<vtkPolyData> vtkPltReader(const char* plt_file);

	void initActions();
	void ViewDirection(
		double lookX, double lookY, double lookZ,
		double upX, double upY, double upZ);
	void setViewDirectionToX();
	void setViewDirectionToNegativeX();
	void setViewDirectionToY();
	void setViewDirectionToNegativeY();
	void setViewDirectionToZ();
	void setViewDirectionToNegativeZ();
	void amplificationView();
	void narrowView();
	void initToolBars();
	void initClipUnit();
	void initSliceUnit();

	void resetCamera(bool bCompulsion);
	void initVariables();
	void initFileOpToolBar();
	void showVTKData();
	void initArrowActor();
	void initAttributeSelToolBar();
	void initResetDataToolBtn();

	void SetActivateAttributeToMapperForVtk(vtkSmartPointer<vtkDataSetMapper> mapper);
	void addScalarBarActorForVtk(vtkSmartPointer<vtkDataSetMapper> mapper, string title, double * range);
	void initDisplayModeToolBar();
	void initUI();
	void initConnection();
	vtkSmartPointer<vtkActor> addOutlineActorForVtk(vtkSmartPointer<vtkOutlineFilter> outlineData);
	vtkSmartPointer<vtkActor> addOutlineActor(vtkSmartPointer<vtkOutlineFilter> outlineData);
	vtkSmartPointer<vtkActor> addPointsActorForVtk(vtkSmartPointer<vtkDataSet> dataSet);
	vtkSmartPointer<vtkActor> addWireFrameActorForVtk(vtkSmartPointer<vtkDataSet> dataSet, bool bBlack);
	void updateAttributeNameMap(vtkSmartPointer<vtkPolyData> mesh);
	void resetAttribute();
	void initPlaneChooseToolBar();
	void initLegendToolBar();
	void loadStyleSheet(QString sheetName);
	// bdFile.dat格式的阅读
	vtkSmartPointer<vtkPolyData> parseBDFiledFile(QString fileName);
	vtkSmartPointer<vtkPolyData> parseDomain1File(QString fileName);
	vtkSmartPointer<vtkUnstructuredGrid> parseBlockFileVtkFile(QString fileName);
	vtkSmartPointer<vtkUnstructuredGrid> parseVolumeVtkFile(QString fileName);
	// 将点scalar转cell scalar
	vtkSmartPointer<vtkUnstructuredGrid> calculate_scalar_cell(std::string fileName);
	void initPostProcessFunctionToolBar();
	void initPostProcessStates();

	vtkSmartPointer<vtkImageData> polyDataToImageData(vtkSmartPointer<vtkPolyData> polydata);

	void updateDataSetFromUnstructedGrid(vtkSmartPointer<vtkUnstructuredGrid> unsGrid);


	void launchWarning(const QString & message);

	void closeEvent(QCloseEvent *e) {
		// 想要实现的操作
		this->hide();
		e->ignore();
	}

public slots:
	//! Zoom to the extent of the data set in the scene
	void zoomToExtent();
	void resetVtkData();
	bool LoadPlot3DFile(QString filename);
	void attributeSelSlot(QString attribute);
	void displayModeSlot(QString mode);
	bool LoadVTKFile();
	void planeChooseSlot(QString pickName);
	void legendSlot(QString legend);
	void slotShowClipPostProcess();
	void slotShowBoxClipPostProcess();
	void slotShowSlicePostProcess();
	void slotBoxCliping();

	void slotClipPostProcess();
	void slotBoxClipPostProcess();

	void slotZonesChanged(QVector<int32_t>zones);
	//voidPostProcess();


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

	QToolBar* m_fileOpToolBar;								  //文件操作工具栏（打开文件/截图..）		
	QToolBar* m_mainToolBar;                                  //主工具栏
	QToolBar* m_attributeToolBar;                             //属性工具栏
	QToolBar* m_visualOpToolBar;							  //可视化工具栏
	QToolBar* m_postFunctionToolBar;						  //后处理方法工具栏
	QToolButton* m_attributeSelToolBtn;                       //属性选择工具按钮
	QToolButton* m_displayModeToolBtn;                        //显示模式工具按钮
	QToolButton* m_legendToolBtn;                             //图例工具按钮
	QToolButton* m_planeToolBtn;							  //面选择工具按钮
	QToolButton* m_resetDataToolBtn;						  //重置数据按钮
	QToolButton* m_opencvToolBtn;							  //智能分析按钮
	/// 工具栏按钮Action
	QAction* m_fileOpSelectAct;								  //选择文件用于解析
	QAction* m_pngSaveAct;
	QAction* m_setViewDirectionToXAct;                        //设置视角方向为X轴正向
	QAction* m_setViewDirectionToNegativeXAct;                //设置视角方向为X轴负向
	QAction* m_setViewDirectionToYAct;                        //设置视角方向为Y轴正向
	QAction* m_setViewDirectionToNegativeYAct;                //设置视角方向为Y轴负向
	QAction* m_setViewDirectionToZAct;                        //设置视角方向为Z轴正向
	QAction* m_setViewDirectionToNegativeZAct;                //设置视角方向为Z轴负向
	QAction* m_amplificationViewAct;                          //放大
	QAction* m_narrowViewAct;

	QAction* m_clipAct;
	QAction* m_sliceAct;
	QAction* m_boxClipAct;

	QPushButton* m_showClipBtn;
	QPushButton* m_clipBtn;

	QPushButton* m_showBoxClipBtn;
	QPushButton* m_boxClipBtn;

	QPushButton* m_showSliceBtn;
	QPushButton* m_sliceBtn;


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


	// 是否已经加载过数据
	bool dataIsLoad;
	QSignalMapper* m_planeMapper;

	// clip function
	vtkSmartPointer<vtkPlane> clip_plane;
	vtkSmartPointer<vtkClipPolyData> clipper;
	vtkSmartPointer<vtkImplicitPlaneRepresentation> clip_rep;
	vtkSmartPointer<vtkImplicitPlaneWidget2> clip_planeWidget;
	vtkSmartPointer<vtkKeyPressCallback> mykeyPressCallback;
	vtkSmartPointer<vtkIPWCallback1> myCallback;
	// vtk_data manager
	vtkSmartPointer<vtkPolyData> vtk_latestData;

	// box clip
	vtkSmartPointer<vtkBoxWidget> boxWidget;
	vtkSmartPointer<vtkBoxWidgetCallback> boxWidgetCallback;
	vtkSmartPointer<vtkPlanes>box_plane;
	vtkSmartPointer<vtkClipPolyData> box_clipper;
	vtkSmartPointer<vtkActor> clippedActor;
	vtkSmartPointer<vtkKeyPressBoxClipCallback> mykeyPressBoxClipCallback;

	// slice
	vtkSmartPointer<vtkPlane> slice_plane;
	vtkSmartPointer<vtkUnstructuredGrid>slice_OriginGrid;
	vtkSmartPointer<vtkDataSetMapper>slice_mapper1;
	vtkSmartPointer<vtkDataSetMapper>slice_mapper2;
	vtkSmartPointer<vtkActor>slice_actor1;
	vtkSmartPointer<vtkActor>slice_actor2;
	vtkSmartPointer<vtkClipPolyData> slice_clipper;
	vtkSmartPointer<vtkCutter> slice_cutter;
	vtkSmartPointer<vtkImplicitPlaneRepresentation> slice_rep;
	vtkSmartPointer<vtkPlaneWidget> slice_planeWidget;
	vtkSmartPointer<vtkKeyPressSliceCallback> mySliceKeyPressCallback;
	vtkSmartPointer<vtkSliceCallback> mySliceCallback;
	vtkSmartPointer<vtkRenderer> sliceOriginRender;
	vtkSmartPointer<vtkRenderer> sliceShowRender;
	// -------------------
	vtkSmartPointer<vtkRenderWindowInteractor> origin_Interactor;
	vtkSmartPointer<vtkRenderWindow> origin_renderWindow;
	vtkSmartPointer<vtkRenderWindowInteractor> slice_Interactor;
	vtkSmartPointer<vtkRenderWindow> slice_renderWindow;
	//状态记录
	bool isClip_ = false;
	bool isBoxClip_ = false;
	bool isSlice_ = false;

	// subZone
	SubZoneWindow* subZone_;

	vtkSmartPointer<vtkMultiBlockDataSet> branch;
	vtkSmartPointer<vtkRightPressCallBack>subZoneWindowShowCallBack;


	// 新增控制点和箭头大小参数
	double m_handleSizeScale = 0.0003;  // 控制点缩放比例（默认更小）
	double m_arrowSize = 0.08;          // 方向轴箭头大小（默认更小）
	int m_arrowResolution = 10;         // 箭头三角锥分辨率
};

