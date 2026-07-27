#include "StyleWidgetFactory.h"

QMessageBox* StyleWidgetFactory::createInfoMessageBox(const QString& title, const QString& text, QWidget* parent)
{
	QMessageBox* msgBox = new QMessageBox(QMessageBox::Information, title, text, QMessageBox::Ok, parent);
	setDefaultStyle(msgBox);
	return msgBox;
}

QMessageBox* StyleWidgetFactory::createWarnMessageBox(const QString& title, const QString& text, QWidget* parent)
{
	QMessageBox* msgBox = new QMessageBox(QMessageBox::Warning, title, text, QMessageBox::Ok, parent);
	setDefaultStyle(msgBox);
	return msgBox;
}
void StyleWidgetFactory::setDefaultStyle(QMessageBox* msgBox) {
	if (!msgBox) return;
	msgBox->setObjectName("FactoryMsgBox");
	msgBox->setAttribute(Qt::WA_StyledBackground, true);

	// 完善的样式表，确保状态变化生效
	msgBox->setStyleSheet(R"(
        #FactoryMsgBox {
            background-color: #f0f0f0;
            color: #333333;
            font-size: 14px;
        }
        #FactoryMsgBox QLabel {
            background-color: transparent;
            color: #333333;
            padding: 10px;
        }
        /* 基础按钮样式 */
        #FactoryMsgBox QPushButton {
            background-color: #e9e9e9;
            color: #333333;
            border: 1px solid #dcdcdc;
            border-radius: 4px;
            padding: 6px 18px;
            font-size: 13px;
            min-width: 80px;
            min-height: 28px;
            margin: 2px;
        }
        /* 悬停状态 - 鼠标放在按钮上 */
        #FactoryMsgBox QPushButton:hover {
            background-color: #d0d0d0;
            border-color: #b0b0b0;
        }
        /* 按下状态 - 鼠标点击按钮时 */
        #FactoryMsgBox QPushButton:pressed {
            background-color: #b8b8b8;
            border-color: #999999;
            /* 增加按压效果，模拟凹陷感 */
            padding-top: 7px;
            padding-bottom: 5px;
        }
        /* 默认按钮样式 */
        #FactoryMsgBox QPushButton:default {
            background-color: #e0e0e0;
            border-color: #b0b0b0;
        }
        /* 焦点状态 */
        #FactoryMsgBox QPushButton:focus {
            outline: 2px solid #90c8f6;
            outline-offset: -1px;
        }
        /* 禁用状态 */
        #FactoryMsgBox QPushButton:disabled {
            background-color: #f0f0f0;
            color: #999999;
            border-color: #e0e0e0;
        }
        #FactoryMsgBox QDialogButtonBox {
            background-color: transparent;
            padding: 10px;
        }
    )");

	// 强制应用按钮样式
	QList<QPushButton*> buttons = msgBox->findChildren<QPushButton*>();
	foreach(QPushButton* btn, buttons) {
		// 为每个按钮设置相同的样式表，确保状态变化生效
		btn->setStyleSheet(R"(
            QPushButton {
                background-color: #e9e9e9;
                color: #333333;
                border: 1px solid #dcdcdc;
                border-radius: 4px;
                padding: 6px 18px;
                font-size: 13px;
                min-width: 80px;
                min-height: 28px;
                margin: 2px;
            }
            QPushButton:hover {
                background-color: #d0d0d0;
                border-color: #b0b0b0;
            }
            QPushButton:pressed {
                background-color: #b8b8b8;
                border-color: #999999;
                padding-top: 7px;
                padding-bottom: 5px;
            }
            QPushButton:default {
                background-color: #e0e0e0;
                border-color: #b0b0b0;
            }
            QPushButton:focus {
                outline: 2px solid #90c8f6;
                outline-offset: -1px;
            }
            QPushButton:disabled {
                background-color: #f0f0f0;
                color: #999999;
                border-color: #e0e0e0;
            }
        )");
	}
}