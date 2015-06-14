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

/*
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
}*/
