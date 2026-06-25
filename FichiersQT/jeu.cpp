#include "jeu.h"

Jeu::Jeu(short int h, short int inul, short int inorm, short int itri)
    :objectifPoints(0), joueurs(QVector<Joueur*>()), joueurActuel(nullptr), pioche(Paquet()){
    // création des différent type ce joueurs
    for (short int indice = 0; indice < h; indice++)
        joueurs.push_back(new Humain(QString("Joueur ") + QString::number(indice + 1)));
    for (short int indice = 0; indice < inul; indice++)
        joueurs.push_back(new IANul(QString("Joueur ") + QString::number(indice + 1), h+inul+inorm+itri));
    for (short int indice = 0; indice < inorm; indice++)
        joueurs.push_back(new IANormale(QString("Joueur ") + QString::number(indice + 1), h+inul+inorm+itri));
    for (short int indice = 0; indice < itri; indice++)
        joueurs.push_back(new IATriche(QString("Joueur ") + QString::number(indice + 1), joueurs));
}

Jeu::~Jeu(){
    for (short int indice = 0; indice < joueurs.size(); indice++)
        delete joueurs[indice];
}

QVector<Joueur*> Jeu::JoueursRestant() const{
    QVector<Joueur*> result;
    for (short int indice = 0; indice < joueurs.size(); indice++)
        if (joueurs[indice]->estEnVie() == true)
            result.push_back(joueurs[indice]);
    return result;
}

bool Jeu::tourSuivant(){
    if (pioche.avoirNbCartesRestantes() > 0){ // si plus de carte
        short int survivant = 0;
        for (short int indice = 0; indice < joueurs.size(); indice++)
            if (joueurs[indice]->estEnVie() >= true)
                survivant++;
        if (survivant > 1){ // si plusieurs joueurs en jeu
            // ici on peut continuer, donc on cherche le joueur suivant:
            short int indiceActuel = 0;
            for (short int indice = 0; indice < joueurs.size(); indice++){
                if (joueurs[indice] == joueurActuel){
                    indiceActuel = indice; // trouvé
                    break;
                }
            }
            if (indiceActuel == joueurs.size() - 1) // si il se trouve à la fin de la liste
                joueurActuel = joueurs[0]; // on revient au début
            else
                joueurActuel = joueurs[indiceActuel];

            return true;
        }
    } else {
        return false;
    }
    return false;
}

void Jeu::miseAJourCartesPotentiel(Carte* carteJouer, Joueur* autreJoueur, Carte* cartePerdent){
    // on cherche l'indice du joueur actuel :
    short int indiceActuel = 0;
    for (short int indice = 0; indice < joueurs.size(); indice++){
        if (joueurs[indice] == joueurActuel){
            indiceActuel = indice; // trouvé
            break;
        }
    }
    short int numCarte = carteJouer->avoirNum(); // num de la carte qui vient d'être joué

    short int numCartePerdent = 0;
    if (cartePerdent != nullptr)
        numCartePerdent = cartePerdent->avoirNum();

    for (short int indice = 0; indice < joueurs.size(); indice++){
        if (indice != indiceActuel && joueurs[indice]->estEnVie() == true)
            joueurs[indice]->miseAJourCartesPotentiel(pioche.avoirCartesJouer(), joueurActuel, numCarte, autreJoueur, numCartePerdent);
    }
}

QVector<Joueur*> Jeu::verifSiGagnants() const{
    // recherche du max au dessus de objectifPoints
    short int maxi = 0;
    for (short int indice = 0; indice < joueurs.size(); indice++)
        if (joueurs[indice]->avoirPoints() >= objectifPoints)
            if (joueurs[indice]->avoirPoints() > maxi)
                maxi = joueurs[indice]->avoirPoints();
    // tri au max
    QVector<Joueur*> result;
    for (short int indice = 0; indice < joueurs.size(); indice++)
        if (joueurs[indice]->avoirPoints() >= maxi)
            result.append(joueurs[indice]);
    return result;
}

void Jeu::reinitialiserManche(){
    for (short int indice = 0; indice < joueurs.size(); indice++){
        Joueur* joueur = joueurs[indice];
        joueur->reinitialiser();
    }

}

void Jeu::eliminationJoueur(Joueur* joueur){
    joueur->eliminer();
}

void Jeu::lancer(){

}

void tourDeJeu(){

}

