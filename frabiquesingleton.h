#ifndef FRABIQUESINGLETON
#define FRABIQUESINGLETON
#include "Calendar.h"
#include <list>

using namespace std;


template <typename...> class FabriqueSingleton;


/*! \class FabriqueSingleton
 * \brief Template class de singleton
 *
 *
 * La classe génère automatique des classes singletons
 */
template <typename T,typename U>
class FabriqueSingleton<T,U>{


    list<U*> items;/*!< Liste d'items U**/
    static T* instance;/*!< instance unique de singleton*/

    //static T* CreateInstance();

    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe FabriqueSingleton
     */
    FabriqueSingleton();

    /*!
     *  \brief Destructeur
     *
     *  Destructeur de la classe FabriqueSingleton
     */
    ~FabriqueSingleton();

public:

    /*!
     *  \brief Accesseur à l'instance unique du singleton
     *
     *  Initialise l'instance si ce n'est pas déjà le cas
     *
     *  \return T&
     */
    static T& getInstance();


    /*!
     *  \brief suppresseur de l'instance unique du singleton
     *
     *  Delete l'instance et la met 0 si elle initialisée
     *
     */
    static void freeInstance();


    /*!
     *  \brief Ajoute un élément à la liste de U*
     *
     *  Throw exception si un iteam à l'id égal à i existe
     *
     *  \param U*
     */
    void ajouterItem(U* i);


    /*!
     *  \brief teste l'existence d'un item U
     *
     *  \param QString Id de l'item a trouver
     *  \return bool true si l'item est trouvé, false sinon
     */
    bool existsItem(const QString& id);


    /*!
     *  \brief renvoie une référence sur un item U
     *
     *  \param QString Id de l'item a retourner
     *
     *  \return U& une référence sur l'item si il est trouvé
     */
    U& getItem(const QString& id);


    /*!
     *  \brief supprimer l'item identifié par id si il est trouvé
     *
     *  \param QString Id de l'item a supprimer
     *
     */
    void suppItem(const QString& id);


    /*!
     *  \brief retourne la liste de pointeur sur des items U
     *
     *  \return list<U*> la liste des pointeurs sur item
     *
     */
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
        if((*it)->getId() == id)
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

