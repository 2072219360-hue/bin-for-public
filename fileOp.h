#pragma once
#include <QString>
#include <QDir>
#include <fstream>
#include <string>
#include <regex>
namespace platforms 
{
	namespace detail 
	{
		class FileOp
		{
		public:
			static FileOp*instance;
			static FileOp*getInstance();
			bool removeDirectory(const QString &dirPath);
			bool copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist = 1);
			bool copyFile(const QString &toDir, const QString & fromFile, const QString & toFile, bool converFileIfExit = 1);
			/**
			* @brief 读取文件的最后几行数据
			* @param fileName,读取的文件路径
			* @param lines,读取倒数行的文本
			* @param result,最终结果进行返回
			* @param lineTag,一行结束的标记
			**/
			int readFileEndLines(QString fileName, int lines, QString& result);

			QString readFile(QString& fileName);
			std::string regexMatch(std::string str, std::regex reg, std::smatch& results);

		private:
			FileOp() = default;
			~FileOp() = default;
		};

		//FileOp* FileOp::instance = nullptr;
		//FileOp * FileOp::getInstance()
		//{
		//	if (instance == nullptr)
		//	{
		//		instance = new FileOp();
		//	}
		//	return instance;
		//}

		//bool FileOp::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
		//{
		//	QDir sourceDir(fromDir);
		//	QDir targetDir(toDir);
		//	if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
		//		if (!targetDir.mkdir(targetDir.absolutePath()))
		//			return false;
		//	}

		//	QFileInfoList fileInfoList = sourceDir.entryInfoList();
		//	foreach(QFileInfo fileInfo, fileInfoList) {
		//		if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
		//			continue;

		//		if (fileInfo.isDir()) {    /**< 当为目录时，递归的进行copy */
		//			if (!copyDirectoryFiles(fileInfo.filePath(),
		//				targetDir.filePath(fileInfo.fileName()),
		//				coverFileIfExist))
		//				return false;
		//		}
		//		else {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
		//			if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
		//				targetDir.remove(fileInfo.fileName());
		//			}

		//			/// 进行文件copy
		//			if (!QFile::copy(fileInfo.filePath(),
		//				targetDir.filePath(fileInfo.fileName()))) {
		//				return false;
		//			}
		//		}
		//	}
		//	return true;
		//}
	}
}