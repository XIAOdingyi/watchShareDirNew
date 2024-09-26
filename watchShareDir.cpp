#include <QDateTime>
#include <QFileDialog>
#include <QDebug>
#include <QScreen>
#include <QApplication>
#include "watchShareDir.h"

watchShareDir::watchShareDir(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    setWindowTitle(QStringLiteral("文件监控工具测试软件 YapethsDY"));
    connect(&m_fileWatcher, &FileSystemWatcher::signalsFileNameChanged, this, &watchShareDir::onFileNameChanged);
    connect(&m_fileWatcher, &FileSystemWatcher::signalsFileCreated, this, &watchShareDir::onFileCreated);
    connect(&m_fileWatcher, &FileSystemWatcher::signalsFileRemoved, this, &watchShareDir::onFileRemoved);

    connect(ui.write, &QPushButton::clicked, this, &watchShareDir::slotsWrite);
    // 添加监视文件的路径
    path = "\\\\192.168.10.118/dyTest/";                //局域网共享文件夹 IP地址写法       要求必须同一网段
    //path = "\\\\DESKTOP-6RVKQA3/dyTest/";             //局域网共享文件夹 计算机名写法   不要求必须同一网段
    m_fileWatcher.addWatchPath(path);
}

watchShareDir::~watchShareDir()
{}

QString watchShareDir::setSingleGrabImagePath()
{
    QDateTime time = QDateTime::currentDateTime();
    QString currentTime = time.toString("yyyyMMddhhmmss");
    currentTime += "CameraGrab";
    QString fileName = QFileDialog::getSaveFileName(this, "保存当次任务图像文件", currentTime + ".png", "Image files (*.bmp *.jpg *.pgm *.png *.tif);;All files (*.*)");
    return fileName;   
}

QPixmap watchShareDir::getScreenPixmap()
{
    QDateTime time = QDateTime::currentDateTime();
    QString currentTime = time.toString("yyyyMMddhhmmss");
    currentTime += "ScreenShot";
    //DESKTOP-6RVKQA3
    m_ScreenSavePath = "\\\\DESKTOP-6RVKQA3/dyTest/" + currentTime + ".png";
    //m_ScreenSavePath = "D://dytest1//" + currentTime + ".png";
    //m_ScreenSavePath = "\\\\192.168.10.120/dytest1/" + currentTime + ".png";
    //m_ScreenSavePath = QFileDialog::getSaveFileName(this, "保存截屏图像文件", currentTime + ".png", "Image files (*.bmp *.jpg *.pgm *.png *.tif);;All files (*.*)");
    QScreen* screen = QApplication::primaryScreen();
    QPixmap pixmap = screen->grabWindow(winId(), 0, 0, width(), height());
    qDebug() << m_ScreenSavePath;
    return pixmap;
}

void watchShareDir::timerEvent(QTimerEvent* event)
{
    slotsRead();
}

void watchShareDir::slotsRead()
{

}

void watchShareDir::slotsWrite()
{
    QPixmap pix = getScreenPixmap();
    if (!pix.isNull()) {
        pix.save(m_ScreenSavePath,"PNG",100);
    }
}

void watchShareDir::slotImageDisplay()
{
    //打印时间
    QString data_time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss.zzz\n");
    ui.textEdit->insertPlainText(data_time);
    QStringList currEntryList = m_currentContentsMap[path];
    const QDir dir(path);
    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    QSet<QString> newDirSet(newEntryList.begin(), newEntryList.end());
     QSet<QString> currentDirSet(currEntryList.begin(), currEntryList.end());
    // 添加了文件
    QSet<QString> newFiles = newDirSet - currentDirSet;
    QStringList newFile = newFiles.values();
    // 文件已被移除
    QSet<QString> deletedFiles = currentDirSet - newDirSet;
    QStringList deleteFile = deletedFiles.values();
    // 更新当前设置
    m_currentContentsMap[path] = newEntryList;
    //ui.textEdit->setText(m_currentContentsMap[path]);
    if (!newFile.isEmpty() && !deleteFile.isEmpty())
    {
        // 文件/目录重命名
        if ((newFile.count() == 1) && (deleteFile.count() == 1))
        {
            qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
            ui.textEdit->setText(QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first()));
        }
    }
    else
    {
        // 添加新文件/目录至Dir
        if (!newFile.isEmpty())
        {
            //ui.textEdit->setText(newFile);
        }
        // 从Dir中删除文件/目录
        if (!deleteFile.isEmpty())
        {
            //qDebug() << "Files/Dirs deleted: " << deleteFile;
            foreach(QString file, deleteFile)
            {
                // 处理操作每个被删除的文件....
            }
        }
    }
}

void watchShareDir::onFileCreated(const QString& fileName)
{
    ui.textEdit->append(QStringLiteral("[%1]创建了一个新文件：%2")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        .arg(fileName));
}

void watchShareDir::onFileNameChanged(const QString& previousFileName, const QString& currentFileName)
{
    ui.textEdit->append(QStringLiteral("[%1]修改了一个文件：%2 ---> %3")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        .arg(previousFileName)
        .arg(currentFileName));
}

void watchShareDir::onFileRemoved(const QString& fileName)
{
    ui.textEdit->append(QStringLiteral("[%1]删除了一个文件：%2")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        .arg(fileName));
}
