/****************************************************************************

 Copyright (C) 2002-2011 Zhidaofangyuan . All rights reserved.

*****************************************************************************/

#ifndef HUANGZB_BC_INSP_READER_H_
#define HUANGZB_BC_INSP_READER_H_

#include "QtGui"
#include<vector>
#include <windows.h>
#include "gl/gl.h"
#include "gl/glu.h"
//#include "glut.h"
#include "stdio.h"


#include "log4qt\logger.h"
#include "log4qt\propertyconfigurator.h"
#include "huangzbPlot3Dcommon.h"

class HuangzbBCInspReaderClass:public QWidget
{
	LOG4QT_DECLARE_QCLASS_LOGGER
private:
	QString FFileName;
	FILE* FFile;
	std::vector <double> Finfo;
	std::vector <double> FinfoRlt;
	int FFenQu;
	int FVarNum;
	int FMaxEdgeEntry;
	int FDotNum;
	bool FbInpFound;

public :
	TVastRGB FColorMapInit[10];//: array[0..9] of TVastRGB;
	TVastRGB FColorMapList[NCOLOR+1];//: array[0..NCOLOR] of TVastRGB;
	TVastSeg FSeg; //颜色分段结构体成员

	int LoadState;
	bool FbCheckOk;
	TRltInfo FRltInfo;
	std::vector <TFenQue> FFenQuAry;//: array of TFenQue;
	std::vector <TFenQue> FFenQuAryRlt;//: array of TFenQue;
	std::vector <int> nStartPosAry;//: array of integer;
	std::vector <int> nStartPosAryRlt;//: array of integer;
	double FXMax, FXMin, FYMax, FYMin, FZMax, FZMin;//: double;
	int FXMaxDot, FXMinDot, FYMaxDot, FYMinDot, FZMaxDot, FZMinDot;//: Integer;
	int FITotalDot, FJTotalDot, FKTotalDot;//: Integer;
	int FITotalDotRlt, FJTotalDotRlt, FKTotalDotRlt;//: Integer;
	TEdgeInfo EdgeInfo;//: TEdgeInfo;
	//void renderBitmapCharacher(double x, double y, double z, String content);

	int curVarIdx;
	bool IsLoaded;
	bool IsRltLoaded;
	bool IsDrawing;
	//bShowEdge:Boolean;
	double nBaseRange;
	int lodLevel;
public:
	QProgressDialog *FprogressBar;

	HuangzbPlot3DFormattedReaderClass(QString AFileName="",QWidget *parent=0);
	void Load(QString AFileName,QProgressDialog *progressBar);
	void AnalyseEdge(QString AFileName);
	void AnalyseResult(QString AFileName);
	void DrawGrid();
	void DrawContour();
	void DrawBoundary();
	void ComputeNormalVector(bool & Bup,bool& Bdown,bool & Bleft,bool& Bright,bool &Bfront,bool & Bback,int colorPara[]);//: array of integer);
	void DrawBox();
	void DrawBoxNew();
	void DrawColorTable();
	void DrawClip(int I, int J, int K);
	void DrawLod();
	QString GetFenQuColor(int FenQuIndex);
	QString GetEdgeDescription(int EdgeIndex);
	T3DPoint GetXYZ(int q, int i, int j, int k);
	GLdouble GetXYZIndex(int q, int i, int j, int k, int rltIdx);
	GLdouble GetResultByIndex(int q, int I, int J, int K, int rltIdx); //rltIdx=0,1,2,3....以变量列表为序
	void InitColorMap(double pmin, double pmax);
	int CreateglName(int q, int I, int J, int K);
	bool DecodeglName(int value, int &q, int &I, int &J, int &K);
	void invertMatrix(GLdouble m[],GLdouble o[]);
	//void renderBitmapCharacher(double x, double y, double z,void *fontname,String content);
private :

};


#endif
