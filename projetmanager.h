#ifndef PROJETMANAGER
#define PROJETMANAGER
#include "frabiquesingleton.h"
#include <list>

using namespace std;


/*! \class ProjetManager
 * \brief singleton de projet
 *
 *  La classe gère la création, la suppression et le stockage des différents projets.
 */
class ProjetManager /*: public FrabriqueSingleton*/{
private :
    static ProjetManager* instanceUnique;/*!< Instance unique du singleton*/
    list<Projet*> projets; /*!< Liste des projets*/
    //unsigned int nb;
    //unsigned int nbMax;

    /*!
          *  \brief Constructeur
          *
          *  Constructeur de la classe ProjetManager
          */
    ProjetManager();

    /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe ProjetManager
     */
    ~ProjetManager(){for(unsigned int i=0; i<nb; i++) delete projets[i]; delete[] projets;}

    /*!
          *  \brief Constructeur par recopie
          *
          *  Constructeur par recopie de la classe ProjetManager
          */
    ProjetManager(const Projet& um);

    /*!
          *  \brief Operator =
          *
          *  Operateur d'affectation de la classe ProjetManager
          */

    ProjetManager& operator=(const Projet& um);

    /*struct Handler{
        ProjetManager* instance;
        Handler():instance(0){}
        // destructeur appelé à la fin du programme
        ~Handler(){ if (instance) delete instance; }
    };
    static Handler handler;
    string genererId();
*/

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
    void libereInstance() {
        if(instanceUnique) delete instanceUnique;
    }

    Projet& getProjetByTache(const Tache& t);

    Projet& ajouterProjet(const QString& id,const QDate& dispo, const QDate& echeance);
    void suppProjet(const QString& id);

    Projet& getProjet(const QString& id);

    bool existsProjet(const QString& id);


/*
    class Iterator {
        friend class ProjetManager;
        Projet** currentProjet;
        unsigned int nbRemain;
        Iterator(Projet** p, unsigned nb):currentProjet(p),nbRemain(nb){}
    public:
        Iterator():nbRemain(0),currentProjet(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            nbRemain--;
            currentProjet++;
        }
        Projet& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentProjet;
        }

    };
    Iterator getIterator() {
        return Iterator(projets,nb);
    }

    class ConstIterator {
        friend class ProjetManager;
        Projet** currentProjet;
        unsigned int nbRemain;
        ConstIterator(Projet** p, unsigned nb):currentProjet(p),nbRemain(nb){}
    public:
        ConstIterator():nbRemain(0),currentProjet(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            nbRemain--;
            currentProjet++;
        }
        const Projet& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentProjet;
        }
    };
    ConstIterator getIterator() const{
        return ConstIterator(projets,nb);
    }
    */
};

#endif // PROJETMANAGER

