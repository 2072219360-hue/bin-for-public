/****************************************************************************

 Copyright (C) 2002-2011 Zhidaofangyuan . All rights reserved.

*****************************************************************************/
#include "QtGui"
#include "huangzbPlot3DFormattedReader.h"
#include "huangzbPlot3Dcommon.h"

#include <qimage.h>
#include <qfiledialog.h>

#include "huangglobalconfig.h"
#include "huangzbGraphUtils.h"

extern HuangzbGlobalConfig globalcfg;

#if QT_VERSION >= 0x040000
# include <QKeyEvent>
#endif

using namespace std;

int HuangzbPlot3DFormattedReaderClass::CreateglName(int q, int I, int J, int K)
{
	return K + J * 1000 + I * 1000000 + q * 1000000000;
}

bool HuangzbPlot3DFormattedReaderClass::DecodeglName(int value, int &q, int &I, int &J, int &K)
{
  K = value % 1000;
  value = value / 1000;
  J = value % 1000;
  value = value / 1000;
  I = value % 1000;
  value = value / 1000;
  q = value % 1000;
  return true;
}

T3DPoint HuangzbPlot3DFormattedReaderClass::GetXYZ(int q, int i, int j, int k)
{
  T3DPoint Result;
  Result.X = Finfo[nStartPosAry[q] + i + j * FFenQuAry[q].IDotNums + k * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums];
  Result.Y = Finfo[nStartPosAry[q] + i + j * FFenQuAry[q].IDotNums + k * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums + FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums * FFenQuAry[q].KDotNums];
  Result.Z = Finfo[nStartPosAry[q] + i + j * FFenQuAry[q].IDotNums + k * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums + 2 * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums * FFenQuAry[q].KDotNums];
  return Result;
}

GLdouble HuangzbPlot3DFormattedReaderClass::GetXYZIndex(int q, int i, int j, int k, int rltIdx)
{
  GLdouble Result = 0;
  if (rltIdx == 0)
    Result = Finfo[nStartPosAry[q] + i + j * FFenQuAry[q].IDotNums + k * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums];
  if (rltIdx == 1)
    Result = Finfo[nStartPosAry[q] + i + j * FFenQuAry[q].IDotNums + k * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums + FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums * FFenQuAry[q].KDotNums];
  if (rltIdx == 2) 
    Result = Finfo[nStartPosAry[q] + i + j * FFenQuAry[q].IDotNums + k * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums + 2 * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums * FFenQuAry[q].KDotNums];
  return Result;
}

GLdouble HuangzbPlot3DFormattedReaderClass::GetResultByIndex(int q, int i, int j, int k, int rltIdx)
{
  rltIdx = rltIdx - 3;
  return FinfoRlt[nStartPosAryRlt[q] + i + j * FFenQuAryRlt[q].IDotNums + k * FFenQuAryRlt[q].IDotNums * FFenQuAryRlt[q].JDotNums + rltIdx * FFenQuAryRlt[q].IDotNums * FFenQuAryRlt[q].JDotNums * FFenQuAryRlt[q].KDotNums];
}

QString HuangzbPlot3DFormattedReaderClass::GetEdgeDescription(int EdgeIndex)
{
  QString Result = "";
  if (EdgeIndex == -1) Result = "[内边界]";
  if ((EdgeIndex == 73) || (EdgeIndex = 72) || (EdgeIndex = 71)) Result = "[极轴]";
  if (EdgeIndex == 2)  Result = "[固壁]";
  if (EdgeIndex == 3)  Result = "[对称面]";
  if (EdgeIndex == 4)  Result = "[远场]";
  if (EdgeIndex == 5)  Result = "[入流]";
  if (EdgeIndex == 6)  Result = "[出流-外插]";
  if (EdgeIndex == 8)  Result = "[黎曼边界]";
  if (EdgeIndex == 9)  Result = "[出流-常数]";
  return Result;
}

QString HuangzbPlot3DFormattedReaderClass::GetFenQuColor(int FenQuIndex)
{
  QString Result = "unKnown";
  switch (FenQuIndex % 27){
    case 0: Result = "DarkRed";break;
    case 1: Result = "DarkMagenta";break;
    case 2: Result = "DarkBlue";break;
    case 3: Result = "LightGreen";break;
    case 4: Result = "DarkGoldenrod3";break;
    case 5: Result = "Green4";break;
    case 6: Result = "SeaGreen4";break;
    case 7: Result = "SeaGreen1";break;
    case 8: Result = "DarkSeaGreen4";break;
    case 9: Result = "Cyan1";break;
    case 10: Result = "MistyRose1";break;
    case 11: Result = "Magenta";break;
    case 12: Result = "Red"; break;
    case 13: Result = "Orange"; break;
    case 14: Result = "Brown";break;
    case 15: Result = "OliveDrab";break;
    case 16: Result = "ForestGreen";break;
    case 17: Result = "LawnGreen"; break;
    case 18: Result = "DarkOrchid";break;
    case 19: Result = "goldenrod"; break;
    case 20: Result = "WhiteSmoke"; break;
    case 21: Result = "Chartreuse4";break;
    case 22: Result = "Chartreuse1"; break;
    case 23: Result = "DarkSlateGray"; break;
    case 24: Result = "SlateGrey";break;
    case 25: Result = "DarkSlateBlue"; break;
    case 26: Result = "DeepSkyBlue";break;
  }
  return Result;
}

void HuangzbPlot3DFormattedReaderClass::InitColorMap(double pmin, double pmax)
{
  int i, j;
  double x, x0, x1;
  int j0, j1;
  double r0, r1, g0, g1, b0, b1;

  FColorMapInit[0].R = 0;
  FColorMapInit[0].G = 0;
  FColorMapInit[0].B = 0.5;

  FColorMapInit[1].R = 0;
  FColorMapInit[1].G = 0;
  FColorMapInit[1].B = 1.0;

  FColorMapInit[2].R = 7.0 / 255.0;
  FColorMapInit[2].G = 176.0 / 255.0;
  FColorMapInit[2].B = 1.0;

  FColorMapInit[3].R = 8.0 / 255.0;
  FColorMapInit[3].G = 1.0;
  FColorMapInit[3].B = 1.0;

  FColorMapInit[4].R = 0;
  FColorMapInit[4].G = 1.0;
  FColorMapInit[4].B = 179.0 / 255.0;

  FColorMapInit[5].R = 7.0 / 255.0;
  FColorMapInit[5].G = 1.0;
  FColorMapInit[5].B = 0.0;

  FColorMapInit[6].R = 176.0 / 255.0;
  FColorMapInit[6].G = 1.0;
  FColorMapInit[6].B = 8.0 / 255.0;

  FColorMapInit[7].R = 1.0;
  FColorMapInit[7].G = 1.0;
  FColorMapInit[7].B = 0.0;

  FColorMapInit[8].R = 1.0;
  FColorMapInit[8].G = 178.0 / 255.0;
  FColorMapInit[8].B = 0.0;

  FColorMapInit[9].R = 1.0;
  FColorMapInit[9].G = 0;
  FColorMapInit[9].B = 0.0;

  FSeg.SegStartValueList[0] = pmin;
  FSeg.SegStartValueList[9] = pmax;
  for(i=0;i<=8;i++)// i := 0 to 8 do
  {
    FSeg.lclr[i + 1] = FColorMapInit[i + 1];
    FSeg.rclr[i] = FColorMapInit[i];
    if ((i > 0) || (i < 8))
      FSeg.SegStartValueList[i] = pmin + i * (pmax - pmin) / 10;
  }
  for(i=0;i<=8;i++)// i := 0 to 8 do
  {
    x0 = FSeg.SegStartValueList[i];
    x1 = FSeg.SegStartValueList[i + 1];
    j0 = floor(((FSeg.SegStartValueList[i] - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5));
    j1 = floor(((FSeg.SegStartValueList[i + 1] - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5));
    if ((j1 < 0) || (j0 > NCOLOR)) return;
    if (j0 < 0)  j0 = 0;
    if (j1 > NCOLOR)  j1 = NCOLOR;
    for (j=j0;j<=j1;j++)//j := j0 to j1 do
    {
      x = j / NCOLOR * (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) + FSeg.SegStartValueList[0];
      r0 = FSeg.rclr[i].R;
      r1 = FSeg.lclr[i + 1].R;
      g0 = FSeg.rclr[i].G;
      g1 = FSeg.lclr[i + 1].G;
      b0 = FSeg.rclr[i].B;
      b1 = FSeg.lclr[i + 1].B;

      if (x < x0) 
      {
        FColorMapList[j].R = r0;
        FColorMapList[j].G = g0;
        FColorMapList[j].B = b0;
      }
      else if (x > x1)
      {
        FColorMapList[j].R = r1;
        FColorMapList[j].G = g1;
        FColorMapList[j].B = b1;
      }
      else
      {
        FColorMapList[j].R = (x - x0) / (x1 - x0) * (r1 - r0) + r0;
        FColorMapList[j].G = (x - x0) / (x1 - x0) * (g1 - g0) + g0;
        FColorMapList[j].B = (x - x0) / (x1 - x0) * (b1 - b0) + b0;
      }
    }
  }
}
/*
void HuangzbPlot3DFormattedReaderClass::renderBitmapCharacher(double x, double y, double z, String content)
{
  String str;
  int i;
  int ch;
  void *fontname;

  glRasterPos3f(x, y, z);
  fontname = GLUT_BITMAP_TIMES_ROMAN_10;
  for(i=1;i<=content.Length();i++)// i := 1 to Length(content) do
  {
    ch = content[i];
    glutBitmapCharacter(fontname, ch);
  }
}
*/

HuangzbPlot3DFormattedReaderClass::HuangzbPlot3DFormattedReaderClass(QString AFileName,QWidget *parent)
{
  //int i, j;

  LoadState=0;
  FFileName = AFileName;
  IsLoaded = false;
  IsRltLoaded = false;
  IsDrawing = false;
  FXMax = -99999;
  FYMax = -99999;
  FZMax = -99999;
  FXMin = 99999;
  FYMin = 99999;
  FZMin = 99999;
  FITotalDot = 0;
  FJTotalDot = 0;
  FKTotalDot = 0;
  lodLevel = 8;
  FbCheckOk = false;
  FMaxEdgeEntry = 0;
  curVarIdx = 0;
	
  FinfoRlt.clear();
  FFenQuAryRlt.clear();

  //FinfoRlt.Length = 0;
  //FFenQuAryRlt.Length = 0;
}

/*
void  TVastPlot3DReader::AnalyseEdge(String AFileName)
var
  INo: Integer;
  tmpStr: string;
  i, j, k: Integer;
  flist: TstringList;
  sectlist: TstringList;
  tmp: double;
  nFenStart, nFenEnd, nIdx, nFenQuIdx: Integer;
  function SplitStringCount(src: string; sp: Char): Integer;
  var
    tmpList: TStringList;
  begin
    tmpList := TStringList.Create;
    tmpList.Delimiter := sp;
    tmpList.DelimitedText := src;
    Result := tmpList.Count;
    tmpList.Free;
  end;
begin
  FbCheckOk := False;
  FbInpFound := True;
  flist := TStringList.Create;
  sectlist := TStringList.Create;
  if (not FileExists(AFileName)) then begin FbInpFound := False; Exit; end;
  flist.LoadFromFile(AFileName);
  //第0行无意义,删除
  //第1行为分区的数量
  EdgeInfo.FenQuNum := StrToInt(flist.Strings[1]); //获取分区的数量
  SetLength(EdgeInfo.FebQuList, EdgeInfo.FenQuNum);
  SetLength(EdgeInfo.EdgeEntryListOfFenQue, EdgeInfo.FenQuNum);

  //第2行开始是分区的信息
  nFenStart := 2; nFenEnd := 2;
  nIdx := 2;
  nFenQuIdx := 0;
  //解析出每一个的分区 ,每个部分的第一行是分区的I,J,K点数
  //第二行开始是分区的边界详细信息记录
  while (flist.Count > 0) do
  begin
    if (SplitStringCount(flist.Strings[nIdx], ' ') = 3) then
    begin
      sectlist.Clear;
      sectlist.Delimiter := ' ';
      sectlist.DelimitedText := flist.Strings[nIdx];
      EdgeInfo.FebQuList[nFenQuIdx].IdotNums := StrToInt(sectlist.Strings[0]);
      EdgeInfo.FebQuList[nFenQuIdx].JDotNums := StrtoInt(sectlist.Strings[1]);
      EdgeInfo.FebQuList[nFenQuIdx].KDotNums := StrToInt(sectlist.Strings[2]);
      EdgeInfo.FebQuList[nFenQuIdx].nIdxFenStart := nIdx;
      if (nFenStart <> nFenEnd) then
      begin
        EdgeInfo.FebQuList[nFenQuIdx - 1].nIdxFenEnd := nFenEnd - 1;
      end;
      nFenQuIdx := nFenQuIdx + 1;
      nFenStart := nIdx; nFenEnd := nIdx;
    end;

//    if (SplitStringCount(flist.Strings[nIdx],' ')=7) then
//    begin
    nFenEnd := nFenEnd + 1;
//    end;
    nIdx := nIdx + 1;
    if (nIdx > flist.Count - 1) then begin nFenEnd := nIdx - 1; break; end;
  end;
  EdgeInfo.FebQuList[nFenQuIdx - 1].nIdxFenEnd := nFenEnd;
  //输出调试信息
  for i := 0 to EdgeInfo.FenQuNum - 1 do
  begin
    applog.Debug(' ');
    applog.Debug('分区：' + IntToStr(i + 1) + '  I=' + IntToStr(EdgeInfo.FebQuList[i].IdotNums));
    applog.Debug('分区：' + IntToStr(i + 1) + '  J=' + IntToStr(EdgeInfo.FebQuList[i].JdotNums));
    applog.Debug('分区：' + IntToStr(i + 1) + '  K=' + IntToStr(EdgeInfo.FebQuList[i].KdotNums));
    applog.Debug('分区：' + IntToStr(i + 1) + '  FenStartIndex=' + IntToStr(EdgeInfo.FebQuList[i].nIdxFenStart + 1));
    applog.Debug('分区：' + IntToStr(i + 1) + '  FenEndIndex=' + IntToStr(EdgeInfo.FebQuList[i].nIdxFenEnd + 1));
  end;
  //把每个分区的开始和结束点分析出来以后，接着分析每个节点的记录信息
  INo := 0;
  for i := 0 to EdgeInfo.FenQuNum - 1 do
  begin
    SetLength(EdgeInfo.EdgeEntryListOfFenQue[i], EdgeInfo.FebQuList[i].nIdxFenEnd - EdgeInfo.FebQuList[i].nIdxFenStart);
    for j := EdgeInfo.FebQuList[i].nIdxFenStart + 3 to EdgeInfo.FebQuList[i].nIdxFenEnd do
    begin
      sectlist.Clear;
      sectlist.Delimiter := ' ';
      sectlist.DelimitedText := flist.Strings[j];
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].INo := INo;
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].IStart := StrToInt(sectlist.Strings[0]);
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].IEnd := StrToInt(sectlist.Strings[1]);
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].JStart := StrToInt(sectlist.Strings[2]);
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].JEnd := StrToInt(sectlist.Strings[3]);
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].KStart := StrToInt(sectlist.Strings[4]);
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].KEnd := StrToInt(sectlist.Strings[5]);
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].EntryType := StrToInt(sectlist.Strings[6]);
      EdgeInfo.EdgeEntryListOfFenQue[i][j - EdgeInfo.FebQuList[i].nIdxFenStart - 3].bDisable := False;
      INC(INo);
    end;
  end;
  FMaxEdgeEntry := INo;
  //测试一下所有的信息点
  for i := Low(EdgeInfo.EdgeEntryListOfFenQue) to High(EdgeInfo.EdgeEntryListOfFenQue) do
  begin
    for j := Low(EdgeInfo.EdgeEntryListOfFenQue[i]) to High(EdgeInfo.EdgeEntryListOfFenQue[i]) do
    begin
      tmpStr := IntToStr(EdgeInfo.EdgeEntryListOfFenQue[i][j].IStart) + '  ' + IntToStr(EdgeInfo.EdgeEntryListOfFenQue[i][j].IEND);
      tmpStr := tmpStr + '  ' + IntToStr(EdgeInfo.EdgeEntryListOfFenQue[i][j].JStart) + '  ' + IntToStr(EdgeInfo.EdgeEntryListOfFenQue[i][j].JEND);
      tmpStr := tmpStr + '  ' + IntToStr(EdgeInfo.EdgeEntryListOfFenQue[i][j].KStart) + '  ' + IntToStr(EdgeInfo.EdgeEntryListOfFenQue[i][j].KEND);
      tmpStr := tmpStr + '  类别：' + IntToStr(EdgeInfo.EdgeEntryListOfFenQue[i][j].EntryType);
      applog.Debug(tmpStr);
    end;
  end;
  applog.Debug('AnalyseEdge OVER!');
end;
*/

/*
procedure TVastPlot3DReader.AnalyseResult(AFileName: string);
var
  tmp: double;
  tmpChar: string;
  tmpMax: Integer;
  fenquNums: Integer;
  tmpList: TStringList;
  i, j, k, q, v: Integer;
  nCnt: Integer;
  //nTotalStartPoint: Integer;
  //nStartPoint: Integer;
  tPos: Integer;
  tst: array of array of array of T3DPoint;
  tmpValueMin, tmpValueMax: double;
begin
  while IsDrawing do SendMsgData(cparam.frmMainHandle, DateTimeToStr(Now()) + '等待中...');
  if IsRltLoaded then
  begin
      //初始化和清理一下。
    //FinfoRlt := nil;
    //SetLength(FInfoRlt,0);
    //FXYZ := nil;
    FFenQuAryRlt := nil;
    IsRltLoaded := False;
  end;
  SendMsgData(cparam.frmMainHandle, '正在解析文件:' + AFilename);
  if not FileExists(AFilename) then begin IsRltLoaded := False; Exit; end;
  try
    AssignFile(FFile, AFilename);
    Reset(FFile);
    //读取变量的个数
    Readln(FFile, FVarNum);
    //读取变量名字列表
    SetLength(FRltInfo.VarnameList, FVarNum);
    //for i:= 0 to FVarNum-1 do
    //begin
    Readln(FFile, tmpChar);
    //end;
    tmpList := TStringList.Create;
    tmpList.DelimitedText := tmpChar;
    tmpList.Delimiter := ',';
    FRltInfo.VarCount := FVarNum;
    for i := 0 to FVarNum - 1 do
    begin
      FRltInfo.VarnameList[i].VarName := tmpList.Strings[i];
    end;
    tmpList.Free;
    //读取分区的数量
    Read(FFile, FRltInfo.FenQuNum);
    SendMsgData(cparam.frmMainHandle, '读取分区的数量:' + IntToStr(FFenQu));
    applog.Debug('分区数是:' + IntToStr(FRltInfo.FenQuNum));
    SetLength(FFenQuAryRlt, FRltInfo.FenQuNum + 1);
    FDotNum := 0;
    //FMaxPointNum := 0;
    for i := 0 to FRltInfo.FenQuNum - 1 do
    begin
      FFenQuAryRlt[i].IsDraw := True;
      Read(FFile, FFenQuAryRlt[i].IdotNums);
      tmpMax := FFenQuAryRlt[i].IdotNums;

      Read(FFile, FFenQuAryRlt[i].JDotNums);
      if (tmpMax < FFenQuAryRlt[i].JDotNums) then tmpMax := FFenQuAryRlt[i].JDotNums;

      Read(FFile, FFenQuAryRlt[i].KDotNums);
      if (tmpMax < FFenQuAryRlt[i].KDotNums) then tmpMax := FFenQuAryRlt[i].KDotNums;

      FFenQuAryRlt[i].MaxDotNum := tmpMax;
      //FMaxPointNum := FMaxPointNum + tmpMax; //最大点数
      FDotNum := FDotNum + FFenQuAryRlt[i].IdotNums * FFenQuAryRlt[i].JDotNums * FFenQuAryRlt[i].KDotNums;
      applog.Debug('分区：' + IntToStr(i) + ':的点数是：' + IntToStr(FFenQuAryRlt[i].IdotNums * FFenQuAryRlt[i].JDotNums * FFenQuAryRlt[i].KDotNums));
      FITotalDotRlt := FITotalDotRlt + FFenQuAryRlt[i].IDotNums;
      FJTotalDotRlt := FJTotalDotRlt + FFenQuAryRlt[i].JDotNums;
      FKTotalDotRlt := FKTotalDotRlt + FFenQuAryRlt[i].KDotNums;
      applog.Debug('分区：' + IntToStr(i) + ':的I方向点数是：' + IntToStr(FFenQuAryRlt[i].IdotNums));
      applog.Debug('分区：' + IntToStr(i) + ':的J方向点数是：' + IntToStr(FFenQuAryRlt[i].JdotNums));
      applog.Debug('分区：' + IntToStr(i) + ':的K方向点数是：' + IntToStr(FFenQuAryRlt[i].KdotNums));
      SendMsgData(cparam.frmMainHandle, '正在解析分区:' + IntToStr(i) + ' I方向点数:' + IntToStr(FFenQuAryRlt[i].IdotNums) + ' J方向点数:' + IntToStr(FFenQuAryRlt[i].JdotNums) + ' K方向点数:' + IntToStr(FFenQuAryRlt[i].KdotNums));
    end;

    //设置点数组的大小
    SetLength(tst, 50, 50, 50);
    if (FInfoRlt=nil) then
    SetLength(FinfoRlt, FDotNum * FVarNum);

    //读取坐标点
    nCnt := 0;
    while (not EOF(FFile)) do
    begin
      Read(FFile, tmp);
      FinfoRlt[nCnt] := tmp;
      INC(nCnt);
      //applog.Debug(FloatToStr(tmp));
    end;
    CloseFile(FFile);

    SetLength(nStartPosAryRlt, FRltInfo.FenQuNum + 1);
    nStartPosAryRlt[0] := 0;
    tst := nil;

    //对每一个分区操作
    for q := 0 to FRltInfo.FenQuNum - 1 do
    begin
      SendMsgData(cparam.frmMainHandle, '解析分区 ' + IntToStr(q) + ' 起始节点和结束节点');
      nStartPosAryRlt[q + 1] := nStartPosAryRlt[q] + FVarNum * FFenQuAryRlt[q].IdotNums * FFenQuAryRlt[q].JDotNums * FFenQuAryRlt[q].KDotNums;
    end;
  except
    ;
  end;
  //每一个变量的最大值和最小值
  for v := 0 to FVarNum - 1 do
  begin
    tmpValueMin := 999999; tmpvalueMax := -999999;
    for q := 0 to FRltInfo.FenQuNum - 1 do
    begin

      for i := 0 to FFenQuAryRlt[q].IdotNums - 1 do
        for j := 0 to FFenQuAryRlt[q].JDotNums - 1 do
          for k := 0 to FFenQuAryRlt[q].KDotNums - 1 do
          begin
            if (tmpValuemin > GetResultByIndex(q, i, j, k, v + 3)) then
              tmpValueMin := GetResultByIndex(q, i, j, k, v + 3);
            if (tmpValueMax < GetResultByIndex(q, i, j, k, v + 3)) then
              tmpValueMax := GetResultByIndex(q, i, j, k, v + 3);
          end;
    end;
    FRltInfo.VarnameList[v].VarValueMin := tmpValueMin;
    FRltInfo.VarnameList[v].VarValueMax := tmpValueMax;
  end;
  IsRltLoaded := True;
end;
*/

void HuangzbPlot3DFormattedReaderClass::Load(QString AFileName,QProgressDialog *progressBar)
{
  double tmp;
  char ttt[1024];
  string tmpStr;
  //TStringList *tmpLine;
  int tmpMax;
  int fenquNums;
  int i, j, k, q;
  int nCnt;
  int tPos;
  //tst: array of array of array of T3DPoint;
  std::vector< std::vector <std::vector< T3DPoint > > > tst;

  FprogressBar=progressBar;
  FFileName = AFileName;
  //while IsDrawing
  // SendMsgData(cparam.frmMainHandle, DateTimeToStr(Now()) + '等待中...');
  if (IsLoaded )
  {
      //初始化和清理一下。
    FXMax = -99999;
    FYMax = -99999;
    FZMax = -99999;
    FXMin = 99999;
    FYMin = 99999;
    FZMin = 99999;
    //nStartPosAry := nil;
    //Finfo.Length =0;//:= nil;
	Finfo.clear();
    //FXYZ := nil;
    //FFenQuAry.Length =0 ;// := nil;
	FFenQuAry.clear();
	nStartPosAry.clear();
	nStartPosAryRlt.clear();
	

    IsLoaded = false;
  }
  logger()->debug("plot3dfilename="+AFileName);

  IsLoaded = false;
  //SendMsgData(cparam.frmMainHandle, '正在解析文件:' + AFilename);
  QFileInfo f(AFileName);
  if (!f.exists()) { IsLoaded = false; return; }
  try{
    //AssignFile(FFile, AFilename);
    FFile=fopen(AFileName.toStdString().c_str(),"r");
    if (FFile==NULL) {IsLoaded=false;return;}
    //Reset(FFile);
    //读取分区的数量
    //Read(FFile, FFenQu);
    fscanf(FFile,"%d",&FFenQu);
	logger()->debug(QString("%1").arg(FFenQu));
    //SendMsgData(cparam.frmMainHandle, '读取分区的数量:' + IntToStr(FFenQu));
    //SetLength(FFenQuAry, FFenQu + 1);
    //FFenQuAry.Length = FFenQu+1;

	TFenQue tmpFenQuInfo;

    FDotNum = 0;
    //for i := 0 to FFenQu - 1 do
    for(i=0;i<=FFenQu-1;i++)
    {
      tmpFenQuInfo.IsDraw = true;

      tmpFenQuInfo.nIBegin = 0;
      //Read(FFile, FFenQuAry[i].IdotNums);
      fscanf(FFile,"%d",&tmpFenQuInfo.IDotNums);
	  logger()->debug(QString("%1").arg(tmpFenQuInfo.IDotNums));

      tmpMax = tmpFenQuInfo.IDotNums;
      tmpFenQuInfo.nIEnd  = tmpFenQuInfo.IDotNums - 1;
      tmpFenQuInfo.nISkip = tmpFenQuInfo.IDotNums - 1;
      tmpFenQuInfo.bIDir = true;

      tmpFenQuInfo.nJBegin = 0;
      //Read(FFile, FFenQuAry[i].JDotNums);
      fscanf(FFile,"%d",&tmpFenQuInfo.JDotNums);
	  logger()->debug(QString("%1").arg(tmpFenQuInfo.JDotNums));

      if (tmpMax < tmpFenQuInfo.JDotNums) tmpMax = tmpFenQuInfo.JDotNums;
      tmpFenQuInfo.nJEnd = tmpFenQuInfo.JDotNums - 1;
      tmpFenQuInfo.nJSkip = tmpFenQuInfo.JDotNums - 1;
      tmpFenQuInfo.bJDir = true;

      tmpFenQuInfo.nKBegin = 0;
      //Read(FFile, FFenQuAry[i].KDotNums);
      fscanf(FFile,"%d",&tmpFenQuInfo.KDotNums);
	  logger()->debug(QString("%1").arg(tmpFenQuInfo.KDotNums));

      if (tmpMax < tmpFenQuInfo.KDotNums) tmpMax = tmpFenQuInfo.KDotNums;
      tmpFenQuInfo.nKEnd = tmpFenQuInfo.KDotNums - 1;
      tmpFenQuInfo.nKSkip = tmpFenQuInfo.KDotNums - 1;
      tmpFenQuInfo.bKDir = true;

      tmpFenQuInfo.MaxDotNum = tmpMax;

	  tmpFenQuInfo.bSelected = false;

	  FFenQuAry.push_back(tmpFenQuInfo);

      //FMaxPointNum := FMaxPointNum + tmpMax; //最大点数
      FDotNum = FDotNum + tmpFenQuInfo.IDotNums * tmpFenQuInfo.JDotNums * tmpFenQuInfo.KDotNums;
      FITotalDot = FITotalDot + tmpFenQuInfo.IDotNums;
      FJTotalDot = FJTotalDot + tmpFenQuInfo.JDotNums;
      FKTotalDot = FKTotalDot + tmpFenQuInfo.KDotNums;
    }

    //设置点数组的大小
    //SetLength(tst, 50, 50, 50);
    //SetLength(Finfo, FDotNum * 3);
    //Finfo.Length = FDotNum*4;
	int nsteps=0;
	FprogressBar->setRange(0,FDotNum * 3+100);
	QString strprompt("正在加载网格文件...\n根据网格文件的大小，需要的时间会有所不同，请稍等!");
	strprompt=strprompt+"\n文件名："+AFileName;
	QFileInfo tmpinfo(AFileName);
	strprompt=strprompt+QString("\n文件大小：%1bytes").arg(tmpinfo.size());

	FprogressBar->setLabelText(strprompt);

    //读取坐标点
    nCnt = 0;
    //while (not EOF(FFile)) do
    tmp=0.0;
    QString tmpLine;//=new TStringList();
	QString strSpace(" ");
    while (!feof(FFile))
    {
      fgets(ttt,1024,FFile);
	  //logger()->debug(ttt);

      tmpLine = ttt;
	  tmpLine.simplified();
	  tmpLine.trimmed();
	  if (tmpLine.isEmpty()||(tmpLine.size()==1)) continue;
	  QStringList tmplist=tmpLine.split(" ",QString::SkipEmptyParts);
	  //QList<QString>::Iterator it = tmplist.begin(),itend = tmplist.end();
	  //int i=0;
	  int nCount = tmplist.size();
      for(int i=0;i<nCount;i++)
      {
	     nsteps++;
	     FprogressBar->setValue(nsteps);
		 qApp->processEvents();
		 //QCoreApplication::processEvents(QEventLoop::AllEvents, 100); 
		 if (FprogressBar->wasCanceled())
		 {
			 IsLoaded=false;
			 //初始化和清理一下。
			 FXMax = -99999;
			 FYMax = -99999;
			 FZMax = -99999;
			 FXMin = 99999;
			 FYMin = 99999;
			 FZMin = 99999;
			 //nStartPosAry := nil;
			 //Finfo.Length =0;//:= nil;
			 Finfo.clear();
			 //FXYZ := nil;
			 //FFenQuAry.Length =0 ;// := nil;
			 FFenQuAry.clear();
			 nStartPosAry.clear();
			 nStartPosAryRlt.clear();
			 return;
		 }
    	 QString tmpss = static_cast<QString>(tmplist.at(i));
		 tmpss.simplified();
		 tmpss.trimmed();
		 if (tmpss.isEmpty()||(tmpss.size()==1)) continue;
         Finfo.push_back(tmpss.toDouble());
         //nCnt++;
      }
    }
    //CloseFile(FFile);
    fclose(FFile);

    //把所有的点的坐标值转换到对应的点的X,Y,Z
//    FFenQuAry[0].fenquIStartPoint := 0;
//    FFenQuAry[0].fenquJStartPoint := 0;
//    FFenQuAry[0].fenqukStartPoint := 0;
    //SetLength(nStartPosAry, FFenQu + 1);
    //nStartPosAry.Length = FFenQu + 1;
    //nStartPosAry[0] = 0;
	nStartPosAry.push_back(0);
    //tst := nil;

    //对每一个分区操作
    //for q := 0 to FFenQu - 1 do
    for(q=0;q<=FFenQu-1;q++)
    {
      //SendMsgData(cparam.frmMainHandle, '解析分区 ' + IntToStr(q) + ' 起始节点和结束节点');
	  //int tmpStartPos=nStartPosAry.at(q);
      //nStartPosAry[q + 1] = nStartPosAry.at[q] + 3 * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums * FFenQuAry[q].KDotNums;
	  nStartPosAry.push_back(nStartPosAry[q] + 3 * FFenQuAry[q].IDotNums * FFenQuAry[q].JDotNums * FFenQuAry[q].KDotNums);
    }

    //for q := 0 to FFenQu - 1 do
    for(q=0;q<=FFenQu-1;++q)
    {
      //分辨出最大的和最小的坐标值
      //SendMsgData(cparam.frmMainHandle, '分辨出分区 ' + IntToStr(q) + ' 最大的和最小的坐标值');
      //for k := 0 to FFenQuAry[q].KDotNums - 1 do
        //for j := 0 to FFenQuAry[q].JDotNums - 1 do
          //for i := 0 to FFenQuAry[q].IdotNums - 1 do
      for(k=0;k<=FFenQuAry[q].KDotNums - 1;k++)
        for(j=0;j<=FFenQuAry[q].JDotNums - 1;j++)
          for(i=0;i<=FFenQuAry[q].IDotNums - 1;i++)
          {
		  nsteps++;
		  FprogressBar->setValue(nsteps);
		  qApp->processEvents();

            if (FXMax < GetXYZ(q, i, j, k).X)
            {
              FXMax = GetXYZ(q, i, j, k).X;
              FXMaxDot = i;
            }
            if (FXMin > GetXYZ(q, i, j, k).X)
            {
              FXMin = GetXYZ(q, i, j, k).X;
              FXMinDot = i;
            }

            if (FYMax < GetXYZ(q, i, j, k).Y)
            {
              FYMax = GetXYZ(q, i, j, k).Y;
              FYMaxDot = i;
            }
            if (FYMin > GetXYZ(q, i, j, k).Y)
            {
              FYMin = GetXYZ(q, i, j, k).Y;
              FYMinDot = i;
            }

            if (FZMax < GetXYZ(q, i, j, k).Z)
            {
              FZMax = GetXYZ(q, i, j, k).Z;
              FZMaxDot = i;
            }
            if (FZMin > GetXYZ(q, i, j, k).Z)
            {
              FZMin = GetXYZ(q, i, j, k).Z;
              FZMinDot = i;
            }
          }
    }//; //for q:=0 to FFenQu-1
    //调整视景体的形状
    nBaseRange = (FYMax - FYMin);
	if ((FXMax-FXMin)>nBaseRange) nBaseRange=(FXMax-FXMin);
	if ((FZMax-FZMin)>nBaseRange) nBaseRange=(FZMax-FZMin);
    globalcfg.cparam.nRange = nBaseRange;
	logger()->debug(QString("globalcfg.cparam.nRange=%1").arg(globalcfg.cparam.nRange));

  }
  catch(...)
  {
    FFenQuAry.clear();// = nil;
    LoadState=-1;
  }
  //检测plot3d文件与边界文件是否匹配。
  //if (FbInpFound)
  if (0)
  {
    FbCheckOk = true;
    if (EdgeInfo.FenQuNum != FFenQu) FbCheckOk = false;
    //for q := 0 to FFenQu - 1 do
    for(q=0;q<=FFenQu-1;q++)
    {
      if ((EdgeInfo.FebQuList[q].IDotNums != FFenQuAry[q].IDotNums) ||
        (EdgeInfo.FebQuList[q].JDotNums != FFenQuAry[q].JDotNums) ||
        (EdgeInfo.FebQuList[q].KDotNums != FFenQuAry[q].KDotNums))
        FbCheckOk = false;
    }
   // if (not FbcheckOk) then
   //   ShowMessage('边界文件与网格文件不匹配');
  }
  //如果匹配成功，设置FbCheckOk:=True;
  IsLoaded = true;
}

void HuangzbPlot3DFormattedReaderClass::DrawLod()
{
  int i, j, k, q;
  int itmpMax, jtmpMax, ktmpMax;
  bool bi, bj, bk;
  int iLodLevel, jLodLevel, kLodLevel;
  int iB1, iB2, jB1, jB2, kB1, kB2;

  //K切面是平行于X轴
  //J切面是平行于Y轴
  //I切面是平行于Z轴
  //glEnable(GL_LINE_SMOOTH);

  if (!this->IsLoaded) return;

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  if ((! globalcfg.cparam.bKDir) || (! globalcfg.cparam.bIDir) || (! globalcfg.cparam.bJDir)) return;
  IsDrawing = true;
  glColor3ub(0, 0, 0);
    //画主体
  glInitNames();
  //for q := 0 to FFenQu - 1 do
  for(q=0;q<=FFenQu-1;q++)
  {
    if (! FFenQuAry[q].IsDraw)  continue;
    if (FFenQuAry[q].bSelected)
      glColor3ub(255, 255, 255);
    else
      glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
      //K=0//////////////////////////////////////////////////////////////////////
      //k:=0;
    glPushName(q);
    glBegin(GL_QUADS);
    bi = false; bj = false; bk = false;
    iLodLevel = floor(sqrt(1.0*FFenQuAry[q].IDotNums));
    itmpMax = iLodLevel * iLodLevel - iLodLevel - 1;
    if (itmpMax < FFenQuAry[q].IDotNums - 1 - iLodLevel)  { bi = true; itmpMax = itmpMax + 1; }
    jLodLevel = floor(sqrt(1.0*FFenQuAry[q].JDotNums));
    jtmpMax = jLodLevel * jLodLevel - jLodLevel - 1;
    if (jtmpMax < FFenQuAry[q].JDotNums - 1 - jLodLevel)  { bj = true; jtmpMax = jtmpMax + 1; }
    kLodLevel = floor(sqrt(1.0*FFenQuAry[q].KDotNums));
    ktmpMax = kLodLevel * kLodLevel - kLodLevel - 1;
    if (ktmpMax < FFenQuAry[q].KDotNums - 1 - kLodLevel)  { bk = true; ktmpMax = ktmpMax + 1; }

    if (bi)
    { iB1 = itmpMax + iLodLevel - 1; iB2 = FFenQuAry[q].IDotNums - 1; }
    else { iB1 = itmpMax; iB2 = FFenQuAry[q].IDotNums - 1; }
    if (bj)
    { jB1 = jtmpMax + jLodLevel - 1; jB2 = FFenQuAry[q].JDotNums - 1; }
    else { jB1 = jtmpMax; jB2 = FFenQuAry[q].JDotNums - 1; }
    if (bk)
    { kB1 = ktmpMax + kLodLevel - 1; kB2 = FFenQuAry[q].KDotNums - 1; }
    else { kB1 = ktmpMax; kB2 = FFenQuAry[q].KDotNums - 1; }

    //中间地段的处理
    //for k := 0 to FFenQuAry[q].KdotNums - 1 do
    for(k=0;k<=FFenQuAry[q].KDotNums - 1;k++)
    {
      if (! ((k < 1) || (k >= FFenQuAry[q].KDotNums - 1)))   continue;
      //for i := 0 to itmpMax do
      for(i=0;i<=itmpMax;i++)
      {
        if (! (((i % iLodLevel) == 0) || (i > itmpMax - 1)))   continue; 
        //for j := 0 to jtmpMax do
        for(j=0;j<=jtmpMax;j++)
        {
          if (! (((j % jLodLevel) == 0) || (j > jtmpMax - 1)))   continue; 
          glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
            GetXYZ(q, i, j, k).Y / nBaseRange,
            GetXYZ(q, i, j, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, j + jLodLevel, k).X / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, k).Y / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i + iLodLevel, j + jLodLevel, k).X / nBaseRange,
            GetXYZ(q, i + iLodLevel, j + jLodLevel, k).Y / nBaseRange,
            GetXYZ(q, i + iLodLevel, j + jLodLevel, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i + iLodLevel, j, k).X / nBaseRange,
            GetXYZ(q, i + iLodLevel, j, k).Y / nBaseRange,
            GetXYZ(q, i + iLodLevel, j, k).Z / nBaseRange);
        }
        ////////////////////////////////////////////////////////////////////////
        if (jtmpMax >= 0) 
        {
          glVertex3f(GetXYZ(q, i, jtmpMax, k).X / nBaseRange,
            GetXYZ(q, i, jtmpMax, k).Y / nBaseRange,
            GetXYZ(q, i, jtmpMax, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, jB1, k).X / nBaseRange,
            GetXYZ(q, i, jB1, k).Y / nBaseRange,
            GetXYZ(q, i, jB1, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i + iLodLevel, jB1, k).X / nBaseRange,
            GetXYZ(q, i + iLodLevel, jB1, k).Y / nBaseRange,
            GetXYZ(q, i + iLodLevel, jB1, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i + iLodLevel, jtmpMax, k).X / nBaseRange,
            GetXYZ(q, i + iLodLevel, jtmpMax, k).Y / nBaseRange,
            GetXYZ(q, i + iLodLevel, jtmpMax, k).Z / nBaseRange);
        }
      //边界的处理/////////////////////////////////////////////////////////////
        if (jB1 >= 0)
        {
          glVertex3f(GetXYZ(q, i, jB1, k).X / nBaseRange,
            GetXYZ(q, i, jB1, k).Y / nBaseRange,
            GetXYZ(q, i, jB1, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, jB2, k).X / nBaseRange,
            GetXYZ(q, i, jB2, k).Y / nBaseRange,
            GetXYZ(q, i, jB2, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i + iLodLevel, jB2, k).X / nBaseRange,
            GetXYZ(q, i + iLodLevel, jB2, k).Y / nBaseRange,
            GetXYZ(q, i + iLodLevel, jB2, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i + iLodLevel, jB1, k).X / nBaseRange,
            GetXYZ(q, i + iLodLevel, jB1, k).Y / nBaseRange,
            GetXYZ(q, i + iLodLevel, jB1, k).Z / nBaseRange);
        }
      }
    }
    //修补一下平行
    //for j := 0 to jtmpMax do
    for(j=0;j<=jtmpMax;j++)
    {
      if (! (((j % jLodLevel) == 0) || (j > jtmpMax - 2)))   continue; 
      //修补平行于X切面的漏洞/////////////////////////////////////////////////
      glVertex3f(GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
        GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
        GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
        GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
        GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
        GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
        GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
        GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
        GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);
      /////////////////////////////////////////////////////////////////////////
      glVertex3f(GetXYZ(q, iB1, j, 0).X / nBaseRange,
        GetXYZ(q, iB1, j, 0).Y / nBaseRange,
        GetXYZ(q, iB1, j, 0).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB2, j, 0).X / nBaseRange,
        GetXYZ(q, iB2, j, 0).Y / nBaseRange,
        GetXYZ(q, iB2, j, 0).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB2, j + jLodLevel, 0).X / nBaseRange,
        GetXYZ(q, iB2, j + jLodLevel, 0).Y / nBaseRange,
        GetXYZ(q, iB2, j + jLodLevel, 0).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB1, j + jLodLevel, 0).X / nBaseRange,
        GetXYZ(q, iB1, j + jLodLevel, 0).Y / nBaseRange,
        GetXYZ(q, iB1, j + jLodLevel, 0).Z / nBaseRange);
    }
    glEnd();
    glPopName();
  }
  ////////////////////////ABOVE//GOOOD/////////////////////////////////////////////
 //画边界面I(X)切面的边界
  //for q := 0 to FFenQu - 1 do
  for(q=0;q<=FFenQu-1;q++)
  {
    if (! FFenQuAry[q].IsDraw)  continue;
    if (FFenQuAry[q].bSelected) 
      glColor3ub(255, 255, 255);
    else
      glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    glPushName(q);
    glBegin(GL_QUADS);
    bi = false; bj = false; bk = false;
    iLodLevel = floor(sqrt(1.0*FFenQuAry[q].IDotNums));
    itmpMax = iLodLevel * iLodLevel - iLodLevel - 1;
    if (itmpMax < FFenQuAry[q].IDotNums - 1 - iLodLevel)  { bi = true; itmpMax = itmpMax + 1; }
    jLodLevel = floor(sqrt(1.0*FFenQuAry[q].JDotNums));
    jtmpMax = jLodLevel * jLodLevel - jLodLevel - 1;
    if (jtmpMax < FFenQuAry[q].JDotNums - 1 - jLodLevel)  { bj = true; jtmpMax = jtmpMax + 1; }
    kLodLevel = floor(sqrt(1.0*FFenQuAry[q].KDotNums));
    ktmpMax = kLodLevel * kLodLevel - kLodLevel - 1;
    if (ktmpMax < FFenQuAry[q].KDotNums - 1 - kLodLevel)  { bk = true; ktmpMax = ktmpMax + 1; }

    if (bi)
    { iB1 = itmpMax + iLodLevel; iB2 = FFenQuAry[q].IDotNums - 1; }
    else { iB1 = itmpMax; iB2 = FFenQuAry[q].IDotNums - 1; }
    if (bj)
    { jB1 = jtmpMax + jLodLevel; jB2 = FFenQuAry[q].JDotNums - 1; }
    else { jB1 = jtmpMax; jB2 = FFenQuAry[q].JDotNums - 1; }
    if (bk) 
    { kB1 = ktmpMax + kLodLevel; kB2 = FFenQuAry[q].KDotNums - 1; }
    else { kB1 = ktmpMax; kB2 = FFenQuAry[q].KDotNums - 1; }

    //for i := 0 to FFenQuAry[q].IdotNums - 1 do
    for(i=0;i<=FFenQuAry[q].IDotNums - 1;i++)
    {
      if (! ((i < 1) || (i > FFenQuAry[q].IDotNums - 2)))  continue;
      //for j := 0 to jtmpMax do
      for(j=0;j<=jtmpMax;j++)
      {
        if (! (((j % jLodLevel) == 0) || (j > jtmpMax - 1))) continue;
        //for k := 0 to ktmpMax do
        for(k=0;k<=ktmpMax;k++)
        {
          if (! (((k % kLodLevel) == 0) || (k > ktmpMax - 1)))  continue;
          glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
            GetXYZ(q, i, j, k).Y / nBaseRange,
            GetXYZ(q, i, j, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, j, k + kLodLevel).X / nBaseRange,
            GetXYZ(q, i, j, k + kLodLevel).Y / nBaseRange,
            GetXYZ(q, i, j, k + kLodLevel).Z / nBaseRange);


          glVertex3f(GetXYZ(q, i, j + jLodLevel, k + kLodLevel).X / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, k + kLodLevel).Y / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, k + kLodLevel).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, j + jLodLevel, k).X / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, k).Y / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, k).Z / nBaseRange);
        }
        //?
        if (ktmpMax >= 0) 
        {
          glVertex3f(GetXYZ(q, i, j, ktmpMax).X / nBaseRange,
            GetXYZ(q, i, j, ktmpMax).Y / nBaseRange,
            GetXYZ(q, i, j, ktmpMax).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, j, kB1).X / nBaseRange,
            GetXYZ(q, i, j, kB1).Y / nBaseRange,
            GetXYZ(q, i, j, kB1).Z / nBaseRange);


          glVertex3f(GetXYZ(q, i, j + jLodLevel, kB1).X / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, kB1).Y / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, kB1).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, j + jLodLevel, ktmpMax).X / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, ktmpMax).Y / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, ktmpMax).Z / nBaseRange);
        }
//        //////////////////////////////////////////////////////////////////////
        if (kB1 >= 0)
        {
          glVertex3f(GetXYZ(q, i, j, kB2).X / nBaseRange,
            GetXYZ(q, i, j, kB2).Y / nBaseRange,
            GetXYZ(q, i, j, kB2).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, j, kB1).X / nBaseRange,
            GetXYZ(q, i, j, kB1).Y / nBaseRange,
            GetXYZ(q, i, j, kB1).Z / nBaseRange);


          glVertex3f(GetXYZ(q, i, j + jLodLevel, kB1).X / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, kB1).Y / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, kB1).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, j + jLodLevel, kB2).X / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, kB2).Y / nBaseRange,
            GetXYZ(q, i, j + jLodLevel, kB2).Z / nBaseRange);
        }
      }
      //边界的处理
    //修补一下平行
      //for j := 0 to jtmpMax do
      for(j=0;j<=jtmpMax;j++)
      {
        if (! (((j % jLodLevel) == 0) || (j > jtmpMax - 2)))   continue; 
      //修补平行于X切面的漏洞/////////////////////////////////////////////////
        glVertex3f(GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
          GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
          GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

        glVertex3f(GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
          GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
          GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

        glVertex3f(GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
          GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
          GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

        glVertex3f(GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
          GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
          GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);
      /////////////////////////////////////////////////////////////////////////
        glVertex3f(GetXYZ(q, iB1, j, 0).X / nBaseRange,
          GetXYZ(q, iB1, j, 0).Y / nBaseRange,
          GetXYZ(q, iB1, j, 0).Z / nBaseRange);

        glVertex3f(GetXYZ(q, iB2, j, 0).X / nBaseRange,
          GetXYZ(q, iB2, j, 0).Y / nBaseRange,
          GetXYZ(q, iB2, j, 0).Z / nBaseRange);

        glVertex3f(GetXYZ(q, iB2, j + jLodLevel, 0).X / nBaseRange,
          GetXYZ(q, iB2, j + jLodLevel, 0).Y / nBaseRange,
          GetXYZ(q, iB2, j + jLodLevel, 0).Z / nBaseRange);

        glVertex3f(GetXYZ(q, iB1, j + jLodLevel, 0).X / nBaseRange,
          GetXYZ(q, iB1, j + jLodLevel, 0).Y / nBaseRange,
          GetXYZ(q, iB1, j + jLodLevel, 0).Z / nBaseRange);
      }

    }
    glEnd();
    glPopName();
  }
  ///////////////////////////////////////////////////////////////////////////
  ///////////ABOVE GOOD/////////////////////////////////////////////////////
  //画边界面J(Y)切面的边界
  //for q := 0 to FFenQu - 1 do
  for(q=0;q<=FFenQu-1;q++)
  {
    if (! FFenQuAry[q].IsDraw)  continue;
    if (FFenQuAry[q].bSelected)
      glColor3ub(255, 255, 255);
    else
      glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    glPushName(q);
    glBegin(GL_QUADS);
    bi = false; bj = false; bk = false;
    iLodLevel = floor(sqrt(1.0*FFenQuAry[q].IDotNums));
    itmpMax = iLodLevel * iLodLevel - iLodLevel - 1;
    if (itmpMax < FFenQuAry[q].IDotNums - 1 - iLodLevel)  { bi = true; itmpMax = itmpMax + 1; }
    jLodLevel = floor(sqrt(1.0*FFenQuAry[q].JDotNums));
    jtmpMax = jLodLevel * jLodLevel - jLodLevel - 1;
    if (jtmpMax < FFenQuAry[q].JDotNums - 1 - jLodLevel)  { bj = true; jtmpMax = jtmpMax + 1; }
    kLodLevel = floor(sqrt(1.0*FFenQuAry[q].KDotNums));
    ktmpMax = kLodLevel * kLodLevel - kLodLevel - 1;
    if (ktmpMax < FFenQuAry[q].KDotNums - 1 - kLodLevel)  { bk = true; ktmpMax = ktmpMax + 1; }

    if (bi)
    { iB1 = itmpMax + iLodLevel; iB2 = FFenQuAry[q].IDotNums - 1; }
    else { iB1 = itmpMax; iB2 = FFenQuAry[q].IDotNums - 1; }
    if (bj)
    { jB1 = jtmpMax + jLodLevel; jB2 = FFenQuAry[q].JDotNums - 1; }
    else { jB1 = jtmpMax; jB2 = FFenQuAry[q].JDotNums - 1; }
    if (bk)
    { kB1 = ktmpMax + kLodLevel; kB2 = FFenQuAry[q].KDotNums - 1; }
    else { kB1 = ktmpMax; kB2 = FFenQuAry[q].KDotNums - 1; }

    //for j := 0 to FFenQuAry[q].JdotNums - 1 do
    for(j=0;j<=FFenQuAry[q].JDotNums - 1;j++)
    {
      if (! ((j < 1) || (j >= FFenQuAry[q].JDotNums - 1)))  continue;
      //for k := 0 to ktmpMax do
      for(k=0;k<=ktmpMax;k++)
      {
        if (! (((k % kLodLevel) == 0) || (k > ktmpMax - 1)))  continue;
        //for i := 0 to itmpMax do
        for(i=0;i<itmpMax;i++)
        {
          if (! (((i % iLodLevel) == 0) || (i > itmpMax - 1)))  continue;
          glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
            GetXYZ(q, i, j, k).Y / nBaseRange,
            GetXYZ(q, i, j, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i, j, k + kLodLevel).X / nBaseRange,
            GetXYZ(q, i, j, k + kLodLevel).Y / nBaseRange,
            GetXYZ(q, i, j, k + kLodLevel).Z / nBaseRange);


          glVertex3f(GetXYZ(q, i + iLodLevel, j, k + kLodLevel).X / nBaseRange,
            GetXYZ(q, i + iLodLevel, j, k + kLodLevel).Y / nBaseRange,
            GetXYZ(q, i + iLodLevel, j, k + kLodLevel).Z / nBaseRange);

          glVertex3f(GetXYZ(q, i + iLodLevel, j, k).X / nBaseRange,
            GetXYZ(q, i + iLodLevel, j, k).Y / nBaseRange,
            GetXYZ(q, i + iLodLevel, j, k).Z / nBaseRange);
        }
        if (itmpMax >= 0)
        {
          glVertex3f(GetXYZ(q, itmpMax, j, k).X / nBaseRange,
            GetXYZ(q, itmpMax, j, k).Y / nBaseRange,
            GetXYZ(q, itmpMax, j, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, itmpMax, j, k + kLodLevel).X / nBaseRange,
            GetXYZ(q, itmpMax, j, k + kLodLevel).Y / nBaseRange,
            GetXYZ(q, itmpMax, j, k + kLodLevel).Z / nBaseRange);


          glVertex3f(GetXYZ(q, iB1, j, k + kLodLevel).X / nBaseRange,
            GetXYZ(q, iB1, j, k + kLodLevel).Y / nBaseRange,
            GetXYZ(q, iB1, j, k + kLodLevel).Z / nBaseRange);

          glVertex3f(GetXYZ(q, iB1, j, k).X / nBaseRange,
            GetXYZ(q, iB1, j, k).Y / nBaseRange,
            GetXYZ(q, iB1, j, k).Z / nBaseRange);
        }

        if (iB1 >= 0)
        {
          glVertex3f(GetXYZ(q, iB2, j, k).X / nBaseRange,
            GetXYZ(q, iB2, j, k).Y / nBaseRange,
            GetXYZ(q, iB2, j, k).Z / nBaseRange);

          glVertex3f(GetXYZ(q, iB2, j, k + kLodLevel).X / nBaseRange,
            GetXYZ(q, iB2, j, k + kLodLevel).Y / nBaseRange,
            GetXYZ(q, iB2, j, k + kLodLevel).Z / nBaseRange);


          glVertex3f(GetXYZ(q, iB1, j, k + kLodLevel).X / nBaseRange,
            GetXYZ(q, iB1, j, k + kLodLevel).Y / nBaseRange,
            GetXYZ(q, iB1, j, k + kLodLevel).Z / nBaseRange);

          glVertex3f(GetXYZ(q, iB1, j, k).X / nBaseRange,
            GetXYZ(q, iB1, j, k).Y / nBaseRange,
            GetXYZ(q, iB1, j, k).Z / nBaseRange);
        }
      }
      //边界的处理
    }

    //for j := 0 to jtmpMax do
    for(j=0;j<=jtmpMax;j++)
    {
      if (! (((j % jLodLevel) == 0) || (j > jtmpMax - 2)))  continue; 
      //修补平行于X切面的漏洞/////////////////////////////////////////////////
      glVertex3f(GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
        GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
        GetXYZ(q, iB1, j, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
        GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
        GetXYZ(q, iB2, j, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
        GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
        GetXYZ(q, iB2, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).X / nBaseRange,
        GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Y / nBaseRange,
        GetXYZ(q, iB1, j + jLodLevel, FFenQuAry[q].KDotNums - 1).Z / nBaseRange);
      /////////////////////////////////////////////////////////////////////////
      glVertex3f(GetXYZ(q, iB1, j, 0).X / nBaseRange,
        GetXYZ(q, iB1, j, 0).Y / nBaseRange,
        GetXYZ(q, iB1, j, 0).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB2, j, 0).X / nBaseRange,
        GetXYZ(q, iB2, j, 0).Y / nBaseRange,
        GetXYZ(q, iB2, j, 0).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB2, j + jLodLevel, 0).X / nBaseRange,
        GetXYZ(q, iB2, j + jLodLevel, 0).Y / nBaseRange,
        GetXYZ(q, iB2, j + jLodLevel, 0).Z / nBaseRange);

      glVertex3f(GetXYZ(q, iB1, j + jLodLevel, 0).X / nBaseRange,
        GetXYZ(q, iB1, j + jLodLevel, 0).Y / nBaseRange,
        GetXYZ(q, iB1, j + jLodLevel, 0).Z / nBaseRange);
    }

    glEnd();
    glPopName();
  }
 /////////////////////////////////////////////////////////////////////////////
  ///////////Above Good////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////////////
 //画各分区的边界线
  if (globalcfg.cparam.bShowEdge)
  {
    //for q := 0 to FFenQu - 1 do
    for(q=0;q<=FFenQu - 1;q++)
    {
      //for i := 0 to FFenQuAry[q].IdotNums - 2 do
      for(i=0;i<=FFenQuAry[q].IDotNums - 2;i++)
      {
        k = FFenQuAry[q].KDotNums - 1;
        j = FFenQuAry[q].JDotNums - 1;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
          GetXYZ(q, i + 1, j, k).Y / nBaseRange,
          GetXYZ(q, i + 1, j, k).Z / nBaseRange);
        glEnd();

        k = 0;
        j = 0;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
          GetXYZ(q, i + 1, j, k).Y / nBaseRange,
          GetXYZ(q, i + 1, j, k).Z / nBaseRange);
        glEnd();

        k = FFenQuAry[q].KDotNums - 1;
        j = 0;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
          GetXYZ(q, i + 1, j, k).Y / nBaseRange,
          GetXYZ(q, i + 1, j, k).Z / nBaseRange);
        glEnd();

        k = 0;
        j = FFenQuAry[q].JDotNums - 1;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
          GetXYZ(q, i + 1, j, k).Y / nBaseRange,
          GetXYZ(q, i + 1, j, k).Z / nBaseRange);
        glEnd();

      }

      //for j := 0 to FFenQuAry[q].JdotNums - 2 do
      for(j=0;j<=FFenQuAry[q].JDotNums - 2;j++)
      {
        k = FFenQuAry[q].KDotNums - 1;
        i = FFenQuAry[q].IDotNums - 1;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
          GetXYZ(q, i, j + 1, k).Y / nBaseRange,
          GetXYZ(q, i, j + 1, k).Z / nBaseRange);
        glEnd();

        k = 0;
        i = 0;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
          GetXYZ(q, i, j + 1, k).Y / nBaseRange,
          GetXYZ(q, i, j + 1, k).Z / nBaseRange);
        glEnd();

        k = FFenQuAry[q].KDotNums - 1;
        i = 0;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
          GetXYZ(q, i, j + 1, k).Y / nBaseRange,
          GetXYZ(q, i, j + 1, k).Z / nBaseRange);
        glEnd();


        k = 0;
        i = FFenQuAry[q].IDotNums - 1;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
          GetXYZ(q, i, j + 1, k).Y / nBaseRange,
          GetXYZ(q, i, j + 1, k).Z / nBaseRange);
        glEnd();
      }

      //for K := 0 to FFenQuAry[q].KdotNums - 2 do
      for(k=0;k<=FFenQuAry[q].KDotNums - 2;k++)
      {
        j = FFenQuAry[q].JDotNums - 1;
        i = FFenQuAry[q].IDotNums - 1;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
          GetXYZ(q, i, j, k + 1).Y / nBaseRange,
          GetXYZ(q, i, j, k + 1).Z / nBaseRange);
        glEnd();

        j = 0;
        i = 0;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
          GetXYZ(q, i, j, k + 1).Y / nBaseRange,
          GetXYZ(q, i, j, k + 1).Z / nBaseRange);
        glEnd();

        j = FFenQuAry[q].JDotNums - 1;
        i = 0;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
          GetXYZ(q, i, j, k + 1).Y / nBaseRange,
          GetXYZ(q, i, j, k + 1).Z / nBaseRange);
        glEnd();

        j = 0;
        i = FFenQuAry[q].IDotNums - 1;
        glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
          GetXYZ(q, i, j, k + 1).Y / nBaseRange,
          GetXYZ(q, i, j, k + 1).Z / nBaseRange);
        glEnd();
      }
    }
  }
  glDisable(GL_LINE_SMOOTH);
  IsDrawing = false;
}

void HuangzbPlot3DFormattedReaderClass::DrawClip(int I, int J, int K)
{
  if (I != -1)
  {

  }
  else if (J != -1)
  {

  }
  else if (K != -1)
  {


  }
}

void HuangzbPlot3DFormattedReaderClass::invertMatrix(GLdouble m[],GLdouble o[])
{
  GLdouble det, invDet;
  GLdouble d12, d13, d23, d24, d34, d41;
  GLdouble tmp[16];//: array[0..15] of GLdouble;
  GLdouble m11, m12, m13, m14;
  GLdouble m21, m22, m23, m24;
  GLdouble m31, m32, m33, m34;
  GLdouble m41, m42, m43, m44;
  int i;


  m11 = MAT(m, 0, 0);
  m12 = MAT(m, 0, 1);
  m13 = MAT(m, 0, 2);
  m14 = MAT(m, 0, 3);
  m21 = MAT(m, 1, 0);
  m22 = MAT(m, 1, 1);
  m23 = MAT(m, 1, 2);
  m24 = MAT(m, 1, 3);
  m31 = MAT(m, 2, 0);
  m32 = MAT(m, 2, 1);
  m33 = MAT(m, 2, 2);
  m34 = MAT(m, 2, 3);
  m41 = MAT(m, 3, 0);
  m42 = MAT(m, 3, 1);
  m43 = MAT(m, 3, 2);
  m44 = MAT(m, 3, 3);
  d12 = (m31 * m42 - m41 * m32);
  d13 = (m31 * m43 - m41 * m33);
  d23 = (m32 * m43 - m42 * m33);
  d24 = (m32 * m44 - m42 * m34);
  d34 = (m33 * m44 - m43 * m34);
  d41 = (m34 * m41 - m44 * m31);
  tmp[0] = (m22 * d34 - m23 * d24 + m24 * d23);
  tmp[1] = -(m21 * d34 + m23 * d41 + m24 * d13);
  tmp[2] = (m21 * d24 + m22 * d41 + m24 * d12);
  tmp[3] = -(m21 * d23 - m22 * d13 + m23 * d12);

   //* Compute determinant as early as possible using these cofactors. */
  det = m11 * tmp[0] + m12 * tmp[1] + m13 * tmp[2] + m14 * tmp[3];
   //* Run singularity test. */
  if (det == 0.0)
  {
    ;
  }
  else
  {
    invDet = 1.0 / det;
    tmp[0] = tmp[0] * invDet;
    tmp[1] = tmp[1] * invDet;
    tmp[2] = tmp[2] * invDet;
    tmp[3] = tmp[3] * invDet;

    tmp[4] = -(m12 * d34 - m13 * d24 + m14 * d23) * invDet;
    tmp[5] = (m11 * d34 + m13 * d41 + m14 * d13) * invDet;
    tmp[6] = -(m11 * d24 + m12 * d41 + m14 * d12) * invDet;
    tmp[7] = (m11 * d23 - m12 * d13 + m13 * d12) * invDet;

    // * Pre - compute 2 x2 dets for first two rows when computing * /
    // * cofactors of last two rows. * /
    d12 = m11 * m22 - m21 * m12;
    d13 = m11 * m23 - m21 * m13;
    d23 = m12 * m23 - m22 * m13;
    d24 = m12 * m24 - m22 * m14;
    d34 = m13 * m24 - m23 * m14;
    d41 = m14 * m21 - m24 * m11;

    tmp[8] = (m42 * d34 - m43 * d24 + m44 * d23) * invDet;
    tmp[9] = -(m41 * d34 + m43 * d41 + m44 * d13) * invDet;
    tmp[10] = (m41 * d24 + m42 * d41 + m44 * d12) * invDet;
    tmp[11] = -(m41 * d23 - m42 * d13 + m43 * d12) * invDet;
    tmp[12] = -(m32 * d34 - m33 * d24 + m34 * d23) * invDet;
    tmp[13] = (m31 * d34 + m33 * d41 + m34 * d13) * invDet;
    tmp[14] = -(m31 * d24 + m32 * d41 + m34 * d12) * invDet;
    tmp[15] = (m31 * d23 - m32 * d13 + m33 * d12) * invDet;
    for(i=0;i<=15;i++)
      o[i] = tmp[i];
  }
}

void HuangzbPlot3DFormattedReaderClass::ComputeNormalVector(bool & Bup,bool& Bdown,bool & Bleft,bool& Bright,bool &Bfront,bool & Bback,int colorPara[])//: array of integer);
{
  const float  zero = 0.001;
  typedef double F4double[4];
  typedef double F16double[16];
  F4double up, down, left, right, front, back, compareVector, tempup, tempdown, templeft, tempright, tempfront, tempback, tempcompareVector;
  F16double modelview, modelview2;
  double angleUp, angleDown, angleLeft, angleRight, angleFront, angleBack;
  int i, j, k, Isort;
  std::vector<double>* SortAngle= new std::vector<double>();

  up[0] = 0; up[1] = 0; up[2] = 1; up[3] = 1; //{0,1,0,1};//上
  down[0] = 0; down[1] = 0; down[2] = -1; down[3] = 1; //down[4]={0,-1,0,1};//下
  left[0] = 0; left[1] = -1; left[2] = 0; left[3] = 1; //left[4]={-1,0,0,1};//
  right[0] = 0; right[1] = 1; right[2] = 0; right[3] = 1; //right[4]={1,0,0,1};
  front[0] = 1; front[1] = 0; front[2] = 0; front[3] = 1; //front[4]={0,0,1,1};
  back[0] = -1; back[1] = 0; back[2] = 0; back[3] = 1; //back[4]={0,0,-1,1};

  compareVector[0] = 0; compareVector[1] = 0; compareVector[2] = 1; compareVector[3] = 1; //compareVector[4]={0,0,1,1};

  tempup[0] = 0; tempup[1] = 0; tempup[2] = 0; tempup[3] = 0; //tempup[4]={0,0,0,0};
  tempdown[0] = 0; tempdown[1] = 0; tempdown[2] = 0; tempdown[3] = 0; //tempup[4]={0,0,0,0};
  templeft[0] = 0; templeft[1] = 0; templeft[2] = 0; templeft[3] = 0; //tempup[4]={0,0,0,0};
  tempright[0] = 0; tempright[1] = 0; tempright[2] = 0; tempright[3] = 0; //tempup[4]={0,0,0,0};
  tempfront[0] = 0; tempfront[1] = 0; tempfront[2] = 0; tempfront[3] = 0; //tempup[4]={0,0,0,0};
  tempback[0] = 0; tempback[1] = 0; tempback[2] = 0; tempback[3] = 0; //tempup[4]={0,0,0,0};

  //C3DPoint p1;
  //for i := 0 to 15 do modelview[i] := 0;
  for(i=0;i<=15;i++) modelview[i] =0;

  glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
  modelview[12] = 0;
  modelview[13] = 0;
  modelview[14] = 0;
  modelview[15] = 0;
  Bup = FALSE;
  Bdown = FALSE;
  Bfront = FALSE;
  Bback = FALSE;
  Bleft = FALSE;
  Bright = FALSE;

  //for j := 0 to 3 do
  for(j=0;j<4;j++)
  {
    //for i := 0 to 3 do
    for(i=0;i<4;i++)
    {
      tempup[j] = tempup[j] + modelview[j + 4 * i] * up[i];
      tempdown[j] = tempdown[j] + modelview[j + 4 * i] * down[i];
      templeft[j] = templeft[j] + modelview[j + 4 * i] * left[i];
      tempright[j] = tempright[j] + modelview[j + 4 * i] * right[i];
      tempfront[j] = tempfront[j] + modelview[j + 4 * i] * front[i];
      tempback[j] = tempback[j] + modelview[j + 4 * i] * back[i];
      //tempcompareVector[j]:=tempcompareVector[j]+_matrixAxes[j+4*i]*compareVector[i];
    }
  }

  //for k := 0 to 3 do
  for(k=0;k<4;k++)
  {
    up[k] = tempup[k];
    down[k] = tempdown[k];
    left[k] = templeft[k];
    right[k] = tempright[k];
    front[k] = tempfront[k];
    back[k] = tempback[k];
    //compareVector[k]:=tempcompareVector[k];
  }

  angleUp = AngleOfTwoVector(up, compareVector);
  angleDown = AngleOfTwoVector(down, compareVector);
  angleLeft = AngleOfTwoVector(left, compareVector);
  angleRight = AngleOfTwoVector(right, compareVector);
  angleFront = AngleOfTwoVector(front, compareVector);
  angleBack = AngleOfTwoVector(back, compareVector);

  if (angleUp < 130)  colorPara[0] = 152;
  else if (angleUp < 150)  colorPara[0] = 243;
  else colorPara[0] = 250;

  if (angleDown < 130)  colorPara[1] = 152;
  else if (angleDown < 150)  colorPara[1] = 243;
  else colorPara[1] = 250;

  if (angleLeft < 130)  colorPara[2] = 152;
  else if (angleLeft < 150)  colorPara[2] = 243;
  else colorPara[2] = 250;

  if (angleRight < 130)  colorPara[3] = 152;
  else if (angleRight < 150)  colorPara[3] = 243;
  else colorPara[3] = 250;

  if (angleFront < 130)  colorPara[4] = 152;
  else if (angleFront < 150)  colorPara[4] = 243;
  else colorPara[4] = 250;

  if (angleBack < 130)  colorPara[5] = 152;
  else if (angleBack < 150)  colorPara[5] = 243;
  else colorPara[5] = 250;


  SortAngle->push_back(angleUp);
  SortAngle->push_back(angleDown);
  SortAngle->push_back(angleLeft);
  SortAngle->push_back(angleRight);
  SortAngle->push_back(angleFront);
  SortAngle->push_back(angleBack);

  //SortAngle->Sort(compareSortAngle);

  //for Isort := 0 to 5 do
  for(Isort=0;Isort<=5;Isort++)
  {
    ;//applog.Debug('Isort=' + IntToStr(iSort) + ' SortAngle[Isort]=' + FloatToStr(Double(SortAngle.Items[Isort]^)));
  }

  if (((angleUp < SortAngle->at(3) + zero) && (angleUp > SortAngle->at(3) - zero))
    || ((angleUp < SortAngle->at(4) + zero) && (angleUp > (SortAngle->at(4) - zero)))
    || ((angleUp < SortAngle->at(5) + zero) && (angleUp > (SortAngle->at(5) - zero)) ))
    Bup = true;
/*临时注释
  if (((angleDown < *(double*)(SortAngle->at[3]) + zero) && (angleDown > *(double*)(SortAngle->at[3]) - zero))
    || ((angleDown < *(double*)(SortAngle->at[4]) + zero) && (angleDown > *(double*)(SortAngle->at[4]) - zero))
    || ((angleDown < *(double*)(SortAngle->at[5]) + zero) && (angleDown > *(double*)(SortAngle->at[5]) - zero)) )
    Bdown = true;

  if (((angleLeft < *(double*)(SortAngle->at[3]) + zero) && (angleLeft > *(double*)(SortAngle->at[3]) - zero))
    || ((angleLeft < *(double*)(SortAngle->at[4]) + zero) && (angleLeft > *(double*)(SortAngle->at[4]) - zero))
    || ((angleLeft < *(double*)(SortAngle->at[5]) + zero) && (angleLeft > *(double*)(SortAngle->at[5]) - zero)) )
    Bleft = true;

  if (((angleRight < *(double*)(SortAngle->at[3]) + zero) && (angleRight > *(double*)(SortAngle->at[3]) - zero))
    || ((angleRight < *(double*)(SortAngle->at[4]) + zero) && (angleRight > *(double*)(SortAngle->at[4]) - zero))
    || ((angleRight < *(double*)(SortAngle->at[5]) + zero) && (angleRight > *(double*)(SortAngle->at[5]) - zero)) )
    Bright = true;

  if (((angleFront < *(double*)(SortAngle->at[3]) + zero) && (angleFront > *(double*)(SortAngle->at[3]) - zero))
    || ((angleFront < *(double*)(SortAngle->at[4]) + zero) && (angleFront > *(double*)(SortAngle->at[4]) - zero))
    || ((angleFront < *(double*)(SortAngle->at[5]) + zero) && (angleFront > *(double*)(SortAngle->at[5]) - zero)) )
    Bfront = true;

  if (((angleBack < *(double*)(SortAngle->at[3]) + zero) && (angleBack > *(double*)(SortAngle->at[3]) - zero))
    || ((angleBack < *(double*)(SortAngle->at[4]) + zero) && (angleBack > *(double*)(SortAngle->at[4]) - zero))
    || ((angleBack < *(double*)(SortAngle->at[5]) + zero) && (angleBack > *(double*)(SortAngle->at[5]) - zero)) )
    Bback = true;
*/
//  SortAngle->Free();
}

void HuangzbPlot3DFormattedReaderClass::DrawBoxNew()
{
  const double  delt = 0.01;

  int i, j, k, q;
  int index;
  int u, v, w;
  int x, y, z;
  bool bSkipOne;
  int XtmpStart, XtmpEnd, YtmpStart, YtmpEnd, ZtmpStart, ZtmpEnd, tmp;
  bool Bup, Bdown, Bleft, Bright, Bfront, Bback;
  int colorPara[6];//: array[0..5] of integer;
  int colorup, colordown, colorleft, colorright, colorfront, colorback;
  double offy, offx, offz;

  //applog.Debug('DrawBoxNew.nBaseRange='+FloatToStr(nBaseRange));
  ComputeNormalVector(Bup, Bdown, Bleft, Bright, Bfront, Bback, colorPara);

  colorup = colorPara[0];
  colordown = colorPara[1];
  colorleft = colorPara[2];
  colorright = colorPara[3];
  colorfront = colorPara[4];
  colorback = colorPara[5];
  offx = FXMax - FXMin;
  offy = FYMax - FYMin;
  offz = FZMax - FZMin;

  glColor3ub(0, 0, 0);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glLineWidth(2);
  //if (cparam.bShowBox)
  if (1)
  {
    glLineWidth(2);
    if (Bup)
    {
      //applog.Debug('DrawBoxNew.Bup.nBaseRange='+FloatToStr(nBaseRange));
      glBegin(GL_QUADS); //上表面
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //C
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D1
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //c1
      glEnd();

      glLineWidth(1);
      glBegin(GL_LINES);
      //for i := 0 to 5 do
      for(i=0;i<=5;i++)
      {
        glVertex3f((FXMin + delt + offx * i / 6) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange);
        glVertex3f((FXMin - delt + offx * i / 6) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange);

        glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt + offy * i / 6) / nBaseRange, (FZMax + delt) / nBaseRange); //C
        glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt + offy * i / 6) / nBaseRange, (FZMax + delt) / nBaseRange); //C

      }
      glEnd();
    }
    glLineWidth(2);
    if (Bdown)
    {
      glBegin(GL_QUADS); //下表面
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B1
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A1
      glEnd();
      glLineWidth(1);
      glBegin(GL_LINES);
      //for i := 0 to 5 do
      for(i=0;i<=5;i++)
      {
        glVertex3f((FXMin - delt + offx * i / 6) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
        glVertex3f((FXMin - delt + offx * i / 6) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A

        glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt + offy * i / 6) / nBaseRange, (FZMin - delt) / nBaseRange); //A
        glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt + offy * i / 6) / nBaseRange, (FZMin - delt) / nBaseRange); //A
      }
      glEnd();
    }
    glLineWidth(2);
    if (Bleft)
    {
      glBegin(GL_QUADS); //左表面
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //C
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D
      glEnd();
      glLineWidth(1);
      glBegin(GL_LINES);
      //for i := 0 to 5 do
      for(i=0;i<=5;i++)
      {
        glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt + offz * i / 6) / nBaseRange); //A
        glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt + offz * i / 6) / nBaseRange); //A

        glVertex3f((FXMin - delt + offx * i / 6) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
        glVertex3f((FXMin - delt + offx * i / 6) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //A
      }
      glEnd();
    }
    glLineWidth(2);
    if (Bright)
    {
      glBegin(GL_QUADS); //右表面
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A1
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B1
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //c1
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D1
      glEnd();
      glLineWidth(1);
      glBegin(GL_LINES);
      //for i := 0 to 5 do
      for(i=0;i<=5;i++)
      {
        glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt + offz * i / 6) / nBaseRange); //A
        glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt + offz * i / 6) / nBaseRange); //A

        glVertex3f((FXMin - delt + offx * i / 6) / nBaseRange, (FYMax - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
        glVertex3f((FXMin - delt + offx * i / 6) / nBaseRange, (FYMax - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //A
      }
      glEnd();

    }
    glLineWidth(2);
    if (Bfront) 
    {
      glBegin(GL_QUADS); //前表面
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //C
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //c1
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B1
      glEnd();
      glLineWidth(1);
      glBegin(GL_LINES);
      //for i := 0 to 5 do
      for(i=0;i<=5;i++)
      {
        glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt + offz * i / 6) / nBaseRange); //B
        glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt + offz * i / 6) / nBaseRange); //B

        glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt + offy * i / 6) / nBaseRange, (FZMin - delt) / nBaseRange); //B
        glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt + offy * i / 6) / nBaseRange, (FZMax + delt) / nBaseRange); //B

      }
      glEnd();
    }
    glLineWidth(2);
    if (Bback)
    {
      glBegin(GL_QUADS); //后表面
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D1
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A1
      glEnd();
      glLineWidth(1);
      glBegin(GL_LINES);
      //for i := 0 to 5 do
      for(i=0;i<=5;i++)
      {
        glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt + offz * i / 6) / nBaseRange); //B
        glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt + offz * i / 6) / nBaseRange); //B

        glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt + offy * i / 6) / nBaseRange, (FZMin - delt) / nBaseRange); //B
        glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt + offy * i / 6) / nBaseRange, (FZMax + delt) / nBaseRange); //B
      }
      glEnd();

    }
  }
  glLineWidth(1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glEnable(GL_POLYGON_OFFSET_FILL);
  glPolygonOffset(0.9, 0.9);
  //if (cparam.bShowBox)
  if (1)
  {
    if (Bup)
    {
      glColor3ub(colorup, colorup, colorup);
      glBegin(GL_QUADS); //上表面
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //C
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D1
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //c1
      glEnd();
    }

    if (Bdown)
    {
      glColor3ub(colordown, colordown, colordown);
      glBegin(GL_QUADS); //下表面
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B1
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A1
      glEnd();
    }

    if (Bleft)
    {
      glColor3ub(colorleft, colorleft, colorleft);
      glBegin(GL_QUADS); //左表面
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //C
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D
      glEnd();
    }

    if (Bright)
    {
      glColor3ub(colorright, colorright, colorright);
      glBegin(GL_QUADS); //右表面
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A1
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B1
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //c1
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D1
      glEnd();
    }

    if (Bfront)
    {
      glColor3ub(colorfront, colorfront, colorfront);
      glBegin(GL_QUADS); //前表面
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B
      glVertex3f((FXMax + delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //C
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //c1
      glVertex3f((FXMax + delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //B1
      glEnd();
    }

    if (Bback)
    {
      glColor3ub(colorback, colorback, colorback);
      glBegin(GL_QUADS); //后表面
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A
      glVertex3f((FXMin - delt) / nBaseRange, (FYMin - delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMax + delt) / nBaseRange); //D1
      glVertex3f((FXMin - delt) / nBaseRange, (FYMax + delt) / nBaseRange, (FZMin - delt) / nBaseRange); //A1
      glEnd();
    }
  }
  glPolygonOffset(-0.9, -0.9);
  glDisable(GL_POLYGON_OFFSET_FILL);

  IsDrawing = false;
}

void HuangzbPlot3DFormattedReaderClass::DrawColorTable()
{


}

void HuangzbPlot3DFormattedReaderClass::DrawBox()
{
  int i, j, k, q;
  int index;
  int u, v, w;
  int x, y, z;
  bool bSkipOne;
  int XtmpStart, XtmpEnd, YtmpStart, YtmpEnd, ZtmpStart, ZtmpEnd, tmp;

  glColor3ub(0, 0, 0);
  glLineWidth(1);
  //if (cparam.bShowBox)
  if (1)
  {
    //画一个包围盒
    glBegin(GL_LINE_LOOP);
    glVertex3f((FXMin - 0.001) / nBaseRange, (FYMin - 0.001) / nBaseRange, (FZMin - 0.001) / nBaseRange); //A
    glVertex3f((FXMax + 0.001) / nBaseRange, (FYMin - 0.001) / nBaseRange, (FZMin - 0.001) / nBaseRange); //B
    glVertex3f((FXMax + 0.001) / nBaseRange, (FYMin - 0.001) / nBaseRange, (FZMax + 0.001) / nBaseRange); //C
    glVertex3f((FXMin - 0.001) / nBaseRange, (FYMin - 0.001) / nBaseRange, (FZMax + 0.001) / nBaseRange); //D
    glEnd();


    glBegin(GL_LINE_LOOP);
    glVertex3f((FXMin - 0.001) / nBaseRange, (FYMax + 0.001) / nBaseRange, (FZMin - 0.001) / nBaseRange); //A1
    glVertex3f((FXMax + 0.001) / nBaseRange, (FYMax + 0.001) / nBaseRange, (FZMin - 0.001) / nBaseRange); //B1
    glVertex3f((FXMax + 0.001) / nBaseRange, (FYMax + 0.001) / nBaseRange, (FZMax + 0.001) / nBaseRange); //c1
    glVertex3f((FXMin - 0.001) / nBaseRange, (FYMax + 0.001) / nBaseRange, (FZMax + 0.001) / nBaseRange); //D1
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f((FXMin - 0.001) / nBaseRange, (FYMin - 0.001) / nBaseRange, (FZMin - 0.001) / nBaseRange); //A
    glVertex3f((FXMax + 0.001) / nBaseRange, (FYMin - 0.001) / nBaseRange, (FZMin - 0.001) / nBaseRange); //B
    glVertex3f((FXMax + 0.001) / nBaseRange, (FYMax + 0.001) / nBaseRange, (FZMin - 0.001) / nBaseRange); //B1
    glVertex3f((FXMin - 0.001) / nBaseRange, (FYMax + 0.001) / nBaseRange, (FZMin - 0.001) / nBaseRange); //A1
    glEnd();

    glBegin(GL_LINE_LOOP);
    glVertex3f((FXMin - 0.001) / nBaseRange, (FYMin - 0.001) / nBaseRange, (FZMax + 0.001) / nBaseRange); //D
    glVertex3f((FXMax + 0.001) / nBaseRange, (FYMin - 0.001) / nBaseRange, (FZMax + 0.001) / nBaseRange); //C
    glVertex3f((FXMax + 0.001) / nBaseRange, (FYMax + 0.001) / nBaseRange, (FZMax + 0.001) / nBaseRange); //c1
    glVertex3f((FXMin - 0.001) / nBaseRange, (FYMax + 0.001) / nBaseRange, (FZMax + 0.001) / nBaseRange); //D1
    glEnd();
  }
  IsDrawing = false;
}

void HuangzbPlot3DFormattedReaderClass::DrawGrid()
{
  int i, j, k, q;
  int index;
  int u, v, w;
  int x, y, z;
  bool bSkipOne;
  int XtmpStart, XtmpEnd, YtmpStart, YtmpEnd, ZtmpStart, ZtmpEnd, tmp;
  if (!this->IsLoaded) return;

  /////////////////////////////////////////////////////////////////////////////
  if (globalcfg.cparam.bShowShader)
  {
    //glClearColor((255.0), (255.0), (255.0), 1.0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_SMOOTH);
    IsDrawing = true;
    glLineWidth(1);
    if (globalcfg.cparam.bFlatOrSmooth) glShadeModel(GL_SMOOTH); else glShadeModel(GL_FLAT);
	//if (1) glShadeModel(GL_SMOOTH); else glShadeModel(GL_FLAT);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.9, 0.9);
    //画主体
    glDisable(GL_TEXTURE_2D);
    glInitNames();
    if (globalcfg.cparam.bKDir)
    {
      //for q := 0 to FFenQu - 1 do
      for(q=0;q<=FFenQu-1;++q)
      {
        if (!FFenQuAry[q].bKDir)  continue;
        if (! FFenQuAry[q].IsDraw ) continue;
        if (FFenQuAry[q].bSelected)
          glColor3ub(255, 255, 255);
        else
          glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
        glColor3ub(255, 255, 255);
       for( i = FFenQuAry[q].nIBegin;i<=FFenQuAry[q].nIEnd - 1;i++)
          for( j = FFenQuAry[q].nJBegin;j<=FFenQuAry[q].nJEnd - 1 ;j++)
            for( k = FFenQuAry[q].nKBegin;k<=FFenQuAry[q].nKEnd;k++)
            {
              if (! ((k < 1) || ((k % FFenQuAry[q].nKSkip) == 0)))  continue;
              glPushName(CreateglName(q, i, j, k));
              glBegin(GL_QUADS);
              glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
                GetXYZ(q, i, j, k).Y / nBaseRange,
                GetXYZ(q, i, j, k).Z / nBaseRange);

              glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
                GetXYZ(q, i + 1, j, k).Y / nBaseRange,
                GetXYZ(q, i + 1, j, k).Z / nBaseRange);

              glVertex3f(GetXYZ(q, i + 1, j + 1, k).X / nBaseRange,
                GetXYZ(q, i + 1, j + 1, k).Y / nBaseRange,
                GetXYZ(q, i + 1, j + 1, k).Z / nBaseRange);

              glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
                GetXYZ(q, i, j + 1, k).Y / nBaseRange,
                GetXYZ(q, i, j + 1, k).Z / nBaseRange);

              glEnd();
              glPopName();
              glLineWidth(1);
            }
      }
    }

    if (globalcfg.cparam.bIDir)
    {
      //画边界面I(X)切面的边界
      //for q := 0 to FFenQu - 1 do
      for(q=0;q<=FFenQu - 1;q++)
      {
        if (! FFenQuAry[q].bIDir)  continue;
        if (!FFenQuAry[q].bIDir)  continue;
        if (! FFenQuAry[q].IsDraw)  continue;
        if (FFenQuAry[q].bSelected) 
          glColor3ub(255, 255, 255);
        else
          glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    //glPushName(q);
        glColor3ub(255, 255, 255);
        for (i = FFenQuAry[q].nIBegin;i<=FFenQuAry[q].nIEnd ;i++)
          for (j = FFenQuAry[q].nJBegin;j<=FFenQuAry[q].nJEnd - 1 ;j++)
            for(k = FFenQuAry[q].nKBegin;k<=FFenQuAry[q].nKEnd - 1 ;k++)
            {
              if (! ((i < 1) || ((i % FFenQuAry[q].nISkip) == 0))) continue;
              glPushName(CreateglName(q, i, j, k));
              glBegin(GL_QUADS);
              glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
                GetXYZ(q, i, j, k).Y / nBaseRange,
                GetXYZ(q, i, j, k).Z / nBaseRange);

              glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
                GetXYZ(q, i, j + 1, k).Y / nBaseRange,
                GetXYZ(q, i, j + 1, k).Z / nBaseRange);

              glVertex3f(GetXYZ(q, i, j + 1, k + 1).X / nBaseRange,
                GetXYZ(q, i, j + 1, k + 1).Y / nBaseRange,
                GetXYZ(q, i, j + 1, k + 1).Z / nBaseRange);

              glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
                GetXYZ(q, i, j, k + 1).Y / nBaseRange,
                GetXYZ(q, i, j, k + 1).Z / nBaseRange);

              glEnd();
              glPopName();
              glLineWidth(1);
            }
      }
    }

    if (globalcfg.cparam.bJDir)
    {
  //画边界面J(Y)切面的边界
      for( q = 0;q<=FFenQu - 1;++q)
      {
        if  (!FFenQuAry[q].bJDir)  continue;
        if (! FFenQuAry[q].IsDraw)  continue;
        if (FFenQuAry[q].bSelected)
          glColor3ub(255, 255, 255);
        else
          glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
        glColor3ub(255, 255, 255);
        for( i = FFenQuAry[q].nIBegin;i<=FFenQuAry[q].nIEnd - 1 ;i++)
          for (j = FFenQuAry[q].nJBegin;j<=FFenQuAry[q].nJEnd ;j++)
            for( k = FFenQuAry[q].nKBegin;k<=FFenQuAry[q].nKEnd - 1;k++)
            {
              if (! ((j < 1) || ((j % FFenQuAry[q].nJSkip) == 0)))  continue;
              glPushName(CreateglName(q, i, j, k));
              glBegin(GL_QUADS);
              glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
                GetXYZ(q, i, j, k).Y / nBaseRange,
                GetXYZ(q, i, j, k).Z / nBaseRange);

              glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
                GetXYZ(q, i, j, k + 1).Y / nBaseRange,
                GetXYZ(q, i, j, k + 1).Z / nBaseRange);


              glVertex3f(GetXYZ(q, i + 1, j, k + 1).X / nBaseRange,
                GetXYZ(q, i + 1, j, k + 1).Y / nBaseRange,
                GetXYZ(q, i + 1, j, k + 1).Z / nBaseRange);

              glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
                GetXYZ(q, i + 1, j, k).Y / nBaseRange,
                GetXYZ(q, i + 1, j, k).Z / nBaseRange);

              glEnd();
              glPopName();
              glLineWidth(1);
            }
      }
    }
    glDisable(GL_POLYGON_SMOOTH);
    glPolygonOffset(-0.9, -0.9);
    glDisable(GL_POLYGON_OFFSET_FILL);
  }
  /////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////
  //FFenQuAry[q].nKSkip := FFenQuAry[q].nKEnd DIV 5;
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_POLYGON_SMOOTH);
  IsDrawing = true;
  glLineWidth(1);
  glEnable(GL_DEPTH);
  if (globalcfg.cparam.bFlatOrSmooth) glShadeModel(GL_SMOOTH); else glShadeModel(GL_FLAT);
  //if (IsRltLoaded) then InitColorMap(FRltInfo.VarnameList[curVarIdx].VarValueMin, FRltInfo.VarnameList[curVarIdx].VarValueMax);
  if (! globalcfg.cparam.bShowGrid) return;
  glColor3ub(255, 0, 0);
    //画主体
  glInitNames();
  if (globalcfg.cparam.bKDir)
  if (1)
  {
    for (q = 0;q<=FFenQu - 1;q++)
    {
      if (!FFenQuAry[q].bKDir)  continue;
      if (! FFenQuAry[q].IsDraw)  continue;
      if (FFenQuAry[q].bSelected)
        glColor3ub(255, 255, 255);
      else
        glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    //glPushName(q);
      for (i = FFenQuAry[q].nIBegin;i<=FFenQuAry[q].nIEnd - 1;i++)
        for (j = FFenQuAry[q].nJBegin;j<=FFenQuAry[q].nJEnd - 1;j++)
          for (k = FFenQuAry[q].nKBegin;k<=FFenQuAry[q].nKEnd;k++)
          {
            if (! ((k < 1) || ((k % FFenQuAry[q].nKSkip) == 0)))  continue;
            glPushName(CreateglName(q, i, j, k));
            glBegin(GL_QUADS);
            glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
              GetXYZ(q, i, j, k).Y / nBaseRange,
              GetXYZ(q, i, j, k).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
              GetXYZ(q, i + 1, j, k).Y / nBaseRange,
              GetXYZ(q, i + 1, j, k).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i + 1, j + 1, k).X / nBaseRange,
              GetXYZ(q, i + 1, j + 1, k).Y / nBaseRange,
              GetXYZ(q, i + 1, j + 1, k).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
              GetXYZ(q, i, j + 1, k).Y / nBaseRange,
              GetXYZ(q, i, j + 1, k).Z / nBaseRange);

            glEnd();
            glPopName();
            glLineWidth(1);
          }

    }
  }

  if (globalcfg.cparam.bIDir) 
  {
  //画边界面I(X)切面的边界
    for( q = 0;q<=FFenQu - 1 ;q++)
    {
      if (!FFenQuAry[q].bIDir)  continue;
      if (! FFenQuAry[q].IsDraw)  continue;
      if (FFenQuAry[q].bSelected)
        glColor3ub(255, 255, 255);
      else
        glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    //glPushName(q);
      for( i = FFenQuAry[q].nIBegin;i<=FFenQuAry[q].nIEnd;i++)
        for( j = FFenQuAry[q].nJBegin;j<=FFenQuAry[q].nJEnd - 1 ;j++)
          for (k = FFenQuAry[q].nKBegin;k<= FFenQuAry[q].nKEnd - 1;k++)
          {
            if (! ((i < 1) || ((i % FFenQuAry[q].nISkip) == 0)))  continue;
            glPushName(CreateglName(q, i, j, k));
            glBegin(GL_QUADS);
            glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
              GetXYZ(q, i, j, k).Y / nBaseRange,
              GetXYZ(q, i, j, k).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
              GetXYZ(q, i, j + 1, k).Y / nBaseRange,
              GetXYZ(q, i, j + 1, k).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i, j + 1, k + 1).X / nBaseRange,
              GetXYZ(q, i, j + 1, k + 1).Y / nBaseRange,
              GetXYZ(q, i, j + 1, k + 1).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
              GetXYZ(q, i, j, k + 1).Y / nBaseRange,
              GetXYZ(q, i, j, k + 1).Z / nBaseRange);

            glEnd();
            glPopName();
            glLineWidth(1);
          }
    }
  }

  if (globalcfg.cparam.bJDir)
  {
  //画边界面J(Y)切面的边界
    for (q = 0;q<=FFenQu - 1;q++)
    {
      if (!FFenQuAry[q].bJDir)  continue;
      if (! FFenQuAry[q].IsDraw)  continue;
      if (FFenQuAry[q].bSelected)
        glColor3ub(255, 255, 255);
      else
        glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    //glPushName(q);
    //glScale(1+q*0.1,1+q*0.1,1+q*0.1);
      for (i = FFenQuAry[q].nIBegin ;i<=FFenQuAry[q].nIEnd - 1;i++)
        for( j = FFenQuAry[q].nJBegin;j<= FFenQuAry[q].nJEnd ;j++)
          for (k = FFenQuAry[q].nKBegin;k<=FFenQuAry[q].nKEnd - 1;k++)
          {
            if (! ((j < 1) || ((j % FFenQuAry[q].nJSkip) == 0)))  continue;
            glPushName(CreateglName(q, i, j, k));
            glBegin(GL_QUADS);
            glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
              GetXYZ(q, i, j, k).Y / nBaseRange,
              GetXYZ(q, i, j, k).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
              GetXYZ(q, i, j, k + 1).Y / nBaseRange,
              GetXYZ(q, i, j, k + 1).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i + 1, j, k + 1).X / nBaseRange,
              GetXYZ(q, i + 1, j, k + 1).Y / nBaseRange,
              GetXYZ(q, i + 1, j, k + 1).Z / nBaseRange);

            glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
              GetXYZ(q, i + 1, j, k).Y / nBaseRange,
              GetXYZ(q, i + 1, j, k).Z / nBaseRange);

            glEnd();
            glPopName();
            glLineWidth(1);
          }
    }
  }
/*  //FbInpFound
  bSkipOne = False;
  if (FbInpFound) and (FbCheckOk) then
  begin
    //绘制
    for i := Low(EdgeInfo.EdgeEntryListOfFenQue) to High(EdgeInfo.EdgeEntryListOfFenQue) do
    begin
      for j := Low(EdgeInfo.EdgeEntryListOfFenQue[i]) to High(EdgeInfo.EdgeEntryListOfFenQue[i]) do
      begin
        if not EdgeInfo.EdgeEntryListOfFenQue[i][j].IsDraw then continue;
        glPushName(1000 + EdgeInfo.EdgeEntryListOfFenQue[i][j].INo);
        if (EdgeInfo.EdgeEntryListOfFenQue[i][j].EntryType = -1) then begin EdgeInfo.EdgeEntryListOfFenQue[i][j].bDisable := True; bSkipOne := True; continue; end;
        if (bSkipOne) then begin EdgeInfo.EdgeEntryListOfFenQue[i][j].bDisable := True; bSkipOne := False; continue; end;
        if (EdgeInfo.EdgeEntryListOfFenQue[i][j].bSelected) then
          glColor3ub(255, 255, 255)
        else
          glCOlor3ub(FenQuColor[EdgeInfo.EdgeEntryListOfFenQue[i][j].EntryType mod 27][0], FenQuColor[EdgeInfo.EdgeEntryListOfFenQue[i][j].EntryType mod 27][1], FenQuColor[EdgeInfo.EdgeEntryListOfFenQue[i][j].EntryType mod 27][2]);
        XtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].IStart) - 1;
        XtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].IEnd) - 2;
        if ((XtmpStart - 1) > XtmpEnd) then begin tmp := XtmpStart - 1; XtmpStart := XtmpEnd + 1; XtmpEnd := tmp; end;
        YtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].JStart) - 1;
        YtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].JEnd) - 1;
        if (YtmpStart > YtmpEnd) then begin tmp := YtmpStart; YtmpStart := YtmpEnd; YtmpEnd := tmp; end;
        ZtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].KStart) - 1;
        ZtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].KEnd) - 2;
        if ((ZtmpStart - 1) > ZtmpEnd) then begin tmp := ZtmpStart - 1; ZtmpStart := ZtmpEnd + 1; ZtmpEnd := tmp; end;
        for y := YtmpStart to YtmpEnd do
          for x := XtmpStart to XtmpEnd do
          //for y := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].JStart) - 1 to abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].JEnd) - 1 do
            for z := ZtmpStart to ZtmpEnd do
            begin
              glBegin(GL_QUADS);
              glVertex3f(GetXYZ(i, x, y, z).X / nBaseRange,
                GetXYZ(i, x, y, z).Y / nBaseRange,
                GetXYZ(i, x, y, z).Z / nBaseRange);

              glVertex3f(GetXYZ(i, x, y, z + 1).X / nBaseRange,
                GetXYZ(i, x, y, z + 1).Y / nBaseRange,
                GetXYZ(i, x, y, z + 1).Z / nBaseRange);


              glVertex3f(GetXYZ(i, x + 1, y, z + 1).X / nBaseRange,
                GetXYZ(i, x + 1, y, z + 1).Y / nBaseRange,
                GetXYZ(i, x + 1, y, z + 1).Z / nBaseRange);

              glVertex3f(GetXYZ(i, x + 1, y, z).X / nBaseRange,
                GetXYZ(i, x + 1, y, z).Y / nBaseRange,
                GetXYZ(i, x + 1, y, z).Z / nBaseRange);
              glEnd();
            end;

        XtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].IStart) - 1;
        XtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].IEnd) - 2;
        if ((XtmpStart - 1) > XtmpEnd) then begin tmp := XtmpStart - 1; XtmpStart := XtmpEnd + 1; XtmpEnd := tmp; end;
        YtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].JStart) - 1;
        YtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].JEnd) - 2;
        if ((YtmpStart - 1) > YtmpEnd) then begin tmp := YtmpStart - 1; YtmpStart := YtmpEnd + 1; YtmpEnd := tmp; end;
        ZtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].KStart) - 1;
        ZtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].KEnd) - 1;
        if (ZtmpStart > ZtmpEnd) then begin tmp := ZtmpStart; ZtmpStart := ZtmpEnd + 1; ZtmpEnd := tmp; end;

        for z := ZtmpStart to ZtmpEnd do
          for x := XtmpStart to XtmpEnd do
            for y := YtmpStart to YtmpEnd do
            //for z := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].KStart) - 1 to abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].KEnd) - 1 do
            begin
              glBegin(GL_QUADS);
              glVertex3f(GetXYZ(i, x, y, z).X / nBaseRange,
                GetXYZ(i, x, y, z).Y / nBaseRange,
                GetXYZ(i, x, y, z).Z / nBaseRange);

              glVertex3f(GetXYZ(i, x + 1, y, z).X / nBaseRange,
                GetXYZ(i, x + 1, y, z).Y / nBaseRange,
                GetXYZ(i, x + 1, y, z).Z / nBaseRange);

              glVertex3f(GetXYZ(i, x + 1, y + 1, z).X / nBaseRange,
                GetXYZ(i, x + 1, y + 1, z).Y / nBaseRange,
                GetXYZ(i, x + 1, y + 1, z).Z / nBaseRange);

              glVertex3f(GetXYZ(i, x, y + 1, z).X / nBaseRange,
                GetXYZ(i, x, y + 1, z).Y / nBaseRange,
                GetXYZ(i, x, y + 1, z).Z / nBaseRange);
              glEnd();
            end;

        XtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].IStart) - 1;
        XtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].IEnd) - 1;
        if ((XtmpStart) > XtmpEnd) then begin tmp := XtmpStart; XtmpStart := XtmpEnd; XtmpEnd := tmp; end;
        YtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].JStart) - 1;
        YtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].JEnd) - 2;
        if ((YtmpStart - 1) > YtmpEnd) then begin tmp := YtmpStart - 1; YtmpStart := YtmpEnd + 1; YtmpEnd := tmp; end;
        ZtmpStart := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].KStart) - 1;
        ZtmpEnd := abs(EdgeInfo.EdgeEntryListOfFenQue[i][j].KEnd) - 2;
        if ((ZtmpStart - 1) > ZtmpEnd) then begin tmp := ZtmpStart - 1; ZtmpStart := ZtmpEnd + 1; ZtmpEnd := tmp; end;

        for x := XtmpStart to XtmpEnd do
          for y := YtmpStart to YtmpEnd do
            for z := ZtmpStart to ZtmpEnd do
            begin
              glBegin(GL_QUADS);
              glVertex3f(GetXYZ(i, x, y, z).X / nBaseRange,
                GetXYZ(i, x, y, z).Y / nBaseRange,
                GetXYZ(i, x, y, z).Z / nBaseRange);

              glVertex3f(GetXYZ(i, x, y + 1, z).X / nBaseRange,
                GetXYZ(i, x, y + 1, z).Y / nBaseRange,
                GetXYZ(i, x, y + 1, z).Z / nBaseRange);

              glVertex3f(GetXYZ(i, x, y + 1, z + 1).X / nBaseRange,
                GetXYZ(i, x, y + 1, z + 1).Y / nBaseRange,
                GetXYZ(i, x, y + 1, z + 1).Z / nBaseRange);

              glVertex3f(GetXYZ(i, x, y, z + 1).X / nBaseRange,
                GetXYZ(i, x, y, z + 1).Y / nBaseRange,
                GetXYZ(i, x, y, z + 1).Z / nBaseRange);
              glEnd();
            end;
        glPopName();
      end;
    end;
  end;
*/
  IsDrawing = false;
  glLineWidth(1);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void HuangzbPlot3DFormattedReaderClass::DrawBoundary()
{
  int i, j, k, q;
  int index;
  int u, v, w;
  int x, y, z;
  bool bSkipOne;
  int XtmpStart, XtmpEnd, YtmpStart, YtmpEnd, ZtmpStart, ZtmpEnd, tmp;

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_POLYGON_SMOOTH);
  glEnable(GL_DEPTH);
  glEnable(GL_POLYGON_SMOOTH);
  //画各分区的边界线
  //if (cparam.bShowEdge)
  if (1)
  {
    for( q = 0;q<=FFenQu - 1;q++)
    {
      glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
      for (i = 0 ;i<=FFenQuAry[q].IDotNums - 2;i++)
      {
        k = FFenQuAry[q].KDotNums - 1;
        j = FFenQuAry[q].JDotNums - 1;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
          GetXYZ(q, i + 1, j, k).Y / nBaseRange,
          GetXYZ(q, i + 1, j, k).Z / nBaseRange);
        glEnd();

        k = 0;
        j = 0;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
          GetXYZ(q, i + 1, j, k).Y / nBaseRange,
          GetXYZ(q, i + 1, j, k).Z / nBaseRange);
        glEnd();

        k = FFenQuAry[q].KDotNums - 1;
        j = 0;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
          GetXYZ(q, i + 1, j, k).Y / nBaseRange,
          GetXYZ(q, i + 1, j, k).Z / nBaseRange);
        glEnd();

        k = 0;
        j = FFenQuAry[q].JDotNums - 1;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
          GetXYZ(q, i + 1, j, k).Y / nBaseRange,
          GetXYZ(q, i + 1, j, k).Z / nBaseRange);
        glEnd();

      }

      for (j = 0;j<=FFenQuAry[q].JDotNums - 2;j++)
      {
        k = FFenQuAry[q].KDotNums - 1;
        i = FFenQuAry[q].IDotNums - 1;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
          GetXYZ(q, i, j + 1, k).Y / nBaseRange,
          GetXYZ(q, i, j + 1, k).Z / nBaseRange);
        glEnd();

        k = 0;
        i = 0;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
          GetXYZ(q, i, j + 1, k).Y / nBaseRange,
          GetXYZ(q, i, j + 1, k).Z / nBaseRange);
        glEnd();

        k = FFenQuAry[q].KDotNums - 1;
        i = 0;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
          GetXYZ(q, i, j + 1, k).Y / nBaseRange,
          GetXYZ(q, i, j + 1, k).Z / nBaseRange);
        glEnd();

        k = 0;
        i = FFenQuAry[q].IDotNums - 1;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
          GetXYZ(q, i, j + 1, k).Y / nBaseRange,
          GetXYZ(q, i, j + 1, k).Z / nBaseRange);
        glEnd();


      }

      for(k= 0;k<=FFenQuAry[q].KDotNums - 2;k++)
      {
        j = FFenQuAry[q].JDotNums - 1;
        i = FFenQuAry[q].IDotNums - 1;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
          GetXYZ(q, i, j, k + 1).Y / nBaseRange,
          GetXYZ(q, i, j, k + 1).Z / nBaseRange);
        glEnd();

        j = 0;
        i = 0;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
          GetXYZ(q, i, j, k + 1).Y / nBaseRange,
          GetXYZ(q, i, j, k + 1).Z / nBaseRange);
        glEnd();

        j = FFenQuAry[q].JDotNums - 1;
        i = 0;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
          GetXYZ(q, i, j, k + 1).Y / nBaseRange,
          GetXYZ(q, i, j, k + 1).Z / nBaseRange);
        glEnd();

        j = 0;
        i = FFenQuAry[q].IDotNums - 1;
        //glColor3f(1, 0, 0);
        glLineWidth(2);

        glBegin(GL_LINES);
        glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
          GetXYZ(q, i, j, k).Y / nBaseRange,
          GetXYZ(q, i, j, k).Z / nBaseRange);

        glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
          GetXYZ(q, i, j, k + 1).Y / nBaseRange,
          GetXYZ(q, i, j, k + 1).Z / nBaseRange);
        glEnd();

      }
    }
  }

  IsDrawing = false;
}

typedef  GLfloat GLTVector3[3];

void HuangzbPlot3DFormattedReaderClass::DrawContour()
{
  int i, j, k, q;
  int index;
  int u, v, w;
  int x, y, z;
  bool bSkipOne;
  int XtmpStart, XtmpEnd, YtmpStart, YtmpEnd, ZtmpStart, ZtmpEnd, tmp;
  GLTVector3 vNormal,vNormal1,vNormal2,vNormal3,vNormal4,vPoints1, vPoints2, vPoints3; // Storeage for calculated surface normal

  //if (! cparam.bShowContour) return;
  glEnable(GL_POLYGON_SMOOTH);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.9, 0.9);
  IsDrawing = true;
  glColor3ub(0, 0, 0);
  glLineWidth(1);
  //if (cparam.bFlatOrSmooth) glShadeModel(GL_SMOOTH); else glShadeModel(GL_FLAT);
  if (1) glShadeModel(GL_SMOOTH); else glShadeModel(GL_FLAT);
  if (curVarIdx == 0) InitColorMap(FXMin, FXMax);
  if (curVarIdx == 1) InitColorMap(FYMin, FYMax);
  if (curVarIdx == 2) InitColorMap(FZMin, FZMax);
  if (curVarIdx >= 3)
  {
    if (IsRltLoaded) InitColorMap(FRltInfo.VarnameList[curVarIdx - 3].VarValueMin, FRltInfo.VarnameList[curVarIdx - 3].VarValueMax);
  }
  //cparam.bReadyColorMap = true;
  //画主体
  glInitNames();
  //if (cparam.bKDir)
  if (1)
  {
    for (q = 0;q<=FFenQu - 1;q++)
    {
      if (!FFenQuAry[q].bKDir)  continue;
      if (! FFenQuAry[q].IsDraw)  continue;
      if (FFenQuAry[q].bSelected)
        glColor3ub(255, 255, 255);
      else
        glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    //glPushName(q);
      for( i = FFenQuAry[q].nIBegin;i<=FFenQuAry[q].nIEnd - 1 ;i++)
        for (j = FFenQuAry[q].nJBegin;j<=FFenQuAry[q].nJEnd - 1;j++)
          for (k = FFenQuAry[q].nKBegin ;k<=FFenQuAry[q].nKEnd;k++)
          {
            if (! ((k < FFenQuAry[q].nKBegin+1) || (((k-FFenQuAry[q].nKBegin) % FFenQuAry[q].nKSkip) == 0)))  continue;
            glPushName(CreateglName(q, i, j, k));
            glBegin(GL_QUADS);
            if (curVarIdx == 0) index = floor((GetXYZ(q, i, j, k).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1) index = floor((GetXYZ(q, i, j, k).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2) index = floor((GetXYZ(q, i, j, k).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded) index = floor((GetResultByIndex(q, i, j, k, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            vNormal1[0] = 0; vNormal1[1] = 0; vNormal1[2] = 0;
            vNormal2[0] = 0; vNormal2[1] = 0; vNormal2[2] = 0;
            vNormal3[0] = 0; vNormal3[1] = 0; vNormal3[2] = 0;
            vNormal4[0] = 0; vNormal4[1] = 0; vNormal4[2] = 0;
            
            vPoints1[0] = GetXYZ(q, i, j, k).X / nBaseRange;
            vPoints1[1] = GetXYZ(q, i, j, k).Y / nBaseRange;
            vPoints1[2] = GetXYZ(q, i, j, k).Z / nBaseRange;

            vPoints2[0] = GetXYZ(q, i + 1, j, k).X / nBaseRange;
            vPoints2[1] = GetXYZ(q, i + 1, j, k).Y / nBaseRange;
            vPoints2[2] = GetXYZ(q, i + 1, j, k).Z / nBaseRange;


            vPoints3[0] = GetXYZ(q, i + 1, j + 1, k).X / nBaseRange;
            vPoints3[1] = GetXYZ(q, i + 1, j + 1, k).Y / nBaseRange;
            vPoints3[2] = GetXYZ(q, i + 1, j + 1, k).Z / nBaseRange;
            gltGetNormalVector(vPoints1, vPoints2, vPoints3, vNormal1);


            glNormal3fv(vNormal1);
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
              GetXYZ(q, i, j, k).Y / nBaseRange,
              GetXYZ(q, i, j, k).Z / nBaseRange);

            if (curVarIdx == 0)  index = floor((GetXYZ(q, i + 1, j, k).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1)  index = floor((GetXYZ(q, i + 1, j, k).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i + 1, j, k).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded) index = floor((GetResultByIndex(q, i + 1, j, k, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
              GetXYZ(q, i + 1, j, k).Y / nBaseRange,
              GetXYZ(q, i + 1, j, k).Z / nBaseRange);

            if (curVarIdx == 0)  index = floor((GetXYZ(q, i + 1, j + 1, k).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1)  index = floor((GetXYZ(q, i + 1, j + 1, k).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i + 1, j + 1, k).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded) index = floor((GetResultByIndex(q, i + 1, j + 1, k, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i + 1, j + 1, k).X / nBaseRange,
              GetXYZ(q, i + 1, j + 1, k).Y / nBaseRange,
              GetXYZ(q, i + 1, j + 1, k).Z / nBaseRange);

            if (curVarIdx == 0)  index = floor((GetXYZ(q, i, j + 1, k).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1)  index = floor((GetXYZ(q, i, j + 1, k).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i, j + 1, k).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded) index = floor((GetResultByIndex(q, i, j + 1, k, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
              GetXYZ(q, i, j + 1, k).Y / nBaseRange,
              GetXYZ(q, i, j + 1, k).Z / nBaseRange);

            glEnd();
            glPopName();
            glLineWidth(1);
          }

    //glPopName();
    }
  }

  //if (cparam.bIDir)
  if (1)
  {
  //画边界面I(X)切面的边界
    for (q = 0;q<=FFenQu - 1;q++)
    {
      if (!FFenQuAry[q].bIDir)  continue;
      if (! FFenQuAry[q].IsDraw)  continue;
      if (FFenQuAry[q].bSelected) 
        glColor3ub(255, 255, 255);
      else
        glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    //glPushName(q);
      for (i = FFenQuAry[q].nIBegin;i<=FFenQuAry[q].nIEnd;i++)
        for (j = FFenQuAry[q].nJBegin ;j<=FFenQuAry[q].nJEnd - 1 ;j++)
          for( k = FFenQuAry[q].nKBegin;k<=FFenQuAry[q].nKEnd - 1;k++)
          {
            if (! ((i < FFenQuAry[q].nIBegin+1) || (((i-FFenQuAry[q].nIBegin) % FFenQuAry[q].nISkip) == 0)))   continue;
            glPushName(CreateglName(q, i, j, k));
            glBegin(GL_QUADS);
            if (curVarIdx == 0)  index = floor((GetXYZ(q, i, j, k).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1)  index = floor((GetXYZ(q, i, j, k).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i, j, k).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded)  index = floor((GetResultByIndex(q, i, j, k, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            vNormal1[0] = 0; vNormal1[1] = 0; vNormal1[2] = 0;
            vNormal2[0] = 0; vNormal2[1] = 0; vNormal2[2] = 0;
            vNormal3[0] = 0; vNormal3[1] = 0; vNormal3[2] = 0;
            vNormal4[0] = 0; vNormal4[1] = 0; vNormal4[2] = 0;
            
            vPoints1[0] = GetXYZ(q, i, j, k).X / nBaseRange;
            vPoints1[1] = GetXYZ(q, i, j, k).Y / nBaseRange;
            vPoints1[2] = GetXYZ(q, i, j, k).Z / nBaseRange;

            vPoints2[0] = GetXYZ(q, i , j, k+1).X / nBaseRange;
            vPoints2[1] = GetXYZ(q, i , j, k+1).Y / nBaseRange;
            vPoints2[2] = GetXYZ(q, i , j, k+1).Z / nBaseRange;


            vPoints3[0] = GetXYZ(q, i , j + 1, k+1).X / nBaseRange;
            vPoints3[1] = GetXYZ(q, i , j + 1, k+1).Y / nBaseRange;
            vPoints3[2] = GetXYZ(q, i , j + 1, k+1).Z / nBaseRange;
            gltGetNormalVector(vPoints1, vPoints2, vPoints3, vNormal1);

            glNormal3fv(vNormal1);
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
              GetXYZ(q, i, j, k).Y / nBaseRange,
              GetXYZ(q, i, j, k).Z / nBaseRange);

            if (curVarIdx == 0)  index = floor((GetXYZ(q, i, j + 1, k).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1)  index = floor((GetXYZ(q, i, j + 1, k).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i, j + 1, k).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded) index = floor((GetResultByIndex(q, i, j + 1, k, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i, j + 1, k).X / nBaseRange,
              GetXYZ(q, i, j + 1, k).Y / nBaseRange,
              GetXYZ(q, i, j + 1, k).Z / nBaseRange);

            if (curVarIdx == 0)  index = floor((GetXYZ(q, i, j + 1, k + 1).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1)  index = floor((GetXYZ(q, i, j + 1, k + 1).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i, j + 1, k + 1).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded)  index = floor((GetResultByIndex(q, i, j + 1, k + 1, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i, j + 1, k + 1).X / nBaseRange,
              GetXYZ(q, i, j + 1, k + 1).Y / nBaseRange,
              GetXYZ(q, i, j + 1, k + 1).Z / nBaseRange);

            if (curVarIdx == 0)  index = floor((GetXYZ(q, i, j, k + 1).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1)  index = floor((GetXYZ(q, i, j, k + 1).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i, j, k + 1).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded) index = floor((GetResultByIndex(q, i, j, k + 1, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
              GetXYZ(q, i, j, k + 1).Y / nBaseRange,
              GetXYZ(q, i, j, k + 1).Z / nBaseRange);

            glEnd();
            glPopName();
            glLineWidth(1);
          }
    }
  }

  //if (cparam.bJDir)
  if (1)
  {
  //画边界面J(Y)切面的边界
    for( q = 0;q<=FFenQu - 1;q++)
    {
      if (!FFenQuAry[q].bJDir)  continue;
      if (! FFenQuAry[q].IsDraw )  continue;
      if (FFenQuAry[q].bSelected)
        glColor3ub(255, 255, 255);
      else
        glColor3ub(FenQuColor[q % 27][0], FenQuColor[q % 27][1], FenQuColor[q % 27][2]);
    //glPushName(q);
    //glScale(1+q*0.1,1+q*0.1,1+q*0.1);
      for (i = FFenQuAry[q].nIBegin;i<=FFenQuAry[q].nIEnd - 1;i++)
        for (j = FFenQuAry[q].nJBegin;j<=FFenQuAry[q].nJEnd;j++)
          for (k = FFenQuAry[q].nKBegin;k<=FFenQuAry[q].nKEnd - 1;k++)
          {
            if (! ((j < FFenQuAry[q].nJBegin+1) || (((j-FFenQuAry[q].nJBegin) % FFenQuAry[q].nJSkip) == 0)))  continue;
            glPushName(CreateglName(q, i, j, k));
            glBegin(GL_QUADS);
            if (curVarIdx == 0) index = floor((GetXYZ(q, i, j, k).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1) index = floor((GetXYZ(q, i, j, k).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2) index = floor((GetXYZ(q, i, j, k).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
               if (IsRltLoaded) index = floor((GetResultByIndex(q, i, j, k, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            vNormal1[0] = 0; vNormal1[1] = 0; vNormal1[2] = 0;
            vNormal2[0] = 0; vNormal2[1] = 0; vNormal2[2] = 0;
            vNormal3[0] = 0; vNormal3[1] = 0; vNormal3[2] = 0;
            vNormal4[0] = 0; vNormal4[1] = 0; vNormal4[2] = 0;

            vPoints1[0] = GetXYZ(q, i, j, k).X / nBaseRange;
            vPoints1[1] = GetXYZ(q, i, j, k).Y / nBaseRange;
            vPoints1[2] = GetXYZ(q, i, j, k).Z / nBaseRange;

            vPoints2[0] = GetXYZ(q, i + 1, j, k).X / nBaseRange;
            vPoints2[1] = GetXYZ(q, i + 1, j, k).Y / nBaseRange;
            vPoints2[2] = GetXYZ(q, i + 1, j, k).Z / nBaseRange;


            vPoints3[0] = GetXYZ(q, i + 1, j , k+1).X / nBaseRange;
            vPoints3[1] = GetXYZ(q, i + 1, j , k+1).Y / nBaseRange;
            vPoints3[2] = GetXYZ(q, i + 1, j , k+1).Z / nBaseRange;
            gltGetNormalVector(vPoints1, vPoints2, vPoints3, vNormal1);

            glNormal3fv(vNormal1);
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i, j, k).X / nBaseRange,
              GetXYZ(q, i, j, k).Y / nBaseRange,
              GetXYZ(q, i, j, k).Z / nBaseRange);

            if (curVarIdx == 0)  index = floor((GetXYZ(q, i, j, k + 1).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx ==1)  index = floor((GetXYZ(q, i, j, k + 1).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i, j, k + 1).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded)  index = floor((GetResultByIndex(q, i, j, k + 1, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i, j, k + 1).X / nBaseRange,
              GetXYZ(q, i, j, k + 1).Y / nBaseRange,
              GetXYZ(q, i, j, k + 1).Z / nBaseRange);


            if (curVarIdx ==0) index = floor((GetXYZ(q, i + 1, j, k + 1).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx ==1)  index = floor((GetXYZ(q, i + 1, j, k + 1).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i + 1, j, k + 1).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded)  index = floor((GetResultByIndex(q, i + 1, j, k + 1, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i + 1, j, k + 1).X / nBaseRange,
              GetXYZ(q, i + 1, j, k + 1).Y / nBaseRange,
              GetXYZ(q, i + 1, j, k + 1).Z / nBaseRange);

            if (curVarIdx == 0)  index = floor((GetXYZ(q, i + 1, j, k).X - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 1)  index = floor((GetXYZ(q, i + 1, j, k).Y - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx == 2)  index = floor((GetXYZ(q, i + 1, j, k).Z - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            if (curVarIdx >= 3)
            {
              if (IsRltLoaded) index = floor((GetResultByIndex(q, i + 1, j, k, curVarIdx) - FSeg.SegStartValueList[0]) / (FSeg.SegStartValueList[9] - FSeg.SegStartValueList[0]) * NCOLOR + 0.5);
            }
            glColor3f(FColorMapList[index].R, FColorMapList[index].G, FColorMapList[index].B);
            glVertex3f(GetXYZ(q, i + 1, j, k).X / nBaseRange,
              GetXYZ(q, i + 1, j, k).Y / nBaseRange,
              GetXYZ(q, i + 1, j, k).Z / nBaseRange);

            glEnd();
            glPopName();
            glLineWidth(1);
          }
    }
  }
  glPolygonOffset(-0.9, -0.9);
  glDisable(GL_POLYGON_OFFSET_FILL);
  IsDrawing = false;
  glLineWidth(1);
}


