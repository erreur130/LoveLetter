#ifndef HUMAIN_H
#define HUMAIN_H

#include "joueur.h"
#include <QDebug>

class Humain : public Joueur{
public:
    Humain(QString);
    ~Humain();
    void afficheCartes() const; // juste dans QDebug()
    Carte* choisirCarte(short int nbCartesRestantes, QVector<bool> joueursProteger) const;
    short int  choisirJoueur(Carte*, QVector<bool> joueursProteger) const;
};

#endif // HUMAIN_H
