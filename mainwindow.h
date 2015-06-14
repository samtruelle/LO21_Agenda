#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QTextStream>
#include <QPlainTextEdit>
#include <QStyledItemDelegate>
#include <QTextDocument>
#include <QPainter>
#include<QTableWidget>
#include <QStyleOptionViewItem>
#include <QModelIndex>
#include <QItemDelegate>
#include <QStyle>
#include <QGraphicsWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QGraphicsTextItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QTableWidgetItem>
#include "Calendar.h"
#include "programmationmanager.h"
#include"projetmanager.h"
#include <QComboBox>
#include <QCheckBox>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    Ui::MainWindow *ui;
    //void donnerProgrammation( Programmation& p);
    ProjetManager &projm;  /*!< Référence vers la classe singleton ProjetManager*/
   ProgrammationManager &progm; /*!< Référence vers la classe singleton EvenementManager*/
   Projet* currentProjet; /*!< Projet sélectionné dans l'interface graphique*/
   int firstDay; /*!< Numéro du premier jour affiché sur le calendrier*/
   QDate firstDate; /*!< Première date affichée sur le calendrier*/
   /*!
    *  \brief Getter du projet actuel
    *
    *  Methode qui permet de récupérer une référence sur le projet actuellement utilisé dans le calendrier.
    *
    *  \return une référence sur un projet
    */
   Projet& curProjet() { return *currentProjet; }

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
   void nouveauProjet();
   void ajouterTache();
   void ajoutersoustache();
   void ProgrammerActivite();
    void ProgrammerTache();


};

#endif

// MAINWINDOW_H

