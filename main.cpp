#include "ui/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    // set the wndow size
    w.setFixedSize(800, 800);

    w.show();
    return QApplication::exec();
}
