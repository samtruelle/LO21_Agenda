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

void ProgrammationManager::saveActivite(const QString& fichierbis) {
    QFile newfile(fichierbis);
    if (!newfile.open(QIODevice::WriteOnly | QIODevice::Text))
        throw CalendarException("Erreur : Save Failed,impossible d'ouvrir le fichier xml .");
    QXmlStreamWriter stream(&newfile);
    stream.setAutoFormatting(true);
    stream.writeStartDocument();
//DEBUT SAVE ACTIVITE
    stream.writeStartElement("Activité");
    ProgrammationManager& Progra = ProgrammationManager::getInstance();
    std::list<Programmation*> itprog = Progra.getProgrammations();
    // LOOP Programmation.
    for(std::list<Programmation*>::iterator it = itprog.begin(); it != itprog.end(); ++it) {
        stream.writeStartElement("Activite");
               const Evenement* ev=(*it)->getEvenement();
               Evenement* e = const_cast<Evenement*> (ev);
    /*
        if(dynamic_cast<Activite*>(e)) {
            stream.writeAttribute("type", "Activite");
            stream.writeTextElement("Description", ((Activite*)e->getDescription()));
            stream.writeTextElement("Duree", QString::number(((Activite*)e)->getDuree()));
            stream.writeTextElement("Lieu", ((Activite*)e->getLieu()));

            stream.writeTextElement("Date", (p->getDate()).toString("yyyy-MM-dd"));
            stream.writeTextElement("Horaire", QString::number(p->getHoraire()));

        } else {
            throw CalendarException("Erreur: SAVE Activite failed  ");
        }

*/
        stream.writeEndElement();
    }
    stream.writeEndElement();
    // FIN LOOP PROGRAMMATION

    stream.writeEndDocument();
    newfile.close();
}
