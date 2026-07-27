#include "FileOpUtils.h"


FileOpUtils* FileOpUtils::instance = nullptr;
FileOpUtils * FileOpUtils::getInstance()
{
	if (instance == nullptr)
	{
		instance = new FileOpUtils();
	}
	return instance;
}

bool FileOpUtils::removeFile(const QString path)
{
	auto isExits = FileOpUtils::getInstance()->fileExist(path);
	if (!isExits)
	{
		return false;
	}
	if (remove(path.toLocal8Bit().toStdString().c_str()) != 0) { // 尝试删除文件
		return false;
	}
	else {
		return true;
	}
}

QStringList FileOpUtils::getFileContent(const QString path)
{
	QStringList result;
	std::ifstream infile(path.toLocal8Bit().toStdString());
	if (!infile.is_open()) {
		QMessageBox::information(nullptr, u8"提示", u8"无法打开" + path);
		return result;
	}

	std::string line;

	while (std::getline(infile, line))
	{
		result << QString::fromStdString(line);
	}
	return result;
}

QString FileOpUtils::fileToSuffix(QString path)
{
	QString fileName = pathToFileName(path);
	QString suffix = "";
	for (int i = fileName.size() - 1; i >= 0; i--)
	{
		if (fileName[i] == '.')
		{
			break;
		}
		suffix.push_front(fileName[i]);
	}
	return suffix;
}

QString FileOpUtils::fileToPrefix(QString path)
{
	QString fileName = pathToFileName(path);
	QString prefix = "";
	for (int i = 0; i < fileName.size(); i++)
	{
		if (fileName[i] == '.')
		{
			break;
		}
		prefix.push_back(fileName[i]);
	}
	return prefix;
}

FileOpUtils::FileOpUtils()
{
}

FileOpUtils::~FileOpUtils()
{
}

QString FileOpUtils::pathToFileName(QString path)
{
	QString fileName = "";
	int i;
	for (i = path.size() - 1; i >= 0; i--)
	{
		if (path[i] == '/')
		{
			break;
		}
	}
	auto str = path.toLocal8Bit().toStdString();
	auto strfileName = str.substr(i + 1, str.size() - i - 1);
	fileName = QString::fromStdString(strfileName);
	return fileName;
}

bool FileOpUtils::isExistInDir(const QString& Dir, const QString& File)
{
	int index = File.indexOf(Dir, 0, Qt::CaseInsensitive);
	if (index == -1)
	{
		return false;
	}
	return true;
}

// 将文件从fromFile复制到toDir中，取名为toFile
bool FileOpUtils::copyFile(const QString & toDir, const QString & fromFile, const QString & toFile, bool converFileIfExit)
{
	if (isExistInDir(toDir + "/", fromFile))
	{
		return true;
	}
	QDir targetDir(toDir);
	if (!targetDir.exists()) {    /**< 如果目标目录不存在，则进行创建 */
		if (!targetDir.mkdir(targetDir.absolutePath()))
			return false;
	}

	QFileInfo fileInfo(toFile);
	if (converFileIfExit && targetDir.exists(toFile)) {
		targetDir.remove(toFile);
	}

	/// 进行文件copy
	if (!QFile::copy(fromFile, toDir + "/" + toFile)) {
		return false;
	}
	return true;
}



bool FileOpUtils::fileExist(const QString filePath)
{
	QFile file(filePath);
	return file.exists();
}

bool FileOpUtils::removeAllFileOnDir(const QString path)
{
	QDir Dir(path);
	if (Dir.isEmpty())
	{
		return true;
	}

	// 第三个参数是QDir的过滤参数，这三个表示收集所有文件和目录，且不包含"."和".."目录。
	// 因为只需要遍历第一层即可，所以第四个参数填QDirIterator::NoIteratorFlags
	QDirIterator DirsIterator(path, QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot, QDirIterator::NoIteratorFlags);
	while (DirsIterator.hasNext())
	{
		if (!Dir.remove(DirsIterator.next())) // 删除文件操作如果返回否，那它就是目录
		{
			QDir(DirsIterator.filePath()).removeRecursively(); // 删除目录本身以及它下属所有的文件及目录
		}
	}
}

