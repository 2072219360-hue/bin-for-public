#include "fileOp.h"
#include <QDebug>
namespace platforms
{
	namespace detail
	{
		FileOp*FileOp::instance = nullptr;
		FileOp*FileOp::getInstance()
		{
			
				if (instance == nullptr)
				{
					instance = new FileOp();
				}
				return instance;
			
		}
		bool FileOp ::removeDirectory(const QString &dirPath) {
			QDir dir(dirPath);
			if (!dir.exists()) {
				qDebug() << "Directory does not exist:" << dirPath;
				return false;
			}
			// Get all entries in the directory
			QFileInfoList fileInfoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::AllEntries);
			foreach(const QFileInfo &fileInfo, fileInfoList) {
				if (fileInfo.isDir()) {
					// Recursively remove subdirectory
					if (!removeDirectory(fileInfo.absoluteFilePath())) {
						return false;
					}
				}
				else {
					// Remove file
					if (!QFile::remove(fileInfo.absoluteFilePath())) {
						qDebug() << "Failed to remove file:" << fileInfo.absoluteFilePath();
						return false;
					}
				}
			}

			// Remove the empty directory itself
			if (!dir.rmdir(dirPath)) {
				qDebug() << "Failed to remove directory:" << dirPath;
				return false;
			}

			return true;
		}

		bool FileOp::copyDirectoryFiles(const QString &fromDir, const QString &toDir, bool coverFileIfExist)
		{
			
				QDir sourceDir(fromDir);
				QDir targetDir(toDir);
				if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
					if (!targetDir.mkdir(targetDir.absolutePath()))
						return false;
				}

				QFileInfoList fileInfoList = sourceDir.entryInfoList();
				foreach(QFileInfo fileInfo, fileInfoList) {
					if (fileInfo.fileName() == "." || fileInfo.fileName() == "..")
						continue;

					if (fileInfo.isDir()) {    /**< 当为目录时，递归的进行copy */
						if (!copyDirectoryFiles(fileInfo.filePath(),
							targetDir.filePath(fileInfo.fileName()),
							coverFileIfExist))
							return false;
					}
					else {            /**< 当允许覆盖操作时，将旧文件进行删除操作 */
						if (coverFileIfExist && targetDir.exists(fileInfo.fileName())) {
							targetDir.remove(fileInfo.fileName());
						}

						/// 进行文件copy
						if (!QFile::copy(fileInfo.filePath(),
							targetDir.filePath(fileInfo.fileName()))) {
							return false;
						}
					}
				}
				return true;
			
		}
		bool FileOp::copyFile(const QString &toDir, const QString & fromFile, const QString & toFile, bool converFileIfExit)
		{
			QDir targetDir(toDir);
			if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
				if (!targetDir.mkdir(targetDir.absolutePath()))
					return false;
			}
			QFileInfo fileInfo(toFile);
			if (converFileIfExit && targetDir.exists(toFile)) {
				//targetDir.remove(toFile);
				QFile::remove(toDir+"/"+toFile);
			}

			/// 进行文件copy
			if (!QFile::copy(fromFile, toDir + "/" + toFile)) {
				return false;
			}
			return true;


		}
		
		int platforms::detail::FileOp::readFileEndLines(QString fileName, int lines, QString & result)
		{
			std::ifstream  File(fileName.toLocal8Bit().toStdString(), std::ios::ate);
			//std::ifstream  fin("/var/log/test.log", std::ios::ate);
			if (!File)
			{
				//cout << "cannot open file！";
				return -1;
			}
			// 先倒回文件末尾两个字符
			File.seekg(-2, File.cur);
			// 假定反向读第3行的记录
			int lineCount = lines;
			for (int i = 0; i < lineCount; i++)
			{
				// 查看前一个字符是否为回车符
				while (File.peek() != File.widen('\n'))
				{
					File.seekg(-1, File.cur);
				}
				// 走到这里表示跳过一行了，所以继续向前跳直到到lines行
				File.seekg(-1, File.cur);
			}

			File.seekg(2, File.cur);
			std::string  line;
			
			//如果想将倒数lineCount行都输出，咋加入循环while
			getline(File, line);//输出为‘\n’
			getline(File, line);
			result = QString::fromStdString(line);
			qDebug() << "fileName:"<<fileName <<" readLine:" << result;
			File.clear();
			File.close();
			return 0;

		}

		QString FileOp::readFile(QString& fileName)
		{
			QFile file(fileName);
			QString fileContent = "";
			file.open(QFile::ReadOnly);//以只读的方式读取文件
			if (file.isOpen()) {
				fileContent = QLatin1String(file.readAll());
			}
			file.close();
			return fileContent;
		}
		std::string FileOp::regexMatch(std::string str, std::regex reg, std::smatch& results)
		{
			//string text = "Nsc:Ip:Nst=1:21:2330Xnew= 0.233965     Step=  0.4996D-04";
			//cout << text << endl;
			//regex pattern("(.*)guid=(.*?),.*");
			//regex pattern("\s[Nsc:Ip:Nst] = [\s\d:\s(.*?):(.*)]");
			std::regex pattern = reg;
			//std::smatch results;
			regex_match(str, results, pattern);
			for (auto it = results.begin(); it != results.end(); ++it)
				qDebug() << QString::fromStdString(*it);
			if (results.size() < 3)return  "";
			return results[2];
			/*if (regex_match(str, results, pattern))
				for (auto it = results.begin(); it != results.end(); ++it)
					cout << *it << endl;
			else
				cout << "match failed: " << text << endl;
			cout << *(results.begin() + 2);*/
		}
	}	
}