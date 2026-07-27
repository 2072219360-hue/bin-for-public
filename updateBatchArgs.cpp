#include "updateBatchArgs.h"
updateBatchArgs::updateBatchArgs() {
	ptComputer = new USStandardAtmosphere();
	this->inpPath = "";
	this->grdPath = "";
	fullFlag = 0;
}
void updateBatchArgs::setInpAndGrd(QString inp, QString grd) {
	this->inpPath = inp;
	this->grdPath = grd;
}
void updateBatchArgs::setFullFlag(int fullFlag) {
	this->fullFlag = fullFlag;
}
void updateBatchArgs::updateInputDat(const QString& workspacePath, double H,
	double Ma, double AOA, double AOB)
{
	// 1. 通过高度H计算压力P和温度T
	std::pair<double, double> PT = ptComputer->getPT(H);
	double P = PT.first;
	double T = PT.second;
	// 2. 处理body/input.dat文件 和 head/config
	QString inputDatPath = workspacePath + "/body/input.dat";
	BodyPathChecker tempChecker;
	inputDatPath = tempChecker.checkAndGetPath(inputDatPath);
	QString headConfigDatPath = workspacePath + "/head/headconfig.dat";
	headConfigDatPath = tempChecker.checkAndGetPath(headConfigDatPath);
	QFile file(inputDatPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << u8"无法打开input.dat文件：" << inputDatPath;
		return;
	}
	if (fullFlag == 0) {
		//如果是分离网格，则要根据输入网格名称进行调整
		fileEditor.changeConfig(inputDatPath, inpPath, grdPath, 1);
	}
	else {
		//fileEditor.changeConfig(inputDatPath, inpPath, grdPath, 1);
		//如果是完整网格，则要调整成固定的输入输出
		QString dirPath = SignalInstance::getWorkSpace();
		QString headInpPath = dirPath + "/" + "head_bd.inp";
		QString headGridPath = dirPath + "/" + "head_grid.grd";
		QString bodyInpPath = dirPath + "/" + "body_genericbd.inp";
		QString bodyGridPath = dirPath + "/" + "body_grid.grd";
		fileEditor.changeConfig(headConfigDatPath, headInpPath, headGridPath, 0);
		fileEditor.changeConfig(inputDatPath, bodyInpPath, bodyGridPath, 1);
	}
	
	// 读取文件内容（使用GBK编码）
	QTextStream in(&file);
	in.setCodec("GBK");  // 关键修改：适配GBK编码的原始文件
	QStringList lines;
	while (!in.atEnd()) {
		lines.append(in.readLine());
	}
	file.close();
	// 更新[Farfield]部分参数
	bool inFarfield = false;
	string tw;
	int decimal = 1;
	for (int i = 0; i < lines.size(); ++i) {
		QString line = lines[i];
		QStringList list;
		list = line.split("=");
		if (list.at(0) == "Tw") tw = list.at(1).toStdString();
		if (line == "[Farfield]") {
			inFarfield = true;
			continue;
		}
		if (inFarfield) {
			if (line.startsWith("Ma=")) {
				lines[i] = QString("Ma=%1").arg(Ma, 0, 'f',decimal);
			}
			else if (line.startsWith("P=")) {
				lines[i] = QString("P=%1").arg(P, 0, 'f');
			}
			else if (line.startsWith("T=")) {
				lines[i] = QString("T=%1").arg(T, 0, 'f');
			}
			else if (line.startsWith("AoA=")) {
				lines[i] = QString("AoA=%1").arg(AOA, 0, 'f',decimal);
			}
			else if (line.startsWith("AoB=")) {
				lines[i] = QString("AoB=%1").arg(AOB, 0, 'f',decimal);
			}
			else if (line.startsWith("[")) {
				inFarfield = false; // 退出Farfield section
			}
		}
	}
	// 写入input.dat（保持GBK编码）
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text |
		QIODevice::Truncate)) {
		qDebug() << u8"无法写入input.dat文件：" << inputDatPath;
		return;
	}
	QTextStream out(&file);
	out.setCodec("GBK");  // 关键修改：保持与读取编码一致
	out << lines.join("\n");
	file.close();
	// 3. 处理头部配置文件headconfig.dat
	QString headConfigPath = workspacePath + "/head/headconfig.dat";
	if (QFile::exists(headConfigPath)) {
		QString dirPath = SignalInstance::getWorkSpace();
		QString headInpPath = dirPath + "/" + "head_bd.inp";
		QString headGridPath = dirPath + "/" + "head_grid.grd";
		fileEditor.changeConfig(headConfigDatPath, headInpPath, headGridPath, 0);
		//fileEditor.changeConfig(headConfigPath, inpPath, grdPath, 0);
		updateHeadConfigDat(headConfigPath, P, T, Ma, AOA, AOB, tw);
	}
}
void updateBatchArgs::updateHeadConfigDat(const QString& headConfigPath, double
	P, double T, double Ma, double AOA, double AOB, string tw)
{
	if (tw == u8"adiabatic") tw = "-1";
	QFile file(headConfigPath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << u8"无法打开headconfig.dat文件：" << headConfigPath;
		return;
	}
	// 读取文件内容（使用GBK编码）
	QTextStream in(&file);
	in.setCodec("UTF-8");  // 关键修改：适配GBK编码
	QStringList lines = in.readAll().split("\n");
	file.close();
	int decimal = 1;
	// 更新第12行参数（与原saveHeadEdgeConfig逻辑一致）
	if (lines.size() > 12) {
		lines[12] = QString("%1 %2 %3 %4 %5 %6")
			.arg(P, 0, 'f', 6)
			.arg(T, 0, 'f', 6)
			// 压力
			// 温度
			.arg(Ma, 0, 'f', decimal)   // 马赫数
			.arg(AOA, 0, 'f', decimal)  // 迎角
			.arg(AOB, 0, 'f', decimal)  // 侧滑角
			.arg(QString::fromStdString(tw));  // 因为tw跟这几个参数在一行，所以每次改要带上他
	}
	// 写入headconfig.dat（保持GBK编码）
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text |
		QIODevice::Truncate)) {
		qDebug() << u8"无法写入headconfig.dat文件：" << headConfigPath;
		return;
	}
	QTextStream out(&file);
	out.setCodec("UTF-8");  // 关键修改：保持编码一致
	out << lines.join("\n");
	file.close();
}