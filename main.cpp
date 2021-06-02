#include "fsaquawindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FSAquaWindow w;
    w.show();

    return a.exec();
}
