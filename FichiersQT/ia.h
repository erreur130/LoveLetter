#ifndef IA_H
#define IA_H

#include "joueur.h"
#include "QVector"

class IA{
    bool intelligent;
    QVector<QVector<bool>> cartesPotentiellesDesAutres;
public:
    IA(QString, bool);
    ~IA();
    Carte* choisirCarte() const;
    Joueur* choisirJoueur(TypeCarte) const;
};

#endif // IA_H
