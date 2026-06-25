#ifndef IANORMALE_H
#define IANORMALE_H

#include "joueur.h"
#include "carte.h"
#include <QVector>

class IANormale : public Joueur{
    QVector<QVector<short int>> cartesPotentiellesDesAutres;
    QVector<QVector<short int>> cartesConnuesDesAutres;
public:
    IANormale(QString, short int);
    ~IANormale();
    Carte* choisirCarte(short int nbCartesRestantes, QVector<bool> joueursProteger) const;
    short int  choisirJoueur(Carte*, QVector<bool> joueursProteger) const;
    void miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur = nullptr, short int cartePerdent = 0);
};

#endif // IANORMALE_H
