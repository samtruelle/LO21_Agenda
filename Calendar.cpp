#include "Calendar.h"
#include <QFile>
#include <QTextCodec>
//#include <QtXml>
#include <QMessageBox>
#include "tachemanager.h"
#include <iostream>
#include <sstream>


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

void Projet::ajoutTacheUni (const QString& t, const QDate& d, const QDate& e, const unsigned int& dur, const bool p){
    if (trouverTache(t)) throw CalendarException("erreur Projet tache deja existante");
    TacheUnitaire* tacheuni=new TacheUnitaire(t,d,e,dur,p);
    ajouterTache(tacheuni);
}

void Projet::ajoutTacheComp(const QString& t, const QDate& d, const QDate& e)
{
    if (trouverTache(t))
        throw CalendarException("erreur Projet tache deja existante");
    TacheComposite* comptache= new TacheComposite(t,d,e);
    ajouterTache(comptache);
}



//******************************************************************************************

/*
ProgrammationManager::ProgrammationManager():programmations(0),nb(0),nbMax(0){}
void ProgrammationManager::addItem(Programmation* t){
	if (nb==nbMax){
		Programmation** newtab=new Programmation*[nbMax+10];
		for(unsigned int i=0; i<nb; i++) newtab[i]=programmations[i];
		// ou memcpy(newtab,Programmations,nb*sizeof(Programmation*));
		nbMax+=10;
		Programmation** old=programmations;
		programmations=newtab;
		delete[] old;
	}
	programmations[nb++]=t;
}

Programmation* ProgrammationManager::trouverProgrammation(const Tache& t)const{
	for(unsigned int i=0; i<nb; i++)
		if (&t==&programmations[i]->getTache()) return programmations[i];
	return 0;
}

void ProgrammationManager::ajouterProgrammation(const Tache& t, const QDate& d, const QTime& h){
	if (trouverProgrammation(t)) throw CalendarException("erreur, ProgrammationManager, Programmation deja existante");	
	Programmation* newt=new Programmation(t,d,h);
	addItem(newt);
}


ProgrammationManager::~ProgrammationManager(){
	for(unsigned int i=0; i<nb; i++) delete programmations[i];
	delete[] programmations;
}

ProgrammationManager::ProgrammationManager(const ProgrammationManager& um):nb(um.nb),nbMax(um.nbMax), programmations(new Programmation*[um.nb]){
	for(unsigned int i=0; i<nb; i++) programmations[i]=new Programmation(*um.programmations[i]);
}

ProgrammationManager& ProgrammationManager::operator=(const ProgrammationManager& um){
	if (this==&um) return *this;
	this->~ProgrammationManager();
	for(unsigned int i=0; i<um.nb; i++) addItem(new Programmation(*um.programmations[i]));
	return *this;
}

*/

ProgrammationManager::saveActivite(const QString& fichierbis){
    QFile newfile(fichierbis);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException("Erreur : Save Failed,impossible d'ouvrir le fichier xml .");
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
//DEBUT SAVE ACTIVITE
    stream.writeStartElement("Activité");
    EvenementManager& monEvenementManager = EvenementManager::getInstance();
    std::list<Programmation*> iter = monEvenementManager.getEvenements();
    // LOOP évènements.
    for(std::list<>::iterator it = iter.begin(); it != iter.end(); it++) {
        stream.writeStartElement("evenement");

        if(dynamic_cast<Rdv*>(*it)) {
            stream.writeAttribute("type", "rdv");
            stream.writeTextElement("participant", ((Rdv*)(*it))->getParticipant());
        } else if(dynamic_cast<Reunion*>(*it)) {
            stream.writeAttribute("type", "reunion");
            stream.writeTextElement("participants", ((Reunion*)(*it))->getParticipants());
        } else if(dynamic_cast<ActiviteTrad*>(*it)) {
            stream.writeAttribute("type", "acttrad");
            stream.writeTextElement("lieu", ((ActiviteTrad*)(*it))->getLieu());
        } else if(dynamic_cast<Partie*>(*it)) {
            stream.writeAttribute("type", "partie");
            stream.writeTextElement("id", QString::number(((Partie*)(*it))->getId()));
        } else {
            throw CalendarException("Erreur : type d'évènement inconnu, export impossible.");
        }

        stream.writeTextElement("date", ((*it)->getDate()).toString("yyyy-MM-dd"));
        stream.writeTextElement("sujet", (*it)->getDescription());
        stream.writeTextElement("heureDebut", QString::number((*it)->getHeureDebut()));
        stream.writeTextElement("dureeHeure", QString::number((*it)->getDuree()));

        stream.writeEndElement();
    }
    stream.writeEndElement();
    // FIN ECRITURE EVENEMENTS

    stream.writeEndDocument();
    newfile.close();
}
