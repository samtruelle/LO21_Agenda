/*
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Calendar.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(afficherT(Tache& t)));
    //QMetaObject::connectSlotsByName(this);

    ui->EmploiDuT->setRowCount(24);
    ui->EmploiDuT->setColumnCount(7);

    QStringList jours;
    jours << "lundi" << "mardi" << "mercredi" << "jeudi" << "vendredi" << "samedi" << "dimanche";
    ui->EmploiDuT->setHorizontalHeaderLabels(jours);
    QStringList heure;
    heure << "00h00" << "01h00" << "02h00" << "03h00" <<  "04h00" << "05h00" << "06h00" << "07h00" << "08h00" << "09h00" << "10h00" << "11h00" << "12h00" << "13h00" << "14h00" << "15h00" << "16h00" << "17h00" << "18h00" << "19h00" << "20h00" << "21h00" << "22h00" << "23h00" << "24h00";
    ui->EmploiDuT->setVerticalHeaderLabels(heure);
    for(int i =0; i < 24; i++){
        for(int j =0; j < 7; j++){
            ui->EmploiDuT->setItem(i, j, new QTableWidgetItem);

        }
    }
    ui->EmploiDate->setDate(QDate::currentDate());

}


MainWindow::~MainWindow()
{
    delete ui;

}

void MainWindow::afficherProgrammation(Programmation &p)
{
    int date = p.getDate().dayOfWeek();
    int i=0;
    ui->Retour_2->appendPlainText( p.getHoraire().toString("h"));
    for( i = p.getHoraire().toString("h").toInt(); i <p.getfin().toString("h").toInt(); i++)
    {
        ui->Retour_2->appendPlainText("ghggh");
    }
}
*/
