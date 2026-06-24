#ifndef JOUEUR_H
#define JOUEUR_H

#include <QVector>

class Carte; // forward declaration #include "carte.h" dans le .cpp
enum class TypeCarte; // forward declaration #include "carte.h" dans le .cpp

class Joueur{
    QString nom;
    short int points;
    short int id;
    bool pointBonusManche;
    bool protection;
    bool enVie;
    QVector<Carte*> main;
    static short int idGlobal;
public:
    Joueur(QString);
    virtual ~Joueur();
    inline bool estEnVie() const {return enVie;};
    inline bool estPorteger() const {return protection;};
    inline const QVector<Carte*>& avoirMain() const {return main;};
    inline const QString& avoirNom() const {return nom;};
    inline short int avoirPoints() const {return points;};
    inline short int avoirID() const {return id;};
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
    virtual void miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur = nullptr, short int cartePerdent = 0); // les deux autres paramètre sont nésésaire en cas de baron (num 3)
};

#endif // JOUEUR_H
