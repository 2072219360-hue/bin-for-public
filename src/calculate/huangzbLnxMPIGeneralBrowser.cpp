#include "huangzbTableForInsp.h"
#include "huangzbPlot3Dcommon.h"

#include "huangglobalconfig.h"
#include "huangzbTableForInspEdit.h"

extern HuangzbGlobalConfig globalcfg;

THuangTableForInsp::THuangTableForInsp(QWidget *parent)
    : QWidget(parent)
{  

	FbCheckOk =false;
	FbInpFound =false;
	EdgeInfo.FenQuNum=0;
	EdgeInfo.LineNumsOfEachFenQu.clear();
	EdgeInfo.LineTitles.clear();

    setWindowTitle(tr("边界条件维护"));

	QHBoxLayout *hlayout=new QHBoxLayout();
	
	showButton = new QPushButton(QIcon(":/images/showinsp.png"),tr("显示所选边界面"));
	//hlayout->addWidget(showButton);
	connect(showButton,SIGNAL(clicked()),this,SLOT(OnshowButtonClicked()));

	editButton = new QPushButton(QIcon(":/images/EditInsp.png"),tr("编辑边界属性(&P)")); 
	//hlayout->addWidget(editButton);
	connect(editButton,SIGNAL(clicked()),this,SLOT(OneditButtonClicked()));

	saveButton = new QPushButton(QIcon(":/images/SaveInsp.png"),tr("保存边界文件"));
	connect(saveButton,SIGNAL(clicked()),this,SLOT(OnsaveButtonClicked()));

	buttonBox = new QDialogButtonBox;
	buttonBox->addButton(showButton, QDialogButtonBox::ActionRole); 	
	buttonBox->addButton(editButton, QDialogButtonBox::ActionRole); 	
	buttonBox->addButton(saveButton, QDialogButtonBox::ActionRole); 	
	
	contentQW=new QTableWidget();
	contentQW->setSelectionBehavior(QAbstractItemView::SelectRows); //整行选中的方式
	contentQW->setSelectionMode(QAbstractItemView::ExtendedSelection); //设置为可以选中多个目标
	contentQW->resizeColumnsToContents(); 
	contentQW->resizeRowsToContents(); 
	contentQW->setEditTriggers(QAbstractItemView::NoEditTriggers);

	connect(contentQW,SIGNAL(itemClicked(QTableWidgetItem*)),this, SLOT(UpdateInspItem(QTableWidgetItem*)) );
	connect(contentQW,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this, SLOT(UpdateInspItemGL(QTableWidgetItem*)) );



	mainLayout = new QVBoxLayout; 
	mainLayout->addWidget(contentQW);
	mainLayout->addWidget(buttonBox);
	//mainLayout->addLayout(hlayout);
	setLayout(mainLayout);
/*
	setColumnCount (5);
	setRowCount (5);
	
	QTableWidgetItem *TableWidgetItemSex = new QTableWidgetItem(tr("Sex"));
	setItem(0, 0, TableWidgetItemSex);
	QTableWidgetItem *TableWidgetItemName = new QTableWidgetItem(tr("Name"));
	setItem(0, 1, TableWidgetItemName);
	QTableWidgetItem *TableWidgetItemBirth = new QTableWidgetItem(tr("Birth"));
	setItem(0, 2, TableWidgetItemBirth);
	QTableWidgetItem *TableWidgetItemWork = new QTableWidgetItem(tr("Work"));
	setItem(0, 3, TableWidgetItemWork);	
	QTableWidgetItem *TableWidgetItemIncome = new QTableWidgetItem(tr("Income"));
	setItem(0, 4, TableWidgetItemIncome);		

	QLabel *LabelPixmap1 = new QLabel();
	LabelPixmap1->setPixmap(QPixmap(":/images/Male.png"));
	setCellWidget (1,0,LabelPixmap1);
	QTableWidgetItem *TableWidgetItem1 = new QTableWidgetItem(tr("Tom"));
	setItem(1, 1, TableWidgetItem1);	
	QDateTimeEdit *DateTimeEdit1= new QDateTimeEdit();
	DateTimeEdit1->setDateTime(QDateTime::currentDateTime());
	DateTimeEdit1->setDisplayFormat("dd/M/yyyy");
	DateTimeEdit1->setCalendarPopup(true);
	setCellWidget (1,2,DateTimeEdit1);		
	QComboBox *ComboBoxWork1 = new QComboBox();
	ComboBoxWork1->addItem(tr("Worker")); 
	ComboBoxWork1->addItem(tr("Farmer")); 
	ComboBoxWork1->addItem(tr("Doctor")); 
	ComboBoxWork1->addItem(tr("Lawyer")); 
	ComboBoxWork1->addItem(tr("Soldier")); 
	setCellWidget(1,3,ComboBoxWork1);
	QSpinBox *SpiBoxIncome1 = new QSpinBox();
	SpiBoxIncome1->setRange (1000,10000); 
	setCellWidget(1,4,SpiBoxIncome1);
	
	QLabel *LabelPixmap2 = new QLabel();
	LabelPixmap2->setPixmap(QPixmap(":/images/Male.png"));
	setCellWidget (2,0,LabelPixmap2);
	QTableWidgetItem *TableWidgetItem2 = new QTableWidgetItem(tr("Jack"));
	setItem(2, 1, TableWidgetItem2);	
	QDateTimeEdit *DateTimeEdit2= new QDateTimeEdit();
	DateTimeEdit2->setDateTime(QDateTime::currentDateTime());
	DateTimeEdit2->setDisplayFormat("dd/M/yyyy");
	DateTimeEdit2->setCalendarPopup(true);
	setCellWidget (2,2,DateTimeEdit2);		
	QComboBox *ComboBoxWork2 = new QComboBox();
	ComboBoxWork2->addItem(tr("Worker")); 
	ComboBoxWork2->addItem(tr("Farmer")); 
	ComboBoxWork2->addItem(tr("Doctor")); 
	ComboBoxWork2->addItem(tr("Lawyer")); 
	ComboBoxWork2->addItem(tr("Soldier")); 
	setCellWidget(2,3,ComboBoxWork2);
	QSpinBox *SpiBoxIncome2 = new QSpinBox();
	SpiBoxIncome2->setRange (1000,10000); 
	setCellWidget(2,4,SpiBoxIncome2);

	QLabel *LabelPixmap3 = new QLabel();
	LabelPixmap3->setPixmap(QPixmap(":/images/Female.png"));
	setCellWidget (3,0,LabelPixmap3);
	QTableWidgetItem *TableWidgetItem3 = new QTableWidgetItem(tr("Alice"));
	setItem(3, 1, TableWidgetItem3);	
	QDateTimeEdit *DateTimeEdit3= new QDateTimeEdit();
	DateTimeEdit3->setDateTime(QDateTime::currentDateTime());
	DateTimeEdit3->setDisplayFormat("dd/M/yyyy");
	DateTimeEdit3->setCalendarPopup(true);
	setCellWidget (3,2,DateTimeEdit3);		
	QComboBox *ComboBoxWork3 = new QComboBox();
	ComboBoxWork3->addItem(tr("Worker")); 
	ComboBoxWork3->addItem(tr("Farmer")); 
	ComboBoxWork3->addItem(tr("Doctor")); 
	ComboBoxWork3->addItem(tr("Lawyer")); 
	ComboBoxWork3->addItem(tr("Soldier")); 
	setCellWidget(3,3,ComboBoxWork3);
	QSpinBox *SpiBoxIncome3 = new QSpinBox();
	SpiBoxIncome3->setRange (1000,10000); 
	setCellWidget(3,4,SpiBoxIncome3);
	
	QLabel *LabelPixmap4 = new QLabel();
	LabelPixmap4->setPixmap(QPixmap(":/images/Male.png"));
	setCellWidget (4,0,LabelPixmap4);
	QTableWidgetItem *TableWidgetItem4 = new QTableWidgetItem(tr("John"));
	setItem(4, 1, TableWidgetItem4);		
	QDateTimeEdit *DateTimeEdit4= new QDateTimeEdit();
	DateTimeEdit4->setDateTime(QDateTime::currentDateTime());
	DateTimeEdit4->setDisplayFormat("dd/M/yyyy");
	DateTimeEdit4->setCalendarPopup(true);
	setCellWidget (4,2,DateTimeEdit4);		
	QComboBox *ComboBoxWork4 = new QComboBox();
	ComboBoxWork4->addItem(tr("Worker")); 
	ComboBoxWork4->addItem(tr("Farmer")); 
	ComboBoxWork4->addItem(tr("Doctor")); 
	ComboBoxWork4->addItem(tr("Lawyer")); 
	ComboBoxWork4->addItem(tr("Soldier")); 
	setCellWidget(4,3,ComboBoxWork4);
	QSpinBox *SpiBoxIncome4 = new QSpinBox();
	SpiBoxIncome4->setRange (1000,10000); 
	setCellWidget(4,4,SpiBoxIncome4);	
*/
}

THuangTableForInsp::~THuangTableForInsp()
{

}

void  THuangTableForInsp::AnalyseEdgeDetail(QString AFileName)
{

  int Ino;
  QString tmpStr;
  int i, j, k;
  double tmp;
  //int nFenStart, nFenEnd, nIdx;
  int nFenQuIdx;
 

  EdgeInfo.FebQuList.clear();
  for (i=0;i<EdgeInfo.EdgeEntryListOfFenQue.size();i++)
  {
	  EdgeInfo.EdgeEntryListOfFenQue.at(i).clear();
  }
  EdgeInfo.EdgeEntryListOfFenQue.clear();
  EdgeInfo.FenQuNum=0;

  if (!QFile(AFileName).exists()) 
  {
	FbInpFound =false;
	return; 
  }

  QFile flist(AFileName);//: QFile;
  if (!flist.open(QIODevice::ReadOnly))
  {
	FbInpFound = false;
	return;
  }

  QTextStream stream( &flist );
  int nLineNo=0;
  bool ok;
  QString line;
  bool nNewFenQuFlag=false;
  std::vector<TEdgeEntry> *ee=NULL;
  bool bSkip=false;

  while ( !stream.atEnd() ) {
	  line = stream.readLine(); // 不包括“\n”的一行文本
	  //第0行无意义,删除
	  if (nLineNo==0)
	  {
		  EdgeInfo.lineOne=line;
	  }
	  //第1行为分区的数量
	  if (nLineNo==1)
	  {
		EdgeInfo.FenQuNum = line.toInt();
		//nFenStart = 2; nFenEnd = 2;
		//nIdx = 0;
		nFenQuIdx = 0;
	  }
	  //解析出每一个的分区 ,每个部分的第一行是分区的I,J,K点数
	  //第二行开始是分区的边界详细信息记录
	  if (nLineNo>1)
	  {
		  int nLineItemCount=0;

		  line.simplified();
		  line.trimmed();
		  if (line.isEmpty()) continue;

		  QStringList tmpList=line.split(" ",QString::SkipEmptyParts);
		  nLineItemCount=tmpList.size();

		  if (nLineItemCount==3)
		  {
			  TFenQue tmpFenQue;
			  tmpFenQue.IDotNums = tmpList.at(0).toInt();
			  tmpFenQue.JDotNums = tmpList.at(1).toInt();
			  tmpFenQue.KDotNums = tmpList.at(2).toInt();
			  tmpFenQue.nIdxFenStart = nFenQuIdx;

			  EdgeInfo.FebQuList.push_back(tmpFenQue);

			  //if (nFenStart != nFenEnd)
			  //{
			  //	  ;//EdgeInfo.FebQuList[nFenQuIdx - 1].nIdxFenEnd = nFenEnd - 1;
			  // }
			  nFenQuIdx = nFenQuIdx + 1;
			  //nFenStart = nIdx; 
			  //nFenEnd = nIdx;
			  nNewFenQuFlag =true;

			  line = stream.readLine(); // 不包括“\n”的一行文本
			  EdgeInfo.LineTitles.append(line);

			  line = stream.readLine(); // 不包括“\n”的一行文本
			  EdgeInfo.LineNumsOfEachFenQu.append(line);
		  }
		  if (nLineItemCount==7)
		  {			  
			  if (nNewFenQuFlag)
			  {
				  if (ee!=NULL)
				  {
					  EdgeInfo.EdgeEntryListOfFenQue.push_back(*ee);
				  }
				  ee=new std::vector<TEdgeEntry>;
				  ee->clear();
				  nNewFenQuFlag=false;
			  }
			  TEdgeEntry tmpEE;
			  tmpEE.r=255;
			  tmpEE.g=255;
			  tmpEE.b=255;
			  tmpEE.bSkip  = bSkip;
			  if (bSkip) bSkip=false;
			  tmpEE.IStart = tmpList.at(0).toInt();
			  tmpEE.IEnd   = tmpList.at(1).toInt();
			  tmpEE.JStart = tmpList.at(2).toInt();
			  tmpEE.JEnd   = tmpList.at(3).toInt();
			  tmpEE.KStart = tmpList.at(4).toInt();
			  tmpEE.KEnd   = tmpList.at(5).toInt();
			  tmpEE.EntryType = tmpList.at(6).toInt();

			  if (tmpEE.EntryType==-1)
				  bSkip = true;

			  ee->push_back(tmpEE);
		  }

	  }
	  nLineNo++;
  }
  EdgeInfo.EdgeEntryListOfFenQue.push_back(*ee);
  flist.close();
}

QString THuangTableForInsp::fromNotoText(int No)
{
/*
# bcType为边界类型：
#-1='patch'
#0='unspecified'
#1='not defined'
#2='wall'
#3='symetry'
#4='farfield'
#5='inflow'
#6='outflow'
#7='not defined'
#8='sigular axis'
#9='not defined'
#10='user specified'
*/
	if (No==-1) return QString("patch");
	if (No==0)  return QString("unspecified");
	if (No==1)  return QString("notdefined");
	if (No==2)  return QString("wall");

	if (No==3)  return QString("symetry");
	if (No==4)  return QString("farfield");
	if (No==5)  return QString("inflow");
	if (No==6)  return QString("outflow");
	if (No==7)  return QString("notdefined");
	if (No==8)  return QString("sigularaxis");
	if (No==9)  return QString("notdefined");
	if (No==10)  return QString("userspecified");
	return "notdefined";
}
int THuangTableForInsp::fromTextToNo(QString text)
{
	if (text==QString("patch")) return -1;
	if (text==QString("unspecified")) return 0;
	if (text==QString("notdefined")) return 1;
	if (text==QString("wall")) return 2;
	if (text==QString("symetry")) return 3;
	if (text==QString("farfield")) return 4;
	if (text==QString("inflow")) return 5;
	if (text==QString("outflow")) return 6;
	if (text==QString("notdefined")) return 7;
	if (text==QString("sigularaxis")) return 8;
	if (text==QString("notdefined")) return 9;
	if (text==QString("userspecified")) return 10;
	return -9;
}


void THuangTableForInsp::Load(QString filename)
{
	AnalyseEdgeDetail(filename);
	UpdateTableWidget();
}

void THuangTableForInsp::UpdateTableWidget()
{
	contentQW->clear();

	int nItemCount=0;
	nItemCount=EdgeInfo.EdgeEntryListOfFenQue.size();

	nItemCount=0;
	for(int i=0;i<EdgeInfo.EdgeEntryListOfFenQue.size();i++)
	{
		for(int j=0;j<EdgeInfo.EdgeEntryListOfFenQue.at(i).size();j++)
			if (!EdgeInfo.EdgeEntryListOfFenQue.at(i).at(j).bSkip) nItemCount++;
	}
	nItemCount=nItemCount+2;

	contentQW->setColumnCount(8);
	contentQW->setRowCount(nItemCount);

	contentQW->clear();
	QStringList header;  
	header<<"边界类型"<<"颜色"<<"I-起始"<<"I-结束"<<"J-起始"<<"J-结束"<<"K-起始"<<"K-结束";  
	contentQW->setHorizontalHeaderLabels(header);  

	nItemCount=0;
	for(int j=0;j<EdgeInfo.FenQuNum;j++)
	{
		QString tmpstr=QString("分区%1;I=%2,J=%3,K=%4").arg(j+1).arg(EdgeInfo.FebQuList.at(j).IDotNums).arg(EdgeInfo.FebQuList.at(j).JDotNums).arg(EdgeInfo.FebQuList.at(j).KDotNums);

		QTableWidgetItem *tmpTableWidgetItem = new QTableWidgetItem(tmpstr);
		tmpTableWidgetItem->setBackgroundColor(QColor::fromRgb(0x58,0x5e,0xaa,255));
		tmpTableWidgetItem->setTextColor(Qt::white);
		tmpTableWidgetItem->setData(Qt::UserRole,"-1:-1");
		contentQW->setItem(nItemCount, 0, tmpTableWidgetItem);
		contentQW->setSpan(nItemCount,0,1,8);

		nItemCount++;
		for(int i=0;i<EdgeInfo.EdgeEntryListOfFenQue.at(j).size();i++)
		{
			if (EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).bSkip) continue;

			tmpTableWidgetItem = new QTableWidgetItem(fromNotoText(EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).EntryType));
			tmpTableWidgetItem->setData(Qt::UserRole,QString("%1:%2").arg(j).arg(i));
			contentQW->setItem(nItemCount, 0, tmpTableWidgetItem);

			tmpTableWidgetItem = new QTableWidgetItem(QString("Fenqu=%1:Seq=%2").arg(j).arg(i));
			tmpTableWidgetItem->setData(Qt::UserRole,QString("%1:%2").arg(j).arg(i));
			tmpTableWidgetItem->setBackgroundColor(QColor::fromRgb(EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).r,
				EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).g,
				EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).b,255));
			contentQW->setItem(nItemCount, 1, tmpTableWidgetItem);

			tmpTableWidgetItem = new QTableWidgetItem(QString("%1").arg(EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).IStart));
			tmpTableWidgetItem->setData(Qt::UserRole,QString("%1:%2").arg(j).arg(i));
			contentQW->setItem(nItemCount, 2, tmpTableWidgetItem);

			tmpTableWidgetItem = new QTableWidgetItem(QString("%1").arg(EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).IEnd));
			tmpTableWidgetItem->setData(Qt::UserRole,QString("%1:%2").arg(j).arg(i));
			contentQW->setItem(nItemCount, 3, tmpTableWidgetItem);

			tmpTableWidgetItem = new QTableWidgetItem(QString("%1").arg(EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).JStart));
			tmpTableWidgetItem->setData(Qt::UserRole,QString("%1:%2").arg(j).arg(i));
			contentQW->setItem(nItemCount, 4, tmpTableWidgetItem);

			tmpTableWidgetItem = new QTableWidgetItem(QString("%1").arg(EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).JEnd));
			tmpTableWidgetItem->setData(Qt::UserRole,QString("%1:%2").arg(j).arg(i));
			contentQW->setItem(nItemCount, 5, tmpTableWidgetItem);

			tmpTableWidgetItem = new QTableWidgetItem(QString("%1").arg(EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).KStart));
			tmpTableWidgetItem->setData(Qt::UserRole,QString("%1:%2").arg(j).arg(i));
			contentQW->setItem(nItemCount, 6, tmpTableWidgetItem);

			tmpTableWidgetItem = new QTableWidgetItem(QString("%1").arg(EdgeInfo.EdgeEntryListOfFenQue.at(j).at(i).KEnd));
			tmpTableWidgetItem->setData(Qt::UserRole,QString("%1:%2").arg(j).arg(i));
			contentQW->setItem(nItemCount, 7, tmpTableWidgetItem);

			nItemCount++;
		}

	}
}

void THuangTableForInsp::Save(QString filename)
{
	//保存边界文件,依据EdgeInfo.
	QMessageBox::information(this, "INFORMATION", globalcfg.gridInspfilename);
	if ((globalcfg.gridInspfilename=="9999")||(filename=="9999")) return;

	QFile *file = new QFile();
	file->setFileName(filename);  
	file->open(QIODevice::WriteOnly | QIODevice::Text);  
	//char StrLine[1024];
	//sprintf(StrLine,"
	file->write((globalcfg.q2s(EdgeInfo.lineOne)+"\n").c_str());
	file->write((globalcfg.q2s(QString("%1").arg(EdgeInfo.FenQuNum))+"\n").c_str());
	QString tmpstr;

	for(int i=0;i<EdgeInfo.FenQuNum;i++)
	{
		tmpstr=QString("%1\t\t%2\t\t%3").arg(EdgeInfo.FebQuList.at(i).IDotNums).arg(EdgeInfo.FebQuList.at(i).JDotNums).arg(EdgeInfo.FebQuList.at(i).KDotNums);
		file->write((globalcfg.q2s(QString(tmpstr))+"\n").c_str());
		file->write((globalcfg.q2s(EdgeInfo.LineTitles.at(i))+"\n").c_str());
		file->write((globalcfg.q2s(EdgeInfo.LineNumsOfEachFenQu.at(i))+"\n").c_str());
		for(int j=0;j<EdgeInfo.EdgeEntryListOfFenQue.at(i).size();j++)
		{
			tmpstr=QString("%1\t\t%2\t\t%3\t\t%4\t\t%5\t\t%6\t\t%7").arg(EdgeInfo.EdgeEntryListOfFenQue.at(i).at(j).IStart).arg(EdgeInfo.EdgeEntryListOfFenQue.at(i).at(j).IEnd).arg(EdgeInfo.EdgeEntryListOfFenQue.at(i).at(j).JStart).arg(EdgeInfo.EdgeEntryListOfFenQue.at(i).at(j).JEnd).arg(EdgeInfo.EdgeEntryListOfFenQue.at(i).at(j).KStart).arg(EdgeInfo.EdgeEntryListOfFenQue.at(i).at(j).KEnd).arg(EdgeInfo.EdgeEntryListOfFenQue.at(i).at(j).EntryType);
			file->write((globalcfg.q2s(QString(tmpstr))+"\n").c_str());
		}

	}

	file->close();
}

void THuangTableForInsp::OnsaveButtonClicked()
{
	Save(globalcfg.gridInspfilename);
}

void THuangTableForInsp::UpdateInspItem(QTableWidgetItem* item)
{
	QString cont=item->data(Qt::UserRole).toString();
	//QMessageBox::information(this, "INFORMATION", cont);
}

void THuangTableForInsp::UpdateInspItemGL(QTableWidgetItem* item)
{
	QString cont=item->data(Qt::UserRole).toString();
	globalcfg.GlobalListForInsp.clear();
	globalcfg.GlobalListForInsp<<cont;
	//QMessageBox::information(this, "INFORMATION", cont);
	gridshow->updateViewer();

}

void THuangTableForInsp::OnshowButtonClicked()
{
	for(int i=0;i<contentQW->selectedItems().size();i++)
	{
		QTableWidgetItem *item = contentQW->selectedItems().at(i);
		QString str=item->data(Qt::UserRole).toString();
		//QMessageBox::information(this, "INFORMATION", str);
	}

	QString tmpStr="";
	//获得当前的模型类型
	QItemSelectionModel *selectionModel = contentQW->selectionModel();
	//获得选择项的列表，即选择了哪几行
	QModelIndexList selected = selectionModel->selectedIndexes();
	QMap<int, int> rowMap;
	foreach (QModelIndex index, selected)
	{
		//记录已选择行的<键，值>对
		rowMap.insert(index.row(), 0);		
	}

	globalcfg.GlobalListForInsp.clear();
	//globalcfg.inspShowMapByFenQuAndLineNo.clear();

	for(int j=0;j<rowMap.keys().size();j++)
	{
		tmpStr.append(QString("%1").arg(rowMap.keys().at(j)));
		QTableWidgetItem *item = contentQW->item(rowMap.keys().at(j),0);
		QString str=item->data(Qt::UserRole).toString();
		globalcfg.GlobalListForInsp<<str;

		//QStringList FenLineNo=str.split(":");
		//globalcfg.inspShowMapByFenQuAndLineNo.insert(FenLineNo.at(0).toInt(),FenLineNo.at(1).toInt());

		QMessageBox::information(this, "INFORMATION", str);
	}

	gridshow->updateViewer();
}

void THuangTableForInsp::OneditButtonClicked()
{
	int nFeQu=0;
	int nLineNo=0;

	for(int i=0;i<contentQW->selectedItems().size();i++)
	{
		QTableWidgetItem *item = contentQW->selectedItems().at(i);
		QString str=item->data(Qt::UserRole).toString();
		//QMessageBox::information(this, "INFORMATION", str);
	}

	QString tmpStr="";
	//获得当前的模型类型
	QItemSelectionModel *selectionModel = contentQW->selectionModel();
	//获得选择项的列表，即选择了哪几行
	QModelIndexList selected = selectionModel->selectedIndexes();
	QMap<int, int> rowMap;
	foreach (QModelIndex index, selected)
	{
		//记录已选择行的<键，值>对
		rowMap.insert(index.row(), 0);		
	}

	for(int j=0;j<rowMap.keys().size();j++)
	{
		tmpStr.append(QString("%1").arg(rowMap.keys().at(j)));
		QTableWidgetItem *item = contentQW->item(rowMap.keys().at(j),0);
		QString str=item->data(Qt::UserRole).toString();

		QStringList FenLineNo=str.split(":");
		//globalcfg.inspShowMapByFenQuAndLineNo.insert(FenLineNo.at(0).toInt(),FenLineNo.at(1).toInt());
		nFeQu = FenLineNo.at(0).toInt();
		nLineNo =FenLineNo.at(1).toInt();

		if (nFeQu==-1) continue;

		THuangTableForInspEditDialog form(nFeQu,nLineNo,&EdgeInfo, this);
		form.exec();

		//更新显示列表
		UpdateTableWidget();
		break;
	}

}
