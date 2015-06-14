#include "programmationmanager.h"

ProgrammationManager* ProgrammationManager::instance = 0;

bool ProgrammationManager::ExistProgrammation(Evenement* e) {
    for(list<Programmation*>::iterator it = programmations.begin(); it != programmations.end(); it++) {
        if((*it)->getEvenement()->getDescription() == e->getDescription()) {
            return true;
        }
    }
    return false;
}

Programmation* ProgrammationManager::trouverProgrammation(Evenement* e){
    for(list<Programmation*>::iterator it = programmations.begin(); it != programmations.end(); it++) {
        if((*it)->getEvenement()->getDescription() == e->getDescription()) {
            return (*it);
        }
    }
    throw CalendarException("La programmation de l'évenement n'existe pas");
}


void ProgrammationManager::ajouterProgrammation(Evenement* e, const QDate& d, const QTime& h){
    if (ExistProgrammation(e))
        throw CalendarException("erreur, ProgrammationManager, Programmation deja existante");
    Programmation* tmp=new Programmation(e,d,h);
    programmations.push_back(tmp);
}

void ProgrammationManager::supprimerProgrammation(Evenement *e){
    for(list<Programmation*>::iterator it = programmations.begin(); it != programmations.end(); it++) {
        if((*it)->getEvenement()->getDescription() == e->getDescription()) {
            programmations.erase(it);
            break;
        }
    }
    throw CalendarException("suppProgrammation : pas de programmation correspondant");
}
