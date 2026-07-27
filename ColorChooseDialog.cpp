//#include "ColorChooseDialog.h"
//#include "SignalInstance.h"
//#include <QDir>
//#include <QMessageBox>
//
//extern QString appComDir;
//
//
//ColorChooseDialog::ColorChooseDialog(QWidget*parent) :QWidget(parent)
//{
//	initUi();
//	SW = NULL;
//}
//ColorChooseDialog::~ColorChooseDialog()
//{
//	
//	delete m_mainLayout;
//}
//
//void ColorChooseDialog::initUi()
//{
//	this->setFixedSize(QSize(800, 600));
//	m_mainLayout = new QVBoxLayout;
//	QHBoxLayout*layout1 = new QHBoxLayout;
//	
//
//	m_btnOk = new QPushButton(u8"确定");
//	m_btnCancel = new QPushButton(u8"取消");
//	
//	m_mainLayout->addLayout(layout1);
//	this->setLayout(m_mainLayout);
//
//	connect(m_btnOk, SIGNAL(clicked()), this, SLOT(slotLoadDirData()));
//	connect(m_btnCancel, SIGNAL(clicked()), this, SLOT(slotCancel()));
//
//}