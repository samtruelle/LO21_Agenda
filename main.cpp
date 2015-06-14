#include "mainwindow.h"
#include <QApplication>
#include "Calendar.h"
#include "projetmanager.h"
#include "mainwindow.h"
#include <QDebug>



int main(int argc, char* argv[])
{
    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();*/

    ProjetManager pm = ProjetManager::getInstance();

    pm.ajouterProjet((QString)"test",QDate(2000, 1, 1),QDate(2000, 2, 1));

    qDebug()<<pm.getProjet((QString)"test").getId();


    return 0;

}
