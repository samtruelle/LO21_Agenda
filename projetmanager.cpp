#include "projetmanager.h"

ProjetManager* ProjetManager::instanceUnique = 0;

Projet& ProjetManager::getProjet(const QString& id) {
    for(list<Projet*>::iterator it = projets.begin(); it != projets.end(); it++) {
        if((*it)->getId() == id) {
            return *(*it);
        }
    }
    throw CalendarException("ProjetManager::getProjet Pas de projet correspondant");

}


bool ProjetManager::existsProjet(const QString& id){
    for(list<Projet*>::iterator it = projets.begin(); it != projets.end(); it++) {
        if((*it)->getId() == id) {
            return true;
        }
    }

    return false;
}

void ProjetManager::ajouterProjet(const QString& id, const QDate& dispo, const QDate& echeance) {

    if(existsProjet(id))
        throw CalendarException("ProjetManager::ajouterProjet Un projet avec cet id existe déjà");
    if(dispo>echeance)
        throw CalendarException("Date de disponibilité du projet ultérieur à la date d'échéance");

    Projet* nProjet = new Projet(id, dispo, echeance);
    projets.push_back(nProjet);
}

void ProjetManager::suppProjet(const QString& id) {

    for(list<Projet*>::iterator it = projets.begin(); it != projets.end(); it++) {
        if((*it)->getId() == id) {
            projets.erase(it);
            break;
        }
    }
    throw CalendarException("suppProjet : pas de projet correspondant");
}

Projet& ProjetManager::getProjetByTache(Tache& t){
    for(list<Projet*>::iterator it = projets.begin();it != projets.end();++it){
        if((*it)->ExistTache(&t))
            return *(*it);
    }
    throw CalendarException("ProjetManager::getProjetByTache : pas de projet correspondant");
}

ProjetManager::~ProjetManager(){}
