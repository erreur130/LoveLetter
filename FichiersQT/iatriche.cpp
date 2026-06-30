#include "iatriche.h"

IATriche::IATriche(QString nom, QVector<Joueur*> listeJoueurs) : Joueur(nom), joueurs(listeJoueurs){}

IATriche::~IATriche(){

}

Carte* IATriche::choisirCarte(short int nbCartesRestantes, QVector<bool> joueursNonProteger) const{

}

short int  IATriche::choisirJoueur(Carte* carte, QVector<bool> joueursNonProteger) const{

}

void IATriche::voirCarteDUnJoueur(Carte* carte, short int joueur){

}

short int IATriche::choisir1DeNos3Cartes() const{

}

short int IATriche::demanderCarteAJoueur(Joueur*, QVector<short int> cartesJouer) const{

}