#ifndef CALENDAR_h
#define CALENDAR_h
#include <QString>
#include <QDate>
#include <QTextStream>
#include <list>
#include  "programmationmanager.h"
#include "projetmanager.h"

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
        list<Tache*> taches;/*!< list de t�ches du projet*/

        /*!
         *  \brief Constructeur
         *
         *  Constructeur de la classe Projet
         *
         *  \param id l''id du projet � cr�er
         *  \param d la date de disponibilit� du projet
         *  \param e la date d'�ch�ance du projet
         */
        Projet(const QString& id,const QDate& d, const QDate& e):
                id(id),disponibilite(d), echeance(e){}

        /*!
         * \brief Destructeur
         *
         */
        ~Projet(){taches.clear();}
        friend class ProjetManager;

    public:


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
         *  \brief trouve la Tache li� au projet au titre correspondant
         *
         *  \param t le titre de la tache � trouver
         *  \return Tache* Pointeur sur la tache recherch� ou 0
         */
        Tache* trouverTache(const QString& t);

        /*!
         *  \brief trouve la Tache correspondant dans les tache de projet
         *
         *  \param t la tache � trouver
         *  \return bool true si trouv�e, false sinon
         */
        bool ExistTache(Tache* t);

        /*!
         *  \brief Getter de la list de taches d'un projet
         *
         *  \return list<Tache*> La liste de tache du projet
         */
        list<Tache*>& getTaches(){return taches;}

        /*!
         *  \brief ajout d'une t�che
         *
         *  \param t Pointeur vers la tache � ajouter
         */
        void ajouterTache(Tache*);

        /*!
         *  \brief suppression d'une t�che
         *
         *  \param t le titre de la t�che � supprimer
         */
        void suppTache(const QString &t);

        /*!
         *  \brief R�cup�ration d'une t�che
         *
         *  Template Methode qui permet de r�cup�rer une r�f�rence sur une t�che voulue.
         *  La t�che peut �tre de tout type de t�che.
         *
         *  \param nomTache le nom de la t�che � r�cup�rer
         *  \return une r�f�rence sur la t�che demand�e
         */
         template<typename T> T& getTache(const QString& t);


        /*!
         *  \brief ajout d'une tacheUnitaire
         *
         *  \param t le titre de la t�che � cr�er
         *  \param d date dispo de la t�che
         *  \param d date d'�cheance de la t�che
         *  \param dur duree de la tache Unitaire
         *  \param p preemptable
         */
        void ajoutTacheUni (const QString& t, const QDate& d, const QDate& e, const unsigned int& dur, const QString& l, const bool p);


        /*!
         *  \brief ajout d'une tacheComposite
         *
         *  \param t le titre de la t�che � cr�er
         *  \param d date dispo de la t�che
         *  \param d date d'�cheance de la t�che
         */
        void ajoutTacheComp(const QString& t, const QDate& d, const QDate& e);
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
 * \brief classe abstraite repr�sentant une t�che g�n�rique
 *
 */
class Tache {
   protected:
    QString titre;/*!< Nom de la t�che*/

    QDate dispo;/*!< Date de disponibilit� de la t�che*/
    QDate echeance;/*!< Date d'�ch�ance de la t�che*/
    list<Tache*> precedence;/*!< Liste des pr�c�dences requises*/
    /*!
      *  \brief Constructeur
      *
      *  Constructeur protected de la classe Tache
      *
      *  \param _t le titre de la t�che � cr�er
      *  \param _d la date de disponibilit� de la t�che � cr�er
      *  \param _e la date d'�ch�ance de la t�che � cr�er
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
      *  operateur= en priv� pour �viter la recopie
      */
	Tache& operator=(const Tache&);

    /*!
     *  \brief Destructeur
     *
     *  Destructeur virtuel de la classe Tache
     */
    virtual ~Tache(){}

    friend class Projet;
public:   


    /*!
     *  \brief Getter de titre
     *
     *  Methode qui permet de r�cup�rer le titre de la t�che.
     *
     *  \return un QString
     */
    QString getId() const { return titre; }
    void setId(const QString& str);

    /*!
     *  \brief Getter de dispo
     *
     *  Methode qui permet de r�cup�rer la date de dispo de la t�che.
     *
     *  \return une QDate
     */
    QDate getDateDispo() const {  return dispo; }

    /*!
     *  \brief Getter de echeance
     *
     *  Methode qui permet de r�cup�rer la date d'�ch�ance de la t�che.
     *
     *  \return une QDate
     */
    QDate getDateEcheance() const {  return echeance; }

    void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e) {
        if (e<disp) throw CalendarException("erreur T�che : date ech�ance < date disponibilit�");
        dispo=disp; echeance=e;
    }

    /*!
     *  \brief rechercher une tache
     *
     *  Methode qui permet de trouver une tache gr�ce � son titre
     *
     *  \param list<Tache*> list dans laquelle cherche la tache
     *  \param Tache* la t�che � trouver
     *  \return Tache& une r�f�rence vers la t�che recherch�e ou null
     */
    Tache* findTache(Tache* t,list<Tache*> l);

    /*!
     *  \brief rechercher une tache
     *
     *  Methode qui permet de trouver une tache gr�ce � son titre
     *
     *  \param list<Tache*> list dans laquelle cherche la tache
     *  \param QString& titre de la t�che � trouver
     *  \return Tache& une r�f�rence vers la t�che recherch�e ou null
     */
    Tache* findTache(const QString& t,list<Tache*> l);

    /*!
     *  \brief rechercher une tache
     *
     *  Methode qui permet de trouver une tache gr�ce � son titre
     *
     *  \param list<Tache*> list dans laquelle cherche la tache
     *  \param Tache* la t�che � trouver
     *  \bool True si la tache existe, false sinon
     */
    bool existTache(Tache* t,list<Tache*> l);

    /*!
     *  \brief rechercher une tache
     *
     *  Methode qui permet de trouver une tache gr�ce � son titre
     *
     *  \param list<Tache*> list dans laquelle cherche la tache
     *  \param QString& titre de la t�che � trouver
     *  \bool True si la tache existe, false sinon
     */
    bool existTache(const QString& t,list<Tache*> l);

    /*!
     *  \brief Ajout d'une t�che pr�c�dente
     *
     *  Methode qui permet d'ajouter une t�che requise existante en tant que pr�c�dence.
     *
     *  \param tache pointeur vers la t�che � ajouter
     */
    void addPrecedente(Tache* t);

    /*!
     *  \brief Suppression d'une t�che pr�c�dente
     *
     *  Methode qui permet de retirer une t�che pr�c�dente.
     *
     *  \param QString& titre de la t�che � supprimer
     */
    void suppPrecedente(Tache* t);

    bool operator ==(Tache* t1){
        return(titre==t1->getId());
    }

    bool operator ==(const Tache* t1){
        return(titre==t1->getId());
    }

    template<typename T>
    bool EstProgrammable();

    /*!
     *  \brief Affichage des informations de la t�che
     *
     *  Methode virtuelle pure permettant de rendre la classe abstraite.
     */
    //virtual void DisplayTache() const = 0;
};


/*! \class Evenement
 * \brief classe abstraite repr�sentant un evenement programmable
 *
 */
class Evenement{
  protected:
    unsigned int duree;/*!< Dur�e de l'�v�nement*/

    QString description;/*!< description de l'�v�nement*/



public :

    /*!
     *  \brief Constructeur
     *
     *  Constructeur public de la classe Evenement
     *
     *  \param _dur la dur�e de l'evenement a creer
     */
    Evenement(const unsigned int& dur, const QString& d):duree(dur),description(d){}

    /*!
     *  \brief Getter de duree
     *
     *  Methode qui permet de r�cup�rer la duree d'un evenement.
     *
     *  \return une Duree
     */
    unsigned int getDuree() const { return duree; }
    void setDuree(const unsigned int& d) { duree=d; }

    /*!
     *  \brief Getter de description
     *
     *  Methode qui permet de r�cup�rer la description d'une activite.
     *
     *  \return un QString
     */
    QString getDescription() const { return description; }

    /*!
     *  \brief Un evenement est-il programm�
     *
     *
     *  \return bool true si programm�, false sinon
     */
    bool estProgramme();
};


/*! \class Activite
 * \brief classe repr�sentant activit� traditionnelle
 *
 *  La classe Activite h�rite de la classe Evenement.
 */
class Activite : public Evenement{

    QString lieu;

public:
    /*!
     *  \brief Constructeur
     *
     *  Constructeur public de la classe Activite
     *
     *  \param _dur la dur�e de l'activite a creer
     *  \param _d la description de l'activite a creer
     */
    Activite(const unsigned int& dur,const QString& d,const QString& l):Evenement(dur,d),lieu(l){}



    /*!
     *  \brief Getter de lieu
     *
     *  Methode qui permet de r�cup�rer le lieu d'une activite.
     *
     *  \return un QString
     */
    QString getLieu(){return lieu;}
    void setLieu(const QString& l) { lieu=l; }
};

/*! \class TacheUnitaire
 * \brief classe repr�sentant une t�che unitaire
 *
 *  La classe TacheUnitaire h�rite de la classe Tache et de la classe Evenement.
 */
class TacheUnitaire : public Tache,Evenement{

    bool preemptable; /*!< Statut de la 'pr�emptivit�' de la t�che*/


    /*!
     *  \brief Constructeur
     *
     *  Constructeur private de la classe TacheUnitaire
     *
     *  \param _t le titre de la t�che � cr�er
     *  \param _d la date de disponibilit� de la t�che � cr�er
     *  \param _e la date d'�ch�ance de la t�che � cr�er
     *  \param _dur la dur�e de la t�che � cr�er
     *  \param _p le statut de la preemptivit� de la t�che
     */
    TacheUnitaire(const QString& t, const QDate& d, const QDate& e, const unsigned int& dur, const QString& des, const bool p):
        Tache(t,d,e),Evenement(dur,des),preemptable(p){
        if(dur > 12*60)
            throw CalendarException("Tache preemptable trop longue (12h max)");
    }
    friend class Projet;
public:

    /*!
     *  \brief Getter preemptable
     *
     *  Methode qui permet de r�cup�rer l'attribut preemptable d'une TacheUnitaire.
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

    //void DisplayTache(std::ostream& f= std::cout);
};


/*! \class TacheComposite
 * \brief classe repr�sentant une tache composite, compos�e de taches
 *
 *  La classe Tache composite h�rite de la classe Tache.
 */
class TacheComposite : public Tache{

    list<Tache*> sous_taches;/*!< Description de l'activite*/

    /*!
     *  \brief Constructeur
     *
     *  Constructeur private de la classe TacheComposite
     *
     *  \param _t le titre de la t�che � cr�er
     *  \param _d la date de disponibilit� de la t�che � cr�er
     *  \param _e la date d'�ch�ance de la t�che � cr�er
     */
    TacheComposite(const QString& t, const QDate& d, const QDate& e):
        Tache(t,d,e),sous_taches(0){}
    friend class Projet;
public :

    /*!
     * \brief ~TacheComposite
     *
     * Destructuer virtuel de Tache Composite
     */
    virtual ~TacheComposite() {}


    /*!
     *  \brief Ajout d'une sous t�che
     *
     *  Methode qui permet d'ajouter une sous t�che existante � la t�che composite.
     *
     *  \param t pointeur vers la t�che � ajouter
     */
    void addSousTache(Tache* t);


    /*!
     *  \brief Suppression d'une sous t�che
     *
     *  Methode qui permet de supprimer une t�che de la liste des sous t�ches.
     *
     *  \param t le titre de la sous t�che � supprimer
     */
    void suppSousTache(const QString& t);


    /*!
     *  \brief R�cup�ration d'une sous tache de la t�che composite
     *
     *  Methode qui permet de r�cup�rer une sous tache cibl�e de la t�che.
     *
     *  \param t le titre de la sous t�che � r�cup�rer
     *  \return une r�f�rence sur la t�che recherch�e
     */
    Tache* getTacheComposante(const QString& t);


    /*!
     *  \brief Getter des sous t�ches de la t�che.
     *
     *  Methode qui permet de r�cup�rer toutes les t�ches composantes de la t�che.
     *
     *  \return une r�f�renc vers une list de toutes les sous t�ches de la t�che
     */
    list<Tache*>& getSousTaches() { return sous_taches; }

    //void DisplayTache(std::ostream& f= std::cout);
};

QTextStream& operator<<(QTextStream& f, const Tache& t);




/*! \class Programmation
 * \brief classe repr�sentant la programmation d'un evenement a une date un horaire pr�cis
 *
 *  La classe Tache composite h�rite de la classe Tache.
 */
class Programmation {

    const Evenement* eve;/*!< Evenement programm�*/
    QDate date;/*!< Date de programmation*/
    QTime horaire;/*!< Horaire de programmation*/

    /*!
     *  \brief Constructeur
     *
     *  Constructeur priv� d'une programmation
     *
     *  \param _t le titre de la t�che � cr�er
     *  \param _d la date de disponibilit� de la t�che � cr�er
     *  \param _e la date d'�ch�ance de la t�che � cr�er
     */
    Programmation(const Evenement* e, const QDate& d, const QTime& h):eve(e), date(d), horaire(h){}

    /*!
      *  \brief Constructeur par recopie
      *
      *  Constructeur par recopie priv� de la classe programmation
      */
    Programmation(const Programmation& p);

    /*!
      *  \brief operateur=
      *
      *  operateur d'affectation en priv� pour �viter la recopie
      */
    Programmation& operator=(const Programmation&);

    friend class ProgrammationManager;

public:


    /*!
     *  \brief Getter de l'evenement programm�
     *
     *  \return Evenement& l'evenement programm�
     */
    const Evenement* getEvenement(){ return const_cast<const Evenement*>(eve); }


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

<<<<<<< HEAD
<<<<<<< HEAD
/*<<<<<<< HEAD
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
    void saveActivite(const QString& fichierbis);

};
=======

>>>>>>> origin/master*/
=======


>>>>>>> origin/master


=======
>>>>>>> origin/master
#endif
