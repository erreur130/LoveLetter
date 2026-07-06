#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "jeu.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
        Q_OBJECT
    Ui::MainWindow *ui;
    Jeu* jeu;
    QVector<QString> joueursCible; // liste les joueurs en vie
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;
        void lancer();
    private slots:
        void on_actionRegles_de_jeu_triggered();
        void on_actionListe_des_cartes_triggered();
        void on_actionProjet_triggered();
        void recevoirJoueur(short int h, short int inul, short int inorm, short int itri);
        void on_listeCarteChoixGarde_itemClicked(QListWidgetItem *item);
        void on_listeJoueursCible_itemClicked(QListWidgetItem *item);
        void on_listeCarteMain_itemClicked(QListWidgetItem *item);
        void on_listeCarteAGarder_itemClicked(QListWidgetItem *item);

    signals:
        void envoyerSuiteAction6(Joueur*, short int carteAGarder);
        void envoyerChoixCarte(short int idCarte);
        void envoyerChoixValeurGarde(short int valeur);
        void envoyerChoixCibleJoueur(short int joueur);
        void rejouer();
    public slots:
        void recevoirChoixCarteAGarder(Joueur* joueurARenvoyer);
        void recevoirMessageLog(QString);
        void recevoirReinitialiserLog(); // à chaque manche
        void recevoirDemanderChoixValeurGarde(QVector<Carte*>, QVector<short int>);
        void recevoirInitialiserListeJoueurs(QVector<QString> nomJoueurs); // liste complète pour l'affichage
        void recevoirDemanderChoixCibleJoueur(QVector<QString> nomJoueurs, QVector<short int> idJoueurs); // liste réduite
        void recevoirMiseAJourPointsJoueurs(QVector<short int>);
        void recevoirMessageAlerteMainJoueurVasEtreMontre(QString, bool);
        void recevoirAfficherMain(Carte* carte1, Carte* carte2);
        void recevoirAfficherVictoireManche(QVector<QString> nomJoueurs);
        void recevoirAfficherVictoireJeu(QVector<QString> nomJoueurs);
        void recevoirMiseAJourCartesJouees(QVector<short int>);
        void recevoirMiseAJourNbCartesRestantes(short int);
        void recevoirAfficherCarte(Carte*);

};
#endif // MAINWINDOW_H
