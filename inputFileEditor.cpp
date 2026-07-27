#include "inputFileEditor.h"

inputFileEditor::inputFileEditor()
{
}

void inputFileEditor::changeConfig(QString configFile, QString inpDir, QString grdDir, int flag)
{
	filePathComputer pathComputer;
	if (flag == 0) {
		qDebug() << u8"ÐÞ¸Äheadconfig.dat";
		qDebug() << "configFile:" << configFile
			<< "inpDir:" << inpDir
			<< "grdDir:" << grdDir
			<< "flag:" << flag;
		pathComputer.changeHeadconfigPath(configFile, grdDir, inpDir);
	}
	else {
		qDebug() << u8"ÐÞ¸Äinput.dat";
		qDebug() << "configFile:" << configFile
			<< "inpDir:" << inpDir
			<< "grdDir:" << grdDir
			<< "flag:" << flag;
		pathComputer.changeInputPath(configFile, grdDir, inpDir);
	}
	qDebug() << "inputFileFinish";
	return;
}