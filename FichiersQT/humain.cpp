#include "humain.h"

Humain::Humain(QString nom) : Joueur(nom) {}

Humain::~Humain() {}

void Humain::afficheCartes() const{
    qDebug() << "Main :";
    /*
    for(Carte *carte : avoirMain())
        qDebug() << carte->avoirNum();
    */
}

Carte* Humain::choisirCarte(short int, QVector<bool>) const{
    return nullptr; // permet à MainWindow de faire choisir une carte à un utilisateur avec l'ui
}

short int Humain::choisirJoueur(Carte*, QVector<bool>, short int) const{
    return -1; // permet à MainWindow de faire choisir une carte à un utilisateur avec l'ui
}

void Humain::voirCarteDUnJoueur(Carte*, short int){
}

short int Humain::choisir1DeNos3Cartes() const{
    return -1; // permet à MainWindow de faire choisir une carte à un utilisateur avec l'ui
}

short int Humain::demanderCarteAJoueur(Joueur*, QVector<short int>) const{
    return -1; // permet à MainWindow de faire choisir une carte à un utilisateur avec l'ui
}