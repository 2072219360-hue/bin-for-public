#pragma once
#include"mySubject.h"
#include<list>
#include"SignalInstance.h"
#include<string>
#include<iostream>
#include<QDebug>
#include <fstream>
using namespace std;
enum btnType :unsigned int {
	CLIP_WINDOW_VISIBLE = 1 ,
	CLIP_WINDOW_INVISIBLE= 1 << 1,
	CLIP_SINGLE= 1 << 2,
	CLIP_BOTH= 1 << 3,
	STOP_COLOR= 1 << 4,
	DEFAULT_ARGMENTS= 1 << 5,
	RUN_HEAD= 1 << 6,
	RUN_BODY= 1 << 7,
	RUN_SEGMENT= 1 << 8,
	JUMP= 1 << 9,
	BATCH=1<<10,
	COMPUTE=1<<11,
	PROFILING=1<<12,
	RE_CLIP=1<<13,
};
//负责按钮状态
enum Status :unsigned int {
	ENABLE_HEAD = (RUN_HEAD | COMPUTE),
	ENABLE_SECT = (RUN_SEGMENT | JUMP | STOP_COLOR),
	ENABLE_BODY = (RUN_BODY | COMPUTE),

	//打开文件前的默认状态
	DEFAULT_STATUS = 0,
	//新建工作区后的状态
	NEW_WORKSPACE = (CLIP_WINDOW_VISIBLE),
	//显示切割窗口后的状态
	CLIP_WINDOW_SHOW = (CLIP_WINDOW_INVISIBLE | CLIP_SINGLE | CLIP_BOTH),
	//切割完之后
	CLIP_FINISH = (DEFAULT_ARGMENTS | ENABLE_HEAD | ENABLE_SECT|COMPUTE| RE_CLIP),

	//可以运行身部了
	HEAD_SECT_FINISH = (DEFAULT_ARGMENTS | ENABLE_HEAD | ENABLE_SECT | ENABLE_BODY | BATCH | COMPUTE | RE_CLIP),


	////运行完头部
	//HEAD_FINISH=(DEFAULT_ARGMENTS | ENABLE_HEAD | ENABLE_SECT),
	////运行完分段之后
	SEGMENT_FINISH = (DEFAULT_ARGMENTS | ENABLE_HEAD | ENABLE_SECT| BATCH | COMPUTE | RE_CLIP),
	////运行完身部之后
	//BODY_FINISH= (DEFAULT_ARGMENTS | ENABLE_HEAD | ENABLE_SECT | ENABLE_BODY),
	
	
	
};
//负责控制状态
class statusSubject:public mySubject
{
public:
	statusSubject() :m_status(DEFAULT_STATUS),headFinish(false),sectFinish(true), clip_dire(-1),
		bodyFinish(false),bothClip(false),clipFinish(false),clip_index(0),last_status(NEW_WORKSPACE){}
	void Attach(myObserver* ob);
	void Remove(myObserver* ob);
	void Notify();
	void RemoveAll();
	void setStatus(Status current_status);
	void setHeadFinish(bool _headFinish);
	void setSectFinish(bool _sectFinish);
	void setBodyFinish(bool _bodyFinish);
	void setBothClip(bool _bothClip);
	void setClipFinish(bool _clipFinish);
	void setClipIndex(int _index);
	void setClipDire(int _dire);
	void AttachVector(QVector<myObserver*>& v);
	void changeStatus();
	Status getStatus();
	bool getBothClip();
	bool getClipFinish();
	int getClipDire();
	void readStatus(string path);
	void writeStatus(string path);
	bool getSectFinish();
	int getClipIndex();
	void setLastStatus();
	Status getLastStatus();
private:
	std::list<myObserver*> m_observerList;
	Status m_status;
	bool headFinish, sectFinish, bodyFinish;
	bool bothClip;
	bool clipFinish;
	int clip_index;
	int clip_dire;// 0 代表默认的右侧切割 ,1 代表左侧切割，-1代表第一次新建，未设置
	Status last_status;
	//mutex toList 
	QMutex mu;
};

