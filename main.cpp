#include "mainwindow.h"
#include <QApplication>
#include "Calendar.h"
#include "projetmanager.h"
#include "mainwindow.h"
#include <QDebug>



int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();

    return 0;

}
