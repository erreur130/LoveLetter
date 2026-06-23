#include "jeu.h"

Jeu::Jeu(short int h, short int i, short int ii)
    :objectifPoints(0), joueurs(QVector<Joueur*>()), joueurActuel(nullptr), pioche(Paquet()){
    // création joueur
    for (short int indice = 0; indice < h; indice++)
        joueurs.push_back(new Humain(QString("Joueur ") + QString::number(indice + 1)));
    for (short int indice = 0; indice < i; indice++)
        joueurs.push_back(new IA(QString("Joueur ") + QString::number(indice + 1), 0));
    for (short int indice = 0; indice < ii; indice++)
        joueurs.push_back(new IA(QString("Joueur ") + QString::number(indice + 1), 1));
}

Jeu::~Jeu(){

}
/*
void Jeu::afficheJoueursRestant() const{

}

void Jeu::tourSuivant(){

}

void Jeu::miseAJourCartesPotentiel(Carte*){

}

bool Jeu::aGagnerLeJeu(Joueur*) const{

}

Joueur* Jeu::verifSiGagnant() const{

}

void Jeu::reinitialiserManche(){

}

void Jeu::eliminationJoueur(Joueur*){

}

void Jeu::lancer(){

}

void tourDeJeu(){

}
*/
