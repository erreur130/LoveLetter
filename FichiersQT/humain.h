#ifndef HUMAIN_H
#define HUMAIN_H

#include "joueur.h"
#include <QDebug>

class Humain : public Joueur{
    Carte* carteVue;
public:
    Humain(QString);
    ~Humain();
    void afficheCartes() const; // juste dans QDebug()
    Carte* choisirCarte(short int nbCartesRestantes, QVector<bool> joueursProteger) const;
    short int  choisirJoueur(Carte*, QVector<bool> joueursProteger) const;
    void voirCarteDUnJoueur(Carte*, short int joueur);
    inline Carte* avoirCarteVue() const {return carteVue;};
    short int choisir1DeNos3Cartes() const;
    inline void retirerCarteVue() {carteVue = nullptr;};
};

#endif // HUMAIN_H
