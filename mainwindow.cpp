
#include "mainwindow.h"
#include "ui_mainwindow.h"


#include"Calendar.h"
#include "projetmanager.h"
#include"programmationmanager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow),firstDay(0), firstDate(QDate(2015, 1, 1)),
    em(EvenementManager::getInstance()),currentProjet(0),projm(ProjetManager::getInstance())
{
    ui->setupUi(this);



// Partie Projet - Gestion projets
QObject::connect( ui->AjouterProjet , SIGNAL (clicked()), this, SLOT (nouveauProjet()));
// Partie Projet - Gestion tâches
QObject::connect(ui->AjouterTache, SIGNAL (clicked()), this, SLOT (ajouterTache()));
QObject::connect(ui->ProgrammeTache, SIGNAL (clicked()), this, SLOT (ProgrammerTache()));
// Gestion Evenements
QObject::connect(ui->AjouterActivite, SIGNAL (clicked()), this, SLOT (ajouterTache()));




}
MainWindow::~MainWindow() {
    pm.freeInstance();
    delete ui;
}


void MainWindow::nouveauProjet(){
    ProjetManager&  pj=ProjetManager::getInstance();
    pj.ajouterProjet(ui->NomProjet_2->text(),ui->DatedispoProj->date(),ui->EcheanceProj->date());
    ui->TreeProjet->addTopLevelItem(ui->NomProjet_2->text());
    ui->ProjetList->addItem(ui->NomProjet_2->text());

}
void MainWindow::ajouterTache(){
    Projet& p = ProjetManager::getProjet(ui->ProjetList->currentText());

    if (ui->unitaire->isChecked())
  { //Ajout Tache Unitaire au projet
       if (ui->Preempt->isChecked())
        p.ajoutTacheUni(ui->titreTache->text(),ui->dispoTache->date(),ui->echeanceTache->date(),Duree(ui->DureeTache->value()),ui->DecrTacheUni->text(),TRUE);
        else
           p.ajoutTacheUni(ui->titreTache->text(),ui->dispoTache->date(),ui->echeanceTache->date(),Duree(ui->DureeTache->value()),ui->DecrTacheUni->text(),FALSE);
      //Precedence Tache Unitaire
       Tache* t= p.trouverTache(ui->titreTache->text());
      Tache* t2=p.trouverTache(ui->TachePrec->currentItem()->text());
       t->addPrecedente(t2);
       //Ajout a une tache composite
       if (ui->composite->isChecked())
       {  Tache* t1=Projet::trouverTache(ui->titreTache->text());

           Tache* t=Projet::trouverTache(ui->TacheCompoList->currentText());
           if(dynamic_cast<TacheComposite*>(t))
           {t->addSousTache(t1);
           }
           else throw CalendarException("Ajout a une tache Composite Impossible");

}}
    else
    {
        if (ui->composite->isChecked()) {
            //Ajout Tache Composite au projet
     p.ajoutTacheComp(ui->titreTache->text(),ui->dispoTache->date(),ui->echeanceTache->date());
     //Precedence Tache Composite

     Tache* t= p.trouverTache(ui->titreTache->text());
     Tache* t2=p.trouverTache(ui->TachePrec->currentItem()->text());
      t->addPrecedente(t2);
      //Ajout a une tache composite
      if (ui->composite->isChecked())
      {  Tache* t1=Projet::trouverTache(ui->titreTache->text());

          Tache* t=Projet::trouverTache(ui->TacheCompoList->currentText());
          if(dynamic_cast<TacheComposite*>(t))
          {t->addSousTache(t1);
          }
          else throw CalendarException("Ajout a une tache Composite Impossible");

}

        }
else throw CalendarException("AJout Impossible");
    }


}
void MainWindow::ProgrammerActivite(){
    Activite* act=new Activite(ui->DureeActivite->value(),ui->DescripActivite->text(),ui->LieuActivite->text());
 ProgrammationManager::ajouterProgrammation(act,ui->dateActi->date(),Horaire(ui->HeureActi->text())) ;


}
void MainWindow::ProgrammerTache(){
    Tache* t=Projet::trouverTache(ui->ProgrammeTache->text());
    if (dynamic_cast<TacheUnitaire*>(t)){ Evenement* ev=new Evenement(t->getDuree(),t->getDescription());
    ProgrammationManager::ajouterProgrammation(ev,ui->datetache->date(),Horaire(ui->heuretache->text()));
}
}

