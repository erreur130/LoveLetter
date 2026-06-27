#ifndef CARTE_H
#define CARTE_H

#include <QString>
class Paquet; // forward declaration, pour ne pas faire de récursivité avec une #include "paquet.h", il est présent dans le .cpp

class Joueur; // forward declaration #include "joueur.h" dans .cpp

enum class TypeCarte{
    Offensif,
    Defensif,
    OffensifOuDefensif,
    Duel,
    SansEffet
};

class Carte{
    short int nbExemplaires;
    short int num;
    QString nom;
    QString image;
    TypeCarte type;
public:
    Carte(short int, short int, QString, QString, TypeCarte);
    ~Carte();
    inline TypeCarte estType() const {return type;};
    inline short int avoirNum() const {return num;};
    inline short int avoirNbExemplaires() const {return nbExemplaires;};
    inline QString avoirNom() const {return nom;};
    inline QString avoirImage() const {return image;};
    virtual QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const = 0;
};

class Carte0 : public Carte{
public :
    Carte0(short int, short int, QString, QString, TypeCarte);
    ~Carte0();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
};

class Carte1 : public Carte{
public :
    Carte1(short int, short int, QString, QString, TypeCarte);
    ~Carte1();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
};

class Carte2 : public Carte{
public :
    Carte2(short int, short int, QString, QString, TypeCarte);
    ~Carte2();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
};

class Carte3 : public Carte{
public :
    Carte3(short int, short int, QString, QString, TypeCarte);
    ~Carte3();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
};

class Carte4 : public Carte{
public :
    Carte4(short int, short int, QString, QString, TypeCarte);
    ~Carte4();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
};

class Carte5 : public Carte{
    Paquet* pioche;
public :
    Carte5(short int, short int, QString, QString, TypeCarte, Paquet* pioche_);
    ~Carte5();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
    inline Paquet* avoirPioche() const {return pioche;};
};

class Carte6 : public Carte{
    Paquet* pioche;
public :
    Carte6(short int, short int, QString, QString, TypeCarte, Paquet* pioche_);
    ~Carte6();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
    inline Paquet* avoirPioche() const {return pioche;};

};

class Carte7 : public Carte{
public :
    Carte7(short int, short int, QString, QString, TypeCarte);
    ~Carte7();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
};

class Carte8 : public Carte{
public :
    Carte8(short int, short int, QString, QString, TypeCarte);
    ~Carte8();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
};

class Carte9 : public Carte{
public :
    Carte9(short int, short int, QString, QString, TypeCarte);
    ~Carte9();
    QString action(Joueur* = nullptr, Joueur* = nullptr, Carte* = nullptr) const;
};

#endif // CARTE_H
