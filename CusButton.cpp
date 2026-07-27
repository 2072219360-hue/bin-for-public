#include "CusButton.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QDoubleValidator>

CusButton::CusButton(QWidget* parent)
	: QPushButton(parent)
{
	m_imageLabel = new QLabel;
	m_imageLabel->setFixedWidth(30);
	m_imageLabel->setFixedHeight(30);
	m_imageLabel->setScaledContents(true);
	//m_imageLabel->setStyleSheet("QLabel{background-color:transparent;}");

	m_textLabel = new QLabel;
	//m_textLabel->setStyleSheet("QLabel{background-color:transparent;}");
	m_textLabel->setFixedHeight(30);
	QHBoxLayout* mainLayout = new QHBoxLayout;
	mainLayout->addWidget(m_imageLabel);
	mainLayout->addWidget(m_textLabel);
	mainLayout->setMargin(0);
	mainLayout->setSpacing(0);
	this->setLayout(mainLayout);
}

void CusButton::SetImageLabel(const QPixmap &pixmap)
{
	m_imageLabel->setPixmap(pixmap);
}

void CusButton::SetTextLabel(QString text) 
{
	m_textLabel->setText(text);
}

QLabel* CusButton::GetImageHandle()
{
	return m_imageLabel;
}

QLabel* CusButton::GetTextHandle()
{
	return m_textLabel;
}

void CusButton::mousePressEvent(QMouseEvent * event)
{
	if (event->button() == Qt::LeftButton) {
		sheet = this->styleSheet();
		this->setStyleSheet("background-color:#257FE6;");
		emit clicked(true);
	}
	//QWidget::mousePressEvent(event);
}

void CusButton::mouseReleaseEvent(QMouseEvent * event)
{
	this->setStyleSheet(sheet);
}

