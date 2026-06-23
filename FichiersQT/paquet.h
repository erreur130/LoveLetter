#ifndef PAQUET_H
#define PAQUET_H

#include "carte.h"
#include "QQueue"

class Paquet{
    QQueue<Carte*> cartes;
public:
    Paquet();
    ~Paquet();
    inline short int avoirNbCartesRestantes() const {return cartes.length();};
    Carte* piocher();
    void defausser(Carte*);
    void remplir();
};

#endif // PAQUET_H
