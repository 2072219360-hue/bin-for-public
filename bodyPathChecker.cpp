#include "bodyPathChecker.h"
#include <QDir>
#include <QFileInfo>
#include <QStringList>

QString BodyPathChecker::checkAndGetPath(const QString& originalPath) {
	//return originalPath;
	// 获取文件所在目录路径
	QFileInfo fileInfo(originalPath);
	QString directoryPath = fileInfo.path();

	// 检查目录是否存在
	if (QDir(directoryPath).exists()) {
		qDebug() << "truePath=" << originalPath;
		return originalPath;
	}
	else {
		// 目录不存在，返回移除body层级后的路径
		qDebug() << "truePath=" << removeBodyDirectory(originalPath);
		return removeBodyDirectory(originalPath);
	}
}

QString BodyPathChecker::removeBodyDirectory(const QString& originalPath) {
	// 将路径转换为统一的分隔符（使用'/'）
	QString normalizedPath = QDir::fromNativeSeparators(originalPath);

	// 分割路径组件
	QStringList pathComponents = normalizedPath.split('/', Qt::SkipEmptyParts);

	// 查找"body"组件的位置
	int bodyIndex = -1;
	for (int i = 0; i < pathComponents.size(); ++i) {
		if (pathComponents[i] == "body") {
			bodyIndex = i;
			break;
		}
	}

	// 如果找到"body"组件，则移除它
	if (bodyIndex != -1) {
		pathComponents.removeAt(bodyIndex);
	}

	// 重新组合路径
	QString newPath = pathComponents.join('/');

	// 转换为当前系统的路径分隔符
	return QDir::toNativeSeparators(newPath);
}