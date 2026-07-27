#pragma execution_character_set("utf-8")
#include "scenewidget.h"

#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
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
#include "huangmainwindow.h"
/*
#include "vtkAutoInit.h"
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkRenderingVolumeOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkInteractionStyle);

#include <vector>
*/

//#ifndef INITIAL_OPENGL
//#define INITIAL_OPENGL
#include <vtkAutoInit.h>
VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);

//*****************
//此声明极为重要
//#define vtkRenderingCore_AUTOINIT 4(vtkInteractionStyle, vtkRenderingFreeType, vtkRenderingFreeTypeOpenGL, vtkRenderingOpenGL);
//#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL);
//*****************

//#endif //INITIAL_OPENGL

SceneWidget::SceneWidget(QWidget* parent)
{
	m_pQVTKWidget = new QVTKWidget(this);
	setCentralWidget(m_pQVTKWidget);

	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	camera->Zoom(0.5);

	// Renderer
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
	m_renderer->SetActiveCamera(camera);
	// m_renderer->SetBackground(0.5, 0.5, 0.5);
	//RenderWindow()->AddRenderer(m_renderer);
	// RenderWindow->AddRenderer(m_renderer);
	m_renderer->SetBackground(1.0, 1.0, 1.0);                        // 设置页面底部颜色值
	m_renderer->SetBackground2(0.529, 0.8078, 0.92157);    // 设置页面顶部颜色值
	m_renderer->SetGradientBackground(1);                           // 开启渐变色背景设置
	m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);
	m_pQVTKWidget->update();
	initVariables();
	initActions();
	initToolBars();
	initArrowActor();
	logger()->info("VTK INIT SUCCESS!");
	// LoadPlot3DFile("");
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
{/*
	//取得actor
	vtkActor* actor = m_renderer->GetActors()->GetLastActor();
	//删除已经有的actor
	if (actor != nullptr) {
		m_renderer->RemoveActor(actor);
	}

	//渲染窗口,对窗口进行初始化
	m_pQVTKWidget->update();
	*/
}

vtkSmartPointer<vtkPolyData> SceneWidget::vtkPltReader(const char* plt_file) {
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
	vtkSmartPointer<vtkPolyData> data_set = vtkSmartPointer<vtkPolyData>::New();
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
	data_set->SetPolys(cells);

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
	plt_dataSet = vtkSmartPointer<vtkPolyData>::New();
	plt_dataSet = vtkPltReader(pltFile.toStdString().c_str());
	addDataSet(plt_dataSet);
	//boolisLoaded = false;
	QString attr_string = "Cp";
	attributeSelSlot(attr_string);
	return true;
}

bool SceneWidget::LoadVTKFile()
{
	QString vtkFile = "original_data.vtk";
	QFile file(vtkFile);
	file.open(QIODevice::ReadOnly);
	if (!file.exists()) {
		QMessageBox::information(this, "提示", "文件不存在!!");
		
		return false;
	}
	//清除已经有的图片和初始化背景
	this->removeDataSet();

	vtk_reader->SetFileName(vtkFile.toStdString().c_str());
	vtk_reader->Update();
	vtk_dataSet = vtkSmartPointer<vtkPolyData>::New();
	vtk_dataSet = vtk_reader->GetOutput();// vtkPltReader("bnd_morph00000001.plt");
	showVTKData();
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
	const QIcon setViewDirectionToXIcon = QIcon::fromTheme(tr("设置视图朝向X轴正向"), QIcon("images/setViewDirectionToX.png"));
	m_setViewDirectionToXAct = new QAction(setViewDirectionToXIcon, tr("设置视图朝向X轴正向(&X)"), this);
	m_setViewDirectionToXAct->setStatusTip(tr("设置视图朝向X轴正向"));
	connect(m_setViewDirectionToXAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToX);

	//设置视图朝向X轴负向
	const QIcon setViewDirectionToNegativeXIcon = QIcon::fromTheme(tr("设置视图朝向X轴负向"), QIcon("images/setViewDirectionToNegativeX.png"));
	m_setViewDirectionToNegativeXAct = new QAction(setViewDirectionToNegativeXIcon, tr("设置视图朝向X轴负向(&A)"), this);
	m_setViewDirectionToNegativeXAct->setStatusTip(tr("设置视图朝向X轴负向"));
	connect(m_setViewDirectionToNegativeXAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToNegativeX);

	//设置视图朝向Y轴正向
	const QIcon setViewDirectionToYIcon = QIcon::fromTheme(tr("设置视图朝向Y轴正向"), QIcon("images/setViewDirectionToY.png"));
	m_setViewDirectionToYAct = new QAction(setViewDirectionToYIcon, tr("设置视图朝向Y轴正向(&Y)"), this);
	m_setViewDirectionToYAct->setStatusTip(tr("设置视图朝向Y轴正向"));
	connect(m_setViewDirectionToYAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToY);

	//设置视图朝向Y轴负向
	const QIcon setViewDirectionToNegativeYIcon = QIcon::fromTheme(tr("设置视图朝向Y轴负向"), QIcon("images/setViewDirectionToNegativeY.png"));
	m_setViewDirectionToNegativeYAct = new QAction(setViewDirectionToNegativeYIcon, tr("设置视图朝向Y轴负向(&B)"), this);
	m_setViewDirectionToNegativeYAct->setStatusTip(tr("设置视图朝向Y轴负向"));
	connect(m_setViewDirectionToNegativeYAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToNegativeY);

	//设置视图朝向Z轴正向
	const QIcon setViewDirectionToZIcon = QIcon::fromTheme(tr("设置视图朝向Z轴正向"), QIcon("images/setViewDirectionToZ.png"));
	m_setViewDirectionToZAct = new QAction(setViewDirectionToZIcon, tr("设置视图朝向Z轴正向(&Z)"), this);
	m_setViewDirectionToZAct->setStatusTip(tr("设置视图朝向Z轴正向"));
	connect(m_setViewDirectionToZAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToZ);

	//设置视图朝向Z轴负向
	const QIcon setViewDirectionToNegativeZIcon = QIcon::fromTheme(tr("设置视图朝向Z轴负向"), QIcon("images/setViewDirectionToNegativeZ.png"));
	m_setViewDirectionToNegativeZAct = new QAction(setViewDirectionToNegativeZIcon, tr("设置视图朝向Z轴负向(&C)"), this);
	m_setViewDirectionToNegativeZAct->setStatusTip(tr("设置视图朝向Z轴负向"));
	connect(m_setViewDirectionToNegativeZAct, &QAction::triggered, this, &SceneWidget::setViewDirectionToNegativeZ);

	//放大
	const QIcon amplificationViewIcon = QIcon::fromTheme(tr("放大"), QIcon("images/amplificationView.png"));
	m_amplificationViewAct = new QAction(amplificationViewIcon, tr("放大(&Z)"), this);
	m_amplificationViewAct->setStatusTip(tr("放大"));
	connect(m_amplificationViewAct, &QAction::triggered, this, &SceneWidget::amplificationView);

	//缩小
	const QIcon narrowViewIcon = QIcon::fromTheme(tr("缩小"), QIcon("images/narrowView.png"));
	m_narrowViewAct = new QAction(narrowViewIcon, tr("缩小(&Z)"), this);
	m_narrowViewAct->setStatusTip(tr("缩小"));
	connect(m_narrowViewAct, &QAction::triggered, this, &SceneWidget::narrowView);
}


void SceneWidget::setViewDirectionToX() {
	vtkCamera* curCamera = m_renderer->GetActiveCamera();
	curCamera->SetViewUp(0, 1, 0);
	curCamera->SetPosition(1, 0, 0);
	curCamera->SetFocalPoint(0, 0, 0);
	m_renderer->SetActiveCamera(curCamera);
	// resetCamera(true);
	m_pQVTKWidget->update();
}

void SceneWidget::setViewDirectionToNegativeX() {
	vtkCamera* curCamera = m_renderer->GetActiveCamera();
	curCamera->SetViewUp(0, -1, 0);
	curCamera->SetPosition(1, 0, 0);
	curCamera->SetFocalPoint(0, 0, 0);
	m_renderer->SetActiveCamera(curCamera);
	m_pQVTKWidget->update();
}

void SceneWidget::setViewDirectionToY() {
	vtkCamera* curCamera = m_renderer->GetActiveCamera();
	curCamera->SetViewUp(0, 1, 0);
	curCamera->SetPosition(0, 1, 0);
	curCamera->SetFocalPoint(0, 0, 0);
	m_renderer->SetActiveCamera(curCamera);
	m_pQVTKWidget->update();
}

void SceneWidget::setViewDirectionToNegativeY() {
	vtkCamera* curCamera = m_renderer->GetActiveCamera();
	curCamera->SetViewUp(0, -1, 0);
	curCamera->SetPosition(0, 1, 0);
	curCamera->SetFocalPoint(0, 0, 0);
	m_renderer->SetActiveCamera(curCamera);
	m_pQVTKWidget->update();
}

void SceneWidget::setViewDirectionToZ() {
	vtkCamera* curCamera = m_renderer->GetActiveCamera();
	curCamera->SetViewUp(0, 1, 0);
	curCamera->SetPosition(0, 0, 1);
	curCamera->SetFocalPoint(0, 0, 0);
	m_renderer->SetActiveCamera(curCamera);
	m_pQVTKWidget->update();
}

void SceneWidget::setViewDirectionToNegativeZ() {
	vtkCamera* curCamera = m_renderer->GetActiveCamera();
	curCamera->SetViewUp(0, -1, 0);
	curCamera->SetPosition(0, 0, 1);
	curCamera->SetFocalPoint(0, 0, 0);
	m_renderer->SetActiveCamera(curCamera);
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
	initAttributeSelToolBar();
	initDisplayModeToolBar();
	initLegendToolBar();
	initPlaneChooseToolBar();
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
	m_displayModeToolBtn = nullptr;
	isVTKData = 1;
	m_legendMapper = nullptr;
	m_legendToolBtn = nullptr;
	m_planeToolBtn = nullptr;
	m_planeMapper = nullptr;
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
	//else if (m_curShowAttrType == "FIELD") {
	//    auto pField = mapper->GetInput()->GetFieldData();
	//    if (-1 != pField->SetActiveScalars(m_curShowAttrName.c_str())
	//        || -1 != pField->SetActiveVectors(m_curShowAttrName.c_str())
	//        || -1 != pField->SetActiveTensors(m_curShowAttrName.c_str())
	//        || -1 != pField->SetActiveNormals(m_curShowAttrName.c_str())
	//        || -1 != pField->SetActiveTCoords(m_curShowAttrName.c_str())) {
	//        double* range = pField->GetArray(m_curShowAttrName.c_str())->GetRange();
	//        addScalarBarActorForVtk(mapper, m_curShowAttrName, range);
	//    }
	//    else { //非显示属性
	//        double range[2] = { 0.0,0.0 };
	//        addScalarBarActorForVtk(mapper, m_curShowAttrName, range);
	//    }
	//}
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
	//m_scalarBarActor->SetUnconstrainedFontSize(true);
	//m_scalarBarActor->SetTextPositionToPrecedeScalarBar();
	//m_scalarBarActor->GetTitleTextProperty()->SetColor(0., 0., 1.);
	//m_scalarBarActor->GetLabelTextProperty()->SetColor(0., 0., 1.);
	//m_scalarBarActor->GetAnnotationTextProperty()->SetColor(0., 0., 1.);
	//m_scalarBarActor->SetDrawFrame(1);
	//m_scalarBarActor->GetFrameProperty()->SetColor(0., 0., 0.);
	//m_scalarBarActor->SetDrawBackground(1);
	//m_scalarBarActor->GetBackgroundProperty()->SetColor(1., 1., 1.);
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
		const QIcon icon = QIcon::fromTheme(tr(mode.c_str()), QIcon(("images/" + mode + ".png").c_str()));
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
	m_displayModeToolBtn->setIcon(QIcon::fromTheme(tr("显示模式"),
		QIcon(("images/" + vecDisplayMode[0] + ".png").c_str())));
	m_displayModeToolBtn->setText(QString(tr("显示模式")));

	// 为QToolButton设置菜单
	m_displayModeToolBtn->setMenu(vecAction.empty() ? nullptr : menu);
	// m_curShowMeshMode = "表面模式";
}

void SceneWidget::displayModeSlot(QString mode) {
	m_curShowMeshMode = mode.toUtf8().toStdString();
	QIcon icon = QIcon::fromTheme(tr(mode.toUtf8().toStdString().c_str()),
		QIcon(("images/" + mode.toUtf8().toStdString() + ".png").c_str()));
	m_displayModeToolBtn->setIcon(icon);
	if (mode.size() > 15) {
		mode.resize(15);
		mode += "...";
	}
	m_displayModeToolBtn->setText(mode);

	//updateReShowFlag();
	initUI();
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
	vtkSmartPointer<vtkPolyDataMapper> mapOutline = vtkSmartPointer<vtkPolyDataMapper>::New();
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

void SceneWidget::updateAttributeNameMap(vtkSmartPointer<vtkPolyData> mesh) {
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
	m_planeToolBtn->setMenu(vecAction.empty() ? nullptr : menu);
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