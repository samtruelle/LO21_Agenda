/*#ifndef TACHEMANAGER
#define TACHEMANAGER
#include "frabiquesingleton.h"


class TacheManager : public FabriqueSingleton<TacheManager>{

    list<Tache*> taches;

    QString file;
    TacheManager();
    ~TacheManager();
    TacheManager(const TacheManager& tm);
    TacheManager& operator=(const TacheManager& tm);

public:
    //template<Tache>
    //void ajouterItem(Tache* i);

    Tache& getTache(const QString& id);

    bool isTacheExistante(const QString& id) const { return trouverTache(id)!=0; }
    const Tache& getTache(const QString& code) const;
    void load(const QString& f);
    void save(const QString& f);
    Tache* trouverTache(const QString& t) const;
    static TacheManager& getInstance();

    static void freeInstance();










    class Iterator {
        friend class TacheManager;
        Tache** currentTache;
        unsigned int nbRemain;
        Iterator(Tache** u, unsigned nb):currentTache(u),nbRemain(nb){}
    public:
        Iterator():nbRemain(0),currentTache(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            nbRemain--;
            currentTache++;
        }
        Tache& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentTache;
        }
    };
    Iterator getIterator() {
        return Iterator(taches,nb);
    }

    class ConstIterator {
        friend class TacheManager;
        Tache** currentTache;
        unsigned int nbRemain;
        ConstIterator(Tache** u, unsigned nb):currentTache(u),nbRemain(nb){}
    public:
        ConstIterator():nbRemain(0),currentTache(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            nbRemain--;
            currentTache++;
        }
        const Tache& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentTache;
        }
    };
    ConstIterator getIterator() const{
        return ConstIterator(taches,nb);
    }

    class iterator {
        Tache** current;
        iterator(Tache** u):current(u){}
        friend class TacheManager;
    public:
        iterator():current(0){}
        Tache& operator*() const { return **current; }
        bool operator!=(iterator it) const { return current!=it.current; }
        iterator& operator++(){ ++current; return *this; }
    };
    iterator begin() { return iterator(taches); }
    iterator end() { return iterator(taches+nb); }

    class const_iterator {
        Tache** current;
        const_iterator(Tache** u):current(u){}
        friend class TacheManager;
    public:
        const_iterator():current(0){}
        Tache& operator*() const { return **current; }
        bool operator!=(const_iterator it) const { return current!=it.current; }
        const_iterator& operator++(){ ++current; return *this; }
    };
    const_iterator begin() const { return const_iterator(taches); }
    const_iterator end() const { return const_iterator(taches+nb); }

    class DisponibiliteFilterIterator {
        friend class TacheManager;
        Tache** currentTache;
        unsigned int nbRemain;
        QDate dispo;
        DisponibiliteFilterIterator(Tache** u, unsigned nb, const QDate& d):currentTache(u),nbRemain(nb),dispo(d){
            while(nbRemain>0 && dispo<(*currentTache)->getDateDispo()){
                nbRemain--; currentTache++;
            }
        }
    public:
        DisponibiliteFilterIterator():nbRemain(0),currentTache(0){}
        bool isDone() const { return nbRemain==0; }
        void next() {
            if (isDone())
                throw CalendarException("error, next on an iterator which is done");
            do {
                nbRemain--; currentTache++;
            }while(nbRemain>0 && dispo<(*currentTache)->getDateDispo());
        }
        Tache& current() const {
            if (isDone())
                throw CalendarException("error, indirection on an iterator which is done");
            return **currentTache;
        }
    };
    DisponibiliteFilterIterator getDisponibiliteFilterIterator(const QDate& d) {
        return DisponibiliteFilterIterator(taches,nb,d);
    }
};

#endif // TACHEMANAGER

*/
