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
#include <queue>
#include <iomanip>
#include <string>
#include <cstring>
#include <algorithm>
#include <map>
#include <vtkDoubleArray.h>
#include <vtkPolyData.h>
#include <vtkUnstructuredGrid.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPolyDataMapper.h>
#include <vtkPointData.h>
#include <vtkPolyDataWriter.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolyDataReader.h>
#include <vtkUnstructuredGridReader.h>
#include <vtkDataSet.h>
#include <vtkDataSetMapper.h>
#include <vtkNew.h>
#include <vtkAxesActor.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkScalarBarActor.h>
#include <vtkTextProperty.h>
#include <vtkCellData.h>
#include <vtkOutlineFilter.h>
#include <vtkImplicitPlaneRepresentation.h>
#include <vtkNamedColors.h>
#include <vtkTriangle.h>
#include <vtkUnsignedCharArray.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkInteractorStyleTrackballActor.h>
#include <vtkIdTypeArray.h>
#include <vtkCellPicker.h>
#include <vtkCamera.h>
#include <vtkAppendFilter.h>
#include <vtkSphereSource.h>
#include <vtkXMLUnstructuredGridReader.h>
#include <vtkProperty.h>
#include <vtkPointSource.h>
#include <vtkSelectionNode.h>
#include <vtkExtractSelection.h>
#include <vtkSelection.h>
#include <vtkDecimatePro.h>
#include <vtkPointPicker.h>
#include <vtkPlane.h>
#include <vtkGenericClip.h>
#include <vtkClipDataSet.h>
#include <vtkTransform.h>
#include <vtkClipPolyData.h>
#include <vtkCommand.h>
#include <vtkImplicitPlaneWidget2.h>
#include <vtkXMLPolyDataReader.h>
#include <vtkDataSetSurfaceFilter.h>
#include <QToolBar>
#include <QAction>
#include <QVTKWidget.h>
#include <QMainWindow>
#include <QSignalMapper>
#include <QToolButton>
#include <map>
#include <list>
#include <set>
#include <unordered_set>
#include <iostream>
#include <chrono>  
#include <SectInfoDialog.h>
#include <QAbstractButton>
#include<QMutex>
#include<QMessageBox>
#include "SignalInstance.h"
//#include "RWThread.h"
#include "myColorTable.h"
#include "SwitchButton.h"
#include "Common.h"
#include "btnObserver.h"
#include "ProgressDialog.h"


#define FIlEINSTANCE platforms::detail::FileOp::getInstance()
using namespace std;
using platforms::detail::SwitchButton;

typedef long long int ll;
//extern SceneWidget*main_sceneWidget;
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
	vtkSmartPointer<vtkUnstructuredGrid> vtkPltReader(const char* plt_file);
	//vtkSmartPointer<vtkPolyData> vtkPltReader(const char* plt_file);
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
	vtkNew<vtkUnstructuredGridReader> vtk_reader;
	//vtkNew<vtkPolyDataReader> vtk_reader;
	vtkSmartPointer<vtkUnstructuredGrid> vtk_dataSet;
	//vtkSmartPointer<vtkPolyData> vtk_dataSet;
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

#ifdef vtkGenericDataArray_h
#define InsertNextTupleValue InsertNextTypedTuple
#endif

namespace {
	vtkSmartPointer<vtkUnstructuredGrid>
		ReadUnstructuredGrid(std::string const& fileName);
}
namespace {
	vtkSmartPointer<vtkUnstructuredGrid>
		ReadUnstructuredGrid(std::string const& fileName)
	{
		vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid;
		std::string extension = "";
		if (fileName.find_last_of(".") != std::string::npos)
		{
			extension = fileName.substr(fileName.find_last_of("."));
		}

		// Drop the case of the extension
		std::transform(extension.begin(), extension.end(), extension.begin(),
			::tolower);

		if (extension == ".vtu")
		{
			vtkNew<vtkXMLUnstructuredGridReader> reader;
			reader->SetFileName(fileName.c_str());
			reader->Update();
			unstructuredGrid = reader->GetOutput();
		}
		else if (extension == ".vtk")
		{
			vtkNew<vtkUnstructuredGridReader> reader;
			reader->SetFileName(fileName.c_str());
			reader->Update();
			unstructuredGrid = reader->GetOutput();
		}
		else
		{
			vtkNew<vtkSphereSource> source;
			source->Update();
			vtkNew<vtkAppendFilter> appendFilter;
			appendFilter->AddInputData(source->GetOutput());
			appendFilter->Update();
			unstructuredGrid = appendFilter->GetOutput();
		}

		return unstructuredGrid;
	}

} // namespace


namespace {

	// Define interaction style.
	class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
	{
	public:
		static KeyPressInteractorStyle* New();
		vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);
		int windowFlag = 0;
		bool canCliped = true;
		virtual void OnKeyPress() override
		{
			// Get the keypress.
			vtkRenderWindowInteractor* rwi = this->Interactor;
			std::string key = rwi->GetKeySym();

			// Output the key that was pressed.

			// Handle an arrow key.
			if (!canCliped) return;
			canCliped = false;
			//取消快捷键，使用按钮进行控制
			
			// Forward events.
			//vtkInteractorStyleTrackballCamera::OnKeyPress();
		}
	};
	vtkStandardNewMacro(KeyPressInteractorStyle);

} // namespace

namespace {

	// Catch mouse events.
	class MouseInteractorStyle : public vtkInteractorStyleTrackballCamera
	{

	public:
		lineInfo* gridLineInfo;
		vtkUnstructuredGrid* Data;
		sectPool* sectInfo;

		vtkRenderer* Renderer;
		vtkDataSetMapper* mapper;
		vtkActor* actor;
		vtkUnsignedCharArray* ColorsSect;
		vtkUnsignedCharArray* ColorsCurrent;
		vtkUnsignedCharArray* ColorsGroup;
		vtkRenderWindow* renderWindow;
		UNSTRUCTURED_GRID *un_grid;
		bool m_statusIsCanML;
		bool m_statusOperate;
		int windowClipFlag;
		bool canCliped = false;
		unsigned char myRed[3] = { 255, 0, 0 };
		unsigned char myGreen[3] = { 0, 255, 0 };
		unsigned char myBlue[3] = { 0, 0, 255 };
		unsigned char myYellow[3] = { 255,255,0 };
		unsigned char myBlack[3] = { 0,0,0 };
		unsigned char mySilver[3] = { 192,192,192 };
		unsigned char myPurple[3] = { 128,0,128 };
		unsigned char myBrown[3] = { 165,42,42 };
		int get_sect(int id, UNSTRUCTURED_GRID *un_grid) {
			if (id >= un_grid->n_points || id < 0) {
				//cout << "error! cant find point!" << endl;
				return -1;
			}
			return un_grid->pointData[id].sectId;
		}
		MouseInteractorStyle() :NumberOfClicks(0), ResetPixelDistance(5)
		{
			this->PreviousPosition[0] = 0;
			this->PreviousPosition[1] = 0;
			m_statusIsCanML = false;
		}
		static MouseInteractorStyle* New();
		virtual void OnKeyPress() override
		{
			// Get the keypress.
			vtkRenderWindowInteractor* rwi = this->Interactor;
			std::string key = rwi->GetKeySym();

			// Output the key that was pressed.
			//qDebug() << "canClip=" << canCliped;
			// Handle an arrow key.
			if (!canCliped) return;
			//qDebug() << "ClipChanged=" << canCliped;
			canCliped = false;
			//qDebug() << "ClipChanged=" << canCliped;
			
			// Forward events.
			//vtkInteractorStyleTrackballCamera::OnKeyPress();
		}
		//点击滚轮，按I进行不断刷新染色
		//gapModel变量操作染色模式，lineGap是染色间隔，gapTime是多久染一次色
		virtual void OnMiddleButtonDown() override {
			/*if (sectInfo == NULL) return;
			emit SignalInstance::getInstance()->signalColorLine();*/
		}
		//点击鼠标右键，选择分段并染色
		virtual void OnLeftButtonDown() override
		{
			return vtkInteractorStyleTrackballCamera::OnLeftButtonDown();;
			if (sectInfo == NULL) return;


			//// Get the location of the click (in window coordinates).
			int* pos = this->GetInteractor()->GetEventPosition();


			vtkNew<vtkPointPicker> picker;
			//picker->SetTolerance(0.0005);

			//// Pick from this location.
			picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

			double* worldPosition = picker->GetPickPosition();
			//qDebug() << "(" << worldPosition[0] << "," << worldPosition[1] << ")";

			int id = picker->GetPointId();
			if (id == -1) return;
			//	
			int sectId = get_sect(id, un_grid);
			int sectId3D = -1;
			for (int i = 0; i < sectInfo->sectNum; i++) {
				double x1, x2, y1, y2, z1, z2;
				x1 = sectInfo->min_x[i];
				x2 = sectInfo->max_x[i];
				y1 = sectInfo->min_y[i];
				y2 = sectInfo->max_y[i];
				z1 = sectInfo->min_z[i];
				z2 = sectInfo->max_z[i];
				if (worldPosition[0] >= x1 && worldPosition[0] <= x2
					&& worldPosition[1] >= y1 && worldPosition[1] <= y2
					&& worldPosition[2] >= z1 && worldPosition[2] <= z2) {
					sectId3D = i;
					break;
				}
			}
			emit SignalInstance::getInstance()->signaleSelectCell(sectId);
		}
		void virtual OnRightButtonDown() override
		{
			qDebug() << "rightButton clicked";
			if (sectInfo == NULL) {

				return vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
			}

			vtkNew<vtkNamedColors> colors;

			// Get the location of the click (in window coordinates).
			int* pos = this->GetInteractor()->GetEventPosition();

			vtkNew<vtkPointPicker> picker;
			//picker->SetTolerance(0.0005);

			// Pick from this location.
			picker->Pick(pos[0], pos[1], 0, this->GetDefaultRenderer());

			double* worldPosition = picker->GetPickPosition();
			int cellId = 0;
			//cellId = picker->GetCellId();
			if (cellId == -1) {
				return vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
			}
			if (cellId != -1)
			{
				int id = picker->GetPointId();
				qDebug() << "x=" << pos[0] << " y=" << pos[1] << " id=" << id<<" m_statusIsCanML="<< m_statusIsCanML;
				if (id == -1) return vtkInteractorStyleTrackballCamera::OnRightButtonDown();
				if (!m_statusIsCanML)
					return vtkInteractorStyleTrackballCamera::OnRightButtonDown();
				int sectId = get_sect(id, un_grid);
				//emit SignalInstance::getInstance()->signalChooseSect(sectId);

				SectInfoDialog*dialog = new SectInfoDialog(sectId);
				//qDebug() << "id:" << sectId << endl;
				dialog->setAttribute(Qt::WA_ShowModal, true);
				dialog->exec();
				//return vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
				return;
			}
			return vtkInteractorStyleTrackballCamera::OnRightButtonDown();

		}
		void clipFlagChanged(int flag)
		{
			windowClipFlag = flag;
		}
		int getClipFlag() {
			return windowClipFlag;
		}
		virtual void OnMouseWheelForward(void)
		{
			//qDebug() << "OnMouseWheelForward!";
			vtkInteractorStyleTrackballCamera::OnMouseWheelForward();
		}
		virtual void OnMouseWheelBackward(void)
		{
			//qDebug() << "OnMouseWheelBackward!";
			vtkInteractorStyleTrackballCamera::OnMouseWheelBackward();
		}
	private:
		unsigned int NumberOfClicks;
		int PreviousPosition[2];
		int ResetPixelDistance;
	};

	vtkStandardNewMacro(MouseInteractorStyle);

} // namespace

namespace platforms {
	namespace sspns {
		namespace reveal {
			class SceneWidget : public QMainWindow {
				Q_OBJECT
			public:
				explicit SceneWidget(int nsFlag=0,QWidget* parent = nullptr);
				~SceneWidget();
				//! Add a data set to the scene
				/*!
				\param[in] dataSet The data set to add
			  */
				void addDataSet(vtkSmartPointer<vtkDataSet> dataSet);

				//! Remove the data set from the scene
				void removeDataSet();
				vtkSmartPointer<vtkUnstructuredGrid> vtkPltReader(const char* plt_file);
				//vtkSmartPointer<vtkPolyData> vtkPltReader(const char* plt_file);
				void trOnlyLoad();
				void onltShowTr();
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
				void initConnection();
				void resetCamera(bool bCompulsion);
				void initVariables();
				void showVTKData();
				void initArrowActor();
				void initAttributeSelToolBar();
				void SetActivateAttributeToMapperForVtk(vtkSmartPointer<vtkDataSetMapper> mapper);
				void addScalarBarActorForVtk(vtkSmartPointer<vtkDataSetMapper> mapper, string title, double * range);
				void initDisplayModeToolBar();
				/// 初始化切割选项(右侧作为pns3d网格/左侧作为pns3d网格)
				void initDisplayClipModelToolBar();
				void setClipModel();
				void initUI();
				void addObserver();
				
				void processFile(const std::string& inputPath, const std::string& outputPath);
				vtkSmartPointer<vtkActor> addOutlineActorForVtk(vtkSmartPointer<vtkOutlineFilter> outlineData);
				vtkSmartPointer<vtkActor> addOutlineActor(vtkSmartPointer<vtkOutlineFilter> outlineData);
				vtkSmartPointer<vtkActor> addPointsActorForVtk(vtkSmartPointer<vtkDataSet> dataSet);
				vtkSmartPointer<vtkActor> addWireFrameActorForVtk(vtkSmartPointer<vtkDataSet> dataSet, bool bBlack);
				void updateAttributeNameMap(vtkSmartPointer<vtkUnstructuredGrid> mesh);
				void initScene();
				void initPlaneChooseToolBar();
				void initSelectClickModel();
				void initOperateSwitchToolBar();
				void initLegendToolBar();
				void initClipWindowVisibleToolBar();
				void initClipWindowInvisibleToolBar();
				void initClipToolBar();
				void initReClipToolBar();
				void initClipSplitBar();
				void initColorStopBar();
				void setBatchMode();
				double redution = 0.1;
				sectPool *sectInfo = NULL;
				sectPool *decimated_sectInfo = NULL;
				vtkNew<MouseInteractorStyle> mouseStyle;
				vtkNew<KeyPressInteractorStyle> keyStyle;
				UNSTRUCTURED_GRID *un_grid = NULL;
				UNSTRUCTURED_GRID *decimated_grid = NULL;
				decimatedInfo* deciInfo = NULL;
				lineInfo* gridLine = NULL;
				lineInfo* decimatedLine = NULL;
				myColorTable* myColor = NULL;
				void setFullGridReadFlag(bool flag);
				/// @brief 获取染色lineId
				int getBodyLineId();
				int decimatedFlag = 0;
				double gridBound[6];
				int m_nsFlag;
			signals:
				//void signalRWFinsh(QString messInfo);
			public slots:
				//! Zoom to the extent of the data set in the scene
				void zoomToExtent();
				void ColorStop();
				bool LoadPlot3DFile(QString filename);
				void attributeSelSlot(QString attribute);
				void displayModeSlot(QString mode);
				void clipModelSlot(QString model);
				bool LoadVTKFile();
				void planeChooseSlot(QString pickName);
				void legendSlot(QString legend);
				void slotLoadSectData(QString path);
				void slotLoadGridData(QString path, int pathFlag,int renderFlag=1);

				void showMessage(QString mess);
				void preRead(QString inPath);
				void onlyOut(QString outPath);
				void readAndOut(QString inPath, QString outPath);
				void onlyReadGrid(QString inPath);
				void onlyReadSect(QString inPath);
				void onlyPreReadGirdSect(QString inPath);
				void setSectColor(int r, int g, int b, int sectId);
				void onChangeOperateStatus();
				void onChangeMlStatus();
				void onReset();
				/// @brief 切割网格
				void onClipGrid();
				/// @brief 暂停染色
				void onColorLineStop();
				//显示两部分
				void onClipSplit();
				//new function
				void get_idOffset(int n_blocks, block *bk);
				void get_coordinateOffset(int n_blocks, block *bk);
				int get_id(int ni, int nj, int nk, int i, int j, int k, int blockIndex, int overlapFlag);
				//use id to get information
				int get_sect(int id, UNSTRUCTURED_GRID *un_grid);
				//get nums of different x
				void get_totalBlockInfo(block *bk, int n_blocks);

				void transform(block *bk, int n_blocks, UNSTRUCTURED_GRID *grid, int overlapFlag);
				void out_to_file(QString file_name);

				//read grid and build sectPool
				void build_sectPool(sectPool *sectInfo, UNSTRUCTURED_GRID *grid);
				// block Id1 < blockId2
				void judge_link(int blockId1, int blockId2);
				void get_line_range(int blockId1, int line);
				void test();
				void test2();
				//read grid.grd
				void read_grid(string file_name);
				//read grid.dat
				void read_dat(string file_name);
				//read TR,dat
				void read_sect(string file_name);
				//恢复上次运行状态
				void recover();

				void out_to_preBlock(QString file_name);
				void readPreBlock(QString file_name);
				void out_to_preGrid(QString file_name, UNSTRUCTURED_GRID* un_grid,int decimateGridFlag = 0);
				void readPreGrid(QString file_name, UNSTRUCTURED_GRID* &un_grid, int decimateGridFlag = 0);
				void out_to_preSect(QString file_name, sectPool* sectInfo);
				void readPreSect(QString file_name, sectPool* &sectInfo, UNSTRUCTURED_GRID* &un_grid);
				void out_to_preIdmap(QString file_name, map<xyzPoint, int>* idMap);
				void readPreIdmap(QString file_name, map<xyzPoint, int>* idMap);
				void out_to_preLineInfo(QString file_name, lineInfo* gridLine);
				void readPreLineInfo(QString file_name, lineInfo* &gridLine);
				void judgeAndAddCells(int id1, int id2, int id3, int id4);
				void judgeAndAddCells(int id1, int id2, int id3);
				//decimate grid
				void decimateGrid();
				void decimateGridSurface();
				void decimateSect();
				void markPoints();

				//get lineInfo
				void get_lineInfo();
				void get_decimatedLineInfo();
				//将vtk_dataset指向grid或者简化后的grid
				void selectGrid();
				void selectDecimatedGrid();
				//将decimatedgrid数据结构转化成vtk grid
				void buildDecimatedGrid();
				//按行染色
				void colorLine();
				void colorLineTimer();
				//按段染色 flag==0默认是简化前的网格  ==1是简化后的网格
				void colorSect(int decimatedFlag);
				void trPreLoad();
				void changeGapModel();
				void clearAll();
				void highlight(int sectId);
				void cancelHighlight();
				void out_to_preIdtrans(QString file_name, int *idTrans,int transSize);
				void readPreIdTrans(QString file_name);
				//新加内容
				void sort_to_getI();
				void clip_out(int I);
				void clip_out_inp(int I);
				void clip(int clipLine, lineInfo* gridLine, UNSTRUCTURED_GRID *Grid);
				void showClip();
				void cancelShowClip();
				void slotClip();
				void slotClipSplit();
				void slotClipBody(Status lastStatus);
				void slotBothClip();
				void clearBothClip();
				int get_decimateId(int ni, int nj, int nk, int i, int j, int k, int blockIndex, int overlapFlag);
				void out_to_obj();
				// 选取切割模式
				/*void slotLeftAsInput();
				void slotRightAsInput();*/
			private:
				
				//int * m
				vtkNew<vtkUnstructuredGridReader> vtk_reader;
				//vtkNew<vtkPolyDataReader> vtk_reader;
				vtkSmartPointer<vtkUnstructuredGrid> vtk_dataSet;
				//vtkSmartPointer<vtkPolyData> vtk_dataSet;
				vtkNew<vtkDataSetMapper> vtk_mapper;
				vtkNew<vtkActor> vtk_actor;
				vtkSmartPointer<vtkUnstructuredGrid> plt_dataSet;
				//vtkSmartPointer<vtkPolyData> plt_dataSet;
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
				QToolButton* m_clipToolBtn;								  //切割模式选取按钮
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
				string m_curShowClipModel;
				vtkSmartPointer<vtkScalarBarActor> m_scalarBarActor;
				QSignalMapper* m_displayModeMapper;                       //显示模式
				QSignalMapper* m_legendMapper;                            //图例
				QSignalMapper* m_clipModelMapper;						  //切割方式
				bool isVTKData;
				QSignalMapper* m_planeMapper;
				block *bk = NULL;
				int n_blocks;
				map<xyzPoint, int> idMap;
				map<xyzPoint, int> decimated_idMap;
				//sectPool *sectInfo = NULL;
				int *idOffset = NULL;
				double *coordinateOffset = NULL;
				int *idTrans = NULL;
				int transSize = 0;
				vtkNew<vtkNamedColors> colors;
				vtkNew<vtkRenderer> renderer;
				vtkNew<vtkRenderWindow> renderWindow;
				vtkNew<vtkRenderWindowInteractor> interactor;
				vtkNew<vtkUnsignedCharArray> ColorsGroup;
				vtkNew<vtkUnsignedCharArray> ColorsSect;
				vtkNew<vtkUnsignedCharArray> ColorsCurrent;

				//vtkNew<vtkUnstructuredGridQuadricDecimation> decimate;
				float reduction;

				vtkNew<vtkDataSetMapper> mapper;
				vtkNew<vtkActor> actor;
				vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;

				vtkNew<vtkUnstructuredGrid> ugrid;
				vtkNew<vtkPoints> points;

				vtkNew<vtkUnstructuredGrid> decimatedGrid;
				vtkNew<vtkUnsignedCharArray> ColorsDecimated;
				vtkNew<vtkPoints> decimatedPoints;

				vtkNew<vtkActor> preActor;
				vtkNew<vtkDataSetMapper> preMapper;
				vtkSmartPointer<vtkUnstructuredGrid> preGrid;


				vtkNew<vtkPoints>emptyPoints;
				vtkNew<vtkUnstructuredGrid>emptyGrid;
				//highlight
				vtkNew<vtkActor> highlightActor;
				vtkNew<vtkUnstructuredGrid> highlightData;
				vtkNew<vtkPoints> hilightPoints;
				vtkNew<vtkDataSetMapper> highlightMapper;
				vtkNew<vtkNamedColors> namedColors;

				int lineId = 0;
				QTimer* timer = NULL;
				int lineGap = 4;
				int beforeLineId = 0;
				unsigned char(*lastColor)[3] = NULL;
				//来控制按I染色时，是覆盖模式还是跳跃模式,0是跳跃模式，1是覆盖模式
				int gapModel = 0;
				//真正染色时需要的变量。
				int currentGapModel = 0;
				//按I染色时的速度，ms为单位
				int gapTime = 3000;

				//新加内容
				UNSTRUCTURED_GRID* headGrid;
				UNSTRUCTURED_GRID* bodyGrid;
				//clip以后head和body分离的grid
				UNSTRUCTURED_GRID* clipGrid;
				int *clipIndex = NULL;
				int *tagI = NULL;
				vtkNew<vtkImplicitPlaneWidget2> planeWidget;
				vtkNew<vtkPlane> plane;
				vtkNew<vtkClipPolyData> clipper;
				friend class MouseInteractorStyle;
				/// 用于将切割完的右边设置为pns3d.exe的输入,默认为true
				bool m_rightClipFlag;
				//是否将两个部分全部显示
				bool m_bothClipFlag;
				//用来设置head和body的间隔
				double m_clipOff;
				

				bool m_fullGridReadFlag;
				platforms::detail::SwitchButton*m_mainTainOrBrowseButton = NULL;
				platforms::detail::SwitchButton*m_coverOrdumpButton = NULL;
				QPushButton* m_clipWindowVisible;
				QPushButton* m_clipWindowInvisible;
				QPushButton* m_btnClip;
				QPushButton* m_btnReClip;
				QPushButton* m_btnClipSplit;
				QPushButton* m_btnColorStop;
				
				int *clip_flag = NULL;
				//clip_I是切开的I
				int *clip_I = NULL;
				//first==head_index    second==body_index
				pair<int, int> *clip_index = NULL;
				/// 保存各个路径信息
				QString m_fullInpPath;
				QString m_headClipPath;
				QString m_bodyClipPath;
				QString m_bothClipPath;
				QString m_headBdInpPath;
				QString m_bodyBdInpPath;
				QMutex colorLineMutex;
			};
		}
	}
}


#endif // SCENEWIDGET_H

