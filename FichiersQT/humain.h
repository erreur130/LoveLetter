#ifndef HUMAIN_H
#define HUMAIN_H

#include "joueur.h"

class Humain{
public:
    Humain(QString);
    ~Humain();
    void afficheCartes() const; // juste dans QDebug()
    Carte* choisirCarte() const;
    Joueur* choisirJoueur(TypeCarte) const;
};

#endif // HUMAIN_H
