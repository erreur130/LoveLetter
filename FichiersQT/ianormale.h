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
    void reinitialiser();
    Carte* choisirCarte(short int nbCartesRestantes, QVector<bool> joueursNonProteger) const;
    short int choisirJoueur(Carte*, QVector<bool> joueursNonProteger, short int nbCartesRestantes) const;
    void miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur = nullptr, short int cartePerdent = 0);
    void voirCarteDUnJoueur(Carte*, short int joueur);
    short int choisir1DeNos3Cartes() const;
    short int demanderCarteAJoueur(Joueur*, QVector<short int>) const;
};

#endif // IANORMALE_H
