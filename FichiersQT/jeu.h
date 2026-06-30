#ifndef JEU_H
#define JEU_H

#include <QVector>
#include "humain.h"
#include "ianul.h"
#include "ianormale.h"
#include "iatriche.h"
#include "paquet.h"
#include <iostream>

class Jeu : public QObject {
    Q_OBJECT
    short int objectifPoints;
    QVector<Joueur*> joueurs;
    Joueur* joueurActuel;
    Paquet pioche;
    public:
        Jeu(QObject*, short int, short int, short int, short int);
        ~Jeu();
        QVector<Joueur*> JoueursRestant() const;
        inline Joueur* avoirJoueurActuel() const {return joueurActuel;};
        inline QVector<Joueur*> avoirJoueurs() const {return joueurs;};
        inline const Paquet& avoirPaquet() const {return pioche;};
        bool tourSuivant();
        void miseAJourCartesPotentiel(Carte* carteJouer, Joueur* autreJoueur = nullptr, Carte* cartePerdent = nullptr); // les deux autres paramètre sont nésésaire en cas de baron (num 3)
        QVector<Joueur*> verifSiGagnants() const;
        void reinitialiserManche();
        void eliminationJoueur(Joueur*);
        void lancerManche();
        void lancerTour();
    signals: // Jeu -> MainWindow
        void messageLog(QString);
        void reinitialiserLog(); // à chaque manche
        void joueurElimine(QString);
        void reinitialiserJoueurARetirerChoix();
        void demanderChoixCarte();
        void demanderChoixValeurGarde();
        void initialiserListeJoueurs(QVector<QString> nomJoueurs); // liste complète pour l'affichage
        void demanderChoixCibleJoueur(QVector<QString> nomJoueurs, QVector<short int> idJoueurs); // liste réduite
        void miseAJourPointsJoueurs(QVector<short int>);
        void messageAlerteMainJoueurVasEtreMontre(QString);
        void afficherMain(short int carte1, short int carte2);
        void afficherVictoireManche(QVector<QString> nomJoueurs);
        void afficherVictoireJeu(QVector<QString> nomJoueurs);
    public slots: // MainWindow -> Jeu
        void recevoirChoixCarte(short int idCarte);
        void recevoirChoixValeurGarde(short int valeur);
        void recevoirChoixCibleJoueur(short int joueur);
        void rejouer();

};

#endif // JEU_H
