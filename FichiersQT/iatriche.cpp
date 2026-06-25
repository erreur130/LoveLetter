#include "iatriche.h"

IATriche::IATriche(QString nom, QVector<Joueur*> listeJoueurs) : Joueur(nom), joueurs(listeJoueurs){}

IATriche::~IATriche(){

}

Carte* IATriche::choisirCarte(short int nbCartesRestantes, QVector<bool> joueursProteger) const{

}

short int  IATriche::choisirJoueur(Carte* carte, QVector<bool> joueursProteger) const{

}
