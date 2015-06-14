
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Calendar.h"
)

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
void MainWindow::displayProgrammation(const Programmation &p)
{
    int date = p.getDate().dayOfWeek() -1;
    int i=0;
    for( i = p.getHoraire().toString("h").toInt(); i <p.getfin().toString("h").toInt(); i++)
    {
        ui->EmploiDuT->item(i, date)->setBackgroundColor(Qt::red);
    }
}

void MainWindow::update()
{
    ProgrammationManager &p = ProgrammationManager::getInstance();
    MainWindow::afficherEdt();
    this->afficherEvt();
}
void MainWindow::on_AjouterTache_clicked(){
  try{
    TacheManager &m = TacheManager::getInstance();

    if(ui->Preempt->isChecked()){
    m.(ui->titreTache->text(),ui->dispoTache->date(),ui->echeanceTache->date(),ui->DureeActivite->value());
    }
    else if (ui->composite->isChecked()){
        m.(ui->idTache->text(),ui->titre->text(), ui->dispoTache->date(),ui->echeanceTache->date());

    }
    Tache t  =m.getTache(ui->idTache->text());
    ui->Retour->appendPlainText(t.afficherTache());


    }catch(CalendarException e){

ui->Retour->appendPlainText("Erreur ajout tache ");
    }

}

MainWindow::~MainWindow()
{
    delete ui;

}

