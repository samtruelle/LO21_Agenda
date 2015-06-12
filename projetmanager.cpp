#include "projetmanager.h"


Projet* ProjetManager::findProjet(const string &id) const {
    for(unsigned int i = ; i < projets.size();i++)
    {
        if(projets[i]->id == id)
            return projets[i];
    }
}


