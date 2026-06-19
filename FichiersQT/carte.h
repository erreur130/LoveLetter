#ifndef CARTE_H
#define CARTE_H

#include "QString"

class Joueur; // forward declaration #include "joueur.h" dans .cpp

enum class TypeCarte{
    Offensif,
    Defensif,
    Tous,
    Duel
};

class Carte{
    short int nbExemplaires;
    short int num;
    QString nom;
    QString description;
    TypeCarte type;
public:
    Carte(short int, short int, QString, QString, TypeCarte);
    ~Carte();
    TypeCarte estType() const;
    short int avoirNum() const;
    virtual void action(Joueur* = nullptr, Joueur* = nullptr) const = 0;
};

class Carte0 : public Carte{
    Carte0(short int, short int, QString, QString, TypeCarte);
    ~Carte0();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte1 : public Carte{
    Carte1(short int, short int, QString, QString, TypeCarte);
    ~Carte1();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte2 : public Carte{
    Carte2(short int, short int, QString, QString, TypeCarte);
    ~Carte2();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte3 : public Carte{
    Carte3(short int, short int, QString, QString, TypeCarte);
    ~Carte3();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte4 : public Carte{
    Carte4(short int, short int, QString, QString, TypeCarte);
    ~Carte4();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte5 : public Carte{
    Carte5(short int, short int, QString, QString, TypeCarte);
    ~Carte5();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte6 : public Carte{
    Carte6(short int, short int, QString, QString, TypeCarte);
    ~Carte6();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte7 : public Carte{
    Carte7(short int, short int, QString, QString, TypeCarte);
    ~Carte7();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte8 : public Carte{
    Carte8(short int, short int, QString, QString, TypeCarte);
    ~Carte8();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

class Carte9 : public Carte{
    Carte9(short int, short int, QString, QString, TypeCarte);
    ~Carte9();
    void action(Joueur* = nullptr, Joueur* = nullptr) const;
};

#endif // CARTE_H
