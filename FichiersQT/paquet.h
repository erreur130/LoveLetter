#ifndef PAQUET_H
#define PAQUET_H

#include "carte.h"
#include <QQueue>

class Paquet{
    QQueue<Carte*> cartes;
    QVector<short int> cartesJouer;
public:
    Paquet();
    ~Paquet();
    inline short int avoirNbCartesRestantes() const {return cartes.length();};
    inline short int nbCartesJouer(short int index) const {return cartesJouer[index];};
    inline void carteAEtaitJouer(short int index){cartesJouer[index]++;};
    inline QVector<short int> avoirCartesJouer() {return cartesJouer;};
    Carte* piocher();
    void defausser(Carte*);
    void remplir();
};

#endif // PAQUET_H
