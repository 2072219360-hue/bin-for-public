#ifndef STYLEWIDGETFACTORY_H
#define STYLEWIDGETFACTORY_H

#include <QMessageBox>
#include <QWidget>
#include <QPushButton>
class StyleWidgetFactory
{
public:
	// 生成信息提示框
	static QMessageBox* createInfoMessageBox(const QString& title, const QString& text, QWidget* parent = nullptr);

	// 生成警告提示框
	static QMessageBox* createWarnMessageBox(const QString& title, const QString& text, QWidget* parent = nullptr);

private:
	// 私有构造函数，防止实例化
	StyleWidgetFactory() = default;

	// 设置统一样式
	static void setDefaultStyle(QMessageBox* msgBox);
};

#endif // STYLEWIDGETFACTORY_H