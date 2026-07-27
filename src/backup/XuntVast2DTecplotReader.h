//---------------------------------------------------------------------------

#ifndef untVast2DTecplotReaderH
#define untVast2DTecplotReaderH
#include <vcl.h>
#pragma hdrstop

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "stdio.h"

//---------------------------------------------------------------------------

  class TXY2DZone : TObject{
  public:
    int FVarNum;
    int FDotNum;
    DynamicArray< DynamicArray< double> > FData;
    DynamicArray<double> FVarMax;
    DynamicArray<double> FVarmin;
  };

  class TVastXY2DReader :TObject{
  public:
    TStringList * VarNameList;
    int FVarNum;
    int FZoneNum;
    bool IsLoaded;
    FILE *FFile;
    DynamicArray<TXY2DZone*> Finfo;
    DynamicArray<int> FZoneIndex;
  public:
    TVastXY2DReader(String AFileName = "");
    void Load(String AFileName);
    void LoadNew(String AFileName);
  public:
    String FFileName;
  };

  typedef DynamicArray<TVastXY2DReader> TVastXY2DReaderAry;


#endif
