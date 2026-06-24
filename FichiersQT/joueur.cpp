#include "joueur.h"
#include "carte.h"

short int Joueur::idGlobal = 0;

Joueur::Joueur(QString nom) : nom(nom), points(0), id(idGlobal++), pointBonusManche(0), protection(0), enVie(0), main(QVector<Carte*>()) {}

Joueur::~Joueur(){}

void Joueur::gainPoints(){

}

void Joueur::reinitialiser(){

}

void Joueur::jouerCarte(Carte*, Joueur*){

}

void Joueur::miseAJourCartesPotentiel(QVector<short int>, Joueur*, short int, Joueur*, short int){
    // par défaut ne fait rien
}