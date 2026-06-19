#ifndef JEU_H
#define JEU_H

#include "QVector"
#include "joueur.h"
#include "paquet.h"

class Jeu {
    short int objectifPoints;
    QVector<Joueur*> joueurs;
    Joueur* joueurActuel;
    Paquet pioche;
    public:
        Jeu(short int, short int, QString);
        ~Jeu();
        void afficheJoueursRestant() const;
        Joueur* avoirJoueurActuel() const;
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
