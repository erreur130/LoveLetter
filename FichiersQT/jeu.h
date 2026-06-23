#ifndef JEU_H
#define JEU_H

#include "QVector"
#include "humain.h"
#include "ia.h"
#include "paquet.h"

class Jeu {
    short int objectifPoints;
    QVector<Joueur*> joueurs;
    Joueur* joueurActuel;
    Paquet pioche;
    public:
        Jeu(short int, short int, short int);
        ~Jeu();
        void afficheJoueursRestant() const;
        inline Joueur* avoirJoueurActuel() const {return joueurActuel;};
        inline QVector<Joueur*> avoirJoueurs() const {return joueurs;};
        void tourSuivant();
        void miseAJourCartesPotentiel(Carte*);
        bool aGagnerLeJeu(Joueur*) const;
        Joueur* verifSiGagnant() const;
        void reinitialiserManche();
        void eliminationJoueur(Joueur*);
        void lancer();
        void tourDeJeu();

};

#endif // JEU_H
