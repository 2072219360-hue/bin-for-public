#pragma once
#include <QPushButton>
#include <QTableWidget>
#include <QLabel>
#include <QLineEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QTableView>
#include <QHeaderView>
#include <QWidget>
#include <iostream>
#include<QFile>
#include "json/json.h"
#include <QDebug>
#include <string>
#include <iostream>
#include <fstream>
#include "SignalInstance.h"
#include "fileOp.h"
#include"ParamSetDialog.h"
#include"batchCommon.h"
#include "PTGeneration.h"
#include"updateBatchArgs.h"
#include"bodyPathChecker.h"
#include"USStandardAtmosphere.h"
#include<QItemDelegate>
#include<QFileDialog>
#include<QMap>
#define FIlEINSTANCE platforms::detail::FileOp::getInstance()
namespace platforms {
	namespace sspns {
		namespace detail {
			// 小数输入验证委托（限制1位小数）
			class OneDecimalDelegate : public QItemDelegate {
				Q_OBJECT
			public:
				explicit OneDecimalDelegate(QObject *parent = nullptr) : QItemDelegate(parent) {}

				QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override {
					QLineEdit *editor = new QLineEdit(parent);
					// 设置验证器：范围-100000到100000，保留1位小数
					QDoubleValidator *validator = new QDoubleValidator(-100000.0, 100000.0, 1, editor);
					validator->setNotation(QDoubleValidator::StandardNotation);
					editor->setValidator(validator);
					return editor;
				}
			};
            class BatchConfigWidget:public QWidget
            {
				Q_OBJECT
            public:
				BatchConfigWidget(int ns_flag);
                ~BatchConfigWidget();
				bool updateInput();
                //初始化w
				void init();
                //初始化界面
				void initControl();
                //初始化信号与槽
				void initConnection();
				void initTableWidget(QTableWidget *tableWidget, QStringList headerList);
				//生成批量计算信息文件
				bool generateBatchFile();
				//生成批量处理文件夹
				void generateBatchDirs();
				//复制资源到特定文件夹
				void copyBatchRes(const QString& fromDir, const QString& toPath);
				void copyBatchFiles(QString toPath);
				void setParamSetDialog(ParamSetDialog* _dialog);
				int nsFlag;
            public slots:
				void onUpdateTable();
				void onLoadEx();
                //加载数据
				void onLoadData();
                //生成算例
				void onGenerateEx();

            private:
				OneDecimalDelegate *m_oneDecimalDelegate; // 新增委托对象
				int ns_flag;
                QLabel* m_btnExampleNum;
                QLineEdit* m_inputExampleNum;
                QPushButton* m_inputExOk;
				updateBatchArgs* updateArg;
				int m_maxNum;
                QStringList m_tableHeader;
				//QMap<QString, int>m_header2Column;
				QTableWidget* m_batchTabel;
				QWidget* m_batchTabelWidget;
                //加载数据
                QPushButton* m_btnLoadData;
                //更新数据
                QPushButton* m_btnUpdateData;
                //生成算例
                QPushButton* m_btnGenerateEx;
				//记载文件数据
				QPushButton* m_btnLoadEx;
                QVBoxLayout* m_mainLayout;
				ParamSetDialog* m_paramSetDialog;
                int m_exampleNum;
                int m_width;
                int m_height;
				int decimal;
				QMap<double, double> hp,ht;
				USStandardAtmosphere ptComputer;
            };
        }
    }
}