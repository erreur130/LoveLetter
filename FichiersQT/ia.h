#ifndef IA_H
#define IA_H

#include "joueur.h"
#include "carte.h"
#include <QVector>

class IA : public Joueur{
    bool intelligent;
    QVector<QVector<short int>> cartesPotentiellesDesAutres;
    QVector<QVector<short int>> cartesConnuesDesAutres;
public:
    IA(QString, bool, short int);
    ~IA();
    Carte* choisirCarte() const;
    Joueur* choisirJoueur(TypeCarte) const;
    void miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur = nullptr, short int cartePerdent = 0);
};

#endif // IA_H
