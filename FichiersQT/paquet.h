#ifndef PAQUET_H
#define PAQUET_H

#include "carte.h"
#include "QQueue"

class Paquet{
    short int nbCartesRestantes;
    QQueue<Carte*> cartes;
public:
    Paquet();
    ~Paquet();
    short int avoirNbCartesRestantes() const;
    Carte* piocher();
    void defausser(Carte*);
    void remplir();
};

#endif // PAQUET_H
