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
    inline bool estEnVie() const {return enVie;};
    inline bool estPorteger() const {return protection;};
    inline QVector<Carte*> avoirMain() const {return main;};
    inline QString avoirNom() const {return nom;};
    inline short int avoirPoints() const {return points;};
    inline bool avoirPointBonus() const {return pointBonusManche;};
    void gainPoints();
    inline void ajouterPtBonus() {pointBonusManche=true;};
    inline void retirerPtBonus() {pointBonusManche=false;};
    inline void ajouterProtection() {protection=true;};
    inline void retirerProtection() {protection=false;};
    void reinitialiser();
    inline void eliminer() {protection=false;};
    void jouerCarte(Carte*, Joueur* = nullptr);
    virtual Carte* choisirCarte() const = 0;
    virtual Joueur* choisirJoueur(TypeCarte) const = 0;
};

#endif // JOUEUR_H
