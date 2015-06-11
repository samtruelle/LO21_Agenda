#ifndef FRABIQUESINGLETON
#define FRABIQUESINGLETON


//creation de la fabrique de singleton$
template <class T>
class FabriqueSingleton{

    //Handler commun aux singleton avec instance unique, constructeur et desctructeur
    struct Handler{
        T* instance;
        Handler():instance(0){}
        ~Handler(){if(instance) delete instance;}
    };
    static Handler handler;


protected:
    FabriqueSingleton();
    ~FabriqueSingleton();

public:
    //retourne instance unique, si l'instance n'existe pas la créer
    static T* getInstance(){
        if(!handler.instance) handler.instance = new T;
        return (static_cast<T*>(handler.instance));
    }

    //libere l'instance unique si elle existe, affecte zero après la libération
    static void freeInstance(){
        if(handler.instance){
            delete handler.instance;
            handler.instance = 0;
        }

    }
};


#endif // FRABIQUESINGLETON

