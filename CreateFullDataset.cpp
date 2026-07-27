#include "CreateFullDataset.h"



namespace platforms {
	namespace echv {
		void CreateFullDataset::AlgorithmInterface() {
			vectorName = "vectors";
			
			//vtkSMPTools::SetBackend("STDThread");
			
			vtkSMPTools::Initialize(2);// 设置最大使用线程数为 2
			int num_threads = vtkSMPTools::GetEstimatedNumberOfThreads();
			qDebug()<< "Total threads: " << num_threads << endl;
			vtkSmartPointer<vtkStructuredGrid> dataset = data->getGrid();
			if (dataset == NULL) {
				qDebug() << "vtkStructuredGrid is null." << endl;
				return;
			}
			/*for (int i = 0; i < dataset->GetPointData()->GetNumberOfArrays(); i++) {
				string str = dataset->GetPointData()->GetArrayName(i);
				qDebug() << " i="<<i<<" name=" << str.c_str()<<" dim="<<dataset->GetPointData()->GetArray(i)->GetNumberOfComponents();
			}*/
			dataset->Squeeze();
			
			vtkDataArray* velocity = dataset->GetPointData()->GetArray(vectorName.c_str());
			if (velocity == nullptr)
			{
				qDebug() << "Velocity array not found." << endl;
				return;
			}
			// 1. Compute the jacobian from the velocity field
			vtkSmartPointer<vtkDataArray> jacobian;
			{
				vtkNew<vtkGradientFilter> gradient;
				gradient->SetInputData(dataset);
				gradient->SetResultArrayName("jacobian-vtk");
				gradient->ComputeDivergenceOn();
				gradient->SetDivergenceArrayName("divergence-vtk");
				gradient->ComputeVorticityOn();
				gradient->SetVorticityArrayName("vorticity-vtk");
				gradient->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, velocity->GetName());
				gradient->Update();

				jacobian = vtkDataArray::SafeDownCast(gradient->GetOutput()->GetPointData()->GetAbstractArray("jacobian-vtk"));
				vtkSmartPointer<vtkDataArray> div = vtkDataArray::SafeDownCast(gradient->GetOutput()->GetPointData()->GetAbstractArray("divergence-vtk"));
				vtkSmartPointer<vtkDataArray> vor = vtkDataArray::SafeDownCast(gradient->GetOutput()->GetPointData()->GetAbstractArray("vorticity-vtk"));
				dataset->GetPointData()->AddArray(jacobian);
				dataset->GetPointData()->AddArray(div);
				dataset->GetPointData()->AddArray(vor);
			}
			//// Compute the acceleration field: a = J * v
			vtkSmartPointer<vtkFloatArray> acceleration;
			{
				acceleration = vtkSmartPointer<vtkFloatArray>::New();
				acceleration->SetName("acceleration-vtk");
				acceleration->SetNumberOfComponents(3);
				acceleration->SetNumberOfTuples(jacobian->GetNumberOfTuples());

				MatrixVectorMultiplyWorker worker;

				using Dispatcher = vtkArrayDispatch::Dispatch3ByValueType<vtkArrayDispatch::Reals,
					vtkArrayDispatch::Reals, vtkArrayDispatch::Reals>;

				if (!Dispatcher::Execute(jacobian, velocity, acceleration, worker))
				{
					worker(jacobian.Get(), velocity, acceleration.Get());
				}

				dataset->GetPointData()->AddArray(acceleration);
			}
			qDebug() << "begin Criterion!";
			vtkSmartPointer<vtkFloatArray> qCriterion;
			vtkSmartPointer<vtkFloatArray> deltaCriterion;
			vtkSmartPointer<vtkFloatArray> lambda2Criterion;
			vtkSmartPointer<vtkFloatArray> lambdaciCriterion;
			{
				qCriterion = vtkSmartPointer<vtkFloatArray>::New();
				deltaCriterion = vtkSmartPointer<vtkFloatArray>::New();
				lambda2Criterion = vtkSmartPointer<vtkFloatArray>::New();
				lambdaciCriterion = vtkSmartPointer<vtkFloatArray>::New();

				qCriterion->SetName("q-vtk");
				deltaCriterion->SetName("delta-vtk");
				lambda2Criterion->SetName("lambda2-vtk");
				lambdaciCriterion->SetName("lambdaci-vtk");

				qCriterion->SetNumberOfComponents(1);
				deltaCriterion->SetNumberOfComponents(1);
				lambda2Criterion->SetNumberOfComponents(1);
				lambdaciCriterion->SetNumberOfComponents(1);

				qCriterion->SetNumberOfTuples(jacobian->GetNumberOfTuples());
				deltaCriterion->SetNumberOfTuples(jacobian->GetNumberOfTuples());
				lambda2Criterion->SetNumberOfTuples(jacobian->GetNumberOfTuples());
				lambdaciCriterion->SetNumberOfTuples(jacobian->GetNumberOfTuples());

				ComputeCriteriaWorker worker;
				worker(jacobian.Get(), qCriterion.Get(), deltaCriterion.Get(), lambda2Criterion.Get(), lambdaciCriterion.Get());

				dataset->GetPointData()->AddArray(qCriterion);
				dataset->GetPointData()->AddArray(deltaCriterion);
				dataset->GetPointData()->AddArray(lambda2Criterion);
				dataset->GetPointData()->AddArray(lambdaciCriterion);
			}

			qDebug() << "begin compute!";
			int dims[3];
			dataset->GetDimensions(dims);

			int xDim = dims[0];
			int yDim = dims[1];
			int zDim = dims[2];

			// vtkNew<vtkIdList> cellsList;
			vtkSmartPointer<vtkFloatArray> vel_uf_array = vtkSmartPointer<vtkFloatArray>::New();
			vel_uf_array->SetName("velocity-uf-vtk");
			vel_uf_array->SetNumberOfComponents(3);
			vel_uf_array->SetNumberOfTuples(jacobian->GetNumberOfTuples());
			calculateVelUf(xDim, yDim, zDim, dataset, vel_uf_array);
			dataset->GetPointData()->AddArray(vel_uf_array);

			// vtkNew<vtkIdList> cellsList;
			vtkSmartPointer<vtkFloatArray> oyf_array = vtkSmartPointer<vtkFloatArray>::New();
			oyf_array->SetName("oyf-vtk");
			oyf_array->SetNumberOfComponents(1);
			oyf_array->SetNumberOfTuples(jacobian->GetNumberOfTuples());
			calculateOyf(xDim, yDim, zDim, dataset, oyf_array);
			dataset->GetPointData()->AddArray(oyf_array);

			// Compute & Generate vorticity with oyf
			vtkSmartPointer<vtkFloatArray> vorticity_oyf_array = vtkSmartPointer<vtkFloatArray>::New();
			vorticity_oyf_array->SetName("vorticity-oyf-vtk");
			vorticity_oyf_array->SetNumberOfComponents(3);
			vorticity_oyf_array->SetNumberOfTuples(oyf_array->GetNumberOfTuples());

			vtkDataArray* vorticityArray = dataset->GetPointData()->GetArray("vorticity-vtk");
			float ox, oyf, oz;
			double vorticity_value[3];
			for (int i = 0; i < oyf_array->GetNumberOfTuples(); i++)
			{
				vorticityArray->GetTuple(i, vorticity_value);
				ox = vorticity_value[0];
				oyf = oyf_array->GetTuple1(i);
				oz = vorticity_value[2];
				vorticity_oyf_array->SetTuple3(i, ox, oyf, oz);
			}
			dataset->GetPointData()->AddArray(vorticity_oyf_array);

			qDebug() << "begin write new_dataset!";
			
			//生成唯一的全局ID
			//vtkNew<vtkGenerateGlobalIds> generator;
			//// generator->SetInputData(dataReader->GetOutput());
			//generator->SetInputDataObject(dataset);
			//generator->Update();

			//vtkSmartPointer<vtkDataSet> new_dataset = vtkDataSet::SafeDownCast(generator->GetOutput());
			//new_dataset->GetPointData()->SetActiveGlobalIds("");
			//new_dataset->GetCellData()->SetActiveGlobalIds("");
			//new_dataset->GetPointData()->RemoveArray("vtkGhostType");
			//vtkDataArray* arr1 = new_dataset->GetCellData()->GetArray("GlobalCellIds");
			//if(arr1!=nullptr)arr1->SetName("DomainCellIds");
			//vtkDataArray* arr2 = new_dataset->GetPointData()->GetArray("GlobalPointIds");
			//if (arr2 != nullptr)arr2->SetName("DomainPointIds");
			
			//vtkIdFilter用来代替vtkGenerateGlobalIds
			//vtkIdFilter_Ids:pointData和cellData中的id名称  
			vtkSmartPointer<vtkIdFilter> idFilter = vtkSmartPointer<vtkIdFilter>::New();
			idFilter->SetInputData(dataset);
			idFilter->PointIdsOn(); // 为点生成ID
			idFilter->CellIdsOn();  // 为单元生成ID
			idFilter->Update();

			// 获取带有全局ID的数据集
			vtkSmartPointer<vtkDataSet> new_dataset= idFilter->GetOutput();
			new_dataset->GetPointData()->SetActiveGlobalIds("vtkIdFilter_Ids");
			new_dataset->GetCellData()->SetActiveGlobalIds("vtkIdFilter_Ids");
			

			vtkDataArray* arr1 = new_dataset->GetCellData()->GetGlobalIds();
			/*if (arr1 == nullptr) {
				qDebug() << "cell id find fail";
			}else qDebug() << "cell id find success";*/
			if(arr1!=nullptr)arr1->SetName("DomainCellIds");
			vtkDataArray* arr2 = new_dataset->GetPointData()->GetGlobalIds();
			if (arr2 != nullptr)arr2->SetName("DomainPointIds");

			
			vtkSmartPointer<vtkStructuredGrid> newGrid = vtkStructuredGrid::SafeDownCast(new_dataset);
			if (newGrid != nullptr) {
				//processDouble(newGrid);
				data->setGird(newGrid);
				qDebug() << u8"转换成功";
			}else {
				qDebug() << u8"转换失败，dataset 不是 vtkStructuredGrid 类型";
			}


			//计算liutex
			string scalar_name = "vectors";
			createLiutex(newGrid, scalar_name);

			/*vtkDataArray* rotex = newGrid->GetCellData()->GetArray("Rotex");
			for (int i = 0; i < 50; i++) {
				qDebug() << "rotex i=" << i << " " << rotex->GetComponent(i, 0) << " " << rotex->GetComponent(i, 1) << " " << rotex->GetComponent(i, 2);
			}*/
			/*if (arr2 == nullptr) {
				qDebug() << "point id find fail";
			}else qDebug() << "point id find success";*/




			qDebug() << "point information";
			for (int i = 0; i < new_dataset->GetPointData()->GetNumberOfArrays(); i++) {
				string str = new_dataset->GetPointData()->GetArrayName(i);
				qDebug() << " i=" << i << " name=" << str.c_str() << " dim=" << new_dataset->GetPointData()->GetArray(i)->GetNumberOfComponents()
					<< " num=" << new_dataset->GetPointData()->GetArray(i)->GetNumberOfTuples();
			}
			qDebug() << "cell information";
			for (int i = 0; i < new_dataset->GetCellData()->GetNumberOfArrays(); i++) {
				string str = new_dataset->GetCellData()->GetArrayName(i);
				qDebug() << " i=" << i << " name=" << str.c_str() << " dim=" << new_dataset->GetCellData()->GetArray(i)->GetNumberOfComponents()
					<< " num=" << new_dataset->GetCellData()->GetArray(i)->GetNumberOfTuples();
			}
			
			

			vtkNew<vtkDataSetWriter> writer;
			writer->SetInputData(new_dataset);
			writer->SetFileName("D:/BUPT/devTest/testDev701/Dev701QtNSPlatformBase/ECHVsrc/new_data.vtk");
			//writer->SetFileTypeToBinary();
			writer->Write();
			qDebug() << "create dataset finish!";



		}

		void CreateFullDataset::calculateOyf(int xDims, int yDims, int zDims, vtkDataSet* input, vtkFloatArray* oyf_array)
		{
			vtkDataArray* vorticityArray = input->GetPointData()->GetArray(this->vectorName.c_str());
			//vtkDataArray* vorticityArray = input->GetPointData()->GetArray("vorticity");
			if (vorticityArray == nullptr)
			{
				qDebug() << " calculateOyf vorticityArray is null!";
				return;
				//vorticityArray = input->GetPointData()->GetArray("vorticity-vtk");
			}
			double o_value[3];
			double avg_oy;
			for (int k = 0; k < zDims; k++)
			{
				avg_oy = 0.0;
				// Compute the average oyf value for 1 plane
				for (int j = 0; j < yDims; j++)
				{
					for (int i = 0; i < xDims; i++)
					{
						vtkIdType pointId = i + (xDims * j) + (xDims * yDims * k);
						vorticityArray->GetTuple(pointId, o_value);
						avg_oy += o_value[1]; // Compute Average oy
					}
				}
				avg_oy = avg_oy / (xDims * yDims);

				// Assign a value to oyf_array oyf = oy - avg_oy
				for (int jj = 0; jj < yDims; jj++)
				{
					for (int ii = 0; ii < xDims; ii++)
					{
						vtkIdType pointId = ii + (xDims * jj) + (xDims * yDims * k);
						vorticityArray->GetTuple(pointId, o_value);
						double oy = o_value[1]; // Compute Average oy
						double oyf = oy - avg_oy;
						oyf_array->SetTuple1(pointId, oyf);
					}
				}
			}
		}

		void CreateFullDataset::calculateVelUf(int xDims, int yDims, int zDims, vtkDataSet* input,
			vtkFloatArray* vel_uf_array)
		{
			vtkDataArray* velocityArray = input->GetPointData()->GetArray(this->vectorName.c_str());
			if (velocityArray == nullptr) {
				qDebug() << " calculateVelUf vorticityArray is null!";
				return;
			}
			//vtkDataArray* velocityArray = input->GetPointData()->GetArray("velocity");
			double vel_value[3];
			double avg_u;
			for (int k = 0; k < zDims; k++)
			{
				avg_u = 0.0;
				// Compute the average oyf value for 1 plane
				for (int j = 0; j < yDims; j++)
				{
					for (int i = 0; i < xDims; i++)
					{
						vtkIdType pointId = i + (xDims * j) + (xDims * yDims * k);
						velocityArray->GetTuple(pointId, vel_value);
						avg_u += vel_value[0]; // Compute Average oy
					}
				}
				avg_u = avg_u / (xDims * yDims);

				// Assign a value to oyf_array oyf = oy - avg_oy
				for (int jj = 0; jj < yDims; jj++)
				{
					for (int ii = 0; ii < xDims; ii++)
					{
						vtkIdType pointId = ii + (xDims * jj) + (xDims * yDims * k);
						velocityArray->GetTuple(pointId, vel_value);
						double u = vel_value[0];
						double v = vel_value[0];
						double w = vel_value[0];
						double uF = u - avg_u;
						vel_uf_array->SetTuple3(pointId, uF, v, w);
					}
				}
			}
		}

		void CreateFullDataset::processDouble(vtkSmartPointer<vtkStructuredGrid> grid) {
			vtkSmartPointer<vtkPointData> pointData = grid->GetPointData();
			for (int i = 0; i < pointData->GetNumberOfArrays(); i++) {
				//string name=pointData->GetArray(i)->GetName();
				//scalarName.push_back(name);
				int dataType = pointData->GetArray(i)->GetDataType();
				if (dataType != VTK_FLOAT) continue;
				qDebug() << "name=" << pointData->GetArray(i)->GetName() << " dim=" << pointData->GetArray(i)->GetNumberOfComponents();
				for (int j = 0; j < pointData->GetArray(i)->GetNumberOfTuples(); j++) {
					for (int k = 0; k < pointData->GetArray(i)->GetNumberOfComponents(); k++) {
						float x = pointData->GetArray(i)->GetComponent(j, k);
						if (std::isinf(x)) {
							if (x > 0) {
								x = numeric_limits<float>::max();
							}
							else {
								x = -std::numeric_limits<float>::max();
							}
						}
						else if (isnan(x)) x = 0;
						pointData->GetArray(i)->SetComponent(j, k, x);
					}
				}


			}
		}

		void CreateFullDataset::createLiutex(vtkSmartPointer<vtkStructuredGrid> newGrid, string scalar_name) {
			vtkDataArray* velocity = newGrid->GetPointData()->GetArray(vectorName.c_str());
			if (velocity == nullptr)
			{
				qDebug() << "U array not found." << endl;
				return;
			}
			if (velocity->GetNumberOfComponents() != 3) {
				qDebug() << "U array dim is not 3." << endl;
				return;
			}
			std::vector<std::vector<int>>  cells(newGrid->GetNumberOfCells());
			for (int i = 0; i < newGrid->GetNumberOfCells(); i++) {
				vtkCell* cell = newGrid->GetCell(i);
				for (int j = 0; j < cell->GetNumberOfPoints(); j++) {
					cells[i].push_back(cell->GetPointId(j));
				}
			}
			std::vector<Liutex::Vector> U(velocity->GetNumberOfTuples());
			for (int i = 0; i < velocity->GetNumberOfTuples(); i++) {
				U[i]= Liutex::Vector(velocity->GetComponent(i,0), velocity->GetComponent(i, 1), velocity->GetComponent(i, 2));
			}
			qDebug() << "begin computeRotex." ;
			std::vector<Liutex::Vector> Rotex = Liutex::computRotex(cells, U);
			qDebug() << "end computeRotex." << endl;
			vtkSmartPointer<vtkFloatArray> Rotex_array = vtkSmartPointer<vtkFloatArray>::New();
			Rotex_array->SetName("Rotex");
			Rotex_array->SetNumberOfComponents(3);
			Rotex_array->SetNumberOfTuples(Rotex.size());
			for (int i = 0; i < Rotex.size(); i++) {
				Rotex_array->SetComponent(i, 0, Rotex[i].x());
				Rotex_array->SetComponent(i, 1, Rotex[i].y());
				Rotex_array->SetComponent(i, 2, Rotex[i].z());
			}
			newGrid->GetCellData()->AddArray(Rotex_array);
		}
	}
}

