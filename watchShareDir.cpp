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
    setWindowTitle(QStringLiteral("�ļ���ع��߲������ YapethsDY"));
    connect(&m_fileWatcher, &FileSystemWatcher::signalsFileNameChanged, this, &watchShareDir::onFileNameChanged);
    connect(&m_fileWatcher, &FileSystemWatcher::signalsFileCreated, this, &watchShareDir::onFileCreated);
    connect(&m_fileWatcher, &FileSystemWatcher::signalsFileRemoved, this, &watchShareDir::onFileRemoved);

    connect(ui.write, &QPushButton::clicked, this, &watchShareDir::slotsWrite);
    // ��Ӽ����ļ���·��
    path = "\\\\192.168.10.118/dyTest/";                //�����������ļ��� IP��ַд��       Ҫ�����ͬһ����
    //path = "\\\\DESKTOP-6RVKQA3/dyTest/";             //�����������ļ��� �������д��   ��Ҫ�����ͬһ����
    m_fileWatcher.addWatchPath(path);
}

watchShareDir::~watchShareDir()
{}

QString watchShareDir::setSingleGrabImagePath()
{
    QDateTime time = QDateTime::currentDateTime();
    QString currentTime = time.toString("yyyyMMddhhmmss");
    currentTime += "CameraGrab";
    QString fileName = QFileDialog::getSaveFileName(this, "���浱������ͼ���ļ�", currentTime + ".png", "Image files (*.bmp *.jpg *.pgm *.png *.tif);;All files (*.*)");
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
    //m_ScreenSavePath = QFileDialog::getSaveFileName(this, "�������ͼ���ļ�", currentTime + ".png", "Image files (*.bmp *.jpg *.pgm *.png *.tif);;All files (*.*)");
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
    //��ӡʱ��
    QString data_time = QDateTime::currentDateTime().toString("yyyy-MM-dd-hh:mm:ss.zzz\n");
    ui.textEdit->insertPlainText(data_time);
    QStringList currEntryList = m_currentContentsMap[path];
    const QDir dir(path);
    QStringList newEntryList = dir.entryList(QDir::NoDotAndDotDot | QDir::AllDirs | QDir::Files, QDir::DirsFirst);
    QSet<QString> newDirSet(newEntryList.begin(), newEntryList.end());
     QSet<QString> currentDirSet(currEntryList.begin(), currEntryList.end());
    // ������ļ�
    QSet<QString> newFiles = newDirSet - currentDirSet;
    QStringList newFile = newFiles.values();
    // �ļ��ѱ��Ƴ�
    QSet<QString> deletedFiles = currentDirSet - newDirSet;
    QStringList deleteFile = deletedFiles.values();
    // ���µ�ǰ����
    m_currentContentsMap[path] = newEntryList;
    //ui.textEdit->setText(m_currentContentsMap[path]);
    if (!newFile.isEmpty() && !deleteFile.isEmpty())
    {
        // �ļ�/Ŀ¼������
        if ((newFile.count() == 1) && (deleteFile.count() == 1))
        {
            qDebug() << QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first());
            ui.textEdit->setText(QString("File Renamed from %1 to %2").arg(deleteFile.first()).arg(newFile.first()));
        }
    }
    else
    {
        // ������ļ�/Ŀ¼��Dir
        if (!newFile.isEmpty())
        {
            //ui.textEdit->setText(newFile);
        }
        // ��Dir��ɾ���ļ�/Ŀ¼
        if (!deleteFile.isEmpty())
        {
            //qDebug() << "Files/Dirs deleted: " << deleteFile;
            foreach(QString file, deleteFile)
            {
                // �������ÿ����ɾ�����ļ�....
            }
        }
    }
}

void watchShareDir::onFileCreated(const QString& fileName)
{
    ui.textEdit->append(QStringLiteral("[%1]������һ�����ļ���%2")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        .arg(fileName));
}

void watchShareDir::onFileNameChanged(const QString& previousFileName, const QString& currentFileName)
{
    ui.textEdit->append(QStringLiteral("[%1]�޸���һ���ļ���%2 ---> %3")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        .arg(previousFileName)
        .arg(currentFileName));
}

void watchShareDir::onFileRemoved(const QString& fileName)
{
    ui.textEdit->append(QStringLiteral("[%1]ɾ����һ���ļ���%2")
        .arg(QDateTime::currentDateTime().toString("hh:mm:ss"))
        .arg(fileName));
}
