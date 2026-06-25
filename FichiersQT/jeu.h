#ifndef JEU_H
#define JEU_H

#include <QVector>
#include "humain.h"
#include "ianul.h"
#include "ianormale.h"
#include "iatriche.h"
#include "paquet.h"
#include <algorithm> // Pour std::sort et std::unique
#include <iostream>

class Jeu {
    short int objectifPoints;
    QVector<Joueur*> joueurs;
    Joueur* joueurActuel;
    Paquet pioche;
    public:
        Jeu(short int, short int, short int, short int);
        ~Jeu();
        QVector<Joueur*> JoueursRestant() const;
        inline Joueur* avoirJoueurActuel() const {return joueurActuel;};
        inline QVector<Joueur*> avoirJoueurs() const {return joueurs;};
        bool tourSuivant();
        void miseAJourCartesPotentiel(Carte* carteJouer, Joueur* autreJoueur = nullptr, Carte* cartePerdent = nullptr); // les deux autres paramètre sont nésésaire en cas de baron (num 3)
        QVector<Joueur*> verifSiGagnants() const;
        void reinitialiserManche();
        void eliminationJoueur(Joueur*);
        void lancer();
        void tourDeJeu();

};

#endif // JEU_H
