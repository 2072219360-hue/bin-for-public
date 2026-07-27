#include "SectInfoDialog.h"
#include <QDebug>
#include <QMessageBox>
#include <QFile>
extern QString appComDir;
SectInfoDialog::SectInfoDialog(int id, QDialog * parent):QDialog(parent)
{
	m_sectId = id;
	setWindowTitle(QString(u8"分段%1信息填充").arg(id));
	//this->setFixedSize(QSize(350, 400));
	init();
	loadData();
}

SectInfoDialog::~SectInfoDialog()
{
	//qDebug() << __FUNCTION__;
}
void SectInfoDialog::loadData() {
	using namespace std;
	std::ifstream inTrLoad;
	BodyPathChecker tempChecker;
	QString truePath = tempChecker.checkAndGetPath(QString(SignalInstance::getInstance()->m_workspace + "/body/trData.dat"));
	inTrLoad.open(truePath.toLocal8Bit().toStdString(),std::ios::in);
	if (!inTrLoad.is_open()) return;
	string str;
	while (getline(inTrLoad,str)) {
		if (str.length()>=1&&str[0] == '#') {
			for (int i = 0; i <= m_sectId; i++) {
				getline(inTrLoad, str);
			}
			stringstream ss;
			ss << str;
			string s;
			QVector<QLineEdit*> v = { m_xStepLineEdit,m_xStepMinLineEdit,
			m_min1LineEdit, m_maxLineEdit, m_kStepLineEdit, m_nCstpLineEdit };

			ss >> s;
			for (int i = 0; i < v.size(); i++) {
				ss >> s;
				v[i]->setText(QString::fromStdString(s));
			}
			ss.str("");  // 清空内容
			ss.clear();  // 清除状态标志
			break;
		}
	}
	inTrLoad.close();

}
void SectInfoDialog::init()
{
	m_xStep = new QLabel(u8"初始步长");
	m_xStepLineEdit = new QLineEdit;
	m_xStepMin = new QLabel(u8"初始阶段步长限制");
	m_xStepMinLineEdit = new QLineEdit;
	m_min1 = new QLabel(u8"终止阶段步长限制");
	m_min1LineEdit = new QLineEdit;
	m_max = new QLabel(u8"最大步长限制");
	m_maxLineEdit = new QLineEdit;
	m_kStep = new QLabel(u8"步长增长因子");
	m_kStepLineEdit = new QLineEdit;
	m_nCstp = new QLabel(u8"初始步长计算步数");
	m_nCstpLineEdit = new QLineEdit;
	m_mainGridLayout = new QGridLayout;

	m_mainGridLayout->addWidget(m_xStep, 0, 0, 1, 1);
	m_mainGridLayout->addWidget(m_xStepLineEdit, 0, 1, 1, 2);
	m_mainGridLayout->addWidget(m_xStepMin, 1, 0, 1, 1);
	m_mainGridLayout->addWidget(m_xStepMinLineEdit, 1, 1, 1, 2);
	m_mainGridLayout->addWidget(m_min1, 2, 0, 1, 1);
	m_mainGridLayout->addWidget(m_min1LineEdit, 2, 1, 1, 2);
	m_mainGridLayout->addWidget(m_max, 3, 0, 1, 1);
	m_mainGridLayout->addWidget(m_maxLineEdit, 3, 1, 1, 2);
	m_mainGridLayout->addWidget(m_kStep, 4, 0, 1, 1);
	m_mainGridLayout->addWidget(m_kStepLineEdit, 4, 1, 1, 2);
	m_mainGridLayout->addWidget(m_nCstp, 5, 0, 1, 1);
	m_mainGridLayout->addWidget(m_nCstpLineEdit, 5, 1, 1, 2);
	m_saveBtn = new QPushButton(u8"保存");
	m_cancelBtn = new QPushButton(u8"取消");


	m_btnLayout = new QHBoxLayout;
	m_btnLayout->addWidget(m_saveBtn);
	m_btnLayout->addWidget(m_cancelBtn);
	m_mainLayout = new QVBoxLayout;
	connect(m_saveBtn, SIGNAL(clicked()), this,SLOT(onLoadTrDada()));
	connect(m_cancelBtn, &QPushButton::clicked, this, [=]() 
	{
		close();
	});
	m_mainLayout->addLayout(m_mainGridLayout);
	m_mainLayout->addLayout(m_btnLayout);
	//m_mainLayout->addStretch();
	//m_mainLayout->setSpacing(2);
	//m_mainLayout->setMargin(2);
	this->setLayout(m_mainLayout);
	this->loadStyleSheet("sectInfoDialog");
}
void SectInfoDialog::onLoadTrDada()
{
	QStringList list;
	if (m_xStepLineEdit->text() == "" || m_xStepMinLineEdit->text() == "" ||
		m_min1LineEdit->text() == "" || m_maxLineEdit->text() == "" || m_kStepLineEdit->text() == "" || m_nCstpLineEdit->text() == "")
	{
		QMessageBox::information(nullptr, u8"提示", u8"请将分段信息填写完全", QMessageBox::Ok);
		return;
	}
	list << QString(m_sectId) << m_xStepLineEdit->text() << m_xStepMinLineEdit->text() << m_min1LineEdit->text() << m_maxLineEdit->text()
		<< m_kStepLineEdit->text() << m_nCstpLineEdit->text();
	emit SignalInstance::getInstance()->signalSelectTrData(m_sectId, list);
	close();
}
void SectInfoDialog::loadStyleSheet(QString sheetName)
{
	QFile file(appComDir + "/images/SSPNS/CSS/" + sheetName + ".css");
	file.open(QFile::ReadOnly);//以只读的方式读取文件
	if (file.isOpen()) {
		setStyleSheet("");
		//将样式表全部读取，保存到字符串中
		QString qsstyleSheet = QLatin1String(file.readAll());

		setStyleSheet(qsstyleSheet);
		//qDebug() << "qsstyleSheet:" << qsstyleSheet;
	}
	file.close();
}