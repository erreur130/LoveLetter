#ifndef JEU_H
#define JEU_H

#include <QVector>
#include "humain.h"
#include "ianul.h"
#include "ianormale.h"
#include "iatriche.h"
#include "paquet.h"
#include <iostream>
#include <QTimer>

class Jeu : public QObject {
    Q_OBJECT
    short int objectifPoints;
    QVector<Joueur*> joueurs;
    Joueur* joueurActuel;
    Joueur* joueurCible;
    Carte* carteEnCourDeJeux;
    Paquet pioche;

    public:
        Jeu(QObject*);
        Jeu(QObject*, short int, short int, short int, short int);
        ~Jeu();
        QVector<Joueur*> JoueursRestant() const;
        QVector<Joueur*> JoueursChoixPossible() const;
        inline Joueur* avoirJoueurActuel() const {return joueurActuel;};
        inline QVector<Joueur*> avoirJoueurs() const {return joueurs;};
        inline const Paquet& avoirPaquet() const {return pioche;};
        bool tourSuivant();
        void miseAJourCartesPotentiel(Carte* cartePerdent = nullptr); //  paramètre nésésaire en cas de baron (num 3) et garde (num 1)
        QVector<Joueur*> verifSiGagnants() const;
        void reinitialiserManche();
        void eliminationJoueur(Joueur*);
        void lancerManche();
        void lancerTour();
        void finDeManche(bool finDuPaquet);
    signals: // Jeu -> MainWindow
        void messageLog(QString);
        void reinitialiserLog(); // à chaque manche
        void demanderChoixValeurGarde(QVector<Carte*>, QVector<short int>);
        void initialiserListeJoueurs(QVector<QString> nomJoueurs); // liste complète pour l'affichage
        void demanderChoixCibleJoueur(QVector<QString> nomJoueurs, QVector<short int> idJoueurs); // liste réduite
        void miseAJourPointsJoueurs(QVector<short int>);
        void messageAlerteMainJoueurVasEtreMontre(QString, bool popup);
        void afficherMain(Carte* carte1, Carte* carte2);
        void afficherVictoireManche(QVector<QString> nomJoueurs);
        void afficherVictoireJeu(QVector<QString> nomJoueurs);
        void miseAJourCartesJouees(QVector<short int>);
        void miseAJourNbCartesRestantes(short int);
        void afficherCarte(Carte*);
    public slots: // MainWindow -> Jeu
        void recevoirChoixCarte(short int idCarte);
        void recevoirChoixValeurGarde(short int valeur);
        void recevoirChoixCibleJoueur(short int joueur);
        void rejouer();
        // Carte6 -> Jeu
        void recevoirContinuer();

};

#endif // JEU_H
