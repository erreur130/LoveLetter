#include "joueur.h"
#include "carte.h"
#include "mainwindow.h"

short int Joueur::idGlobal = 0;

Joueur::Joueur(QString nom) : nom(nom), points(0), id(idGlobal++), pointBonusManche(false), protection(false), enVie(true), aDecouvert(false), main(QVector<Carte*>()) {}

Joueur::~Joueur(){}

void Joueur::gainPoints(){
    if (pointBonusManche){
        points += 2;
        pointBonusManche = false;
    } else
        points++;
}

void Joueur::reinitialiser(){
    pointBonusManche = false;
    protection = false;
    enVie = true;
    aDecouvert = false;
    main.clear();
}

QString Joueur::jouerCarte(Carte* carte, Joueur* autreJoueur, Carte* cartechoix, MainWindow* fenetre){
    TypeCarte type = carte->estType();

    // Action dif suivant le type de la carte
    switch (type) {
    case TypeCarte::Offensif: // sur autre personne et le choix d'une carte (num : 1)
        return avoirNom() + carte->action(autreJoueur, nullptr, cartechoix);
    case TypeCarte::Defensif: // sur soi (num : 0,4,6,9) le 9 ne protège pas vraiment ;) mais ce joue sur soie même
        return avoirNom() + carte->action(this);
    case TypeCarte::DefensifBis:
        return avoirNom() + carte->action(this, nullptr, nullptr, fenetre);
    case TypeCarte::OffensifOuDefensif: // sur une autres personne et soi même (num 5)
        return avoirNom() + carte->action(autreJoueur);
    case TypeCarte::Duel: // sur une autres personne et soi même (num : 2,3,7) / num 2 car la personne qui la joue regarde l'autre
        return avoirNom() + carte->action(this, autreJoueur);
    case TypeCarte::SansEffet:
    default: // SansEffet
        return avoirNom() + carte->action();
    }
    return "Aucune carte n'a était touver pour l'action de " + avoirNom(); // Message d'erreur
}

void Joueur::miseAJourCartesPotentiel(QVector<short int>, Joueur*, short int, Joueur*, short int){
    // par défaut ne fait rien
}