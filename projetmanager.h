#ifndef PROJETMANAGER
#define PROJETMANAGER
#include "frabiquesingleton.h"
#include <list>
#include "Calendar.h"

/*! \class ProjetManager
 * \brief singleton de projet
 *
 *  La classe gère la création, la suppression et le stockage des différents projets.
 */
class ProjetManager /*: public FabriqueSingleton<ProjetManager,Projet>*/{

    static ProjetManager* instanceUnique;/*!< Instance unique du singleton*/

    list<Projet*> projets; /*!< Liste des projets*/


    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe TacheManager
     */
    ProjetManager();

    /*!
     *  \brief Constructeur par recopie
     *
     *  Constructeur par recopie de la classe ProjetManager
     */
    ProjetManager(const Projet& t);

    /*!
     *  \brief Operator =
     *
     *  Operateur d'affectation de la classe ProjetManager
     */
    ProjetManager& operator=(const Projet& t);

   public:


    /*!
     *  \brief Récupération de l'instance singleton de la classe ProjetManager
     *
     *  Methode qui permet de créer l'unique instance autorisée de la classe ProjetManager,
     *  ou de la créer si celle-ci n'existe pas.
     *
     *  \return une référence sur l'instance de la classe ProjetManager
     */
    static ProjetManager& getInstance() {
        if(!instanceUnique) instanceUnique = new ProjetManager();
            return *instanceUnique;
        }


    /*!
     *  \brief Suppression de l'instance de la classe ProjetManager
     *
     *  Methode qui permet de liberer l'instance de la classe ProjetManager.
     */
       void freeInstance() {
        if(instanceUnique) delete instanceUnique;
        instanceUnique = 0;
    }

   /*!
    *  \brief Récupération d'un projet correspondant à une tach
    *
    *  Methode qui permet de trouver le projet auquel  une tache est associée
    *
    *  \param Tache& la tache
    *  \return une référence sur un projet
    */
    Projet& getProjetByTache(Tache& t);


    /*!
     *  \brief ajout d'un projet
     *
     *  \param Tache& la tache
     */
    void ajouterProjet(const QString& id,const QDate& dispo, const QDate& echeance);

    /*!
     *  \brief suppression d'un projet
     *
     *  \param QString id du projet
     */
    void suppProjet(const QString& id);


    /*!
     *  \brief selection d'un projet
     *
     *  \param QString id du projet
     *  \return Projet& une référence sur le projet correspondant à l'Id
     */
    Projet& getProjet(const QString& id);


    /*!
     *  \brief vérification de l'existence d'un projet
     *
     *  \param QString id du projet
     *  \return bool vrai si il existe, faux sinon
     */
    bool existsProjet(const QString& id);


    /*!
     *  \brief Permet de sauver un projet dans un fichier XML
     *
     *  \param QString fichier nom du fichier
     */
    void save(const QString& fichier);

    /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe ProjetManager
     */
    virtual ~ProjetManager();

};

#endif // PROJETMANAGER

