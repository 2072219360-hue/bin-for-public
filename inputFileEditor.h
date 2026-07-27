#ifndef INPUTFILEEDITOR_H
#define INPUTFILEEDITOR_H

#include <QString>
#include <QDebug>
#include"filePathComputer.h"
class inputFileEditor
{
public:
	inputFileEditor();
	void changeConfig(QString configFile, QString inpDir, QString grdDir, int flag);
private:
	
};

#endif // INPUTFILEEDITOR_H