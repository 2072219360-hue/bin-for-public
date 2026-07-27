//---------------------------------------------------------------------------


#pragma hdrstop

#include "untVast2DTecplotReader.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TVastXY2DReader::TVastXY2DReader(String AFileName)
{
  FFileName = AFileName;
  IsLoaded = False;
  VarNameList = new TStringList();
  FZoneNum = 0;
  FVarNum = 0;
}

void TVastXY2DReader::Load(String AFileName)
{
  TStringList *tmpList;
  String tmpToken;
  int tmpInt;
  int i, j, k, z;
  int idx;
  TStringList *ValueList;
  TStringList *VtmpList;
  int nPos;

  if (IsLoaded)
  {
    //初始化和清理一下。
    VarNameList->Clear();
    FVarNum = 0;
    FZoneNum = 0;
  }
  IsLoaded = True;
  if (! FileExists(AFileName)) { IsLoaded = False; return; }
  tmpList = new TStringList();
  tmpList->LoadFromFile(AFileName);
  if (tmpList->Count < 2) { IsLoaded = False; return; }
  //第一行数据
  if (tmpList->Strings[0].Length() < 10) { IsLoaded = False; return; }
  tmpToken = tmpList->Strings[0];
  //分解第一行，得出变量名列表
  nPos=tmpToken.Pos('=');

  if (nPos<1)  { IsLoaded=False;return;}
  tmpToken=tmpToken.SubString(nPos+1,tmpToken.Length()- nPos+1);
  //tmpToken = Copy(tmpToken, nPos+1, Length(tmpToken) - nPos+1);
  ValueList=new TStringList();
  
  VarNameList->DelimitedText = tmpToken;
  VarNameList->Delimiter = ',';
  FVarNum = VarNameList->Count;

  //SetLength(FZoneIndex, 10000);
  FZoneIndex.Length = 10000;
  for(i=0;i<=tmpList->Count-1;i++)
  //for i := 0 to tmpList.Count - 1 do
  {
    //if Copy(trim(tmpList.Strings[i]), 0, 4) == 'ZONE'
    if (tmpList->Strings[i].SubString(0,4)=="ZONE")
    {
      FZoneIndex[FZoneNum] = i;
      FZoneNum = FZoneNum + 1;
    }
  }

  //applog.Debug('Zone Num=' + IntToStr(FZoneNum));
  //SetLength(Finfo, FZoneNum + 1); //分配Zone的个数。
  Finfo.Length = FZoneNum + 1;
  //SetLength(FZoneIndex, FZoneNum + 1); //分配Zone的索引空间
  FZoneIndex.Length = FZoneNum + 1;

  //第二行数据丢弃
  //索引每一个Zone.
  //for z := 0 to FZoneNum-1 do
  for(z=0;z<=FZoneNum-1;z++)
  {
    //第Zone+1行数据是所包括的行数
    tmpInt = StrToInt(tmpList->Strings[FZoneIndex[z]+1]);
    Finfo[z]=new TXY2DZone();
    //SetLength(FInfo[z].Fdata,FVarNum+1);
    Finfo[z]->FData.Length =FVarNum+1;

    //SetLength(FInfo[z].FVarmax,FVarNum+1);
    Finfo[z]->FVarMax.Length = FVarNum+1;

    //SetLength(FInfo[z].FVarMin,FVarNum+1);
    Finfo[z]->FVarmin.Length =FVarNum+1;

    Finfo[z]->FDotNum = tmpInt;
    //分配当前Zone中的变量的数据个数。
    //for i:=0 to FVarNum-1 do
    for(i=0;i<=FVarNum-1;i++)
    {
      //SetLength(Finfo[z].FData[i],tmpInt+1);
      Finfo[z]->FData[i].Length = tmpInt+1;

      Finfo[z]->FVarMax[i]=-9999999;
      Finfo[z]->FVarmin[i]=9999999;
    }

    //for i := 0 to tmpInt - 1 do
    for(i=0;i<=tmpInt-1;i++)
    {
      ValueList->DelimitedText = tmpList->Strings[FZoneIndex[z]+2+i];
      ValueList->Delimiter = ',';
      //for j:=0 to ValueList.Count-1 do
      for(j=0;j<=ValueList->Count-1;j++)
      {
         //Finfo[z].FData[j][i]:=StrToFloat(ValueList.Strings[j]);
         Finfo[z]->FData[j][i] = StrToFloat(ValueList->Strings[j]);
         if (Finfo[z]->FData[j][i]>Finfo[z]->FVarMax[j])
            Finfo[z]->FVarMax[j]=  Finfo[z]->FData[j][i];
         if (Finfo[z]->FData[j][i]<Finfo[z]->FVarmin[j])
            Finfo[z]->FVarmin[j]=  Finfo[z]->FData[j][i]; 
      }
    }
  }
  //数据读取成功!
  FFileName= AFileName;
  //applog.Debug(FloatToStr(Finfo[0].FData[0][1]));
  //每个Zone中的每个变量的值的最大值，最小值
  //for z:=0 to FZoneNum-1 do
  for(z=0;z<=FZoneNum-1;z++)
  {
    //for i:=0 to FVarNum-1 do
    for(i=0;i<=FVarNum-1;i++)
    {
      //applog.Debug('变量'+IntToStr(i)+'变量名='+VarNameList.Strings[i]);
      //applog.Debug('最大值：'+FloatToStr(FInfo[z].FVarMax[i]));
      ;//applog.Debug('最小值：'+FloatToStr(FInfo[z].FVarMin[i]));
    }
  }
}

void TVastXY2DReader::LoadNew(String AFileName)
{
  TStringList *tmpList;
  String tmpToken;
  int tmpInt;
  int i, j, k, z;
  int idx;
  TStringList *ValueList;
  TStringList *VtmpList;
  int nPos;

  if (IsLoaded)
  {
    //初始化和清理一下。
    VarNameList->Clear();
    FVarNum = 0;
    FZoneNum = 0;
  }
  IsLoaded = True;
  if (! FileExists(AFileName)) { IsLoaded = False; return; }
  tmpList = new TStringList();
  tmpList->LoadFromFile(AFileName);
  if (tmpList->Count < 2) {IsLoaded = False; return; }
  //第一行数据
  if (tmpList->Strings[0].Length() < 10) { IsLoaded = False; return; }
  tmpToken = tmpList->Strings[0];
  //分解第一行，得出变量名列表
  nPos=tmpToken.Pos('=');
  if (nPos<1) {IsLoaded=False;return;}
  tmpToken = tmpToken.SubString(nPos+1,tmpToken.Length()-nPos+1);
  ValueList=new TStringList();
  
  VarNameList->DelimitedText = tmpToken;
  VarNameList->Delimiter = ',';
  FVarNum = VarNameList->Count;

  //SetLength(FZoneIndex, 10000);
  FZoneIndex.Length=10000;
  //for i := 0 to tmpList.Count - 1 do
  for(i=0;i<=tmpList->Count-1;i++)
  {
    //if Copy(trim(uppercase(tmpList.Strings[i])), 0, 4) = 'ZONE' then
    if (tmpList->Strings[i].UpperCase().Trim().SubString(0,4)=="ZONE")
    {
      FZoneIndex[FZoneNum] = i;
      FZoneNum = FZoneNum + 1;
    }
  }
  FZoneIndex[FZoneNum]= tmpList->Count - 1;

  //applog.Debug('Zone Num=' + IntToStr(FZoneNum));
  //SetLength(Finfo, FZoneNum + 1); //分配Zone的个数。
  Finfo.Length = FZoneNum+1;

  //SetLength(FZoneIndex, FZoneNum + 1); //分配Zone的索引空间
  FZoneIndex.Length= FZoneNum+1;

  //第二行数据丢弃
  //索引每一个Zone.
  //for z := 0 to FZoneNum-1 do
  for(z=0;z<=FZoneNum-1;z++)
  {
    //第Zone+1行数据是所包括的行数
    tmpInt = FZoneIndex[z+1]-FZoneIndex[z]-1;//StrToInt(tmpList.Strings[FZoneIndex[z]+1]);
    Finfo[z]=new TXY2DZone();
    //SetLength(FInfo[z].Fdata,FVarNum+1);
    Finfo[z]->FData.Length = FVarNum+1;

    //SetLength(FInfo[z].FVarmax,FVarNum+1);
    Finfo[z]->FVarMax.Length = FVarNum+1;

    //SetLength(FInfo[z].FVarMin,FVarNum+1);
    Finfo[z]->FVarmin.Length = FVarNum+1;

    Finfo[z]->FDotNum = tmpInt;
    //分配当前Zone中的变量的数据个数。
    //for i:=0 to FVarNum-1 do
    for(i=0;i<=FVarNum-1;i++)
    {
      //SetLength(Finfo[z].FData[i],tmpInt+1);
      Finfo[z]->FData[i].Length = tmpInt+1;

      Finfo[z]->FVarMax[i]=-9999999;
      Finfo[z]->FVarmin[i]=9999999;
    }

    //for i := 0 to tmpInt - 1 do
    for(i=0;i<=tmpInt-1;i++)
    {
      ValueList->DelimitedText = tmpList->Strings[FZoneIndex[z]+1+i];
      ValueList->Delimiter = ',';
      //for j:=0 to ValueList->Count-1 do
      //for(j=0;j<=ValueList->Count-1;j++)
      for(j=0;(j<=FVarNum-1)&&(j<=ValueList->Count-1);j++)
      {
         Finfo[z]->FData[j][i]=StrToFloat(ValueList->Strings[j]);
         if (Finfo[z]->FData[j][i]>Finfo[z]->FVarMax[j])
            Finfo[z]->FVarMax[j]=  Finfo[z]->FData[j][i];
         if (Finfo[z]->FData[j][i]<Finfo[z]->FVarmin[j])
            Finfo[z]->FVarmin[j]=  Finfo[z]->FData[j][i];
      }
    }
  }
  //数据读取成功!
  FFileName= AFileName;
  //applog.Debug(FloatToStr(Finfo[0].FData[0][1]));
  //每个Zone中的每个变量的值的最大值，最小值
  //for z:=0 to FZoneNum-1 do
  //begin
  //  for i:=0 to FVarNum-1 do
  //  begin
  //    applog.Debug('变量'+IntToStr(i)+'变量名='+VarNameList.Strings[i]);
  //    applog.Debug('最大值：'+FloatToStr(FInfo[z].FVarMax[i]));
  //    applog.Debug('最小值：'+FloatToStr(FInfo[z].FVarMin[i]));
  //  end;
  //end;
}
