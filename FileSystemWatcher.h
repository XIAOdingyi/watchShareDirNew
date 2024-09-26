#ifndef FILE_SYSTEM_WATCHER_H
#define FILE_SYSTEM_WATCHER_H

#include <QObject>
#include <QMap>
#include <QFileSystemWatcher>
/*
* ����Ŀ��Ҫ��QT����д�����ļ���
* �����ļ��е��趨�ɰٶ�
* ·������Ϊ "\\\\�����ļ��е���IP/�����ļ���·��/"����.
* ·��Ҳ��������Ϊ "\\\\�����ļ��е��Լ������/�����ļ���·��/"
* ����ipʱ����ͬһ�����޷����ʹ����ļ��У����õ��Լ������ʱ��ʹ����ͬһ����Ҳ���Է���
* �ѵ��
*		�ڼ�ؾ������ڵĹ����ļ���ʱdirectoryChanged�źŻᴥ�����Σ���δ���
* �ɼ��Ŀ¼��ɼ��Ŀ¼�µ��ļ�
* 2024�޸� ȥ������ģʽ
* ���ļ��仯�źŽ��в�֣�����������ɾ�����޸����ַ�ʽ
*/
class QStringList;
class FileSystemWatcher : public QFileSystemWatcher
{
	Q_OBJECT
public:
    explicit FileSystemWatcher(QObject* parent = Q_NULLPTR);
    explicit FileSystemWatcher(const QString& path, QObject* parent = Q_NULLPTR);
public:
    //���Ӽ���·��
    void addWatchPath(QString path);
signals:
    //�ļ������ź�
    void signalsFileCreated(const QString& fileName);
    //�ļ����ı��ź�
    void signalsFileNameChanged(const QString& previousFileName, const QString& currentFileName);
    //�ļ�ɾ���ź�
    void signalsFileRemoved(const QString& fileName);
public slots:	
	void slotsDirectoryUpdated(const QString& path);  
private:
	// ��ǰÿ����ص�����Ŀ¼�б�
	//QMap<QString, QStringList> m_currentContentsMap; 
    QStringList m_fileEntryList;
};
#endif// FILE_SYSTEM_WATCHER_H