#ifndef IANUL_H
#define IANUL_H

#include "joueur.h"
#include "carte.h"
#include <QVector>

class IANul : public Joueur{
    QVector<QVector<short int>> cartesConnuesDesAutres;
public:
    IANul(QString, short int);
    ~IANul();
    Carte* choisirCarte(short int nbCartesRestantes, QVector<bool> joueursProteger) const;
    short int  choisirJoueur(Carte*, QVector<bool> joueursProteger) const;
    void miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur = nullptr, short int cartePerdent = 0);
};

#endif // IANUL_H
