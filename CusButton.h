#pragma once
#include <qpushbutton.h>
#include <QLabel>
#include <QMouseEvent>
class CusButton :
	public QPushButton
{
	Q_OBJECT
public:
	/// @brief 构造方法
	explicit CusButton(QWidget* parent = nullptr);

	/// @brief SetImageLabel
	/// 设置按钮图标
	void SetImageLabel(const QPixmap &pixmap);

	/// @brief SetTextLabel
	/// 设置按钮文字
	void SetTextLabel(QString text);

	/// @brief GetImageHandle
	/// 返回m_imageLabel
	QLabel* GetImageHandle();

	/// @brief GetImageHandle
	/// 返回m_textLabel
	QLabel* GetTextHandle();
protected:
	void mousePressEvent(QMouseEvent*event)override;
	void mouseReleaseEvent(QMouseEvent*event)override;
	//void leaveEvent(QEvent *event)override;
private:
	// 按钮图标
	QLabel* m_imageLabel;
	// 按钮文字
	QLabel* m_textLabel;
	QString sheet;
};

