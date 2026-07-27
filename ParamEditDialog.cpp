#include "ParamEditDialog.h"
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QFont>
#include <QStyle>

namespace platforms {
	namespace sspns {

		ParamEditDialog::ParamEditDialog(double ma, double h, double aoa, double aob,
			const QString& workspacePath, int index,
			QWidget *parent)
			: QDialog(parent),
			m_workspacePath(workspacePath),
			m_index(index),
			// 初始化原始参数变量（存储构造时传入的初始值）
			m_originalMa(ma),
			m_originalH(h),
			m_originalAOA(aoa),
			m_originalAOB(aob)
		{
			setWindowTitle(u8"编辑参数");
			// 大幅增加窗口尺寸以适应超大字体
			setMinimumWidth(800);
			setMinimumHeight(600);

			updateDat = new updateBatchArgs();
			// 全局超大字体设置
			QFont defaultFont = font();
			defaultFont.setPointSize(20); // 基础字体大幅增加
			defaultFont.setBold(true);    // 加粗字体增强可读性
			setFont(defaultFont);

			// 样式表调整，适配超大字体
			setStyleSheet(R"(
				/* 对话框背景 */
				QDialog {
					background-color: #f0f5f9;
					padding: 20px;
				}
				
				/* 标签样式 - 超大字体 */
				QLabel {
					color: #1e272e;
					font-size: 24px; /* 标签字体超大 */
					font-weight: bold;
					padding: 15px 0;
					min-height: 50px;
				}
				
				/* 输入框样式 - 超大字体设置 */
				QDoubleSpinBox {
					background-color: #ffffff;
					border: 2px solid #b2bec3; /* 加粗边框配合大尺寸 */
					border-radius: 6px;
					padding: 18px 20px;
					font-size: 24px; /* 输入框字体超大 */
					color: #1e272e;
					min-height: 60px;  /* 大幅增加高度 */
					min-width: 300px; /* 大幅增加宽度 */
				}
				
				/* 确保没有增量按钮 */
				QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
					height: 0px;
					width: 0px;
				}
				
				QDoubleSpinBox:focus {
					border-color: #0984e3;
					outline: none;
					box-shadow: 0 0 0 3px rgba(9, 132, 227, 0.2);
				}
				
				/* 按钮样式 - 超大尺寸和字体 */
				QPushButton {
					background-color: #0984e3;
					color: white;
					border: none;
					border-radius: 6px;
					padding: 15px 30px;
					font-size: 22px; /* 按钮字体超大 */
					min-width: 150px; /* 按钮宽度大幅增加 */
					min-height: 60px; /* 按钮高度大幅增加 */
				}
				
				QPushButton:hover {
					background-color: #0652dd;
				}
				
				QPushButton:default {
					background-color: #218c74;
				}
				
				QPushButton:default:hover {
					background-color: #27ae60;
				}
			)");
			int decimal = 1;
			// 初始化输入控件
			maSpin = new QDoubleSpinBox(this);
			// 马赫数设置为大于0的浮点数，使用一个极小值作为下界
			maSpin->setRange(0, 1e9);  // 修改：将最小值从0改为接近0的极小值
			maSpin->setDecimals(decimal);
			maSpin->setValue(ma > 0 ? ma : 0.5);  // 确保初始值大于0
			maSpin->setFont(defaultFont);
			maSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);

			hSpin = new QDoubleSpinBox(this);
			hSpin->setRange(0, 1e9);
			hSpin->setDecimals(decimal);
			hSpin->setValue(h);
			hSpin->setFont(defaultFont);
			hSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);

			aoaSpin = new QDoubleSpinBox(this);
			// 攻角没有特别限制，保持原有范围但可根据实际需求调整
			aoaSpin->setRange(-360, 360);  // 修改：扩大攻角取值范围
			aoaSpin->setDecimals(decimal);
			aoaSpin->setValue(aoa);
			aoaSpin->setFont(defaultFont);
			aoaSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);

			aobSpin = new QDoubleSpinBox(this);
			// 侧滑角没有特别限制，保持原有范围但可根据实际需求调整
			aobSpin->setRange(-360, 360);  // 修改：扩大侧滑角取值范围
			aobSpin->setDecimals(decimal);
			aobSpin->setValue(aob);
			aobSpin->setFont(defaultFont);
			aobSpin->setButtonSymbols(QAbstractSpinBox::NoButtons);

			// 创建布局，适应大字体
			QFormLayout* formLayout = new QFormLayout();
			formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
			formLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
			formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
			formLayout->setSpacing(40); // 大幅增加行间距
			formLayout->setContentsMargins(50, 50, 50, 50); // 大幅增加边距

			formLayout->addRow(u8"马赫数Ma:", maSpin);
			formLayout->addRow(u8"高度H:", hSpin);
			formLayout->addRow(u8"攻角AOA:", aoaSpin);
			formLayout->addRow(u8"侧滑角AOB:", aobSpin);

			// 创建按钮盒
			QDialogButtonBox* buttonBox = new QDialogButtonBox(
				QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
				Qt::Horizontal,
				this
			);
			buttonBox->setCenterButtons(true);
			buttonBox->setFont(defaultFont);
			buttonBox->setStyleSheet("QDialogButtonBox { spacing: 40px; }"); // 加大按钮间距

			connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
			connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
			connect(SignalInstance::getInstance(), &SignalInstance::signalSetJsonFinished, this, &ParamEditDialog::onSetJsonFinished);
			// 主布局
			QVBoxLayout* mainLayout = new QVBoxLayout(this);
			mainLayout->addLayout(formLayout);
			mainLayout->addWidget(buttonBox);
			mainLayout->setContentsMargins(30, 30, 30, 30);
			mainLayout->setSpacing(60); // 大幅增加表单和按钮之间的距离

			setLayout(mainLayout);
		}

		double ParamEditDialog::getMa() const {
			return maSpin->value();
		}

		double ParamEditDialog::getH() const {
			return hSpin->value();
		}

		double ParamEditDialog::getAOA() const {
			return aoaSpin->value();
		}

		double ParamEditDialog::getAOB() const {
			return aobSpin->value();
		}

		bool ParamEditDialog::updateJsonConfig() {
			// 检查是否存在重复配置
			if (!checkBatchConfig()) {
				qDebug() << u8"存在重复的参数配置，无法更新";
				return false;
			}
			
			// 再更新文件夹名称
			if (!updateBatchDir()) {
				qDebug() << u8"更新文件夹名称失败";
				return false;
			}
			// 新增：更新 input.dat
			if (!updateInputDat()) {
				qDebug() << u8"更新 input.dat 失败";
				return false;
			}
			// 先更新JSON配置
			if (!updateBatchConfigJson()) {
				qDebug() << u8"更新batchConfig.json失败";
				return false;
			}
			return true;
		}
		bool ParamEditDialog::checkBatchConfig() {
			// 构建batchConfig.json文件路径
			QString batchConfigPath = m_workspacePath + "/Batch/batchConfig.json";
			qDebug() << "batchConfigPath=" << batchConfigPath;
			QFile file(batchConfigPath);

			// 尝试打开文件
			if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
				// 文件不存在或无法打开，视为出错
				return false;
			}

			// 读取文件内容
			QByteArray data = file.readAll();
			file.close();

			// 解析JSON
			Json::Reader reader;
			Json::Value root;
			if (!reader.parse(data.toStdString(), root)) {
				// JSON解析失败，返回false表示检查不通过
				return false;
			}

			// 获取当前编辑的参数值
			double currentMa = getMa();
			double currentH = getH();
			double currentAOA = getAOA();
			double currentAOB = getAOB();

			// 获取总案例数
			int caseNumber = root["case_number"].asInt();

			// 遍历所有案例检查是否存在重复
			for (int i = 0; i < caseNumber; ++i) {
				std::string caseKey = "case_" + std::to_string(i);
				if (!root.isMember(caseKey)) {
					continue; // 跳过不存在的案例配置
				}

				Json::Value caseData = root[caseKey];

				// 检查当前案例是否与编辑的参数完全一致（忽略自身案例）
				if (i != m_index &&
					caseData["Ma"].asDouble() == currentMa &&
					caseData["H"].asDouble() == currentH &&
					caseData["AOA"].asDouble() == currentAOA &&
					caseData["AOB"].asDouble() == currentAOB) {
					return false; // 发现重复配置
				}
			}

			// 未发现重复配置
			return true;
		}
		void ParamEditDialog::onSetJsonFinished() {
			m_semaphore.release();
		}
		bool ParamEditDialog::updateBatchConfigJson() {
			// 发送信号通知batchProcess更新JSON数据
			emit SignalInstance::getInstance()->signalSetJson(m_index, "Ma", getMa());
			m_semaphore.acquire(); // 等待更新完成

			emit SignalInstance::getInstance()->signalSetJson(m_index, "H", getH());
			m_semaphore.acquire();

			emit SignalInstance::getInstance()->signalSetJson(m_index, "AOA", getAOA());
			m_semaphore.acquire();

			emit SignalInstance::getInstance()->signalSetJson(m_index, "AOB", getAOB());
			m_semaphore.acquire();

			emit SignalInstance::getInstance()->signalSetJson(m_index, "last_run_time", std::string("2000-00-00"));
			m_semaphore.acquire();

			emit SignalInstance::getInstance()->signalSetJson(m_index, "status", 0);
			m_semaphore.acquire();

			emit SignalInstance::getInstance()->signalSetJson(m_index, "finish_head", false);
			m_semaphore.acquire();

			emit SignalInstance::getInstance()->signalSetJson(m_index, "finish_body", false);
			m_semaphore.acquire();

			return true;
		}
		bool ParamEditDialog::updateBatchDir() {
			// 构建Batch文件夹路径
			QString batchDirPath = m_workspacePath + "/Batch";
			QDir batchDir(batchDirPath);
			if (!batchDir.exists()) {
				qDebug() << u8"Batch文件夹不存在:" << batchDirPath;
				return false;
			}

			// 构建旧文件夹名称（使用原始参数）
			QString oldDirName = QString("Ma%1_H%2_AOA%3_AOB%4")
				.arg(m_originalMa, 0, 'f', 1)       // 原始Ma值（保留5位小数）
				.arg(m_originalH, 0, 'f', 1)        // 原始H值（保留5位小数）
				.arg(m_originalAOA, 0, 'f', 1)      // 原始AOA值（保留5位小数）
				.arg(m_originalAOB, 0, 'f', 1);     // 原始AOB值（保留5位小数）
			QString oldDirPath = batchDirPath + "/" + oldDirName;
			int decimal = 1;
			// 构建新文件夹名称（使用当前编辑的参数）
			QString newDirName = QString("Ma%1_H%2_AOA%3_AOB%4")
				.arg(getMa(), 0, 'f', decimal)            // 当前编辑的Ma值（保留5位小数）
				.arg(getH(), 0, 'f', decimal)             // 当前编辑的H值（保留5位小数）
				.arg(getAOA(), 0, 'f', decimal)           // 当前编辑的AOA值（保留5位小数）
				.arg(getAOB(), 0, 'f', decimal);          // 当前编辑的AOB值（保留5位小数）
			QString newDirPath = batchDirPath + "/" + newDirName;

			// 检查新文件夹是否已存在
			if (QDir(newDirPath).exists()) {
				qDebug() << u8"新文件夹已存在:" << newDirName;
				return false;
			}

			// 重命名文件夹（仅当原始文件夹存在时）
			QDir oldDir(oldDirPath);
			if (oldDir.exists()) {
				/*if (QDir::currentPath() == "") {
					QDir::setCurrent(SignalInstance::getWorkSpace());
				}*/
				QString currentPath = QDir::currentPath();
				bool flag = currentPath.startsWith(oldDirPath);
				if (flag) {
					QDir::setCurrent(SignalInstance::getWorkSpace());
				}
				if (!batchDir.rename(oldDirName, newDirName)) {
					qDebug() << u8"文件夹重命名失败:" << oldDirName << "->" << newDirName;
					qDebug() << u8"文件夹重命名失败:" << oldDirPath << "->" << newDirPath;
					qDebug() << "current path=" << currentPath;
					return false;
				}
			}
			else {
				qDebug() << u8"原始文件夹不存在，无需重命名:" << oldDirName;
				return false;
			}

			return true;
		}
		bool ParamEditDialog::updateInputDat() {
			// 获取当前编辑的参数值
			double currentMa = getMa();
			double currentH = getH();
			double currentAOA = getAOA();
			double currentAOB = getAOB();
			int decimal = 1;
			// 构建目标文件夹路径
			QString targetDirName = QString("Ma%1_H%2_AOA%3_AOB%4")
				.arg(currentMa, 0, 'f', decimal)
				.arg(currentH, 0, 'f', decimal)
				.arg(currentAOA, 0, 'f', decimal)
				.arg(currentAOB, 0, 'f', decimal);
			QString targetDirPath = m_workspacePath + "/Batch/" + targetDirName;

			// 检查目标文件夹是否存在
			QDir targetDir(targetDirPath);
			if (!targetDir.exists()) {
				qDebug() << u8"目标文件夹不存在:" << targetDirPath;
				return false;
			}

			// 调用updateBatchArgs更新input.dat和headconfig.dat
			// 注意：这里需要根据实际项目结构调整工作空间路径的传递方式
			updateDat->updateInputDat(targetDirPath, currentH, currentMa, currentAOA, currentAOB);

			return true;
		}
	} // namespace sspns
} // namespace platforms
