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
    Ui::MainWindow *ui;
    Jeu* jeu;
    QVector<QString> joueursCible; // liste les joueurs en vie
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;
    private slots:
        //void on_ListWiged_listCartes_activated(index : int);
        //void on_ListWiged_listJoueurs_activated(index : int); // à mettre autre part ------------------------------------------------------
        void on_actionReges_de_jeu_triggered();
        void on_actionListe_des_cartes_triggered();
        void on_actionProjet_triggered();
        void recevoirJoueur(short int h, short int inul, short int inorm, short int itri);
    signals:
        void envoyerSuiteAction6(Joueur*, short int carteAGarder);
        void envoyerChoixCarte(short int idCarte);
        void envoyerChoixValeurGarde(short int valeur);
        void envoyerChoixCibleJoueur(short int joueur);
        void rejouer();
    public slots:
        void afficheChoixToutesLesCartesSauf1(Joueur* joueurARenvoyer);
        void recevoirMessageLog(QString);
        void recevoirReinitialiserLog(); // à chaque manche
        void recevoirDemanderChoixCarte();
        void recevoirDemanderChoixValeurGarde();
        void recevoirInitialiserListeJoueurs(QVector<QString> nomJoueurs); // liste complète pour l'affichage
        void recevoirDemanderChoixCibleJoueur(QVector<QString> nomJoueurs, QVector<short int> idJoueurs); // liste réduite
        void recevoirMiseAJourPointsJoueurs(QVector<short int>);
        void recevoirMessageAlerteMainJoueurVasEtreMontre(QString);
        void recevoirAfficherMain(QString imageCarte1, QString imageCarte2);
        void recevoirAfficherVictoireManche(QVector<QString> nomJoueurs);
        void recevoirAfficherVictoireJeu(QVector<QString> nomJoueurs);

};
#endif // MAINWINDOW_H
