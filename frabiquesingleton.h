#ifndef FRABIQUESINGLETON
#define FRABIQUESINGLETON
#include "Calendar.h"
#include <list>
using namespace std;

using namespace std;

//creation de la fabrique de singleton$
template <typename...> class FabriqueSingleton;


template <typename T,typename U>
class FabriqueSingleton<T,U>{


    list<U*> items;
    static T* instance;

    //static T* CreateInstance();

    FabriqueSingleton();

    ~FabriqueSingleton();

public:

    static T& getInstance();

    static void freeInstance();

    void ajouterItem(U* i);

    bool existsItem(const QString& id);

    U& getItem(const QString& id);

    void suppItem(const QString& id);

    list<U*> getItems(){return items;}
};

template<typename T,typename U>
T* FabriqueSingleton<T,U>::instance = 0;


template <typename T,typename U>
FabriqueSingleton<T,U>::FabriqueSingleton()
{
    assert(FabriqueSingleton::instance == 0);
    FabriqueSingleton::instance = static_cast<T*>(this);
}


template<typename T,typename U>
 T& FabriqueSingleton<T,U>::getInstance()
{
    if (FabriqueSingleton::instance == 0)
    {
        FabriqueSingleton::instance = new T();
    }
    return *(FabriqueSingleton::instance);
}


template<typename T,typename U>
FabriqueSingleton<T,U>::~FabriqueSingleton()
{
    if(FabriqueSingleton::instance != 0)
    {
        delete FabriqueSingleton::instance;
    }
    FabriqueSingleton::instance = 0;
}

/*template<typename T,typename U>
inline T* FabriqueSingleton<T,U>::CreateInstance()
{
    return new T();
}
*/
template<typename T,typename U>
bool FabriqueSingleton<T,U>::existsItem(const QString& id){
    for(list<U*>::iterator it = items.begin(); it!=items.end();++it){
        if((*it)->getId() == id)
            return true;
    }
    return false;
}


template<typename T,typename U>
void FabriqueSingleton<T,U>::ajouterItem(U* i){
    if(existsItem(i->getId()))
        throw CalendarException("Un item avec cet id existe déjà");
    l.push_back(i);
}


template<typename T,typename U>
U& FabriqueSingleton<T,U>::getItem(const QString& id){
    for(list<U*>::iterator it=items.begin();it != items.end;++it){
        if((*it)->getId == id)
            return *(*it);
    }
    return 0;
}


template<typename T,typename U>
void FabriqueSingleton<T,U>::suppItem(const QString& id){
    U* tmp = this->getItem(id);
    if(!tmp)
        throw CalendarException("Aucune item correspondant");
    else
        items.remove(this->getItem(id));
}


/*class ProjetManager : public FabriqueSingleton<ProjetManager,Projet>{


public :

    ProjetManager();
    ~ProjetManager();
};
*/
#endif // FRABIQUESINGLETON

