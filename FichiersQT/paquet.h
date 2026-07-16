#ifndef PAQUET_H
#define PAQUET_H

#include "carte.h"
#include <QQueue>
#include <QRandomGenerator>
#include <QDebug>

class Paquet{
    QQueue<Carte*> cartes;
    QVector<short int> cartesJouer;
    QVector<Carte*> listeDesCartes;
public:
    Paquet();
    ~Paquet();
    inline short int avoirNbCartesRestantes() const {return cartes.length();};
    inline short int nbCartesJouer(short int indice) const {return cartesJouer[indice];};
    inline void carteAEtaitJouer(short int indice){cartesJouer[indice]++; /*qDebug() << "VUE carte :" + QString::number(indice);*/};
    inline QVector<short int> avoirCartesJouer() {return cartesJouer;};
    inline QVector<Carte*> avoirListeDesCartes() const {return listeDesCartes;};
    inline Carte* operator[](short int indice) const {return listeDesCartes[indice];};
    Carte* piocher();
    void defausser(Carte*);
    void remplir();
};

#endif // PAQUET_H
