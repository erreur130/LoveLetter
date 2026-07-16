#ifndef IATRICHE_H
#define IATRICHE_H

#include "joueur.h"
#include "carte.h"
#include <QVector>

class IATriche : public Joueur{
    QVector<Joueur*> joueurs;
public:
    IATriche(QString, QVector<Joueur*>);
    ~IATriche();
    void reinitialiser();
    Carte* choisirCarte(short int nbCartesRestantes, QVector<bool> joueursProteger) const;
    short int choisirJoueur(Carte*, QVector<bool> joueursProteger, short int nbCartesRestantes) const;
    void voirCarteDUnJoueur(Carte*, short int joueur);
    short int choisir1DeNos3Cartes() const;
    short int demanderCarteAJoueur(Joueur*, QVector<short int>) const;
};


#endif // IATRICHE_H
