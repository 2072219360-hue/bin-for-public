#include "PostProcessProfiling.h"
#pragma execution_character_set("utf-8")
#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkPolyDataMapper.h>
#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkProperty.h>
#include <vtkRenderWindowInteractor.h>
#include <QMessageBox>
#include <QDockWidget>
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
#include <vtkAutoInit.h>
#include <vtkImageData.h>
#include <vtkColorTransferFunction.h>


// 新增类，用于操纵杆
class MySimplePlaneWidget : public vtkPlaneWidget
{
public:
	static MySimplePlaneWidget* New(); // 由 vtkStandardNewMacro 实现
	vtkTypeMacro(MySimplePlaneWidget, vtkPlaneWidget);

protected:
	MySimplePlaneWidget()
	{
		// 设置一个合理的默认像素大小 (0.02 = 2% 视口大小)
		// 你可以在这里调整这个值，比如 0.03 或 0.04，让它变大
		this->SetHandleSize(0.02);
	}
	~MySimplePlaneWidget() override = default;

	// 这是关键：我们重写 vtkPlaneWidget 的空函数，
	// 并调用它 "祖父类" vtk3DWidget 的真正实现
	void SizeHandles() override
	{
		this->vtk3DWidget::SizeHandles();
	}

private:
	MySimplePlaneWidget(const MySimplePlaneWidget&) = delete;
	void operator=(const MySimplePlaneWidget&) = delete;
};

// 这一行也必须在类的外面
vtkStandardNewMacro(MySimplePlaneWidget);



VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingContextOpenGL2);
VTK_MODULE_INIT(vtkRenderingFreeType);
extern QString appComDir;
//vtkSmartPointer<vtkImplicitPlaneWidget2> planeWidget;
vector<string> Stringsplit(string str, const const char split)
{
	vector<string>res;
	istringstream iss(str);	// 输入流
	string token;			// 接收缓冲区
	while (getline(iss, token, split))	// 以split为分隔符
	{
		res.push_back(token);
	}
	return res;
}


PostProcessProfiling::PostProcessProfiling(QWidget* parent)
{
	m_pQVTKWidget = new QVTKWidget(this);
	setCentralWidget(m_pQVTKWidget);
	subZone_ = nullptr;
	branch = vtkSmartPointer<vtkMultiBlockDataSet>::New();
	subZoneWindowShowCallBack = vtkSmartPointer<vtkRightPressCallBack>::New();
	auto renderCollections = m_pQVTKWidget->GetRenderWindow()->GetRenderers();
	int32_t renderNum = renderCollections->GetNumberOfItems();
	qDebug() << "1 PostProcessProfiling::PostProcessProfiling m_pQVTKWidget->GetRenderWindow()->GetRenderers():" << renderNum;
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	camera->Zoom(0.5);

	// Renderer
	m_renderer = vtkSmartPointer<vtkRenderer>::New();
	m_renderer->SetActiveCamera(camera);
	m_renderer->SetBackground(1.0, 1.0, 1.0);                        // 设置页面底部颜色值
	m_renderer->SetBackground2(0.529, 0.8078, 0.92157);				 // 设置页面顶部颜色值
	m_renderer->SetGradientBackground(1);                            // 开启渐变色背景设置
	m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);
	m_pQVTKWidget->update();
	// ----
	renderCollections = m_pQVTKWidget->GetRenderWindow()->GetRenderers();
	renderNum = renderCollections->GetNumberOfItems();
	qDebug() << "2 AddRenderer PostProcessProfiling::PostProcessProfiling m_pQVTKWidget->GetRenderWindow()->GetRenderers():" << renderNum;
	// ----
	initVariables();
	initActions();
	initToolBars();
	initArrowActor();
	initConnection();
	loadStyleSheet("SSPNS");
	renderCollections = m_pQVTKWidget->GetRenderWindow()->GetRenderers();
	renderNum = renderCollections->GetNumberOfItems();
	qDebug() << "3 AddRenderer after PostProcessProfiling::PostProcessProfiling m_pQVTKWidget->GetRenderWindow()->GetRenderers():" << renderNum;
}

void PostProcessProfiling::addDataSet(vtkSmartPointer<vtkDataSet> dataSet)
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

void PostProcessProfiling::removeDataSet()
{
	//取得actor
	vtkActor* actor = m_renderer->GetActors()->GetLastActor();
	//删除已经有的actor
	if (actor != nullptr) {
		m_renderer->RemoveActor(actor);
	}
	if (clip_planeWidget) {
		clip_planeWidget->RemoveAllObservers();
		clip_planeWidget->Off();
	}
	if (boxWidget)
	{
		boxWidget->RemoveAllObservers();
		boxWidget->Off();
	}
	//渲染窗口,对窗口进行初始化
	m_pQVTKWidget->update();
}

vtkSmartPointer<vtkPolyData> PostProcessProfiling::vtkPltReader(const char* plt_file) {
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
	file_stream.close();

	return data_set;

}

bool PostProcessProfiling::LoadPlot3DFile(QString filename)
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
	plt_dataSet = vtkPltReader(pltFile.toLocal8Bit().toStdString().c_str());
	addDataSet(plt_dataSet);
	return true;
}

bool PostProcessProfiling::LoadVTKFile()
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

	vtk_reader->SetFileName(vtkFile.toLocal8Bit().toStdString().c_str());
	vtk_reader->Update();
	vtk_dataSet = vtkSmartPointer<vtkPolyData>::New();
	vtk_dataSet = vtk_reader->GetOutput();// vtkPltReader("bnd_morph00000001.plt");
	showVTKData();
	return true;
}

void PostProcessProfiling::showVTKData() {
	vtk_mapper->SetInputData(vtk_dataSet);
	vtk_actor->SetMapper(vtk_mapper);
	m_renderer->AddActor(vtk_actor);

	m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);
	m_pQVTKWidget->update();
	auto renderCollections = m_pQVTKWidget->GetRenderWindow()->GetRenderers();
	int32_t renderNum = renderCollections->GetNumberOfItems();
	qDebug() << "showVTKData() m_pQVTKWidget->GetRenderWindow()->GetRenderers():" << renderNum;
}

void PostProcessProfiling::zoomToExtent()
{
	// Zoom to extent of last added actor
	vtkSmartPointer<vtkActor> actor = m_renderer->GetActors()->GetLastActor();
	if (actor != nullptr) {
		m_renderer->ResetCamera(actor->GetBounds());
	}

	//renderWindow()->Render();
	m_pQVTKWidget->update();
}

void PostProcessProfiling::initActions() {
	//设置视图朝向X轴正向
	const QIcon setViewDirectionToXIcon = QIcon::fromTheme(tr("设置视图朝向X轴正向"), QIcon(":/binForPublic/images/setViewDirectionToX.png"));
	m_setViewDirectionToXAct = new QAction(setViewDirectionToXIcon, tr("设置视图朝向X轴正向(&X)"), this);
	m_setViewDirectionToXAct->setStatusTip(tr("设置视图朝向X轴正向"));
	connect(m_setViewDirectionToXAct, &QAction::triggered, this, &PostProcessProfiling::setViewDirectionToX);

	//设置视图朝向X轴负向
	const QIcon setViewDirectionToNegativeXIcon = QIcon::fromTheme(tr("设置视图朝向X轴负向"), QIcon(":/binForPublic/images/setViewDirectionToNegativeX.png"));
	m_setViewDirectionToNegativeXAct = new QAction(setViewDirectionToNegativeXIcon, tr("设置视图朝向X轴负向(&A)"), this);
	m_setViewDirectionToNegativeXAct->setStatusTip(tr("设置视图朝向X轴负向"));
	connect(m_setViewDirectionToNegativeXAct, &QAction::triggered, this, &PostProcessProfiling::setViewDirectionToNegativeX);

	//设置视图朝向Y轴正向
	const QIcon setViewDirectionToYIcon = QIcon::fromTheme(tr("设置视图朝向Y轴正向"), QIcon(":/binForPublic/images/setViewDirectionToY.png"));
	m_setViewDirectionToYAct = new QAction(setViewDirectionToYIcon, tr("设置视图朝向Y轴正向(&Y)"), this);
	m_setViewDirectionToYAct->setStatusTip(tr("设置视图朝向Y轴正向"));
	connect(m_setViewDirectionToYAct, &QAction::triggered, this, &PostProcessProfiling::setViewDirectionToY);

	//设置视图朝向Y轴负向
	const QIcon setViewDirectionToNegativeYIcon = QIcon::fromTheme(tr("设置视图朝向Y轴负向"), QIcon(":/binForPublic/images/setViewDirectionToNegativeY.png"));
	m_setViewDirectionToNegativeYAct = new QAction(setViewDirectionToNegativeYIcon, tr("设置视图朝向Y轴负向(&B)"), this);
	m_setViewDirectionToNegativeYAct->setStatusTip(tr("设置视图朝向Y轴负向"));
	connect(m_setViewDirectionToNegativeYAct, &QAction::triggered, this, &PostProcessProfiling::setViewDirectionToNegativeY);

	//设置视图朝向Z轴正向
	const QIcon setViewDirectionToZIcon = QIcon::fromTheme(tr("设置视图朝向Z轴正向"), QIcon(":/binForPublic/images/setViewDirectionToZ.png"));
	m_setViewDirectionToZAct = new QAction(setViewDirectionToZIcon, tr("设置视图朝向Z轴正向(&Z)"), this);
	m_setViewDirectionToZAct->setStatusTip(tr("设置视图朝向Z轴正向"));
	connect(m_setViewDirectionToZAct, &QAction::triggered, this, &PostProcessProfiling::setViewDirectionToZ);

	//设置视图朝向Z轴负向
	const QIcon setViewDirectionToNegativeZIcon = QIcon::fromTheme(tr("设置视图朝向Z轴负向"), QIcon(":/binForPublic/images/setViewDirectionToNegativeZ.png"));
	m_setViewDirectionToNegativeZAct = new QAction(setViewDirectionToNegativeZIcon, tr("设置视图朝向Z轴负向(&C)"), this);
	m_setViewDirectionToNegativeZAct->setStatusTip(tr("设置视图朝向Z轴负向"));
	connect(m_setViewDirectionToNegativeZAct, &QAction::triggered, this, &PostProcessProfiling::setViewDirectionToNegativeZ);

	//放大
	const QIcon amplificationViewIcon = QIcon::fromTheme(tr("放大"), QIcon(":/binForPublic/images/zoomin.png"));
	m_amplificationViewAct = new QAction(amplificationViewIcon, tr("放大(&Z)"), this);
	m_amplificationViewAct->setStatusTip(tr("放大"));
	connect(m_amplificationViewAct, &QAction::triggered, this, &PostProcessProfiling::amplificationView);

	//缩小
	const QIcon narrowViewIcon = QIcon::fromTheme(tr("缩小"), QIcon(":/binForPublic/images/zoomout.png"));
	m_narrowViewAct = new QAction(narrowViewIcon, tr("缩小(&Z)"), this);
	m_narrowViewAct->setStatusTip(tr("缩小"));
	connect(m_narrowViewAct, &QAction::triggered, this, &PostProcessProfiling::narrowView);

	//选择文件
	const QIcon fileOpSelectIcon = QIcon::fromTheme(tr("文件选择"), QIcon(":/binForPublic/images/SSPNS/fileopen.png"));
	m_fileOpSelectAct = new QAction(fileOpSelectIcon, tr("文件选择"), this);
	m_fileOpSelectAct->setStatusTip(tr("文件选择"));
	connect(m_fileOpSelectAct, &QAction::triggered, this, [this]() {
		QString fileName = QFileDialog::getOpenFileName(this, u8"文件选择", appComDir + "/ui/ui.body");
		/*QString prefix = FileOpUtils::getInstance()->fileToPrefix(fileName);
		QString suffix = FileOpUtils::getInstance()->fileToSuffix(fileName);*/

		// 使用QFileInfo处理路径，确保中文路径正确解析
		QFileInfo fileInfo(fileName);
		QString prefix = fileInfo.baseName(); // 获取不带扩展名的文件名（自动处理中文）
		QString suffix = fileInfo.suffix().toLower(); // 获取扩展名（小写处理）

		bool boolIsDomain = false;
		if (prefix.size() >= 6) {
			boolIsDomain = prefix.toLocal8Bit().toStdString().substr(0, 6) == "Domain";
		}
		
		qDebug() << "suffix=" << suffix << " prefix=" << prefix << " boolIsDomain" << boolIsDomain;
		if (prefix == "simplification"&&suffix == "vtk")
		{
			vtkSmartPointer<vtkPolyDataReader> readerPoly = vtkSmartPointer<vtkPolyDataReader>::New();
			readerPoly->SetFileName(fileName.toLocal8Bit().toStdString().c_str());
			readerPoly->ReadAllScalarsOn(); // 确保读取标量值
			readerPoly->ReadAllFieldsOn();
			readerPoly->ReadAllColorScalarsOn();
			readerPoly->Update();
			vtk_dataSet = vtkSmartPointer<vtkPolyData>::New();
			vtk_dataSet->DeepCopy(readerPoly->GetOutput());
			showVTKData();
			planeChooseSlot(u8"原始网格数据");
		}
		else {
			if (suffix == "vtk")
			{
				resetVtkData();
				dataIsLoad = true;
				resetAttribute();  // 添加此行
				parseVolumeVtkFile(fileName);
				updateDataSetFromUnstructedGrid(slice_OriginGrid);
				showVTKData();
				planeChooseSlot(u8"原始网格数据");
			}
			else {
				if (fileName.size() && prefix == "bdField")
				{
					resetVtkData();
					dataIsLoad = true;
					if (slice_OriginGrid) {
						slice_OriginGrid = nullptr;
					}
					resetAttribute();  // 添加此行
					parseBDFiledFile(fileName);
					planeChooseSlot(u8"原始网格数据");
				}
				else if (fileName.size() && prefix == "block")
				{
					auto renderCollections = m_pQVTKWidget->GetRenderWindow()->GetRenderers();
					int32_t renderNum = renderCollections->GetNumberOfItems();
					qDebug() << "4 parse block before PostProcessProfiling::PostProcessProfiling m_pQVTKWidget->GetRenderWindow()->GetRenderers():" << renderNum;
					resetVtkData();
					dataIsLoad = true;
					resetAttribute();  // 添加此行
					slice_OriginGrid = parseBlockFileVtkFile(fileName);
					updateDataSetFromUnstructedGrid(slice_OriginGrid);
					showVTKData();
					planeChooseSlot(u8"原始网格数据");
					//slotShowSlicePostProcess();
					renderCollections = m_pQVTKWidget->GetRenderWindow()->GetRenderers();
					renderNum = renderCollections->GetNumberOfItems();
					qDebug() << "5 parse block after PostProcessProfiling::PostProcessProfiling m_pQVTKWidget->GetRenderWindow()->GetRenderers():" << renderNum;
				}
				else if ((fileName.size() && prefix == "Domain1") || boolIsDomain)
				{
					resetVtkData();
					if (slice_OriginGrid) {
						slice_OriginGrid = nullptr;
					}
					dataIsLoad = true;
					resetAttribute();  // 添加此行
					parseDomain1File(fileName);
					planeChooseSlot(u8"原始网格数据");
				}
				else {
					if (fileName == "")return;
					launchWarning(u8"目前没有支持打开" + fileName + u8"的网格解析器");
					return;
				}
			}
		}
	});

	//保存文件
	const QIcon pngSaveIcon = QIcon::fromTheme(tr("文件保存"), QIcon(":/binForPublic/images/save.png"));
	m_pngSaveAct = new QAction(pngSaveIcon, tr("图像保存"), this);
	m_pngSaveAct->setStatusTip(tr("文件保存"));


	//切割
	const QIcon clipIcon = QIcon::fromTheme(tr("切割Clip"), QIcon(":/binForPublic/images/gridr.png"));
	m_clipAct = new QAction(clipIcon, tr("网格分析方法Clip(&C)"), this);
	m_clipAct->setStatusTip(tr("网格分析方法Clip"));
	connect(m_clipAct, &QAction::triggered, this, &PostProcessProfiling::slotShowClipPostProcess);


	//切片
	const QIcon sliceIcon = QIcon::fromTheme(tr("切片Slice"), QIcon(":/binForPublic/images/gridl.png"));
	m_sliceAct = new QAction(sliceIcon, tr("网格分析方法Slice(&T)"), this);
	m_sliceAct->setStatusTip(tr("网格分析方法Slice"));
	connect(m_sliceAct, &QAction::triggered, this, &PostProcessProfiling::slotShowSlicePostProcess);

	//Box切割
	const QIcon boxClipIcon = QIcon::fromTheme(tr("Box切割"), QIcon(":/binForPublic/images/gridr.png"));
	m_boxClipAct = new QAction(boxClipIcon, tr("网格分析方法Box切割(&B)"), this);
	m_boxClipAct->setStatusTip(tr("网格分析方法Box切割"));
	connect(m_boxClipAct, &QAction::triggered, this, &PostProcessProfiling::slotShowBoxClipPostProcess);
}

void PostProcessProfiling::ViewDirection(
	double lookX, double lookY, double lookZ,
	double upX, double upY, double upZ)
{
	vtkRenderer *renderer = m_renderer;
	renderer->GetActiveCamera()->SetPosition(0, 0, 0);    //相机位置
	renderer->GetActiveCamera()->SetFocalPoint(lookX, lookY, lookZ);    //焦点位置
	renderer->GetActiveCamera()->SetViewUp(upX, upY, upZ);    //朝上方向
	renderer->ResetCamera();
}
void PostProcessProfiling :: setViewDirectionToX() {

	ViewDirection( -1, 0, 0, 0, 0, 1);
	// resetCamera(true);
	m_pQVTKWidget->update();
}

void PostProcessProfiling::setViewDirectionToNegativeX() {

	ViewDirection(1, 0, 0, 0, 0, 1);
	m_pQVTKWidget->update();
}

void PostProcessProfiling::setViewDirectionToY() {

	ViewDirection( 0, -1, 0, 0, 0, 1);
	m_pQVTKWidget->update();
}

void PostProcessProfiling::setViewDirectionToNegativeY() {
	ViewDirection( 0, 1, 0, 0, 0, 1);
	m_pQVTKWidget->update();
}

void PostProcessProfiling ::setViewDirectionToZ() {
	ViewDirection( 0, 0, -1, 0, 1, 0);
	m_pQVTKWidget->update();
}

void PostProcessProfiling::setViewDirectionToNegativeZ() {
	ViewDirection( 0, 0, 1, 0, 1, 0);
	m_pQVTKWidget->update();
}


void PostProcessProfiling::amplificationView() {
	vtkCamera* curCamera = m_renderer->GetActiveCamera();
	curCamera->Zoom(1.01); //微调
	m_pQVTKWidget->update();
}

void PostProcessProfiling::narrowView() {
	vtkCamera* curCamera = m_renderer->GetActiveCamera();
	curCamera->Zoom(0.99); //微调
	m_pQVTKWidget->update();
}


void PostProcessProfiling::initToolBars() {

	m_fileOpToolBar = addToolBar(tr("文件操作工具栏"));
	m_fileOpToolBar->addAction(m_fileOpSelectAct);
	//m_fileOpToolBar->addAction(m_pngSaveAct);

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




	m_postFunctionToolBar = addToolBar(tr("后处理工具栏"));
	// 包含clip和slice....
	initPostProcessFunctionToolBar();
	m_postFunctionToolBar->addSeparator();
	m_postFunctionToolBar->addSeparator();
	m_postFunctionToolBar->addWidget(m_showClipBtn);
	m_postFunctionToolBar->addWidget(m_clipBtn);
	m_postFunctionToolBar->addSeparator();
	m_postFunctionToolBar->addSeparator();
	m_postFunctionToolBar->addWidget(m_showBoxClipBtn);
	m_postFunctionToolBar->addWidget(m_boxClipBtn);
	m_postFunctionToolBar->addSeparator();
	m_postFunctionToolBar->addSeparator();
	m_postFunctionToolBar->addWidget(m_showSliceBtn);
	m_postFunctionToolBar->addSeparator();
	initResetDataToolBtn();
}

void PostProcessProfiling::initClipUnit()
{
	clip_plane = vtkSmartPointer<vtkPlane>::New();
	clipper = vtkSmartPointer<vtkClipPolyData>::New();
	clip_rep = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
	clip_planeWidget = vtkSmartPointer<vtkImplicitPlaneWidget2>::New();

}

void PostProcessProfiling::initSliceUnit()
{
	if (slice_OriginGrid == nullptr) {
		slice_OriginGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	}
	slice_mapper1 = vtkSmartPointer<vtkDataSetMapper>::New();
	slice_mapper2 = vtkSmartPointer<vtkDataSetMapper>::New();
	slice_actor1 = vtkSmartPointer<vtkActor>::New();
	slice_actor2 = vtkSmartPointer<vtkActor>::New();
	slice_plane = vtkSmartPointer<vtkPlane>::New();
	slice_clipper = vtkSmartPointer<vtkClipPolyData>::New();
	slice_rep = vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();

	// slice_planeWidget = vtkSmartPointer<vtkPlaneWidget>::New(); // <-- 删除或注释掉这行
	slice_planeWidget = vtkSmartPointer<MySimplePlaneWidget>::New(); // <-- 改成这行

	slice_cutter = vtkSmartPointer<vtkCutter>::New();
	sliceShowRender = vtkSmartPointer<vtkRenderer>::New();
	sliceOriginRender = vtkSmartPointer<vtkRenderer>::New();
	mySliceCallback = vtkSmartPointer<vtkSliceCallback>::New();


	//-------------------------------
	slice_Interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	slice_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
}

void PostProcessProfiling::resetCamera(bool bCompulsion) {
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

void PostProcessProfiling::initVariables() {
	m_bResetCamera = true;
	m_attributeSelMapper = nullptr;
	m_attributeToolBar = nullptr;
	m_attributeSelToolBtn = nullptr;
	m_displayModeMapper = nullptr;
	m_displayModeToolBtn = nullptr;
	m_visualOpToolBar = nullptr;
	isVTKData = 1;
	m_legendMapper = nullptr;
	m_legendToolBtn = nullptr;
	m_planeToolBtn = nullptr;
	m_planeMapper = nullptr;
	m_curShowAttrType = ""; // 原为 "POINT"，改为空表示无属性
	//m_curShowAttrType = "POINT";
	dataIsLoad = false;

	m_resetDataToolBtn = nullptr;
	m_opencvToolBtn = nullptr;
}

void PostProcessProfiling::initFileOpToolBar()
{
}

void PostProcessProfiling::initArrowActor() {
	m_widgetAxes->SetOrientationMarker(axesActor);
	m_widgetAxes->SetInteractor(m_pQVTKWidget->GetRenderWindow()->GetInteractor());
	m_widgetAxes->SetEnabled(1);
	m_widgetAxes->InteractiveOn();
	m_pQVTKWidget->update();
}

void PostProcessProfiling::initAttributeSelToolBar() {
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

void PostProcessProfiling::initResetDataToolBtn()
{
	if (m_resetDataToolBtn == nullptr)
	{
		m_resetDataToolBtn = new QToolButton(this);
		m_postFunctionToolBar->addWidget(m_resetDataToolBtn);
	}
	//if (m_opencvToolBtn == nullptr)
	//{
	//	m_opencvToolBtn = new QToolButton(this);
	//	m_postFunctionToolBar->addSeparator();
	//	m_postFunctionToolBar->addWidget(m_opencvToolBtn);
	//	m_opencvToolBtn->setText(u8"智能分析");
	//}
	m_resetDataToolBtn->setText(QString(tr("重置网格数据")));
	connect(m_resetDataToolBtn, &QPushButton::clicked, this, [=]()
	{
		resetVtkData();
		showVTKData();
	});
	m_clipBtn->setEnabled(false);
	m_boxClipBtn->setEnabled(false);
	m_sliceBtn->setEnabled(false);
}

void PostProcessProfiling::attributeSelSlot(QString attribute) {
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

void PostProcessProfiling::SetActivateAttributeToMapperForVtk(vtkSmartPointer<vtkDataSetMapper> mapper) {
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
	m_pQVTKWidget->GetRenderWindow()->Render();
}

void PostProcessProfiling::addScalarBarActorForVtk(vtkSmartPointer<vtkDataSetMapper> mapper, string title, double * range) {
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

void PostProcessProfiling::initDisplayModeToolBar() {
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
	m_displayModeToolBtn->setIcon(QIcon::fromTheme(tr("显示模式"),
		QIcon((":/binForPublic/images/" + vecDisplayMode[0] + ".png").c_str())));
	m_displayModeToolBtn->setText(QString(tr("显示模式")));

	// 为QToolButton设置菜单
	m_displayModeToolBtn->setMenu(vecAction.empty() ? nullptr : menu);
	// m_curShowMeshMode = "表面模式";
}

void PostProcessProfiling::displayModeSlot(QString mode) {
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
	initConnection();
}

void PostProcessProfiling::initUI() {
	// 注释或修改原有强制设置属性的代码
	// SetActivateAttributeToMapperForVtk(vtk_mapper); 
	// 改为：仅当存在属性时才设置
	
	if (!m_curShowAttrName.empty() && !m_curShowAttrType.empty()) {
		SetActivateAttributeToMapperForVtk(vtk_mapper);
	}
	else {
		if (vtk_mapper) {
			vtk_mapper->SetScalarVisibility(false); // 确保无属性时不显示
		}
	}
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

void PostProcessProfiling::initConnection()
{
	connect(SignalInstance::getInstance(), SIGNAL(signalZonesVisbleChanged(QVector<int32_t>)),
		this, SLOT(slotZonesChanged(QVector<int32_t>)));
}

vtkSmartPointer<vtkActor> PostProcessProfiling::addOutlineActorForVtk(vtkSmartPointer<vtkOutlineFilter> outlineData) {
	return addOutlineActor(outlineData);
}

vtkSmartPointer<vtkActor> PostProcessProfiling::addOutlineActor(vtkSmartPointer<vtkOutlineFilter> outlineData) {
	vtkSmartPointer<vtkPolyDataMapper> mapOutline = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapOutline->SetInputConnection(outlineData->GetOutputPort());

	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	outlineActor->SetMapper(mapOutline);
	outlineActor->GetProperty()->SetColor(1, 0, 0);

	m_renderer->AddActor(outlineActor);
	m_pQVTKWidget->update();

	return outlineActor;
}

vtkSmartPointer<vtkActor> PostProcessProfiling::addPointsActorForVtk(vtkSmartPointer<vtkDataSet> dataSet) {
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

vtkSmartPointer<vtkActor> PostProcessProfiling::addWireFrameActorForVtk(vtkSmartPointer<vtkDataSet> dataSet, bool bBlack) {
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

void PostProcessProfiling::updateAttributeNameMap(vtkSmartPointer<vtkPolyData> mesh) {
	if (!mesh) return;
	m_mapAttrNames.clear();

	//mesh->GetAttributes(vtkDataSet::POINT);
	vector<string> vecAttrNames1;
	auto pPoint = mesh->GetAttributes(vtkDataSet::POINT);
	for (int i = 0; i < pPoint->GetNumberOfArrays(); ++i) {
		vecAttrNames1.push_back(pPoint->GetArrayName(i));
	}
	m_mapAttrNames["POINT"] = vecAttrNames1;

	// 解析单元格数据（CELL）
	vector<string> vecAttrNames2;
	auto pCell = mesh->GetAttributes(vtkDataSet::CELL);
	if (pCell) {
		qDebug() << "Cell data array count:" << pCell->GetNumberOfArrays();
		for (int i = 0; i < pCell->GetNumberOfArrays(); ++i) {
			vtkDataArray* arr = pCell->GetArray(i);  // 获取数组指针
			if (!arr) {  // 关键：检查数组是否为空
				qDebug() << "Invalid array at index" << i;
				continue;  // 跳过无效数组，避免崩溃
			}
			const char* arrName = arr->GetName();  // 再访问数组名称
			if (arrName) {
				vecAttrNames2.push_back(arrName);
				qDebug() << "Array" << i << "name:" << arrName;
			}
			else {
				qDebug() << "Array" << i << "has no name";
				//vecAttrNames2.push_back("Unnamed");  // 处理无名称的情况
			}
		}
	}
	m_mapAttrNames["CELL"] = vecAttrNames2;  // 即使为空也存入，避免后续访问异常

	// 解析字段数据（FIELD）
	vector<string> vecAttrNames3;
	auto pField = mesh->GetAttributesAsFieldData(vtkDataSet::FIELD);
	if (pField) {  // 校验pField有效性
		for (int i = 0; i < pField->GetNumberOfArrays(); ++i) {
			vecAttrNames3.push_back(pField->GetArrayName(i));
		}
	}
	m_mapAttrNames["FIELD"] = vecAttrNames3;
	
	
	
}

void PostProcessProfiling::initPlaneChooseToolBar() {
	if (m_planeMapper) {
		disconnect(m_planeMapper, SIGNAL(mapped(QString)), this, SLOT(planeChooseSlot(QString)));
	}
	m_planeMapper = new QSignalMapper(this);
	const vector<string> vecPlane = {
		"原始网格数据"/*,
		"最新动网格数据"*/
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

void PostProcessProfiling::legendSlot(QString legend) {
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

void PostProcessProfiling::planeChooseSlot(QString pickName) {
	if (pickName == "原始网格数据") {
		isVTKData = 1;
		updateAttributeNameMap(vtk_dataSet);
		resetAttribute();  // 先重置
		initAttributeSelToolBar();
		m_planeToolBtn->setText(QString(tr("原始网格数据")));
	}
	else if (pickName == "最新动网格数据") {
		isVTKData = 0;
		updateAttributeNameMap(plt_dataSet);
		resetAttribute();  // 先重置
		initAttributeSelToolBar();
		m_planeToolBtn->setText(QString(tr("最新动网格数据")));
	}
	else {
	}
}

void PostProcessProfiling::initLegendToolBar() {
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

void PostProcessProfiling::loadStyleSheet(QString sheetName)
{
	QFile file(appComDir + "/images/SSPNS/CSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);
	if (file.isOpen()) {
		setStyleSheet("");
		//将样式表全部读取，保存到字符串中
		QString qsstyleSheet = QLatin1String(file.readAll());

		setStyleSheet(qsstyleSheet);
		//qDebug() << "qsstyleSheet:" << qsstyleSheet;
	}
	file.close();
}
void PostProcessProfiling::resetAttribute() {
	// 清除当前属性状态
	m_curShowAttrName = "";
	m_curShowAttrType = "";

	// 更新UI
	if (m_attributeSelToolBtn) {
		m_attributeSelToolBtn->setIcon(QIcon("images/default_attr.png"));
		m_attributeSelToolBtn->setText(QString(tr("属性选择")));
	}

	// 禁用标量显示
	if (vtk_mapper) {
		vtk_mapper->SetScalarVisibility(false);
	}
	if (plt_mapper) {  // 如果有其他mapper也需要处理
		plt_mapper->SetScalarVisibility(false);
	}
	// 移除标量条演员（关键修改：取消图例显示）
	if (m_scalarBarActor) {
		m_renderer->RemoveActor(m_scalarBarActor);
		m_scalarBarActor = nullptr; // 可选：彻底释放资源
	}
	// 刷新渲染
	if (m_pQVTKWidget) {
		m_pQVTKWidget->GetRenderWindow()->Render();
	}
}
vtkSmartPointer<vtkPolyData> PostProcessProfiling::parseBDFiledFile(QString fileName)
{
	auto *progressDialog = new PostProgressDialog(u8"提示", u8"正在解析网格数据文件....");
	progressDialog->show();
	//emit SignalInstance::getInstance()->signalShowDialog(u8"loading....", u8"正在渲染网格，请等待渲染完成......");

	std::cout << __FUNCTION__ << std::endl;
	int PointsNum = 0;
	std::string filename = fileName.toLocal8Bit().toStdString(); // 你的数据文件名

	// 2. 转换中文路径为系统本地编码（关键修复）
	QByteArray localPath = QFile::encodeName(fileName); // 转换为系统兼容编码（如Windows GBK）
	const char* filePath = localPath.constData();

	// 3. 输出路径调试信息（便于定位编码问题）
	qDebug() << u8"原始路径：" << fileName;
	qDebug() << u8"转换后路径：" << localPath;

	// 4. 尝试打开文件（使用转换后的路径）
	std::ifstream data_file(filePath);
	if (!data_file.is_open()) {
		// 详细错误提示：包含系统错误码
		qDebug() << "无法打开文件，错误原因：" << strerror(errno);
		return nullptr;
	}

	// 读取变量名
	std::string line;
	std::getline(data_file, line);
	std::istringstream var_stream(line);
	std::string var;
	std::vector<std::string> variable_names(14);
	variable_names[0] = "x";
	variable_names[1] = "y";
	variable_names[2] = "z";
	variable_names[3] = "u";
	variable_names[4] = "v";
	variable_names[5] = "w";
	variable_names[6] = "p";
	variable_names[7] = "t";
	variable_names[8] = "rho";
	variable_names[9] = "ma";
	variable_names[10] = "heatflux";
	variable_names[11] = "taux";
	variable_names[12] = "tauy";
	variable_names[13] = "tauz";
	int n = 0;
	int zoneNum = 0;
	vector<int>zonePoints;
	vector<vector<vector<double>>*>Points;
	vector<vtkSmartPointer<vtkStructuredGrid>>structuredGrids;
	QVector<QString>zoneNames_;
	while (std::getline(data_file, line)) {
		n++;
		//cout << line << endl;
		// 过滤掉ZONE信息
		vector<string> res = Stringsplit(line, ' ');
		if (res[1] == "ZONE")
		{
			vtkNew<vtkPoints> structedPoints;

			//cout << n << endl;
			// 获取I J范围
			vector<string> res1 = Stringsplit(line, '=');
			string zoneName = Stringsplit(line, '"')[1];
			zoneNames_.push_back(QString::fromStdString(zoneName));
			// RES1[3]直接转为J
			// RES1[2]逗号分割转为I
			int32_t J = atoi(res1[3].c_str());
			vector<string>res2 = Stringsplit(res1[2], ',');
			int32_t I = atoi(res2[0].c_str());
			vector<vector<double>>* Point = new vector<vector<double>>(14, vector<double>(I * J));

			// 正则表达式匹配数值：支持正负号、整数/小数、科学计数法（E/e或缺失E的情况）
			std::regex num_regex(R"([+-]?\d*\.?\d+(?:[eE]?[+-]?\d+)?)");

			for (int row = 0; row < I * J; row++)
			{
				n++;
				std::getline(data_file, line);
				if (line.empty()) continue;

				int index = 0;
				size_t pos = 0;
				const size_t len = line.size();

				// 手动分割数值（按空格）
				while (pos < len && index < 14) {
					// 跳过空格
					while (pos < len && isspace(line[pos])) pos++;
					if (pos >= len) break;

					// 提取数值字符串（从pos开始到下一个空格）
					size_t start = pos;
					while (pos < len && !isspace(line[pos])) pos++;
					std::string num_str = line.substr(start, pos - start);

					// 修复缺失'E'的科学计数法（如"-0.2827757-297" -> "-0.2827757E-297"）
					if (!num_str.empty()) {
						// 查找最后一个'+'或'-'（排除第一个字符）
						size_t last_sign = num_str.find_last_of("+-", num_str.size() - 1);
						if (last_sign != std::string::npos && last_sign > 0) {
							char prev = num_str[last_sign - 1];
							if (prev != 'e' && prev != 'E') {
								num_str.insert(last_sign, "E"); // 插入'E'
							}
						}
					}

					// 转换为double（用c_str() + atof避免异常）
					double property = atof(num_str.c_str()); // atof无异常，效率高于stod
					(*Point)[index++][row] = property;
				}

				// 检查是否读取到14个数值
				if (index != 14) {
					std::cerr << "警告：行 " << n << " 只读取到 " << index << " 个数值（预期14个）" << std::endl;
				}

				// 提取坐标（即使数值不足14个，避免越界）
				double x = (index > 0) ? (*Point)[0][row] : 0.0;
				double y = (index > 1) ? (*Point)[1][row] : 0.0;
				double z = (index > 2) ? (*Point)[2][row] : 0.0;
				structedPoints->InsertNextPoint(x, y, z);
			}

			/*for (int row = 0; row < I*J; row++)
			{
				n++;
				std::getline(data_file, line);
				double property;
				stringstream ss(line);
				int index = 0;
				while (ss >> property)
				{
					(*Point)[index++][row] = property;

				}
				double x = (*Point)[0][row];
				double y = (*Point)[1][row];
				double z = (*Point)[2][row];
				structedPoints->InsertNextPoint(x, y, z);
			}*/


			Points.push_back(Point);
			PointsNum += I * J;
			zonePoints.push_back(I * J);

			// set Cell
			auto numberOfCells = (I - 1) * (J - 1);
			vtkNew<vtkDoubleArray> cellValues;
			cellValues->SetNumberOfTuples(numberOfCells);
			for (size_t i = 0; i < numberOfCells; ++i)
			{
				cellValues->SetValue(i, i);
			}
			// Create a grid
			vtkNew<vtkStructuredGrid> structuredGrid;
			// Specify the dimensions of the grid
			structuredGrid->SetDimensions(static_cast<int>(I), static_cast<int>(J),
				static_cast<int>(1));

			structuredGrid->SetPoints(structedPoints);
			structuredGrid->GetCellData()->SetScalars(cellValues);

			//-----------------------------------------------
			/*
			set property
			*/
			// "u" "v" "w" "p" "t" "rho" "ma" "heatflux" "taux" "tauy" "tauz"
			vtkSmartPointer<vtkDoubleArray> FES_U = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_V = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_W = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_P = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_T = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_RHO = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_MA = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_HEAT_FLUE = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_TAU_X = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_TAU_Y = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_TAU_Z = vtkSmartPointer<vtkDoubleArray>::New();
			FES_U->SetName("U");
			FES_V->SetName("V");
			FES_W->SetName("W");
			FES_P->SetName("P");
			FES_T->SetName("T");
			FES_RHO->SetName("RHO");
			FES_MA->SetName("MA");
			FES_HEAT_FLUE->SetName("HEATFLUE");
			FES_TAU_X->SetName("TAUX");
			FES_TAU_Y->SetName("TAUY");
			FES_TAU_Z->SetName("TAUZ");
			for (int i = 0; i < structuredGrid->GetNumberOfPoints(); i++) {
				FES_U->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_U->SetValue(i, (*Point)[3][i]);
				structuredGrid->GetPointData()->SetScalars(FES_U);

				FES_V->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_V->SetValue(i, (*Point)[4][i]);
				structuredGrid->GetPointData()->SetScalars(FES_V);

				FES_W->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_W->SetValue(i, (*Point)[5][i]);
				structuredGrid->GetPointData()->SetScalars(FES_W);

				FES_P->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_P->SetValue(i, (*Point)[6][i]);
				structuredGrid->GetPointData()->SetScalars(FES_P);

				FES_T->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_T->SetValue(i, (*Point)[7][i]);
				structuredGrid->GetPointData()->SetScalars(FES_T);

				FES_RHO->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_RHO->SetValue(i, (*Point)[8][i]);
				structuredGrid->GetPointData()->SetScalars(FES_RHO);

				FES_MA->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_MA->SetValue(i, (*Point)[9][i]);
				structuredGrid->GetPointData()->SetScalars(FES_MA);

				FES_HEAT_FLUE->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_HEAT_FLUE->SetValue(i, (*Point)[10][i]);
				structuredGrid->GetPointData()->SetScalars(FES_HEAT_FLUE);

				FES_TAU_X->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_TAU_X->SetValue(i, (*Point)[11][i]);
				structuredGrid->GetPointData()->SetScalars(FES_TAU_X);

				FES_TAU_Y->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_TAU_Y->SetValue(i, (*Point)[12][i]);
				structuredGrid->GetPointData()->SetScalars(FES_TAU_Y);

				FES_TAU_Z->SetNumberOfValues(structuredGrid->GetNumberOfPoints());
				FES_TAU_Z->SetValue(i, (*Point)[13][i]);
				structuredGrid->GetPointData()->SetScalars(FES_TAU_Z);
			}
			structuredGrid->GetPointData()->AddArray(FES_P);
			structuredGrid->GetPointData()->AddArray(FES_U);
			structuredGrid->GetPointData()->AddArray(FES_V);
			structuredGrid->GetPointData()->AddArray(FES_W);
			structuredGrid->GetPointData()->AddArray(FES_T);
			structuredGrid->GetPointData()->AddArray(FES_HEAT_FLUE);
			structuredGrid->GetPointData()->AddArray(FES_RHO);
			structuredGrid->GetPointData()->AddArray(FES_MA);
			structuredGrid->GetPointData()->AddArray(FES_TAU_X);
			structuredGrid->GetPointData()->AddArray(FES_TAU_Y);
			structuredGrid->GetPointData()->AddArray(FES_TAU_Z);

			//-----------------------------------------------
			branch->SetBlock(zoneNum, structuredGrid);
			zoneNum++;

		}
	}
	// 创建一个vtkAppendPolyData来合并所有的PolyData块
	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();

	// 遍历multiBlockDataSet中的每个块
	for (unsigned int i = 0; i < branch->GetNumberOfBlocks(); i++)
	{
		vtkDataSet* dataSet = vtkDataSet::SafeDownCast(branch->GetBlock(i));
		if (dataSet)
		{
			// 如果块是vtkPolyData，直接添加到appendFilter
			if (vtkPolyData::SafeDownCast(dataSet))
			{
				//appendFilter->AddInputData(dataSet);
				appendFilter->AddInputDataObject(dataSet);
			}
			// 如果块不是vtkPolyData，使用DataSetSurfaceFilter转换它
			else
			{
				vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
				surfaceFilter->SetInputData(dataSet);
				surfaceFilter->Update();
				appendFilter->AddInputData(surfaceFilter->GetOutput());
			}
		}
	}

	// 更新appendFilter以合并所有的PolyData
	appendFilter->Update();

	// 获取合并后的PolyData
	vtkPolyData* mergedPolyData = appendFilter->GetOutput();
	vtk_dataSet = vtkSmartPointer<vtkPolyData>::New();
	vtk_latestData = vtkSmartPointer<vtkPolyData>::New();
	vtk_dataSet = mergedPolyData;
	showVTKData();
	progressDialog->close();
	//emit SignalInstance::getInstance()->signalCloseDialog();
	if (subZone_ == nullptr)
	{
		subZone_ = new SubZoneWindow(zoneNames_.size(), zoneNames_, nullptr);
		subZoneWindowShowCallBack->zoneWindow_ = subZone_;
		m_pQVTKWidget->GetInteractor()->AddObserver(vtkCommand::RightButtonPressEvent, subZoneWindowShowCallBack);
	}
	else {
		subZone_->setZoneNum(zoneNames_.size());
		subZone_->setZoneNames(zoneNames_);
		subZone_->update();
	}

	subZone_->okBtn_->click();
	return mergedPolyData;
}

vtkSmartPointer<vtkPolyData> PostProcessProfiling::parseDomain1File(QString fileName)
{
	auto *progressDialog = new PostProgressDialog(u8"提示", u8"正在解析网格数据文件....");
	progressDialog->setTitle(u8"loading....", u8"正在解析网格数据文件....");
	progressDialog->show();
	//emit SignalInstance::getInstance()->signalShowDialog(u8"loading....", u8"正在渲染网格，请等待渲染完成......");

	std::string path;
	path = fileName.toLocal8Bit().toStdString();
	std::cout << "__FUNCTION_:" << __FUNCTION__ << std::endl;
	//std::ifstream data_file(path);
	// 2. 转换中文路径为系统本地编码（关键修复）
	QByteArray localPath = QFile::encodeName(fileName); // 转换为系统兼容编码（如Windows GBK）
	const char* filePath = localPath.constData();

	// 3. 输出路径调试信息（便于定位编码问题）
	qDebug() << u8"原始路径：" << fileName;
	qDebug() << u8"转换后路径：" << localPath;

	// 4. 尝试打开文件（使用转换后的路径）
	std::ifstream data_file(filePath);
	if (!data_file.is_open()) {
		// 详细错误提示：包含系统错误码
		qDebug() << "无法打开文件，错误原因：" << strerror(errno);
		return nullptr;
	}

	// 读取变量名
	/*
	x y z
	 u v w p rho t gamma c mach
	 Lmu k
	*/
	std::string line;
	std::getline(data_file, line); // 读取xyz
	int index = 0;
	std::vector<std::string> variable_names(14);
	//X Y Z Ma U V W p T Rho OMG GAMMA
	variable_names[0] = "X";
	variable_names[1] = "Y";
	variable_names[2] = "Z";
	variable_names[3] = "Ma";
	variable_names[4] = "U";
	variable_names[5] = "V";
	variable_names[6] = "W";
	variable_names[7] = "p";
	variable_names[8] = "T";
	variable_names[9] = "Rho";
	variable_names[10] = "OMG";
	variable_names[11] = "GAMMA";
	int n = 0;
	int zoneNum = 0;
	vector<int>zonePoints;
	vector<vector<vector<double>>*>Points;
	vector<vtkSmartPointer<vtkStructuredGrid>>structuredGrids;
	QVector<QString>zoneNames_;
	// vtkNew<vtkMultiBlockDataSet> branch;
	//vtkSmartPointer<vtkAppendFilter> appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
	while (std::getline(data_file, line)) {
		n++;
		string lineTmp = "";
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] != '\0')
			{
				lineTmp += line[i];
			}
		}
		line = lineTmp;
		vector<string> res = Stringsplit(line, ' ');
		if (res.size() == 0)break;
		// zone T="  1",I=   25,J=   71,K=   24,DATAPACKING=BLOCK,VARLOCATION=([4-14]=CELLCENTERED)
		if (res[1] == "ZONE")
		{
			vtkNew<vtkPoints> structedPoints;
			string zoneName = Stringsplit(line, '"')[1];
			zoneNames_.push_back(QString::fromStdString(zoneName));
			// 解析I J K
			vector<string> IJK = Stringsplit(line, ',');
			string tmp;
			int I, J, K;
			stringstream ssi(IJK[1]);
			std::string istr = IJK[1].substr(5, IJK[1].size() - 5).c_str();
			I = atoi(IJK[1].substr(5, IJK[1].size() - 5).c_str());
			J = atoi(IJK[2].substr(5, IJK[2].size() - 5).c_str());
			J = atoi(IJK[2].substr(5, IJK[2].size() - 5).c_str());
			K = atoi(IJK[3].substr(5, IJK[3].size() - 5).c_str());
			std::cout << "I:" << I << " J:" << J << " K:" << K << std::endl;
			int index = 0;
			int sumData = I * J * K * 12;
			vector<vector<double>>* Point = new vector<vector<double>>(12, vector<double>(I*J*K));
			// 读取71 * 71
			for (long long row = 0; row < I * J * K; row++)
			{
				n++;
				std::getline(data_file, line);

				line.erase(remove(line.begin(), line.end(), '\0'), line.end());
				line.erase(0, line.find_first_not_of(" "));
				line.erase(line.find_last_not_of(" ") + 1);
				replace(line.begin(), line.end(), 'D', 'E');//将逗号替换为空格
				double property;
				vector<string> properties = Stringsplit(line, ' ');
				int index = 0;
				for (int i = 0; i < properties.size(); i++)
				{
					string tmp = properties[i];

					if (tmp.size() != 0)
					{
						stringstream ss(tmp);
						ss >> property;
						(*Point)[index++][row] = property;
					}
				}
				double x = (*Point)[0][row];
				double y = (*Point)[1][row];
				double z = (*Point)[2][row];
				structedPoints->InsertNextPoint(x, y, z);
				if (n % 50000 == 0)
				{
					std::cout << n << " x:" << x << " y:" << y << " z:" << z << std::endl;
				}
			}
			std::cout << structedPoints->GetNumberOfPoints() << std::endl;
			Points.push_back(Point);
			zonePoints.push_back(I*J*K);

			// set Cell
			auto numberOfCells = (I - 1) * (J - 1)*(K - 1);
			vtkNew<vtkDoubleArray> cellValues;
			cellValues->SetNumberOfTuples(numberOfCells);
			for (size_t i = 0; i < numberOfCells; ++i)
			{
				cellValues->SetValue(i, i);
			}
			// Create a grid
			vtkNew<vtkStructuredGrid> structuredGrid;
			// Specify the dimensions of the grid
			structuredGrid->SetDimensions(static_cast<int>(I), static_cast<int>(J),
				static_cast<int>(K));

			structuredGrid->SetPoints(structedPoints);
			structuredGrid->GetCellData()->SetScalars(cellValues);

			vtkSmartPointer<vtkDoubleArray> FES_U = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_V = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_W = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_P = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_T = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_RHO = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_MA = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_OMG = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_GAMMA = vtkSmartPointer<vtkDoubleArray>::New();
			FES_U->SetName("U");
			FES_V->SetName("V");
			FES_W->SetName("W");
			FES_P->SetName("P");
			FES_T->SetName("T");
			FES_RHO->SetName("RHO");
			FES_MA->SetName("MA");
			FES_OMG->SetName("OMG");
			FES_GAMMA->SetName("GAMMA");
			// 获取点数量，在循环外统一设置数组大小（避免重复执行）
			int numPoints = structuredGrid->GetNumberOfPoints();

			// 为所有数组设置长度（循环外执行一次即可）
			FES_U->SetNumberOfValues(numPoints);
			FES_V->SetNumberOfValues(numPoints);
			FES_W->SetNumberOfValues(numPoints);
			FES_P->SetNumberOfValues(numPoints);
			FES_T->SetNumberOfValues(numPoints);
			FES_RHO->SetNumberOfValues(numPoints);
			FES_MA->SetNumberOfValues(numPoints);
			FES_OMG->SetNumberOfValues(numPoints);
			FES_GAMMA->SetNumberOfValues(numPoints);

			// 循环为每个点赋值
			for (int i = 0; i < numPoints; i++) {
				FES_MA->SetValue(i, (*Point)[3][i]);    // Ma对应Point[3]（正确索引）
				FES_U->SetValue(i, (*Point)[4][i]);     // U对应Point[4]（正确索引）
				FES_V->SetValue(i, (*Point)[5][i]);     // V对应Point[5]（正确索引）
				FES_W->SetValue(i, (*Point)[6][i]);     // W对应Point[6]（正确索引）
				FES_P->SetValue(i, (*Point)[7][i]);     // p对应Point[7]（正确索引）
				FES_T->SetValue(i, (*Point)[8][i]);     // T对应Point[8]（正确索引）
				FES_RHO->SetValue(i, (*Point)[9][i]);   // Rho对应Point[9]（正确索引）
				FES_OMG->SetValue(i, (*Point)[10][i]);  // OMG对应Point[10]（正确）
				FES_GAMMA->SetValue(i, (*Point)[11][i]);// GAMMA对应Point[11]（正确）
			}


			structuredGrid->GetPointData()->AddArray(FES_P);
			structuredGrid->GetPointData()->AddArray(FES_U);
			structuredGrid->GetPointData()->AddArray(FES_V);
			structuredGrid->GetPointData()->AddArray(FES_W);
			structuredGrid->GetPointData()->AddArray(FES_T);
			structuredGrid->GetPointData()->AddArray(FES_RHO);
			structuredGrid->GetPointData()->AddArray(FES_MA);
			structuredGrid->GetPointData()->AddArray(FES_OMG);
			structuredGrid->GetPointData()->AddArray(FES_GAMMA);
			//-----------------------------------------------
			branch->SetBlock(zoneNum, structuredGrid);
			zoneNum++;
		}
	}
	// 创建一个vtkAppendPolyData来合并所有的PolyData块
	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();

	// 遍历multiBlockDataSet中的每个块
	for (unsigned int i = 0; i < branch->GetNumberOfBlocks(); i++)
	{
		vtkDataSet* dataSet = vtkDataSet::SafeDownCast(branch->GetBlock(i));
		if (dataSet)
		{
			// 如果块是vtkPolyData，直接添加到appendFilter
			if (vtkPolyData::SafeDownCast(dataSet))
			{
				appendFilter->AddInputDataObject(dataSet);
			}
			// 如果块不是vtkPolyData，使用DataSetSurfaceFilter转换它
			else
			{
				vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
				surfaceFilter->SetInputData(dataSet);
				surfaceFilter->Update();
				appendFilter->AddInputData(surfaceFilter->GetOutput());
			}
		}
	}

	// 更新appendFilter以合并所有的PolyData
	appendFilter->Update();

	// 获取合并后的PolyData
	vtkPolyData* mergedPolyData = appendFilter->GetOutput();
	vtk_dataSet = vtkSmartPointer<vtkPolyData>::New();
	vtk_latestData = vtkSmartPointer<vtkPolyData>::New();
	vtk_dataSet = mergedPolyData;
	showVTKData();
	progressDialog->close();
	//emit SignalInstance::getInstance()->signalCloseDialog();
	if (subZone_ == nullptr)
	{
		subZone_ = new SubZoneWindow(zoneNames_.size(), zoneNames_, nullptr);
		subZoneWindowShowCallBack->zoneWindow_ = subZone_;
		m_pQVTKWidget->GetInteractor()->AddObserver(vtkCommand::RightButtonPressEvent, subZoneWindowShowCallBack);
	}
	else {
		subZone_->setZoneNum(zoneNames_.size());
		subZone_->setZoneNames(zoneNames_);
		subZone_->update();
	}
	subZone_->okBtn_->click();


	return mergedPolyData;
}

vtkSmartPointer<vtkUnstructuredGrid> PostProcessProfiling::parseBlockFileVtkFile(QString fileName)
{
	auto *progressDialog = new PostProgressDialog(u8"提示", u8"正在解析网格数据文件....");
	progressDialog->show();
	//emit SignalInstance::getInstance()->signalShowDialog(u8"loading....", u8"正在渲染网格，请等待渲染完成......");

	std::string path;

	path = fileName.toLocal8Bit().toStdString();
	std::cout << "__FUNCTION_:" << __FUNCTION__ << std::endl;
	// 2. 转换中文路径为系统本地编码（关键修复）
	QByteArray localPath = QFile::encodeName(fileName); // 转换为系统兼容编码（如Windows GBK）
	const char* filePath = localPath.constData();

	// 3. 输出路径调试信息（便于定位编码问题）
	qDebug() << u8"原始路径：" << fileName;
	qDebug() << u8"转换后路径：" << localPath;

	// 4. 尝试打开文件（使用转换后的路径）
	std::ifstream data_file(filePath);
	if (!data_file.is_open()) {
		// 详细错误提示：包含系统错误码
		qDebug() << "无法打开文件，错误原因：" << strerror(errno);
		return nullptr;
	}

	// 读取变量名
	/*
	x y z
	 u v w p rho t gamma c mach
	 Lmu k
	*/
	std::string line;
	std::getline(data_file, line); // 读取xyz
	std::getline(data_file, line); // 读取xyz
	std::getline(data_file, line); // 读取xyz
	int index = 0;
	std::vector<std::string> variable_names(14);
	variable_names[0] = "x";
	variable_names[1] = "y";
	variable_names[2] = "z";
	variable_names[3] = "u";
	variable_names[4] = "v";
	variable_names[5] = "w";
	variable_names[6] = "p";
	variable_names[7] = "rho";
	variable_names[8] = "t";
	variable_names[9] = "gamma";
	variable_names[10] = "c";
	variable_names[11] = "mach";
	variable_names[12] = "Lmu";
	variable_names[13] = "k";
	int n = 0;
	int zoneNum = 0;
	vector<int>zonePoints;
	vector<vector<vector<double>>*>Points;
	vector<vtkSmartPointer<vtkStructuredGrid>>structuredGrids;
	vtkNew<vtkMultiBlockDataSet> branch;
	vtkSmartPointer<vtkAppendFilter> appendFilter = vtkSmartPointer<vtkAppendFilter>::New();
	while (std::getline(data_file, line)) {
		n++;
		string lineTmp = "";
		for (int i = 0; i < line.size(); i++)
		{
			if (line[i] != '\0')
			{
				lineTmp += line[i];
			}
		}
		line = lineTmp;
		vector<string> res = Stringsplit(line, ' ');
		if (res.size() == 0)break;
		// zone T="  1",I=   25,J=   71,K=   24,DATAPACKING=BLOCK,VARLOCATION=([4-14]=CELLCENTERED)
		if (res[0] == "zone")
		{
			vtkNew<vtkPoints> structedPoints;
			// 解析I J K
			vector<string> IJK = Stringsplit(line, ',');
			string tmp;
			int I, J, K;
			stringstream ssi(IJK[1]);
			I = atoi(IJK[1].substr(2, IJK[1].size() - 2).c_str());
			J = atoi(IJK[2].substr(2, IJK[2].size() - 2).c_str());
			K = atoi(IJK[3].substr(2, IJK[3].size() - 2).c_str());
			std::cout << "I:" << I << " J:" << J << " K:" << K << std::endl;
			int index = 0;
			int sumData = I * J * K * 3 + ((I - 1) * (J - 1) * (K - 1)) * 11;
			vector<double>DATA(sumData);
			while (1 == 1)
			{
				std::getline(data_file, line);
				line.erase(remove(line.begin(), line.end(), '\0'), line.end());
				stringstream ss(line);
				for (int i = 0; i < 5 && index < sumData; i++)
				{
					double data;
					ss >> data;
					DATA[index++] = data;
				}
				if (index >= sumData)break;
			}
			// 每个属性的起始坐标
			vector<int>variableBeginePos(14);
			int pos = 0;
			for (int i = 0; i < 14; i++)
			{
				variableBeginePos[i] = pos;
				if (i < 3)
				{
					pos += I * J * K;;
				}
				else {
					pos += (I - 1) * (J - 1) * (K - 1);
				}
			}
			index = 0;
			for (int i = 0; i < I * J * K; i++)
			{
				double x = DATA[variableBeginePos[0] + i];
				double y = DATA[variableBeginePos[1] + i];
				double z = DATA[variableBeginePos[2] + i];
				index++;
				structedPoints->InsertNextPoint(x, y, z);
			}
			// Create a grid
			vtkNew<vtkStructuredGrid> structuredGrid;
			// Specify the dimensions of the grid
			structuredGrid->SetDimensions(static_cast<int>(I), static_cast<int>(J),
				static_cast<int>(K));

			structuredGrid->SetPoints(structedPoints);

			vtkSmartPointer<vtkDoubleArray> FES_U = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_V = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_W = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_P = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_RHO = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_T = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_GAMMA = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_C = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_MACH = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_LMU = vtkSmartPointer<vtkDoubleArray>::New();
			vtkSmartPointer<vtkDoubleArray> FES_K = vtkSmartPointer<vtkDoubleArray>::New();
			FES_U->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_V->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_W->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_P->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_RHO->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_T->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_GAMMA->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_C->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_MACH->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_LMU->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));
			FES_K->SetNumberOfValues((I - 1) * (J - 1) * (K - 1));

			FES_U->SetName(variable_names[3].c_str());
			FES_V->SetName(variable_names[4].c_str());
			FES_W->SetName(variable_names[5].c_str());
			FES_P->SetName(variable_names[6].c_str());
			FES_RHO->SetName(variable_names[7].c_str());
			FES_T->SetName(variable_names[8].c_str());
			FES_GAMMA->SetName(variable_names[9].c_str());
			FES_C->SetName(variable_names[10].c_str());
			FES_MACH->SetName(variable_names[11].c_str());
			FES_LMU->SetName(variable_names[12].c_str());
			FES_K->SetName(variable_names[13].c_str());

			index = 0;
			for (int i = 0; i < (I - 1) * (J - 1) * (K - 1); i++)
			{
				double u = DATA[variableBeginePos[3] + index];
				double v = DATA[variableBeginePos[4] + index];
				double w = DATA[variableBeginePos[5] + index];
				double p = DATA[variableBeginePos[6] + index];
				double rho = DATA[variableBeginePos[7] + index];
				double t = DATA[variableBeginePos[8] + index];
				double gamma = DATA[variableBeginePos[9] + index];
				double c = DATA[variableBeginePos[10] + index];
				double mach = DATA[variableBeginePos[11] + index];
				double lmu = DATA[variableBeginePos[12] + index];
				double k = DATA[variableBeginePos[13] + index];
				index++;
				FES_U->SetValue(i, u);
				FES_V->SetValue(i, v);
				FES_W->SetValue(i, w);
				FES_P->SetValue(i, p);
				FES_RHO->SetValue(i, rho);
				FES_T->SetValue(i, t);
				FES_GAMMA->SetValue(i, gamma);
				FES_C->SetValue(i, c);
				FES_MACH->SetValue(i, mach);
				FES_LMU->SetValue(i, lmu);
				FES_K->SetValue(i, k);
			}
			structuredGrid->GetCellData()->SetScalars(FES_U);
			structuredGrid->GetCellData()->SetScalars(FES_V);
			structuredGrid->GetCellData()->SetScalars(FES_W);
			structuredGrid->GetCellData()->SetScalars(FES_P);
			structuredGrid->GetCellData()->SetScalars(FES_RHO);
			structuredGrid->GetCellData()->SetScalars(FES_T);
			structuredGrid->GetCellData()->SetScalars(FES_GAMMA);
			structuredGrid->GetCellData()->SetScalars(FES_C);
			structuredGrid->GetCellData()->SetScalars(FES_MACH);
			structuredGrid->GetCellData()->SetScalars(FES_LMU);
			structuredGrid->GetCellData()->SetScalars(FES_K);

			structuredGrid->GetCellData()->AddArray(FES_U);
			structuredGrid->GetCellData()->AddArray(FES_V);
			structuredGrid->GetCellData()->AddArray(FES_W);
			structuredGrid->GetCellData()->AddArray(FES_P);
			structuredGrid->GetCellData()->AddArray(FES_RHO);
			structuredGrid->GetCellData()->AddArray(FES_T);
			structuredGrid->GetCellData()->AddArray(FES_GAMMA);
			structuredGrid->GetCellData()->AddArray(FES_C);
			structuredGrid->GetCellData()->AddArray(FES_MACH);
			structuredGrid->GetCellData()->AddArray(FES_LMU);
			structuredGrid->GetCellData()->AddArray(FES_K);
			structuredGrid->UpdateCellGhostArrayCache();
			//-----------------------------------------------
			branch->SetBlock(zoneNum, structuredGrid);
			zoneNum++;
			std::cout << "zoneNum:" << zoneNum << std::endl;
			appendFilter->AddInputData(structuredGrid);
			appendFilter->Update();
		}
	}
	appendFilter->Update();
	vtkUnstructuredGrid* mergedGrid = appendFilter->GetOutput();
	progressDialog->close();
	//emit SignalInstance::getInstance()->signalCloseDialog();
	return mergedGrid;
}

vtkSmartPointer<vtkUnstructuredGrid> PostProcessProfiling::calculate_scalar_cell(std::string fileName)
{
	vtkSmartPointer<vtkUnstructuredGridReader>reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	reader->SetReadAllScalars(1);
	reader->SetFileName(fileName.c_str());
	reader->Update();
	auto unstructured_grid = reader->GetOutput();
	auto originalPointData = unstructured_grid->GetPointData();
	auto point_data = unstructured_grid->GetPointData();
	vtkSmartPointer<vtkPointDataToCellData> p2c = vtkSmartPointer<vtkPointDataToCellData>::New();
	p2c->SetInputData(unstructured_grid);
	p2c->Update();
	auto cellData = p2c->GetOutput();
	cellData->GetPointData()->ShallowCopy(originalPointData);

	//保存结果到VTK文件
	vtkSmartPointer<vtkUnstructuredGridWriter> writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
	writer->SetFileName("output.vtk");
	writer->SetInputData(cellData);
	//writer->SetFileVersion(42);
	writer->Write();
	return nullptr;
}


vtkSmartPointer<vtkUnstructuredGrid> PostProcessProfiling::parseVolumeVtkFile(QString fileName)
{
	auto *progressDialog = new PostProgressDialog(u8"提示", u8"正在解析网格数据文件....");
	progressDialog->show();
	//emit SignalInstance::getInstance()->signalShowDialog(u8"loading....", u8"正在渲染网格，请等待渲染完成......");

	/*
	* 数据源进行加载
	*/
	// 读取reader进行配置
	vtkSmartPointer<vtkUnstructuredGridReader>reader = vtkSmartPointer< vtkUnstructuredGridReader>::New();
	std::string path = fileName.toLocal8Bit().toStdString();
	reader->SetFileName(path.c_str());
	reader->ReadAllScalarsOn(); // 确保读取标量值
	reader->ReadAllFieldsOn();
	reader->ReadAllColorScalarsOn();
	reader->Update();
	// 更新slice_OriginGrid
	slice_OriginGrid = vtkSmartPointer<vtkUnstructuredGrid>::New();
	slice_OriginGrid->DeepCopy(reader->GetOutput());

	/*
	* 配置标量属性
	*/
	auto cellData = slice_OriginGrid->GetCellData();
	// 获取各个标量名
	auto cellArrayDatas = cellData->GetNumberOfArrays();
	if (cellArrayDatas == 0)
	{
		calculate_scalar_cell(fileName.toLocal8Bit().toStdString());
		reader->SetFileName("output.vtk");
		reader->ReadAllScalarsOn(); // 确保读取标量值
		reader->ReadAllFieldsOn();
		reader->ReadAllColorScalarsOn();
		reader->Update();
		slice_OriginGrid->DeepCopy(reader->GetOutput());
		cellArrayDatas = cellData->GetNumberOfArrays();
	}
	for (int32_t i = 0; i < cellArrayDatas; i++)
	{
		std::cout << cellData->GetArray(i)->GetName() << std::endl;
	}

	std::string scalar_name = cellData->GetArray(0)->GetName();
	// 获取scalar_name名字的array
	vtkDataArray* d = slice_OriginGrid->GetCellData()->GetArray(scalar_name.c_str());
	// 获取range的最小值-最大值
	std::cout << d->GetRange()[0] << " " << d->GetRange()[1] << std::endl;
	slice_OriginGrid->GetCellData()->SetScalars(d);

	/*
	* 设置传输函数
	*/
	vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
	// 设置颜色转换函数的参数，例如：
	colorTransferFunction->AddRGBPoint(d->GetRange()[0], 0.0, 0.0, 1.0); // 低值对应蓝色
	colorTransferFunction->AddRGBPoint(d->GetRange()[1], 1.0, 0.0, 0.0); // 高值
	progressDialog->close();
	//emit SignalInstance::getInstance()->signalCloseDialog();
	return nullptr;
}


void PostProcessProfiling::slotShowClipPostProcess()
{
	if (!dataIsLoad)
	{
		QMessageBox::information(this, tr("提示"), tr("数据文件未加载请选择加载文件!"));
		return;
	}
	if (isClip_ && !m_clipBtn->isEnabled())
	{
		mykeyPressCallback->showClipExecute();
		return;
	}
	if (isClip_ || isSlice_ || isBoxClip_)
	{
		QMessageBox::information(this, tr("提示"), tr("当前处于操作状态未进行重置网格，请先重置网格!"));
		return;
	}

	resetVtkData();
	initClipUnit();
	removeDataSet();
	m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(m_renderer);
	m_pQVTKWidget->GetRenderWindow()->Render();

	// Setup a visualization pipeline
	clip_plane->SetOrigin(0, 0, 0);
	clip_plane->SetNormal(1, 0, 0);
	clipper->SetClipFunction(clip_plane);
	clipper->InsideOutOn();
	clipper->SetInputData(vtk_dataSet);
	clipper->Update();

	vtk_mapper->SetInputData(vtk_dataSet);
	vtk_mapper->SetInputConnection(clipper->GetOutputPort());
	vtk_mapper->Update();
	vtk_actor->SetMapper(vtk_mapper);
	vtkSmartPointer<vtkProperty> backFaces =
		vtkSmartPointer<vtkProperty>::New();
	backFaces->SetDiffuseColor(.8, .8, .4);
	vtk_actor->SetBackfaceProperty(backFaces);
	m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);
	m_renderer->AddActor(vtk_actor);

	// The callback will do the work
	myCallback =
		vtkSmartPointer<vtkIPWCallback1>::New();
	myCallback->Plane = clip_plane;
	myCallback->Actor = vtk_actor;

	vtkSmartPointer<vtkImplicitPlaneRepresentation> rep =
		vtkSmartPointer<vtkImplicitPlaneRepresentation>::New();
	rep->SetPlaceFactor(1.25); // This must be set prior to placing the widget
	rep->SetNormal(clip_plane->GetNormal());
	double bounds[6];
	vtk_dataSet->GetBounds(bounds);
	double center[3] = {
		(bounds[0] + bounds[1]) / 2.0,
		(bounds[2] + bounds[3]) / 2.0,
		(bounds[4] + bounds[5]) / 2.0
	};
	rep->SetOrigin(center);
	//rep->SetOrigin(bounds[0], bounds[2], bounds[4]);
	rep->PlaceWidget(bounds); // 使用数据集的边界来放置小部件
	rep->OutsideBoundsOff();
	rep->OutlineTranslationOff();

	//---------------
	rep->SetNormal(1.0, 0, 0);

	rep->UpdatePlacement();
	m_pQVTKWidget->GetRenderWindow()->Render();

	// 定义向右移动的向量，这里假设单位是毫米，您可以根据需要调整
	double moveVector[3] = { 10.0, 0.0, 0.0 }; // 向右移动10个单位
	// 将移动向量加到平面的原点上
	double origin[3];
	clip_plane->GetOrigin(origin);
	clip_plane->SetOrigin(origin[0] + moveVector[0], origin[1] + moveVector[1], origin[2] + moveVector[2]);


	clip_planeWidget->SetInteractor(m_pQVTKWidget->GetRenderWindow()->GetInteractor());
	clip_planeWidget->SetRepresentation(rep);
	clip_planeWidget->AddObserver(vtkCommand::InteractionEvent, myCallback);


	//****************************************************
	//-----------------------keycallback
	mykeyPressCallback = vtkSmartPointer<vtkKeyPressCallback>::New();
	mykeyPressCallback->planeWidget_ = clip_planeWidget;
	mykeyPressCallback->plane_ = clip_plane;
	mykeyPressCallback->Clipper_ = clipper;
	mykeyPressCallback->Renderer_ = m_renderer;
	mykeyPressCallback->Mapper_ = vtk_mapper;
	mykeyPressCallback->Actor_ = vtk_actor;
	mykeyPressCallback->Vtk_latestData_ = vtk_latestData;
	mykeyPressCallback->m_pQVTKWidget = m_pQVTKWidget;
	mykeyPressCallback->m_clipBtn = m_clipBtn;
	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::KeyPressEvent, mykeyPressCallback);

	//-----------------------
	// Render
	clip_planeWidget->On();
	m_pQVTKWidget->GetRenderWindow()->Render();
	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Initialize();
	/*m_pQVTKWidget->update();
	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Start();*/

	isClip_ = true;
	m_clipBtn->setEnabled(true);
}

void PostProcessProfiling::slotShowBoxClipPostProcess()
{
	if (!dataIsLoad)
	{
		QMessageBox::information(this, tr("提示"), tr("数据文件未加载请选择加载文件!"));
		return;
	}
	if (isBoxClip_ && !m_boxClipBtn->isEnabled())
	{
		mykeyPressBoxClipCallback->showBoxClipExecute();
		return;
	}
	if (isBoxClip_ && !m_boxClipBtn->isEnabled())
	{
		mykeyPressBoxClipCallback->showBoxClipExecute();
		return;
	}
	if (isClip_ || isSlice_ || isBoxClip_)
	{
		QMessageBox::information(this, tr("提示"), tr("当前处于操作状态未进行重置网格，请先重置网格!"));
		return;
	}
	resetVtkData();
	initClipUnit();
	removeDataSet();
	m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(m_renderer);
	m_pQVTKWidget->GetRenderWindow()->Render();

	vtk_mapper->SetInputData(vtk_dataSet);
	vtk_actor->SetMapper(vtk_mapper);
	m_pQVTKWidget->GetRenderWindow()->AddRenderer(m_renderer);

	// Add the actor to the scene
	m_renderer->AddActor(vtk_actor);

	// Create the box widget
	boxWidget = vtkSmartPointer<vtkBoxWidget>::New();
	boxWidget->SetInteractor(m_pQVTKWidget->GetRenderWindow()->GetInteractor());
	boxWidget->SetPlaceFactor(2);
	box_plane = vtkSmartPointer<vtkPlanes>::New();
	double bound[6];
	vtk_actor->GetBounds(bound);
	boxWidget->PlaceWidget(bound);
	boxWidget->SetPlaceFactor(1.0); // 设置BoxWidget的大小与actor相同
	boxWidget->On(); // 启用BoxWidget

	// Manage the interactions
	boxWidgetCallback = vtkSmartPointer<vtkBoxWidgetCallback>::New();
	boxWidgetCallback->SetClipPolyData(vtk_dataSet);
	boxWidgetCallback->Plane_ = box_plane;
	boxWidgetCallback->Clipper_ = box_clipper;
	boxWidgetCallback->Renderer_ = m_renderer;
	boxWidgetCallback->Mapper_ = vtk_mapper;
	boxWidgetCallback->Actor_ = vtk_actor;
	boxWidgetCallback->Vtk_latestData_ = vtk_latestData;
	boxWidgetCallback->m_pQVTKWidget = m_pQVTKWidget;
	boxWidget->AddObserver(vtkCommand::InteractionEvent, boxWidgetCallback);

	// Create the clipper
	box_clipper = vtkSmartPointer<vtkClipPolyData>::New();
	box_clipper->SetInputData(vtk_dataSet);
	box_clipper->SetClipFunction(box_plane);
	box_clipper->SetValue(0.0);
	box_clipper->InsideOutOn();
	box_clipper->Update();


	mykeyPressBoxClipCallback = vtkSmartPointer<vtkKeyPressBoxClipCallback>::New();
	mykeyPressBoxClipCallback->ClipPolyData = vtk_dataSet;
	mykeyPressBoxClipCallback->Plane_ = box_plane;
	mykeyPressBoxClipCallback->Clipper_ = box_clipper;
	mykeyPressBoxClipCallback->Renderer_ = m_renderer;
	mykeyPressBoxClipCallback->Mapper_ = vtk_mapper;
	mykeyPressBoxClipCallback->Actor_ = vtk_actor;
	mykeyPressBoxClipCallback->Vtk_latestData_ = vtk_latestData;
	mykeyPressBoxClipCallback->m_pQVTKWidget = m_pQVTKWidget;
	mykeyPressBoxClipCallback->boxWidget_ = boxWidget;
	mykeyPressBoxClipCallback->m_boxClipBtn = m_boxClipBtn;

	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->AddObserver(vtkCommand::KeyPressEvent, mykeyPressBoxClipCallback);


	// Create a mapper and actor for the clipped polydata
	vtk_mapper->SetInputConnection(box_clipper->GetOutputPort());
	vtk_mapper->SetClippingPlanes(box_plane);

	m_pQVTKWidget->GetRenderWindow()->Render();
	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Initialize();
	//m_pQVTKWidget->update();
	//m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Start();
	isBoxClip_ = true;
	m_boxClipBtn->setEnabled(true);
}


void PostProcessProfiling::slotShowSlicePostProcess()
{
	if (!dataIsLoad)
	{
		QMessageBox::information(this, tr("提示"), tr("数据文件未加载，请选择数据文件!"));
		return;
	}
	if (isClip_ || isSlice_ || isBoxClip_)
	{
		QMessageBox::information(this, tr("提示"), tr("当前处于处理状态未完成，请先取消当前操作!"));
		return;
	}

	// 检查是否有可用数据（支持vtkUnstructuredGrid和vtkPolyData）
	vtkSmartPointer<vtkDataSet> inputData = nullptr;
	if (slice_OriginGrid)
	{
		inputData = slice_OriginGrid;
	}
	else if (vtk_dataSet)
	{
		// 尝试从vtk_dataSet获取PolyData
		if (vtkPolyData::SafeDownCast(vtk_dataSet))
		{
			inputData = vtk_dataSet;
		}
	}

	if (!inputData)
	{
		launchWarning(u8"当前场景没有可用数据，无法创建切片");
		return;
	}

	// 重置VTK数据
	resetVtkData();
	initSliceUnit();

	// 创建相机并设置给两个渲染器
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();
	sliceOriginRender->SetActiveCamera(camera);
	sliceShowRender->SetActiveCamera(camera);
	sliceOriginRender->ResetCamera();
	sliceShowRender->ResetCamera();
	camera->Dolly(0.3);
	camera->Azimuth(45);    // 向右转45°
	camera->Elevation(-45);  // 向上抬头45°

	// 设置渲染器背景
	sliceOriginRender->SetBackground(1.0, 1.0, 1.0);
	sliceOriginRender->SetBackground2(0.529, 0.8078, 0.92157);
	sliceOriginRender->SetGradientBackground(1);

	sliceShowRender->SetBackground(1.0, 1.0, 1.0);
	sliceShowRender->SetBackground2(0.6078, 0.6078, 0.92157);
	sliceShowRender->SetGradientBackground(1);

	// 初始化交互器
	if (origin_Interactor == nullptr) {
		origin_Interactor = m_pQVTKWidget->GetRenderWindow()->GetInteractor();
		origin_renderWindow = m_pQVTKWidget->GetRenderWindow();

		vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		slice_Interactor->SetInteractorStyle(style);
		slice_Interactor->SetRenderWindow(slice_renderWindow);
		slice_renderWindow->SetInteractor(slice_Interactor);
	}
	m_pQVTKWidget->SetRenderWindow(slice_renderWindow);
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	slice_Interactor->SetInteractorStyle(style);
	m_pQVTKWidget->GetRenderWindow()->SetInteractor(slice_Interactor);
	m_pQVTKWidget->update();
	slice_Interactor->Initialize();

	// 设置渲染器视口
	m_pQVTKWidget->GetRenderWindow()->AddRenderer(sliceOriginRender);
	m_pQVTKWidget->GetRenderWindow()->AddRenderer(sliceShowRender);
	m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(m_renderer);
	m_pQVTKWidget->update();

	// 获取标量数据（同时支持CellData和PointData）
	vtkDataArray* d = nullptr;
	vtkCellData* cellData = inputData->GetCellData();
	if (cellData->GetNumberOfArrays() > 0)
	{
		d = cellData->GetArray(0);
	}
	else
	{
		vtkPointData* pointData = inputData->GetPointData();
		if (pointData->GetNumberOfArrays() > 0)
		{
			d = pointData->GetArray(0);
		}
	}

	if (d)
	{
		// 设置标量数据用于显示
		if (cellData->GetNumberOfArrays() > 0)
		{
			inputData->GetCellData()->SetScalars(d);
		}
		else
		{
			inputData->GetPointData()->SetScalars(d);
		}
	}

	// 创建颜色映射表
	vtkNew<vtkLookupTable> lookupTable;
	if (d)
	{
		lookupTable->SetTableRange(d->GetRange());
	}
	lookupTable->Build();

	// 创建标量条
	vtkNew<vtkScalarBarActor> scalarBarActor;
	scalarBarActor->SetLookupTable(lookupTable);
	scalarBarActor->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
	scalarBarActor->GetPositionCoordinate()->SetValue(0.8, 0.01);
	scalarBarActor->SetWidth(0.1);
	scalarBarActor->SetHeight(0.3);

	// 获取数据边界并设置切片平面
	double bounds[6] = { 0 };
	inputData->GetBounds(bounds);
	slice_plane->SetOrigin(
		(bounds[0] + bounds[1]) * 0.5,
		(bounds[2] + bounds[3]) * 0.5,
		(bounds[4] + bounds[5]) * 0.5
	);
	slice_plane->SetNormal(0, 0, 1);

	// 初始化切割器（支持任意vtkDataSet类型）
	slice_cutter->SetInputData(inputData);
	slice_cutter->SetCutFunction(slice_plane);

	// 设置原始数据映射器
	slice_mapper1->SetInputData(inputData);
	slice_mapper1->SetScalarVisibility(true);
	slice_mapper1->SetLookupTable(lookupTable);
	if (d)
	{
		slice_mapper1->SetScalarRange(d->GetRange());
	}

	// 添加原始数据到左侧视图
	slice_actor1->SetMapper(slice_mapper1);
	sliceOriginRender->AddActor(slice_actor1);
	sliceOriginRender->AddActor(scalarBarActor);
	sliceOriginRender->SetViewport(0, 0, 0.5, 1);

	// 设置切片数据映射器
	vtkSmartPointer<vtkContourTriangulator> triangulator =
		vtkSmartPointer<vtkContourTriangulator>::New();
	triangulator->SetInputConnection(slice_cutter->GetOutputPort());
	triangulator->Update();

	slice_cutter->Update();
	auto output = slice_cutter->GetOutput();
	if ((output->GetNumberOfPolys()) > 0) {
		slice_mapper2->SetInputConnection(slice_cutter->GetOutputPort());
		slice_actor2->SetMapper(slice_mapper2);
	}
	else {
		slice_mapper2->SetInputConnection(triangulator->GetOutputPort());
		slice_actor2->SetMapper(slice_mapper2);
		slice_actor2->GetProperty()->SetColor(0.1, 0.4, 1.0); // 蓝色
		slice_actor2->GetProperty()->SetOpacity(0.5);         // 半透明效果
		slice_actor2->GetProperty()->SetRepresentationToSurface(); // 显示为面
		slice_actor2->GetProperty()->EdgeVisibilityOff(); // 若想显示边框线
		slice_actor2->GetProperty()->SetEdgeColor(0.0, 0.0, 0.5);
	}

	//slice_mapper2->SetInputConnection(slice_cutter->GetOutputPort());
	//slice_actor2->SetMapper(slice_mapper2);
	sliceShowRender->AddActor(slice_actor2);
	sliceShowRender->SetViewport(0.5, 0, 1, 1);
	//// 设置切片数据映射器
	//slice_mapper2->SetInputConnection(slice_cutter->GetOutputPort());
	//slice_actor2->SetMapper(slice_mapper2);
	//sliceShowRender->AddActor(slice_actor2);
	//sliceShowRender->SetViewport(0.5, 0, 1, 1);

	//// 调整交互器手柄大小
	//double sizeX = bounds[1] - bounds[0];
	//double sizeY = bounds[3] - bounds[2];
	//double sizeZ = bounds[5] - bounds[4];
	//double minSize = std::min({ sizeX, sizeY, sizeZ });
	//double handleSize = minSize * 0.001;
	//double centerX = (bounds[0] + bounds[1]) / 2;
	//double centerY = (bounds[2] + bounds[3]) / 2;
	//double centerZ = (bounds[4] + bounds[5]) / 2;
	//// 1. 将 Origin 设置为平面的 "左下角"
	//// (X 最小值, Y 最小值, Z 中心)
	//slice_planeWidget->SetOrigin(bounds[0] - 1, bounds[2], centerZ);

	//// 2. 将 Point1 设置为平面的 "右下角"
	//// (X 最大值, Y 最小值, Z 中心)
	//slice_planeWidget->SetPoint1(bounds[1] + 1, bounds[2], centerZ);

	//// 3. 将 Point2 设置为平面的 "左上角"
	//// (X 最小值, Y 最大值, Z 中心)
	//slice_planeWidget->SetPoint2(bounds[0] - 1, bounds[3] + 1, centerZ);
	////// 设置平面中心（仍然在体数据中间）
	////slice_planeWidget->SetOrigin(centerX, centerY, centerZ);

	////// 沿 X 轴方向的点（覆盖整个 X 范围）
	////slice_planeWidget->SetPoint1(bounds[0], centerY, centerZ);

	////// 沿 Y 轴方向的点（覆盖整个 Y 范围）
	////slice_planeWidget->SetPoint2(centerX, bounds[2], centerZ);

	//// 设置法向量（平面平行于Z轴）
	//slice_planeWidget->SetNormal(0, 0, 1);

		// 调整交互器手柄大小
	double sizeX = bounds[1] - bounds[0];
	double sizeY = bounds[3] - bounds[2];
	double sizeZ = bounds[5] - bounds[4];
	double minSize = std::min({ sizeX, sizeY, sizeZ });
	double maxSize = std::max({ sizeX,sizeY,sizeZ });
	double handleSize = minSize * 0.001;
	double centerX = (bounds[0] + bounds[1]) / 2;
	double centerY = (bounds[2] + bounds[3]) / 2;
	double centerZ = (bounds[4] + bounds[5]) / 2;
	double size_adjust = 0.25;
	// 设置平面中心（仍然在体数据中间）
	//slice_planeWidget->SetOrigin(centerX, centerY, centerZ);
	//slice_planeWidget->SetCenter(0, 0, 0);

	//// 沿 X 轴方向的点（覆盖整个 X 范围）
	//slice_planeWidget->SetPoint1(bounds[0]+1, centerY, centerZ);

	//// 沿 Y 轴方向的点（覆盖整个 Y 范围）
	//slice_planeWidget->SetPoint2(centerX, bounds[2]+1, centerZ);

	//// 设置法向量（平面平行于Z轴）
	//slice_planeWidget->SetNormal(0, 0, 1);

	//slice_planeWidget->SetCenter(centerX, centerY, centerZ);


	// 1. 将 Origin 设置为平面的 "左下角"
	// (X 最小值, Y 最小值, Z 中心)
	slice_planeWidget->SetOrigin(bounds[0] - size_adjust, bounds[2] - size_adjust, centerZ);

	// 2. 将 Point1 设置为平面的 "右下角"
	// (X 最大值, Y 最小值, Z 中心)
	slice_planeWidget->SetPoint1(bounds[1] + size_adjust, bounds[2] - size_adjust, centerZ);

	// 3. 将 Point2 设置为平面的 "左上角"
	// (X 最小值, Y 最大值, Z 中心)
	slice_planeWidget->SetPoint2(bounds[0] - size_adjust, bounds[3] + size_adjust, centerZ);

	double diagonal = sqrt(sizeX*sizeX + sizeY * sizeY + sizeZ * sizeZ);

	slice_planeWidget->SetNormal(0, 0, 1);
	qDebug() << "SizeX=" << sizeX << " sizeY" << sizeY << "sizeZ" << sizeZ << "digonal" << diagonal;
	if (diagonal < 0.1) {
		camera->Dolly(1.5);
	}
	else {
		camera->Dolly(0.5);
	}


	vtkProperty *handler = slice_planeWidget->GetHandleProperty();
	qDebug() << "min_size=" << handler->GetPointSizeMinValue() << " max_size=" << handler->GetPointSizeMaxValue()
		<<" current_size="<<handler->GetPointSize();
	handler->SetPointSize(10);
	
	qDebug() << "min_size=" << handler->GetPointSizeMinValue() << " max_size=" << handler->GetPointSizeMaxValue()
		<< " current_size=" << handler->GetPointSize();
	//slice_planeWidget->GetHandleProperty()->SetPointSize(handleSize);
	//slice_planeWidget->SetHandleSize(handleSize);

	// 设置平面交互器
	slice_planeWidget->SetInteractor(m_pQVTKWidget->GetRenderWindow()->GetInteractor());
	slice_planeWidget->SetDefaultRenderer(sliceOriginRender);
	//slice_planeWidget->SetOrigin(slice_plane->GetOrigin());
	slice_planeWidget->EnabledOn();

	// 设置交互回调
	mySliceCallback->m_plane = slice_plane;
	slice_planeWidget->AddObserver(vtkCommand::InteractionEvent, mySliceCallback);



	// 渲染更新
	auto renderCollections = m_pQVTKWidget->GetRenderWindow()->GetRenderers();
	int32_t renderNum = renderCollections->GetNumberOfItems();
	qDebug() << "show slice m_pQVTKWidget->GetRenderWindow()->GetRenderers():" << renderNum;
	slice_planeWidget->Modified();
	m_pQVTKWidget->GetRenderWindow()->Render();
	// （省略后续渲染逻辑）
	m_pQVTKWidget->update();

	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Initialize();
	qDebug() << "min_size=" << handler->GetPointSizeMinValue() << " max_size=" << handler->GetPointSizeMaxValue()
		<< " current_size=" << handler->GetPointSize();
	isSlice_ = true;
	qDebug() << "min_size=" << handler->GetPointSizeMinValue() << " max_size=" << handler->GetPointSizeMaxValue()
		<< " current_size=" << handler->GetPointSize();
	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->Start();
	

}

void PostProcessProfiling::slotBoxCliping()
{

}

void PostProcessProfiling::slotClipPostProcess()
{
	if (!isClip_)return;
	mykeyPressCallback->ClipExecute();
}

void PostProcessProfiling::slotBoxClipPostProcess()
{
	if (!isBoxClip_)return;
	mykeyPressBoxClipCallback->BoxClipExecute();
}

void PostProcessProfiling::slotZonesChanged(QVector<int32_t> zones)
{
	resetVtkData();
	// 创建一个vtkAppendPolyData来合并所有的PolyData块
	vtkSmartPointer<vtkAppendPolyData> appendFilter = vtkSmartPointer<vtkAppendPolyData>::New();

	// 遍历multiBlockDataSet中的每个块
	for (unsigned int i = 0; i < zones.size(); i++)
	{
		vtkDataSet* dataSet = vtkDataSet::SafeDownCast(branch->GetBlock(zones[i]));
		if (dataSet)
		{
			// 如果块是vtkPolyData，直接添加到appendFilter
			if (vtkPolyData::SafeDownCast(dataSet))
			{
				//appendFilter->AddInputData(dataSet);
				appendFilter->AddInputDataObject(dataSet);
			}
			// 如果块不是vtkPolyData，使用DataSetSurfaceFilter转换它
			else
			{
				vtkSmartPointer<vtkDataSetSurfaceFilter> surfaceFilter = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
				surfaceFilter->SetInputData(dataSet);
				surfaceFilter->Update();
				appendFilter->AddInputData(surfaceFilter->GetOutput());
			}
		}
	}

	// 更新appendFilter以合并所有的PolyData
	appendFilter->Update();

	// 获取合并后的PolyData
	vtkPolyData* mergedPolyData = appendFilter->GetOutput();
	vtk_dataSet = vtkSmartPointer<vtkPolyData>::New();
	vtk_latestData = vtkSmartPointer<vtkPolyData>::New();
	vtk_dataSet = mergedPolyData;
	showVTKData();
	// 新增：重新应用当前选择的属性
	if (!m_curShowAttrName.empty() && !m_curShowAttrType.empty()) {
		SetActivateAttributeToMapperForVtk(vtk_mapper);
	}
}

void PostProcessProfiling::initPostProcessFunctionToolBar()
{
	m_showClipBtn = new QPushButton(u8"切割窗口");
	m_clipBtn = new QPushButton(u8"切割");
	m_clipBtn->setEnabled(false);
	connect(m_showClipBtn, SIGNAL(clicked()), this, SLOT(slotShowClipPostProcess()));
	connect(m_clipBtn, SIGNAL(clicked()), this, SLOT(slotClipPostProcess()));

	m_showBoxClipBtn = new QPushButton(u8"盒型切割窗口");
	m_boxClipBtn = new QPushButton(u8"盒型切割");
	m_boxClipBtn->setEnabled(false);
	connect(m_showBoxClipBtn, SIGNAL(clicked()), this, SLOT(slotShowBoxClipPostProcess()));
	connect(m_boxClipBtn, SIGNAL(clicked()), this, SLOT(slotBoxClipPostProcess()));

	m_showSliceBtn = new QPushButton(u8"切片窗口");
	m_sliceBtn = new QPushButton(u8"切片");
	m_sliceBtn->setEnabled(false);
	connect(m_showSliceBtn, SIGNAL(clicked()), this, SLOT(slotShowSlicePostProcess()));
}

void PostProcessProfiling::initPostProcessStates()
{
	isClip_ = false;

}

vtkSmartPointer<vtkImageData> PostProcessProfiling::polyDataToImageData(vtkSmartPointer<vtkPolyData> polydata)
{
	vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
	double bounds[6];
	polydata->GetBounds(bounds);
	double spacing[3]; // desired volume spacing
	spacing[0] = 0.01;
	spacing[1] = 0.01;
	spacing[2] = 0.01;
	imageData->SetSpacing(spacing);
	// 计算维度
	int dim[3];
	for (int i = 0; i < 3; i++)
	{
		dim[i] = static_cast<int>(ceil((bounds[i * 2 + 1] - bounds[i * 2]) / spacing[i]) + 2);//加一圈空气
	}
	imageData->SetDimensions(dim);
	//imageData->SetExtent(1, dim[0] - 2, 1, dim[1] - 2, 1, dim[2] - 2);

	double origin[3];
	origin[0] = bounds[0] + 2 * spacing[0];
	origin[1] = bounds[2] + 2 * spacing[1];
	origin[2] = bounds[4] + 2 * spacing[2];
	imageData->SetOrigin(origin);

#if VTK_MAJOR_VERSION <= 5
	whiteImage->SetScalarTypeToUnsignedChar();
	whiteImage->AllocateScalars();
#else
	imageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
#endif
	//全部填充
	unsigned char inval = 255;
	unsigned char outval = 0;
	vtkIdType count = imageData->GetNumberOfPoints();
	for (vtkIdType i = 0; i < count; ++i)
	{
		imageData->GetPointData()->GetScalars()->SetTuple1(i, inval);
	}
	//外围填充
	for (int z = 0; z < dim[2]; ++z) {
		for (int y = 0; y < dim[1]; ++y) {
			for (int x = 0; x < dim[0]; ++x) {
				// Check if the current voxel is outside the extent
				if (x < 1 || x >= dim[0] - 1 || y < 1 || y >= dim[1] - 1 || z < 1 || z >= dim[2] - 1) {
					vtkIdType index = x + dim[0] * (y + dim[1] * z);
					imageData->GetPointData()->GetScalars()->SetTuple1(index, outval);
				}
			}
		}
	}

	vtkSmartPointer<vtkPolyDataToImageStencil> pdtoImageStencil = vtkSmartPointer<vtkPolyDataToImageStencil>::New();
	pdtoImageStencil->SetInputData(polydata);
	pdtoImageStencil->SetOutputOrigin(origin);
	pdtoImageStencil->SetOutputSpacing(spacing);
	pdtoImageStencil->SetOutputWholeExtent(imageData->GetExtent());
	pdtoImageStencil->Update();

	vtkSmartPointer<vtkImageStencil> imageStencil = vtkSmartPointer<vtkImageStencil>::New();
	imageStencil->SetInputData(imageData);
	imageStencil->SetStencilConnection(pdtoImageStencil->GetOutputPort());
	imageStencil->ReverseStencilOff();
	imageStencil->SetBackgroundValue(outval);
	imageStencil->Update();

	imageData->DeepCopy(imageStencil->GetOutput());
	return imageData;
}

void PostProcessProfiling::updateDataSetFromUnstructedGrid(vtkSmartPointer<vtkUnstructuredGrid> unsGrid)
{
	Create(vtkDataSetSurfaceFilter, surfaceFilter);
	surfaceFilter->SetInputData(unsGrid);
	surfaceFilter->Update();
	vtk_latestData = vtkSmartPointer<vtkPolyData>::New();
	vtk_dataSet = vtkSmartPointer<vtkPolyData>::New();
	vtk_dataSet = surfaceFilter->GetOutput();
}

void PostProcessProfiling::launchWarning(const QString & message)
{
	QPointer<QMessageBox> confirmBox = new QMessageBox();
	confirmBox->setWindowTitle(tr("WARNING"));
	confirmBox->setWindowIcon(QIcon(":/img/BUPT-SCIV.png"));
	confirmBox->setIcon(QMessageBox::Information);
	confirmBox->setText(message);
	confirmBox->setStandardButtons(QMessageBox::Ok);
	confirmBox->exec();
}

void PostProcessProfiling::resetVtkData()
{
	if (isSlice_)
	{
		vtkSmartPointer<vtkInteractorStyleTrackballCamera> style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
		origin_Interactor->SetInteractorStyle(style);
		m_pQVTKWidget->SetRenderWindow(origin_renderWindow);
		m_pQVTKWidget->GetRenderWindow()->SetInteractor(origin_Interactor);
		m_pQVTKWidget->update();
		initArrowActor();
		origin_Interactor->Initialize();
	}
	isClip_ = false;
	isBoxClip_ = false;
	isSlice_ = false;
	m_clipBtn->setEnabled(false);
	m_boxClipBtn->setEnabled(false);
	if (slice_OriginGrid && !vtk_dataSet)
	{
		updateDataSetFromUnstructedGrid(slice_OriginGrid);
	}
	if (vtk_latestData) {
		vtk_latestData->DeepCopy(vtk_dataSet);
	}

	m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(m_renderer);
	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->RemoveObserver(mykeyPressCallback);
	m_pQVTKWidget->GetRenderWindow()->GetInteractor()->RemoveObserver(mykeyPressBoxClipCallback);
	m_pQVTKWidget->update();
	if (sliceShowRender) {
		m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(sliceShowRender);
		m_pQVTKWidget->update();
		vtkActorCollection * allActor = sliceShowRender->GetActors();
		vtkCollectionSimpleIterator pit;
		vtkActor *currActor = nullptr;
		for (allActor->InitTraversal(pit); (currActor = allActor->GetNextActor(pit)); )
		{
			sliceShowRender->RemoveActor(currActor);
		}
	}
	if (sliceOriginRender) {
		m_pQVTKWidget->GetRenderWindow()->RemoveRenderer(sliceOriginRender);
		m_pQVTKWidget->update();
		vtkActorCollection * allActor = sliceOriginRender->GetActors();
		int32_t actionNum = allActor->GetNumberOfItems();
		vtkCollectionSimpleIterator pit;
		vtkActor *currActor = nullptr;
		for (allActor->InitTraversal(pit); (currActor = allActor->GetNextActor(pit)); )
		{
			sliceOriginRender->RemoveActor(currActor);
		}
	}
	removeDataSet();

}
