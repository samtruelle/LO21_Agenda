#ifndef TACHEMANAGER
#define TACHEMANAGER
#include "frabiquesingleton.h"

/*! \class TacheManager
 * \brief singleton de tache
 *
 *  La classe gère la création, la suppression et le stockage des différentes taches.
 */
class TacheManager /*: public FabriqueSingleton<TacheManager>*/{

    list<Tache*> taches; /*!< Liste des taches*/

    static TacheManager* instance; /*!< Instance unique du singleton*/

    //QString file;

    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe TacheManager
     */
    TacheManager();


    /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe TacheManager
     */
    ~TacheManager(){taches.clear(); freeInstance(); }

    /*!
     *  \brief Constructeur par recopie
     *
     *  Constructeur par recopie de la classe TacheManager
     */
    TacheManager(const TacheManager& tm);

    /*!
     *  \brief Operator =
     *
     *  Operateur d'affectation de la classe TacheManager
     */
    TacheManager& operator=(const TacheManager& tm);

public:

    /*!
     *  \brief Récupération de l'instance singleton de la classe TacheManager
     *
     *  Methode qui permet de créer l'unique instance autorisée de la classe TacheManager,
     *  ou de la créer si celle-ci n'existe pas.
     *
     *  \return une référence sur l'instance de la classe TacheManager
     */
    static TacheManager& getInstance(){
        if(!instance) instance = new TacheManager();
            return *instance;
    }

    /*!
     *  \brief Suppression de l'instance de la classe TacheManager
     *
     *  Methode qui permet de liberer l'instance de la classe TacheManager.
     */
    static void freeInstance(){
        if(instance) delete instance;
    }


    /*!
     *  \brief template methode ajout d'une tache
     *
     *  \param Tache* pointeur sur la t
     *  \return Tache& reférence sur Tache à trouver
     */
   void ajouterTache(Tache* i);

   /*!
    *  \brief Cherche une tache à l'id correspondant dans taches
    *
    *  \param QString& id de la tache à chercher
    *  \return Tache& reférence sur Tache à trouver
    */
    Tache& getTache(const QString& id);

    /*!
     *  \brief Cherche une tache à l'id correspondant dans taches
     *
     *  \param QString& id de la tache à chercher
     *  \return bool vrai si trouvée, faux sinon
     */
    bool trouverTache(const QString& t);

    /*!
     *  \brief supprimer la tache à l'id correspondant dans taches
     *
     *  \param QString& id de la tache à chercher
     */
    void suppTache(const QString& t);


    //const Tache& getTache(const QString& id) const;


    /*void load(const QString& f);
    void save(const QString& f);
    */





};

#endif // TACHEMANAGER


