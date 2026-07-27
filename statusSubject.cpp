#include "statusSubject.h"

void statusSubject::AttachVector(QVector<myObserver*>& v) {
	mu.lock();
	for (auto& it : v) {
		m_observerList.push_back(it);
	}
	mu.unlock();
}
void statusSubject::Attach(myObserver* ob) {
	mu.lock();
	m_observerList.push_back(ob);
	mu.unlock();
}
void statusSubject::Remove(myObserver* ob) {
	mu.lock();
	m_observerList.remove(ob);
	mu.unlock();
}
void statusSubject::Notify() {
	mu.lock();
	for (auto it : m_observerList) {
		it->Update();
	}
	mu.unlock();
}
void statusSubject::RemoveAll() {
	mu.lock();
	m_observerList.clear();
	mu.unlock();
}
void statusSubject::setStatus(Status current_status) {
	
	if (current_status == Status::DEFAULT_STATUS) {
		headFinish = sectFinish = bodyFinish = bothClip=false;
	}
	if (current_status == Status::CLIP_FINISH) {
		clipFinish = true;
	}
	m_status = current_status;
	this->Notify();
	//TODO:0423
	if (current_status == Status::CLIP_FINISH || current_status == Status::HEAD_SECT_FINISH|| current_status == Status::SEGMENT_FINISH)
	writeStatus(SignalInstance::getInstance()->getWorkSpace().toLocal8Bit().toStdString() + "/status.txt");

}
Status statusSubject::getStatus() {
	return m_status;
}
Status statusSubject::getLastStatus() {
	return last_status;
}
void statusSubject::setLastStatus() {
	last_status = m_status;
}
bool statusSubject::getBothClip() { return bothClip; }
bool statusSubject::getClipFinish() { return clipFinish; }
bool statusSubject::getSectFinish() { return sectFinish; }
int statusSubject::getClipIndex() { return clip_index; }
int statusSubject::getClipDire() { return clip_dire; }
void statusSubject::setHeadFinish(bool _headFinish) { headFinish = _headFinish; changeStatus(); }
void statusSubject::setSectFinish(bool _sectFinish) { sectFinish = _sectFinish; changeStatus(); }
void statusSubject::setBodyFinish(bool _bodyFinish) { bodyFinish = _bodyFinish; changeStatus(); }
void statusSubject::setBothClip(bool _bothClip) { bothClip = _bothClip; };
void statusSubject::setClipFinish(bool _clipFinish) { clipFinish = _clipFinish; };
void statusSubject::setClipIndex(int _index) { clip_index = _index; changeStatus();};
void statusSubject::setClipDire(int _dire) { clip_dire = _dire; changeStatus(); };
void statusSubject::changeStatus(){
	
	if (headFinish&&sectFinish) {
		setStatus(Status::HEAD_SECT_FINISH);
		//TODO:0423
		writeStatus(SignalInstance::getInstance()->getWorkSpace().toLocal8Bit().toStdString() + "/status.txt");
	}
}
void statusSubject::readStatus(string path) {
	ifstream input(path,ios::in);
	if (!input.is_open()) {
		qDebug() << "read Status fail!";
		setStatus(Status::NEW_WORKSPACE);
		setLastStatus();
		return;
	}
	unsigned int _status;
	input >> _status;
	m_status = static_cast<Status>(_status);
	input >> headFinish;
	input >> sectFinish;
	input >> bodyFinish;
	input >> bothClip;
	input >> clip_index;
	input >> clip_dire;
	input >> clipFinish;
	input.close();
	setLastStatus();
	qDebug() << "status=" << m_status << " lastStatus=" << getLastStatus();
	this->Notify();
}
void statusSubject::writeStatus(string path){
	ofstream output(path, ios::out);
	if (!output.is_open()) {
		qDebug() << "write Status fail! Path="<< QString::fromStdString(path);
		//setStatus(Status::DEFAULT_STATUS);
		return;
	}
	output << m_status<<endl;
	output << headFinish<<endl;
	output << sectFinish<<endl;
	output << bodyFinish<<endl;
	output << bothClip << endl;
	output << clip_index << endl;
	output << clip_dire << endl;
	output << clipFinish<<endl;
	output.close();

}
