#ifndef PROJETMANAGER
#define PROJETMANAGER
#include "frabiquesingleton.h"

class ProjetManager /*: public FrabriqueSingleton*/{
private :
    static ProjetManager* instanceUnique;
    Projet** projets;
    unsigned int nb;
    unsigned int nbMax;

    void addItem(Projet* p);
    Projet* findProjet(const string& id) const;
    ProjetManager():projets(0),nb(0),nbMax(0){}
    ~ProjetManager(){for(unsigned int i=0; i<nb; i++) delete projets[i]; delete[] projets;}
    ProjetManager(const Projet& um);
    ProjetManager& operator=(const Projet& um);
    struct Handler{
        ProjetManager* instance;
        Handler():instance(0){}
        // destructeur appelé à la fin du programme
        ~Handler(){ if (instance) delete instance; }
    };
    static Handler handler;
    string genererId();

   public:

    static ProjetManager& donneInstance();
    static void freeInstance();

    Projet& getProjetByTache(const Tache&);

    Projet& ajouterProjet(const QString& nom,const QDate& dispo);
    void deleteProjet(const QString& nom);

    Projet& getProjet(const QString& id);
    const Projet& getProjet(const QString& id) const;


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
};

#endif // PROJETMANAGER

