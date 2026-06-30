#include "jeu.h"

Jeu::Jeu(QObject* parent, short int h, short int inul, short int inorm, short int itri)
    :QObject(parent), objectifPoints(0), joueurs(QVector<Joueur*>()), joueurActuel(nullptr), pioche(Paquet()){
    // création des différent type ce joueurs
    for (short int indice = 0; indice < h; indice++)
        joueurs.push_back(new Humain(QString("Joueur ") + QString::number(indice + 1)));
    for (short int indice = 0; indice < inul; indice++)
        joueurs.push_back(new IANul(QString("Joueur ") + QString::number(indice + 1), h+inul+inorm+itri));
    for (short int indice = 0; indice < inorm; indice++)
        joueurs.push_back(new IANormale(QString("Joueur ") + QString::number(indice + 1), h+inul+inorm+itri));
    for (short int indice = 0; indice < itri; indice++)
        joueurs.push_back(new IATriche(QString("Joueur ") + QString::number(indice + 1), joueurs));

    // Détermine le nb de points à faire
    if (joueurs.size() > 4) // 5,6
        objectifPoints = 3;
    else if (joueurs.size() == 4)
        objectifPoints = 4;
    else if (joueurs.size() == 3)
        objectifPoints = 5;
    else if (joueurs.size() == 2)
        objectifPoints = 6;

    // On mélange l'ordre des joueurs
    for (int i = joueurs.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        joueurs.swapItemsAt(i, j); // échange les cartes entre 2 endroit aléatoire
    }
    joueurActuel = joueurs[0]; // On choisis le premier joueur
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
        QVector<Joueur*> joueurEnVie = JoueursRestant();
        if (joueurEnVie.size() > 1){ // si plusieurs joueurs en jeu
            // ici on peut continuer, donc on cherche le joueur suivant:
            short int indiceActuel = 0;

            for (short int indice = 0; indice < joueurEnVie.size(); indice++){
                if (joueurEnVie[indice] == joueurActuel){
                    indiceActuel = indice; // trouvé
                    break;
                }
            }
            if (indiceActuel == joueurEnVie.size() - 1) // si il se trouve à la fin de la liste
                joueurActuel = joueurEnVie[0]; // on revient au début
            else
                joueurActuel = joueurEnVie[indiceActuel + 1];
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
            result.push_back(joueurs[indice]);
    return result;
}

void Jeu::reinitialiserManche(){
    for (short int indice = 0; indice < joueurs.size(); indice++){
        Joueur* joueur = joueurs[indice];
        joueur->reinitialiser();
    }
    pioche.remplir();
}

void Jeu::eliminationJoueur(Joueur* joueur){
    joueur->eliminer();
}

void Jeu::lancerManche(){
    reinitialiserManche();
    lancerTour(); // démarre la première manche, premier tour et s'nchaine à la suite
}

void Jeu::lancerTour(){

}

// ---------------------------slots------------------------------------------- MainWindow -> Jeu

void Jeu::recevoirChoixCarte(short int idCarte){

}

void Jeu::recevoirChoixValeurGarde(short int valeur){

}

void Jeu::recevoirChoixCibleJoueur(short int joueur){

}

void Jeu::rejouer(){

}

