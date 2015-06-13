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
    void afficherEvt();
    void displayProgrammation(const Programmation& p);

public slots:
    void on_AfficherTache_clicked();
    void on_AjouterEvt_clicked();
     void on_AjouterTache_clicked();
    //void on_MainWindow_quit();
    void on_suivant_clicked();
    void on_precedent_clicked();
    void on_ajoutProg_clicked();

};
