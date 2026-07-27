#ifndef PARAMEDITDIALOG_H
#define PARAMEDITDIALOG_H

#include <QDialog>
#include <QDoubleSpinBox>
#include <QString>
#include <QDir>
#include <iostream>
#include <fstream>
#include <QSemaphore>
#include<PTGeneration.h>
#include "SignalInstance.h"
#include "json/json.h"
#include"updateBatchArgs.h"
#include<QString>
#include<QDir>
namespace platforms {
	namespace sspns {

		class ParamEditDialog : public QDialog
		{
			Q_OBJECT
		public:
			// 构造函数：接收初始参数值和工作区路径
			ParamEditDialog(double ma, double h, double aoa, double aob,
				const QString& workspacePath, int index,
				QWidget *parent = nullptr);

			// 获取编辑后的参数值
			double getMa() const;
			double getH() const;
			double getAOA() const;
			double getAOB() const;
			bool updateBatchConfigJson();
			bool updateBatchDir();
			bool checkBatchConfig();
		
			// 更新JSON配置文件
			bool updateJsonConfig();
			bool updateInputDat();
		private:
			QDoubleSpinBox* maSpin;
			QDoubleSpinBox* hSpin;
			QDoubleSpinBox* aoaSpin;
			QDoubleSpinBox* aobSpin;
			updateBatchArgs* updateDat;
			QString m_workspacePath;  // 工作区路径
			int m_index;
			// 添加原始参数私有变量（构造函数传入的值）
			double m_originalMa;      // 原始Ma值
			double m_originalH;       // 原始H值
			double m_originalAOA;     // 原始AOA值
			double m_originalAOB;     // 原始AOB值
			QSemaphore m_semaphore;
		private slots:
			void onSetJsonFinished();
		};

	} // namespace sspns
} // namespace platforms

#endif // PARAMEDITDIALOG_H
