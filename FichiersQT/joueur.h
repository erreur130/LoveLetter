#ifndef JOUEUR_H
#define JOUEUR_H

#include "QVector"

class Carte; // forward declaration #include "carte.h" dans le .cpp
enum class TypeCarte; // forward declaration #include "carte.h" dans le .cpp

class Joueur{
    QString nom;
    short int points;
    bool pointBonusManche;
    bool protection;
    bool enVie;
    QVector<Carte*> main;
public:
    Joueur(QString);
    ~Joueur();
    bool estEnVie() const;
    bool estPorteger() const;
    QVector<Carte*> avoirMain() const;
    QString avoirNom() const;
    short int avoirPoints() const;
    bool avoirPointBonus() const;
    void gainPoints();
    void ajouterPtBonus();
    void retirerPtBonus();
    void ajouterProtection();
    void retirerProtection();
    void reinitialiser();
    void eliminer();
    void jouerCarte(Carte*, Joueur* = nullptr);
    virtual Carte* choisirCarte() const = 0;
    virtual Joueur* choisirJoueur(TypeCarte) const = 0;
};

#endif // JOUEUR_H
