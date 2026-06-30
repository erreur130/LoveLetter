#include "ianul.h"

IANul::IANul(QString nom, short int nbJoueurs):
    Joueur(nom), cartesConnuesDesAutres(nbJoueurs, QVector<short int>()){}

IANul::~IANul(){

}

Carte* IANul::choisirCarte(short int nbCartesRestantes, QVector<bool> joueursNonProteger) const{

}

short int  IANul::choisirJoueur(Carte* carte, QVector<bool> joueursNonProteger) const{

}

void IANul::miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur, short int cartePerdent){

}

void IANul::voirCarteDUnJoueur(Carte* carte, short int joueur){

}

short int IANul::choisir1DeNos3Cartes() const{

}

short int IANul::demanderCarteAJoueur(Joueur*, QVector<short int> cartesJouer) const{

}