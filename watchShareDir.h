#pragma once

#include <QtWidgets/QWidget>
#include "ui_watchShareDir.h"
#include <QFileSystemWatcher>
#include <QTime>
#include "FileSystemWatcher.h"


class watchShareDir : public QWidget
{
    Q_OBJECT

public:
    watchShareDir(QWidget *parent = nullptr);
    ~watchShareDir();
    QString setSingleGrabImagePath();
    QPixmap getScreenPixmap();
protected:
    void timerEvent(QTimerEvent* event);
public slots:
    void slotsRead();
    void slotsWrite();
    void slotImageDisplay();
    void onFileCreated(const QString& fileName);
    void onFileNameChanged(const QString& previousFileName, const QString& currentFileName);
    void onFileRemoved(const QString& fileName);
private:
    Ui::watchShareDirClass ui;
    QString m_ScreenSavePath = "";
    QString path = "";
    int time_ID1 = -1;
    QFileSystemWatcher* m_pluginWatcher = nullptr;
    QMap<QString, QStringList> m_currentContentsMap;
    FileSystemWatcher m_fileWatcher;
};
