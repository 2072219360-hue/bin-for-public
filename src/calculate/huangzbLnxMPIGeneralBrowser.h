#ifndef HUANGZB_TABLE_FOR_INSP_H
#define HUANGZB_TABLE_FOR_INSP_H

#include <QtGui>
#include "huangzbPlot3Dcommon.h"
#include "gridpreprocess/gridshowQGLViewer.h"

struct TEdgeDetailInfo{
	QString lineOne;
	int FenQuNum;
	QStringList LineTitles;
	QStringList LineNumsOfEachFenQu;
	std::vector<TFenQue> FebQuList;
	std::vector<std::vector<TEdgeEntry>> EdgeEntryListOfFenQue;
};

class THuangTableForInsp : public QWidget
{
    Q_OBJECT

private:
	bool FbCheckOk;
	bool FbInpFound;
	TEdgeDetailInfo EdgeInfo;//: TEdgeInfo;
	QTableWidget *contentQW;
	QVBoxLayout *mainLayout;

    QDialogButtonBox *buttonBox;
	QPushButton *editButton; 
	QPushButton *showButton; 
	QPushButton *saveButton;

public:
    THuangTableForInsp(QWidget *parent=0);
    ~THuangTableForInsp();

	void AnalyseEdgeDetail(QString AFileName);
	void Load(QString filename);
	void Save(QString filename);

	QString fromNotoText(int No);
	int fromTextToNo(QString text);

	gridshowQGLViewer *gridshow;
	
private slots:
	void UpdateInspItem(QTableWidgetItem* item);
	void UpdateInspItemGL(QTableWidgetItem* item);

	void UpdateTableWidget();

	void OneditButtonClicked();
	void OnshowButtonClicked();
	void OnsaveButtonClicked();
};


#endif // HUANGZB_TABLE_FOR_INSP_H
