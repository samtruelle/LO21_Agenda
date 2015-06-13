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


class Projet {
        QString id;
        unsigned int nb;
        unsigned int nbMax;
        QDate disponibilite;
        QDate echeance;
        Tache** taches;

        Projet(const QString& id,const QDate& d, const QDate& e):
                id(id),nb(0),nbMax(0),disponibilite(d), echeance(e){}
        friend class ProjetManager;
    public:
        QString getId() const { return id; }
        void setId(const QString& id);

        unsigned int getNb() const {return nb;}
        unsigned int getNbMax() const {return nbMax;}

        QDate getDateDisponibilite() const { return disponibilite; }
        QDate getDateEcheance() const { return echeance; }
        void setDatesDisponibiliteEcheance(const QDate& disp, const QDate& e);
        Tache* trouverTache(const QString& t) const;

        //ListTachesConst getTaches() const;
        //void addTaches(const ListTaches &t);
        void addTache(const Tache* t);
        //void suppTache(const Tache* t);

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
 * \brief classe abstraite repr�sentant une t�che g�n�rique
 *
 */
class Tache {
    QString titre;/*!< Nom de la t�che*/

    QDate dispo;/*!< Date de disponibilit� de la t�che*/
    QDate echeance;/*!< Date d'�ch�ance de la t�che*/
    list<Tache*> precedence;/*!< Liste des pr�c�dences requises*/
    /*!
      *  \brief Constructeur
      *
      *  Constructeur priv� de la classe Tache
      *
      *  \param _t le titre de la t�che � cr�er
      *  \param _d la date de disponibilit� de la t�che � cr�er
      *  \param _e la date d'�ch�ance de la t�che � cr�er
      */
    Tache( const QString& t, const QDate& d, const QDate& e):
            titre(t),dispo(d),echeance(e){}

    //Tache* trouverTache(const QString& id) const;
    /*!
      *  \brief Constructeur par recopie
      *
      *  Constructeur par recopie priv� de la classe Tache
      */
    Tache(const Tache& t);

    /*!
      *  \brief operateur=
      *
      *  operateur= en priv� pour �viter la recopie
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
     *  Methode qui permet de r�cup�rer le titre de la t�che.
     *
     *  \return un QString
     */
    QString getTitre() const { return titre; }
    void setTitre(const QString& str);

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
     *  \brief Ajout d'une t�che pr�c�dente
     *
     *  Methode qui permet d'ajouter une t�che requise existante en tant que pr�c�dence.
     *
     *  \param tache pointeur vers la t�che � ajouter
     */
    void addPrecedente(const Tache* t);


    /*!
     *  \brief Suppression d'une t�che pr�c�dente
     *
     *  Methode qui permet de retirer une t�che pr�c�dente.
     *
     *  \param QString& titre de la t�che � supprimer
     */
    void suppPrecedente(const QString& title);

    /*!
     *  \brief Affichage des informations de la t�che
     *
     *  Methode virtuelle pure permettant de rendre la classe abstraite.
     */
    virtual void DisplayTache() const = 0;
};


class Evenement{

};

class TacheUnitaire : public Tache,Evenement{
    bool preemptive;
    Duree duree;

public :
    Duree getDuree() const { return duree; }
    void setDuree(const Duree& d) { duree=d; }
    bool isPreemptive() const { return preemptive; }
    void setPreemptive(bool b) { preemptive=b; }

};

class TacheComposite : public Tache{
    Tache** sous_taches;
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
		// destructeur appel� � la fin du programme
		~Handler(){ if (instance) delete instance; }
	};
	static Handler handler;
public:
    Tache& ajouterTache(const QString& id, const QString& t, const Duree& dur, const QDate& dispo, const QDate& deadline, bool preempt=false);
    Tache& getTache(const QString& id);
    bool isTacheExistante(const QString& id) const { return trouverTache(id)!=0; }
    const Tache& getTache(const QString& code) const;
    //void load(const QString& f);
    //void save(const QString& f);
    Tache* trouverTache(const QString& t) const;
	static TacheManager& getInstance();
	static void libererInstance();

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


class Programmation {
    const Evenement* eve;
    QDate date;
    QTime horaire;
public:
    Programmation(const Evenement& e, const QDate& d, const QTime& h):eve(&e), date(d), horaire(h){}
    const Evenement& getProg() const { return *eve; }
    QDate getDate() const { return date; }
    QTime getHoraire() const { return horaire; }
};

class ProgrammationManager {

	Programmation** programmations;
	unsigned int nb;
	unsigned int nbMax;
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
