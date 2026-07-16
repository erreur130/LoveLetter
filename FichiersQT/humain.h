#ifndef HUMAIN_H
#define HUMAIN_H

#include "joueur.h"
#include <QDebug>

class Humain : public Joueur{
public:
    Humain(QString);
    ~Humain();
    void reinitialiser();
    void afficheCartes() const; // juste dans QDebug()
    Carte* choisirCarte(short int nbCartesRestantes, QVector<bool> joueursNonProteger) const;
    short int  choisirJoueur(Carte*, QVector<bool> joueursNonProteger, short int nbCartesRestantes) const;
    void voirCarteDUnJoueur(Carte*, short int joueur);
    short int choisir1DeNos3Cartes() const;
    short int demanderCarteAJoueur(Joueur*, QVector<short int>) const;
};

#endif // HUMAIN_H
