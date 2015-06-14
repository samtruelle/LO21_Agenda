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




//ProjetManager::~ProjetManager(){}


void ProjetManager::saveProjet(const QString& fichier) {

    QFile newfile(fichier);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException("Erreur : Save Failed,impossible d'ouvrir le fichier xml .");
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();

    // SAVE PROJETS
    stream.writeStartElement("projets");
    // LOOP  projets.
    for (std::list<Projet*>::iterator it = projets.begin(); it != projets.end(); it++)
    {
        stream.writeStartElement("projet");
        Projet* i=*it;
        stream.writeAttribute("ID", i->getId());
        stream.writeAttribute("Date de disponibilite", ((i->getDisponibilite()).toString("yyyy-MM-dd")));
        stream.writeAttribute("dateEcheance", (i->getEcheance()).toString("yyyy-MM-dd"));
        // LOOP tâches  projet
        std::list<Tache*>& t = (*it)->getTaches();
        for(std::list<Tache*>::iterator itt = t.begin(); itt != t.end(); itt++)
        {
            stream.writeStartElement("tache");
            Tache* i2=*itt;

            if(dynamic_cast<TacheUnitaire*>(i2))
            {
                 if(dynamic_cast<TacheUnitaire*>(i2)->ispreemptable())
                        {stream.writeAttribute("type", "Tache Unitaire Preemptive");}
                 else
                        {stream.writeAttribute("type", "Tache Unitaire Non Preemptive");}

                stream.writeTextElement("Duree", QString::number(((TacheUnitaire*)i2)->getDuree()));


                stream.writeEndElement();

            }
            else
                            if(dynamic_cast<TacheComposite*>(i2))
                            {
                                stream.writeAttribute("type", "TacheComposite");
                                stream.writeStartElement("composantes");
                                std::list<Tache*>& tc = ((TacheComposite*)i2)->getSousTaches();
                                for(std::list<Tache*>::iterator itc = tc.begin(); itc != tc.end(); itc++)
                                {
                                    stream.writeTextElement("nom", ((Tache*)*itc)->getId());
                                }
                                stream.writeEndElement();
                                }
                else
            {
                throw CalendarException("Erreur : Save failed, tâche non reconnue.");
            }

            stream.writeTextElement("ID", i2->getId());
            stream.writeTextElement("dateDispo", (i2->getDateDispo()).toString("yyyy-MM-dd"));
            stream.writeTextElement("dateEcheance", (i2->getDateEcheance()).toString("yyyy-MM-dd"));

            stream.writeEndElement();
      }
        stream.writeEndElement();
    }
    stream.writeEndElement();

    // FIN SAVE PROJETS
    stream.writeEndDocument();
    newfile.close();
}

