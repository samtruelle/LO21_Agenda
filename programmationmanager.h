#ifndef PROGRAMMATIONMANAGER
#define PROGRAMMATIONMANAGER
#include "Calendar.h"
#include <QFile>
#include <QTextStream>
#include <QXmlStreamWriter>
#include <list>
#include <vector>

using namespace std;

/*! \class ProgrammationManager
 * \brief classe singleton gérant les programmations
 *
 *  La classe ProgrammationManager centralise toutes les opérations applicables au programmation
 */
class ProgrammationManager{


    list<Programmation*> programmations;/*!< Liste des programmations*/

    static ProgrammationManager* instance;/*!< instance unique de programmationManager*/

    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe ProgrammationManager
     */
    ProgrammationManager();

    /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe ProgrammationManager
     */
    virtual ~ProgrammationManager();


    /*!
     *  \brief Constructeur par recopie
     *
     *  Constructeur par recopie de la classe ProgrammationManager
     */
    ProgrammationManager(const ProgrammationManager& um);


    /*!
     *  \brief Operateur=
     *
     *  Operateur d'affectation de la classe ProgrammationManager
     */
    ProgrammationManager& operator=(const ProgrammationManager& um);

public:

    /*!
     *  \brief Récupération de l'instance singleton de la classe ProgrammationManager
     *
     *  Methode qui permet de créer l'unique instance autorisée de la classe ProgrammationManager,
     *  ou de la créer si celle-ci n'existe pas.
     *
     *  \return une référence sur l'instance de la classe ProgrammationManager
     */
     static ProgrammationManager& getInstance() {
        if(!instance) instance = new ProgrammationManager();
            return *instance;
     }

     /*!
      *  \brief Suppression de l'instance singleton de la classe EvenementProgrammationManager
      *
      *  Methode qui permet de supprimer l'unique instance autorisée de la classe ProgrammationManager.
      */
      void freeInstance(){
         if(instance) delete instance;
      }

      /*!
       *  \brief cherche la programmation correspondant à l'evenement
       *
       *  Methode qui permet retourne la programmation de l'evenement e si elle existe.
       *  \param Evenement& Evenement dont il faut trouver la programmation
       */
      Programmation* trouverProgrammation(Evenement* e);


      /*!
       *  \brief ajoutee la programmation d'un l'evenement
       *
       *  \param Evenement& Evenement dont il faut trouver la programmation
       */
      void ajouterProgrammation(Evenement* e, const QDate& d, const QTime& h);


      /*!
       *  \brief supprime la programmation d'un l'evenement
       *
       *  \param Evenement& Evenement dont il faut trouver la programmation
       */
      void supprimerProgrammation(Evenement* e);

      /*!
       *  \brief retourne toutes les programmations
       *
       *  Methode qui permet retourner la liste des programmations.
       *  \return list<Programmation*> la liste des programmations
       */
      list<Programmation*> getProgrammations(){return programmations;}

      /*!
       *  \brief cherche si une programmation existe pour un evenement donné
       *
       *  \param Evenement& Evenement dont il faut trouver la programmation
       *  \return bool true si programmation de l'evenement existe, false sinon
       */
      bool ExistProgrammation(Evenement* e);

      /*!
       *  \brief Permet de sauvegarder les activites créée sur un fichier xml
       *
       *  \param  const QString& nom du fichier xml
       */

      void saveActivite(const QString& fichierbis);

};


#endif // PROGRAMMATIONMANAGER

