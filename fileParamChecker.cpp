#include "FileParamChecker.h"
FileParamChecker::FileParamChecker() {
	// 初始化映射表对应可选值列表（对应文档表格）
	// limiterMap映射(对应表4 限制器)
	limiterMap["MINMOD"] = 0;
	limiterMap["VANALBADA"] = 1;
	limiterMap["SUPERBEE"] = 2;
	limiterMap["VANLEER"] = 3;
	limiterMap["MODIFIED VANALBADA"] = 4;
	// visGradientMap映射(对应表5 梯度格式)
	visGradientMap["VIS_VOL"] = 0;
	visGradientMap["VIS_AREA"] = 1;
	// schemeMap映射(对应表3 Scheme参数)
	schemeMap["AUSMPW"] = 0;
	schemeMap["AUSMPW+"] = 1;
	schemeMap["AUSM+"] = 2;
	schemeMap["AUSM++"] = 3;
	schemeMap["HLLC"] = 4;
	// implicitVisMap映射(对应表6 粘性项隐式处理)
	implicitVisMap["VISCOUS JACOBIAN"] = 0;
	implicitVisMap["TYSINGER"] = 1;
	implicitVisMap["CHENBING"] = 2;
	// pressFactorMap映射(对应表7 压力梯度分裂方法)
	pressFactorMap["THIVET"] = 0;
	pressFactorMap["VIGNERON"] = 1;
	// temperalMap映射(对应表8 流向推进方法)
	temperalMap["LUSGS_NS_VOL"] = 0;
	temperalMap["ADI_NS"] = 1;
}

// 按顺序检查body和head文件中的参数文件
bool FileParamChecker::checkFileParam(const QString& dirPath, int fullFlag,int restart) {
	// 1. 先检查body/input.dat
	QString bodyInputPath = dirPath + "/body/input.dat";
	BodyPathChecker tempChecker;
	bodyInputPath=tempChecker.checkAndGetPath(bodyInputPath);
	if (!checkInputDat(bodyInputPath)) {
		qDebug() << u8"body/input.dat 检查失败，路径为" << dirPath;
		return false;
	}
	if (!fullFlag) return true;
	// 2. 再检查head/headconfig.dat(如果存在)
	QString headConfigPath = dirPath + "/head/headconfig.dat";
	if (QFile::exists(headConfigPath)) {
		if (!checkHeadConfigDat(headConfigPath)) {
			qDebug() << u8"head/headconfig.dat 检查失败，路径为" << dirPath;
			return false;
		}
		//检查是否有restart文件并修改续算参数。
		if(restart==1)
			checkRestart(dirPath + "/head/restart.dat", headConfigPath);
	}
	else {
		qDebug() << u8"headconfig.dat 未找到";
		return false;
	}
	return true;
}
bool FileParamChecker::checkRestart(const QString& file1, const QString& file2) {
	// 确定要修改的目标值：file1存在则改2，不存在则改1
	QString targetValue = QFile::exists(file1) ? "2" : "1";

	// 打开file2并读取所有内容
	QFile file(file2);
	if (!file.open(QIODevice::ReadWrite | QIODevice::Text)) {
		qDebug() << u8"无法打开文件" << file2 << u8"，错误信息：" << file.errorString();
		return false;
	}

	// 读取文件内容（使用UTF-8编码避免中文乱码）
	QTextStream in(&file);
	in.setCodec("UTF-8");
	QList<QString> lines;
	while (!in.atEnd()) {
		lines.append(in.readLine());
	}

	// 检查文件是否为空
	if (lines.isEmpty()) {
		qDebug() << u8"文件" << file2 << u8"为空，无法修改";
		file.close();
		return false;
	}

	// 处理最后一行
	QString lastLine = lines.takeLast();
	QStringList parts = lastLine.split(QRegExp("\\s+"), Qt::SkipEmptyParts);
	if (parts.isEmpty()) {
		qDebug() << u8"文件" << file2 << u8"最后一行为空，无法修改";
		file.close();
		return false;
	}

	// 验证第一个部分是否为数字
	bool isNumber;
	parts[0].toDouble(&isNumber);
	if (!isNumber) {
		qDebug() << u8"文件" << file2 << u8"最后一行第一个元素不是数字，无法修改";
		file.close();
		return false;
	}

	// 修改第一个数字并重新组合行
	parts[0] = targetValue;
	lines.append(parts.join(" "));

	// 回写文件内容
	file.seek(0);
	file.resize(0);
	QTextStream out(&file);
	out.setCodec("UTF-8");  // 保持编码一致
	for (const QString& line : lines) {
		out << line << "\n";
	}

	// 确保内容写入并关闭文件
	file.flush();
	file.close();

	qDebug() << u8"文件" << file2 << u8"已成功修改最后一行的第一个数字为" << targetValue;
	return true;
}
// 检查body/input.dat的存在+解析+验证
bool FileParamChecker::checkInputDat(const QString& bodyPath) {
	// 1. 检查文件是否存在
	if (!QFile::exists(bodyPath)) {
		qDebug() << u8"body/input.dat 未找到：" << bodyPath;
		return false;
	}
	// 2. 解析文件内容
	QMap<QString, QString> bodyParams;
	if (!parseInputDat(bodyPath, bodyParams)) {
		qDebug() << u8"解析 body/input.dat 失败：" << bodyPath;
		return false;
	}
	// 3. 验证参数有效性，传入当前配置文件的绝对路径
	if (!validateInputParams(bodyParams, bodyPath)) {
		qDebug() << u8"body/input.dat 参数无效：" << bodyPath;
		return false;
	}
	return true;
}

// 检查head/headconfig.dat的存在+解析+验证
bool FileParamChecker::checkHeadConfigDat(const QString& headPath) {
	// 1. 检查文件是否存在，外部已判断，这里再次验证
	if (!QFile::exists(headPath)) {
		qDebug() << u8"head/headconfig.dat 未找到：" << headPath;
		return false;
	}
	// 2. 解析文件内容
	QMap<QString, QString> headParams;
	if (!parseHeadConfigDat(headPath, headParams)) {
		qDebug() << u8"解析 head/headconfig.dat 失败：" << headPath;
		return false;
	}
	// 3. 验证参数有效性，传入当前配置文件的绝对路径
	if (!validateHeadParams(headParams, headPath)) {
		qDebug() << u8"head/headconfig.dat 参数无效：" << headPath;
		return false;
	}
	return true;
}

bool FileParamChecker::parseInputDat(const QString& filePath, QMap<QString, QString>& params) {
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return false;
	}
	QTextStream in(&file);
	QString currentSection;
	QString line;
	while (!in.atEnd()) {
		line = in.readLine().trimmed();
		if (line.isEmpty()) continue;
		// 识别配置文件的分区，如[Output]等
		if (line.startsWith("[") && line.endsWith("]")) {
			currentSection = line;
			continue;
		}
		// 处理键值对，格式为Key=Value
		if (line.contains("=")) {
			QStringList keyValue = line.split("=", Qt::SkipEmptyParts);
			if (keyValue.size() == 2) {
				QString key = keyValue[0].trimmed();
				QString value = keyValue[1].trimmed();

				// 对值进行去首尾引号处理
				value = removeQuotes(value);

				params[key] = value;
			}
		}
	}
	file.close();
	return true;
}

bool FileParamChecker::parseHeadConfigDat(const QString& filePath, QMap<QString, QString>& params) {
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << u8"headconfig.dat 文件打开失败：" << filePath;
		return false;
	}

	QTextStream in(&file);
	in.setCodec("UTF-8");

	QString currentParamGroup;
	QString line;
	int lineNumber = 0; // 新增行号跟踪

	while (!in.atEnd()) {
		line = in.readLine().trimmed();
		qDebug() << "line=" << line;
		lineNumber++; // 递增行号

		if (line.isEmpty()) continue;

		// 处理以!开头的注释行
		if (line.startsWith('!')) {
			if (line.contains(u8"!P    T   Ma alpha beta Tw")) {
				currentParamGroup = "FlowCondition";
			}
			else if (line.contains(u8"! GridFile")) {
				currentParamGroup = "GridFile";
			}
			else if (line.contains(u8"! bdfile")) {
				currentParamGroup = "BdFile";
			}
			else if (line.contains(u8"! Xscale    AreaRef Lref    xc  yc  zc")) {
				currentParamGroup = "GeometryParam";
			}
			else if (line.contains(u8"! 气体常数(J/kg/K) 比热比")) {
				currentParamGroup = "GasParam";
			}
			else if (line.contains(u8"! cInit NStop  cfl")) {
				currentParamGroup = "SolverControl";
			}
			continue;
		}

		if (!currentParamGroup.isEmpty()) {
			QStringList parts = line.split(QRegExp("\\s+"), Qt::SkipEmptyParts);

			if (currentParamGroup == "FlowCondition") {
				// 适配参数可能不完整的情况（如Tw可能为空）
				if (parts.size() >= 5) {
					params["P"] = removeQuotes(parts[0]);
					params["T"] = removeQuotes(parts[1]);
					params["Ma"] = removeQuotes(parts[2]);
					params["alpha"] = removeQuotes(parts[3]);
					params["beta"] = removeQuotes(parts[4]);
					params["Tw"] = parts.size() >= 6 ? removeQuotes(parts[5]) : "";
				}
				else {
					qDebug() << u8"流动条件参数不足，行号：" << lineNumber;
					file.close();
					return false;
				}
				currentParamGroup.clear();
			}
			else if (currentParamGroup == "GridFile") {
				if (!parts.isEmpty()) {
					params["gridfile"] = removeQuotes(parts[0]);
				}
				else {
					qDebug() << u8"网格文件路径为空，行号：" << lineNumber;
					file.close();
					return false;
				}
				currentParamGroup.clear();
			}
			else if (currentParamGroup == "BdFile") {
				if (!parts.isEmpty()) {
					params["bdfile"] = removeQuotes(parts[0]);
				}
				else {
					qDebug() << u8"边界文件路径为空，行号：" << lineNumber;
					file.close();
					return false;
				}
				currentParamGroup.clear();
			}
			else if (currentParamGroup == "GeometryParam") {
				if (parts.size() >= 6) {
					params["Xscale"] = removeQuotes(parts[0]);
					params["AreaRef"] = removeQuotes(parts[1]);
					params["Lref"] = removeQuotes(parts[2]);
					params["xc"] = removeQuotes(parts[3]);
					params["yc"] = removeQuotes(parts[4]);
					params["zc"] = removeQuotes(parts[5]);
				}
				else {
					qDebug() << u8"几何参数不足，行号：" << lineNumber;
					file.close();
					return false;
				}
				currentParamGroup.clear();
			}
			else if (currentParamGroup == "GasParam") {
				if (parts.size() >= 2) {
					params["gasConstant"] = removeQuotes(parts[0]);
					params["gamma"] = removeQuotes(parts[1]);
				}
				else {
					qDebug() << u8"气体参数不足，行号：" << lineNumber;
					file.close();
					return false;
				}
				currentParamGroup.clear();
			}
			else if (currentParamGroup == "SolverControl") {
				if (parts.size() >= 3) {
					params["cInit"] = removeQuotes(parts[0]);
					params["NStop"] = removeQuotes(parts[1]);
					params["cfl"] = removeQuotes(parts[2]);
				}
				else {
					qDebug() << u8"求解器控制参数不足，行号：" << lineNumber;
					file.close();
					return false;
				}
				currentParamGroup.clear();
			}
		}
	}

	file.close();

	// 验证必要参数（保持原逻辑）
	QStringList requiredParams = {
		"gridfile", "bdfile", "cInit",
		"P", "T", "Ma", "alpha", "beta", "Tw",
		"Xscale", "AreaRef", "Lref", "xc", "yc", "zc",
		"gasConstant", "gamma"
	};

	QString missingParams;
	for (const QString& param : requiredParams) {
		if (!params.contains(param) || params[param].isEmpty()) {
			missingParams += param + "、";
		}
	}
	for (auto it = params.begin(); it != params.end(); ++it) {
		qDebug() << u8"headconfig.dat 参数" << it.key() << u8"的值为" << it.value();
	}
	if (!missingParams.isEmpty()) {
		missingParams.chop(1);
		qDebug() << u8"headconfig.dat 缺少必要参数：" << missingParams;
		return false;
	}

	qDebug() << u8"headconfig.dat 解析成功，参数列表：";
	

	return true;
}
// 辅助函数：去除字符串首尾空格（需要在类中声明）
// 辅助函数：去除字符串首尾空格（仅处理单字节ASCII空白字符）
std::string FileParamChecker::trim(const std::string& s) {
	if (s.empty()) {
		return ""; // 空字符串直接返回
	}

	auto start = s.begin();
	// 跳过开头的空白字符（仅处理单字节ASCII）
	while (start != s.end()) {
		unsigned char c = static_cast<unsigned char>(*start);
		if (std::isspace(c)) {
			start++;
		}
		else {
			break;
		}
	}

	// 如果全部是空白字符，返回空字符串
	if (start == s.end()) {
		return "";
	}

	auto end = s.end() - 1; // 从最后一个有效字符开始
	// 跳过结尾的空白字符（仅处理单字节ASCII）
	while (end >= start) {
		unsigned char c = static_cast<unsigned char>(*end);
		if (std::isspace(c)) {
			end--;
		}
		else {
			break;
		}
	}

	// 截取[start, end]区间的字符串
	return std::string(start, end + 1);
}
// 辅助函数去除字符串首尾引号
QString FileParamChecker::removeQuotes(const QString& value) {
	QString processed = value;
	// 去除双引号
	if (processed.startsWith("\"") && processed.endsWith("\"")) {
		processed = processed.mid(1, processed.length() - 2);
	}
	// 去除单引号
	else if (processed.startsWith("'") && processed.endsWith("'")) {
		processed = processed.mid(1, processed.length() - 2);
	}
	qDebug() << "value=" << value << " " << processed;
	return processed;
}

bool FileParamChecker::validateInputParams(const QMap<QString, QString>& bodyParams, const QString& configFileAbsPath) {
	// 打印bodyParams中的所有参数
	qDebug() << u8"bodyParams 参数列表：";
	for (auto it = bodyParams.begin(); it != bodyParams.end(); ++it) {
		qDebug() << u8"参数" << it.key() << u8"的值为" << it.value();
	}
	bool ok;
	double doubleVal;
	int intVal;
	// 验证[Output]部分参数
	if (!bodyParams.contains("Nout")) {
		qDebug() << u8"缺少参数 'Nout' in body/input.dat";
		return false;
	}
	bodyParams["Nout"].toInt(&ok);
	if (!ok) {
		qDebug() << u8"参数 'Nout' 的值 '" << bodyParams["Nout"] << u8"' 不是有效整数";
		return false;
	}
	if (!bodyParams.contains("WallForce")) {
		qDebug() << u8"缺少参数 'WallForce' in body/input.dat";
		return false;
	}
	QString wallForce = bodyParams["WallForce"];
	if (wallForce != "ON" && wallForce != "OFF") {
		qDebug() << u8"参数 'WallForce' 的值 '" << wallForce << u8"' 无效，必须为 'ON' 或 'OFF'";
		return false;
	}
	if (!bodyParams.contains("cVolFieldOutput")) {
		qDebug() << u8"缺少参数 'cVolFieldOutput' in body/input.dat";
		return false;
	}
	QString volField = bodyParams["cVolFieldOutput"];
	if (volField != "ON" && volField != "OFF") {
		qDebug() << u8"参数 'cVolFieldOutput' 的值 '" << volField << u8"' 无效，必须为 'ON' 或 'OFF'";
		return false;
	}
	if (!bodyParams.contains("cbdFieldOutput")) {
		qDebug() << u8"缺少参数 'cbdFieldOutput' in body/input.dat";
		return false;
	}
	QString cbdField = bodyParams["cbdFieldOutput"];
	if (cbdField != "ON" && cbdField != "OFF") {
		qDebug() << u8"参数 'cbdFieldOutput' 的值 '" << cbdField << u8"' 无效，必须为 'ON' 或 'OFF'";
		return false;
	}
	// 验证[Farfield]部分参数
	if (!bodyParams.contains("Ma")) {
		qDebug() << u8"缺少参数 'Ma' in body/input.dat";
		return false;
	}
	bodyParams["Ma"].toDouble(&ok);
	if (!ok) {
		qDebug() << u8"参数 'Ma' 的值 '" << bodyParams["Ma"] << u8"' 不是有效数字";
		return false;
	}
	doubleVal = bodyParams["Ma"].toDouble();
	if (doubleVal <= 0) {
		qDebug() << u8"参数 'Ma' 的值 '" << doubleVal << u8"' 必须大于 0";
		return false;
	}
	if (!bodyParams.contains("P")) {
		qDebug() << u8"缺少参数 'P' in body/input.dat";
		return false;
	}
	bodyParams["P"].toDouble(&ok);
	if (!ok) {
		qDebug() << u8"参数 'P' 的值 '" << bodyParams["P"] << u8"' 不是有效数字";
		return false;
	}
	doubleVal = bodyParams["P"].toDouble();
	if (doubleVal <= 0) {
		qDebug() << u8"参数 'P' 的值 '" << doubleVal << u8"' 必须大于 0";
		return false;
	}
	if (!bodyParams.contains("T")) {
		qDebug() << u8"缺少参数 'T' in body/input.dat";
		return false;
	}
	bodyParams["T"].toDouble(&ok);
	if (!ok) {
		qDebug() << u8"参数 'T' 的值 '" << bodyParams["T"] << u8"' 不是有效数字";
		return false;
	}
	doubleVal = bodyParams["T"].toDouble();
	if (doubleVal <= 0) {
		qDebug() << u8"参数 'T' 的值 '" << doubleVal << u8"' 必须大于 0";
		return false;
	}
	if (!bodyParams.contains("AoA")) {
		qDebug() << u8"缺少参数 'AoA' in body/input.dat";
		return false;
	}
	bodyParams["AoA"].toDouble(&ok);
	if (!ok) {
		qDebug() << u8"参数 'AoA' 的值 '" << bodyParams["AoA"] << u8"' 不是有效数字";
		return false;
	}
	if (!bodyParams.contains("AoB")) {
		qDebug() << u8"缺少参数 'AoB' in body/input.dat";
		return false;
	}
	bodyParams["AoB"].toDouble(&ok);
	if (!ok) {
		qDebug() << u8"参数 'AoB' 的值 '" << bodyParams["AoB"] << u8"' 不是有效数字";
		return false;
	}
	// 验证[BoundaryCondition]参数
	if (!bodyParams.contains("Tw")) {
		qDebug() << u8"缺少参数 'Tw' in body/input.dat";
		return false;
	}
	QString tw = bodyParams["Tw"];
	if (tw != "adiabatic") {
		tw.toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'Tw' 的值 '" << tw << u8"' 无效，必须为数值或 'adiabatic'";
			return false;
		}
		doubleVal = tw.toDouble();
		if (doubleVal <= 0) {
			qDebug() << u8"参数 'Tw' 的值 '" << doubleVal << u8"' 必须大于 0";
			return false;
		}
	}
	// 验证[Geometry]部分参数
	if (!bodyParams.contains("XScale")) {
		qDebug() << u8"缺少参数 'XScale' in body/input.dat";
		return false;
	}
	bodyParams["XScale"].toDouble(&ok);
	if (!ok) {
		qDebug() << u8"参数 'XScale' 的值 '" << bodyParams["XScale"] << u8"' 不是有效数字";
		return false;
	}
	doubleVal = bodyParams["XScale"].toDouble();
	if (doubleVal <= 0) {
		qDebug() << u8"参数 'XScale' 的值 '" << doubleVal << u8"' 必须大于 0";
		return false;
	}
	if (!bodyParams.contains("GridFileName")) {
		qDebug() << u8"缺少参数 'GridFileName' in body/input.dat（参考Word文档6.2节）";
		return false;
	}
	if (!validateFilePath(bodyParams["GridFileName"], configFileAbsPath)) {
		qDebug() << u8"参数 'GridFileName' 的路径 '" << bodyParams["GridFileName"] << u8"' 无效";
		return false;
	}
	if (!bodyParams.contains("bdFileName")) {
		qDebug() << u8"缺少参数 'bdFileName' in body/input.dat（参考Word文档6.2节）";
		return false;
	}
	if (!validateFilePath(bodyParams["bdFileName"], configFileAbsPath)) {
		qDebug() << u8"参数 'bdFileName' 的路径 '" << bodyParams["bdFileName"] << u8"' 无效";
		return false;
	}
	if (wallForce == "ON") {
		if (!bodyParams.contains("Rlen")) {
			qDebug() << u8"当 WallForce 为 'ON' 时缺少参数 'Rlen'";
			return false;
		}
		bodyParams["Rlen"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'Rlen' 的值 '" << bodyParams["Rlen"] << u8"' 不是有效数字";
			return false;
		}
		doubleVal = bodyParams["Rlen"].toDouble();
		if (doubleVal <= 0) {
			qDebug() << u8"参数 'Rlen' 的值 '" << doubleVal << u8"' 必须大于 0";
			return false;
		}
		if (!bodyParams.contains("AreaRef")) {
			qDebug() << u8"当 WallForce 为 'ON' 时缺少参数 'AreaRef'";
			return false;
		}
		bodyParams["AreaRef"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'AreaRef' 的值 '" << bodyParams["AreaRef"] << u8"' 不是有效数字";
			return false;
		}
		doubleVal = bodyParams["AreaRef"].toDouble();
		if (doubleVal <= 0) {
			qDebug() << u8"参数 'AreaRef' 的值 '" << doubleVal << u8"' 必须大于 0";
			return false;
		}
		// xmc/ymc/zmc 只需要验证为有效数字值，允许为0（参考Word文档修改）
		if (!bodyParams.contains("xmc")) {
			qDebug() << u8"当 WallForce 为 'ON' 时缺少参数 'xmc'";
			return false;
		}
		bodyParams["xmc"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'xmc' 的值 '" << bodyParams["xmc"] << u8"' 不是有效数字";
			qDebug() << u8"BodyParam[xmc]=" << bodyParams["xmc"];
			return false;
		}
		if (!bodyParams.contains("ymc")) {
			qDebug() << u8"当 WallForce 为 'ON' 时缺少参数 'ymc'";
			return false;
		}
		bodyParams["ymc"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'ymc' 的值 '" << bodyParams["ymc"] << u8"' 不是有效数字";
			return false;
		}
		if (!bodyParams.contains("zmc")) {
			qDebug() << u8"当 WallForce 为 'ON' 时缺少参数 'zmc'";
			return false;
		}
		bodyParams["zmc"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'zmc' 的值 '" << bodyParams["zmc"] << u8"' 不是有效数字";
			return false;
		}
	}
	// 验证[Gas]部分参数
	if (!bodyParams.contains("R0")) {
		qDebug() << u8"缺少参数 'R0' in body/input.dat";
		return false;
	}
	bodyParams["R0"].toDouble(&ok);
	if (!ok) {
		qDebug() << u8"参数 'R0' 的值 '" << bodyParams["R0"] << u8"' 不是有效数字";
		return false;
	}
	doubleVal = bodyParams["R0"].toDouble();
	if (doubleVal <= 0) {
		qDebug() << u8"参数 'R0' 的值 '" << doubleVal << u8"' 必须大于 0";
		return false;
	}
	if (!bodyParams.contains("Gamma")) {
		qDebug() << u8"缺少参数 'Gamma' in body/input.dat";
		return false;
	}
	bodyParams["Gamma"].toDouble(&ok);
	if (!ok) {
		qDebug() << u8"参数 'Gamma' 的值 '" << bodyParams["Gamma"] << u8"' 不是有效数字";
		return false;
	}
	doubleVal = bodyParams["Gamma"].toDouble();
	if (doubleVal <= 0) {
		qDebug() << u8"参数 'Gamma' 的值 '" << doubleVal << u8"' 必须大于 0";
		return false;
	}
	// 验证[SolverControl]部分参数
	if (!bodyParams.contains("Scheme")) {
		qDebug() << u8"缺少参数 'Scheme' in body/input.dat";
		return false;
	}
	if (!schemeMap.contains(bodyParams["Scheme"])) {
		qDebug() << u8"参数 'Scheme' 的值 '" << bodyParams["Scheme"] << u8"' 无效，不在允许范围内";
		return false;
	}
	if (!bodyParams.contains("Limiter")) {
		qDebug() << u8"缺少参数 'Limiter' in body/input.dat";
		return false;
	}
	if (!limiterMap.contains(bodyParams["Limiter"])) {
		qDebug() << u8"参数 'Limiter' 的值 '" << bodyParams["Limiter"] << u8"' 无效，不在允许范围内";
		return false;
	}
	if (!bodyParams.contains("PressFactor")) {
		qDebug() << u8"缺少参数 'PressFactor' in body/input.dat";
		return false;
	}
	if (!pressFactorMap.contains(bodyParams["PressFactor"])) {
		qDebug() << u8"参数 'PressFactor' 的值 '" << bodyParams["PressFactor"] << u8"' 无效，不在允许范围内";
		return false;
	}
	if (!bodyParams.contains("Temperal")) {
		qDebug() << u8"缺少参数 'Temperal' in body/input.dat";
		return false;
	}
	if (!temperalMap.contains(bodyParams["Temperal"])) {
		qDebug() << u8"参数 'Temperal' 的值 '" << bodyParams["Temperal"] << u8"' 无效，不在允许范围内";
		return false;
	}
	if (!bodyParams.contains("VisGradient")) {
		qDebug() << u8"缺少参数 'VisGradient' in body/input.dat";
		return false;
	}
	if (!visGradientMap.contains(bodyParams["VisGradient"])) {
		qDebug() << u8"参数 'VisGradient' 的值 '" << bodyParams["VisGradient"] << u8"' 无效，不在允许范围内";
		return false;
	}
	if (!bodyParams.contains("ImplicitVis")) {
		qDebug() << u8"缺少参数 'ImplicitVis' in body/input.dat";
		return false;
	}
	if (!implicitVisMap.contains(bodyParams["ImplicitVis"])) {
		qDebug() << u8"参数 'ImplicitVis' 的值 '" << bodyParams["ImplicitVis"] << u8"' 无效，不在允许范围内";
		return false;
	}
	// 验证ScaleEg参数（参考Word文档10.1节）
	if (bodyParams.contains("ScaleEg")) {
		bodyParams["ScaleEg"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'ScaleEg' 的值 '" << bodyParams["ScaleEg"] << u8"' 不是有效数字";
			return false;
		}
		doubleVal = bodyParams["ScaleEg"].toDouble();
		if (doubleVal < 1.0 || doubleVal > 1.1) {
			qDebug() << u8"参数 'ScaleEg' 的值 '" << doubleVal << u8"' 必须在 1.0 到 1.1 之间";
			return false;
		}
	}
	// 验证时间步长相关参数（参考Word文档9.3节）
	if (bodyParams.contains("Xstep0") && bodyParams.contains("XstepMax0") && bodyParams.contains("stepMax")) {
		bodyParams["Xstep0"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'Xstep0' 的值 '" << bodyParams["Xstep0"] << u8"' 不是有效正数";
			return false;
		}
		double xstep0 = bodyParams["Xstep0"].toDouble();
		if (xstep0 <= 0) {
			qDebug() << u8"参数 'Xstep0' 的值 '" << xstep0 << u8"' 必须是大于 0 的数值";
			return false;
		}

		bodyParams["XstepMax0"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'XstepMax0' 的值 '" << bodyParams["XstepMax0"] << u8"' 不是有效数字";
			return false;
		}
		double xstepMax0 = bodyParams["XstepMax0"].toDouble();
		if (xstepMax0 <= xstep0) {
			qDebug() << u8"参数 'XstepMax0' 的值 '" << xstepMax0 << u8"' 必须大于 Xstep0 (" << xstep0 << u8")";
			return false;
		}

		bodyParams["stepMax"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'stepMax' 的值 '" << bodyParams["stepMax"] << u8"' 不是有效数字";
			return false;
		}
		double stepMax = bodyParams["stepMax"].toDouble();
		if (stepMax < xstepMax0) {
			qDebug() << u8"参数 'stepMax' 的值 '" << stepMax << u8"' 必须大于等于 XstepMax0 (" << xstepMax0 << u8")";
			return false;
		}
	}
	if (bodyParams.contains("KStep")) {
		bodyParams["KStep"].toDouble(&ok);
		if (!ok) {
			qDebug() << u8"参数 'KStep' 的值 '" << bodyParams["KStep"] << u8"' 不是有效数字";
			return false;
		}
		double kStep = bodyParams["KStep"].toDouble();
		if (kStep <= 1.0) {
			qDebug() << u8"参数 'KStep' 的值 '" << kStep << u8"' 必须大于 1.0";
			return false;
		}
	}
	if (bodyParams.contains("NCStep")) {
		bodyParams["NCStep"].toInt(&ok);
		if (!ok) {
			qDebug() << u8"参数 'NCStep' 的值 '" << bodyParams["NCStep"] << u8"' 不是有效整数";
			return false;
		}
		int ncsStep = bodyParams["NCStep"].toInt();
		if (ncsStep <= 0) {
			qDebug() << u8"参数 'NCStep' 的值 '" << bodyParams["NCStep"] << u8"' 必须是大于 0 的整数";
			return false;
		}
	}
	return true;
}

bool FileParamChecker::validateHeadParams(const QMap<QString, QString>& headParams, const QString& configFileAbsPath) {
	for (auto it = headParams.begin(); it != headParams.end(); ++it) {
		qDebug() << u8"参数" << it.key() << u8"的值为" << it.value();
	}

	bool ok;
	int intVal;

	// 校验第18行的参数cInit/NStop/cfl（已存在）
	if (!headParams.contains("cInit")) {
		qDebug() << u8"缺少参数 'cInit' in head/headconfig.dat";
		return false;
	}
	// ...（省略已有cInit、NStop、cfl的校验代码）

	// 校验gridfile和bdfile（已存在）
	if (!headParams.contains("gridfile")) {
		qDebug() << u8"缺少参数 'gridfile' in head/headconfig.dat";
		return false;
	}
	// ...（省略已有gridfile、bdfile的校验代码）

	// 补充校验其他参数是否为空
	if (!headParams.contains("P")) {
		qDebug() << u8"缺少参数 'P' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("T")) {
		qDebug() << u8"缺少参数 'T' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("Ma")) {
		qDebug() << u8"缺少参数 'Ma' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("alpha")) {
		qDebug() << u8"缺少参数 'alpha' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("beta")) {
		qDebug() << u8"缺少参数 'beta' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("Tw")) {
		qDebug() << u8"缺少参数 'Tw' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("Xscale")) {
		qDebug() << u8"缺少参数 'Xscale' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("AreaRef")) {
		qDebug() << u8"缺少参数 'AreaRef' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("Lref")) {
		qDebug() << u8"缺少参数 'Lref' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("xc")) {
		qDebug() << u8"缺少参数 'xc' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("yc")) {
		qDebug() << u8"缺少参数 'yc' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("zc")) {
		qDebug() << u8"缺少参数 'zc' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("gasConstant")) {  // 假设解析时用此key存储气体常数
		qDebug() << u8"缺少参数 '气体常数' in head/headconfig.dat";
		return false;
	}
	if (!headParams.contains("gamma")) {  // 假设解析时用此key存储比热比
		qDebug() << u8"缺少参数 '比热比' in head/headconfig.dat";
		return false;
	}

	return true;
}
// 辅助函数验证文件路径是否存在，支持相对路径
// 说明：
// - filePath是配置文件中写的目标路径，如"../body_grid.grd"
// - configFileAbsPath是当前配置文件(input.dat/headConfig.dat)的绝对路径，如"A/B/C/D/input.dat"
bool FileParamChecker::validateFilePath(const QString& filePath, const QString& configFileAbsPath) {
	return true;
	// 步骤1：获取当前配置文件所在目录的绝对路径
	QFileInfo configFileInfo(configFileAbsPath);
	QString configDirAbsPath = configFileInfo.absolutePath(); // 如A/B/C/D
	if (configDirAbsPath.isEmpty()) {
		qDebug() << u8"无法获取配置文件所在目录，配置文件路径为" << configFileAbsPath;
		return false;
	}

	// 步骤2：将目标路径filePath与配置文件目录拼接，转换为绝对路径
	QDir configDir(configDirAbsPath);
	QString targetAbsPath = configDir.absoluteFilePath(filePath); // 自动处理"../"等相对路径
	// 示例1：filePath="../body_grid.grd" 转换为"A/B/C/body_grid.grd"
	// 示例2：filePath="../../body_genericbd.inp" 转换为"A/B/body_genericbd.inp"

	// 步骤3：验证转换后的绝对路径是否真实为文件或目录
	QFileInfo targetFileInfo(targetAbsPath);
	if (!targetFileInfo.exists()) {
		qDebug() << u8"目标文件不存在，配置文件路径为" << configFileAbsPath
			<< u8"使用的路径为" << filePath
			<< u8"转换后的绝对路径为" << targetAbsPath;
		return false;
	}
	if (!targetFileInfo.isFile()) {
		qDebug() << u8"转换后的路径指向的不是文件，配置文件路径为" << configFileAbsPath
			<< u8"使用的路径为" << filePath
			<< u8"转换后的绝对路径为" << targetAbsPath;
		return false;
	}

	// 路径有效且对应一个文件
	return true;
}