#include "joueur.h"
#include "carte.h"

Joueur::Joueur(QString nom) : nom(nom), points(0), id(-1), pointBonusManche(false), protection(false), enVie(true), aDecouvert(false), main(QVector<Carte*>()) {}

Joueur::~Joueur(){
    main.clear();
}

void Joueur::gainPoints(){
    if (pointBonusManche){ // si il a le pt bonnus +1pt
        points++;
        pointBonusManche = false;
        qDebug() << "PT Bonnus pour : " << avoirNom();
    }
    if (estEnVie()){ // et si il est en vie +1pt
        points++;
        qDebug() << "PT norm pour : " << avoirNom();
    }
}

void Joueur::reinitialiser(){
    pointBonusManche = false;
    protection = false;
    enVie = true;
    aDecouvert = false;
    main.clear();
}

QString Joueur::jouerCarte(Carte* carte, Joueur* autreJoueur, Carte* cartechoix){   
    TypeCarte type = carte->estType();

    // Action dif suivant le type de la carte
    switch (type) {
    case TypeCarte::Offensif: // sur autre personne et le choix d'une carte (num : 1)
        return avoirNom() + carte->action(autreJoueur, nullptr, cartechoix);
    case TypeCarte::Defensif: // sur soi (num : 0,4,9) le 9 ne protège pas vraiment ;) mais ce joue sur soie même
        return avoirNom() + carte->action(this);
    case TypeCarte::OffensifOuDefensif: // sur une autres personne et soi même (num 5)
        return avoirNom() + carte->action(autreJoueur);
    case TypeCarte::Duel: // sur une autres personne et soi même (num : 2,3,7) / num 2 car la personne qui la joue regarde l'autre
        return avoirNom() + carte->action(this, autreJoueur);
    case TypeCarte::SansEffet: // ne fait rien (num : 8)
        return avoirNom() + carte->action();
    }
    return "Aucune carte n'a était touver pour l'action de " + avoirNom(); // Message d'erreur
}

void Joueur::miseAJourCartesPotentiel(QVector<short int>, Joueur*, short int, Joueur*, short int){
    // par défaut ne fait rien
}

bool Joueur::carteEstPossible(short int numcarte, short int nbExemplaires, short int carteJouer) const {
    return ((carteJouer + (avoirMain().at(0)->avoirNum() == numcarte)) < nbExemplaires); // on aditionne les cartes jouer avec le bool de si on à la carte
}