#include "humain.h"

Humain::Humain(QString nom) : Joueur(nom) {}

void Humain::afficheCartes() const{
    qDebug() << "Main :";
    for(Carte *carte : avoirMain())
        //qDebug() << carte->avoirNum();
        ;
}

Carte* Humain::choisirCarte() const{
    return nullptr; // permet à MainWindow de faire choisir une carte à un utilisateur avec l'ui
}

Joueur* Humain::choisirJoueur(TypeCarte) const{
    return nullptr; // permet à MainWindow de faire choisir une carte à un utilisateur avec l'ui
}