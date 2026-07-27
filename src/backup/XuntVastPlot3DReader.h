//---------------------------------------------------------------------------

#ifndef untVastPlot3DReaderH
#define untVastPlot3DReaderH
#include <vcl.h>
#pragma hdrstop

#include <system.hpp>
#include <windows.h>
#include "gl.h"
#include "glu.h"
#include "glut.h"
#include "stdio.h"

//---------------------------------------------------------------------------
const int  NCOLOR=1024;
const int  FenQuColor[][3] = {(200, 0, 0), //DarkRed
    (200, 0, 200), //DarkMagenta
    (0, 0, 200), //DarkBlue
    (255, 0, 244), //LightGreen
    (205, 149, 12), //DarkGoldenrod3
    (0, 250, 255), //Green4
    (146, 139, 187), //SeaGreen4
    (184, 255, 159), //SeaGreen1
    (230, 139, 180), //DarkSeaGreen4
    (0, 255, 255), //Cyan1
    (255, 228, 225), //MistyRose1
    (255, 0, 255), //Magenta
    (255, 0, 0), //Red
    (255, 165, 0), //Orange
    (165, 142, 142), //Brown
    (227, 242, 135), //OliveDrab
    (234, 139, 234), //ForestGreen
    (224, 252, 128), //LawnGreen
    (153, 250, 204), //DarkOrchid
    (218, 165, 232), //goldenrod
    (245, 245, 245), //WhiteSmoke
    (169, 239, 0), //Chartreuse4
    (255, 218, 185), //PeachPuff
    (127, 255, 0), //Chartreuse1
    (247, 179, 179), //DarkSlateGray
    (112, 128, 144), //SlateGrey
    (172, 161, 139), //DarkSlateBlue
    (0, 191, 255), //DeepSkyBlue

    (247, 101, 4), //黄色
    (89, 37, 2), //黄色
    (4, 79, 87), //黄色
    (91, 0, 78), //黄色
    (44, 48, 47), //黄色
    (3, 88, 18), //黄色
    (64, 28, 51), //黄色
    (3, 88, 88), //黄色
    (31, 35, 61), //黄色
    (36, 41, 55), //黄色
    (30, 100, 100), //黄色
    (30, 60, 200), //黄色
    (50, 50, 100), //黄色
    (80, 20, 30), //黄色
    (56, 78, 98), //黄色
    (123, 245, 100), //黄色
    (46, 78, 123), //黄色
    (77, 99, 222), //黄色
    (234, 56, 100), //黄色
    (178, 160, 100), //黄色
    (69, 12, 100), //黄色
    (79, 245, 100), //黄色
    (38, 100, 100), //黄色
    (250, 250, 12) //黄色
    };

  struct T3DPoint{
    double X, Y, Z;
  };

  struct T3Color{
    double r, g, b;
  };

  struct TFenQue{
    int IDotNums;
    int JDotNums;
    int KDotNums;
    int MaxDotNum;
    bool IsDraw;
    T3Color fqColor;
    bool bSelected;
    int nIdxFenStart;
    int nIdxFenEnd;
    //for surface slice.
    int nIBegin;
    int nIEnd;
    int nISkip;
    int nJBegin;
    int nJEnd;
    int nJSkip;
    int nKBegin;
    int nKEnd;
    int nKSkip;
    //for i,j,k visible flag
    bool bIDir;
    bool bJDir;
    bool bKDir;
  };

  struct TEdgeEntry{
    int INo;
    bool bDisable;
    int IStart;
    int IEnd;
    int JStart;
    int JEnd;
    int KStart;
    int KEnd;
    int EntryType;
    bool bSelected;
    bool IsDraw;
  };

  struct TEdgeInfo{
    int FenQuNum;
    DynamicArray <TFenQue> FebQuList;
    DynamicArray <DynamicArray <TEdgeEntry> > EdgeEntryListOfFenQue;
  };

  struct TVarInfo{
    String VarName;
    double VarValueMin;
    double VarValueMax;
  };

  struct TRltInfo{
    int FenQuNum;
    int VarCount;
    DynamicArray <TVarInfo> VarnameList;
  };

  struct TVastRGB{
    GLdouble R,G,B;
  };

  struct TVastSeg{
    GLdouble SegStartValueList[10];//: array[0..9] of GLdouble;
    TVastRGB lclr[10];//: array[0..9] of ;
    TVastRGB rclr[10];//: array[0..9] of ;
  };

  class TVastPlot3DReader:TObject{
    String FFileName;
    FILE* FFile;
    DynamicArray <double> Finfo;
    DynamicArray <double> FinfoRlt;
    int FFenQu;
    int FVarNum;
    int FMaxEdgeEntry;
    int FDotNum;
    bool FbInpFound;
  public:
    TVastRGB FColorMapInit[10];//: array[0..9] of TVastRGB;
    TVastRGB FColorMapList[NCOLOR+1];//: array[0..NCOLOR] of TVastRGB;
    TVastSeg FSeg; //颜色分段结构体成员
  public:
    int LoadState;
    bool FbCheckOk;
    TRltInfo FRltInfo;
    DynamicArray <TFenQue> FFenQuAry;//: array of TFenQue;
    DynamicArray <TFenQue> FFenQuAryRlt;//: array of TFenQue;
    DynamicArray <int> nStartPosAry;//: array of integer;
    DynamicArray <int> nStartPosAryRlt;//: array of integer;
    double FXMax, FXMin, FYMax, FYMin, FZMax, FZMin;//: double;
    int FXMaxDot, FXMinDot, FYMaxDot, FYMinDot, FZMaxDot, FZMinDot;//: Integer;
    int FITotalDot, FJTotalDot, FKTotalDot;//: Integer;
    int FITotalDotRlt, FJTotalDotRlt, FKTotalDotRlt;//: Integer;
    TEdgeInfo EdgeInfo;//: TEdgeInfo;
    void renderBitmapCharacher(double x, double y, double z, String content);
  public:
    int curVarIdx;
    bool IsLoaded;
    bool IsRltLoaded;
    bool IsDrawing;
    //bShowEdge:Boolean;
    double nBaseRange;
    int lodLevel;
    TVastPlot3DReader(String AFileName="");
    void Load(String AFileName);
    void AnalyseEdge(String AFileName);
    void AnalyseResult(String AFileName);
    void DrawGrid();
    void DrawContour();
    void DrawBoundary();
    void ComputeNormalVector(bool & Bup,bool& Bdown,bool & Bleft,bool& Bright,bool &Bfront,bool & Bback,int colorPara[]);//: array of integer);
    void DrawBox();
    void DrawBoxNew();
    void DrawColorTable();
    void DrawClip(int I, int J, int K);
    void DrawLod();
    String GetFenQuColor(int FenQuIndex);
    String GetEdgeDescription(int EdgeIndex);
    T3DPoint GetXYZ(int q, int i, int j, int k);
    GLdouble GetXYZIndex(int q, int i, int j, int k, int rltIdx);
    GLdouble GetResultByIndex(int q, int I, int J, int K, int rltIdx); //rltIdx=0,1,2,3....以变量列表为序
    void InitColorMap(double pmin, double pmax);
    int CreateglName(int q, int I, int J, int K);
    bool DecodeglName(int value, int &q, int &I, int &J, int &K);
    void invertMatrix(GLdouble m[],GLdouble o[]);
    void renderBitmapCharacher(double x, double y, double z,void *fontname,String content);
    //procedure WMMsg(var CopyData: TWmCopyData); message WM_COPYDATA;
};

#endif
