#include "Calendar.h"
#include <QFile>
#include <QTextCodec>
//#include <QtXml>
#include <QMessageBox>
#include "tachemanager.h"
#include <iostream>
#include <sstream>
#include "programmationmanager.h"
#include "projetmanager.h"


QTextStream& operator<<(QTextStream& f, const Duree & d){ d.afficher(f); return f; }

QTextStream& operator>>(QTextStream& flot, Duree& duree){
    unsigned int h,m;
    bool ok=true;
    flot>>h;
    if (flot.status()!=QTextStream::Ok) ok=false;

    if(flot.read(1)=="H") {
        flot>>m;
        if (flot.status()!=QTextStream::Ok) ok=false;
    }
    else {
        ok=false;
    }
    if (ok) duree=Duree(h,m);
    return flot;
}

 void Duree::afficher(QTextStream& f) const {
     f.setPadChar('0');
     f.setFieldWidth(2);
     f<<nb_minutes/60;
     f.setFieldWidth(0);
     f<<"H";
     f.setFieldWidth(2);
     f<<nb_minutes%60;
     f.setFieldWidth(0);
     f.setPadChar(' ');
 }

QTextStream& operator<<(QTextStream& fout, const Tache& t){
    fout<<t.getId()<<"\n";
    fout<<t.getDateDispo().toString()<<"\n";
    fout<<t.getDateEcheance().toString()<<"\n";
	return fout;
}

QTextStream& operator<<(QDataStream& f, const Programmation& p);

/*void Tache::setTitre(const QString& str){
  if (TacheManager::getInstance().isTacheExistante((str))) throw CalendarException("erreur TacheManager : tache déjà existante");
  titre=str;
}*/

Tache* Tache::findTache(Tache* t, list<Tache *> l){
    list<Tache*>::iterator it = find(l.begin(), l.end(), t);
    if (it == l.end())
        return 0;
    return (*it);
}

Tache* Tache::findTache(const QString& t, list<Tache *> l){
    for(list<Tache*>::iterator it = l.begin(); it != l.end(); it++) {
        if((*it)->getId() == t) {
            return *it;
        }
    }
    return 0;
}

bool Tache::existTache(Tache *t, list<Tache *> l){
    list<Tache*>::iterator it = find(l.begin(), l.end(), t);
    if (it == l.end())
        return false;
    return true;
}

bool Tache::existTache(const QString& t, list<Tache *> l){
    for(list<Tache*>::iterator it = l.begin(); it != l.end(); it++) {
        if((*it)->getId() == t) {
            return true;
        }
    }
    return false;
}


void Tache::addPrecedente(Tache *t){
    if(!existTache(t,precedence))
        precedence.push_back(t);
    else{
        throw CalendarException("précedence déjà existante pour cette tâche");
    }
}


void TacheComposite::addSousTache(Tache* t){
    if(!existTache(t,sous_taches))
        sous_taches.push_back(t);
    else
        throw CalendarException("Sous Tâche déjà existante pour cette tâche");
}

void TacheComposite::suppSousTache(const QString &t){
    if(existTache(t,sous_taches))
        //Tache* tmp = findTache(t,sous_taches);
        sous_taches.remove(findTache(t,sous_taches));
}

Tache* TacheComposite::getTacheComposante(const QString& t){
    if(existTache(t,sous_taches))
        return findTache(t,sous_taches);
    else
        throw CalendarException("Aucune tache composante correspondant à ce titre");
}


Tache* Projet::trouverTache(const QString& t) {
    for(list<Tache*>::iterator it = taches.begin(); it != taches.end(); it++) {
        if((*it)->getId() == t) {
            return *it;
        }
    }
    return 0;
}

bool Projet::ExistTache(Tache* t){
    for(list<Tache*>::iterator it = taches.begin(); it != taches.end(); it++) {
        if((*it)->getId() == t->getId()) {
            return true;
        }
    }
    return false;
}

void Projet::ajouterTache(Tache* t){
    if (!trouverTache(t->getId())) throw CalendarException("erreur, TacheManager, tache deja existante");
    taches.push_back(t);
}

void Projet::suppTache(const QString& t){
    for(list<Tache*>::iterator it = taches.begin(); it != taches.end(); it++) {
        if((*it)->getId() == t) {
            taches.erase(it);
        }
    }
}

template<typename T>
T& Projet::getTache(const QString& t) {
    for(list<Tache*>::iterator it = taches.begin(); it != taches.end(); it++) {
        if((*it)->getId() == t && dynamic_cast<T*>(*it)) {
            return *(dynamic_cast<T*>(*it));
        }
    }

    throw CalendarException("Aucune tâche correspondante");
}

void Projet::ajoutTacheUni(const QString& t, const QDate& d, const QDate& e, const unsigned int& dur,const QString& des, const bool p){
    if (trouverTache(t)) throw CalendarException("erreur Projet tache deja existante");
    if(d>e)
        throw CalendarException("Projet::ajoutTacheUni date de disponibilité supérieure a date d'échéance de la tâche");
    if(echeance<e && d>echeance)
        throw CalendarException("Projet::ajoutTacheUni la tâche n'est pas temporellement compatible avec le projet");
    TacheUnitaire* tacheuni=new TacheUnitaire(t,d,e,dur,des,p);
    ajouterTache(tacheuni);
}

void Projet::ajoutTacheComp(const QString& t, const QDate& d, const QDate& e)
{
    if (trouverTache(t))
        throw CalendarException("erreur Projet tache deja existante");
    if(d>e)
        throw CalendarException("Projet::ajoutTacheUni date de disponibilité supérieure a date d'échéance de la tâche");
    if(echeance<e && d>echeance)
        throw CalendarException("Projet::ajoutTacheUni la tâche n'est pas temporellement compatible avec le projet");
    TacheComposite* comptache= new TacheComposite(t,d,e);
    ajouterTache(comptache);
}

/*
bool Evenement::estProgramme(){
   ProgrammationManager pm = ProgrammationManager::getInstance();
   return pm.ExistProgrammation(this);

   ProjetManager pm2 = ProjetManager::getInstance();
}*/


