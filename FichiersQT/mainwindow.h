#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "jeu.h"
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
        Q_OBJECT
    Jeu* jeu;
    Ui::MainWindow *ui;
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;
    private:
        void miseAJourJoueursStatistiques();
        void miseAJourPioche();
        void miseAJourListCartesJouer();
        void creationCartesJoueurActuel();
        void afficherVictoireManche();
        void afficherVictoireJeu();
    private slots:
        //void on_ListWiged_listCartes_activated(index : int)
        //void on_ListWiged_listJoueurs_activated(index : int)
        void on_actionReges_de_jeu_triggered();
        void on_actionListe_des_cartes_triggered();
        void on_actionProjet_triggered();
        void recevoirJoueur(short int, short int, short int, short int);

};
#endif // MAINWINDOW_H
