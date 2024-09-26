#ifndef FILE_SYSTEM_WATCHER_H
#define FILE_SYSTEM_WATCHER_H

#include <QObject>
#include <QMap>
#include <QFileSystemWatcher>
/*
* 因项目需要用QT来读写共享文件夹
* 共享文件夹的设定可百度
* 路径设置为 "\\\\共享文件夹电脑IP/共享文件夹路径/"即可.
* 路径也可以设置为 "\\\\共享文件夹电脑计算机名/共享文件夹路径/"
* 设置ip时不在同一网段无法访问共享文件夹，设置电脑计算机名时即使不在同一网段也可以访问
* 难点项：
*		在监控局域网内的共享文件夹时directoryChanged信号会触发两次，暂未解决
* 可监控目录亦可监控目录下的文件
* 2024修改 去掉单例模式
* 将文件变化信号进行拆分，区别新增、删除、修改三种方式
*/
class QStringList;
class FileSystemWatcher : public QFileSystemWatcher
{
	Q_OBJECT
public:
    explicit FileSystemWatcher(QObject* parent = Q_NULLPTR);
    explicit FileSystemWatcher(const QString& path, QObject* parent = Q_NULLPTR);
public:
    //增加监视路径
    void addWatchPath(QString path);
signals:
    //文件创建信号
    void signalsFileCreated(const QString& fileName);
    //文件名改变信号
    void signalsFileNameChanged(const QString& previousFileName, const QString& currentFileName);
    //文件删除信号
    void signalsFileRemoved(const QString& fileName);
public slots:	
	void slotsDirectoryUpdated(const QString& path);  
private:
	// 当前每个监控的内容目录列表
	//QMap<QString, QStringList> m_currentContentsMap; 
    QStringList m_fileEntryList;
};
#endif// FILE_SYSTEM_WATCHER_H