#include "sync_trData_input.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <regex>

bool sync_trData_input::isDataLine(const std::string& line) {
	return std::regex_match(line, std::regex("^\\s*\\d.*"));
}

void sync_trData_input::replaceSectionData(const std::string& inputFile,
	const std::string& trDataFile,
	const std::string& outputFile) {
	const std::string inputSectionHeader = "#Section Xstep0 XStepMax0 XStepMinEnd   StepMax   KStep    NCStep";
	const std::string trDataSectionHeader = "#Section Xstep0 xstepmin min1  max kstep  ncstep";
	const std::string strpCtrl = "[StepCtrl]";

	std::ifstream input(inputFile);
	std::ifstream trdata(trDataFile);

	if (!input || !trdata) {
		std::cerr << "无法打开文件" << std::endl;
		return;
	}

	std::vector<std::string> inputLines;
	std::string line;

	// 读取 input 文件内容
	while (std::getline(input, line)) {
		inputLines.push_back(line);
	}

	// 找到 input 中段落位置
	/*size_t sectionStartInput = 0;
	while (sectionStartInput < inputLines.size() && inputLines[sectionStartInput].find(inputSectionHeader) == std::string::npos) {
		sectionStartInput++;
	}*/
	size_t sectionStartInput = 1;
	while (sectionStartInput < inputLines.size() && inputLines[sectionStartInput-1].find(strpCtrl) == std::string::npos) {
		sectionStartInput++;
	}

	if (sectionStartInput == inputLines.size()) {
		std::cerr << "未找到 input 文件中指定段落" << std::endl;
		return;
	}

	// 找到 input 中数据开始的位置
	size_t inputDataStart = sectionStartInput + 1;
	while (inputDataStart < inputLines.size() && !isDataLine(inputLines[inputDataStart])) {
		inputDataStart++;
	}

	// 读取 trData 中目标段落后的数据行
	std::vector<std::string> trDataSection;
	bool inTargetSection = false;
	while (std::getline(trdata, line)) {
		if (!inTargetSection) {
			if (line.find(trDataSectionHeader) != std::string::npos) {
				inTargetSection = true;
			}
		}
		else {
			if (isDataLine(line)) {
				trDataSection.push_back(line);
			}
			else if (!line.empty() && line[0] == '#') {
				break; // 进入下一段 section，停止
			}
		}
	}
	input.close();
	trdata.close();

	// 构造新文件内容
	std::ofstream output(outputFile);
	for (size_t i = 0; i < inputDataStart; ++i) {
		output << inputLines[i] << "\n";
	}
	for (const auto& trline : trDataSection) {
		output << trline << "\n";
	}
	// 如果 input 后面还有非数据内容，也写入
	size_t inputDataEnd = inputLines.size();
	output << "\n";
	for (size_t i = inputDataStart; i < inputDataEnd; ++i) {
		if (!isDataLine(inputLines[i])) {
			output << inputLines[i] << "\n";
		}
	}
	output.close();

	std::cout << "替换完成，已写入 " << outputFile << std::endl;
}

void sync_trData_input::run() {
	BodyPathChecker tempChecker;
	string inputPath = workSpace + "/body/" + "input.dat";
	inputPath = tempChecker.checkAndGetPath(QString::fromStdString(inputPath)).toLocal8Bit().toStdString();
	string trDataPath= workSpace + "/body/" + "trData.dat";
	trDataPath = tempChecker.checkAndGetPath(QString::fromStdString(trDataPath)).toLocal8Bit().toStdString();
	/*replaceSectionData(inputPath, trDataPath, inputPath);*/
	std::ifstream inHeadLoad;
	inHeadLoad.open(inputPath);
	if (!inHeadLoad.is_open()) return;
	std::vector<std::string> lines;
	std::string line;
	while (std::getline(inHeadLoad, line)) {
		lines.push_back(line);
	}
	inHeadLoad.close();

	vector<string> trInfos;
	ifstream trLoad;
	trLoad.open(trDataPath);
	while (std::getline(trLoad, line)) {
		if (line == "#Section Xstep0 xstepmin min1  max kstep  ncstep") {
			do {
				if (line == "") break;
				trInfos.push_back(line);
			} while (std::getline(trLoad, line));

		}

	}
	trLoad.close();

	FILE* output = fopen(inputPath.c_str(), "w");
	if (output == NULL) return;
	int p = 0;
	while (p < lines.size()) {
		string str = lines[p];
		if (str == "[StepCtrl]") {
			fprintf(output, "%s\n", str.c_str());
			p++;

			int count = trInfos.size();
			for (auto it : trInfos) {
				fprintf(output, "%s\n", it.c_str());
			}
			while (p < lines.size() && lines[p] != "") p++;
		}
		else {
			fprintf(output, "%s\n", str.c_str());
			p++;
		}

	}
	fclose(output);
}
