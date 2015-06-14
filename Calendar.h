#ifndef CALENDAR_h
#define CALENDAR_h
#include <QString>
#include <QDate>
#include <QTextStream>
#include <list>
using namespace std;

class CalendarException{
public:
    CalendarException(const QString& message):info(message){}
    QString getInfo() const { return info; }
private:
    QString info;
};

class Tache;

/*! \class Projet
 * \brief classe projet
 *
 */
class Projet {
        QString id;/*!< id du projet*/
        QDate disponibilite;/*!< date de dispo du projet*/
        QDate echeance;/*!< date echeance du projet*/
        list<Tache*> taches;/*!< list de tâches du projet*/

        /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe Projet
         *
         *  \param id l''id du projet à créer
         *  \param d la date de disponibilité du projet
         *  \param e la date d'échéance du projet
         */
        Projet(const QString& id,const QDate& d, const QDate& e):
                id(id),disponibilite(d), echeance(e){}
        friend class ProjetManager;
    public:
        /*!
         * \brief Destructeur
         *
         */
        ~Projet(){taches.clear();}

        /*!
         * \brief getId
         * \return QString ID du projet
         */
        QString getId() const { return id; }
        void setId(const QString& id);

        /*!
         * \brief getDisponibilite
         * \return QDate date de disponibilite du projet
         */
        QDate getDisponibilite() const { return disponibilite; }

        /*!
         * \brief getEcheance
         * \return QDate date d'echeance du projet
         */
        QDate getEcheance() const { return echeance; }

        void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e);


        /*!
         *  \brief trouve la Tache lié au projet au titre correspondant
         *
         *  \param t le titre de la tache à trouver
         *  \return Tache* Pointeur sur la tache recherché ou 0
         */
        Tache* trouverTache(const QString& t);

        /*!
         *  \brief Getter de la list de taches d'un projet
         *
         *  \return list<Tache*> La liste de tache du projet
         */
        list<Tache*>& getTaches(){return taches;}

        /*!
         *  \brief ajout d'une tâche
         *
         *  \param t Pointeur vers la tache à ajouter
         */
        void ajouterTache(Tache* t);

        /*!
         *  \brief suppression d'une tâche
         *
         *  \param t le titre de la tâche à supprimer
         */
        void suppTache(const QString &t);

        /*!
         *  \brief Récupération d'une tâche
         *
         *  Template Methode qui permet de récupérer une référence sur une tâche voulue.
         *  La tâche peut être de tout type de tâche.
         *
         *  \param nomTache le nom de la tâche à récupérer
         *  \return une référence sur la tâche demandée
         */
         template<typename T> T& getTache(const QString& t);

        //void save(QXmlStreamWriter& stream) const;
    };



/*! \class Duree
        \brief Classe permettant de manipuler des durees
        L'utilisation de cette classe n?cessite des dates valides au sens commun du terme.
        D?clenchement d'exception dans le cas contraire
*/
class Duree{
public:
    //! Constructeur ? partir de heure et minute
    /*! \param h heure avec h>=0
        \param m minute avec 0<=m<=59
        */
    Duree(unsigned int h, unsigned int m):nb_minutes(h*60+m) {if (m>59) throw CalendarException("erreur: initialisation duree invalide");}
    //! Constructeur ? partir de minute
    /*! \param m minute avec m>=0
        */
    Duree(unsigned int m=0):nb_minutes(m) {}
    void setDuree(unsigned int minutes) { nb_minutes=minutes; }
    void setDuree(unsigned int heures, unsigned int minutes) { if (heures==0 && minutes>59) throw CalendarException("erreur: initialisation duree invalide"); nb_minutes=heures*60+minutes; }
    unsigned int getDureeEnMinutes() const { return nb_minutes; } //<!Retourne la duree en minutes
    double getDureeEnHeures() const { return double(nb_minutes)/60; } //<!Retourne la duree en heures
    unsigned int getMinute() const { return nb_minutes%60; }
    unsigned int getHeure() const { return nb_minutes/60; }
    void afficher(QTextStream& f) const; //<!Affiche la duree sous le format hhHmm
private:
    unsigned int nb_minutes;
};

QTextStream& operator<<(QTextStream& f, const Duree & d);
QTextStream& operator>>(QTextStream&, Duree&); //lecture format hhHmm


/*! \class Tache
 * \brief classe abstraite représentant une tâche générique
 *
 */
class Tache {
   protected:
    QString titre;/*!< Nom de la tâche*/

    QDate dispo;/*!< Date de disponibilité de la tâche*/
    QDate echeance;/*!< Date d'échéance de la tâche*/
    list<Tache*> precedence;/*!< Liste des précédences requises*/
    /*!
      *  \brief Constructeur
      *
      *  Constructeur protected de la classe Tache
      *
      *  \param _t le titre de la tâche à créer
      *  \param _d la date de disponibilité de la tâche à créer
      *  \param _e la date d'échéance de la tâche à créer
      */
    Tache( const QString& t, const QDate& d, const QDate& e):
            titre(t),dispo(d),echeance(e){}

    /*!
      *  \brief Constructeur par recopie
      *
      *  Constructeur par recopie protected de la classe Tache
      */
    Tache(const Tache& t);

    /*!
      *  \brief operateur=
      *
      *  operateur= en privé pour éviter la recopie
      */
	Tache& operator=(const Tache&);
    friend class Projet;
public:   
    /*!
        *  \brief Destructeur
        *
        *  Destructeur virtuel de la classe Tache
        */
    virtual ~Tache(){}

    /*!
     *  \brief Getter de titre
     *
     *  Methode qui permet de récupérer le titre de la tâche.
     *
     *  \return un QString
     */
    QString getId() const { return titre; }
    void setId(const QString& str);

    /*!
     *  \brief Getter de dispo
     *
     *  Methode qui permet de récupérer la date de dispo de la tâche.
     *
     *  \return une QDate
     */
    QDate getDateDispo() const {  return dispo; }

    /*!
     *  \brief Getter de echeance
     *
     *  Methode qui permet de récupérer la date d'échéance de la tâche.
     *
     *  \return une QDate
     */
    QDate getDateEcheance() const {  return echeance; }

    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e) {
        if (e<disp) throw CalendarException("erreur Tâche : date echéance < date disponibilité");
        dispo=disp; echeance=e;
    }

    /*!
     *  \brief rechercher une tache
     *
     *  Methode qui permet de trouver une tache grâce à son titre
     *
     *  \param list<Tache*> list dans laquelle cherche la tache
     *  \param Tache* la tâche à trouver
     *  \return Tache& une référence vers la tâche recherchée ou null
     */
    Tache* findTache(Tache* t,list<Tache*> l);//A TESTER, getTitre plutot que t

    /*!
     *  \brief rechercher une tache
     *
     *  Methode qui permet de trouver une tache grâce à son titre
     *
     *  \param list<Tache*> list dans laquelle cherche la tache
     *  \param QString& titre de la tâche à trouver
     *  \return Tache& une référence vers la tâche recherchée ou null
     */
    Tache* findTache(const QString& t,list<Tache*> l);//A TESTER, getTitre plutot que t

    /*!
     *  \brief rechercher une tache
     *
     *  Methode qui permet de trouver une tache grâce à son titre
     *
     *  \param list<Tache*> list dans laquelle cherche la tache
     *  \param Tache* la tâche à trouver
     *  \bool True si la tache existe, false sinon
     */
    bool existTache(Tache* t,list<Tache*> l);//A TESTER

    /*!
     *  \brief rechercher une tache
     *
     *  Methode qui permet de trouver une tache grâce à son titre
     *
     *  \param list<Tache*> list dans laquelle cherche la tache
     *  \param QString& titre de la tâche à trouver
     *  \bool True si la tache existe, false sinon
     */
    bool existTache(const QString& t,list<Tache*> l);//A TESTER

    /*!
     *  \brief Ajout d'une tâche précédente
     *
     *  Methode qui permet d'ajouter une tâche requise existante en tant que précédence.
     *
     *  \param tache pointeur vers la tâche à ajouter
     */
    void addPrecedente(Tache* t);


    /*!
     *  \brief Suppression d'une tâche précédente
     *
     *  Methode qui permet de retirer une tâche précédente.
     *
     *  \param QString& titre de la tâche à supprimer
     */
    void suppPrecedente(Tache* t);




    /*!
     *  \brief Affichage des informations de la tâche
     *
     *  Methode virtuelle pure permettant de rendre la classe abstraite.
     */
    virtual void DisplayTache() const = 0;
};


/*! \class Evenement
 * \brief classe abstraite représentant un evenement programmable
 *
 */
class Evenement{
    unsigned int duree;/*!< Durée de l'évènement*/

public :

    /*!
     *  \brief Constructeur
     *
     *  Constructeur public de la classe Evenement
     *
     *  \param _dur la durée de l'evenement a creer
     */
    Evenement(const unsigned int& dur):Evenement(dur){}

    /*!
     *  \brief Getter de duree
     *
     *  Methode qui permet de récupérer la duree d'un evenement.
     *
     *  \return une Duree
     */
    unsigned int getDuree() const { return duree; }
    void setDuree(const unsigned int& d) { duree=d; }

    bool estProgramme();
};


/*! \class Activite
 * \brief classe représentant activité traditionnelle
 *
 *  La classe Activite hérite de la classe Evenement.
 */
class Activite : public Evenement{

    QString description;/*!< Description de l'activite*/

public:
    /*!
     *  \brief Constructeur
     *
     *  Constructeur public de la classe Activite
     *
     *  \param _dur la durée de l'activite a creer
     *  \param _d la description de l'activite a creer
     */
    Activite(const unsigned int& dur,const QString& d):Evenement(dur),description(d){}

    /*!
     *  \brief Getter de description
     *
     *  Methode qui permet de récupérer la description d'une activite.
     *
     *  \return un QString
     */
    QString getDescription() const { return description; }
    void setDescription(const QString& d) { description=d; }
};

/*! \class TacheUnitaire
 * \brief classe représentant une tâche unitaire
 *
 *  La classe TacheUnitaire hérite de la classe Tache et de la classe Evenement.
 */
class TacheUnitaire : public Tache,Evenement{

    bool preemptable; /*!< Statut de la 'préemptivité' de la tâche*/


    /*!
     *  \brief Constructeur
     *
     *  Constructeur private de la classe TacheUnitaire
     *
     *  \param _t le titre de la tâche à créer
     *  \param _d la date de disponibilité de la tâche à créer
     *  \param _e la date d'échéance de la tâche à créer
     *  \param _dur la durée de la tâche à créer
     *  \param _p le statut de la preemptivité de la tâche
     */
    TacheUnitaire(const QString& t, const QDate& d, const QDate& e, const unsigned int& dur, const bool p):
        Tache(t,d,e),Evenement(dur),preemptable(p){
        if(dur > 12*60)
            throw CalendarException("Tache preemptable trop longue (12h max)");
    }
        ;
public:

    /*!
     *  \brief Getter preemptable
     *
     *  Methode qui permet de récupérer l'attribut preemptable d'une TacheUnitaire.
     *
     *  \return un boolean
     */
    bool ispreemptable() const { return preemptable; }

    /*!
     *  \brief Setter preemptable
     *
     *  Methode qui permet de modifier la valeur de l'attribut preemptable d'une TacheUnitaire.
     *
     *  \return void
     */
    void setpreemptable(bool b) { preemptable=b; }

};


/*! \class TacheComposite
 * \brief classe représentant une tache composite, composée de taches
 *
 *  La classe Tache composite hérite de la classe Tache.
 */
class TacheComposite : public Tache{

    list<Tache*> sous_taches;/*!< Description de l'activite*/

    /*!
     *  \brief Constructeur
     *
     *  Constructeur private de la classe TacheComposite
     *
     *  \param _t le titre de la tâche à créer
     *  \param _d la date de disponibilité de la tâche à créer
     *  \param _e la date d'échéance de la tâche à créer
     */
    TacheComposite(const QString& t, const QDate& d, const QDate& e):
        Tache(t,d,e),sous_taches(0){}
public :

    /*!
     * \brief ~TacheComposite
     *
     * Destructuer virtuel de Tache Composite
     */
    virtual ~TacheComposite() {}


    /*!
     *  \brief Ajout d'une sous tâche
     *
     *  Methode qui permet d'ajouter une sous tâche existante à la tâche composite.
     *
     *  \param t pointeur vers la tâche à ajouter
     */
    void addSousTache(Tache* t);


    /*!
     *  \brief Suppression d'une sous tâche
     *
     *  Methode qui permet de supprimer une tâche de la liste des sous tâches.
     *
     *  \param t le titre de la sous tâche à supprimer
     */
    void suppSousTache(const QString& t);


    /*!
     *  \brief Récupération d'une sous tache de la tâche composite
     *
     *  Methode qui permet de récupérer une sous tache ciblée de la tâche.
     *
     *  \param t le titre de la sous tâche à récupérer
     *  \return une référence sur la tâche recherchée
     */
    Tache* getTacheComposante(const QString& t);


    /*!
     *  \brief Getter des sous tâches de la tâche.
     *
     *  Methode qui permet de récupérer toutes les tâches composantes de la tâche.
     *
     *  \return une référenc vers une list de toutes les sous tâches de la tâche
     */
    list<Tache*>& getSousTaches() { return sous_taches; }
};

QTextStream& operator<<(QTextStream& f, const Tache& t);


class TacheManager {

	Tache** taches;
	unsigned int nb;
	unsigned int nbMax;
	void addItem(Tache* t);

    QString file;
    TacheManager():taches(0),nb(0),nbMax(0){}
	~TacheManager();
    TacheManager(const TacheManager& tm);
    TacheManager& operator=(const TacheManager& tm);
	struct Handler{
		TacheManager* instance;
		Handler():instance(0){}
		// destructeur appelé à la fin du programme
		~Handler(){ if (instance) delete instance; }
	};
	static Handler handler;
public:
    Tache& ajouterTache(Tache* t);
    void TacheManager::ajoutTacheUni (const QString& t, const QDate& d, const QDate& e, const Duree& dur, const bool p);
    void TacheManager::ajoutTacheComp(const QString& t, const QDate& d, const QDate& e);
     Tache& getTache(const QString& id);
    bool isTacheExistante(const QString& id) const { return trouverTache(id)!=0; }
    const Tache& getTache(const QString& code) const;
    void load(const QString& f);
    void save(const QString& f);
    Tache* trouverTache(const QString& t) const;
	static TacheManager& getInstance();
	static void libererInstance();

};



/*! \class Programmation
 * \brief classe représentant la programmation d'un evenement a une date un horaire précis
 *
 *  La classe Tache composite hérite de la classe Tache.
 */
class Programmation {

    const Evenement* eve;/*!< Evenement programmé*/
    QDate date;/*!< Date de programmation*/
    QTime horaire;/*!< Horaire de programmation*/

    /*!
     *  \brief Constructeur
     *
     *  Constructeur privé d'une programmation
     *
     *  \param _t le titre de la tâche à créer
     *  \param _d la date de disponibilité de la tâche à créer
     *  \param _e la date d'échéance de la tâche à créer
     */
    Programmation(const Evenement* e, const QDate& d, const QTime& h):eve(e), date(d), horaire(h){}

    /*!
      *  \brief Constructeur par recopie
      *
      *  Constructeur par recopie privé de la classe programmation
      */
    Programmation(const Programmation& p);

    /*!
      *  \brief operateur=
      *
      *  operateur d'affectation en privé pour éviter la recopie
      */
    Programmation& operator=(const Programmation&);

public:


    /*!
     *  \brief Getter de l'evenement programmé
     *
     *  \return Evenement& l'evenement programmé
     */
    const Evenement& getProg() const { return *eve; }


    /*!
     *  \brief Getter de la date de programmation
     *
     *  \return QDate La date de programmation
     */
    QDate getDate() const { return date; }


    /*!
     *  \brief Getter de l'horaire de programmation
     *
     *  \return QTime l'horaire de programmation
     */
    QTime getHoraire() const { return horaire; }
};


class ProgrammationManager {


	Programmation** programmations;


	void addItem(Programmation* t);
    Programmation* trouverProgrammation(const Evenement& e) const;
public:
	ProgrammationManager();
	~ProgrammationManager();
	ProgrammationManager(const ProgrammationManager& um);
	ProgrammationManager& operator=(const ProgrammationManager& um);
    void ajouterProgrammation(const Evenement& e, const QDate& d, const QTime& h);
};


#endif
