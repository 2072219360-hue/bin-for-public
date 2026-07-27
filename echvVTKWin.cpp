#include "echvVTKWin.h"

namespace platforms {
	namespace echv {
		echvVTKWin::echvVTKWin(QWidget *parent)
			: QWidget(parent)
		{
			
			initVariables();
			setMinimumSize(300, 300);
			setMaximumSize(2000, 2000);
			QVBoxLayout *layout = new QVBoxLayout(this);
			//vtkWin = new QVTKOpenGLNativeWidget(this);
			vtkWin = new QVTKWidget(this);
			//setCentralWidget(m_pQVTKWidget);
			//vtkWin = new QVTKOpenGLWidget(this);

			m_attributeToolBar = new QToolBar(u8"属性工具栏", this);
			
			initDisplayModeToolBar();
			m_attributeToolBar->addSeparator();
			


			layout->addWidget(m_attributeToolBar);
			layout->addWidget(vtkWin);

			initVTK();
			connectAll();
		}

		echvVTKWin::~echvVTKWin()
		{
			delete vtkWin;
		}
		void echvVTKWin::initVariables() {
			QVTKWidget *vtkWin=nullptr;
			m_renderer=nullptr;
			m_grid = nullptr;

			m_scalarBarActor =nullptr;

			m_displayModeMapper=nullptr;
			m_displayModeToolBtn = nullptr;
			m_attributeToolBar=nullptr;

			m_curShowAttrType = "POINT";

		}
		void echvVTKWin::connectAll()
		{
			//connect(SignalInstance::getInstance(),&SignalInstance::signalReloadVTK,this,&VTKWindow::reloadVTK);
			/*connect(ECHVINSTANCE, &ECHVSIGNAL::signalReloadVTK, this, [=](argData *arg) {

				auto* thr = QThread::create([=](argData *argments) {
					reloadVTK(argments);
				}, arg);
				connect(thr, &QThread::started, this, []() {emit ECHVINSTANCE->signalPrint(u8"开始重新加载VTK模型"); });
				connect(thr, &QThread::finished, this, []() { emit ECHVINSTANCE->signalPrint(u8"加载完成"); });
				thr->start();

			});*/
			connect(ECHVINSTANCE, &ECHVSIGNAL::signalLoadVTK, this, [=](argData *arg,runData *data) {
				auto* thr = QThread::create([=](argData *arg1, runData *data1) {
					loadVTK(arg1,data1);
				}, arg,data);
				//connect(thr, &QThread::started, this, []() {emit ECHVINSTANCE->signalPrint(u8"开始重新加载VTK模型"); });
				connect(thr, &QThread::finished, this, []() { emit ECHVINSTANCE->signalPrint(u8"加载完成"); });
				thr->start();

			});
		}
		void echvVTKWin::initDisplayModeToolBar() {
			//动态加载“显示模式”工具栏
			if (m_displayModeMapper) {
				disconnect(m_displayModeMapper, SIGNAL(mapped(QString)), this, SLOT(displayModeSlot(QString)));
			}
			m_displayModeMapper = new QSignalMapper(this);
			const static vector<string> vecDisplayMode = {
				u8"轮廓模式",//"Outline",
				u8"散点模式",//"Points",
				u8"表面模式",//"Surface",
				u8"网格模式",//"Wireframe",
				u8"网面模式"//"Surface With Edges"
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

			m_displayModeToolBtn->setIcon(QIcon::fromTheme(u8"显示模式",
				QIcon(("images/" + vecDisplayMode[0] + ".png").c_str())));
			m_displayModeToolBtn->setText(QString(u8"显示模式"));

			// 为QToolButton设置菜单
			m_displayModeToolBtn->setMenu(vecAction.empty() ? nullptr : menu);
			// m_curShowMeshMode = "表面模式";

		}
		void echvVTKWin::displayModeSlot(QString mode) {
			//qDebug() << " disPlay:"<<mode;
			m_curShowMeshMode = mode.toUtf8().toStdString();
			QIcon icon = QIcon::fromTheme(tr(mode.toUtf8().toStdString().c_str()),
				QIcon(("images/" + mode.toUtf8().toStdString() + ".png").c_str()));
			m_displayModeToolBtn->setIcon(icon);
			if (mode.size() > 15) {
				mode.resize(15);
				mode += "...";
			}
			m_displayModeToolBtn->setText(mode);
			initUI();
		}
		void echvVTKWin::addScalarBarActorForVtk(vtkSmartPointer<vtkDataSetMapper> mapper, string title, double * range) {
			
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
		void echvVTKWin::SetActivateAttributeToMapperForVtk(vtkSmartPointer<vtkDataSetMapper> mapper) {
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
		void echvVTKWin::initUI() {
			qDebug() << "m_curShowMeshMode=" << m_curShowMeshMode.c_str()<<" flag="<< (m_curShowMeshMode == u8"散点模式");
			if (m_curShowMeshMode == u8"轮廓模式") {
				removeActors();
				vtkSmartPointer<vtkActor> actor = addSurfaceActor(m_grid);
				addActorToShow(actor);

			}
			else if (m_curShowMeshMode == u8"散点模式") {

				removeActors();
				vtkSmartPointer<vtkActor> actor=addPointsActor(m_grid);
				addActorToShow(actor);

			}
			else if (m_curShowMeshMode == u8"表面模式" || m_curShowMeshMode == "") {
				removeActors();
				vtkSmartPointer<vtkActor> actor = addSurfaceActor(m_grid);
				addActorToShow(actor);

			}
			else if (m_curShowMeshMode == u8"网格模式") {
				removeActors();
				vtkSmartPointer<vtkActor> actor = addWireFrameActor(m_grid);
				addActorToShow(actor);
			}
			else if (m_curShowMeshMode == u8"网面模式") {
				removeActors();
				vtkSmartPointer<vtkActor> actor = addWireFrameActor(m_grid);
				addActorToShow(actor);
			}
		}
		void echvVTKWin::initToolBars() {
			//m_attributeToolBar = new QToolBar(tr("属性工具栏"),this);
			
			//m_attributeToolBar = addToolBar(tr("属性工具栏"));
		}

		void echvVTKWin::initVTK() {
			vtkNew<vtkNamedColors> colors;

			// Set the background color.
			std::array<unsigned char, 4> bkg{ {26, 51, 102, 255} };
			colors->SetColor("BkgColor", bkg.data());

			if (!m_grid) m_grid = vtkSmartPointer<vtkStructuredGrid>::New();
			//vtkSmartPointer<vtkStructuredGrid> structuredGrid = m_grid;
			// 定义网格的维度
			int gridDimensions[3] = { 50, 50, 50 }; // 3x3x2 的网格
			m_grid->SetDimensions(gridDimensions);

			// 创建点
			vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
			for (int z = 0; z < gridDimensions[2]; ++z)
			{
				for (int y = 0; y < gridDimensions[1]; ++y)
				{
					for (int x = 0; x < gridDimensions[0]; ++x)
					{
						points->InsertNextPoint(x, y, z);
					}
				}
			}
			m_grid->SetPoints(points);

			vtkNew<vtkDataSetMapper> mapper;
			mapper->SetInputData(m_grid);
			/*vtkNew<vtkCylinderSource> cylinder;
			cylinder->SetResolution(8);

			vtkNew<vtkPolyDataMapper> cylinderMapper;
			cylinderMapper->SetInputConnection(cylinder->GetOutputPort());*/

			vtkNew<vtkActor> actor;
			actor->SetMapper(mapper);
			actor->GetProperty()->SetColor(
				colors->GetColor4d("Tomato").GetData());
			actor->RotateX(30.0);
			actor->RotateY(-45.0);
			
			vtkSmartPointer<vtkRenderer> m_renderer = vtkSmartPointer<vtkRenderer>::New();
			vtkSmartPointer<vtkRenderer> renderer = m_renderer;
			//vtkNew<vtkRenderer> renderer;
			renderer->AddActor(actor);
			renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
			renderer->ResetCamera();
			renderer->GetActiveCamera()->Zoom(1.5);

			/*if (m_renderer == nullptr) m_renderer = vtkSmartPointer<vtkRenderer>::New();
			m_renderer->AddActor(actor);
			m_renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
			m_renderer->ResetCamera();
			m_renderer->GetActiveCamera()->Zoom(1.5);*/

			vtkRenderWindow *renderWindow = vtkWin->GetRenderWindow();
			renderWindow->SetSize(300, 300);
			renderWindow->AddRenderer(renderer);
			renderWindow->SetWindowName("vtkWindow");


			renderWindow->Render();
		}
		void echvVTKWin::removeActors() {
			vtkRenderWindow *renderWindow = vtkWin->GetRenderWindow();
			vtkRendererCollection *allRenderer = renderWindow->GetRenderers();
			vtkCollectionSimpleIterator pit;
			allRenderer->InitTraversal(pit);
			vtkRenderer *renderer = allRenderer->GetNextRenderer(pit);
			vtkActorCollection* allActor = renderer->GetActors();
			vtkActor *currActor = nullptr;
			for (allActor->InitTraversal(pit); (currActor = allActor->GetNextActor(pit)); )
			{
				renderer->RemoveActor(currActor);
			}
			renderWindow->Render();
		}
		void echvVTKWin::removeRenders() {
			vtkRenderWindow *renderWindow = vtkWin->GetRenderWindow();
			vtkRendererCollection *allRenderer = renderWindow->GetRenderers();
			vtkCollectionSimpleIterator pit;
			vtkRenderer *currRender = nullptr;
			for (allRenderer->InitTraversal(pit); (currRender = allRenderer->GetNextRenderer(pit)); )
			{
				renderWindow->RemoveRenderer(currRender);
			}
			renderWindow->Render();
		}
		void echvVTKWin::reloadVTK(argData *arg) {
			////removeActors();
			////removeRenders();
			//vtkNew<vtkNamedColors> colors;

			//// Set the background color.
			//std::array<unsigned char, 4> bkg{ {26, 51, 102, 255} };
			//colors->SetColor("BkgColor", bkg.data());

			////vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid;
			//vtkUnstructuredGrid* unstructuredGrid;
			//vtkNew<vtkUnstructuredGridReader> reader;
			//reader->SetFileName(arg->getFilePath().c_str());
			//reader->Update();
			//unstructuredGrid = reader->GetOutput();

			//vtkNew<vtkDataSetMapper> mapper;
			//mapper->SetInputData(unstructuredGrid);

			//vtkNew<vtkActor> actor;
			//actor->SetMapper(mapper);
			//actor->RotateX(30.0);
			//actor->RotateY(-45.0);

			//vtkRenderWindow *renderWindow = vtkWin->GetRenderWindow();
			//vtkRendererCollection *allRenderer = renderWindow->GetRenderers();
			//vtkCollectionSimpleIterator pit;
			//allRenderer->InitTraversal(pit);
			//vtkRenderer *renderer = allRenderer->GetNextRenderer(pit);

			//removeActors();
			////vtkNew<vtkRenderer> renderer;
			//renderer->AddActor(actor);
			////renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
			//renderer->ResetCamera();
			//renderer->GetActiveCamera()->Zoom(1.5);


			////好奇怪,必须要让window先setSize一下，再恢复原样才行
			////vtkRenderWindow *renderWindow = vtkWin->GetRenderWindow();
			//int x = renderWindow->GetSize()[0], y = renderWindow->GetSize()[1];
			//renderWindow->SetSize(x - 1, y - 1);
			//renderWindow->SetSize(x, y);
			////renderWindow->AddRenderer(renderer);
			//renderWindow->SetWindowName("vtkWindow");

			//renderWindow->Render();

		}

		void echvVTKWin::loadVTK(argData *arg, runData *data) {
			vtkNew<vtkNamedColors> colors;

			// Set the background color.
			std::array<unsigned char, 4> bkg{ {26, 51, 102, 255} };
			colors->SetColor("BkgColor", bkg.data());

			//vtkSmartPointer<vtkUnstructuredGrid> unstructuredGrid;
			//vtkUnstructuredGrid* unstructuredGrid=data->getUnGrid();
			//vtkStructuredGrid* structuredGrid = data->getGrid();
			m_grid = data->getGrid();

			vtkNew<vtkDataSetMapper> mapper;
			mapper->SetInputData(m_grid);

			vtkNew<vtkActor> actor;
			actor->SetMapper(mapper);
			actor->RotateX(30.0);
			actor->RotateY(-45.0);

			vtkRenderWindow *renderWindow = vtkWin->GetRenderWindow();
			vtkRendererCollection *allRenderer = renderWindow->GetRenderers();
			vtkCollectionSimpleIterator pit;
			allRenderer->InitTraversal(pit);

			vtkRenderer *renderer = allRenderer->GetNextRenderer(pit);
			

			removeActors();
			//vtkNew<vtkRenderer> renderer;
			renderer->AddActor(actor);
			//renderer->SetBackground(colors->GetColor3d("BkgColor").GetData());
			renderer->ResetCamera();
			renderer->GetActiveCamera()->Zoom(1.5);


			//好奇怪,必须要让window先setSize一下，再恢复原样才行
			//vtkRenderWindow *renderWindow = vtkWin->GetRenderWindow();
			int x = renderWindow->GetSize()[0], y = renderWindow->GetSize()[1];
			renderWindow->SetSize(x - 1, y - 1);
			renderWindow->SetSize(x, y);
			//renderWindow->AddRenderer(renderer);
			renderWindow->SetWindowName("vtkWindow");

			renderWindow->Render();

		}
		void echvVTKWin::addActorToShow(vtkSmartPointer<vtkActor> actor) {
			vtkRenderWindow *renderWindow = vtkWin->GetRenderWindow();
			vtkRendererCollection *allRenderer = renderWindow->GetRenderers();
			vtkCollectionSimpleIterator pit;
			allRenderer->InitTraversal(pit);
			vtkRenderer *renderer = allRenderer->GetNextRenderer(pit);

			renderer->AddActor(actor);

			renderer->ResetCamera();
			renderer->GetActiveCamera()->Zoom(1.5);

			int x = renderWindow->GetSize()[0], y = renderWindow->GetSize()[1];
			renderWindow->SetSize(x - 1, y - 1);
			renderWindow->SetSize(x, y);

			renderWindow->SetWindowName("vtkWindow");
			renderWindow->Render();
		}

		vtkSmartPointer<vtkActor> echvVTKWin::addOutlineActor(vtkSmartPointer<vtkOutlineFilter> outlineData) {
			return nullptr;
		}
		vtkSmartPointer<vtkActor> echvVTKWin::addPointsActor(vtkSmartPointer<vtkDataSet> dataSet) {
			vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
			mapper->SetInputData(dataSet);
			mapper->SetScalarVisibility(false);
			//SetActivateAttributeToMapperForVtk(mapper);

			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			actor->GetProperty()->SetAmbient(1.0);
			actor->GetProperty()->SetDiffuse(0.0);
			actor->GetProperty()->SetSpecular(0.0);
			actor->SetMapper(mapper);
			actor->GetProperty()->SetRepresentationToPoints();
			//actor->GetProperty()->SetColor(1.0, 1.0, 1.0);
			actor->GetProperty()->SetPointSize(2.0);
			actor->PickableOff();

			//addActorToShow(actor);
			return actor;
		}
		vtkSmartPointer<vtkActor> echvVTKWin::addSurfaceActor(vtkSmartPointer<vtkDataSet> dataSet) {
			vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
			mapper->SetInputData(dataSet);

			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			actor->SetMapper(mapper);
			
			return actor;
		}
		vtkSmartPointer<vtkActor> echvVTKWin::addWireFrameActor(vtkSmartPointer<vtkDataSet> dataSet) {
			vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
			mapper->SetInputData(dataSet);
			mapper->SetScalarVisibility(false);
			

			vtkSmartPointer<vtkActor> actor = vtkSmartPointer<vtkActor>::New();
			actor->GetProperty()->SetAmbient(1.0);
			actor->GetProperty()->SetDiffuse(0.0);
			actor->GetProperty()->SetSpecular(0.0);
			actor->SetMapper(mapper);
			actor->GetProperty()->SetRepresentationToWireframe();
			
			actor->GetProperty()->SetLineWidth(1.2);
			actor->PickableOff();

			

			return actor;
		}
	}
}

