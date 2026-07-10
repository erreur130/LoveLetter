#ifndef JOUEUR_H
#define JOUEUR_H

#include <QVector>
#include <QRandomGenerator>
#include <QMetaType>
#include <QDebug>

class Carte; // forward declaration #include "carte.h" dans le .cpp
enum class TypeCarte; // forward declaration #include "carte.h" dans le .cpp

class Joueur{
    QString nom;
    short int points;
    short int id;
    bool pointBonusManche;
    bool protection;
    bool enVie;
    bool aDecouvert;
    QVector<Carte*> main;
public:
    Joueur(QString);
    virtual ~Joueur();
    inline bool estEnVie() const {return enVie;};
    inline bool estPorteger() const {return protection;};
    inline bool estDecouvert() const {return aDecouvert;};
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
    inline void ajouterDecouvert() {aDecouvert=true;};
    inline void retirerDecouvert() {aDecouvert=false;};
    inline void changerID(short int id_) {id = id_;};
    void reinitialiser();
    inline void retirerPoints() {points = 0;};
    inline void eliminer() {enVie=false;};
    inline void ajouterCarte(Carte* carte) {main.append(carte);};
    inline Carte* retirerCarte(short int indice) {return main.takeAt(indice);};
    QString jouerCarte(Carte*, Joueur* = nullptr, Carte* = nullptr);
    bool carteEstPossible(short int numcarte, short int nbExemplaires, short int carteJouer)  const;
    virtual Carte* choisirCarte(short int nbCartesRestantes, QVector<bool> joueursNonProteger) const = 0;
    virtual short int choisirJoueur(Carte*, QVector<bool> joueursNonProteger, short int nbCartesRestantes) const = 0;
    virtual void miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur = nullptr, short int cartePerdent = 0); // les deux autres paramètre sont nésésaire en cas de baron (num 3)
    virtual void voirCarteDUnJoueur(Carte*, short int joueur) = 0;
    virtual short int choisir1DeNos3Cartes() const = 0;
    virtual short int demanderCarteAJoueur(Joueur*, QVector<short int>) const = 0;
};

Q_DECLARE_METATYPE(Joueur*)

#endif // JOUEUR_H
