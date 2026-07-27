#pragma execution_character_set("utf-8")
#include "scenewidget.h"
#include "SignalInstance.h"

#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include<vtkDataSetMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <QMessageBox>
#include <vtkDataSetReader.h>
#include <vtkDataSetWriter.h>


#include <vtkActor.h>
#include <vtkDataSetMapper.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSuperquadricSource.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkSmartPointer.h>
#include <vtkCameraActor.h>
#include <vtkNamedColors.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkCubeSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkCamera.h>
#include <vtkMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include<QThread>
#include "huangmainwindow.h"
#include <QList>
#include"inp_process.h"

#include <vtkCommand.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkPlane.h>


#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
extern QString appComDir;
#define FIlEINSTANCE platforms::detail::FileOp::getInstance()
extern statusSubject* myStatus;

//*****************
//此声明极为重要
//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle, vtkRenderingFreeType, vtkRenderingFreeTypeOpenGL, vtkRenderingOpenGL);
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL);
//*****************

//#endif //INITIAL_OPENGL
//0:use map  1:not use map
#define defaultOverlapFLag 1
#define decimateSurface
void ViewDirection(vtkRenderer *renderer,
	double lookX, double lookY, double lookZ,
	double upX, double upY, double upZ)
{
	renderer->GetActiveCamera()->SetPosition(0, 0, 0);    //相机位置
	renderer->GetActiveCamera()->SetFocalPoint(lookX, lookY, lookZ);    //焦点位置
	renderer->GetActiveCamera()->SetViewUp(upX, upY, upZ);    //朝上方向
	renderer->ResetCamera();
}
namespace {
	// Callback for the interaction
	// This does the actual work: updates the vtkPlane implicit function.
	// This in turn causes the pipeline to update and clip the object.
	class StatusClass;

	class vtkIPWCallback : public vtkCommand
	{
	public:
		static vtkIPWCallback* New()
		{
			return new vtkIPWCallback;
		}

		virtual void Execute(vtkObject* caller, unsigned long eventId, void * callData)
		{
			// 1. 获取平面控件和交互器
			vtkImplicitPlaneWidget2* planeWidget =
				reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
			vtkRenderWindowInteractor* interactor = planeWidget->GetInteractor();
			if (!interactor || !Representation) return;
			Representation->SetOrigin(xyz[index][0], xyz[index][1], xyz[index][2]);
			// 2. 获取鼠标当前位置和上一次位置，计算移动方向
			int currX = 0, currY = 0;    // 当前鼠标位置
			int lastX = 0, lastY = 0;    // 上一次鼠标位置
			interactor->GetEventPosition(currX, currY);       // 获取当前鼠标位置
			interactor->GetLastEventPosition(lastX, lastY);   // 获取上一次鼠标位置

			// 计算X方向移动增量（核心：判断左右移动）
			// deltaX > 0 表示鼠标向右移动，deltaX < 0 表示向左移动
			int deltaX = currX - lastX;

			// 3. 灵敏度控制：累积移动次数，达到阈值才触发切换
			num++;
			if (num >= sensitivity)
			{
				num = 0; // 重置计数器

				// 4. 根据鼠标移动方向调整index
				if (deltaX > delta) // 向右移动（加阈值避免微小抖动）
				{
					index++;
					// 边界保护：不超过最大值
					if (index >= static_cast<int>(xyz.size()))
					{
						index = static_cast<int>(xyz.size()) - 1;
					}
				}
				else if (deltaX < -delta) // 向左移动（加阈值避免微小抖动）
				{
					index--;
					// 边界保护：不小于0
					if (index < 0)
					{
						index = 0;
					}
				}
				// deltaX在-5~5之间时，视为无有效移动，不修改index

				// 更新状态和平面位置
				if (myStatus)
				{
					myStatus->setClipIndex(index);
				}
				Representation->SetOrigin(xyz[index][0], xyz[index][1], xyz[index][2]);
			}

			// 原有逻辑：锁定平面法线和y/z坐标
			vtkImplicitPlaneRepresentation* rep =
				reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
			if (rep)
			{
				// 锁定平面法线方向（例如，始终保持法线沿 x 轴）
				rep->SetNormal(1.0, 0.0, 0.0);

				// 锁定 y 和 z 坐标，仅允许 x 坐标变化
				double origin[3];
				rep->GetOrigin(origin);
				origin[1] = 0.0;  // 锁定 y 轴
				origin[2] = 0.0;  // 锁定 z 轴
				rep->SetOrigin(origin);
			}

			if (rep && plane)
			{
				rep->GetPlane(this->plane);
			}
		}

		void setPlane()
		{
			Representation->SetOrigin(xyz[index][0], xyz[index][1], xyz[index][2]);
		}

		void setPresentation(vtkSmartPointer<vtkImplicitPlaneRepresentation> rep)
		{
			this->Representation = rep;
		}

		void addPoint(double x, double y, double z)
		{
			xyz.push_back({ x, y, z });
		}

		

		vtkIPWCallback() = default;

		// 成员变量
		vtkPlane* plane{ nullptr };
		vtkImplicitPlaneRepresentation* Representation{ nullptr };
		std::vector<std::vector<double>> xyz;
		int index = 0;
		int num = 0;
		int sensitivity = 15; // 灵敏度：数值越大，需要更多移动才触发切换
		int delta = 2; //判断移动的阈值
		
	};
	//class vtkIPWCallback : public vtkCommand
	//{
	//public:
	//	static vtkIPWCallback* New()
	//	{
	//		return new vtkIPWCallback;
	//	}

	//	virtual void Execute(vtkObject* caller, unsigned long eventId, void * 	callData)
	//	{
	//		/*cout << "getbounds=" << Representation->GetBounds()[0]<<" "
	//			<< Representation->GetBounds()[1] <<" "
	//			<< Representation->GetBounds()[2] << endl;*/
	//			//cout << "eventId=" << eventId << endl;
	//			//Representation->SetOrigin(0, Representation->GetBounds()[2], 0);
	//			//cout << "exexute" << endl;

	//		Representation->SetOrigin(xyz[index][0], xyz[index][1], xyz[index][2]);
	//		num++;
	//		if (num >= sensitivity) {
	//			num = 0;
	//			index++;
	//			//控制index到最右边后不变。
	//			if (index >= xyz.size()) index = xyz.size()-1;
	//			//if (index >= xyz.size()) index = 0;
	//			myStatus->setClipIndex(index);
	//			Representation->SetOrigin(xyz[index][0], xyz[index][1], xyz[index][2]);
	//			//cout << "index=" << index << endl;
	//		}

	//		vtkImplicitPlaneWidget2* planeWidget =
	//			reinterpret_cast<vtkImplicitPlaneWidget2*>(caller);
	//		vtkImplicitPlaneRepresentation* rep =
	//			reinterpret_cast<vtkImplicitPlaneRepresentation*>(planeWidget->GetRepresentation());
	//		if (rep) {
	//			// 锁定平面法线方向（例如，始终保持法线沿 x 轴）
	//			rep->SetNormal(1.0, 0.0, 0.0);  // 法线方向固定为 (1, 0, 0)

	//			// 如果平面位置发生了变化，强制将其返回到固定位置
	//			double origin[3];
	//			rep->GetOrigin(origin);

	//			// 锁定 y 和 z 坐标，仅允许 x 坐标变化
	//			origin[1] = 0.0;  // 锁定 y 轴
	//			origin[2] = 0.0;  // 锁定 z 轴
	//			rep->SetOrigin(origin);  // 设置新的固定原点
	//		}
	//		rep->GetPlane(this->plane);
	//		



	//	}
	//	void setPlane() {
	//		Representation->SetOrigin(xyz[index][0], xyz[index][1], xyz[index][2]);
	//	}
	//	void setPresentation(vtkSmartPointer<vtkImplicitPlaneRepresentation>rep)
	//	{
	//		this->Representation = rep;
	//	}
	//	void addPoint(double x, double y, double z) {
	//		xyz.push_back({ x,y,z });
	//	}
	//	vtkIPWCallback() = default;

	//	vtkPlane* plane{ nullptr };
	//	vtkImplicitPlaneRepresentation* Representation;
	//	vector< vector<double> >xyz;
	//	int index = 0;
	//	int num = 0;
	//	int sensitivity = 15;
	//};

}
namespace platforms {
	namespace sspns {
		namespace reveal {
			SceneWidget::SceneWidget(int nsFlag,QWidget* parent)
			{
				m_nsFlag = nsFlag;

				
				/// @brief 初始化场景
				initScene();
				initVariables();
				initActions();
				initToolBars();
				initArrowActor();
				initConnection();


				addObserver();

				un_grid = NULL;
				headGrid = NULL;
				bodyGrid = NULL;
				gridLine = NULL;
				sectInfo = NULL;

				myColor = NULL;
				decimated_grid = NULL;
				decimatedLine = NULL;
				decimated_sectInfo = NULL;
				gridLine = NULL;
				m_rightClipFlag = true;
				m_bothClipFlag = false;
				myColor = new myColorTable;
			}
			SceneWidget::~SceneWidget() {
				delete myColor;
			}
			void SceneWidget::addObserver() {
				btnObserver* visibleObserver = new btnObserver(myStatus,m_clipWindowVisible,btnType::CLIP_WINDOW_VISIBLE);
				btnObserver* invisibleObserver = new btnObserver(myStatus, m_clipWindowInvisible, btnType::CLIP_WINDOW_INVISIBLE);
				btnObserver* clipObserver = new btnObserver(myStatus, m_btnClip, btnType::CLIP_SINGLE);
				btnObserver* reClipObserver = new btnObserver(myStatus, m_btnReClip, btnType::RE_CLIP);
				btnObserver* clipBothObserver = new btnObserver(myStatus, m_btnClipSplit, btnType::CLIP_BOTH);
				btnObserver* colorStopObserver = new btnObserver(myStatus, m_btnColorStop, btnType::STOP_COLOR);
				btnObserver* jumpObserver = new btnObserver(myStatus, m_coverOrdumpButton, btnType::JUMP);
				
				myStatus->Attach(visibleObserver);
				myStatus->Attach(invisibleObserver);
				myStatus->Attach(clipObserver);
				myStatus->Attach(reClipObserver);
	
				myStatus->Attach(clipBothObserver);
				myStatus->Attach(colorStopObserver);
				myStatus->Attach(jumpObserver);
			}
			void SceneWidget::addDataSet(vtkSmartPointer<vtkDataSet> dataSet)
			{
				plt_mapper->SetInputData(plt_dataSet);
				plt_actor->SetMapper(plt_mapper);
				m_renderer->AddActor(plt_actor);
				m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);
				m_pQVTKWidget->update();

				if (!isVTKData) {
					updateAttributeNameMap(plt_dataSet);
					initAttributeSelToolBar();
				}
			}

			void SceneWidget::removeDataSet()
			{
				vtkActorCollection* allActor = m_renderer->GetActors();
				int num = allActor->GetNumberOfItems();
				vtkCollectionSimpleIterator pit;
				vtkActor *currActor = nullptr;
				for (allActor->InitTraversal(pit); (currActor = allActor->GetNextActor(pit)); )
				{
					m_renderer->RemoveActor(currActor);
				}

				//渲染窗口,对窗口进行初始化
				m_pQVTKWidget->update();

			}

			vtkSmartPointer<vtkUnstructuredGrid> SceneWidget::vtkPltReader(const char* plt_file)
			{
				std::string file_name = plt_file;

				std::ifstream file_stream(file_name.c_str()); //文件流
				std::string line, str1, str2, str3;
				std::getline(file_stream, line);
				std::getline(file_stream, line);
				std::stringstream line_stream;
				line_stream << line;
				line_stream >> str1 >> str2 >> str3;
				int node_len = str2.length();
				int node_num = atoi(str2.substr(0, node_len - 3).c_str());
				int edge_len = str3.length();
				int edge_num = atoi(str3.substr(0, edge_len - 27).c_str());
				//std::cout << "node_num" << node_num << std::endl;
				//std::cout << "edge_num" << edge_num << std::endl;

				int node_loop = 0, cell_loop = 0;
				// double X, Y, Z;
				double *X = new double[3];
				double *Cp = new double[node_num];
				double *U = new double[node_num];
				double *V = new double[node_num];
				double *W = new double[node_num];
				double *NIU = new double[node_num];
				double *TAO_X = new double[node_num];
				double *TAO_Y = new double[node_num];
				double *TAO_Z = new double[node_num];
				double *TAO_N = new double[node_num];
				double *TW = new double[node_num];
				double *QW = new double[node_num];
				vtkSmartPointer<vtkDoubleArray> FES_Cp = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_U = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_V = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_W = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_NIU = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_TAO_X = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_TAO_Y = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_TAO_Z = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_TAO_N = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_TW = vtkSmartPointer<vtkDoubleArray>::New();
				vtkSmartPointer<vtkDoubleArray> FES_QW = vtkSmartPointer<vtkDoubleArray>::New();
				FES_Cp->SetName("Cp");
				FES_U->SetName("U");
				FES_V->SetName("V");
				FES_W->SetName("W");
				FES_NIU->SetName("NIU");
				FES_TAO_X->SetName("TAO_X");
				FES_TAO_Y->SetName("TAO_Y");
				FES_TAO_Z->SetName("TAO_Z");
				FES_TAO_N->SetName("TAO_N");
				FES_TW->SetName("TW");
				FES_QW->SetName("QW");
				vtkSmartPointer<vtkUnstructuredGrid> data_set = vtkSmartPointer<vtkUnstructuredGrid>::New();
				//vtkSmartPointer<vtkPolyData> data_set = vtkSmartPointer<vtkPolyData>::New();
				vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
				while (node_loop < node_num) {
					std::getline(file_stream, line);
					std::stringstream line_stream;
					line_stream << line;
					line_stream >> X[0] >> X[1] >> X[2];
					points->InsertNextPoint(X[0], X[1], X[2]);
					// points->InsertPoint(node_loop + 1, X);
					line_stream.clear();

					std::getline(file_stream, line);
					line_stream << line;
					line_stream >> Cp[node_loop] >> U[node_loop] >> V[node_loop];
					line_stream.clear();

					std::getline(file_stream, line);
					line_stream << line;
					line_stream >> W[node_loop] >> NIU[node_loop] >> TAO_X[node_loop];
					line_stream.clear();

					std::getline(file_stream, line);
					line_stream << line;
					line_stream >> TAO_Y[node_loop] >> TAO_Z[node_loop] >> TAO_N[node_loop];
					line_stream.clear();

					std::getline(file_stream, line);
					line_stream << line;
					line_stream >> TW[node_loop] >> QW[node_loop];
					line_stream.clear();
					node_loop++;
				}
				data_set->SetPoints(points);

				for (int i = 0; i < data_set->GetNumberOfPoints(); i++) {
					FES_Cp->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_Cp->SetValue(i, Cp[i]);
					data_set->GetPointData()->SetScalars(FES_Cp);

					FES_U->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_U->SetValue(i, U[i]);
					data_set->GetPointData()->SetScalars(FES_U);

					FES_V->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_V->SetValue(i, V[i]);
					data_set->GetPointData()->SetScalars(FES_V);

					FES_W->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_W->SetValue(i, W[i]);
					data_set->GetPointData()->SetScalars(FES_W);

					FES_NIU->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_NIU->SetValue(i, NIU[i]);
					data_set->GetPointData()->SetScalars(FES_NIU);

					FES_TAO_X->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_TAO_X->SetValue(i, TAO_X[i]);
					data_set->GetPointData()->SetScalars(FES_TAO_X);

					FES_TAO_Y->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_TAO_Y->SetValue(i, TAO_Y[i]);
					data_set->GetPointData()->SetScalars(FES_TAO_Y);

					FES_TAO_Z->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_TAO_Z->SetValue(i, TAO_Z[i]);
					data_set->GetPointData()->SetScalars(FES_TAO_Z);

					FES_TAO_N->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_TAO_N->SetValue(i, TAO_N[i]);
					data_set->GetPointData()->SetScalars(FES_TAO_N);

					FES_TW->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_TW->SetValue(i, TW[i]);
					data_set->GetPointData()->SetScalars(FES_TW);

					FES_QW->SetNumberOfValues(data_set->GetNumberOfPoints());
					FES_QW->SetValue(i, QW[i]);
					data_set->GetPointData()->SetScalars(FES_QW);
				}
				data_set->GetPointData()->AddArray(FES_Cp);
				data_set->GetPointData()->AddArray(FES_U);
				data_set->GetPointData()->AddArray(FES_V);
				data_set->GetPointData()->AddArray(FES_W);
				data_set->GetPointData()->AddArray(FES_NIU);
				data_set->GetPointData()->AddArray(FES_TAO_X);
				data_set->GetPointData()->AddArray(FES_TAO_Y);
				data_set->GetPointData()->AddArray(FES_TAO_Z);
				data_set->GetPointData()->AddArray(FES_TAO_N);
				data_set->GetPointData()->AddArray(FES_TW);
				data_set->GetPointData()->AddArray(FES_QW);

				vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
				vtkIdType cell[3];
				while (cell_loop < edge_num) {
					std::getline(file_stream, line);
					std::stringstream line_stream;
					line_stream << line;
					line_stream >> cell[0] >> cell[1] >> cell[2];
					// std::cout << cell[0] << std::endl;
					cell[0] = cell[0] - 1;
					cell[1] = cell[1] - 1;
					cell[2] = cell[2] - 1;
					cells->InsertNextCell(3, cell);
					cell_loop++;
				}

				//unfinish
				//data_set->SetPolys(cells);

				//data_set->GetPointData()->SetScalars(FES_Cp);
				/*
				data_set->GetPointData()->SetScalars(FES_U);
				data_set->GetPointData()->SetScalars(FES_V);
				data_set->GetPointData()->SetScalars(FES_W);
				data_set->GetPointData()->SetScalars(FES_NIU);
				data_set->GetPointData()->SetScalars(FES_TAO_X);
				data_set->GetPointData()->SetScalars(FES_TAO_Y);
				data_set->GetPointData()->SetScalars(FES_TAO_Z);
				data_set->GetPointData()->SetScalars(FES_TAO_N);
				data_set->GetPointData()->SetScalars(FES_TW);
				data_set->GetPointData()->SetScalars(FES_QW);*/
				file_stream.close();
				/*
				vtkSmartPointer<vtkPolyDataMapper> mapper =
					vtkSmartPointer<vtkPolyDataMapper>::New();
				mapper->SetInputData(data_set);

				vtkSmartPointer<vtkActor>  actor =
					vtkSmartPointer<vtkActor>::New();
				actor->SetMapper(mapper);

				vtkSmartPointer<vtkRenderer> render =
					vtkSmartPointer<vtkRenderer>::New();
				render->AddActor(actor);
				// render->SetBackground(10, 0, 0);

				vtkSmartPointer<vtkRenderWindow> rw =
					vtkSmartPointer<vtkRenderWindow>::New();
				rw->AddRenderer(render);
				rw->SetSize(640, 480);
				rw->SetWindowName("PolyData Structure Learning");
				rw->Render();

				vtkSmartPointer<vtkRenderWindowInteractor> rwi =
					vtkSmartPointer<vtkRenderWindowInteractor>::New();
				rwi->SetRenderWindow(rw);
				rwi->Initialize();
				rwi->Start();
				*/


				//auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
				/*writer->SetFileName("test_plt_4.vtk");
				// writer->SetInputData(interpolator->GetOutput());
				writer->SetInputData(data_set);
				writer->Update();
				writer->Write();*/
				return data_set;

			}

			void SceneWidget::selectGrid() {
				decimatedFlag = 0;
				mouseStyle->gridLineInfo = gridLine;
				mouseStyle->un_grid = un_grid;
				mouseStyle->sectInfo = sectInfo;
				mouseStyle->ColorsCurrent = ColorsCurrent;

				mouseStyle->Data = vtk_dataSet;
				mouseStyle->Renderer = m_renderer;
				mouseStyle->actor = vtk_actor;
				mouseStyle->ColorsSect = ColorsSect;
				mouseStyle->mapper = vtk_mapper;
				mouseStyle->ColorsGroup = ColorsGroup;
				mouseStyle->renderWindow = m_pQVTKWidget->GetRenderWindow();


				vtkActorCollection* allActor = m_renderer->GetActors();
				int num = allActor->GetNumberOfItems();
				vtkCollectionSimpleIterator pit;
				vtkActor *currActor = nullptr;
				for (allActor->InitTraversal(pit); (currActor = allActor->GetNextActor(pit)); )
				{
					m_renderer->RemoveActor(currActor);
				}

				ColorsCurrent->SetName("ColorsCurrent");
				ugrid->GetPointData()->SetActiveScalars("ColorsCurrent");
				vtk_dataSet = ugrid;
				vtk_mapper->SetInputData(vtk_dataSet);
				vtk_actor->SetMapper(vtk_mapper);
				m_renderer->AddActor(vtk_actor);

				m_pQVTKWidget->update();
			}

			void SceneWidget::selectDecimatedGrid() {
				decimatedFlag = 1;
				mouseStyle->gridLineInfo = decimatedLine;
				mouseStyle->un_grid = decimated_grid;
				mouseStyle->sectInfo = decimated_sectInfo;
				mouseStyle->ColorsCurrent = ColorsDecimated;

				mouseStyle->Data = vtk_dataSet;
				mouseStyle->Renderer = m_renderer;
				mouseStyle->actor = vtk_actor;
				mouseStyle->ColorsSect = ColorsSect;
				mouseStyle->mapper = vtk_mapper;
				mouseStyle->ColorsGroup = ColorsGroup;
				mouseStyle->renderWindow = m_pQVTKWidget->GetRenderWindow();

				vtkActorCollection* allActor = m_renderer->GetActors();
				int num = allActor->GetNumberOfItems();
				vtkCollectionSimpleIterator pit;
				vtkActor *currActor = nullptr;
				for (allActor->InitTraversal(pit); (currActor = allActor->GetNextActor(pit)); )
				{
					m_renderer->RemoveActor(currActor);
				}


				ColorsDecimated->SetName("ColorsDecimated");
				decimatedGrid->GetPointData()->SetActiveScalars("ColorsDecimated");
				vtk_dataSet = decimatedGrid;
				vtk_mapper->SetInputData(vtk_dataSet);
				vtk_actor->SetMapper(vtk_mapper);
				m_renderer->AddActor(vtk_actor);

				m_pQVTKWidget->update();
			}
			void SceneWidget::onlyOut(QString outPath) {
				//out_to_preGrid(outPath + "/preGrid.dat", un_grid);
				//out_to_preIdmap(outPath + "/preIdmap.dat", &idMap);
				//out_to_preBlock(outPath + "/preBlock.dat");
			}

			void SceneWidget::readAndOut(QString inPath, QString outPath) {

				emit SignalInstance::getInstance()->signalShowDialog(u8"loading....",u8"正在渲染网格，请等待渲染完成......");
				emit SignalInstance::getInstance()->signalPreLoad(30);

				slotLoadGridData(inPath, 0);

				emit SignalInstance::getInstance()->signalCloseDialog();

			}
			void SceneWidget::onlyReadGrid(QString inPath) {
				slotLoadGridData(inPath, 1);

			}
			void SceneWidget::preRead(QString inPath) {
				//qDebug() << "inPath=" << inPath;
				emit SignalInstance::getInstance()->signalShowDialog("Loading",u8"等待网格渲染......");
				emit SignalInstance::getInstance()->signalPreLoad(30);
				this->removeDataSet();
				clearAll();
				unsigned char myRed[3] = { 255, 0, 0 };
				unsigned char myGreen[3] = { 0, 255, 0 };
				unsigned char myBlue[3] = { 0, 0, 255 };
				unsigned char myYellow[3] = { 255,255,0 };
				unsigned char myBlack[3] = { 0,0,0 };
				unsigned char mySilver[3] = { 192,192,192 };
				unsigned char myPurple[3] = { 128,0,128 };
				unsigned char myBrown[3] = { 165,42,42 };

				int colorFlag = 0;
				int representId = 0;
				bool isExist = QFile(inPath + "/preGrid.dat").exists();
				if (isExist)readPreGrid(inPath + "/preGrid.dat", un_grid);
				else {
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"工作区不存在上次运行的网格文件");
					//exit(0);
				}
				isExist = QFile(inPath + "/preIdmap.dat").exists();
				if (isExist)readPreIdmap(inPath + "/preIdmap.dat", &idMap);
				else {
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"工作区不存在上次运行的网格文件");
					//exit(0);
				}
				isExist = QFile(inPath + "/preBlock.dat").exists();
				if (isExist)readPreBlock(inPath + "/preBlock.dat");
				else {
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"工作区不存在上次运行的网格文件");
					//exit(0);
				}
				isExist = QFile(inPath + "/sect/preSect.dat").exists();
				if (isExist) {
					readPreSect(inPath + "/sect/preSect.dat", sectInfo, un_grid);
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"上次运行已经分段");
				}
				else {
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"上次运行没有分段");
				}


				mouseStyle->gridLineInfo = gridLine;
				mouseStyle->Data = vtk_dataSet;
				mouseStyle->sectInfo = sectInfo;
				mouseStyle->Renderer = m_renderer;
				mouseStyle->actor = vtk_actor;
				mouseStyle->ColorsSect = ColorsSect;
				mouseStyle->mapper = vtk_mapper;
				mouseStyle->ColorsGroup = ColorsGroup;
				mouseStyle->ColorsCurrent = ColorsCurrent;
				mouseStyle->un_grid = un_grid;
				mouseStyle->renderWindow = m_pQVTKWidget->GetRenderWindow();


				int n_cells = un_grid->n_cells;
				ugrid->Allocate(n_cells);
				for (int i = 0; i < n_cells; i++) {
					int id1 = un_grid->cellData[i].idPoint[0];
					int id2 = un_grid->cellData[i].idPoint[1];
					int id3 = un_grid->cellData[i].idPoint[2];
					int id4 = un_grid->cellData[i].idPoint[3];
					vtkIdType idList[4] = { id1,id2,id3,id4 };
					ugrid->InsertNextCell(VTK_TETRA, 4, idList);

				}
				int n_points = un_grid->n_points;
				//points->Allocate(n_points);
				for (int i = 0; i < n_points; i++) {
					double x = un_grid->pointData[i].x;
					double y = un_grid->pointData[i].y;
					double z = un_grid->pointData[i].z;
					points->InsertNextPoint(x, y, z);
				}
				ugrid->SetPoints(points);


				ColorsGroup->SetNumberOfComponents(3);
				ColorsGroup->SetName("ColorsGroup");
				for (int i = 0; i < n_points; i++) {
					int id;
					id = un_grid->pointData[i].blockId;
					ColorsGroup->InsertNextTupleValue(myColor->colorTable[id]);

				}
				emit SignalInstance::getInstance()->signalPreLoad(50);
				ColorsSect->SetNumberOfComponents(3);
				ColorsSect->SetName("ColorsSect");
				for (int i = 0; i < n_points; i++) {
					int id;
					id = un_grid->pointData[i].sectId;
					ColorsSect->InsertNextTupleValue(myColor->colorTable[id]);

				}

				ColorsCurrent->SetNumberOfComponents(3);
				ColorsCurrent->SetName("ColorsCurrent");
				for (int i = 0; i < n_points; i++) {
					int id;
					id = un_grid->pointData[i].blockId;
					ColorsCurrent->InsertNextTupleValue(myColor->colorTable[id]);
				}

				if (colorFlag == 1) {
					//ugrid->GetPointData()->SetScalars(ColorsGroup);
					ColorsCurrent->DeepCopy(ColorsGroup);
				}
				else if (colorFlag == 2) {
					//ugrid->GetPointData()->SetScalars(ColorsSect);
					ColorsCurrent->DeepCopy(ColorsSect);
				}

				ugrid->GetPointData()->SetScalars(ColorsCurrent);


				vtk_dataSet = ugrid;
				vtk_mapper->SetInputData(vtk_dataSet);
				vtk_actor->SetMapper(vtk_mapper);

				m_renderer->AddActor(vtk_actor);


				mouseStyle->Data = vtk_dataSet;
				mouseStyle->actor = vtk_actor;
				mouseStyle->mapper = vtk_mapper;
				//	isExist = QFile(inPath + "/sect/preSect.dat").exists();
					//if (isExist) colorSect(0);

				m_pQVTKWidget->update();
				emit SignalInstance::getInstance()->signalPreLoad(70);

				//decimated Grid
				isExist = QFile(inPath + "/sect/preDecimatedGrid.dat").exists();
				if (isExist)readPreGrid(inPath + "/sect/preDecimatedGrid.dat", decimated_grid,1);
				else {
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"工作区不存在上次运行的简化网格文件");
					emit SignalInstance::getInstance()->signalCloseDialog();
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"网格解析完成");
					return;
				}
				isExist = QFile(inPath + "/sect/preDecimatedIdmap.dat").exists();
				if (isExist)readPreIdmap(inPath + "/sect/preDecimatedIdmap.dat", &decimated_idMap);
				else {
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"工作区不存在上次运行的简化网格文件");
					emit SignalInstance::getInstance()->signalCloseDialog();
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"网格解析完成");
					return;
				}
				isExist = QFile(inPath + "/sect/preIdTrans.dat").exists();
				if (isExist)readPreIdTrans(inPath + "/sect/preIdTrans.dat");
				else {
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"工作区不存在上次运行的简化网格文件");
					emit SignalInstance::getInstance()->signalCloseDialog();
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"网格解析完成");
					return;
				}
				isExist = QFile(inPath + "/sect/preDecimatedSect.dat").exists();
				if (isExist) {
					readPreSect(inPath + "/sect/preDecimatedSect.dat", decimated_sectInfo, decimated_grid);
				}
				else {
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"上次运行没有对简化网格分段");
					emit SignalInstance::getInstance()->signalCloseDialog();
					//emit SignalInstance::getInstance()->signalPrintInfo(u8"网格解析完成");
					return;
				}
				//emit SignalInstance::getInstance()->signalPrintInfo(u8"上次运行已经简化网格");
				buildDecimatedGrid();
				readPreLineInfo(inPath + "/sect/preLineInfo.dat", gridLine);
				readPreLineInfo(inPath + "/sect/preDecimatedLineInfo.dat", decimatedLine);

				selectDecimatedGrid();
				trPreLoad();
				ViewDirection(m_renderer, 0, 1, 0, 0, 0, 1);
				m_pQVTKWidget->update();

				emit SignalInstance::getInstance()->signalCloseDialog();
				//emit SignalInstance::getInstance()->signalPrintInfo(u8"网格解析完成");
			}
			void SceneWidget::onltShowTr() {
				
			}
			void SceneWidget::out_to_obj() {
				if (decimatedGrid == NULL) return;
				ofstream outFile(SignalInstance::getWorkSpace().toLocal8Bit().toStdString()+"/grid.obj",ios::out);
				//outFile << 0;
				for (int i = 0; i < decimatedGrid->GetNumberOfPoints(); i++) {
					outFile << "v " << decimatedGrid->GetPoint(i)[0] << " " << decimatedGrid->GetPoint(i)[1] << " " << decimatedGrid->GetPoint(i)[2] << std::endl;
				}
				for (int i = 0; i < decimatedGrid->GetNumberOfCells(); i++) {
					vtkIdList* ids = decimatedGrid->GetCell(i)->GetPointIds();
					outFile << "f " << ids->GetId(0)+1<<" "<<ids->GetId(1)+1<<" "<<ids->GetId(2)+1<<std::endl;
				}
				outFile.close();
			}
			void SceneWidget::onlyReadSect(QString inPath) {

				slotLoadSectData(inPath + "/TR.dat");
				//slotLoadSectData(inPath + "/TR.dat");
				colorSect(0);


#ifdef decimateSurface
				decimateGridSurface();
#else
				decimateGrid();
#endif // decimateSurface

				
				
				buildDecimatedGrid();
				//out_to_obj();
				colorSect(1);
				get_lineInfo();

				get_decimatedLineInfo();
				selectDecimatedGrid();
				//emit SignalInstance::getInstance()->signalPreLoad(70);
				//out_to_preGrid(inPath + "/preDecimatedGrid.dat", decimated_grid,1);
				//out_to_preIdmap(inPath + "/preDecimatedIdmap.dat", &decimated_idMap);
				//out_to_preSect(inPath + "/preDecimatedSect.dat", decimated_sectInfo);
				//out_to_preIdtrans(inPath + "/preIdTrans.dat",idTrans, transSize);
				//out_to_preLineInfo(inPath + "/preLineInfo.dat", gridLine);
				//out_to_preLineInfo(inPath + "/preDecimatedLineInfo.dat", decimatedLine);

			}
			void SceneWidget::onlyPreReadGirdSect(QString inPath) {
				
			}
			void SceneWidget::transform(block * bk, int n_blocks, UNSTRUCTURED_GRID * grid, int overlapFlag)
			{

				if (n_blocks == 0 || bk == NULL || grid == NULL) return;
				overlapFlag = defaultOverlapFLag;
				//cout << "--------------------------begin transform-----------------------" << endl;
				int totalPoints = 0, totalCells = 0;
				for (int i = 0; i < n_blocks; i++) {
					totalPoints += bk[i].numOfPoints;
					totalCells += bk[i].numOfCells;
					qDebug() << "bk[i].numOfCells=" << bk[i].numOfCells;
				}
				grid->set_nPoints(totalPoints);
				grid->set_nCells(totalCells);
				qDebug() << "totalCells=" << totalCells;
				bool flag = true;

				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								//x += coordinateOffset[id];
								if (overlapFlag == 1 && grid->index_points != get_id(ni, nj, nk, i, j, k, id, 1)) {
									flag = false;
								}
								if (overlapFlag == 0) {

									xyzPoint temp;
									temp.init(x, y, z);
									auto it = idMap.find(temp);
									if (it == idMap.end()) {
										// find fail

										idMap.insert(make_pair(temp, grid->index_points));

									}
									else {
										//find success
										int index = it->second;
										//grid->pointData[index].set_sectId(bk[id].sectId[i]);
										grid->pointData[index].set_sectId(0);
										continue;
									}
								}

								//grid->addPoint(x, y, z, i, j, k, id, bk[id].sectId[i]);
								grid->addPoint(x, y, z, i, j, k, id, 0);

							}
						}
					}
				}

				if (overlapFlag == 0) {

					grid->n_points = grid->index_points;
				}
				//change 
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk - 1; k++) {
						for (int j = 0; j < nj - 1; j++) {
							for (int i = 0; i < ni - 1; i++) {
								int id1 = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
								int id2 = get_id(ni, nj, nk, i + 1, j, k, id, overlapFlag);
								int id3 = get_id(ni, nj, nk, i, j + 1, k, id, overlapFlag);
								int id4 = get_id(ni, nj, nk, i, j, k + 1, id, overlapFlag);
								int id5 = get_id(ni, nj, nk, i + 1, j + 1, k, id, overlapFlag);
								int id6 = get_id(ni, nj, nk, i + 1, j, k + 1, id, overlapFlag);
								int id7 = get_id(ni, nj, nk, i, j + 1, k + 1, id, overlapFlag);
								int id8 = get_id(ni, nj, nk, i + 1, j + 1, k + 1, id, overlapFlag);
								/*grid->addVetraCell(id1, id2, id3, id7);
								grid->addVetraCell(id1, id3, id4, id7);
								grid->addVetraCell(id1, id2, id5, id7);
								grid->addVetraCell(id2, id5, id6, id7);
								grid->addVetraCell(id3, id5, id7, id8);*/
								grid->addVetraCell(id1, id2, id3, id4);
								grid->addVetraCell(id2, id6, id3, id4);
								grid->addVetraCell(id4, id6, id7, id3);
								grid->addVetraCell(id2, id5, id6, id3);
								grid->addVetraCell(id5, id8, id6, id7);
								grid->addVetraCell(id3, id5, id7, id6);
							}
						}
					}
				}
				grid->count_size_cells();
				//change
				//redution = 0.1;
				//int jGap = 1, kGap = 1;

				//deciInfo = new decimatedInfo;
				//deciInfo->init();
				//deciInfo->setNumOfPoints(grid->n_points);
				//deciInfo->setNumOfCells(grid->n_cells);
				////int overlapFlag = defaultOverlapFLag;
				//markPoints();
				////double redution = 0;
				////int jGap = 1, kGap = 1;
				//for (int id = 0; id < n_blocks; id++) {
				//	int ni, nj, nk;
				//	ni = bk[id].ni;
				//	nj = bk[id].nj;
				//	nk = bk[id].nk;
				//	jGap = nj * redution;
				//	kGap = nk * redution;
				//	if (jGap < 1) jGap = 1;
				//	if (kGap < 1) kGap = 1;
				//	for (int k = 0; k < nk - 1; k += kGap) {
				//		for (int j = 0; j < nj - 1; j += jGap) {
				//			for (int i = 0; i < ni - 1; i++) {
				//				int id1 = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
				//				int id2 = get_id(ni, nj, nk, i + 1, j, k, id, overlapFlag);
				//				int id3 = get_id(ni, nj, nk, i, min(j + jGap, nj - 1), k, id, overlapFlag);
				//				int id4 = get_id(ni, nj, nk, i, j, min(k + kGap, nk - 1), id, overlapFlag);
				//				int id5 = get_id(ni, nj, nk, i + 1, min(j + jGap, nj - 1), k, id, overlapFlag);
				//				int id6 = get_id(ni, nj, nk, i + 1, j, min(k + kGap, nk - 1), id, overlapFlag);
				//				int id7 = get_id(ni, nj, nk, i, min(j + jGap, nj - 1), min(k + kGap, nk - 1), id, overlapFlag);
				//				int id8 = get_id(ni, nj, nk, i + 1, min(j + jGap, nj - 1), min(k + kGap, nk - 1), id, overlapFlag);
				//				int idList[9] = { 0,id1,id2,id3,id4,id5,id6,id7,id8 };
				//				std::vector<std::vector<int>> cellId({ { 1,2,5}, { 1,3,5}, { 1,4,6}, { 1,2,6},
				//					{ 3,5,8}, { 3,7,8}, { 1,3,7}, { 1,4,7}, { 2,5,8}, { 2,6,8}, { 4,6,8}, { 4,7,8} });
				//				for (auto& it : cellId) {
				//					judgeAndAddCells(idList[it[0]], idList[it[1]], idList[it[2]]);
				//				}
				//			}
				//		}
				//	}
				//}

				//int numPoint = 0, numCells = 0;
				//numPoint = deciInfo->index;
				//numCells = deciInfo->cellIndex;
				////if (decimated_grid != NULL) {
				////	decimated_grid->clear();
				////	free(decimated_grid);
				////	decimated_grid = NULL;
				////}
				//decimated_grid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				//decimated_grid->init();
				////统计简化后的点
				//decimated_grid->set_nPoints(numPoint);
				//for (int i = 0; i < numPoint; i++) {
				//	int id = deciInfo->pointList[i];
				//	point* temp = &(grid->pointData[id]);
				//	xyzPoint tempKey;
				//	tempKey.init(temp->x, temp->y, temp->z);
				//	decimated_grid->addPoint(temp->x, temp->y, temp->z,
				//		temp->i, temp->j, temp->k, temp->blockId, temp->sectId);
				//}
				////统计简化后的cell
				//decimated_grid->set_nCells(numCells);
				//for (int i = 0; i < numCells; i++) {
				//	int id1 = deciInfo->cellList[i][0];
				//	int id2 = deciInfo->cellList[i][1];
				//	int id3 = deciInfo->cellList[i][2];
				//	decimated_grid->addVetraCell(id1, id2, id3);
				//}

				//decimated_grid->n_cells = decimated_grid->index_cells;
				//decimated_grid->n_points = decimated_grid->index_points;
				//deciInfo->clear();
				//delete deciInfo;
				////change##

				grid->print();
				grid->check();

			}
			//void SceneWidget::transform(block * bk, int n_blocks, UNSTRUCTURED_GRID * grid, int overlapFlag)
			//{
			//	if (n_blocks == 0 || bk == NULL || grid == NULL) return;
			//	//cout << "--------------------------begin transform-----------------------" << endl;
			//	int totalPoints = 0, totalCells = 0;
			//	for (int i = 0; i < n_blocks; i++) {

			//		totalPoints += bk[i].numOfPoints;
			//		totalCells += bk[i].numOfCells;
			//	}
			//	grid->set_nPoints(totalPoints);
			//	grid->set_nCells(totalCells);
			//	bool flag = true;

			//	for (int id = 0; id < n_blocks; id++) {
			//		int ni, nj, nk;
			//		ni = bk[id].ni;
			//		nj = bk[id].nj;
			//		nk = bk[id].nk;
			//		for (int k = 0; k < nk; k++) {
			//			for (int j = 0; j < nj; j++) {
			//				for (int i = 0; i < ni; i++) {
			//					double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
			//					double x = *X, y = *Y, z = *Z;
			//					//x += coordinateOffset[id];
			//					if (overlapFlag == 1 && grid->index_points != get_id(ni, nj, nk, i, j, k, id, 1)) {
			//						flag = false;
			//					}
			//					if (overlapFlag == 0) {

			//						xyzPoint temp;
			//						temp.init(x, y, z);
			//						auto it = idMap.find(temp);
			//						if (it == idMap.end()) {
			//							// find fail

			//							idMap.insert(make_pair(temp, grid->index_points));

			//						}
			//						else {
			//							//find success
			//							int index = it->second;
			//							//grid->pointData[index].set_sectId(bk[id].sectId[i]);
			//							grid->pointData[index].set_sectId(0);
			//							continue;
			//						}
			//					}

			//					//grid->addPoint(x, y, z, i, j, k, id, bk[id].sectId[i]);
			//					grid->addPoint(x, y, z, i, j, k, id, 0);

			//				}
			//			}
			//		}
			//	}

			//	if (overlapFlag == 0) {

			//		grid->n_points = grid->index_points;
			//	}
			//	for (int id = 0; id < n_blocks; id++) {
			//		int ni, nj, nk;
			//		ni = bk[id].ni;
			//		nj = bk[id].nj;
			//		nk = bk[id].nk;
			//		for (int k = 0; k < nk - 1; k++) {
			//			for (int j = 0; j < nj - 1; j++) {
			//				for (int i = 0; i < ni - 1; i++) {
			//					int id1 = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
			//					int id2 = get_id(ni, nj, nk, i + 1, j, k, id, overlapFlag);
			//					int id3 = get_id(ni, nj, nk, i, j + 1, k, id, overlapFlag);
			//					int id4 = get_id(ni, nj, nk, i, j, k + 1, id, overlapFlag);
			//					int id5 = get_id(ni, nj, nk, i + 1, j + 1, k, id, overlapFlag);
			//					int id6 = get_id(ni, nj, nk, i + 1, j, k + 1, id, overlapFlag);
			//					int id7 = get_id(ni, nj, nk, i, j + 1, k + 1, id, overlapFlag);
			//					int id8 = get_id(ni, nj, nk, i + 1, j + 1, k + 1, id, overlapFlag);
			//					grid->addVetraCell(id1, id2, id3, id4);
			//					grid->addVetraCell(id2, id6, id3, id4);
			//					grid->addVetraCell(id4, id6, id7, id3);
			//					grid->addVetraCell(id2, id5, id6, id3);
			//					grid->addVetraCell(id5, id8, id6, id7);
			//					grid->addVetraCell(id3, id5, id7, id6);

			//				}
			//			}
			//		}
			//	}
			//	grid->count_size_cells();


			//	grid->print();
			//	grid->check();

			//}
			void SceneWidget::out_to_file(QString file_name) {
				QFile file2(file_name + "/UNSTRUCTURED_GRID.vtk");
				if (!file2.open(QIODevice::ReadWrite | QIODevice::Text))
				{
					QMessageBox::critical(this, "错误", "文件加载失败，工作区未生成！", "确定");
					return;
				}
				QTextStream out2(&file2);

				if (un_grid == NULL) return;
				out2 << "# vtk DataFile Version 2.0" << endl;
				out2 << "unstructured_grid" << endl;
				out2 << "ASCII" << endl;
				out2 << "DATASET UNSTRUCTURED_GRID" << endl;
				out2 << "POINTS" << " " << un_grid->n_points << " " << "double" << endl;
				for (int i = 0; i < un_grid->n_points; i++) {
					out2 << un_grid->pointData[i].x << " " << un_grid->pointData[i].y << " " << un_grid->pointData[i].z << endl;
				}
				out2 << endl;

				out2 << "CELLS" << " " << un_grid->n_cells << " " << un_grid->size_cells << endl;
				for (int i = 0; i < un_grid->n_cells; i++) {
					out2 << un_grid->cellData[i].numPoints;
					for (int j = 0; j < un_grid->cellData[i].numPoints; j++) {
						out2 << " " << un_grid->cellData[i].idPoint[j];
					}
					out2 << endl;
				}
				out2 << endl;

				out2 << "CELL_TYPES" << " " << un_grid->n_cells << endl;
				for (int i = 0; i < un_grid->n_cells; i++) {
					out2 << un_grid->cellData[i].type_id << endl;
				}
				file2.close();

			}

			void SceneWidget::build_sectPool(sectPool * sectInfo, UNSTRUCTURED_GRID * grid)
			{
				for (int i = 0; i < grid->n_points; i++) {
					int ret = get_sect(i, grid);
					sectInfo->pointNum[get_sect(i, grid)]++;
				}
				for (int i = 0; i < sectInfo->sectNum; i++) {
					sectInfo->setPointNum(i, sectInfo->pointNum[i]);
				}
				for (int i = 0; i < grid->n_points; i++) {
					sectInfo->addPoint(grid->pointData[i].sectId, i);

				}
				sectInfo->check();
			}

			void SceneWidget::judge_link(int blockId1, int blockId2)
			{
				bool flag = true;
				set<xyzPoint> s;
				for (int id = blockId1; id < blockId1 + 1; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = ni - 1; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								xyzPoint temp;
								temp.init(x, 0, 0);
								s.insert(temp);
							}
						}
					}
				}
				for (int id = blockId2; id < blockId2 + 1; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < 1; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								xyzPoint temp;
								temp.init(x, 0, 0);
								auto it = s.find(temp);
								if (it == s.end()) {
									flag = false;
								}
							}
						}
					}
				}
				s.clear();
				for (int id = blockId2; id < blockId2 + 1; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = ni - 1; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								xyzPoint temp;
								temp.init(x, 0, 0);
								s.insert(temp);
							}
						}
					}
				}
				for (int id = blockId1; id < blockId1 + 1; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < 1; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								xyzPoint temp;
								temp.init(x, 0, 0);
								auto it = s.find(temp);
								if (it == s.end()) {
									flag = false;
								}
							}
						}
					}
				}
			}

			void SceneWidget::get_line_range(int blockId1, int line)
			{
				double min_x = *bk[blockId1].X(line, 0, 0), max_x = *bk[blockId1].X(line, 0, 0);
				for (int id = blockId1; id < blockId1 + 1; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = line; i < line + 1; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								if (x < min_x) min_x = x;
								if (x > max_x) max_x = x;
							}
						}
					}
				}

			}

			void SceneWidget::test()
			{
				judge_link(0, 2);
				judge_link(1, 4);
				judge_link(2, 5);

				for (int i = 0; i < n_blocks; i++) {
					get_line_range(i, 0);
					get_line_range(i, bk[i].ni - 1);
					//cout << endl;
				}


			}

			void SceneWidget::test2()
			{
				double *X = bk[0].X(0, 0, 0), *Y = bk[0].Y(0, 0, 0), *Z = bk[0].Z(0, 0, 0);
				double x = *X, y = *Y, z = *Z;

				double *X2 = bk[0].X(1, 4, 3), *Y2 = bk[0].Y(1, 4, 3), *Z2 = bk[0].Z(1, 4, 3);
				double x2 = *X2, y2 = *Y2, z2 = *Z2;

				map<xyzPoint, int> mp;
				xyzPoint temp;
				temp.init(x, y, z);
				xyzPoint temp2;
				temp2.init(x2, y2, z2);
				auto it = mp.find(temp);
				if (it != mp.end()) {
					//cout << "find!" << endl;
					//cout << it->second << endl;
				}
				else {
					mp[temp] = 1;
					//mp.insert(make_pair(temp,1));
				}
				it = mp.find(temp2);
				if (it != mp.end()) {
					//cout << "find!" << endl;
					//cout << it->second << endl;
				}
				else {
					mp[temp2] = 2;
					//mp.insert(make_pair(temp2, 2));
				}
				for (it = mp.begin(); it != mp.end(); it++) {
					//cout << "key=" << it->first.x << " " << it->first.y << " " << it->first.z << " value=" << it->second << endl;
				}
			}

			void SceneWidget::read_grid(string file_name)
			{
				
				un_grid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				memset(un_grid, 0, sizeof(un_grid));
				un_grid->init();

				/*ifstream file2(file_name, ios::in);
				file2 >> n_blocks;*/
				
				auto start_t = std::chrono::high_resolution_clock::now();
				


				FILE* file = fopen(file_name.c_str(), "r");
				if (file == NULL) {
					qDebug() << "open fail";
					exit(EXIT_FAILURE);
				}
				char line[256];
				fgets(line, sizeof(line), file);
				sscanf(line, "%d", &n_blocks);
				bk = (block*)malloc(n_blocks * sizeof(block));
				memset(bk, 0, sizeof(bk));
				//qDebug() << n_blocks;
				for (int id = 0; id < n_blocks; id++) {
					fgets(line, sizeof(line), file);
					int ni, nj, nk;
					sscanf(line, "%d %d %d", &ni,&nj,&nk);
					//qDebug() << ni << " " << nj << " "<<nk;
					bk[id].ni = ni;
					bk[id].nj = nj;
					bk[id].nk = nk;
					bk[id].build();
				}
				int id = 0, ni = 0, nj = 0, nk = 0, flag = 0;
				while (fgets(line, sizeof(line), file) != NULL) {
					double data[4];
					int count = sscanf(line, "%lf %lf %lf %lf", &data[0], &data[1], &data[2], &data[3]);
					for (int i = 0; i < count; i++) {
						if (flag == 0)  *bk[id].X(ni, nj, nk) = data[i];
						else if(flag==1) *bk[id].Y(ni, nj, nk) = data[i];
						else if(flag==2) *bk[id].Z(ni, nj, nk) = data[i];
						ni++; 
						if (ni == bk[id].ni) {
							nj++; ni = 0;
							if (nj == bk[id].nj) {
								nk++; nj = 0;
								if (nk == bk[id].nk) {
									nk = 0; flag++;
									if (flag == 3) { flag = 0; id++; }
									if (id >= n_blocks) break;
								}
							}
						}
					}
					if (id >= n_blocks) break;
				}
				fclose(file);

				{auto end_t = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = end_t - start_t;
				qDebug() << u8"time2 " << duration.count() << " "; }
				for (int i = 0; i < n_blocks; i++) bk[i].get_info();
				for (int i = 0; i < n_blocks; i++) {
					qDebug() << "min_x=" << bk[i].min_x << " max_x=" << bk[i].max_x;
					qDebug() << "min_y=" << bk[i].min_y << " max_y=" << bk[i].max_y;
					qDebug() << "min_z=" << bk[i].min_z << " max_z=" << bk[i].max_z;
					
				}
				gridBound[0] = bk[0].min_x; gridBound[1] =  bk[0].max_x;
				gridBound[2] = bk[0].min_y; gridBound[3] = bk[0].max_y;
				gridBound[4] = bk[0].min_z; gridBound[5] = bk[0].max_z;

				for (int i = 0; i < n_blocks; i++) {
					gridBound[0] = std::min(gridBound[0], bk[i].min_x); gridBound[1] = std::max(gridBound[1], bk[i].max_x);
					gridBound[2] = std::min(gridBound[2], bk[i].min_y); gridBound[3] = std::max(gridBound[3], bk[i].max_y);
					gridBound[4] = std::min(gridBound[4], bk[i].min_z); gridBound[5] = std::max(gridBound[5], bk[i].max_z);
				}
				{auto end_t = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = end_t - start_t;
				qDebug() << u8"time3 " << duration.count() << " "; }
				
				for (int i = 0; i < n_blocks; i++) bk[i].id = i;
				for (int i = 0; i < n_blocks; i++) bk[i].print();
				//cout << "-----read grid finish:success!----" << endl;
				//file2.close();
			}

			void SceneWidget::read_dat(string file_name)
			{
				un_grid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				memset(un_grid, 0, sizeof(un_grid));
				un_grid->init();

				ifstream file2(file_name, ios::in);
				file2 >> n_blocks;
				bk = (block*)malloc(n_blocks * sizeof(block));
				memset(bk, 0, sizeof(bk));

				for (int id = 0; id < n_blocks; id++) {
					bk[id].id = id;
					int ni, nj, nk;
					file2 >> ni >> nj >> nk;
					bk[id].ni = ni;
					bk[id].nj = nj;
					bk[id].nk = nk;
					bk[id].build();
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								file2 >> *X;
							}
						}
					}
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								file2 >> *Y;
							}
						}
					}
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								file2 >> *Z;
							}
						}
					}

				}
				//for (int i = 0; i < n_blocks; i++) bk[i].get_info();
				////sort(bk, bk + n_blocks);
				//for (int i = 0; i < n_blocks; i++) bk[i].id = i;
				//for (int i = 0; i < n_blocks; i++) bk[i].print();
				//cout << "-----read dat finish:success!----" << endl;
				file2.close();

				
				for (int i = 0; i < n_blocks; i++) bk[i].get_info();

				
				gridBound[0] = bk[0].min_x; gridBound[1] = bk[0].max_x;
				gridBound[2] = bk[0].min_y; gridBound[3] = bk[0].max_y;
				gridBound[4] = bk[0].min_z; gridBound[5] = bk[0].max_z;

				for (int i = 0; i < n_blocks; i++) {
					gridBound[0] = std::min(gridBound[0], bk[i].min_x); gridBound[1] = std::max(gridBound[1], bk[i].max_x);
					gridBound[2] = std::min(gridBound[2], bk[i].min_y); gridBound[3] = std::max(gridBound[3], bk[i].max_y);
					gridBound[4] = std::min(gridBound[4], bk[i].min_z); gridBound[5] = std::max(gridBound[5], bk[i].max_z);
				}


				for (int i = 0; i < n_blocks; i++) bk[i].id = i;
				for (int i = 0; i < n_blocks; i++) bk[i].print();

			}

			void SceneWidget::read_sect(string file_name)
			{
				ifstream file(file_name, ios::in);
				if (file.is_open() == false) {
					qDebug() << "not open!:"<<file_name.c_str();
				}
				stringstream line_stream;
				string line;
				string temp;// to buffer unused string
				int linenum = 0;
				int sect = 0;
				int min_id = 0, max_id = 0;
				getline(file, line);//line1 title
				getline(file, line);//line2 no meaning
				while (getline(file, line)) {
					linenum++;
					if (line.size() == 1) continue;
					if (line[0] == '#') {// #Sect 1

						line_stream << line;
						line_stream >> temp >> sect;
						line_stream.clear();

						sect--;

						getline(file, line);
						line_stream << line;
						line_stream >> min_id >> max_id;
						line_stream.clear();

						min_id--;
						max_id--;

						getline(file, line);
					}
					else {
						int blockId = 0;
						line_stream << line;
						line_stream >> temp >> blockId;
						line_stream.clear();

						blockId--;
						if (bk[blockId].sectFlag == 0) {
							bk[blockId].sectFlag = 1;
							bk[blockId].sectOffset = min_id;
						}

						for (int i = min_id; i <= max_id; i++) {
							int lineId = i - bk[blockId].sectOffset;
							bk[blockId].sectId[lineId] = sect;
						}

					}


				}
				sectInfo = (sectPool*)malloc(sizeof(sectPool));
				memset(sectInfo, 0, sizeof(sectInfo));
				sectInfo->init();
				sectInfo->set_sectNum(sect + 1);


				//cout << "-----read sect finish:success!----" << endl;
				file.close();
			}

			void SceneWidget::out_to_preBlock(QString file_name) {
				FILE* fp = NULL;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "wb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {


				}
				fwrite(&n_blocks, sizeof(int), 1, fp);
				double bufDouble[4];
				int bufInt[8];
				for (int i = 0; i < n_blocks; i++) {
					bufDouble[0] = bk[i].min_x;
					bufDouble[1] = bk[i].max_x;
					bufDouble[2] = bk[i].min_index;
					bufDouble[3] = bk[i].max_index;
					fwrite(bufDouble, sizeof(double), 4, fp);
					bufInt[0] = bk[i].id;
					bufInt[1] = bk[i].ni;
					bufInt[2] = bk[i].nj;
					bufInt[3] = bk[i].nk;
					bufInt[4] = bk[i].numOfPoints;
					bufInt[5] = bk[i].numOfCells;
					bufInt[6] = bk[i].sectOffset;
					bufInt[7] = bk[i].sectFlag;
					fwrite(bufInt, sizeof(int), 8, fp);
					qDebug() << "i=" << i << " bufInt[4]=" << bufInt[4];
					fwrite(bk[i].x, sizeof(double), bk[i].ni*bk[i].nj*bk[i].nk, fp);
					fwrite(bk[i].y, sizeof(double), bk[i].ni*bk[i].nj*bk[i].nk, fp);
					fwrite(bk[i].z, sizeof(double), bk[i].ni*bk[i].nj*bk[i].nk, fp);
					fwrite(bk[i].sectId, sizeof(int), bk[i].ni, fp);
				}
				for (int i = 0; i < n_blocks; i++) {
					qDebug() << "i=" << i << " bk[i].numPoints=" << bk[i].numOfPoints;
				}

				fclose(fp);
				fp = NULL;
			}
			void SceneWidget::readPreBlock(QString file_name) {
				FILE* fp = NULL;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "rb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {

				}
				int blockNum;
				fread(&blockNum, sizeof(int), 1, fp);
				n_blocks = blockNum;
				bk = (block*)malloc(sizeof(block)*blockNum);
				double bufDouble[4];
				int bufInt[8];
				for (int i = 0; i < n_blocks; i++) {
					fread(bufDouble, sizeof(double), 4, fp);
					bk[i].min_x = bufDouble[0];
					bk[i].max_x = bufDouble[1];
					bk[i].min_index = bufDouble[2];
					bk[i].max_index = bufDouble[3];
					fread(bufInt, sizeof(int), 8, fp);
					bk[i].id = bufInt[0];
					bk[i].ni = bufInt[1];
					bk[i].nj = bufInt[2];
					bk[i].nk = bufInt[3];
					bk[i].numOfPoints = bufInt[4];
					bk[i].numOfCells = bufInt[5];
					bk[i].sectOffset = bufInt[6];
					bk[i].sectFlag = bufInt[7];
					qDebug() << "i=" << i << " bufInt[4]=" << bufInt[4];
					bk[i].reBuild();
					//bk[i].build();
					fread(bk[i].x, sizeof(double), bk[i].ni*bk[i].nj*bk[i].nk, fp);
					fread(bk[i].y, sizeof(double), bk[i].ni*bk[i].nj*bk[i].nk, fp);
					fread(bk[i].z, sizeof(double), bk[i].ni*bk[i].nj*bk[i].nk, fp);
					fread(bk[i].sectId, sizeof(int), bk[i].ni, fp);
				}
				for (int i = 0; i < n_blocks; i++) {
					qDebug() << "i=" << i << " bk[i].numPoints=" << bk[i].numOfPoints;
				}
				fclose(fp);
				fp = NULL;
				if (idOffset == NULL) {
					get_idOffset(n_blocks, bk);
				}

			}
			void SceneWidget::out_to_preGrid(QString file_name, UNSTRUCTURED_GRID* un_grid,int decimateGridFlag) {
				//old 耗时25s

				FILE* fp;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "wb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {

				}
				//fwrite fread 272ms
				fwrite(&un_grid->n_points, sizeof(int), 1, fp);
				for (int i = 0; i < un_grid->n_points; i++) {
					fwrite(&un_grid->pointData[i], sizeof(point), 1, fp);
				}
				fwrite(&un_grid->n_cells, sizeof(int), 1, fp);
				for (int i = 0; i < un_grid->n_cells; i++) {
					cell* temp = &(un_grid->cellData[i]);
					if (decimateGridFlag == 1) {
#ifdef decimateSurface
						fwrite(temp->idPoint, sizeof(int), 3, fp);
#else
						fwrite(temp->idPoint, sizeof(int), 4, fp);
#endif // decimateSurface
					}else fwrite(temp->idPoint, sizeof(int), 4, fp);
					//fprintf(fp, "%lf %lf %lf %lf\n", temp->idPoint[0], temp->idPoint[1], temp->idPoint[2], temp->idPoint[3]);
				}
				fwrite(&un_grid->size_cells, sizeof(int), 1, fp);
				fclose(fp);
				fp = nullptr;
			}
			void SceneWidget::readPreGrid(QString file_name, UNSTRUCTURED_GRID* &un_grid,int decimateGridFlag) {
				
				FILE* fp;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "rb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {

				}
				un_grid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				memset(un_grid, 0, sizeof(un_grid));
				un_grid->init();
				int n_points;
				fread(&n_points, sizeof(int), 1, fp);
				un_grid->set_nPoints(n_points);
				point temp;
				for (int I = 0; I < n_points; I++) {
					fread(&temp, sizeof(point), 1, fp);
					un_grid->addPoint(temp.x, temp.y, temp.z, temp.i, temp.j, temp.k, temp.blockId, temp.sectId);
				}
				int n_cells;
				fread(&n_cells, sizeof(int), 1, fp);
				un_grid->set_nCells(n_cells);
				if (decimateGridFlag == 1) {
#ifdef decimateSurface
					int idBuf[3];
					for (int I = 0; I < un_grid->n_cells; I++) {
						fread(idBuf, sizeof(int), 3, fp);
						un_grid->addVetraCell(idBuf[0], idBuf[1], idBuf[2]);
					}
					int size_cells;
					fread(&size_cells, sizeof(int), 1, fp);
					un_grid->size_cells = size_cells;
					fclose(fp);
					fp = NULL;
					qDebug() << "n_cells=" << n_cells;
					return;
#endif // decimateSurface
				}
				int idBuf[4];
				for (int I = 0; I < un_grid->n_cells; I++) {
					fread(idBuf, sizeof(int), 4, fp);
					un_grid->addVetraCell(idBuf[0], idBuf[1], idBuf[2], idBuf[3]);
				}
				int size_cells;
				fread(&size_cells, sizeof(int), 1, fp);
				un_grid->size_cells = size_cells;
				fclose(fp);
				fp = NULL;
				
			}
			void SceneWidget::out_to_preSect(QString file_name, sectPool* sectInfo) {
				FILE* fp;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "wb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {


				}
				fwrite(&sectInfo->sectNum, sizeof(int), 1, fp);
				fwrite(sectInfo->pointNum, sizeof(int), sectInfo->sectNum, fp);
				for (int i = 0; i < sectInfo->sectNum; i++) {
					fwrite(sectInfo->idInfo[i], sizeof(int), sectInfo->pointNum[i], fp);
				}
				fwrite(sectInfo->min_x, sizeof(double), sectInfo->sectNum, fp);
				fwrite(sectInfo->max_x, sizeof(double), sectInfo->sectNum, fp);
				fwrite(sectInfo->min_y, sizeof(double), sectInfo->sectNum, fp);
				fwrite(sectInfo->max_y, sizeof(double), sectInfo->sectNum, fp);
				fwrite(sectInfo->min_z, sizeof(double), sectInfo->sectNum, fp);
				fwrite(sectInfo->max_z, sizeof(double), sectInfo->sectNum, fp);


				fwrite(&sectInfo->n_points, sizeof(int), 1, fp);
				fwrite(sectInfo->sectIdMap, sizeof(int), sectInfo->n_points, fp);
				fwrite(sectInfo->cellNum, sizeof(int), sectInfo->sectNum, fp);
				for (int i = 0; i < sectInfo->sectNum; i++) {
					fwrite(sectInfo->cellId1[i], sizeof(int), sectInfo->cellNum[i], fp);
					fwrite(sectInfo->cellId2[i], sizeof(int), sectInfo->cellNum[i], fp);
					fwrite(sectInfo->cellId3[i], sizeof(int), sectInfo->cellNum[i], fp);
					fwrite(sectInfo->cellId4[i], sizeof(int), sectInfo->cellNum[i], fp);
				}
				fclose(fp);
				fp = NULL;

			}
			void SceneWidget::readPreSect(QString file_name, sectPool* &sectInfo, UNSTRUCTURED_GRID* &un_grid) {
				FILE* fp;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "rb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {


				}
				sectInfo = (sectPool*)malloc(sizeof(sectPool));
				memset(sectInfo, 0, sizeof(sectInfo));
				sectInfo->init();
				int sectNum;
				fread(&sectNum, sizeof(int), 1, fp);
				sectInfo->set_sectNum(sectNum);
				fread(sectInfo->pointNum, sizeof(int), sectInfo->sectNum, fp);
				for (int i = 0; i < sectNum; i++) {
					sectInfo->setPointNum(i, sectInfo->pointNum[i]);
					sectInfo->index[i] = sectInfo->pointNum[i];
				}
				for (int i = 0; i < sectInfo->sectNum; i++) {
					fread(sectInfo->idInfo[i], sizeof(int), sectInfo->pointNum[i], fp);
				}
				fread(sectInfo->min_x, sizeof(double), sectInfo->sectNum, fp);
				fread(sectInfo->max_x, sizeof(double), sectInfo->sectNum, fp);
				fread(sectInfo->min_y, sizeof(double), sectInfo->sectNum, fp);
				fread(sectInfo->max_y, sizeof(double), sectInfo->sectNum, fp);
				fread(sectInfo->min_z, sizeof(double), sectInfo->sectNum, fp);
				fread(sectInfo->max_z, sizeof(double), sectInfo->sectNum, fp);

				fread(&sectInfo->n_points, sizeof(int), 1, fp);
				sectInfo->setNumofPoints(sectInfo->n_points);
				fread(sectInfo->sectIdMap, sizeof(int), sectInfo->n_points, fp);
				fread(sectInfo->cellNum, sizeof(int), sectInfo->sectNum, fp);
				for (int i = 0; i < sectInfo->sectNum; i++) {
					sectInfo->setCellNum(i, sectInfo->cellNum[i]);
				}
				for (int i = 0; i < sectInfo->sectNum; i++) {
					fread(sectInfo->cellId1[i], sizeof(int), sectInfo->cellNum[i], fp);
					fread(sectInfo->cellId2[i], sizeof(int), sectInfo->cellNum[i], fp);
					fread(sectInfo->cellId3[i], sizeof(int), sectInfo->cellNum[i], fp);
					fread(sectInfo->cellId4[i], sizeof(int), sectInfo->cellNum[i], fp);
				}

				fclose(fp);
				fp = NULL;
				//update grid sectId
				int n_points = 0;
				for (int i = 0; i < sectInfo->sectNum; i++) {
					for (int j = 0; j < sectInfo->pointNum[i]; j++) {
						int id = sectInfo->idInfo[i][j];
						un_grid->pointData[id].sectId = i;
						n_points++;
					}
				}
				//qDebug() << "sectInfo pointsNum=" << n_points << " un_grid points=" << un_grid->n_points;
			}
			void SceneWidget::out_to_preIdtrans(QString file_name, int *idTrans,int transSize) {
				FILE* fp;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "wb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {

				}
				int size = transSize;
				fwrite(&size, sizeof(int), 1, fp);
				fwrite(idTrans,sizeof(int),transSize,fp);
				fclose(fp);
				fp = NULL;
			}
			void SceneWidget::out_to_preIdmap(QString file_name, map<xyzPoint, int>* idMap) {
				FILE* fp;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "wb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {

				}
				int size = idMap->size();
				fwrite(&size, sizeof(int), 1, fp);
				xyzPoint* tempKey = NULL;
				tempKey = (xyzPoint*)malloc(sizeof(xyzPoint)*size);
				memset(tempKey, 0, sizeof(xyzPoint)*size);
				int* tempValue = NULL;
				tempValue = (int*)malloc(sizeof(int)*size);
				memset(tempValue, 0, sizeof(int)*size);
				int i = 0;
				for (auto it = idMap->begin(); it != idMap->end(); it++) {
					tempKey[i].init(it->first.x, it->first.y, it->first.z);
					tempValue[i] = it->second;
					i++;
				}
				fwrite(tempKey, sizeof(xyzPoint), size, fp);
				fwrite(tempValue, sizeof(int), size, fp);
				free(tempValue);
				tempValue = NULL;
				free(tempKey);
				tempKey = NULL;
				fclose(fp);
				fp = NULL;
			}
			void SceneWidget::readPreIdTrans(QString file_name) {
				FILE* fp;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "rb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {

				}
				if (idTrans != NULL) {
					delete []idTrans;
					idTrans = NULL;
				}
				transSize = 0;
				fread(&transSize, sizeof(int), 1, fp);
				idTrans = new int[transSize]();
				fread(idTrans, sizeof(int), transSize, fp);
				
				qDebug() << "read preIdTrans finish";

				fclose(fp);
				fp = NULL;
			}
			void SceneWidget::readPreIdmap(QString file_name, map<xyzPoint, int>* idMap) {
				FILE* fp;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "rb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
					
				}
				else {

				}
				int size = 0;
				fread(&size, sizeof(int), 1, fp);
				xyzPoint* tempKey = NULL;
				tempKey = (xyzPoint*)malloc(sizeof(xyzPoint)*size);
				memset(tempKey, 0, sizeof(xyzPoint)*size);
				int* tempValue = NULL;
				tempValue = (int*)malloc(sizeof(int)*size);
				memset(tempValue, 0, sizeof(int)*size);

				fread(tempKey, sizeof(xyzPoint), size, fp);
				fread(tempValue, sizeof(int), size, fp);
				idMap->clear();
				for (int i = 0; i < size; i++) {
					idMap->insert(make_pair(tempKey[i], tempValue[i]));
				}
				free(tempValue);
				tempValue = NULL;
				free(tempKey);
				tempKey = NULL;
				fclose(fp);
				fp = NULL;
			}
			void SceneWidget::out_to_preLineInfo(QString file_name, lineInfo* gridLine) {
				FILE* fp = NULL;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "wb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {

				}
				int lineNum = gridLine->lineNum;
				fwrite(&lineNum, sizeof(int), 1, fp);
				fwrite(gridLine->pointNum, sizeof(int), lineNum, fp);
				fwrite(gridLine->sectId, sizeof(int), lineNum, fp);
				for (int i = 0; i < lineNum; i++) {
					fwrite(gridLine->pointList[i], sizeof(int), gridLine->pointNum[i], fp);
				}

				fclose(fp);
				fp = NULL;
			}
			void SceneWidget::readPreLineInfo(QString file_name, lineInfo* &gridLine) {
				/*new error*/
				FILE* fp = NULL;
				if ((fp = fopen(file_name.toLocal8Bit().toStdString().c_str(), "rb")) == NULL) {
					qDebug() << "Fail to open file!";
					//exit(0);
				}
				else {

				}
				gridLine = (lineInfo*)malloc(sizeof(lineInfo));
				int lineNum;
				fread(&lineNum, sizeof(int), 1, fp);
				gridLine->set_lineNum(lineNum);
				fread(gridLine->pointNum, sizeof(int), lineNum, fp);
				fread(gridLine->sectId, sizeof(int), lineNum, fp);
				for (int i = 0; i < lineNum; i++) {
					gridLine->setPointNum(i, gridLine->pointNum[i]);
				}
				for (int i = 0; i < lineNum; i++) {
					fread(gridLine->pointList[i], sizeof(int), gridLine->pointNum[i], fp);
				}


				fclose(fp);
				fp = NULL;
			}

			//辅助简化网格的函数
			void SceneWidget::judgeAndAddCells(int id1, int id2, int id3, int id4) {
				if (deciInfo == NULL) return;
				int sectId1 = get_sect(id1, un_grid);
				int sectId2 = get_sect(id2, un_grid);
				int sectId3 = get_sect(id3, un_grid);
				int sectId4 = get_sect(id4, un_grid);

				//删除内部sect信息
				if (deciInfo->pointMark[id1] == 0 && deciInfo->pointMark[id2] == 0
					&& deciInfo->pointMark[id3] == 0 && deciInfo->pointMark[id4] == 0
					) return;
				//保留内部sect信息
				/*if (deciInfo->pointMark[id1] == 0 && deciInfo->pointMark[id2] == 0
					&& deciInfo->pointMark[id3] == 0 && deciInfo->pointMark[id4] == 0
					&& sectId1 == sectId2 && sectId2 == sectId3 && sectId3 == sectId4) return;*/
				if (deciInfo->pointIdMap[id1] == -1) {
					deciInfo->addPoint(id1);
				}
				if (deciInfo->pointIdMap[id2] == -1) {
					deciInfo->addPoint(id2);
				}
				if (deciInfo->pointIdMap[id3] == -1) {
					deciInfo->addPoint(id3);
				}
				if (deciInfo->pointIdMap[id4] == -1) {
					deciInfo->addPoint(id4);
				}
				int deciId1 = deciInfo->pointIdMap[id1];
				int deciId2 = deciInfo->pointIdMap[id2];
				int deciId3 = deciInfo->pointIdMap[id3];
				int deciId4 = deciInfo->pointIdMap[id4];
				deciInfo->addCell(deciId1, deciId2, deciId3, deciId4);
			}
			void SceneWidget::judgeAndAddCells(int id1, int id2, int id3) {
				if (deciInfo == NULL) return;
				int sectId1 = get_sect(id1, un_grid);
				int sectId2 = get_sect(id2, un_grid);
				int sectId3 = get_sect(id3, un_grid);
				//删除内部sect信息
				if (deciInfo->pointMark[id1] == 0 || deciInfo->pointMark[id2] == 0
					|| deciInfo->pointMark[id3] == 0 
					) return;
				//保留内部sect信息
				/*if (deciInfo->pointMark[id1] == 0 && deciInfo->pointMark[id2] == 0
					&& deciInfo->pointMark[id3] == 0 && deciInfo->pointMark[id4] == 0
					&& sectId1 == sectId2 && sectId2 == sectId3 && sectId3 == sectId4) return;*/
				if (deciInfo->pointIdMap[id1] == -1) {
					deciInfo->addPoint(id1);
				}
				if (deciInfo->pointIdMap[id2] == -1) {
					deciInfo->addPoint(id2);
				}
				if (deciInfo->pointIdMap[id3] == -1) {
					deciInfo->addPoint(id3);
				}
				int deciId1 = deciInfo->pointIdMap[id1];
				int deciId2 = deciInfo->pointIdMap[id2];
				int deciId3 = deciInfo->pointIdMap[id3];
				deciInfo->addCell(deciId1, deciId2, deciId3);
			}
			void SceneWidget::markPoints() {
				int overlapFlag = defaultOverlapFLag;
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk - 1; k++) {
						for (int j = 0; j < nj - 1; j++) {
							for (int i = 0; i < ni - 1; i++) {
								// 0:(0,0,-1) 1:(0,0,1)
								// 2:(0,-1,0) 3:(0,1,0)
								// 4:(-1,0,0) 5:(1,0,0)
								int id1 = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
								int id2 = get_id(ni, nj, nk, i + 1, j, k, id, overlapFlag);
								int id3 = get_id(ni, nj, nk, i, j + 1, k, id, overlapFlag);
								int id4 = get_id(ni, nj, nk, i, j, k + 1, id, overlapFlag);
								int id5 = get_id(ni, nj, nk, i + 1, j + 1, k, id, overlapFlag);
								int id6 = get_id(ni, nj, nk, i + 1, j, k + 1, id, overlapFlag);
								int id7 = get_id(ni, nj, nk, i, j + 1, k + 1, id, overlapFlag);
								int id8 = get_id(ni, nj, nk, i + 1, j + 1, k + 1, id, overlapFlag);

								//设置每个点的block


								deciInfo->mark(id1, 5);
								deciInfo->mark(id1, 3);
								deciInfo->mark(id1, 1);

								deciInfo->mark(id2, 4);
								deciInfo->mark(id2, 3);
								deciInfo->mark(id2, 1);

								deciInfo->mark(id3, 5);
								deciInfo->mark(id3, 2);
								deciInfo->mark(id3, 1);

								deciInfo->mark(id4, 5);
								deciInfo->mark(id4, 3);
								deciInfo->mark(id4, 0);

								deciInfo->mark(id5, 4);
								deciInfo->mark(id5, 2);
								deciInfo->mark(id5, 1);

								deciInfo->mark(id6, 4);
								deciInfo->mark(id6, 3);
								deciInfo->mark(id6, 0);

								deciInfo->mark(id7, 5);
								deciInfo->mark(id7, 2);
								deciInfo->mark(id7, 0);

								deciInfo->mark(id8, 4);
								deciInfo->mark(id8, 2);
								deciInfo->mark(id8, 0);
							}
						}
					}
				}
				for (int i = 0; i < un_grid->n_points; i++) {
					int count = 0;
					for (int j = 0; j < 6; j++) count += deciInfo->direMark[i][j];
					if (count < 6) {
						//numPoint++;
						deciInfo->pointMark[i] = 1;
					}
				}
			}
			void SceneWidget::decimateGridSurface() {
				qDebug() << "decimateGridSurface" << endl;
				deciInfo = new decimatedInfo;
				deciInfo->init();
				deciInfo->setNumOfPoints(un_grid->n_points);
				deciInfo->setNumOfCells(un_grid->n_cells);
				qDebug() << "n_cells=" << un_grid->n_cells;
				int overlapFlag = defaultOverlapFLag;
				markPoints();
				double redution = 0;
				int jGap = 1, kGap = 1;
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					jGap = nj * redution;
					kGap = nk * redution;
					if (jGap < 1) jGap = 1;
					if (kGap < 1) kGap = 1;
					for (int k = 0; k < nk - 1; k += kGap) {

						for (int j = 0; j < nj - 1; j += jGap) {
							for (int i = 0; i < ni - 1; i++) {
								int id1 = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
								int id2 = get_id(ni, nj, nk, i + 1, j, k, id, overlapFlag);
								int id3 = get_id(ni, nj, nk, i, min(j + jGap, nj - 1), k, id, overlapFlag);
								int id4 = get_id(ni, nj, nk, i, j, min(k + kGap, nk - 1), id, overlapFlag);
								int id5 = get_id(ni, nj, nk, i + 1, min(j + jGap, nj - 1), k, id, overlapFlag);
								int id6 = get_id(ni, nj, nk, i + 1, j, min(k + kGap, nk - 1), id, overlapFlag);
								int id7 = get_id(ni, nj, nk, i, min(j + jGap, nj - 1), min(k + kGap, nk - 1), id, overlapFlag);
								int id8 = get_id(ni, nj, nk, i + 1, min(j + jGap, nj - 1), min(k + kGap, nk - 1), id, overlapFlag);
								int idList[9] = { 0,id1,id2,id3,id4,id5,id6,id7,id8 };
								std::vector<std::vector<int>> cellId({ { 1,2,5}, { 1,3,5}, { 1,4,6}, { 1,2,6}, 
									{ 3,5,8}, { 3,7,8}, { 1,3,7}, { 1,4,7}, { 2,5,8}, { 2,6,8}, { 4,6,8}, { 4,7,8} });
								for (auto& it : cellId) {
									judgeAndAddCells(idList[it[0]], idList[it[1]], idList[it[2]]);
								}
							}
						}
					}

				}
				int numPoint = 0,numCells=0;
				numPoint = deciInfo->index;
				numCells = deciInfo->cellIndex;

				decimated_idMap.clear();
				if (idTrans != NULL) {
					delete[]idTrans;
					idTrans = NULL;
				}
				transSize = un_grid->n_points;
				idTrans = new int[transSize]();
				if (decimated_grid != NULL) {
					decimated_grid->clear();
					free(decimated_grid);
					decimated_grid = NULL;
				}
				decimated_grid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				decimated_grid->init();
				//统计简化后的点
				decimated_grid->set_nPoints(numPoint);
				for (int i = 0; i < numPoint; i++) {
					int id = deciInfo->pointList[i];
					point*temp = &(un_grid->pointData[id]);
					xyzPoint tempKey;
					tempKey.init(temp->x, temp->y, temp->z);
					if (!overlapFlag)
						decimated_idMap.insert(make_pair(tempKey, i));
					else {
						idTrans[id] = i;
					}
					decimated_grid->addPoint(temp->x, temp->y, temp->z,
						temp->i, temp->j, temp->k, temp->blockId, temp->sectId);
				}
				//统计简化后的cell
				decimated_grid->set_nCells(numCells);
				for (int i = 0; i < numCells; i++) {
					int id1 = deciInfo->cellList[i][0];
					int id2 = deciInfo->cellList[i][1];
					int id3 = deciInfo->cellList[i][2];
					decimated_grid->addVetraCell(id1, id2, id3);
				}
				if (decimated_sectInfo != NULL) {
					decimated_sectInfo->clear();
					delete decimated_sectInfo;
					decimated_sectInfo = NULL;
				}
				//更新sect信息
				decimated_sectInfo = new sectPool;
				decimated_sectInfo->init();
				decimated_sectInfo->setNumofPoints(decimated_grid->n_points);
				decimated_sectInfo->set_sectNum(sectInfo->sectNum);
				for (int i = 0; i < numPoint; i++) {
					point*temp = &(decimated_grid->pointData[i]);
					int id = temp->sectId;
					decimated_sectInfo->pointNum[id]++;
				}
				for (int i = 0; i < decimated_sectInfo->sectNum; i++) {
					decimated_sectInfo->setPointNum(i, decimated_sectInfo->pointNum[i]);
				}
				for (int i = 0; i < numPoint; i++) {
					point*temp = &(decimated_grid->pointData[i]);
					int id = temp->sectId;
					decimated_sectInfo->addPoint(id, i);
				}
				//将每一段的cell信息都隔离开
				for (int i = 0; i < numCells; i++) {
					cell* temp = &(decimated_grid->cellData[i]);
					int id1, id2, id3;
					id1 = temp->idPoint[0];
					id2 = temp->idPoint[1];
					id3 = temp->idPoint[2];
					int sectId1, sectId2, sectId3, sectId4;
					sectId1 = decimated_grid->pointData[id1].sectId;
					sectId2 = decimated_grid->pointData[id2].sectId;
					sectId3 = decimated_grid->pointData[id3].sectId;
					if (sectId1 == sectId2 && sectId2 == sectId3 ) {
						decimated_sectInfo->cellNum[sectId1]++;
					}
				}
				for (int i = 0; i < decimated_sectInfo->sectNum; i++) {
					decimated_sectInfo->setCellNum(i, decimated_sectInfo->cellNum[i]);
				}
				for (int i = 0; i < numCells; i++) {
					cell* temp = &(decimated_grid->cellData[i]);
					int id1, id2, id3;
					id1 = temp->idPoint[0];
					id2 = temp->idPoint[1];
					id3 = temp->idPoint[2];
					int sectId1, sectId2, sectId3, sectId4;
					sectId1 = decimated_grid->pointData[id1].sectId;
					sectId2 = decimated_grid->pointData[id2].sectId;
					sectId3 = decimated_grid->pointData[id3].sectId;
					if (sectId1 == sectId2 && sectId2 == sectId3 ) {
						int realId1, realId2, realId3, realId4;
						realId1 = decimated_sectInfo->sectIdMap[id1];
						realId2 = decimated_sectInfo->sectIdMap[id2];
						realId3 = decimated_sectInfo->sectIdMap[id3];
						decimated_sectInfo->addCell(sectId1, realId1, realId2, realId3);
					}
				}

				deciInfo->clear();
				delete deciInfo;
			}
			void SceneWidget::decimateGrid() {
				deciInfo = new decimatedInfo;
				deciInfo->init();
				int overlapFlag = defaultOverlapFLag;

				deciInfo->setNumOfPoints(un_grid->n_points);
				deciInfo->setNumOfCells(un_grid->n_cells);


				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk - 1; k++) {
						for (int j = 0; j < nj - 1; j++) {
							for (int i = 0; i < ni - 1; i++) {
								// 0:(0,0,-1) 1:(0,0,1)
								// 2:(0,-1,0) 3:(0,1,0)
								// 4:(-1,0,0) 5:(1,0,0)
								int id1 = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
								int id2 = get_id(ni, nj, nk, i + 1, j, k, id, overlapFlag);
								int id3 = get_id(ni, nj, nk, i, j + 1, k, id, overlapFlag);
								int id4 = get_id(ni, nj, nk, i, j, k + 1, id, overlapFlag);
								int id5 = get_id(ni, nj, nk, i + 1, j + 1, k, id, overlapFlag);
								int id6 = get_id(ni, nj, nk, i + 1, j, k + 1, id, overlapFlag);
								int id7 = get_id(ni, nj, nk, i, j + 1, k + 1, id, overlapFlag);
								int id8 = get_id(ni, nj, nk, i + 1, j + 1, k + 1, id, overlapFlag);

								//设置每个点的block


								deciInfo->mark(id1, 5);
								deciInfo->mark(id1, 3);
								deciInfo->mark(id1, 1);

								deciInfo->mark(id2, 4);
								deciInfo->mark(id2, 3);
								deciInfo->mark(id2, 1);

								deciInfo->mark(id3, 5);
								deciInfo->mark(id3, 2);
								deciInfo->mark(id3, 1);

								deciInfo->mark(id4, 5);
								deciInfo->mark(id4, 3);
								deciInfo->mark(id4, 0);

								deciInfo->mark(id5, 4);
								deciInfo->mark(id5, 2);
								deciInfo->mark(id5, 1);

								deciInfo->mark(id6, 4);
								deciInfo->mark(id6, 3);
								deciInfo->mark(id6, 0);

								deciInfo->mark(id7, 5);
								deciInfo->mark(id7, 2);
								deciInfo->mark(id7, 0);

								deciInfo->mark(id8, 4);
								deciInfo->mark(id8, 2);
								deciInfo->mark(id8, 0);
							}
						}
					}
				}
				int numPoint = 0;
				for (int i = 0; i < un_grid->n_points; i++) {
					int count = 0;
					for (int j = 0; j < 6; j++) count += deciInfo->direMark[i][j];
					if (count < 6) {
						//numPoint++;
						deciInfo->pointMark[i] = 1;
					}
				}
				int numCells = 0;

				//new gap cell
				int jGap = 1;
				int kGap = 1;

				//point=42360 cells=127044 删除内部sect，j=2，k=1，没有处理sect连接处
				//for (int id = 0; id < 0+1; id++) {
				//int min_nj, min_nk;
				//min_nj = bk[0].nj;
				//min_nk = bk[0].nk;
				//for (int id = 0; id < n_blocks; id++) {
				//	int ni, nj, nk;
				//	ni = bk[id].ni;
				//	nj = bk[id].nj;
				//	nk = bk[id].nk;
				//	//cout << "ni=" << ni << " nj=" << nj << " nk=" << nk << endl;
				//	if (min_nj > nj) min_nj = nj;
				//	if (min_nk > nk) min_nk = nk;
				//}
				//cout << "min_nj=" << min_nj << " min_nk=" << min_nk << endl;

				//redution越大，粒度越大
				double redution = 0.1;
				/*jGap = 10;
				kGap = 10;*/
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;

					jGap = nj * redution;
					kGap = nk * redution;
					if (jGap < 1) jGap = 1;
					if (kGap < 1) kGap = 1;
					for (int k = 0; k < nk - 1; k += kGap) {

						for (int j = 0; j < nj - 1; j += jGap) {
							for (int i = 0; i < ni - 1; i++) {
								int id1 = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
								int id2 = get_id(ni, nj, nk, i + 1, j, k, id, overlapFlag);
								int id3 = get_id(ni, nj, nk, i, min(j + jGap, nj - 1), k, id, overlapFlag);
								int id4 = get_id(ni, nj, nk, i, j, min(k + kGap, nk - 1), id, overlapFlag);
								int id5 = get_id(ni, nj, nk, i + 1, min(j + jGap, nj - 1), k, id, overlapFlag);
								int id6 = get_id(ni, nj, nk, i + 1, j, min(k + kGap, nk - 1), id, overlapFlag);
								int id7 = get_id(ni, nj, nk, i, min(j + jGap, nj - 1), min(k + kGap, nk - 1), id, overlapFlag);
								int id8 = get_id(ni, nj, nk, i + 1, min(j + jGap, nj - 1), min(k + kGap, nk - 1), id, overlapFlag);

								judgeAndAddCells(id1, id2, id3, id4);
								judgeAndAddCells(id2, id6, id3, id4);
								judgeAndAddCells(id4, id6, id7, id3);
								judgeAndAddCells(id2, id5, id6, id3);
								judgeAndAddCells(id5, id8, id6, id7);
								judgeAndAddCells(id3, id5, id7, id6);

								/*judgeAndAddCells(id1, id2, id3, id7);
								judgeAndAddCells(id1, id3, id4, id7);
								judgeAndAddCells(id1, id2, id5, id7);
								judgeAndAddCells(id2, id5, id6, id7);
								judgeAndAddCells(id3, id5, id7, id8);*/

								

							}
						}
					}

				}





				numPoint = deciInfo->index;
				numCells = deciInfo->cellIndex;


				decimated_idMap.clear();

				if (idTrans != NULL) {
					delete[]idTrans;
					idTrans = NULL;
				}
				transSize = un_grid->n_points;
				idTrans = new int[transSize]();

				if (decimated_grid != NULL) {
					decimated_grid->clear();
					free(decimated_grid);
					decimated_grid = NULL;
				}
				decimated_grid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				decimated_grid->init();
				decimated_grid->set_nPoints(numPoint);
				
				for (int i = 0; i < numPoint; i++) {
					int id = deciInfo->pointList[i];
					point*temp = &(un_grid->pointData[id]);
					xyzPoint tempKey;
					tempKey.init(temp->x, temp->y, temp->z);
					if(!overlapFlag)
						decimated_idMap.insert(make_pair(tempKey, i));
					else {
						idTrans[id] = i;
					}

					decimated_grid->addPoint(temp->x, temp->y, temp->z,
						temp->i, temp->j, temp->k, temp->blockId, temp->sectId);
				}
				decimated_grid->set_nCells(numCells);
				for (int i = 0; i < numCells; i++) {
					int id1 = deciInfo->cellList[i][0];
					int id2 = deciInfo->cellList[i][1];
					int id3 = deciInfo->cellList[i][2];
					int id4 = deciInfo->cellList[i][3];
					decimated_grid->addVetraCell(id1, id2, id3, id4);
				}

				if (decimated_sectInfo != NULL) {
					decimated_sectInfo->clear();
					delete decimated_sectInfo;
					decimated_sectInfo = NULL;
				}
				decimated_sectInfo = new sectPool;
				decimated_sectInfo->init();
				decimated_sectInfo->setNumofPoints(decimated_grid->n_points);
				decimated_sectInfo->set_sectNum(sectInfo->sectNum);
				for (int i = 0; i < numPoint; i++) {
					point*temp = &(decimated_grid->pointData[i]);
					int id = temp->sectId;
					decimated_sectInfo->pointNum[id]++;
				}
				for (int i = 0; i < decimated_sectInfo->sectNum; i++) {
					decimated_sectInfo->setPointNum(i, decimated_sectInfo->pointNum[i]);
				}
				for (int i = 0; i < numPoint; i++) {
					point*temp = &(decimated_grid->pointData[i]);
					int id = temp->sectId;
					decimated_sectInfo->addPoint(id, i);
				}
				//将每一段的cell信息都隔离开

				for (int i = 0; i < numCells; i++) {
					cell* temp = &(decimated_grid->cellData[i]);
					int id1, id2, id3, id4;
					id1 = temp->idPoint[0];
					id2 = temp->idPoint[1];
					id3 = temp->idPoint[2];
					id4 = temp->idPoint[3];
					int sectId1, sectId2, sectId3, sectId4;
					sectId1 = decimated_grid->pointData[id1].sectId;
					sectId2 = decimated_grid->pointData[id2].sectId;
					sectId3 = decimated_grid->pointData[id3].sectId;
					sectId4 = decimated_grid->pointData[id4].sectId;
					if (sectId1 == sectId2 && sectId2 == sectId3 && sectId3 == sectId4) {
						decimated_sectInfo->cellNum[sectId1]++;
					}
				}
				for (int i = 0; i < decimated_sectInfo->sectNum; i++) {
					decimated_sectInfo->setCellNum(i, decimated_sectInfo->cellNum[i]);
				}
				for (int i = 0; i < numCells; i++) {
					cell* temp = &(decimated_grid->cellData[i]);
					int id1, id2, id3, id4;
					id1 = temp->idPoint[0];
					id2 = temp->idPoint[1];
					id3 = temp->idPoint[2];
					id4 = temp->idPoint[3];
					int sectId1, sectId2, sectId3, sectId4;
					sectId1 = decimated_grid->pointData[id1].sectId;
					sectId2 = decimated_grid->pointData[id2].sectId;
					sectId3 = decimated_grid->pointData[id3].sectId;
					sectId4 = decimated_grid->pointData[id4].sectId;
					if (sectId1 == sectId2 && sectId2 == sectId3 && sectId3 == sectId4) {
						int realId1, realId2, realId3, realId4;
						realId1 = decimated_sectInfo->sectIdMap[id1];
						realId2 = decimated_sectInfo->sectIdMap[id2];
						realId3 = decimated_sectInfo->sectIdMap[id3];
						realId4 = decimated_sectInfo->sectIdMap[id4];
						decimated_sectInfo->addCell(sectId1, realId1, realId2, realId3, realId4);
					}
				}

				deciInfo->clear();
				delete deciInfo;
			}//
			void SceneWidget::decimateSect() {
				if (decimated_sectInfo != NULL) {
					decimated_sectInfo->clear();
					free(decimated_sectInfo);
					decimated_sectInfo = NULL;
				}
				decimated_sectInfo = (sectPool*)malloc(sizeof(sectPool));
				decimated_sectInfo->init();
				decimated_sectInfo->set_sectNum(sectInfo->sectNum);
				int numPoint = decimated_grid->n_points;
				for (int i = 0; i < numPoint; i++) {
					point*temp = &(decimated_grid->pointData[i]);
					int id = temp->sectId;
					decimated_sectInfo->pointNum[id]++;
				}
				for (int i = 0; i < decimated_sectInfo->sectNum; i++) {
					decimated_sectInfo->setPointNum(i, decimated_sectInfo->pointNum[i]);
				}
				for (int i = 0; i < numPoint; i++) {
					point*temp = &(decimated_grid->pointData[i]);
					int id = temp->sectId;
					decimated_sectInfo->addPoint(id, i);
				}
				memcpy(decimated_sectInfo->min_x, sectInfo->min_x, sizeof(double)*decimated_sectInfo->sectNum);
				memcpy(decimated_sectInfo->min_y, sectInfo->min_y, sizeof(double)*decimated_sectInfo->sectNum);
				memcpy(decimated_sectInfo->min_z, sectInfo->min_z, sizeof(double)*decimated_sectInfo->sectNum);
				memcpy(decimated_sectInfo->max_x, sectInfo->max_x, sizeof(double)*decimated_sectInfo->sectNum);
				memcpy(decimated_sectInfo->max_y, sectInfo->max_y, sizeof(double)*decimated_sectInfo->sectNum);
				memcpy(decimated_sectInfo->max_z, sectInfo->max_z, sizeof(double)*decimated_sectInfo->sectNum);
			}
			void SceneWidget::buildDecimatedGrid() {
				mouseStyle->gridLineInfo = decimatedLine;
				mouseStyle->un_grid = decimated_grid;
				mouseStyle->sectInfo = decimated_sectInfo;
				mouseStyle->ColorsCurrent = ColorsDecimated;

				mouseStyle->Data = vtk_dataSet;
				mouseStyle->Renderer = m_renderer;
				mouseStyle->actor = vtk_actor;
				mouseStyle->ColorsSect = ColorsSect;
				mouseStyle->mapper = vtk_mapper;
				mouseStyle->ColorsGroup = ColorsGroup;
				mouseStyle->renderWindow = m_pQVTKWidget->GetRenderWindow();

				int n_cells = decimated_grid->n_cells;
				decimatedGrid->Allocate(n_cells);
#ifdef decimateSurface
				for (int i = 0; i < n_cells; i++) {
					int id1 = decimated_grid->cellData[i].idPoint[0];
					int id2 = decimated_grid->cellData[i].idPoint[1];
					int id3 = decimated_grid->cellData[i].idPoint[2];
					vtkIdType idList[3] = { id1,id2,id3 };
					decimatedGrid->InsertNextCell(VTK_TRIANGLE, 3, idList);
				}
#else
				for (int i = 0; i < n_cells; i++) {
					int id1 = decimated_grid->cellData[i].idPoint[0];
					int id2 = decimated_grid->cellData[i].idPoint[1];
					int id3 = decimated_grid->cellData[i].idPoint[2];
					int id4 = decimated_grid->cellData[i].idPoint[3];
					vtkIdType idList[4] = { id1,id2,id3,id4 };
					decimatedGrid->InsertNextCell(VTK_TETRA, 4, idList);
				}
#endif // decimateSurface
				
				

				int n_points = decimated_grid->n_points;
				for (int i = 0; i < n_points; i++) {
					double x = decimated_grid->pointData[i].x;
					double y = decimated_grid->pointData[i].y;
					double z = decimated_grid->pointData[i].z;
					decimatedPoints->InsertNextPoint(x, y, z);
				}
				decimatedGrid->SetPoints(decimatedPoints);
				qDebug() << "decimate grid pointsNum=" << n_points << " cellNum=" << n_cells;
				ColorsDecimated->SetNumberOfComponents(3);
				ColorsDecimated->SetName("ColorsDecimated");
				unsigned char mySilver[3] = { 192,192,192 };
				unsigned char myRed[3] = { 255, 0, 0 };
				for (int i = 0; i < n_points; i++) {
					ColorsDecimated->InsertNextTupleValue(myRed);
				}

				decimatedGrid->GetPointData()->SetScalars(ColorsDecimated);
				m_pQVTKWidget->update();
			}
			void SceneWidget::get_lineInfo() {
				
				if (gridLine != NULL) {
					gridLine->clear();
					free(gridLine);
					gridLine = NULL;
				}
				int overlapFlag = defaultOverlapFLag;
				//qDebug() << "begin gridLine->init()";
				gridLine = (lineInfo*)malloc(sizeof(lineInfo));
				gridLine->init();
				int lineNum = 0;
				int *pointMark = NULL;
				
				pointMark = new int[un_grid->n_points]{ 0 };
				//qDebug() << "un_grid->n_points=" << un_grid->n_points;
				//qDebug() << "bk->numOfPoints=" << bk->numOfPoints;
				//qDebug() << "begin for1";
				for (int i = 0; i < n_blocks; i++) {
					//qDebug() << "i=" << i << " offset=" << bk[i].sectOffset << "ni="<< bk[i].ni<<" nj="<< bk[i].nj<<" nk="<< bk[i].nk<<endl;
					if (bk[i].ni + bk[i].sectOffset > lineNum) lineNum = bk[i].ni + bk[i].sectOffset;
					//qDebug() << " i=" << i << " idOffset=" << idOffset[i];
				}
				//qDebug() << "begin set_lineNum";
				gridLine->set_lineNum(lineNum);
				//qDebug() << "begin for2";
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								int value = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
								int lineId = bk[id].sectOffset + i;
								if (pointMark[value] == 0) {
									pointMark[value] = 1;
									gridLine->pointNum[lineId]++;
								}

							}
						}
					}
				}
				//qDebug() << "begin for3";
				for (int i = 0; i < lineNum; i++) {
					gridLine->setPointNum(i, gridLine->pointNum[i]);
				}
				//qDebug() << "begin for4";
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int i = 0; i < ni; i++) {
						int sectId = bk[id].sectId[i];
						int lineId = bk[id].sectOffset + i;
						if (sectId > gridLine->sectId[lineId]) gridLine->sectId[lineId] = sectId;
					}
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;

								int value = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
								int lineId = bk[id].sectOffset + i;
								if (pointMark[value] == 1) {
									pointMark[value] = 2;
									gridLine->addPoint(lineId, value);
								}
							}
						}
					}
				}
				int n_points = 0;
				for (int i = 0; i < lineNum; i++) {
					n_points += gridLine->pointNum[i];
				}
				qDebug() << "n_points=" << n_points;
				delete[]pointMark;
			}//
			void SceneWidget::get_decimatedLineInfo() {//
				if (decimatedLine != NULL) {
					decimatedLine->clear();
					free(decimatedLine);
					decimatedLine = NULL;
				}
				decimatedLine = (lineInfo*)malloc(sizeof(lineInfo));
				decimatedLine->init();
				int lineNum = 0;
				int *pointMark = NULL;
				pointMark = new int[decimated_grid->n_points]{ 0 };
				for (int i = 0; i < n_blocks; i++) {
					if (bk[i].ni + bk[i].sectOffset > lineNum) lineNum = bk[i].ni + bk[i].sectOffset;
				}
				int overlapFlag = defaultOverlapFLag;
				decimatedLine->set_lineNum(lineNum);

				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								int value = get_decimateId(ni, nj, nk, i, j, k, id, overlapFlag);
								int lineId = bk[id].sectOffset + i;
								if (pointMark[value] == 0) {
									pointMark[value] = 1;
									decimatedLine->pointNum[lineId]++;
								}

								//xyzPoint temp;
								//temp.init(x, y, z);
								//auto it = decimated_idMap.find(temp);
								//if (it == decimated_idMap.end()) {

								//}
								//else {
								//	//find success
								//	int value = it->second;
								//	int lineId = bk[id].sectOffset + i;
								//	if (pointMark[value] == 0) {
								//		pointMark[value] = 1;
								//		decimatedLine->pointNum[lineId]++;
								//	}
								//}
							}
						}
					}
				}
				for (int i = 0; i < lineNum; i++) {
					decimatedLine->setPointNum(i, decimatedLine->pointNum[i]);
				}
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int i = 0; i < ni; i++) {
						int sectId = bk[id].sectId[i];
						int lineId = bk[id].sectOffset + i;
						if (sectId > decimatedLine->sectId[lineId]) decimatedLine->sectId[lineId] = sectId;
					}
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;

								int value = get_decimateId(ni, nj, nk, i, j, k, id, overlapFlag);
								int lineId = bk[id].sectOffset + i;
								if (pointMark[value] == 1) {
									pointMark[value] = 2;
									decimatedLine->addPoint(lineId, value);
								}

								//xyzPoint temp;
								//temp.init(x, y, z);
								//auto it = decimated_idMap.find(temp);
								//if (it == decimated_idMap.end()) {

								//}
								//else {
								//	//find success
								//	int value = it->second;
								//	int lineId = bk[id].sectOffset + i;
								//	if (pointMark[value] == 1) {
								//		pointMark[value] = 2;
								//		decimatedLine->addPoint(lineId, value);
								//	}
								//}
							}
						}
					}
				}
				int n_points = 0;
				for (int i = 0; i < lineNum; i++) {
					n_points += decimatedLine->pointNum[i];
				}

				delete[]pointMark;
			}


			void SceneWidget::colorLine() {

				if (lineId != 0) return;
				emit SignalInstance::getInstance()->signalSceneWorked(true);
				if (timer != NULL) {
					timer->stop();
					delete timer;
					timer = NULL;
				}
				if (lastColor != NULL) {
					delete[]lastColor;
					lastColor = NULL;
				}
				//决定是覆盖型还是跳跃型
				if (decimatedFlag == 0) {
					if (un_grid == nullptr)return;
					//emit SignalInstance::getInstance()->signalPrintInfo("网格开始按行染色");
					lastColor = new unsigned char[un_grid->n_points][3];
					for (int i = 0; i < un_grid->n_points; i++) {
						unsigned char temp[3];
						ColorsCurrent->GetTypedTuple(i, temp);
						memcpy(lastColor[i], temp, sizeof(unsigned char) * 3);
					}
				}
				else if (decimatedFlag == 1) {
					//emit SignalInstance::getInstance()->signalPrintInfo("简化网格开始按行染色");
					lastColor = new unsigned char[decimated_grid->n_points][3];
					for (int i = 0; i < decimated_grid->n_points; i++) {
						unsigned char temp[3];
						ColorsDecimated->GetTypedTuple(i, temp);
						memcpy(lastColor[i], temp, sizeof(unsigned char) * 3);
					}
				}

				timer = new QTimer(this);
				lineGap = 3;
				currentGapModel = gapModel;
				QObject::connect(timer, SIGNAL(timeout()), this, SLOT(colorLineTimer()));
				timer->start(gapTime);


			}
			void SceneWidget::colorLineTimer() {
				colorLineMutex.lock();
				unsigned char mySilver[3] = { 192,192,192 };
				unsigned char myWhite[3] = { 255, 255, 255 };
				unsigned char(*colorTemp)[3];
				colorTemp = new unsigned char[lineGap][3];
				if (decimatedFlag == 0) {
					//还原上一段的颜色
					if (currentGapModel == 0) {
						for (int i = max(lineId - lineGap, 0); i < lineId && i < gridLine->lineNum; i++) {
							int sectId = gridLine->sectId[i];
							for (int j = 0; j < gridLine->pointNum[i]; j++) {
								int id = gridLine->pointList[i][j];
								ColorsCurrent->SetTypedTuple(id, lastColor[id]);
							}
						}
					}
					//染色
					for (int i = lineId; i < lineId + lineGap && i < gridLine->lineNum; i++) {
						int sectId = gridLine->sectId[i];
						for (int j = 0; j < gridLine->pointNum[i]; j++) {
							ColorsCurrent->SetTypedTuple(gridLine->pointList[i][j], myWhite);
						
						}
					}
					ugrid->GetPointData()->SetActiveScalars("ColorsCurrent");
					ugrid->GetPointData()->Update();
					m_pQVTKWidget->update();
					lineId += lineGap;
					//if (lineId >= gridLine->lineNum) lineId = gridLine->lineNum;
					if (lineId - lineGap >= gridLine->lineNum) {
						timer->stop();
						delete timer;
						timer = NULL;
						//emit SignalInstance::getInstance()->signalPrintInfo("染色完成");

						lineId = 0;

						if (lastColor != NULL) {
							delete[]lastColor;
							lastColor = NULL;
						}
						emit SignalInstance::getInstance()->signalSceneWorked(false);
					}
				}
				else if (decimatedFlag == 1) {
					/// @brief 获取最后一行的数据并提取出lineId
					auto readLineId = getBodyLineId();
					qDebug() << "readLineId==" << readLineId << " lineId=" << lineId;
					static int cnt = 0;

					if (readLineId == -1) {
						if (cnt < 3) {
							cnt++;
						}
						else lineGap = 1e5;
					}
					else {
						cnt = 0;
					}
					//if ((readLineId == -1) || (readLineId == beforeLineId))
					if (readLineId != -1 && readLineId <= beforeLineId)
					{
						colorLineMutex.unlock();
						return;
					}
					
					//将上次进行染色的颜色进行还原
					//beforeLineId为上一次染色的最后一行‘
					//lineId为当前染色Id
					if (currentGapModel == 0) {
						/*for (int i = max(0, lineId - lineGap); i < lineId && i < decimatedLine->lineNum; i++) {
							int sectId = decimatedLine->sectId[i];

							for (int j = 0; j < decimatedLine->pointNum[i]; j++) {
								int id = decimatedLine->poi  ntList[i][j];
								ColorsDecimated->SetTypedTuple(id, lastColor[id]);

							}
						}*/
						for (int i = 0; i < beforeLineId && i < decimatedLine->lineNum; i++) {
							int sectId = decimatedLine->sectId[i];

							for (int j = 0; j < decimatedLine->pointNum[i]; j++) {
								int id = decimatedLine->pointList[i][j];
								ColorsDecimated->SetTypedTuple(id, lastColor[id]);

							}
						}
					}


					lineId = readLineId==-1?lineId+lineGap:readLineId;
					//染色
					/*for (int i = lineId; i < lineId + lineGap && i < decimatedLine->lineNum; i++) {
						int sectId = decimatedLine->sectId[i];

						for (int j = 0; j < decimatedLine->pointNum[i]; j++) {
							ColorsDecimated->SetTypedTuple(decimatedLine->pointList[i][j], myWhite);
						}
					}*/
					for (int i = beforeLineId; i < lineId && i < decimatedLine->lineNum; i++) {
						int sectId = decimatedLine->sectId[i];

						for (int j = 0; j < decimatedLine->pointNum[i]; j++) {
							ColorsDecimated->SetTypedTuple(decimatedLine->pointList[i][j], myWhite);
						}
					}
					
					decimatedGrid->GetPointData()->SetActiveScalars("ColorsDecimated");
					decimatedGrid->GetPointData()->Update();
					m_pQVTKWidget->update();
					
					if (beforeLineId >= decimatedLine->lineNum) {
						timer->stop();
						delete timer;
						timer = NULL;
						//emit SignalInstance::getInstance()->signalPrintInfo("染色完成");

						lineId = 0;
						beforeLineId = 0;
						if (lastColor != NULL) {
							delete[]lastColor;
							lastColor = NULL;
						}
						emit SignalInstance::getInstance()->signalSceneWorked(false);
					}
					beforeLineId = lineId;
				}


				delete[]colorTemp;
				colorLineMutex.unlock();
			}
			void SceneWidget::setSectColor(int r, int g, int b, int sectId) {


				unsigned char sectColor[3] = { r,g,b };
				if (decimatedFlag == 0) {
					for (int i = 0; i < sectInfo->pointNum[sectId]; i++) {
						ColorsCurrent->SetTypedTuple(sectInfo->idInfo[sectId][i], sectColor);
					}
					ugrid->GetPointData()->SetActiveScalars("ColorsCurrent");
					m_pQVTKWidget->update();
				}
				else if (decimatedFlag == 1) {
					for (int i = 0; i < decimated_sectInfo->pointNum[sectId]; i++) {
						ColorsDecimated->SetTypedTuple(decimated_sectInfo->idInfo[sectId][i], sectColor);
					}
					decimatedGrid->GetPointData()->SetActiveScalars("ColorsDecimated");
					m_pQVTKWidget->update();
				}	
			}
			void SceneWidget::onChangeOperateStatus()
			{
				mouseStyle->m_statusOperate = !mouseStyle->m_statusOperate;
				changeGapModel();
			}
			void SceneWidget::onChangeMlStatus()
			{
				mouseStyle->m_statusIsCanML = !mouseStyle->m_statusIsCanML;
			}
			void SceneWidget::onReset()
			{
				m_mainTainOrBrowseButton->reSetButton();
				m_coverOrdumpButton->reSetButton();
				gapModel = 0;
				mouseStyle->m_statusIsCanML = false;

				/// @brief 切割框去除
				if (mouseStyle->windowClipFlag != 0)
				{
					mouseStyle->windowClipFlag = 0;
					emit SignalInstance::getInstance()->signalCancelShowClip();
				}
			}
			void SceneWidget::onClipGrid()
			{
				
				emit SignalInstance::getInstance()->signalPrintInfo(u8"按a键显示切割窗口\n按s键取消切割窗口\n按d键进行普通切割\n按F键对切割后两部分同时显示");
				if (!mouseStyle->getClipFlag()) {
					emit SignalInstance::getInstance()->signalShowClip();
					mouseStyle->clipFlagChanged(1);
				}
				/*showClip();
				mouseStyle->clipFlagChanged(1);*/
			}
			void SceneWidget::ColorStop() {
				if (timer == NULL || !timer->isActive()) {
					emit SignalInstance::getInstance()->signalColorMutex();
					return;
				}
				colorLineMutex.lock();
				timer->stop();

				delete timer;
				timer = NULL;

				lineId = 0;
				beforeLineId = 0;
				if (lastColor != NULL) {
					delete[]lastColor;
					lastColor = NULL;
				}
				emit SignalInstance::getInstance()->signalSceneWorked(false);
				colorLineMutex.unlock();
				emit SignalInstance::getInstance()->signalColorMutex();
				
			}
			void SceneWidget::onColorLineStop()
			{
				if (timer == NULL || !timer->isActive()) {
					QMessageBox::information(this, u8"提示", u8"未开始染色");
					return;
				}
				colorLineMutex.lock();
				timer->stop();
				/*for (int i = 0; i < decimatedLine->lineNum; i++) {
					int sectId = decimatedLine->sectId[i];
					for (int j = 0; j < decimatedLine->pointNum[i]; j++) {
						int id = decimatedLine->pointList[i][j];
						ColorsDecimated->SetTypedTuple(id, lastColor[id]);

					}
				}
				decimatedGrid->GetPointData()->SetActiveScalars("ColorsDecimated");
				decimatedGrid->GetPointData()->Update();
				m_pQVTKWidget->update();*/
				
				delete timer;
				timer = NULL;
				
				lineId = 0;
				beforeLineId = 0;
				if (lastColor != NULL) {
					delete[]lastColor;
					lastColor = NULL;
				}
				//emit SignalInstance::getInstance()->signalPrintInfo("暂停染色成功");
				emit SignalInstance::getInstance()->signalSceneWorked(false);
				colorLineMutex.unlock();
			}
			void SceneWidget::onClipSplit() {
				emit SignalInstance::getInstance()->signalBothClip();
			}
			void SceneWidget::get_idOffset(int n_blocks, block * bk)
			{
				//qDebug() << "idOffset=" << idOffset << endl; 
				idOffset = (int*)malloc(n_blocks * sizeof(int));
				memset(idOffset, 0, sizeof(int)*n_blocks);
				//memset(idOffset, 0, sizeof(idOffset));
				idOffset[0] = 0;
				for (int i = 1; i < n_blocks; i++) {
					/*int nj = bk[i].nj, nk = bk[i].nk;
					int jGap = max(1.0, nj*redution),kGap= max(1.0, nk*redution);
					int kIterations = (nk - 1) / kGap + 1;
					int jIterations = (nj - 1) / jGap + 1;
					int totalIterations = kIterations * jIterations;
					idOffset[i] = idOffset[i - 1] + totalIterations;*/

					idOffset[i] = idOffset[i - 1] + bk[i - 1].numOfPoints;
					//qDebug() << "i=" << i << " idOffset=" << idOffset[i];
				}
			}
			void SceneWidget::get_coordinateOffset(int n_blocks, block * bk)
			{
				coordinateOffset = (double*)malloc(n_blocks * sizeof(double));
				memset(coordinateOffset, 0, sizeof(coordinateOffset));
				coordinateOffset[0] = 0;
				for (int i = 1; i < n_blocks; i++) {
					coordinateOffset[i] = coordinateOffset[i - 1];
					double x1 = bk[i - 1].max_x;
					double x2 = bk[i - 1].min_x;
					if (x1 > 0)coordinateOffset[i] += x1;
					if (x2 < 0) coordinateOffset[i] -= x2;
					coordinateOffset[i] += 5;
				}
			}
			int SceneWidget::get_id(int ni, int nj, int nk, int i, int j, int k, int blockIndex, int overlapFlag)
			{
				if (overlapFlag == 1) return(idOffset[blockIndex] + i + j * ni + k * ni*nj);
				double *X = bk[blockIndex].X(i, j, k), *Y = bk[blockIndex].Y(i, j, k), *Z = bk[blockIndex].Z(i, j, k);
				double x = *X, y = *Y, z = *Z;
				xyzPoint temp;
				temp.init(x, y, z);
				auto it = idMap.find(temp);
				if (it == idMap.end()) {
					//cout << "----------------------get id error!" << endl;
					//cout << "blockId=" << blockIndex << " i=" << i << " j=" << j << " k=" << k << endl;
					return 0;
				}
				return it->second;
			}
			int SceneWidget::get_decimateId(int ni, int nj, int nk, int i, int j, int k, int blockIndex, int overlapFlag)
			{
				if (overlapFlag == 1) {
					int id = get_id( ni,  nj,  nk,  i,  j, k,blockIndex,overlapFlag);
					return idTrans[id];
				}
				//if (overlapFlag == 1) return(idOffset[blockIndex] + i + j * ni + k * ni*nj);
				double *X = bk[blockIndex].X(i, j, k), *Y = bk[blockIndex].Y(i, j, k), *Z = bk[blockIndex].Z(i, j, k);
				double x = *X, y = *Y, z = *Z;
				xyzPoint temp;
				temp.init(x, y, z);
				auto it = decimated_idMap.find(temp);
				if (it == decimated_idMap.end()) {
					//cout << "----------------------get id error!" << endl;
					//cout << "blockId=" << blockIndex << " i=" << i << " j=" << j << " k=" << k << endl;
					return 0;
				}
				return it->second;
			}
			int SceneWidget::get_sect(int id, UNSTRUCTURED_GRID * un_grid)
			{
				if (id >= un_grid->n_points || id < 0) {
					//cout << "error! cant find point!" << endl;
					return -1;
				}
				return un_grid->pointData[id].sectId;
			}
			void SceneWidget::get_totalBlockInfo(block * bk, int n_blocks)
			{
				set<double> s;
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								s.insert(x);
							}
						}
					}
				}
			}
			void SceneWidget::setFullGridReadFlag(bool flag)
			{
				m_fullGridReadFlag = flag;
			}
			int SceneWidget::getBodyLineId()
			{
				QString strLine;
				/// @brief 获取最后一行的计算数据，结果保存到strLine中
				QString path = SignalInstance::getInstance()->getWorkSpace() + "/body/history.dat";
				BodyPathChecker tempChecker;
				path = tempChecker.checkAndGetPath(path);
				int ret = FIlEINSTANCE->readFileEndLines(path, 2, strLine);

				/// @brief 使用正则表达式提取Id
				if (ret == 0)		///< 成功进行提取
				{
					qDebug() << "strLine=" << strLine;
					/// @brief 将特殊符号进行去除
					strLine.replace("[", "");
					strLine.replace("]", "");
					strLine.replace(QRegExp("[\\s]+"), "");
					regex pattern("(Nsc\:Ip\:Nst)=(.*?)X.*");
					std::smatch matchRes;
					std::string strId = FIlEINSTANCE->regexMatch(strLine.toLocal8Bit().toStdString(), pattern, matchRes);
					if (matchRes.length() < 3) {
						return -1;
					}
					else 
					{
					/*	for (auto it = matchRes.begin(); it != matchRes.end(); ++it)
							qDebug() << QString::fromStdString(*it);*/
							//std::cout << (*it) << std::endl;
						qDebug() << QString::fromStdString(strId);
						QStringList list = QString::fromStdString(strId).split(":");
						QString id = list[1];
						return id.toInt();
					}
				}
			}
			bool SceneWidget::LoadPlot3DFile(QString filename)
			{
				QString pltFile = filename;
				QFile file(pltFile);
				file.open(QIODevice::ReadOnly);

				// Return on Cancel
				if (!file.exists()) {
					QMessageBox::information(this, "提示", "文件不存在!!");
					return false;
				}
				//清除已经有的图片和初始化背景
				this->removeDataSet();
				if (plt_dataSet) {
					plt_dataSet = nullptr;
				}
				plt_dataSet = vtkSmartPointer<vtkUnstructuredGrid>::New();
				//plt_dataSet = vtkSmartPointer<vtkPolyData>::New();
				plt_dataSet = vtkPltReader(pltFile.toLocal8Bit().toStdString().c_str());
				addDataSet(plt_dataSet);
				//boolisLoaded = false;
				QString attr_string = "Cp";
				attributeSelSlot(attr_string);
				return true;
			}
			//pathFlag=0:新建时传的路径 pathFlag=1:打开时传的路径
			void SceneWidget::slotLoadGridData(QString path, int pathFlag,int renderFlag) {
				//emit SignalInstance::getInstance()->signalPrintInfo(u8"开始解析网格");
				auto start_t = std::chrono::high_resolution_clock::now();
				this->removeDataSet();

				uint64_t start = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

				clearAll();
				m_pQVTKWidget->update();
				unsigned char myRed[3] = { 255, 0, 0 };
				unsigned char myGreen[3] = { 0, 255, 0 };
				unsigned char myBlue[3] = { 0, 0, 255 };
				unsigned char myYellow[3] = { 255,255,0 };
				unsigned char myBlack[3] = { 0,0,0 };
				unsigned char mySilver[3] = { 192,192,192 };
				unsigned char myPurple[3] = { 128,0,128 };
				unsigned char myBrown[3] = { 165,42,42 };

				int colorFlag = 0;
				int representId = 0;
				auto judgeGrid = [&](QString str)->bool {
					QFile file(str);
					if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
						qDebug() << "Cannot open file for reading:" << file.errorString();
						return false;
					}
					QTextStream in(&file);
					int lineNumber = 0;
					QString line = in.readLine();
					lineNumber = line.toInt();
					int cnt = 0;
					while (!in.atEnd()) {
						QString line = in.readLine();
						QStringList dataList = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);

						// 统计数据数量
						int dataCount = dataList.size();
						if (dataCount != 3) break;
						cnt++;
					}
					file.close();
					if (cnt == lineNumber) return true;
					return false;
				};
				bool readFull = judgeGrid(path);
				// 如果是整体网格使用grid读取文件
				//if (m_fullGridReadFlag == true)
				if(readFull==true)
					read_grid(QString(path).toLocal8Bit().toStdString());
				else 
					read_dat(QString(path).toLocal8Bit().toStdString());


				get_idOffset(n_blocks, bk);

				get_coordinateOffset(n_blocks, bk);


				transform(bk, n_blocks, un_grid, 0);


				sort_to_getI();

				if (!renderFlag) return;
				mouseStyle->gridLineInfo = gridLine;
				mouseStyle->Data = vtk_dataSet;
				mouseStyle->sectInfo = sectInfo;
				mouseStyle->Renderer = m_renderer;
				mouseStyle->actor = vtk_actor;
				mouseStyle->ColorsSect = ColorsSect;
				mouseStyle->mapper = vtk_mapper;
				mouseStyle->ColorsGroup = ColorsGroup;
				mouseStyle->ColorsCurrent = ColorsCurrent;
				mouseStyle->un_grid = un_grid;
				mouseStyle->renderWindow = m_pQVTKWidget->GetRenderWindow();

				//un_grid = decimated_grid;

				int n_cells = un_grid->n_cells;
				ugrid->Allocate(n_cells);

				for (int i = 0; i < n_cells; i++) {
					int id1 = un_grid->cellData[i].idPoint[0];
					int id2 = un_grid->cellData[i].idPoint[1];
					int id3 = un_grid->cellData[i].idPoint[2];
					int id4 = un_grid->cellData[i].idPoint[3];
					vtkIdType idList[4] = { id1,id2,id3,id4 };
					ugrid->InsertNextCell(VTK_TETRA, 4, idList);
				}

				int n_points = un_grid->n_points;
				//points->Allocate(n_points);
				for (int i = 0; i < n_points; i++) {
					double x = un_grid->pointData[i].x;
					double y = un_grid->pointData[i].y;
					double z = un_grid->pointData[i].z;
					points->InsertNextPoint(x, y, z);
				}
				ugrid->SetPoints(points);

				ColorsGroup->SetNumberOfComponents(3);
				ColorsGroup->SetName("ColorsGroup");
				for (int i = 0; i < n_points; i++) {
					int id;
					id = un_grid->pointData[i].blockId;
					ColorsGroup->InsertNextTupleValue(myColor->colorTable[id]);
				}

				ColorsSect->SetNumberOfComponents(3);
				ColorsSect->SetName("ColorsSect");
				for (int i = 0; i < n_points; i++) {
					int id;
					id = un_grid->pointData[i].sectId;

					ColorsSect->InsertNextTupleValue(myColor->colorTable[id]);
				}

				ColorsCurrent->SetNumberOfComponents(3);
				ColorsCurrent->SetName("ColorsCurrent");
				for (int i = 0; i < n_points; i++) {
					int id;
					id = un_grid->pointData[i].blockId;

					ColorsCurrent->InsertNextTupleValue(myColor->colorTable[id]);
				}

				if (colorFlag == 1) {
					//ugrid->GetPointData()->SetScalars(ColorsGroup);
					ColorsCurrent->DeepCopy(ColorsGroup);
				}
				else if (colorFlag == 2) {
					//ugrid->GetPointData()->SetScalars(ColorsSect);
					ColorsCurrent->DeepCopy(ColorsSect);
				}

				ugrid->GetPointData()->SetScalars(ColorsCurrent);

				vtk_dataSet = ugrid;
				vtk_mapper->SetInputData(vtk_dataSet);
				vtk_actor->SetMapper(vtk_mapper);
				m_renderer->AddActor(vtk_actor);



				mouseStyle->Data = vtk_dataSet;
				mouseStyle->actor = vtk_actor;
				mouseStyle->mapper = vtk_mapper;


				m_pQVTKWidget->update();
				uint64_t end = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();
				ViewDirection(m_renderer, 0, 1, 0, 0, 0, 1);
				m_pQVTKWidget->update();
				


				//emit SignalInstance::getInstance()->signalPrintInfo(u8"网格渲染完成");
				mouseStyle->canCliped = true;

			}
			void SceneWidget::showMessage(QString mess) {

				QMessageBox::information(this, "提示", mess);
			}
			void SceneWidget::setBatchMode() {
				m_btnClip->setParent(nullptr);
				m_btnClipSplit->setParent(nullptr);
				m_clipWindowVisible->setParent(nullptr);
				m_clipWindowInvisible->setParent(nullptr);
				m_clipToolBtn->setParent(nullptr);
			}
			bool SceneWidget::LoadVTKFile()
			{
				////清除已经有的图片和初始化背景
				this->removeDataSet();

				unsigned char myRed[3] = { 255, 0, 0 };
				unsigned char myGreen[3] = { 0, 255, 0 };
				unsigned char myBlue[3] = { 0, 0, 255 };
				unsigned char myYellow[3] = { 255,255,0 };
				unsigned char myBlack[3] = { 0,0,0 };
				unsigned char mySilver[3] = { 192,192,192 };
				unsigned char myPurple[3] = { 128,0,128 };
				unsigned char myBrown[3] = { 165,42,42 };

				//this->removeDataSet();
				int colorFlag = 0;
				int representId = 0;
				


				preMapper->SetInputData(preGrid);
				preActor->SetMapper(preMapper);


				//vtk_dataSet->ShallowCopy(preGrid);
				vtk_dataSet = preGrid;

				vtk_mapper->SetInputData(vtk_dataSet);
				vtk_actor->SetMapper(vtk_mapper);

				/*vtk_mapper->ShallowCopy(preMapper);
				vtk_actor->ShallowCopy(preActor);*/

				//m_renderer->AddActor(vtk_actor);
				m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);
				m_pQVTKWidget->GetInteractor()->SetRenderWindow(m_pQVTKWidget->GetRenderWindow());

				mouseStyle->gridLineInfo = gridLine;
				mouseStyle->Data = vtk_dataSet;
				mouseStyle->sectInfo = sectInfo;
				mouseStyle->Renderer = m_renderer;
				mouseStyle->actor = vtk_actor;
				mouseStyle->ColorsSect = ColorsSect;
				mouseStyle->mapper = vtk_mapper;
				mouseStyle->ColorsGroup = ColorsGroup;
				mouseStyle->ColorsCurrent = ColorsCurrent;
				mouseStyle->un_grid = un_grid;
				mouseStyle->renderWindow = m_pQVTKWidget->GetRenderWindow();
				mouseStyle->windowClipFlag = 0;
				m_pQVTKWidget->GetInteractor()->SetInteractorStyle(mouseStyle);

				//keyStyle->windowFlag = 0;
				//m_pQVTKWidget->GetInteractor()->SetInteractorStyle(keyStyle);


				mouseStyle->SetDefaultRenderer(m_renderer);

				m_pQVTKWidget->update();
				//m_pQVTKWidget->update();
				//slotLoadSectData(appComDir + "/SSPNS/ui/ui/body/");

				//showVTKData();
				return true;


			}

			void SceneWidget::showVTKData() {
				vtk_mapper->SetInputData(vtk_dataSet);
				vtk_actor->SetMapper(vtk_mapper);
				m_renderer->AddActor(vtk_actor);
				m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);
				m_pQVTKWidget->update();
			}

			void SceneWidget::zoomToExtent()
			{
				// Zoom to extent of last added actor
				vtkSmartPointer<vtkActor> actor = m_renderer->GetActors()->GetLastActor();
				if (actor != nullptr) {
					m_renderer->ResetCamera(actor->GetBounds());
				}

				//renderWindow()->Render();
				m_pQVTKWidget->update();
			}

			void SceneWidget::initActions() {
				//设置视图朝向X轴正向
				const QIcon setViewDirectionToXIcon = QIcon::fromTheme(tr("设置视图朝向X轴正向"), QIcon(":/binForPublic/images/setViewDirectionToX.png"));
				m_setViewDirectionToXAct = new QAction(setViewDirectionToXIcon, tr("设置视图朝向X轴正向(&X)"), this);
				m_setViewDirectionToXAct->setStatusTip(tr("设置视图朝向X轴正向"));
				connect(m_setViewDirectionToXAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToX);

				//设置视图朝向X轴负向
				const QIcon setViewDirectionToNegativeXIcon = QIcon::fromTheme(tr("设置视图朝向X轴负向"), QIcon(":/binForPublic/images/setViewDirectionToNegativeX.png"));
				m_setViewDirectionToNegativeXAct = new QAction(setViewDirectionToNegativeXIcon, tr("设置视图朝向X轴负向(&A)"), this);
				m_setViewDirectionToNegativeXAct->setStatusTip(tr("设置视图朝向X轴负向"));
				connect(m_setViewDirectionToNegativeXAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToNegativeX);

				//设置视图朝向Y轴正向
				const QIcon setViewDirectionToYIcon = QIcon::fromTheme(tr("设置视图朝向Y轴正向"), QIcon(":/binForPublic/images/setViewDirectionToY.png"));
				m_setViewDirectionToYAct = new QAction(setViewDirectionToYIcon, tr("设置视图朝向Y轴正向(&Y)"), this);
				m_setViewDirectionToYAct->setStatusTip(tr("设置视图朝向Y轴正向"));
				connect(m_setViewDirectionToYAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToY);

				//设置视图朝向Y轴负向
				const QIcon setViewDirectionToNegativeYIcon = QIcon::fromTheme(tr("设置视图朝向Y轴负向"), QIcon(":/binForPublic/images/setViewDirectionToNegativeY.png"));
				m_setViewDirectionToNegativeYAct = new QAction(setViewDirectionToNegativeYIcon, tr("设置视图朝向Y轴负向(&B)"), this);
				m_setViewDirectionToNegativeYAct->setStatusTip(tr("设置视图朝向Y轴负向"));
				connect(m_setViewDirectionToNegativeYAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToNegativeY);

				//设置视图朝向Z轴正向
				const QIcon setViewDirectionToZIcon = QIcon::fromTheme(tr("设置视图朝向Z轴正向"), QIcon(":/binForPublic/images/setViewDirectionToZ.png"));
				m_setViewDirectionToZAct = new QAction(setViewDirectionToZIcon, tr("设置视图朝向Z轴正向(&Z)"), this);
				m_setViewDirectionToZAct->setStatusTip(tr("设置视图朝向Z轴正向"));
				connect(m_setViewDirectionToZAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToZ);

				//设置视图朝向Z轴负向
				const QIcon setViewDirectionToNegativeZIcon = QIcon::fromTheme(tr("设置视图朝向Z轴负向"), QIcon(":/binForPublic/images/setViewDirectionToNegativeZ.png"));
				m_setViewDirectionToNegativeZAct = new QAction(setViewDirectionToNegativeZIcon, tr("设置视图朝向Z轴负向(&C)"), this);
				m_setViewDirectionToNegativeZAct->setStatusTip(tr("设置视图朝向Z轴负向"));
				connect(m_setViewDirectionToNegativeZAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToNegativeZ);

				//放大
				const QIcon amplificationViewIcon = QIcon::fromTheme(tr("放大"), QIcon(":/binForPublic/images/zoomin.png"));
				m_amplificationViewAct = new QAction(amplificationViewIcon, tr("放大(&Z)"), this);
				m_amplificationViewAct->setStatusTip(tr("放大"));
				connect(m_amplificationViewAct, &QAction::triggered, this, &SceneWidget::amplificationView);

				//缩小
				const QIcon narrowViewIcon = QIcon::fromTheme(tr("缩小"), QIcon(":/binForPublic/images/zoomout.png"));
				m_narrowViewAct = new QAction(narrowViewIcon, tr("缩小(&Z)"), this);
				m_narrowViewAct->setStatusTip(tr("缩小"));
				connect(m_narrowViewAct, &QAction::triggered, this, &SceneWidget::narrowView);
			}


			void SceneWidget::setViewDirectionToX() {
				//vtkCamera* curCamera = m_renderer->GetActiveCamera();
				//curCamera->SetViewUp(0, 1, 0);
				//curCamera->SetPosition(1, 0, 0);
				//curCamera->SetFocalPoint(0, 0, 0);
				//m_renderer->SetActiveCamera(curCamera);
				ViewDirection(m_renderer, -1, 0, 0, 0, 0, 1);
				// resetCamera(true);
				m_pQVTKWidget->update();
			}

			void SceneWidget::setViewDirectionToNegativeX() {
				//vtkCamera* curCamera = m_renderer->GetActiveCamera();
				//curCamera->SetViewUp(0, -1, 0);
				//curCamera->SetPosition(1, 0, 0);
				//curCamera->SetFocalPoint(0, 0, 0);
				//m_renderer->SetActiveCamera(curCamera);
				ViewDirection(m_renderer, 1, 0, 0, 0, 0, 1);
				m_pQVTKWidget->update();
			}

			void SceneWidget::setViewDirectionToY() {
				//vtkCamera* curCamera = m_renderer->GetActiveCamera();
				//curCamera->SetViewUp(0, 1, 0);
				//curCamera->SetPosition(0, 1, 0);
				//curCamera->SetFocalPoint(0, 0, 0);
				//m_renderer->SetActiveCamera(curCamera);
				ViewDirection(m_renderer, 0, -1, 0, 0, 0, 1);
				m_pQVTKWidget->update();
			}

			void SceneWidget::setViewDirectionToNegativeY() {
				ViewDirection(m_renderer, 0, 1, 0, 0, 0, 1);
				m_pQVTKWidget->update();
			}

			void SceneWidget::setViewDirectionToZ() {
				ViewDirection(m_renderer, 0, 0, -1, 0, 1, 0);
				m_pQVTKWidget->update();
			}

			void SceneWidget::setViewDirectionToNegativeZ() {
				ViewDirection(m_renderer, 0, 0, 1, 0, 1, 0);
				m_pQVTKWidget->update();
			}

			void SceneWidget::amplificationView() {
				vtkCamera* curCamera = m_renderer->GetActiveCamera();
				curCamera->Zoom(1.01); //微调
				m_pQVTKWidget->update();
			}

			void SceneWidget::narrowView() {
				vtkCamera* curCamera = m_renderer->GetActiveCamera();
				curCamera->Zoom(0.99); //微调
				m_pQVTKWidget->update();
			}


			void SceneWidget::initToolBars() {
				m_mainToolBar = addToolBar(tr("绘图工具栏"));
				m_mainToolBar->addAction(m_setViewDirectionToXAct);
				m_mainToolBar->addAction(m_setViewDirectionToNegativeXAct);
				m_mainToolBar->addAction(m_setViewDirectionToYAct);
				m_mainToolBar->addAction(m_setViewDirectionToNegativeYAct);
				m_mainToolBar->addAction(m_setViewDirectionToZAct);
				m_mainToolBar->addAction(m_setViewDirectionToNegativeZAct);
				m_mainToolBar->addAction(m_amplificationViewAct);
				m_mainToolBar->addAction(m_narrowViewAct);

				m_attributeToolBar = addToolBar(tr("属性工具栏"));
				//initAttributeSelToolBar();
				initDisplayModeToolBar();
				initDisplayClipModelToolBar();
				initOperateSwitchToolBar();

				initClipWindowVisibleToolBar();
				initClipWindowInvisibleToolBar();
				initClipToolBar();
				
				initClipSplitBar();
				m_attributeToolBar->addSeparator();
				initColorStopBar();
				initReClipToolBar();

			}
			void SceneWidget::clearBothClip() {
				emit SignalInstance::getInstance()->signalPrintInfo(u8"运行分段，需要重新显示身部");
				auto* thr = QThread::create([=]() {
					bool temp_flag = m_fullGridReadFlag;
					m_fullGridReadFlag = false;
					readAndOut(m_bodyClipPath, SignalInstance::getInstance()->getWorkSpace());
					m_fullGridReadFlag = temp_flag;
				});
				thr->start();
				connect(thr, &QThread::finished, this, [=]() {
					myStatus->setBothClip(false);
					emit SignalInstance::getInstance()->signalExeSect();
				});
			}
			void SceneWidget::recover() {
				QString path = SignalInstance::getInstance()->getWorkSpace();
				qDebug() << path;
				string str = SignalInstance::getInstance()->getWorkSpace().toLocal8Bit().toStdString() + "/status.txt";
				myStatus->readStatus(str);
				if (!myStatus->getClipFinish()) {
					QString gridPath = SignalInstance::getWorkSpace() + "/" + SignalInstance::getGridName();
					emit SignalInstance::getInstance()->signalLoadAndOut(gridPath, SignalInstance::getWorkSpace());
				}
				else {
					// ========== 新增：检查四个文件是否存在 ==========
					// 1. 定义需要检查的四个文件路径（请你替换为实际的文件名）
					QString file1 = path + "/body_genericbd.inp";   // 示例文件1，替换为你的实际文件名
					QString file2 = path + "/body_grid.grd";   // 示例文件2，替换为你的实际文件名
					QString file3 = path + "/head_bd.inp";   // 示例文件3，替换为你的实际文件名
					QString file4 = path + "/head_grid.grd";   // 示例文件4，替换为你的实际文件名

					// 2. 检查四个文件是否都存在（QFile::exists 会检查文件是否存在且是普通文件）
					bool file1Exist = QFile::exists(file1);
					bool file2Exist = QFile::exists(file2);
					bool file3Exist = QFile::exists(file3);
					bool file4Exist = QFile::exists(file4);

					// 3. 如果全部存在，打印信息并退出函数
					if (file1Exist && file2Exist && file3Exist && file4Exist) {
						int index = myStatus->getClipIndex();
						int dire = myStatus->getClipDire();
						QString gridPath = SignalInstance::getWorkSpace() + "/" + SignalInstance::getGridName();
						QString inpPath = SignalInstance::getWorkSpace() + "/" + SignalInstance::getInpName();
						emit SignalInstance::getInstance()->signalLoadOld(gridPath, inpPath, index, dire);


						return; // 退出函数，不再执行后续逻辑
					}
					// ========== 新增逻辑结束 ==========

					// 原流程：如果文件不全存在，执行这部分代码
					int index = myStatus->getClipIndex();
					int dire = myStatus->getClipDire();
					QString gridPath = SignalInstance::getWorkSpace() + "/" + SignalInstance::getGridName();
					QString inpPath = SignalInstance::getWorkSpace() + "/" + SignalInstance::getInpName();
					emit SignalInstance::getInstance()->signalLoadAndClip(gridPath, inpPath,index,dire);
				}
			}
			void SceneWidget::slotClipSplit() {
				myStatus->setStatus(Status::DEFAULT_STATUS);
				myStatus->Notify();
				emit SignalInstance::getInstance()->signalPreLoad(30);
				//emit SignalInstance::getInstance()->signalPrintInfo("begin clip_both");
				auto* thr = QThread::create([=]() {
					slotBothClip();
				});
				thr->start();
				connect(thr, &QThread::finished, this, [=]() {
					myStatus->setBothClip(true);
					myStatus->setStatus(Status::CLIP_FINISH);
					myStatus->Notify();

				});
			}
			void SceneWidget::slotClipBody(Status lastStatus) {
				
				myStatus->setStatus(Status::DEFAULT_STATUS);
				myStatus->Notify();
				emit SignalInstance::getInstance()->signalPreLoad(30);
				//emit SignalInstance::getInstance()->signalPrintInfo("show clip");
				auto* thr = QThread::create([=]() {
					slotClip();
				});
				thr->start();
				connect(thr, &QThread::finished, this, [=]() {
					myStatus->setStatus(Status::CLIP_FINISH);
					myStatus->Notify();
				});
			}
			void SceneWidget::initConnection()
			{
				connect(SignalInstance::getInstance(), &SignalInstance::signalColorMutex, this, [&]() {
					if (SignalInstance::getInstance()->color_mutex.tryLock()) {
						SignalInstance::getInstance()->color_mutex.unlock();
					}
					else {
						SignalInstance::getInstance()->color_mutex.unlock();
					}
				});
				connect(SignalInstance::getInstance(), &SignalInstance::signalRecover, this, &SceneWidget::recover);
				connect(SignalInstance::getInstance(), SIGNAL(signalClearBothClip()),this,SLOT(clearBothClip()));
				connect(SignalInstance::getInstance(), &SignalInstance::signalSectFinish, this, [=]() {
					mouseStyle->m_statusIsCanML = true;
				});
				connect(m_clipWindowVisible, &QPushButton::clicked, this, [=]()
				{
					if (n_blocks <= 1) {
						emit SignalInstance::getInstance()->signalPrintInfo(u8"网格只有一个block，无法进行切割");
						return;
					}
					myStatus->setStatus(Status::DEFAULT_STATUS);
					myStatus->Notify();
					emit SignalInstance::getInstance()->signalPreLoad(30);
					//emit SignalInstance::getInstance()->signalPrintInfo("show clip window");
					QString workPath = SignalInstance::getInstance()->getWorkSpace();
					rwTh->setFunFlag(6);
					rwTh->setStatusFlag(3);
					rwTh->start();
					/*connect(rwTh, &QThread::finished, this, [=]() {
						myStatus->setStatus(Status::CLIP_WINDOW_SHOW);
						myStatus->Notify();
					});*/
					
				});

				connect(m_clipWindowInvisible, &QPushButton::clicked, this, [=]()
				{
					myStatus->setStatus(Status::DEFAULT_STATUS);
					myStatus->Notify();
					emit SignalInstance::getInstance()->signalPreLoad(30);
					//emit SignalInstance::getInstance()->signalPrintInfo("cancel clip window");
					QString workPath = SignalInstance::getInstance()->getWorkSpace();
					rwTh->setFunFlag(7);
					rwTh->setStatusFlag(4);
					rwTh->start();
					/*connect(rwTh, &QThread::finished, this, [=]() {
						myStatus->setStatus(Status::NEW_WORKSPACE);
						myStatus->Notify();
					});*/
				});
				connect(m_btnClipSplit, &QPushButton::clicked, this, [=]()
				{
					slotClipSplit();
				});
				connect(m_btnClip, &QPushButton::clicked, this, [=]()
				{
					qDebug() << "lastStatus=" << myStatus->getLastStatus();
					slotClipBody(myStatus->getLastStatus());
				});
				connect(m_btnReClip, &QPushButton::clicked, this, [=]() {
					if (timer == NULL || !timer->isActive()) {
						
						emit SignalInstance::getInstance()->signalReClipClear();
					}
					else {
						QMessageBox::information(this, u8"提示", u8"请停止身部程序/停止染色后重新切割");
					}
					
					
					
				});
				//connect(m_btnClip, SIGNAL(clicked(bool)), this, SLOT(onClipGrid()));
				
				connect(m_btnColorStop, SIGNAL(clicked(bool)), this, SLOT(onColorLineStop()));
				connect(m_btnClipSplit, SIGNAL(clicked(bool)), this, SLOT(onClipSplit()));
				connect(m_mainTainOrBrowseButton, SIGNAL(statusChanged(bool)), this, SLOT(onChangeMlStatus()));
				connect(m_coverOrdumpButton, SIGNAL(statusChanged(bool)), this, SLOT(onChangeOperateStatus()));
				connect(SignalInstance::getInstance(), SIGNAL(signalWorkSpaceChanged(QString)), this, SLOT(onReset()));
				//------------------
				connect(SignalInstance::getInstance(), SIGNAL(signalChooseColor(int, int, int, int)), this, SLOT(setSectColor(int, int, int, int)));
				connect(SignalInstance::getInstance(), SIGNAL(signalColorLine()), this, SLOT(colorLine()));
				connect(SignalInstance::getInstance(), SIGNAL(signalHighlight(int)), this, SLOT(highlight(int)));
				connect(SignalInstance::getInstance(), SIGNAL(signalCancelHighlight()), this, SLOT(cancelHighlight()));


				//connect(SignalInstance::getInstance(), SIGNAL(signalShowClip()), this, SLOT(showClip()));
				//connect(SignalInstance::getInstance(), SIGNAL(signalCancelShowClip()), this, SLOT(cancelShowClip()));
				connect(SignalInstance::getInstance(), &SignalInstance::signalShowClip, this, [=]()
				{
					QString workPath = SignalInstance::getInstance()->getWorkSpace();
					rwTh->setFunFlag(6);
					rwTh->start();
				});
				connect(SignalInstance::getInstance(), &SignalInstance::signalCancelShowClip, this, [=]()
				{
					QString workPath = SignalInstance::getInstance()->getWorkSpace();
					rwTh->setFunFlag(7);
					rwTh->start();
				});
				/*connect(SignalInstance::getInstance(), SIGNAL(signalClip()), this, SLOT(slotClip()));
				connect(SignalInstance::getInstance(), SIGNAL(signalBothClip()), this, SLOT(slotBothClip()));*/
				/*connect(SignalInstance::getInstance(), &SignalInstance::signalClip, this, [=]() {
					auto* thr = QThread::create([=]() {
						
						slotClip();
					});
					
					thr->start();
				});
				connect(SignalInstance::getInstance(), &SignalInstance::signalBothClip, this, [=]() {
					auto* thr = QThread::create([=]() {
						
						slotBothClip();

					});
					thr->start();
				});*/

			}

			void SceneWidget::resetCamera(bool bCompulsion) {
				if (m_bResetCamera || bCompulsion) {
					//m_renderer->GetActiveCamera()->SetViewUp(0.0, 0.0, 1.0);
					m_renderer->ResetCamera();
					if (!bCompulsion) {
						vtkCamera* curCamera = m_renderer->GetActiveCamera();
						curCamera->GetViewUp(m_curViewUp);
						curCamera->GetPosition(m_curPosition);
						curCamera->GetFocalPoint(m_curFocalPoint);

						m_bResetCamera = false;
					}
				}
			}

			void SceneWidget::initVariables() {
				m_bResetCamera = true;
				m_attributeSelMapper = nullptr;
				m_attributeToolBar = nullptr;
				m_attributeSelToolBtn = nullptr;
				m_displayModeMapper = nullptr;
				m_clipModelMapper = nullptr;
				m_displayModeToolBtn = nullptr;
				isVTKData = 1;
				m_legendMapper = nullptr;
				m_legendToolBtn = nullptr;
				m_planeToolBtn = nullptr;
				m_planeMapper = nullptr;
				m_clipToolBtn = nullptr;
				m_curShowAttrType = "POINT";
			}

			void SceneWidget::initArrowActor() {
				m_widgetAxes->SetOrientationMarker(axesActor);
				m_widgetAxes->SetInteractor(m_pQVTKWidget->GetRenderWindow()->GetInteractor());
				m_widgetAxes->SetEnabled(1);
				m_widgetAxes->InteractiveOn();
				m_pQVTKWidget->update();
			}

			void SceneWidget::initAttributeSelToolBar() {
				//动态加载“属性选择”工具栏
				if (m_attributeSelMapper) {
					disconnect(m_attributeSelMapper, SIGNAL(mapped(QString)), this, SLOT(attributeSelSlot(QString)));
				}
				m_attributeSelMapper = new QSignalMapper(this);
				vector<QAction*> vecAction;
				for (auto it = m_mapAttrNames.begin(); it != m_mapAttrNames.end(); ++it) {
					for (auto& mode : it->second) {
						const QIcon icon = QIcon::fromTheme(tr(mode.c_str()), QIcon(("images/" + it->first + ".png").c_str()));
						QAction* action = new QAction(icon, tr(mode.c_str()), this);
						action->setStatusTip(tr(mode.c_str()));
						connect(action, SIGNAL(triggered()), m_attributeSelMapper, SLOT(map()));
						m_attributeSelMapper->setMapping(action, QString(mode.c_str()));
						vecAction.push_back(action);
					}
				}
				connect(m_attributeSelMapper, SIGNAL(mapped(QString)), this, SLOT(attributeSelSlot(QString)));

				// 创建QMenu
				QMenu* menu = new QMenu(this);
				for (auto& action : vecAction) {
					menu->addAction(action);
				}

				// 创建QToolButton
				if (m_attributeSelToolBtn == nullptr) {
					m_attributeSelToolBtn = new QToolButton(this);
					m_attributeToolBar->addWidget(m_attributeSelToolBtn);
					m_attributeSelToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
					m_attributeSelToolBtn->setPopupMode(QToolButton::InstantPopup);
				}
				m_attributeSelToolBtn->setIcon(QIcon::fromTheme(tr("属性选择"), QIcon("images/属性.png")));
				m_attributeSelToolBtn->setText(QString(tr("属性选择")));

				// 为QToolButton设置菜单
				m_attributeSelToolBtn->setMenu(vecAction.empty() ? nullptr : menu);
			}

			void SceneWidget::attributeSelSlot(QString attribute) {
				m_curShowAttrName = attribute.toUtf8().toStdString();
				for (auto it = m_mapAttrNames.begin(); it != m_mapAttrNames.end(); ++it) {
					if (std::find(it->second.begin(), it->second.end(), m_curShowAttrName) != it->second.end()) {
						m_curShowAttrType = it->first;
						break;
					}
				}

				QIcon icon = QIcon::fromTheme(tr(attribute.toUtf8().toStdString().c_str()),
					QIcon(("images/" + m_curShowAttrType + ".png").c_str()));
				m_attributeSelToolBtn->setIcon(icon);
				if (attribute.size() > 15) {
					attribute.resize(15);
					attribute += "...";
				}
				m_attributeSelToolBtn->setText(attribute);

				//updateReShowFlag();
				initUI();
			}

			void SceneWidget::SetActivateAttributeToMapperForVtk(vtkSmartPointer<vtkDataSetMapper> mapper) {
				if (m_curShowAttrName == "" || m_curShowAttrType == "") {
					mapper->SetScalarVisibility(false);
					return;
				}
				mapper->SetScalarVisibility(true);

				if (m_curShowAttrType == "POINT") {
					auto pPoint = mapper->GetInput()->GetPointData();
					if (-1 != pPoint->SetActiveScalars(m_curShowAttrName.c_str())
						|| -1 != pPoint->SetActiveVectors(m_curShowAttrName.c_str())
						|| -1 != pPoint->SetActiveTensors(m_curShowAttrName.c_str())
						|| -1 != pPoint->SetActiveNormals(m_curShowAttrName.c_str())
						|| -1 != pPoint->SetActiveTCoords(m_curShowAttrName.c_str())) {
						double* range = pPoint->GetArray(m_curShowAttrName.c_str())->GetRange();
						addScalarBarActorForVtk(mapper, m_curShowAttrName, range);
					}
					else { //非显示属性
						double range[2] = { 0.0,0.0 };
						addScalarBarActorForVtk(mapper, m_curShowAttrName, range);
					}
				}
				else if (m_curShowAttrType == "CELL") {
					auto pCell = mapper->GetInput()->GetCellData();
					if (-1 != pCell->SetActiveScalars(m_curShowAttrName.c_str())
						|| -1 != pCell->SetActiveVectors(m_curShowAttrName.c_str())
						|| -1 != pCell->SetActiveTensors(m_curShowAttrName.c_str())
						|| -1 != pCell->SetActiveNormals(m_curShowAttrName.c_str())
						|| -1 != pCell->SetActiveTCoords(m_curShowAttrName.c_str())) {
						double* range = pCell->GetArray(m_curShowAttrName.c_str())->GetRange();
						addScalarBarActorForVtk(mapper, m_curShowAttrName, range);
					}
					else { //非显示属性
						double range[2] = { 0.0,0.0 };
						addScalarBarActorForVtk(mapper, m_curShowAttrName, range);
					}
				}
			}

			void SceneWidget::addScalarBarActorForVtk(vtkSmartPointer<vtkDataSetMapper> mapper, string title, double * range) {
				if (m_scalarBarActor) {
					m_renderer->RemoveActor(m_scalarBarActor);
				}
				string original = m_curShowAttrName;
				m_scalarBarActor = vtkSmartPointer<vtkScalarBarActor>::New();
				mapper->SetScalarRange(range);
				m_scalarBarActor->SetLookupTable(mapper->GetLookupTable());
				m_scalarBarActor->SetTitle(original.c_str());
				m_scalarBarActor->GetTitleTextProperty()->SetColor(1, 1, 1);
				m_scalarBarActor->GetTitleTextProperty()->SetFontFamilyToArial();
				m_scalarBarActor->GetTitleTextProperty()->SetFontSize(20);
				m_scalarBarActor->GetLabelTextProperty()->SetColor(1, 1, 1);
				//m_scalarBarActor->SetLabelFormat("%5.3f");
				m_scalarBarActor->GetLabelTextProperty()->SetFontFamilyToArial();
				m_scalarBarActor->GetLabelTextProperty()->SetFontSize(20);
				m_scalarBarActor->SetNumberOfLabels(7);
				m_scalarBarActor->SetUnconstrainedFontSize(1);
				m_scalarBarActor->SetMaximumWidthInPixels(80);
				m_scalarBarActor->SetMaximumHeightInPixels(900);
				//m_scalarBarActor->DrawAnnotationsOn();
				m_scalarBarActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
				m_scalarBarActor->GetPositionCoordinate()->SetValue(.9, .05);
				//m_scalarBarActor->SetWidth(0.15);
				//m_scalarBarActor->SetHeight(0.5);
				m_scalarBarActor->SetWidth(0.05);
				m_scalarBarActor->SetHeight(0.25);
				m_renderer->AddActor(m_scalarBarActor);

			}

			void SceneWidget::initDisplayModeToolBar() {
				//动态加载“显示模式”工具栏
				if (m_displayModeMapper) {
					disconnect(m_displayModeMapper, SIGNAL(mapped(QString)), this, SLOT(displayModeSlot(QString)));
				}
				m_displayModeMapper = new QSignalMapper(this);
				const static vector<string> vecDisplayMode = {
					"轮廓模式",//"Outline",
					"散点模式",//"Points",
					"表面模式",//"Surface",
					"网格模式",//"Wireframe",
					"网面模式"//"Surface With Edges"
				};
				vector<QAction*> vecAction;
				for (auto& mode : vecDisplayMode) {
					const QIcon icon = QIcon::fromTheme(tr(mode.c_str()), QIcon((":/binForPublic/images/" + mode + ".png").c_str()));
					QAction* action = new QAction(icon, tr(mode.c_str()), this);
					action->setStatusTip(tr(mode.c_str()));
					connect(action, SIGNAL(triggered()), m_displayModeMapper, SLOT(map()));
					m_displayModeMapper->setMapping(action, QString(mode.c_str()));
					vecAction.push_back(action);
				}
				connect(m_displayModeMapper, SIGNAL(mapped(QString)), this, SLOT(displayModeSlot(QString)));

				// 创建QMenu
				QMenu* menu = new QMenu(this);
				for (auto& action : vecAction) {
					menu->addAction(action);
				}
				
				// 创建QToolButton
				if (!m_displayModeToolBtn) {
					m_displayModeToolBtn = new QToolButton(this);
					m_attributeToolBar->addWidget(m_displayModeToolBtn);
					//m_displayModeToolBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
					m_displayModeToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
					m_displayModeToolBtn->setPopupMode(QToolButton::InstantPopup);
				}
				qDebug() << "mode str=" << QString::fromStdString(vecDisplayMode[0]);
				m_displayModeToolBtn->setIcon(QIcon::fromTheme(tr("显示模式"),
					QIcon((":/binForPublic/images/" + vecDisplayMode[0] + ".png").c_str())));
				m_displayModeToolBtn->setText(QString(tr("显示模式")));

				// 为QToolButton设置菜单
				m_displayModeToolBtn->setMenu(vecAction.empty() ? nullptr : menu);
				// m_curShowMeshMode = "表面模式";
			}

			void SceneWidget::displayModeSlot(QString mode) {
				m_curShowMeshMode = mode.toUtf8().toStdString();
				QIcon icon = QIcon::fromTheme(tr(mode.toUtf8().toStdString().c_str()),
					QIcon((":/binForPublic/images/" + mode.toUtf8().toStdString() + ".png").c_str()));
				m_displayModeToolBtn->setIcon(icon);
				if (mode.size() > 15) {
					mode.resize(15);
					mode += "...";
				}
				m_displayModeToolBtn->setText(mode);

				//updateReShowFlag();
				initUI();
			}

			void SceneWidget::clipModelSlot(QString model)
			{

				m_curShowClipModel = model.toStdString();
				string iconPath = (model == tr("左侧作为pns3d输入") ? "gridl.png" : "gridr.png");
				QIcon icon = QIcon::fromTheme(tr(model.toUtf8().toStdString().c_str()),
					QIcon(("images/" + iconPath).c_str()));
				m_clipToolBtn->setIcon(icon);
				if (model.size() > 15) {
					model.resize(15);
					model += "...";
				}
				m_clipToolBtn->setText(model);
				setClipModel();
			}

			void SceneWidget::initDisplayClipModelToolBar()
			{
				if (m_clipModelMapper) {
					disconnect(m_clipModelMapper, SIGNAL(mapped(QString)), this, SLOT(clipModelSlot(QString)));
				}
				m_clipModelMapper = new QSignalMapper;

				const static vector<string> vecDisplayClipMode = {
					"左侧作为pns3d输入",//"Outline",
					"右侧作为pns3d输入"
				};
				//创建QAction，并绑定映射关系
				vector<QAction*> vecAction;
				const QIcon icon1 = QIcon::fromTheme(tr(vecDisplayClipMode[0].c_str()), QIcon((":/binForPublic/images/gridl.png")));
				QAction* action1 = new QAction(icon1, tr(vecDisplayClipMode[0].c_str()), this);
				connect(action1, SIGNAL(triggered()), m_clipModelMapper, SLOT(map()));
				m_clipModelMapper->setMapping(action1, QString(vecDisplayClipMode[0].c_str()));
				const QIcon icon2 = QIcon::fromTheme(tr(vecDisplayClipMode[1].c_str()), QIcon((":/binForPublic/images/gridr.png")));
				QAction* action2 = new QAction(icon2, tr(vecDisplayClipMode[1].c_str()), this);
				connect(action2, SIGNAL(triggered()), m_clipModelMapper, SLOT(map()));
				m_clipModelMapper->setMapping(action2, QString(vecDisplayClipMode[1].c_str()));
				connect(m_clipModelMapper, SIGNAL(mapped(QString)), this, SLOT(clipModelSlot(QString)));

				QMenu* menu = new QMenu;
				menu->addAction(action1);
				menu->addAction(action2);
				if (!m_clipToolBtn)
				{
					m_clipToolBtn = new QToolButton(this);
					m_clipToolBtn->setFixedWidth(200);

					
					
					// 样式设置将文字放在Icon边上 
					m_clipToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
					// 样式设置下拉形式的QToolButton
					m_clipToolBtn->setPopupMode(QToolButton::InstantPopup);
				}
				m_clipToolBtn->setIcon(QIcon::fromTheme(tr("切割模式选择"),
					QIcon((":/binForPublic/images/gridr.png"))));
				m_clipToolBtn->setText(tr("切割模式选择"));
				m_clipToolBtn->setMenu(menu);
				if (m_nsFlag == 0)m_attributeToolBar->addWidget(m_clipToolBtn);
				else {
					m_clipToolBtn->setVisible(false);
				}
			}

			void SceneWidget::setClipModel()
			{
				if (m_curShowClipModel == "左侧作为pns3d输入")
				{
					m_rightClipFlag = false;
				}
				else 
				{
					m_rightClipFlag = true;
				}
			}

			void SceneWidget::initUI() {
				// updateAttributeNameMap(vtk_dataSet);
				SetActivateAttributeToMapperForVtk(plt_mapper);
				// ==========================================================
				if (m_curShowMeshMode == "轮廓模式") {
					// enabledAttributeSelToolBtn(false);
					// m_vecActorUi_show.clear();
					vtkSmartPointer<vtkOutlineFilter> outlineData1 = vtkSmartPointer<vtkOutlineFilter>::New();
					if (isVTKData == 1) {
						for (auto v : m_vecActorUi_show_vtk) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show_vtk.clear();
						outlineData1->SetInputData(vtk_dataSet);
						m_vecActorUi_show_vtk.push_back(addOutlineActorForVtk(outlineData1));
						m_renderer->RemoveActor(vtk_actor);
					}
					else {
						for (auto v : m_vecActorUi_show) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show.clear();
						outlineData1->SetInputData(plt_dataSet);
						m_vecActorUi_show.push_back(addOutlineActorForVtk(outlineData1));
						m_renderer->RemoveActor(plt_actor);
					}
					m_pQVTKWidget->update();
				}
				else if (m_curShowMeshMode == "散点模式") {
					// vtkSmartPointer<vtkDataSet> outlineData1 = vtkSmartPointer<vtkDataSet>::New();
					if (isVTKData == 1) {
						for (auto v : m_vecActorUi_show_vtk) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show_vtk.clear();
						m_vecActorUi_show_vtk.push_back(addPointsActorForVtk(vtk_dataSet));
						m_renderer->RemoveActor(vtk_actor);
					}
					else {
						for (auto v : m_vecActorUi_show) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show.clear();
						m_vecActorUi_show.push_back(addPointsActorForVtk(plt_dataSet));
						m_renderer->RemoveActor(plt_actor);
					}
				}
				else if (m_curShowMeshMode == "表面模式" || m_curShowMeshMode == "") {
					if (isVTKData == 1) {
						for (auto v : m_vecActorUi_show_vtk) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show_vtk.clear();
						m_renderer->AddActor(vtk_actor);
					}
					else {
						for (auto v : m_vecActorUi_show) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show.clear();
						m_renderer->AddActor(plt_actor);
					}
				}
				else if (m_curShowMeshMode == "网格模式") {
					// vtkSmartPointer<vtkDataSet> outlineData1 = vtkSmartPointer<vtkDataSet>::New();
					if (isVTKData == 1) {
						for (auto v : m_vecActorUi_show_vtk) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show_vtk.clear();
						m_vecActorUi_show_vtk.push_back(addWireFrameActorForVtk(vtk_dataSet, false));
						m_renderer->RemoveActor(vtk_actor);
					}
					else {
						for (auto v : m_vecActorUi_show) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show.clear();
						m_vecActorUi_show.push_back(addWireFrameActorForVtk(plt_dataSet, false));
						m_renderer->RemoveActor(plt_actor);
					}
				}
				else if (m_curShowMeshMode == "网面模式") {
					// vtkSmartPointer<vtkDataSet> outlineData1 = vtkSmartPointer<vtkDataSet>::New();
					if (isVTKData == 1) {
						for (auto v : m_vecActorUi_show_vtk) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show_vtk.clear();
						m_vecActorUi_show_vtk.push_back(addWireFrameActorForVtk(vtk_dataSet, false));
						m_renderer->AddActor(vtk_actor);
					}
					else {
						for (auto v : m_vecActorUi_show) {
							m_renderer->RemoveActor(v);
						}
						m_vecActorUi_show.clear();
						m_vecActorUi_show.push_back(addWireFrameActorForVtk(plt_dataSet, false));
						m_renderer->AddActor(plt_actor);
					}
				}
				// ==========================================================
			}

			vtkSmartPointer<vtkActor> SceneWidget::addOutlineActorForVtk(vtkSmartPointer<vtkOutlineFilter> outlineData) {
				return addOutlineActor(outlineData);
			}

			vtkSmartPointer<vtkActor> SceneWidget::addOutlineActor(vtkSmartPointer<vtkOutlineFilter> outlineData) {
				vtkSmartPointer<vtkDataSetMapper> mapOutline = vtkSmartPointer<vtkDataSetMapper>::New();
				//vtkSmartPointer<vtkPolyDataMapper> mapOutline = vtkSmartPointer<vtkPolyDataMapper>::New();
				mapOutline->SetInputConnection(outlineData->GetOutputPort());

				vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
				outlineActor->SetMapper(mapOutline);
				outlineActor->GetProperty()->SetColor(1, 0, 0);
				
				m_renderer->AddActor(outlineActor);
				m_pQVTKWidget->update();

				return outlineActor;
			}

			vtkSmartPointer<vtkActor> SceneWidget::addPointsActorForVtk(vtkSmartPointer<vtkDataSet> dataSet) {
				vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
				mapper->SetInputData(dataSet);
				mapper->SetScalarVisibility(false);
				SetActivateAttributeToMapperForVtk(mapper);

				vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
				actor->GetProperty()->SetAmbient(1.0);
				actor->GetProperty()->SetDiffuse(0.0);
				actor->GetProperty()->SetSpecular(0.0);
				actor->SetMapper(mapper);
				actor->GetProperty()->SetRepresentationToPoints();
				//actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
				actor->GetProperty()->SetPointSize(2.0);
				actor->PickableOff();

				m_renderer->AddActor(actor);
				m_pQVTKWidget->update();

				return actor;
			}

			vtkSmartPointer<vtkActor> SceneWidget::addWireFrameActorForVtk(vtkSmartPointer<vtkDataSet> dataSet, bool bBlack) {
				vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
				mapper->SetInputData(dataSet);
				mapper->SetScalarVisibility(false);
				if (!bBlack) {
					SetActivateAttributeToMapperForVtk(mapper);
				}

				vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
				actor->GetProperty()->SetAmbient(1.0);
				actor->GetProperty()->SetDiffuse(0.0);
				actor->GetProperty()->SetSpecular(0.0);
				actor->SetMapper(mapper);
				actor->GetProperty()->SetRepresentationToWireframe();
				if (bBlack) {
					//actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
					actor->GetProperty()->SetColor(0.0, 0.0, 0.0);
				}
				actor->GetProperty()->SetLineWidth(1.2);
				actor->PickableOff();

				m_renderer->AddActor(actor);
				m_pQVTKWidget->update();

				return actor;
			}

			void SceneWidget::updateAttributeNameMap(vtkSmartPointer<vtkUnstructuredGrid> mesh)
				//void SceneWidget::updateAttributeNameMap(vtkSmartPointer<vtkPolyData> mesh) 
			{
				if (!mesh) return;
				m_mapAttrNames.clear();

				//mesh->GetAttributes(vtkDataSet::POINT);
				vector<string> vecAttrNames1;

				auto pPoint = mesh->GetAttributes(vtkDataSet::POINT);
				for (int i = 0; i < pPoint->GetNumberOfArrays(); ++i) {
					vecAttrNames1.push_back(pPoint->GetArrayName(i));
				}
				m_mapAttrNames["POINT"] = vecAttrNames1;

				//mesh->GetAttributes(vtkDataSet::CELL);
				vector<string> vecAttrNames2;

				auto pCell = mesh->GetAttributes(vtkDataSet::CELL);
				for (int i = 0; i < pCell->GetNumberOfArrays(); ++i) {
					vecAttrNames2.push_back(pCell->GetArrayName(i));
				}
				m_mapAttrNames["CELL"] = vecAttrNames2;

				//mesh->GetAttributesAsFieldData(vtkDataSet::FIELD)
				vector<string> vecAttrNames3;


				auto pField = mesh->GetAttributesAsFieldData(vtkDataSet::FIELD);
				for (int i = 0; i < pField->GetNumberOfArrays(); ++i) {
					vecAttrNames3.push_back(pField->GetArrayName(i));
				}
				m_mapAttrNames["FIELD"] = vecAttrNames3;
			}

			void SceneWidget::initScene()
			{
				// Camera
				m_pQVTKWidget = new QVTKWidget(this);
				/// @brief place the m_pQVTKWidget in the center
				setCentralWidget(m_pQVTKWidget);
				vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
				/// @brief 相机朝向
				camera->SetViewUp(0, 0, 1);
				/// @brief 相机位置
				camera->SetPosition(0, 0, 0);
				/// @brief 相机焦点
				camera->SetFocalPoint(1, 0, 0);
				camera->Zoom(0.5);

				// Renderer
				m_renderer = vtkSmartPointer<vtkRenderer>::New();
				m_renderer->SetActiveCamera(camera);
				m_renderer->SetBackground(1.0, 1.0, 1.0);                        // 设置页面底部颜色值
				m_renderer->SetBackground2(0.529, 0.8078, 0.92157);    // 设置页面顶部颜色值
				m_renderer->SetGradientBackground(1);                           // 开启渐变色背景设置
				m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);
				m_pQVTKWidget->update();
			}

			void SceneWidget::initPlaneChooseToolBar() {
				if (m_planeMapper) {
					disconnect(m_planeMapper, SIGNAL(mapped(QString)), this, SLOT(planeChooseSlot(QString)));
				}
				m_planeMapper = new QSignalMapper(this);
				const vector<string> vecPlane = {
					"原始网格数据",
					"最新动网格数据"
				};
				vector<QAction*> vecAction;
				for (auto& picker : vecPlane) {
					QAction* action = new QAction(tr(picker.c_str()), this);
					action->setStatusTip(tr(picker.c_str()));
					connect(action, SIGNAL(triggered()), m_planeMapper, SLOT(map()));
					m_planeMapper->setMapping(action, QString(picker.c_str()));
					vecAction.push_back(action);
				}
				connect(m_planeMapper, SIGNAL(mapped(QString)), this, SLOT(planeChooseSlot(QString)));
				// 创建QMenu
				QMenu* menu = new QMenu(this);
				for (auto& action : vecAction) {
					menu->addAction(action);
				}

				m_planeToolBtn = new QToolButton(this);
				m_attributeToolBar->addWidget(m_planeToolBtn);
				m_planeToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
				m_planeToolBtn->setPopupMode(QToolButton::InstantPopup);

				m_planeToolBtn->setText(QString(tr("数据源选择")));
				//m_planeToolBtn->setMenu(vecAction.empty() ? nullptr : menu);
			}

			void SceneWidget::legendSlot(QString legend) {
				if (legend == "显示图例") {
					m_renderer->AddActor(m_scalarBarActor);
					m_pQVTKWidget->update();
				}
				else if (legend == "隐藏图例") {
					m_renderer->RemoveActor(m_scalarBarActor);
					m_pQVTKWidget->update();
				}
				// }
			}



			void SceneWidget::slotLoadSectData(QString path)
			{

				if (sectInfo != NULL) {
					sectInfo->clear();
					free(sectInfo);
					sectInfo = NULL;
				}

				read_sect(QString(path).toLocal8Bit().toStdString());
				int overlapFlag = defaultOverlapFLag;
				
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								//x += coordinateOffset[id];
								int value = get_id(ni, nj, nk, i, j, k, id, overlapFlag);
								un_grid->pointData[value].set_sectId(bk[id].sectId[i]);

								if (bk[id].sectId[i] < 0) {
									qDebug() << "value=" << value << " sectId=" << bk[id].sectId[i];
								}
								//xyzPoint temp;
								//temp.init(x, y, z);
								//auto it = idMap.find(temp);
								//if (it == idMap.end()) {
								//	// find fail

								//	//idMap.insert(make_pair(temp, grid->index_points));
								//	qDebug() << __FUNCTION__;
								//}
								//else {
								//	//find success
								//	int index = it->second;
								//	un_grid->pointData[index].set_sectId(bk[id].sectId[i]);
								//	continue;
								//}


							}
						}
					}
				}
				sectInfo->setNumofPoints(un_grid->n_points);
				build_sectPool(sectInfo, un_grid);
				//将每一段的cell信息都隔离开
				int numCells = un_grid->n_cells;
				for (int i = 0; i < numCells; i++) {
					cell* temp = &(un_grid->cellData[i]);
					int id1, id2, id3, id4;
					id1 = temp->idPoint[0];
					id2 = temp->idPoint[1];
					id3 = temp->idPoint[2];
					id4 = temp->idPoint[3];
					int sectId1, sectId2, sectId3, sectId4;
					sectId1 = un_grid->pointData[id1].sectId;
					sectId2 = un_grid->pointData[id2].sectId;
					sectId3 = un_grid->pointData[id3].sectId;
					sectId4 = un_grid->pointData[id4].sectId;
					if (sectId1 == sectId2 && sectId2 == sectId3 && sectId3 == sectId4) {
						sectInfo->cellNum[sectId1]++;
					}
				}
				for (int i = 0; i < sectInfo->sectNum; i++) {
					sectInfo->setCellNum(i, sectInfo->cellNum[i]);
				}
				for (int i = 0; i < numCells; i++) {
					cell* temp = &(un_grid->cellData[i]);
					int id1, id2, id3, id4;
					id1 = temp->idPoint[0];
					id2 = temp->idPoint[1];
					id3 = temp->idPoint[2];
					id4 = temp->idPoint[3];
					int sectId1, sectId2, sectId3, sectId4;
					sectId1 = un_grid->pointData[id1].sectId;
					sectId2 = un_grid->pointData[id2].sectId;
					sectId3 = un_grid->pointData[id3].sectId;
					sectId4 = un_grid->pointData[id4].sectId;
					if (sectId1 == sectId2 && sectId2 == sectId3 && sectId3 == sectId4) {
						int realId1, realId2, realId3, realId4;
						realId1 = sectInfo->sectIdMap[id1];
						realId2 = sectInfo->sectIdMap[id2];
						realId3 = sectInfo->sectIdMap[id3];
						realId4 = sectInfo->sectIdMap[id4];
						sectInfo->addCell(sectId1, realId1, realId2, realId3, realId4);
					}
				}




				unsigned char myRed[3] = { 255, 0, 0 };
				unsigned char myGreen[3] = { 0, 255, 0 };
				unsigned char myBlue[3] = { 0, 0, 255 };
				unsigned char myYellow[3] = { 255,255,0 };
				unsigned char myBlack[3] = { 0,0,0 };
				unsigned char mySilver[3] = { 192,192,192 };
				unsigned char myPurple[3] = { 128,0,128 };
				unsigned char myBrown[3] = { 165,42,42 };
				ColorsSect->SetNumberOfTuples(0);
				ColorsSect->SetNumberOfComponents(3);
				ColorsSect->SetName("ColorsSect");
				int n_points = un_grid->n_points;
				for (int i = 0; i < n_points; i++) {
					int id;
					id = un_grid->pointData[i].sectId;
					ColorsSect->InsertNextTupleValue(myColor->colorTable[id]);

				}
			}
			
			void SceneWidget::colorSect(int decimatedFlag) {
				this->decimatedFlag = decimatedFlag;
				//update color
				QString path = SignalInstance::getInstance()->m_workspace + "/body/trData.dat";
				BodyPathChecker tempChecker;
				path = tempChecker.checkAndGetPath(path);
				QFile file(path);
				int sectNum = 0;
				if (decimatedFlag == 0) sectNum = sectInfo->sectNum;
				else sectNum = decimated_sectInfo->sectNum;
				vector<vector<unsigned char>> colorData(sectNum, vector<unsigned char>(3,'0'));
				if (file.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					QTextStream in(&file);
					QString line;
					int indexLine = 0;
					while (!in.atEnd())
					{

						line = in.readLine();

						line.replace(QRegExp("[\\s]+"), " ");
						line.remove(QRegExp("\\s* +$"));
						QStringList list = line.split(" ");

						if (indexLine < sectNum)
						{

							colorData[indexLine][0] = list.value(0).toInt();
							colorData[indexLine][1] = list.value(1).toInt();
							colorData[indexLine][2] = list.value(2).toInt();

							indexLine++;
							continue;
						}

						for (int i = 0; i < sectNum; i++)
						{
							line = in.readLine();
							line.replace(QRegExp("[\\s]+"), " ");
							line.remove(QRegExp("\\s* +$"));
							list = line.split(" ");
							for (int j = 0; j < list.size(); )
							{
								if (list.at(j) == " ")
								{
									list.removeAt(j);
									continue;
								}
								j++;
							}
							indexLine++;
						}
						break;
					}
					file.close();
				}
				else {
					for (int i = 0; i < sectNum; i++) {
						for (int j = 0; j < 3; j++) {
							colorData[i][j] = myColor->colorTable[i][j];
						}
					}
				}


				if (decimatedFlag == 0) {
					for (int i = 0; i < sectInfo->sectNum; i++) {
						emit SignalInstance::getInstance()->signalChooseColor(colorData[i][0], colorData[i][1], colorData[i][2], i);
						//emit SignalInstance::getInstance()->signalChooseColor(myColor->colorTable[i][0], myColor->colorTable[i][1], myColor->colorTable[i][2], i);
					}
					for (int i = 0; i < un_grid->n_points; i++) {
						int id;
						id = un_grid->pointData[i].sectId;
						unsigned char tempColor[3] = {colorData[id][0],colorData[id][1], colorData[id][2]};
						ColorsCurrent->SetTypedTuple(i, tempColor);
						//ColorsCurrent->SetTypedTuple(i, myColor->colorTable[id]);
					}
					ColorsCurrent->SetName("ColorsCurrent");
					ugrid->GetPointData()->SetActiveScalars("ColorsCurrent");
					m_pQVTKWidget->update();
				}
				else if (decimatedFlag == 1) {
					for (int i = 0; i < decimated_sectInfo->sectNum; i++) {//char*colorTable[3]
						emit SignalInstance::getInstance()->signalChooseColor(colorData[i][0], colorData[i][1], colorData[i][2], i);

						//emit SignalInstance::getInstance()->signalChooseColor(myColor->colorTable[i][0], myColor->colorTable[i][1], myColor->colorTable[i][2], i);
					}
					for (int i = 0; i < decimated_grid->n_points; i++) {
						int id;
						id = decimated_grid->pointData[i].sectId;
						unsigned char tempColor[3] = { colorData[id][0],colorData[id][1], colorData[id][2] };
						ColorsDecimated->SetTypedTuple(i, tempColor);
						//ColorsDecimated->SetTypedTuple(i, myColor->colorTable[id]);
					}
					ColorsDecimated->SetName("ColorsDecimated");
					decimatedGrid->GetPointData()->SetActiveScalars("ColorsDecimated");
					m_pQVTKWidget->update();
				}
			}
			void SceneWidget::trOnlyLoad()
			{
				qDebug() << "trOnlyLoad";
				QString path = SignalInstance::getInstance()->m_workspace + "/body/trData.dat";
				BodyPathChecker tempChecker;
				path = tempChecker.checkAndGetPath(path);
				QFile file(path);
				int sectNum = decimated_sectInfo->sectNum;
				qDebug() << "sectNum=" << sectNum;
				unsigned char** colorData = new unsigned char*[sectNum];
				for (int i = 0; i < sectNum; i++)
				{
					colorData[i] = new unsigned char[3];
					memset(colorData[i], '0', 3);
				}
				QStringList* sectTrInfo = new QStringList[decimated_sectInfo->sectNum];
				if (file.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					QTextStream in(&file);
					QString line;
					int indexLine = 0;
					while (!in.atEnd())
					{

						line = in.readLine();

						line.replace(QRegExp("[\\s]+"), " ");
						line.remove(QRegExp("\\s* +$"));
						QStringList list = line.split(" ");

						if (indexLine < decimated_sectInfo->sectNum)
						{

							colorData[indexLine][0] = list.value(0).toInt();
							colorData[indexLine][1] = list.value(1).toInt();
							colorData[indexLine][2] = list.value(2).toInt();

							indexLine++;
							continue;
						}

						for (int i = 0; i < decimated_sectInfo->sectNum; i++)
						{
							line = in.readLine();
							line.replace(QRegExp("[\\s]+"), " ");
							line.remove(QRegExp("\\s* +$"));
							list = line.split(" ");
							for (int j = 0; j < list.size(); )
							{
								if (list.at(j) == " ")
								{
									list.removeAt(j);
									continue;
								}
								j++;
							}
							sectTrInfo[i] = list;
							indexLine++;
						}
						break;
					}
				}
				qDebug() << decimated_sectInfo->sectNum;
				qDebug() << sectTrInfo;
				qDebug() << colorData;
				emit SignalInstance::getInstance()->signalOnlyTrDataLoad(decimated_sectInfo->sectNum, sectTrInfo, colorData);
				
			}
			
			void SceneWidget::trPreLoad()
			{
				QString path = SignalInstance::getInstance()->m_workspace + "/body/trData.dat";
				BodyPathChecker tempChecker;
				path = tempChecker.checkAndGetPath(path);
				QFile file(path);
				int sectNum = decimated_sectInfo->sectNum;
				//unsigned char(*colorData)[3] = new unsigned char[sectNum][3];
				unsigned char** colorData = new unsigned char*[sectNum];
				for (int i = 0; i < sectNum; i++)
				{
					colorData[i] = new unsigned char[3];
					memset(colorData[i], '0', 3);
				}
				QStringList* sectTrInfo = new QStringList[decimated_sectInfo->sectNum];
				if (file.open(QIODevice::ReadOnly | QIODevice::Text))
				{
					QTextStream in(&file);
					QString line;
					int indexLine = 0;
					while (!in.atEnd())
					{

						line = in.readLine();

						line.replace(QRegExp("[\\s]+"), " ");
						line.remove(QRegExp("\\s* +$"));
						QStringList list = line.split(" ");

						if (indexLine < decimated_sectInfo->sectNum)
						{

							colorData[indexLine][0] = list.value(0).toInt();
							colorData[indexLine][1] = list.value(1).toInt();
							colorData[indexLine][2] = list.value(2).toInt();

							indexLine++;
							continue;
						}

						for (int i = 0; i < decimated_sectInfo->sectNum; i++)
						{
							line = in.readLine();
							line.replace(QRegExp("[\\s]+"), " ");
							line.remove(QRegExp("\\s* +$"));
							list = line.split(" ");
							for (int j = 0; j < list.size(); )
							{
								if (list.at(j) == " ")
								{
									list.removeAt(j);
									continue;
								}
								j++;
							}
							sectTrInfo[i] = list;
							indexLine++;
						}
						break;
					}
				}
				emit SignalInstance::getInstance()->signalTrDataLoad(decimated_sectInfo->sectNum, sectTrInfo, colorData);
				for (int i = 0; i < decimated_grid->n_points; i++) {
					int id;
					id = decimated_grid->pointData[i].sectId;
					ColorsDecimated->SetTypedTuple(i, colorData[id]);
				}
				ColorsDecimated->SetName("ColorsDecimated");
				decimatedGrid->GetPointData()->SetActiveScalars("ColorsDecimated");
				m_pQVTKWidget->update();
			}
			void SceneWidget::changeGapModel()
			{
				if (gapModel == 0)gapModel = 1;
				else
				{
					gapModel = 0;
				}
			}
			void SceneWidget::highlight(int sectId) {

				cancelHighlight();

				//vtkNew<vtkUnstructuredGrid> highlightData;
				//vtkNew<vtkPoints> hilightPoints;
				int n_cells, n_points;
				sectPool* tempInfo = NULL;
				UNSTRUCTURED_GRID*Grid = NULL;
				if (decimatedFlag == 0) {
					tempInfo = sectInfo;
					Grid = un_grid;
				}
				else {
					tempInfo = decimated_sectInfo;
					Grid = decimated_grid;
				}
				if (tempInfo == NULL || Grid == NULL) return;
				//emit emit SignalInstance::getInstance()->signalPrintInfo(QString("开始高亮第%1段").arg(sectId + 1));
				if (sectId >= tempInfo->sectNum) return;

				hilightPoints->DeepCopy(emptyPoints);
				highlightData->DeepCopy(emptyGrid);
				//SignalInstance::getInstance()->printInfo("begin highlight");
				n_cells = tempInfo->cellNum[sectId];
				n_points = tempInfo->pointNum[sectId];


				highlightData->Allocate(n_cells);
				for (int i = 0; i < n_cells; i++) {

					int id1 = tempInfo->cellId1[sectId][i];
					int id2 = tempInfo->cellId2[sectId][i];
					int id3 = tempInfo->cellId3[sectId][i];
#ifdef decimateSurface
					vtkIdType idList[3] = { id1,id2,id3};
					highlightData->InsertNextCell(VTK_TRIANGLE, 3, idList);
#else
					int id4 = tempInfo->cellId4[sectId][i];
					vtkIdType idList[4] = { id1,id2,id3,id4 };
					highlightData->InsertNextCell(VTK_TETRA, 4, idList);
#endif // decimateSurface
					
				}

				for (int i = 0; i < n_points; i++) {
					point*temp = &(Grid->pointData[tempInfo->idInfo[sectId][i]]);

					double x = temp->x;
					double y = temp->y;
					double z = temp->z;
					hilightPoints->InsertNextPoint(x, y, z);
				}
				highlightData->SetPoints(hilightPoints);

				//vtkNew<vtkDataSetMapper> highlightMapper;
				//vtkNew<vtkActor> highlightActor;
				highlightMapper->SetInputData(highlightData);
				highlightActor->SetMapper(highlightMapper);
				//vtkNew<vtkNamedColors> namedColors;
				highlightActor->GetProperty()->SetColor(namedColors->GetColor3d("White").GetData());
				m_renderer->AddActor(highlightActor);
				m_pQVTKWidget->update();

			}
			void SceneWidget::cancelHighlight() {

				m_renderer->RemoveActor(highlightActor);
			}

			void SceneWidget::clearAll() {
				emit SignalInstance::getInstance()->finishEventLoop();
				cancelHighlight();
				cancelShowClip();
				ColorsCurrent->SetNumberOfTuples(0);
				ColorsGroup->SetNumberOfTuples(0);
				ColorsSect->SetNumberOfTuples(0);
				ColorsDecimated->SetNumberOfTuples(0);


				points->DeepCopy(emptyPoints);
				decimatedPoints->DeepCopy(emptyPoints);
				hilightPoints->DeepCopy(emptyPoints);

				ugrid->DeepCopy(emptyGrid);
				decimatedGrid->DeepCopy(emptyGrid);
				highlightData->DeepCopy(emptyGrid);

				mouseStyle->canCliped = false;
				
				//ColorStop();

				clipIndex = NULL;

				if (tagI != NULL) {
					delete[]tagI;
					tagI = NULL;
				}
				if (headGrid != NULL) {
					headGrid->clear();
					free(headGrid);
					headGrid = NULL;
				}
				if (bodyGrid != NULL) {
					bodyGrid->clear();
					free(bodyGrid);
					bodyGrid = NULL;
				}
				if (un_grid != NULL) {
					un_grid->clear();
					free(un_grid);
					un_grid = NULL;
				}
				if (bk != NULL) {
					for (int i = 0; i < n_blocks; i++) {
						bk[i].clear();
					}
					free(bk);
					bk = NULL;
				}

				if (sectInfo != NULL) {
					sectInfo->clear();
					free(sectInfo);
					sectInfo = NULL;
				}
				if (idOffset != NULL) {
					qDebug() << "idOffset=" << idOffset << " size=" << sizeof(idOffset) << endl;
					free(idOffset);
					idOffset = NULL;
				}
				if (coordinateOffset != NULL) {
					free(coordinateOffset);
					coordinateOffset = NULL;
				}

				if (decimated_grid != NULL) {
					decimated_grid->clear();
					free(decimated_grid);
					decimated_grid = NULL;
				}
				if (decimated_sectInfo != NULL) {
					decimated_sectInfo->clear();
					free(decimated_sectInfo);
					decimated_sectInfo = NULL;
				}
				if (gridLine != NULL) {
					gridLine->clear();
					free(gridLine);
					gridLine = NULL;
				}
				if (decimatedLine != NULL) {
					decimatedLine->clear();
					free(decimatedLine);
					decimatedLine = NULL;
				}
				idMap.clear();
				decimated_idMap.clear();
				decimatedFlag = 0;
				if (timer != NULL) {
					timer->stop();
					delete timer;
					timer = NULL;
				}
				if (lastColor != NULL) {
					delete[]lastColor;
					lastColor = NULL;
				}
				if (idTrans != NULL) {
					delete[]idTrans;
					idTrans = NULL;
				}
				
			}
			void SceneWidget::planeChooseSlot(QString pickName) {
				if (pickName == "原始网格数据") {
					isVTKData = 1;
					updateAttributeNameMap(vtk_dataSet);
					initAttributeSelToolBar();
					m_planeToolBtn->setText(QString(tr("原始网格数据")));
				}
				else if (pickName == "最新动网格数据") {
					isVTKData = 0;
					updateAttributeNameMap(plt_dataSet);
					initAttributeSelToolBar();
					m_planeToolBtn->setText(QString(tr("最新动网格数据")));
				}
				else {
				}
			}

			void SceneWidget::initOperateSwitchToolBar()
			{
				//qDebug() << "initOperateSwitchToolBar!";
				m_mainTainOrBrowseButton = new platforms::detail::SwitchButton(this);
				m_mainTainOrBrowseButton->setFixedSize(120, 30);
				m_mainTainOrBrowseButton->setTextOn(u8"维护");
				m_mainTainOrBrowseButton->setTextOff(u8"浏览");
				m_mainTainOrBrowseButton->setStatusTip("选择表格管理方式(维护/浏览)");
				m_attributeToolBar->addWidget(m_mainTainOrBrowseButton);



				m_coverOrdumpButton = new platforms::detail::SwitchButton(this);
				m_coverOrdumpButton->setFixedSize(120, 30);
				m_coverOrdumpButton->setTextOn(u8"覆盖");
				m_coverOrdumpButton->setTextOff(u8"跳跃");
				m_coverOrdumpButton->setStatusTip("选择网格操作模式方式(覆盖/跳跃)");
				m_attributeToolBar->addWidget(m_coverOrdumpButton);
			}

			void SceneWidget::initLegendToolBar() {
				//动态加载“图例”工具栏
				if (m_legendMapper) {
					disconnect(m_legendMapper, SIGNAL(mapped(QString)), this, SLOT(legendSlot(QString)));
				}
				m_legendMapper = new QSignalMapper(this);
				const static vector<string> vecLegend = {
					"显示图例",
					"隐藏图例"
				};
				vector<QAction*> vecAction;
				for (auto& legend : vecLegend) {
					const QIcon icon = QIcon::fromTheme(tr(legend.c_str()), QIcon(("images/" + legend + ".png").c_str()));
					QAction* action = new QAction(icon, tr(legend.c_str()), this);
					action->setStatusTip(tr(legend.c_str()));
					connect(action, SIGNAL(triggered()), m_legendMapper, SLOT(map()));
					m_legendMapper->setMapping(action, QString(legend.c_str()));
					vecAction.push_back(action);
				}
				connect(m_legendMapper, SIGNAL(mapped(QString)), this, SLOT(legendSlot(QString)));

				// 创建QMenu
				QMenu* menu = new QMenu(this);
				for (auto& action : vecAction) {
					menu->addAction(action);
				}

				// 创建QToolButton
				if (!m_legendToolBtn) {
					m_legendToolBtn = new QToolButton(this);
					m_attributeToolBar->addWidget(m_legendToolBtn);
					m_legendToolBtn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
					m_legendToolBtn->setPopupMode(QToolButton::InstantPopup);
				}
				m_legendToolBtn->setIcon(QIcon::fromTheme(tr("图例状态"),
					QIcon("images/legend.png")));
				m_legendToolBtn->setText(QString(tr("图例状态")));

				// 为QToolButton设置菜单
				m_legendToolBtn->setMenu(vecAction.empty() ? nullptr : menu);
			}
			void SceneWidget::initClipWindowVisibleToolBar()
			{
				m_clipWindowVisible = new QPushButton(u8"显示切割窗口");
				//QString sheet = "border-radius:10px;color:#0000dd;font-family:Microsoft Yahei;background-color:grey;";
				QString fileName = appComDir + "/images/SSPNS/CSS/detail/CusButton.css";
				//D:\\BUPT\\testDev\reconfiguration\\Dev701QtNSPlatformBase\\binForPublic\images\SSPNS\CSS\detail
				QString sheet = FIlEINSTANCE->readFile(fileName);
				sheet += "background-color:lightgrey";
				m_clipWindowVisible->setStyleSheet(sheet);
				if(m_nsFlag==0)m_attributeToolBar->addWidget(m_clipWindowVisible);
				
			}
			void SceneWidget::initClipWindowInvisibleToolBar()
			{
				m_clipWindowInvisible = new QPushButton(u8"取消切割窗口");
				//QString sheet = "border-radius:10px;color:#0000dd;font-family:Microsoft Yahei;background-color:grey;";
				QString fileName = appComDir + "/images/SSPNS/CSS/detail/CusButton.css";
				//D:\\BUPT\\testDev\reconfiguration\\Dev701QtNSPlatformBase\\binForPublic\images\SSPNS\CSS\detail
				QString sheet = FIlEINSTANCE->readFile(fileName);
				sheet += "background-color:lightgrey";
				m_clipWindowInvisible->setStyleSheet(sheet);
				if (m_nsFlag == 0)m_attributeToolBar->addWidget(m_clipWindowInvisible);
				
			}
			void SceneWidget::initClipToolBar()
			{
				m_btnClip = new QPushButton(u8"切割");
				//QString sheet = "border-radius:10px;color:#0000dd;font-family:Microsoft Yahei;background-color:grey;";
				QString fileName = appComDir + "/images/SSPNS/CSS/detail/CusButton.css";
				//D:\\BUPT\\testDev\reconfiguration\\Dev701QtNSPlatformBase\\binForPublic\images\SSPNS\CSS\detail
				QString sheet = FIlEINSTANCE->readFile(fileName);
				sheet += "background-color:lightgrey";
				m_btnClip->setStyleSheet(sheet);
				if (m_nsFlag == 0)m_attributeToolBar->addWidget(m_btnClip);
				
			}
			void SceneWidget::initReClipToolBar()
			{
				m_btnReClip = new QPushButton(u8"重新切割");
				//QString sheet = "border-radius:10px;color:#0000dd;font-family:Microsoft Yahei;background-color:grey;";
				QString fileName = appComDir + "/images/SSPNS/CSS/detail/CusButton.css";
				//D:\\BUPT\\testDev\reconfiguration\\Dev701QtNSPlatformBase\\binForPublic\images\SSPNS\CSS\detail
				QString sheet = FIlEINSTANCE->readFile(fileName);
				sheet += "background-color:lightgrey";
				m_btnReClip->setStyleSheet(sheet);
				if (m_nsFlag == 0)m_attributeToolBar->addWidget(m_btnReClip);

			}
			void SceneWidget::initClipSplitBar()
			{
				m_btnClipSplit = new QPushButton(u8"切割(头尾同时显示)");
				//QString sheet = "border-radius:10px;color:#0000dd;font-family:Microsoft Yahei;background-color:grey;";
				QString fileName = appComDir + "/images/SSPNS/CSS/detail/CusButton.css";
				//D:\\BUPT\\testDev\reconfiguration\\Dev701QtNSPlatformBase\\binForPublic\images\SSPNS\CSS\detail
				QString sheet = FIlEINSTANCE->readFile(fileName);
				m_btnClipSplit->setStyleSheet(sheet);
				//m_attributeToolBar->addWidget(m_btnClipSplit);
				
			}
			void SceneWidget::initColorStopBar()
			{
				m_btnColorStop = new QPushButton(u8"停止染色");
				//QString sheet = "border-radius:10px;color:#0000dd;fonfamily:Microsoft Yahei;background-color:grey;";
				QString fileName = appComDir + "/images/SSPNS/CSS/detail/CusButton.css";
				//D:\\BUPT\\testDev\reconfiguration\\Dev701QtNSPlatformBase\\binForPublic\images\SSPNS\CSS\detail
				QString sheet = FIlEINSTANCE->readFile(fileName);
				m_btnColorStop->setStyleSheet(sheet);
				m_attributeToolBar->addWidget(m_btnColorStop);
			}
			
			void SceneWidget::sort_to_getI() {
				//tagI就是全局的I，如果已经tagI!=NULL，证明已经排完序了
				if (tagI != NULL) return;
				firstI* I1 = NULL;
				lastI* I2 = NULL;
				I1 = new firstI[n_blocks];
				I2 = new lastI[n_blocks];
				vector<vector<int>> edge;
				edge.resize(n_blocks);
				//第一列和最后一列的数据当min和max
				/*for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					ni = bk[id].ni;
					nj = bk[id].nj;
					nk = bk[id].nk;
					for (int i = 0; i < ni; i+=ni-1) {
						double min_x = 0, max_x = 0;
						min_x = *bk[id].X(i, 0, 0);
						max_x = *bk[id].X(i, 0, 0);
						for (int j = 0; j < nj; j++) {
							for (int k = 0; k < nk; k++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								double x = *X, y = *Y, z = *Z;
								if (x < min_x) min_x = x;
								if (x > max_x) max_x = x;

							}
						}
						if (i == 0) {
							I1[id].id = id;
							I1[id].x = (min_x + max_x) / 2;
						}
						if (i == ni - 1) {
							I2[id].id = id;
							I2[id].x = (min_x + max_x) / 2;
						}
					}
				}*/
				//qDebug() << "get Min and max";
				//整个网格整体的min和max当成min和max
				for (int i = 0; i < n_blocks; i++) {
					bk[i].get_info();
					I1[i].id = i;
					I2[i].id = i;
					I1[i].x = bk[i].min_x;
					I2[i].x = bk[i].max_x;
				}
				//qDebug() << "begin sort I2";
				//sort(I1, I1 + n_blocks);
				sort(I2, I2 + n_blocks);
				//qDebug() << "begin memset";
				int *father = NULL;
				int *deep = NULL;
				int *degree = NULL;
				father = new int[n_blocks];
				::memset(father, -1, sizeof(int)*n_blocks);
				//deep数组记载着offset
				deep = new int[n_blocks];
				::memset(deep, -1, sizeof(int)*n_blocks);

				degree = new int[n_blocks] {0};
				/*for (int i = 0; i < n_blocks; i++) {
					cout << "i=" << i << " I1.x=" << I1[i].x << "I1.id=" << I1[i].id<<endl;
				}
				for (int i = 0; i < n_blocks; i++) {
					cout << "i=" << i << " I2.x=" << I2[i].x << "I2.id=" << I2[i].id << endl;
				}*/
				//qDebug() << "begin get father and degree";
				for (int i = 0; i < n_blocks; i++) {
					double x = I1[i].x;
					//0.5是防止出现I2[0].x刚好比x大一丢丢，程序认为此次i应该是第一段
					//但实际上两者应该是相等的，i应该是紧跟着I2[0]对应的段
					if (I2[0].x - 0.5 > x) continue;
					double lineEps = abs(I2[0].x - x);
					father[i] = I2[0].id;
					for (int j = 0; j < n_blocks; j++) {
						if (abs(I2[j].x - x) < lineEps) {
							lineEps = abs(I2[j].x - x);
							father[i] = I2[j].id;
						}
					}
					/*cout << "father[i]=" << father[i] << endl;
					cout << "edge.size="<<edge.size() << endl;*/
					edge[father[i]].push_back(i);
					degree[i]++;
				}
				//方法1
				/*for (int i = 0; i < n_blocks; i++) {
					get_deep(i, father, deep);
				}
				for (int i = 0; i < n_blocks; i++) {
					bk[i].sectOffset = deep[i];
				}
				*/
				//方法2 拓扑排序
				tagI = new int[n_blocks] {0};

				queue<int> que;
				for (int i = 0; i < n_blocks; i++) {
					if (degree[i] == 0) {
						que.push(i);
						bk[i].sectOffset = 0;
					}
				}
				while (!que.empty()) {
					int x = que.front();
					que.pop();
					for (int i = 0; i < edge[x].size(); i++) {
						int y = edge[x][i];
						bk[y].sectOffset = bk[x].sectOffset + bk[x].ni - 1;
						que.push(y);
					}
				}
				for (int i = 0; i < n_blocks; i++) {
					tagI[i] = bk[i].sectOffset;
				}
				sort(tagI, tagI + n_blocks);
				for (int i = 1; i < n_blocks; i++) {
					if (tagI[i] == tagI[i - 1]) tagI[i - 1] = 0;
				}
				sort(tagI, tagI + n_blocks);
				int num0 = 0;
				for (int i = 0; i < n_blocks; i++) {
					if (tagI[i] == 0) num0++;
				}
				for (int i = num0; i < n_blocks; i++) {
					tagI[i - num0] = tagI[i];
					tagI[i] = 0;
				}
				//上述操作处理后，使得前面的值全是有效值，后面的值全是0


				//print_info("");
				for (int i = 0; i < n_blocks; i++) {
					//cout << "i=" << i << " sectOffset=" << bk[i].sectOffset << endl;
					bk[i].get_info();
					//cout << "min_X=" << bk[i].min_x << " max_x=" << bk[i].max_x << endl;
				}
				//qDebug() << "begin get_lineInfo";
				get_lineInfo();
				//qDebug() << "end get_lineInfo";
				for (int i = 0; i < n_blocks; i++) {
					//cout << "i=" << i << " tagI=" << tagI[i] << endl;
					if (tagI[i] == 0) continue;
					int id = gridLine->pointList[tagI[i]][0];
					point* temp = &(un_grid->pointData[id]);
					//cout << "x=" << temp->x << " y=" << temp->y << " z=" << temp->z << endl;
				}
				for (int i = 0; i < n_blocks; i++) {
					//cout << "i=" << i << " beginI=" << bk[i].sectOffset << " endI=" << bk[i].sectOffset + bk[i].ni - 1;
					bk[i].get_info();
					//cout << " min_x=" << bk[i].min_x << " max_x=" << bk[i].max_x << endl;
				}
				delete[]degree;
				delete[]father;
				delete[]deep;
				delete[]I1;
				delete[]I2;
			}
			void SceneWidget::slotBothClip() {
				m_bothClipFlag = true;
				slotClip();
				m_bothClipFlag = false;
			}
			void SceneWidget::slotClip() {
				int _index = myStatus->getClipIndex();
				int _dire = myStatus->getClipDire();
				if (clipIndex == NULL) {
					clipIndex = &_index;
				}
				planeWidget->EnabledOff();
				/*planeWidget->EnabledOff();
				planeWidget->RemoveAllObservers();
				m_pQVTKWidget->update();*/

				clip_out(tagI[*clipIndex]);
				qDebug() << "clipIndex="<<*clipIndex << " tagI[*clipIndex]=" << tagI[*clipIndex];
				//emit SignalInstance::getInstance()->signalPrintInfo("clip finish");
				cancelShowClip();
				mouseStyle->windowClipFlag = 0;
				if (_dire == -1) {
					if (m_bothClipFlag) {
						m_fullGridReadFlag = true;
						readAndOut(m_bothClipPath, SignalInstance::getInstance()->getWorkSpace());

					}
					else if (m_rightClipFlag) { 
						m_fullGridReadFlag = false;
						myStatus->setClipDire(0);
						readAndOut(m_bodyClipPath, SignalInstance::getInstance()->getWorkSpace());
					}
					else
					{
						m_fullGridReadFlag = true;
						myStatus->setClipDire(1);
						readAndOut(m_headClipPath, SignalInstance::getInstance()->getWorkSpace());
					}
				}
				else if (_dire == 0) {
					m_fullGridReadFlag = false;
					myStatus->setClipDire(0);
					readAndOut(m_bodyClipPath, SignalInstance::getInstance()->getWorkSpace());
				}
				else if (_dire == 1) {
					m_fullGridReadFlag = true;
					myStatus->setClipDire(1);
					readAndOut(m_headClipPath, SignalInstance::getInstance()->getWorkSpace());
				}
				
				emit SignalInstance::getInstance()->signalExeSect();
				
			}
			
			void SceneWidget::clip_out(int I) {
				//if (headGrid == NULL || bodyGrid == NULL) return;
				m_headClipPath = SignalInstance::getInstance()->getWorkSpace() + "/clip_head.grd";
				FILE* input = fopen(m_headClipPath.toLocal8Bit().toStdString().c_str(), "w");
				QString datPath = SignalInstance::getInstance()->getWorkSpace() + "/head_xyz_ijk.dat";
				FILE* datInput = fopen(datPath.toLocal8Bit().toStdString().c_str(), "w");
				fprintf(datInput, "//格式为总共有多少block  \n//整体网格原来的bolckId  在该文件中的的blockid\n//x,y,z,i,j,k 每个点一行\n");
				if (input == NULL) {
					return;
				}
				clip_flag = new int[n_blocks] {0};
				clip_index = new pair<int, int>[n_blocks];
				//qDebug() << "clip_index_init";
				clip_I = new int[n_blocks] {0};
				double* buffer = NULL;
				int buffer_size = 0;
				int index = 0;
				for (int i = 0; i < n_blocks; i++) {
					int ni, nj, nk;
					ni = bk[i].ni;
					nj = bk[i].nj;
					nk = bk[i].nk;
					if (ni*nj*nk > buffer_size) buffer_size = ni * nj*nk;
				}
				buffer = new double[buffer_size + 10]{ 0 };
				int num = 0;
				int num2 = 0;
				//有部分在头部
				for (int i = 0; i < n_blocks; i++) {
					if (bk[i].sectOffset < I && bk[i].sectOffset + bk[i].ni - 1 > I) {
						clip_flag[i] = 3;
						clip_index[i].first = index;
						index++;
						num++;
					}
					else if (bk[i].sectOffset + bk[i].ni - 1 <= I) {
						clip_flag[i] = 1;
						clip_index[i].first = index;
						index++;
						num++;
					}
				}
				fprintf(input, "       %d\n", num);
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					if (bk[id].sectOffset < I && bk[id].sectOffset + bk[id].ni - 1> I) {
						ni = I - bk[id].sectOffset + 1;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else if (bk[id].sectOffset + bk[id].ni - 1 <= I) {
						ni = bk[id].ni;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else continue;
					bool flag = false;

					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								if (bk[id].sectOffset + i != I) continue;
								flag = true;
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								//fprintf(datInput, " %.16e  %.16e  %.16e %d %d %d\n", *X,*Y,*Z ,i+1,j+1,k+1);
								//fprintf(datInput, " %.16e  %.16e  %.16e\n", *X, *Y, *Z);
							}
						}
					}
					if (!flag) continue;
					num2++;


				}

				fprintf(datInput, "       %d\n", num2);
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					if (bk[id].sectOffset < I && bk[id].sectOffset + bk[id].ni - 1> I) {
						ni = I - bk[id].sectOffset + 1;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else if (bk[id].sectOffset + bk[id].ni - 1 <= I) {
						ni = bk[id].ni;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else continue;
					bool flag = false;
					
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								if (bk[id].sectOffset + i != I) continue;
								flag = true;
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								//fprintf(datInput, " %.16e  %.16e  %.16e %d %d %d\n", *X,*Y,*Z ,i+1,j+1,k+1);
								//fprintf(datInput, " %.16e  %.16e  %.16e\n", *X, *Y, *Z);
							}
						}
					}
					if (!flag) continue;
					fprintf(datInput, "       %d  %d %d %d\n", id + 1, clip_index[id].first + 1, nj, nk);
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								if (bk[id].sectOffset + i != I) continue;
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								//fprintf(datInput, " %.16e  %.16e  %.16e %d %d %d\n", *X,*Y,*Z ,i+1,j+1,k+1);
								fprintf(datInput, " %.16e  %.16e  %.16e\n", *X, *Y, *Z);
							}
						}
					}
				}

				for (int i = 0; i < n_blocks; i++) {
					if (bk[i].sectOffset < I && bk[i].sectOffset + bk[i].ni - 1> I) {
						clip_I[i] = I - bk[i].sectOffset + 1;
						fprintf(input, "      %d      %d       %d\n", I - bk[i].sectOffset + 1, bk[i].nj, bk[i].nk);
					}
					else if (bk[i].sectOffset + bk[i].ni - 1 <= I) {

						fprintf(input, "      %d      %d       %d\n", bk[i].ni, bk[i].nj, bk[i].nk);
					}
				}

				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					if (bk[id].sectOffset < I && bk[id].sectOffset + bk[id].ni - 1> I) {
						ni = I - bk[id].sectOffset + 1;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else if (bk[id].sectOffset + bk[id].ni - 1 <= I) {
						ni = bk[id].ni;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else continue;
					int buffer_index = 0;

					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								buffer[buffer_index++] = *X;
							}
						}
					}
					for (int i = 0; i < buffer_index / 4; i++) {
						fprintf(input, " %.16e  %.16e  %.16e  %.16e \n", buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2], buffer[i * 4 + 3]);
					}
					if (buffer_index % 4 != 0) {
						for (int i = 0; i < buffer_index % 4; i++) {
							fprintf(input, " %.16e", buffer[buffer_index / 4 * 4 + i]);
						}
						fprintf(input, " \n");
					}
					buffer_index = 0;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								buffer[buffer_index++] = *Y;
							}
						}
					}
					for (int i = 0; i < buffer_index / 4; i++) {
						fprintf(input, " %.16e  %.16e  %.16e  %.16e \n", buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2], buffer[i * 4 + 3]);
					}
					if (buffer_index % 4 != 0) {
						for (int i = 0; i < buffer_index % 4; i++) {
							fprintf(input, " %.16e", buffer[buffer_index / 4 * 4 + i]);
						}
						fprintf(input, " \n");
					}
					buffer_index = 0;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = 0; i < ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								buffer[buffer_index++] = *Z;
							}
						}
					}
					for (int i = 0; i < buffer_index / 4; i++) {
						fprintf(input, " %.16e  %.16e  %.16e  %.16e \n", buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2], buffer[i * 4 + 3]);
					}
					if (buffer_index % 4 != 0) {
						for (int i = 0; i < buffer_index % 4; i++) {
							fprintf(input, " %.16e", buffer[buffer_index / 4 * 4 + i]);
						}
						fprintf(input, " \n");
					}

				}
				fclose(input);
				fclose(datInput);
				//body
				m_bodyClipPath = SignalInstance::getInstance()->getWorkSpace() + "/clip_body.grd";
				input = fopen(m_bodyClipPath.toLocal8Bit().toStdString().c_str() , "w");
				datPath = SignalInstance::getInstance()->getWorkSpace() + "/body_xyz_ijk.dat";
				datInput = fopen(datPath.toLocal8Bit().toStdString().c_str(), "w");
				fprintf(datInput, "//格式为总共有多少block  \n//整体网格原来的bolckId  在该文件中的的blockid\n//x,y,z,i,j,k 每个点一行\n");
				if (input == NULL) {
					return;
				}
				num = 0;
				num2 = 0;
				index = 0;
				//有部分在身部
				for (int i = 0; i < n_blocks; i++) {
					//cout << "i="<<i<<" bk[i].ni=" << bk[i].ni<<" sectoff="<<bk[i].sectOffset << endl;
					if (bk[i].sectOffset < I && bk[i].sectOffset + bk[i].ni - 1> I) {
						clip_flag[i] = 3;
						clip_index[i].second = index;
						index++;
						num++;
					}
					else if (bk[i].sectOffset >= I) {
						clip_flag[i] = 2;
						clip_index[i].second = index;
						index++;
						num++;
					}
				}
				fprintf(input, "       %d\n", num);
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					//这里ni是起点
					if (bk[id].sectOffset < I && bk[id].sectOffset + bk[id].ni - 1> I) {
						ni = I - bk[id].sectOffset;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else if (bk[id].sectOffset >= I) {
						ni = 0;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else continue;
					//fprintf(datInput, "       %d  %d\n", id + 1, clip_index[id].second + 1);
					bool flag = false;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = ni; i < bk[id].ni; i++) {
								if (bk[id].sectOffset + i != I) continue;
								flag = true;
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								//fprintf(datInpu t, " %.16e  %.16e  %.16e %d %d %d\n", *X, *Y, *Z, i-ni+1, j+1, k+1);
								//fprintf(datInput, " %.16e  %.16e  %.16e\n", *X, *Y, *Z);
							}
						}
					}
					if (!flag) continue;
					num2++;
				}

				fprintf(datInput, "       %d\n", num2);
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					//这里ni是起点
					if (bk[id].sectOffset < I && bk[id].sectOffset + bk[id].ni - 1> I) {
						ni = I - bk[id].sectOffset;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else if (bk[id].sectOffset >= I) {
						ni = 0;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else continue;
					//fprintf(datInput, "       %d  %d\n", id + 1, clip_index[id].second + 1);
					bool flag = false;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = ni; i < bk[id].ni; i++) {
								if (bk[id].sectOffset + i != I) continue;
								flag = true;
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								//fprintf(datInpu t, " %.16e  %.16e  %.16e %d %d %d\n", *X, *Y, *Z, i-ni+1, j+1, k+1);
								//fprintf(datInput, " %.16e  %.16e  %.16e\n", *X, *Y, *Z);
							}
						}
					}
					if (!flag) continue;
					fprintf(datInput, "       %d  %d %d %d\n", id + 1, clip_index[id].second + 1, nj, nk);
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = ni; i < bk[id].ni; i++) {
								if (bk[id].sectOffset + i != I) continue;
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								//fprintf(datInput, " %.16e  %.16e  %.16e %d %d %d\n", *X, *Y, *Z, i-ni+1, j+1, k+1);
								fprintf(datInput, " %.16e  %.16e  %.16e\n", *X, *Y, *Z);
							}
						}
					}
				}
				for (int id = 0; id < n_blocks; id++) {
					int ni, nj, nk;
					//这里ni是起点
					if (bk[id].sectOffset < I && bk[id].sectOffset + bk[id].ni - 1> I) {
						ni = I - bk[id].sectOffset;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else if (bk[id].sectOffset >= I) {
						ni = 0;
						nj = bk[id].nj;
						nk = bk[id].nk;
					}
					else continue;
					if (bk[id].sectOffset < I && bk[id].sectOffset + bk[id].ni - 1> I) {

						fprintf(input, "      %d      %d       %d\n", bk[id].sectOffset + bk[id].ni - I, bk[id].nj, bk[id].nk);
					}
					else if (bk[id].sectOffset >= I) {

						fprintf(input, "      %d      %d       %d\n", bk[id].ni, bk[id].nj, bk[id].nk);
					}

					int buffer_index = 0;
					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = ni; i < bk[id].ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								buffer[buffer_index++] = *X;
							}

						}
					}

					for (int i = 0; i < buffer_index / 4; i++) {

						fprintf(input, " %.16e  %.16e  %.16e  %.16e \n", buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2], buffer[i * 4 + 3]);
					}
					if (buffer_index % 4 != 0) {
						for (int i = 0; i < buffer_index % 4; i++) {
							fprintf(input, " %.16e", buffer[buffer_index / 4 * 4 + i]);

						}
						fprintf(input, " \n");
					}

					buffer_index = 0;

					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = ni; i < bk[id].ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								buffer[buffer_index++] = *Y;
							}
						}
					}
					for (int i = 0; i < buffer_index / 4; i++) {

						fprintf(input, " %.16e  %.16e  %.16e  %.16e \n", buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2], buffer[i * 4 + 3]);
					}
					if (buffer_index % 4 != 0) {
						for (int i = 0; i < buffer_index % 4; i++) {
							fprintf(input, " %.16e", buffer[buffer_index / 4 * 4 + i]);

						}
						fprintf(input, " \n");
					}

					buffer_index = 0;

					for (int k = 0; k < nk; k++) {
						for (int j = 0; j < nj; j++) {
							for (int i = ni; i < bk[id].ni; i++) {
								double *X = bk[id].X(i, j, k), *Y = bk[id].Y(i, j, k), *Z = bk[id].Z(i, j, k);
								buffer[buffer_index++] = *Z;
							}
						}
					}
					for (int i = 0; i < buffer_index / 4; i++) {

						fprintf(input, " %.16e  %.16e  %.16e  %.16e \n", buffer[i * 4], buffer[i * 4 + 1], buffer[i * 4 + 2], buffer[i * 4 + 3]);
					}
					if (buffer_index % 4 != 0) {
						for (int i = 0; i < buffer_index % 4; i++) {
							fprintf(input, " %.16e", buffer[buffer_index / 4 * 4 + i]);

						}
						fprintf(input, " \n");
					}

				}
				fclose(datInput);
				fclose(input);
				input = NULL;
				datInput = NULL;

				
				delete[]buffer;
				/// 文件自动生成到workspace目录下，根据m_rightClipFlag进行文件拷贝
				QString toDirSect = SignalInstance::getInstance()->getWorkSpace() + "/sect";
				clip_out_inp(I);
				QElapsedTimer timer;
				timer.start(); // 开始计时
				int _dire = myStatus->getClipDire();
				if (_dire == -1) {
					if (m_rightClipFlag) {

						FIlEINSTANCE->copyFile(toDirSect, m_bodyClipPath, "grid.dat");
						FIlEINSTANCE->copyFile(toDirSect, m_bodyBdInpPath, "genericbd.inp");
						FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, m_headClipPath, "head_grid.grd");
						FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, m_bodyClipPath, "body_grid.grd");
					}
					else
					{
						FIlEINSTANCE->copyFile(toDirSect, m_headClipPath, "grid.dat");
						FIlEINSTANCE->copyFile(toDirSect, m_headBdInpPath, "genericbd.inp");
						FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, m_headClipPath, "body_grid.grd");
						FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, m_bodyClipPath, "head_grid.grd");
					}
				}
				else if (_dire == 0) {
					FIlEINSTANCE->copyFile(toDirSect, m_bodyClipPath, "grid.dat");
					FIlEINSTANCE->copyFile(toDirSect, m_bodyBdInpPath, "genericbd.inp");
					FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, m_headClipPath, "head_grid.grd");
					FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, m_bodyClipPath, "body_grid.grd");
				}
				else if (_dire == 1) {
					FIlEINSTANCE->copyFile(toDirSect, m_headClipPath, "grid.dat");
					FIlEINSTANCE->copyFile(toDirSect, m_headBdInpPath, "genericbd.inp");
					FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, m_headClipPath, "body_grid.grd");
					FIlEINSTANCE->copyFile(SignalInstance::getInstance()->m_workspace, m_bodyClipPath, "head_grid.grd");
				}
				
				qint64 elapsed = timer.elapsed(); // 获取经过的毫秒数
				qDebug() << "copyFile in milliseconds:" << elapsed;
				QFile headFile(m_headClipPath), bodyFile(m_bodyClipPath);
				if (headFile.exists()) { // 检查文件是否存在
					if (headFile.remove()) { // 尝试删除文件
						qDebug() << "headFile deleted successfully.";
					}
					else {
						qDebug() << "Failed to delete the headFile.";
					}
				}
				if (bodyFile.exists()) { // 检查文件是否存在
					if (bodyFile.remove()) { // 尝试删除文件
						qDebug() << "bodyFile deleted successfully.";
					}
					else {
						qDebug() << "Failed to delete the bodyFile.";
					}
				}
				if (_dire == -1) {
					if (m_rightClipFlag) {
						m_headClipPath = SignalInstance::getWorkSpace() + "/head_grid.grd";
						m_bodyClipPath = SignalInstance::getWorkSpace() + "/body_grid.grd";
					}
					else {
						m_headClipPath = SignalInstance::getWorkSpace() + "/body_grid.grd";
						m_bodyClipPath = SignalInstance::getWorkSpace() + "/head_grid.grd";
					}
				}
				else if (_dire == 0) {
					m_headClipPath = SignalInstance::getWorkSpace() + "/head_grid.grd";
					m_bodyClipPath = SignalInstance::getWorkSpace() + "/body_grid.grd";
				}
				else if (_dire == 1) {
					m_headClipPath = SignalInstance::getWorkSpace() + "/body_grid.grd";
					m_bodyClipPath = SignalInstance::getWorkSpace() + "/head_grid.grd";
				}
				
				
			}
			void SceneWidget::processFile(const std::string& inputPath, const std::string& outputPath) {
				std::ifstream inFile(inputPath);
				std::ofstream outFile(outputPath);

				if (!inFile || !outFile) {
					std::cerr << "无法打开文件" << std::endl;
					return;
				}

				std::string line;
				while (std::getline(inFile, line)) {
					// 检查这一行是否是7列数据行
					if (line.find("      ") == 0) {  // 以6个空格开头的行是数据行
						static int prevLastCol = 0;  // 保存上一行的最后一列值
						std::vector<double> row(7);
						std::istringstream iss(line);

						// 读取这一行的7个数
						for (int j = 0; j < 7; j++) {
							iss >> row[j];
						}

						// 检查条件并处理
						if (row[6] > 0 && prevLastCol != -1) {
							// 取所有数的绝对值
							for (int j = 0; j < 7; j++) {
								row[j] = std::abs(row[j]);
							}
						}

						// 保存当前行的最后一列值
						prevLastCol = row[6];

						// 写入处理后的数据，保持原格式
						for (int j = 0; j < 7; j++) {
							outFile << "      " << row[j];
						}
						outFile << "\n";
					}
					else {
						// 非数据行直接输出
						outFile << line << "\n";
					}
				}

				inFile.close();
				outFile.close();
				

			}

			void SceneWidget::clip_out_inp(int I)
			{
				if (clip_index == NULL || clip_flag == NULL || clip_I == NULL) return;
				//m_fullInpPath = SignalInstance::getInstance()->getWorkSpace() + "/full_bd.inp";
				m_fullInpPath = SignalInstance::getInstance()->getWorkSpace() + "/" + SignalInstance::getInstance()->getInpName();
				ifstream input(m_fullInpPath.toLocal8Bit().toStdString().c_str(), ios::in);
				if (!input.is_open()) {
					return;
				}
				string* bc_name = NULL;
				bcInfo **bcPool = NULL;
				int *regionNum = NULL;
				int blockNum = 0;
				input >> blockNum;
				input >> blockNum;
				bcPool = new bcInfo*[blockNum] {NULL};
				regionNum = new int[blockNum] {0};
				bc_name = new string[blockNum];
				//cout << "blockNum=" << blockNum << endl;
				for (int id = 0; id < blockNum; id++) {
					int i, j, k;
					input >> i >> j >> k;

					//string blockName;
					//input >> blockName;
					input >> bc_name[id];

					input >> regionNum[id];
					bcPool[id] = new bcInfo[regionNum[id]];
					for (int line = 0; line < regionNum[id]; line++) {
						input >> bcPool[id][line].minI >> bcPool[id][line].maxI
							>> bcPool[id][line].minJ >> bcPool[id][line].maxJ
							>> bcPool[id][line].minK >> bcPool[id][line].maxK
							>> bcPool[id][line].bc_num;

						if (bcPool[id][line].bc_num < 0) {
							input >> bcPool[id][line].minI2 >> bcPool[id][line].maxI2
								>> bcPool[id][line].minJ2 >> bcPool[id][line].maxJ2
								>> bcPool[id][line].minK2 >> bcPool[id][line].maxK2
								>> bcPool[id][line].targetId;
							bcPool[id][line].targetId--;
							/*cout << "id=" << id << " line=" << line << endl;
							cout << bcPool[id][line].minI2<<" " << bcPool[id][line].maxI2 << " "
								<< bcPool[id][line].minJ2 << " " << bcPool[id][line].maxJ2 << " "
								<< bcPool[id][line].minK2 << " " << bcPool[id][line].maxK2 << " "
								<< bcPool[id][line].targetId<<endl;*/
						}

					}
				}
				input.close();
				
				m_headBdInpPath = SignalInstance::getInstance()->getWorkSpace() + "/head_bd_before.inp";
				FILE* output = fopen(m_headBdInpPath.toLocal8Bit().toStdString().c_str(), "w");
				if (output == NULL) {
					//cout << "error" << endl;
					return;
				}
				int num = 0;
				for (int i = 0; i < n_blocks; i++) {
					if (clip_flag[i] == 1 || clip_flag[i] == 3) num++;
				}
				fprintf(output, "     1\n");
				fprintf(output, "    %d\n", num);
				for (int i = 0; i < n_blocks; i++) {
					qDebug() << "head i=" << i << " clip_flag=" << clip_flag[i];
				}
				for (int i = 0; i < n_blocks; i++) {
					if (clip_flag[i] == 1) {//切分后整体都在head中
						fprintf(output, "    %d     %d     %d\n", bk[i].ni, bk[i].nj, bk[i].nk);
						fprintf(output, "%s\n", bc_name[i].c_str());
						//fprintf(output, "%s\n", 'A'+clip_index[i].first);
						int count = 0;
						fprintf(output, "     %d\n", regionNum[i]);
						for (int line = 0; line < regionNum[i]; line++) {

							if (bcPool[i][line].bc_num < 0) {//和其他block有共同点的情况
								int id = bcPool[i][line].targetId;
								if (clip_flag[id] == 1) {//target区域也全部在head中
									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										bcPool[i][line].minI, bcPool[i][line].maxI,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK,
										bcPool[i][line].bc_num);
									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										bcPool[i][line].minI2, bcPool[i][line].maxI2,
										bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
										bcPool[i][line].minK2, bcPool[i][line].maxK2,
										clip_index[id].first + 1);
								}
								else if (clip_flag[id] == 3) {//target区域部分在head中
									int l2 = bcPool[i][line].minI2, r2 = bcPool[i][line].maxI2;
									if (l2 < 0) l2 = -l2;
									if (r2 < 0) r2 = -r2;
									if (l2 > r2) swap(l2, r2);
									if (clip_I[id] < l2) {//此时对应区域不在head中,设置为outflow
										fprintf(output, "      %d        %d        %d       %d        %d        %d        6\n",
											bcPool[i][line].minI, bcPool[i][line].maxI,
											bcPool[i][line].minJ, bcPool[i][line].maxJ,
											bcPool[i][line].minK, bcPool[i][line].maxK);

									}
									else { //此时对应区域部分在head中
										fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
											bcPool[i][line].minI, bcPool[i][line].maxI,
											bcPool[i][line].minJ, bcPool[i][line].maxJ,
											bcPool[i][line].minK, bcPool[i][line].maxK,
											bcPool[i][line].bc_num);
										r2 = min(r2, clip_I[id]);
										if (bcPool[i][line].minI2 < 0) l2 = -l2;
										if (bcPool[i][line].maxI2 < 0) r2 = -r2;
										if (l2<r2&&bcPool[i][line].minI2>bcPool[i][line].maxI2) swap(l2, r2);
										if (l2 > r2&&bcPool[i][line].minI2 < bcPool[i][line].maxI2) swap(l2, r2);
										fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
											l2, r2,
											bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
											bcPool[i][line].minK2, bcPool[i][line].maxK2,
											clip_index[id].first + 1);
									}
									/*fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										bcPool[i][line].minI, bcPool[i][line].maxI,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK,
										bcPool[i][line].bc_num);
									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										bcPool[i][line].minI2, bcPool[i][line].maxI2,
										bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
										bcPool[i][line].minK2, bcPool[i][line].maxK2,
										clip_index[id].first);*/
								}
								else if (clip_flag[id] == 2) {//target区域根本不在head中，将该区域设为outflow,即bc_num=6
									fprintf(output, "      %d        %d        %d       %d        %d        %d        6\n",
										bcPool[i][line].minI, bcPool[i][line].maxI,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK);
								}
							}
							else {//bc_num>0时，原来什么样，现在就什么样
								fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
									bcPool[i][line].minI, bcPool[i][line].maxI,
									bcPool[i][line].minJ, bcPool[i][line].maxJ,
									bcPool[i][line].minK, bcPool[i][line].maxK,
									bcPool[i][line].bc_num);
							}

						}

					}
					else if (clip_flag[i] == 3) {//切分后，一半在head，一半在body:head部分
						fprintf(output, "    %d     %d     %d\n", clip_I[i], bk[i].nj, bk[i].nk);
						fprintf(output, "%s\n", bc_name[i].c_str());
						//fprintf(output, "%c\n", 'A' + clip_index[i].first);

						int count = 1;//将完整的网格给一分为二的话，头部会多一个面，身部也会多一个面
						for (int line = 0; line < regionNum[i]; line++) {
							int l = bcPool[i][line].minI, r = bcPool[i][line].maxI;
							if (l < 0) l = -l;
							if (r < 0) r = -r;
							if (l > r) swap(l, r);
							if (clip_I[i] < l) continue;
							count++;
						}
						fprintf(output, "     %d\n", count);
						fprintf(output, "      %d        %d        1       %d        1        %d        6\n",
							clip_I[i], clip_I[i],bk[i].nj,bk[i].nk);//新的和body部分相接的面
						for (int line = 0; line < regionNum[i]; line++) {
							int l = bcPool[i][line].minI, r = bcPool[i][line].maxI;
							if (l < 0) l = -l;
							if (r < 0) r = -r;
							if (l > r) swap(l, r);
							if (clip_I[i] < l) continue;
							if (bcPool[i][line].bc_num < 0) {//连接区域
								int id = bcPool[i][line].targetId;
								if (clip_flag[id] == 1) {//target区域也全部在head中
									r = min(r, clip_I[i]);
									if (bcPool[i][line].minI < 0) l = -l;
									if (bcPool[i][line].maxI < 0) r = -r;
									if (l<r&&bcPool[i][line].minI>bcPool[i][line].maxI) swap(l, r);
									if (l > r&&bcPool[i][line].minI < bcPool[i][line].maxI) swap(l, r);
									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										l, r,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK,
										bcPool[i][line].bc_num);

									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										bcPool[i][line].minI2, bcPool[i][line].maxI2,
										bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
										bcPool[i][line].minK2, bcPool[i][line].maxK2,
										clip_index[id].first + 1);
								}
								else if (clip_flag[id] == 3) {//target区域部分在head中
									r = min(r, clip_I[i]);
									if (bcPool[i][line].minI < 0) l = -l;
									if (bcPool[i][line].maxI < 0) r = -r;
									if (l<r&&bcPool[i][line].minI>bcPool[i][line].maxI) swap(l, r);
									if (l > r&&bcPool[i][line].minI < bcPool[i][line].maxI) swap(l, r);
									/*fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										l, r,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK,
										bcPool[i][line].bc_num);*/
										//判断其对应面的情况
									int l2 = bcPool[i][line].minI2, r2 = bcPool[i][line].maxI2;
									if (l2 < 0) l2 = -l2;
									if (r2 < 0) r2 = -r2;
									if (l2 > r2) swap(l2, r2);
									if (clip_I[id] < l2) {//此时对应区域不在head中,设置为outflow
										fprintf(output, "      %d        %d        %d       %d        %d        %d        6\n",
											l, r,
											bcPool[i][line].minJ, bcPool[i][line].maxJ,
											bcPool[i][line].minK, bcPool[i][line].maxK);

									}
									else {
										fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
											l, r,
											bcPool[i][line].minJ, bcPool[i][line].maxJ,
											bcPool[i][line].minK, bcPool[i][line].maxK,
											bcPool[i][line].bc_num);
										r2 = min(r2, clip_I[id]);
										if (bcPool[i][line].minI2 < 0) l2 = -l2;
										if (bcPool[i][line].maxI2 < 0) r2 = -r2;
										if (l2<r2&&bcPool[i][line].minI2>bcPool[i][line].maxI2) swap(l2, r2);
										if (l2 > r2&&bcPool[i][line].minI2 < bcPool[i][line].maxI2) swap(l2, r2);
										fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
											l2, r2,
											bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
											bcPool[i][line].minK2, bcPool[i][line].maxK2,
											clip_index[id].first + 1);
									}

								}
								else if (clip_flag[id] == 2) {//target区域根本不在head中，将该区域设为outflow,即bc_num=6
									r = min(r, clip_I[i]);
									if (bcPool[i][line].minI < 0) l = -l;
									if (bcPool[i][line].maxI < 0) r = -r;
									if (l<r&&bcPool[i][line].minI>bcPool[i][line].maxI) swap(l, r);
									if (l > r&&bcPool[i][line].minI < bcPool[i][line].maxI) swap(l, r);
									fprintf(output, "      %d        %d        %d       %d        %d        %d        6\n",
										l, r,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK);
								}

							}
							else {//非连接区域
								r = min(r, clip_I[i]);
								if (bcPool[i][line].minI < 0) l = -l;
								if (bcPool[i][line].maxI < 0) r = -r;
								if (l<r&&bcPool[i][line].minI>bcPool[i][line].maxI) swap(l, r);
								if (l > r&&bcPool[i][line].minI < bcPool[i][line].maxI) swap(l, r);
								fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
									l, r,
									bcPool[i][line].minJ, bcPool[i][line].maxJ,
									bcPool[i][line].minK, bcPool[i][line].maxK,
									bcPool[i][line].bc_num);
							}
						}
					}
				}
				fclose(output);
				m_bodyBdInpPath = SignalInstance::getInstance()->getWorkSpace() + "/body_genericbd_before.inp";
				output = fopen(m_bodyBdInpPath.toLocal8Bit().toStdString().c_str(), "w");
				if (output == NULL) {
					//cout << "error" << endl;
					return;
				}
				num = 0;
				for (int i = 0; i < n_blocks; i++) {
					if (clip_flag[i] == 2 || clip_flag[i] == 3) num++;
				}
				fprintf(output, "     1\n");
				fprintf(output, "    %d\n", num);
				for (int i = 0; i < n_blocks; i++) {
					qDebug() << "body i=" << i << " clip_flag=" << clip_flag[i];
				}
				for (int i = 0; i < n_blocks; i++) {
					if (clip_flag[i] == 2) {//切分后整体都在body中
						fprintf(output, "    %d     %d     %d\n", bk[i].ni, bk[i].nj, bk[i].nk);
						fprintf(output, "%s\n", bc_name[i].c_str());
						//fprintf(output, "%c\n", 'A' + clip_index[i].second);
						int count = 0;
						fprintf(output, "     %d\n", regionNum[i]);
						for (int line = 0; line < regionNum[i]; line++) {

							if (bcPool[i][line].bc_num < 0) {//和其他block有共同区域的情况
								int id = bcPool[i][line].targetId;
								if (clip_flag[id] == 2) {//target区域也全部在body中
									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										bcPool[i][line].minI, bcPool[i][line].maxI,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK,
										bcPool[i][line].bc_num);
									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										bcPool[i][line].minI2, bcPool[i][line].maxI2,
										bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
										bcPool[i][line].minK2, bcPool[i][line].maxK2,
										clip_index[id].second + 1);
								}
								else if (clip_flag[id] == 3) {//target区域部分在body中
									int l2 = bcPool[i][line].minI2, r2 = bcPool[i][line].maxI2;
									if (l2 < 0) l2 = -l2;
									if (r2 < 0) r2 = -r2;
									if (l2 > r2) swap(l2, r2);
									if (clip_I[id] > r2) {//此时对应区域不在body中,设置为gen#1
										fprintf(output, "      %d        %d        %d       %d        %d        %d        8\n",
											bcPool[i][line].minI, bcPool[i][line].maxI,
											bcPool[i][line].minJ, bcPool[i][line].maxJ,
											bcPool[i][line].minK, bcPool[i][line].maxK);

									}
									else {
										fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
											bcPool[i][line].minI, bcPool[i][line].maxI,
											bcPool[i][line].minJ, bcPool[i][line].maxJ,
											bcPool[i][line].minK, bcPool[i][line].maxK,
											bcPool[i][line].bc_num);
										l2 = max(clip_I[id], l2);
										l2 = l2 - clip_I[id] + 1;
										r2 = r2 - clip_I[id] + 1;
										if (bcPool[i][line].minI2 < 0) l2 = -l2;
										if (bcPool[i][line].maxI2 < 0) r2 = -r2;
										if (l2<r2&&bcPool[i][line].minI2>bcPool[i][line].maxI2) swap(l2, r2);
										if (l2 > r2&&bcPool[i][line].minI2 < bcPool[i][line].maxI2) swap(l2, r2);
										fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
											l2, r2,
											bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
											bcPool[i][line].minK2, bcPool[i][line].maxK2,
											clip_index[id].second + 1);
									}
								}
								else if (clip_flag[id] == 1) {//target区域根本不在body中，将该区域设为Gneric #1,即bc_num=8
									fprintf(output, "      %d        %d        %d       %d        %d        %d        8\n",
										bcPool[i][line].minI, bcPool[i][line].maxI,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK);
								}
							}
							else {//bc_num>0时，原来什么样，现在就什么样
								fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
									bcPool[i][line].minI, bcPool[i][line].maxI,
									bcPool[i][line].minJ, bcPool[i][line].maxJ,
									bcPool[i][line].minK, bcPool[i][line].maxK,
									bcPool[i][line].bc_num);
							}

						}

					}
					else if (clip_flag[i] == 3) {//切分后，一半在head，一半在body:body部分
						fprintf(output, "    %d     %d     %d\n", bk[i].ni-clip_I[i]+1, bk[i].nj, bk[i].nk);
						//fprintf(output, "    %d     %d     %d\n", clip_I[i], bk[i].nj, bk[i].nk);
						//fprintf(output, "%c\n", 'A' + clip_index[i].second);
						fprintf(output, "%s\n", bc_name[i].c_str());
						int count = 1;
						for (int line = 0; line < regionNum[i]; line++) {
							int l = bcPool[i][line].minI, r = bcPool[i][line].maxI;
							if (l < 0) l = -l;
							if (r < 0) r = -r;
							if (l > r) swap(l, r);
							if (clip_I[i] > r) continue;
							count++;
						}
						fprintf(output, "     %d\n", count);
						fprintf(output, "      1        1        1       %d        1        %d        8\n",
							 bk[i].nj, bk[i].nk);//新的和head部分相接的面
						for (int line = 0; line < regionNum[i]; line++) {
							int l = bcPool[i][line].minI, r = bcPool[i][line].maxI;
							if (l < 0) l = -l;
							if (r < 0) r = -r;
							if (l > r) swap(l, r);
							if (clip_I[i] > r) continue;
							if (bcPool[i][line].bc_num < 0) {
								int id = bcPool[i][line].targetId;
								if (clip_flag[id] == 2) {//target区域也全部在body中
									l = max(l, clip_I[i]);
									l = l - clip_I[i] + 1;
									r = r - clip_I[i] + 1;
									if (bcPool[i][line].minI < 0) l = -l;
									if (bcPool[i][line].maxI < 0) r = -r;
									if (l<r&&bcPool[i][line].minI>bcPool[i][line].maxI) swap(l, r);
									if (l > r&&bcPool[i][line].minI < bcPool[i][line].maxI) swap(l, r);
									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										l, r,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK,
										bcPool[i][line].bc_num);

									fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										bcPool[i][line].minI2, bcPool[i][line].maxI2,
										bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
										bcPool[i][line].minK2, bcPool[i][line].maxK2,
										clip_index[id].second + 1);
								}
								else if (clip_flag[id] == 3) {//target区域部分在body中
									l = max(l, clip_I[i]);
									l = l - clip_I[i] + 1;
									r = r - clip_I[i] + 1;
									if (bcPool[i][line].minI < 0) l = -l;
									if (bcPool[i][line].maxI < 0) r = -r;
									if (l<r&&bcPool[i][line].minI>bcPool[i][line].maxI) swap(l, r);
									if (l > r&&bcPool[i][line].minI < bcPool[i][line].maxI) swap(l, r);
									/*fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
										l, r,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK,
										bcPool[i][line].bc_num);*/
										//判断其对应面的情况
									int l2 = bcPool[i][line].minI2, r2 = bcPool[i][line].maxI2;
									if (l2 < 0) l2 = -l2;
									if (r2 < 0) r2 = -r2;
									if (l2 > r2) swap(l2, r2);

									if (clip_I[id] > r2) {//此时对应区域不在body中,设置为Gneric #1
										fprintf(output, "      %d        %d        %d       %d        %d        %d        8\n",
											l, r,
											bcPool[i][line].minJ, bcPool[i][line].maxJ,
											bcPool[i][line].minK, bcPool[i][line].maxK);

									}
									else {
										fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
											l, r,
											bcPool[i][line].minJ, bcPool[i][line].maxJ,
											bcPool[i][line].minK, bcPool[i][line].maxK,
											bcPool[i][line].bc_num);

										l2 = max(clip_I[id], l2);
										l2 = l2 - clip_I[id] + 1;
										r2 = r2 - clip_I[id] + 1;
										if (bcPool[i][line].minI2 < 0) l2 = -l2;
										if (bcPool[i][line].maxI2 < 0) r2 = -r2;
										if (l2<r2&&bcPool[i][line].minI2>bcPool[i][line].maxI2) swap(l2, r2);
										if (l2 > r2&&bcPool[i][line].minI2 < bcPool[i][line].maxI2) swap(l2, r2);
										fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
											l2, r2,
											bcPool[i][line].minJ2, bcPool[i][line].maxJ2,
											bcPool[i][line].minK2, bcPool[i][line].maxK2,
											clip_index[id].second + 1);
									}

								}
								else if (clip_flag[id] == 1) {//target区域根本不在body中，将该区域设为Gneric #1,即bc_num=8
									l = max(l, clip_I[i]);
									l = l - clip_I[i] + 1;
									r = r - clip_I[i] + 1;
									if (bcPool[i][line].minI < 0) l = -l;
									if (bcPool[i][line].maxI < 0) r = -r;
									if (l<r&&bcPool[i][line].minI>bcPool[i][line].maxI) swap(l, r);
									if (l > r&&bcPool[i][line].minI < bcPool[i][line].maxI) swap(l, r);
									fprintf(output, "      %d        %d        %d       %d        %d        %d        8\n",
										l, r,
										bcPool[i][line].minJ, bcPool[i][line].maxJ,
										bcPool[i][line].minK, bcPool[i][line].maxK);
								}

							}
							else {//原来不是连接的部分
								l = max(l, clip_I[i]);
								l = l - clip_I[i] + 1;
								r = r - clip_I[i] + 1;
								if (bcPool[i][line].minI < 0) l = -l;
								if (bcPool[i][line].maxI < 0) r = -r;
								if (l<r&&bcPool[i][line].minI>bcPool[i][line].maxI) swap(l, r);
								if (l > r&&bcPool[i][line].minI < bcPool[i][line].maxI) swap(l, r);
								fprintf(output, "      %d        %d        %d       %d        %d        %d        %d\n",
									l, r,
									bcPool[i][line].minJ, bcPool[i][line].maxJ,
									bcPool[i][line].minK, bcPool[i][line].maxK,
									bcPool[i][line].bc_num);
							}
						}
					}
				}

				fclose(output);

				delete[]bc_name;
				delete[]clip_I;
				delete[]clip_index;
				delete[]clip_flag;
				delete[]regionNum;
				for (int i = 0; i < blockNum; i++) {
					delete[]bcPool[i];
				}
				delete[]bcPool;
				
				processFile((SignalInstance::getInstance()->getWorkSpace() + "/head_bd_before.inp").toLocal8Bit().toStdString(),
					(SignalInstance::getInstance()->getWorkSpace() + "/head_bd_afther.inp").toLocal8Bit().toStdString());

				processFile((SignalInstance::getInstance()->getWorkSpace() + "/body_genericbd_before.inp").toLocal8Bit().toStdString(),
					(SignalInstance::getInstance()->getWorkSpace() + "/body_genericbd_afther.inp").toLocal8Bit().toStdString());


				{
					using namespace platforms::inp;
					inp_process* p1 = new inp_process();
					inp_process* p2 = new inp_process();
					p1->work((SignalInstance::getInstance()->getWorkSpace() + "/head_bd_afther.inp").toLocal8Bit().toStdString(),
						(SignalInstance::getInstance()->getWorkSpace() + "/head_bd.inp").toLocal8Bit().toStdString(),6);
					p2->work((SignalInstance::getInstance()->getWorkSpace() + "/body_genericbd_afther.inp").toLocal8Bit().toStdString(),
						(SignalInstance::getInstance()->getWorkSpace() + "/body_genericbd.inp").toLocal8Bit().toStdString(),8);
					
					delete p1; delete p2;
				}
				



				m_bodyBdInpPath = SignalInstance::getInstance()->getWorkSpace() + "/body_genericbd.inp";
				m_headBdInpPath = SignalInstance::getInstance()->getWorkSpace() + "/head_bd.inp";
				QDir dir(SignalInstance::getWorkSpace());
				if (!dir.exists()) {
					qDebug() << "Directory does not exist:";

				}
				else {
					QVector<QString> v = { "head_bd_before.inp","body_genericbd_before.inp","head_bd_afther.inp","body_genericbd_afther.inp" };
					QString filePath;
					for (auto it : v) {
						filePath = dir.filePath(it);
						QFile::remove(filePath);
					}
				}

			}
			void SceneWidget::clip(int clipLine, lineInfo* gridLine, UNSTRUCTURED_GRID *Grid) {
				if (Grid == NULL || clipLine < 0) return;
				if (gridLine == NULL) return;
				if (clipLine >= gridLine->lineNum) return;
				if (headGrid != NULL) {
					headGrid->clear();
					free(headGrid);
					headGrid = NULL;
				}
				if (bodyGrid != NULL) {
					bodyGrid->clear();
					free(bodyGrid);
					bodyGrid = NULL;
				}
				if (clipGrid != NULL) {
					clipGrid->clear();
					free(clipGrid);
					clipGrid = NULL;
				}
				headGrid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				headGrid->init();
				bodyGrid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				bodyGrid->init();
				clipGrid = (UNSTRUCTURED_GRID*)malloc(sizeof(UNSTRUCTURED_GRID));
				clipGrid->init();

				int *lineId = NULL;
				lineId = new int[Grid->n_points]{ 0 };

				//idMap[i]=j  Grid中的第i个点，是headGrid中的第j个点
				int *idMap = NULL;
				idMap = new int[Grid->n_points]{ 0 };
				memset(idMap, -1, sizeof(int)*Grid->n_points);
				for (int i = 0; i < gridLine->lineNum; i++) {
					for (int j = 0; j < gridLine->pointNum[i]; j++) {
						int id = gridLine->pointList[i][j];
						lineId[id] = i;
					}
				}
				int n_points = 0;
				for (int i = 0; i < Grid->n_points; i++) {
					if (lineId[i] <= clipLine) {
						idMap[i] = n_points;
						n_points++;
					}
				}
				headGrid->set_nPoints(n_points);
				for (int i = 0; i < Grid->n_points; i++) {
					if (lineId[i] <= clipLine) {
						point*temp = &Grid->pointData[i];
						headGrid->addPoint(temp->x, temp->y, temp->z
							, temp->i, temp->j, temp->k, temp->blockId, temp->sectId);
					}
				}
				int n_cells = 0;
				for (int i = 0; i < Grid->n_cells; i++) {
					cell* temp = &Grid->cellData[i];
					int flag = 1;
					for (int j = 0; j < temp->numPoints; j++) {
						int id = temp->idPoint[j];
						if (lineId[id] > clipLine) {
							flag = 0;
							break;
						}
					}
					if (flag == 1) {
						n_cells++;
					}
				}
				headGrid->set_nCells(n_cells);
				for (int i = 0; i < Grid->n_cells; i++) {
					cell* temp = &Grid->cellData[i];
					int flag = 1;
					for (int j = 0; j < temp->numPoints; j++) {
						int id = temp->idPoint[j];
						if (lineId[id] > clipLine) {
							flag = 0;
							break;
						}
					}
					if (flag == 1) {
						int id1 = temp->idPoint[0];
						int id2 = temp->idPoint[1];
						int id3 = temp->idPoint[2];
						int id4 = temp->idPoint[3];
						headGrid->addVetraCell(idMap[id1], idMap[id2], idMap[id3], idMap[id4]);
					}
				}



				memset(idMap, -1, sizeof(int)*Grid->n_points);
				n_points = 0;
				for (int i = 0; i < Grid->n_points; i++) {
					if (lineId[i] >= clipLine) {
						idMap[i] = n_points;
						n_points++;
					}
				}
				bodyGrid->set_nPoints(n_points);
				for (int i = 0; i < Grid->n_points; i++) {
					if (lineId[i] >= clipLine) {
						point*temp = &Grid->pointData[i];
						bodyGrid->addPoint(temp->x, temp->y, temp->z
							, temp->i, temp->j, temp->k, temp->blockId, temp->sectId);
					}
				}
				n_cells = 0;
				for (int i = 0; i < Grid->n_cells; i++) {
					cell* temp = &Grid->cellData[i];
					int flag = 1;
					for (int j = 0; j < temp->numPoints; j++) {
						int id = temp->idPoint[j];
						if (lineId[id] < clipLine) {
							flag = 0;
							break;
						}
					}
					if (flag == 1) {
						n_cells++;
					}
				}
				bodyGrid->set_nCells(n_cells);
				for (int i = 0; i < Grid->n_cells; i++) {
					cell* temp = &Grid->cellData[i];
					int flag = 1;
					for (int j = 0; j < temp->numPoints; j++) {
						int id = temp->idPoint[j];
						if (lineId[id] < clipLine) {
							flag = 0;
							break;
						}
					}
					if (flag == 1) {
						int id1 = temp->idPoint[0];
						int id2 = temp->idPoint[1];
						int id3 = temp->idPoint[2];
						int id4 = temp->idPoint[3];
						bodyGrid->addVetraCell(idMap[id1], idMap[id2], idMap[id3], idMap[id4]);
					}
				}

				memset(idMap, -1, sizeof(int)*Grid->n_points);
				n_points = 0;




				delete[]lineId;
				lineId = NULL;
				delete[]idMap;
				idMap = NULL;
			}

			void SceneWidget::showClip()
			{
				sort_to_getI();
				auto start = std::chrono::high_resolution_clock::now();
				//vtkUnstructuredGrid* Data = vtk_dataSet;
				//plane->SetNormal(0, 0, 0);
				////plane->SetNormal(plane->GetNormal());

				//clipper->SetClipFunction(plane);
				//clipper->InsideOutOn();

				//vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter =
				//	vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
				//surfaceFilter->SetInputData(Data);
				//surfaceFilter->Update();
				//vtkPolyData* polydata = surfaceFilter->GetOutput();
				//clipper->SetInputData(polydata);
				//mapper->SetInputConnection(clipper->GetOutputPort());
				//actor->SetMapper(mapper);
				//vtkNew<vtkProperty> backFaces;
				//vtkNew<vtkNamedColors>colors;
				//backFaces->SetDiffuseColor(colors->GetColor3d("Gold").GetData());
				//actor->SetBackfaceProperty(backFaces);
				//主要耗时
				{auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = end - start;
				qDebug() << "getSurface Time taken: " << duration.count() << " seconds"; }
				vtkNew<vtkIPWCallback> myCallback;
				myCallback->plane = plane;
				//这里取消成重置为第一个位置
				//myCallback->index = 0;
				//打开时显示上次的位置
				myCallback->index = myStatus->getClipIndex();

				//double point3[3] = { 10, 10, 10 };

				for (int i = 0; i < 6; i++) {
					qDebug() << "gridBound[" << i << "]=" << gridBound[i];
				}
				for (int i = 0; i < 6; i++) {
					qDebug() << "getBound[" << i << "]=" << actor->GetBounds()[i];
				}
				vtkNew<vtkImplicitPlaneRepresentation> rep;
				myCallback->setPresentation(rep);
				rep->SetPlaceFactor(1.25); // 1.25 This must be set prior to placing the widget.
				rep->PlaceWidget(gridBound);
				rep->SetOrigin(gridBound[1], 0, 0);
				/*rep->PlaceWidget(actor->GetBounds());
				rep->SetOrigin(actor->GetBounds()[1], 0, 0);*/
				rep->OutlineTranslationOff();
				rep->NormalToXAxisOn();
				
				//次要耗时
				{auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = end - start;
				qDebug() << "setRep Time taken: " << duration.count() << " seconds"; }
				for (int i = 0; i < n_blocks; i++) {
					if (tagI[i] == 0) continue;
					int id = gridLine->pointList[tagI[i]][0];
					point* temp = &(un_grid->pointData[id]);
					myCallback->addPoint(temp->x, 0, 0);
				}
				//for (int i = 0; i < n_blocks; i++) {
				//	if (tagI[i] == 0) continue;
				//	int id = gridLine->pointList[tagI[i]][0];
				//	//change##1: 防止id越界无法显示切割窗口
				//	//if (id > un_grid->n_points) continue;
				//	//change##1
				//	point* temp = &(un_grid->pointData[id]);
				//	myCallback->addPoint(temp->x, 0, 0);
				//}
				{auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = end - start;
				qDebug() << "for Time taken: " << duration.count() << " seconds"; }
				myCallback->sensitivity = 15;
				myCallback->setPlane();
				//planeWidget->EnabledOff();
				clipIndex = &(myCallback->index);

				planeWidget->SetInteractor(m_pQVTKWidget->GetInteractor());
				planeWidget->SetRepresentation(rep);

				
				planeWidget->AddObserver(vtkCommand::InteractionEvent, myCallback);
				renderWindow->Render();
				m_pQVTKWidget->GetInteractor()->Initialize();
				//rep->SetOrigin(actor->GetBounds()[1] / 2, 0, 0);
				rep->SetOrigin(gridBound[1] / 2, 0, 0);
				planeWidget->On();
				m_pQVTKWidget->update();
				//qDebug() << "1 mouseStyle->canCliped=" << mouseStyle->canCliped;
				mouseStyle->canCliped = true;
				//qDebug() << "2 mouseStyle->canCliped=" << mouseStyle->canCliped;
				{auto end = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> duration = end - start;
				qDebug() << "total Time taken: " << duration.count() << " seconds"; }
			}

			void SceneWidget::cancelShowClip()
			{
				planeWidget->EnabledOff();
				planeWidget->RemoveAllObservers();
				clipIndex = NULL;

				m_pQVTKWidget->update();

				//qDebug() << "1 mouseStyle->canCliped=" << mouseStyle->canCliped;
				mouseStyle->canCliped = true;
				//qDebug() << "2 mouseStyle->canCliped=" << mouseStyle->canCliped;
			}











		}
	}
}



