#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include "FileSystemWatcher.h"

//单例模式
//FileSystemWatcher* FileSystemWatcher::getInstance()
//{
//	static QScopedPointer<FileSystemWatcher> _instance;
//	if (_instance.isNull()) {
//		static QMutex mutex;
//		QMutexLocker locker(&mutex);
//		if (_instance.isNull()) {
//			_instance.reset(new FileSystemWatcher);
//		}
//	}
//	return _instance.data();
//}


FileSystemWatcher::FileSystemWatcher(QObject* parent) :QFileSystemWatcher(parent)
{
    connect(this, &FileSystemWatcher::directoryChanged, this, &FileSystemWatcher::slotsDirectoryUpdated);
}

FileSystemWatcher::FileSystemWatcher(const QString& path, QObject* parent) :QFileSystemWatcher(QStringList(path), parent)
{
	qDebug() << QString("FileSystemWatcher to watch: %1").arg(path);
    QFileSystemWatcher::addPath(path);
    QDir dir(path);
	//QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst
    dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    m_fileEntryList = dir.entryList();

    connect(this, &FileSystemWatcher::directoryChanged, this, &FileSystemWatcher::slotsDirectoryUpdated);
}

//添加路径监视
void FileSystemWatcher::addWatchPath(QString path)
{
	qDebug() << QString("addWatchPath to watch: %1").arg(path);
    if (!directories().isEmpty()) {
        removePath(directories().first());
    }
    QFileSystemWatcher::addPath(path);

    QDir dir(path);
	//QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst
    dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);
    m_fileEntryList = dir.entryList();
}

// 只要任何监控的目录更新（添加、删除、重命名）都会调用。
void FileSystemWatcher::slotsDirectoryUpdated(const QString& path)
{
	//qDebug() << QString("Directory updated: %1").arg(path);
	//// 比较最新的内容和保存的内容找出区别(变化)
	//QStringList currEntryList = m_currentContentsMap[path];
	//const QDir dir(path);
	//QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
 //   QSet<QString> newDirSet(newEntryList.begin(), newEntryList.end());
	//QSet<QString> currentDirSet(currEntryList.begin(), currEntryList.end());
	//// 添加了文件
	//QSet<QString> newFiles = newDirSet - currentDirSet;
	//QStringList newFile = newFiles.values();
	//// 文件已被移除
	//QSet<QString> deletedFiles = currentDirSet - newDirSet;
	//QStringList deleteFile = deletedFiles.values();
	//// 更新当前设置
	//m_currentContentsMap[path] = newEntryList;
	//if (!newFile.isEmpty() && !deleteFile.isEmpty())
	//{
	//	// 文件/目录重命名
	//	if ((newFile.count() == 1) && (deleteFile.count() == 1))
	//	{
	//		qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
	//	}
	//}
	//else
	//{
	//	// 添加新文件/目录至Dir
	//	if (!newFile.isEmpty())
	//	{
	//		qDebug() << "New Files/Dirs added: " << newFile;
	//		foreach(QString file, newFile)
	//		{
	//			// 处理操作每个新文件....
	//		}
	//	}
	//	// 从Dir中删除文件/目录
	//	if (!deleteFile.isEmpty())
	//	{
	//		qDebug() << "Files/Dirs deleted: " << deleteFile;
	//		foreach(QString file, deleteFile)
	//		{
	//			// 处理操作每个被删除的文件....
	//		}
	//	}
	//}

    QDir dir(path);
    dir.setFilter(QDir::NoDotAndDotDot | QDir::AllEntries);

    QStringList fileEntryList = dir.entryList();

    int previousFileCount = m_fileEntryList.count();
    int currentFileCount = fileEntryList.count();
    if (previousFileCount == currentFileCount) {
        for (int i = 0; i < previousFileCount; i++) {
            QString previousFileName = m_fileEntryList.at(i);
            if (!fileEntryList.contains(previousFileName)) {
                for (int j = 0; j < currentFileCount; j++) {
                    QString currentFileName = fileEntryList.at(j);
                    if (!m_fileEntryList.contains(currentFileName)) {
                        emit signalsFileNameChanged(previousFileName, currentFileName);
                        m_fileEntryList = fileEntryList;
                        return;
                    }
                }
            }
        }
    }
    else if (previousFileCount > currentFileCount) {
        for (int i = 0; i < currentFileCount; i++) {
            m_fileEntryList.removeOne(fileEntryList.at(i));
        }
        emit signalsFileRemoved(m_fileEntryList.first());
        m_fileEntryList = fileEntryList;
    }
    else {
        QStringList tempList = fileEntryList;
        for (int i = 0; i < previousFileCount; i++) {
            tempList.removeOne(m_fileEntryList.at(i));
        }
        emit signalsFileCreated(tempList.first());
        m_fileEntryList = fileEntryList;
    }
}
