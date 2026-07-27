#ifndef BODYPATHCHECKER_H
#define BODYPATHCHECKER_H

#include <QString>
#include<qDebug>
class BodyPathChecker {
public:
	// 检查路径中body目录是否存在，返回处理后的路径
	static QString checkAndGetPath(const QString& originalPath);

private:
	// 辅助函数：从原始路径中移除body目录层级
	static QString removeBodyDirectory(const QString& originalPath);
};

#endif // BODYPATHCHECKER_H