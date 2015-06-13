#include "projetmanager.h"

static ProjetManager* instanceUnique = 0;


Projet& ProjetManager::getProjet(const QString& id) {
    for(list<Projet*>::iterator it = projets.begin(); it != projets.end(); it++) {
        if((*it)->getId == id) {
            return *(*it);
        }
    }

    return 0;
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
        throw CalendarException("Un projet avec cet id existe déjà");

    Projet* nProjet = new Projet(id, dispo, echeance);
    projets.push_back(nProjet);
}

void ProjetManager::suppProjet(const QString& id) {

    if(!getProjet(id))
        throw CalendarException("Aucun projet correspondant.");
    else
        projets.remove(getProjet(id));
}

Projet& ProjetManager::getProjetByTache(const Tache& t){

}
