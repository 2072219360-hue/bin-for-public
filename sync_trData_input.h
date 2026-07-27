#pragma once
#include<string>
#include<QString>
#include"bodyPathChecker.h"
using namespace std;
class sync_trData_input
{
public:
	sync_trData_input(string _workSpace) {
		workSpace = _workSpace;
	}
	void run();
	bool sync_trData_input::isDataLine(const std::string& line);

	void sync_trData_input::replaceSectionData(const std::string& inputFile,
		const std::string& trDataFile,
		const std::string& outputFile);
private:
	string workSpace;
};

