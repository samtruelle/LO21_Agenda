/*
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
void afficherProgrammation(Programmation& p);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void update();
    void afficherEdt();
    void ajouterTache();
    void ajouterActivite();
    void afficherEvt();
    void displayProgrammation(const Programmation& p);
    void afficherProjet();
    void ajoutersousTache();
    void afficherPrec();
public slots:
    void on_AjouterTache_clicked();
     void on_AjouterActivite_clicked();
     void on_ProgrammeEvt_clicked();



};
<<<<<<< HEAD
#endif
*/
=======

#endif // MAINWINDOW_H
>>>>>>> origin/master
