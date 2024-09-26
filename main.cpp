#include "watchShareDir.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    watchShareDir w;
    w.show();
    return a.exec();
}
