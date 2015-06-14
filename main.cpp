#include "mainwindow.h"
#include <QApplication>
#include "Calendar.h"
#include "frabiquesingleton.h"
#include "projetmanager.h"


int main(int argc, char *argv[])
{

    /*QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();*/


    ProjetManager test = FabriqueSingleton<ProjetManager,Projet>::getInstance();

    return 0;
}
