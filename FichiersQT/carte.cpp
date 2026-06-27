#include "carte.h"
#include "joueur.h"
#include "paquet.h"

// ---------------------------- Carte ----------------------------

Carte::Carte(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : nbExemplaires(nbExemplaires_), num(num_), nom(nom_), image(image_), type(type_) {}

Carte::~Carte(){}

// ---------------------------- num 0 ----------------------------

Carte0::Carte0(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_) {}

Carte0::~Carte0() {}

QString Carte0::action(Joueur* joueur1, Joueur*, Carte*) const {
    joueur1->ajouterPtBonus();
    return " à jouer une espionne [0]";
}

// ---------------------------- num 1 ----------------------------

Carte1::Carte1(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_) {}

Carte1::~Carte1() {}

QString Carte1::action(Joueur* joueur1, Joueur*, Carte* carte) const {
    QString message = " à jouer un garde [1] contre " + joueur1->avoirNom() + " et pense que sa carte est : " + carte->avoirNom();

    // On verifie si il à bien deviné
    if (carte->avoirNum() == joueur1->avoirMain().at(0)->avoirNum()){
        joueur1->eliminer();
        return message + "\n Ce qui est vrais ! " + joueur1->avoirNom() + " Eliminé !";
    }
    return message + "\n Ce qui est faux ...";
}

// ---------------------------- num 2 ----------------------------

Carte2::Carte2(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_) {}

Carte2::~Carte2() {}

QString Carte2::action(Joueur* joueur1, Joueur* joueur2, Carte*) const {
    joueur1->voirCarteDUnJoueur(joueur2->avoirMain()[0], joueur2->avoirID());
    return " à jouer un prêtre pour voir la carte de " + joueur2->avoirNom();
}

// ---------------------------- num 3 ----------------------------

Carte3::Carte3(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_) {}

Carte3::~Carte3() {}

QString Carte3::action(Joueur* joueur1, Joueur* joueur2, Carte*) const {
    QString message = " à jouer un baron [3] et défit " + joueur2->avoirNom();
    if (joueur1->avoirMain().at(0) < joueur2->avoirMain().at(0)){
        joueur1->eliminer();
        return message + "\n " + joueur1->avoirNom() + " est éliminé.";
    }else if (joueur1->avoirMain().at(0) > joueur2->avoirMain().at(0)){
        joueur2->eliminer();
        return message + "\n " + joueur2->avoirNom() + " est éliminé.";
    }
    // Sinon rien
    return  + "\n Personne n'a été éliminé.";
}

// ---------------------------- num 4 ----------------------------

Carte4::Carte4(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_) {}

Carte4::~Carte4() {}

QString Carte4::action(Joueur* joueur1, Joueur*, Carte*) const {
    joueur1->ajouterProtection();
    return " à jouer la servante [4], il est maintenant protégé jusqu'au prochain tour.";
}

// ---------------------------- num 5 ----------------------------

Carte5::Carte5(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_, Paquet* pioche_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_), pioche(pioche_) {}

Carte5::~Carte5() {}

QString Carte5::action(Joueur* joueur1, Joueur*, Carte*) const {
    /* // pas encore implanté defausser() et piocher() ------------------------------------------------------------------------------------------------------------------
    pioche->defausser(joueur1->retirerCarte(0)); // la carte du joueur est mise sous la pioche
    joueur1->ajouterCarte(pioche->piocher()); // puis le joueur récupère la carte qui était sur la pioche
    */
    return " à jouer le pince [5] sur " + joueur1->avoirNom() + ", celui-ci défausse et reprend une carte.";
}

// ---------------------------- num 6 ----------------------------

Carte6::Carte6(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_, Paquet* pioche_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_), pioche(pioche_) {}
Carte6::~Carte6() {}

QString Carte6::action(Joueur* joueur1, Joueur*, Carte* carte) const {
    // il pioche 2 cartes
    /* // pas encore implanté piocher() ------------------------------------------------------------------------------------------------------------------
    joueur1->ajouterCarte(pioche->piocher());
    joueur1->ajouterCarte(pioche->piocher());
    */
    // il choisis la quelle gardé
    short int carteAGarder = joueur1->choisir1DeNos3Cartes();
    //------------------------------------------------------------ cas du joueur = Humain ? ------------------------------------------------------------------------------
    for (short int indice = 0; indice < joueur1->avoirMain().size(); indice++)
        /* // pas encore implanté defausser() ------------------------------------------------------------------------------------------------------------------
        if (joueur1->avoirMain().at(indice)->avoirNum() != carteAGarder) // Si ce n'est pas la carte que l'on a choisit alors on la retire
            pioche->defausser(joueur1->retirerCarte(indice));
        */;
    return " à jouer le chancelier [6].";
}

// ---------------------------- num 7 ----------------------------

Carte7::Carte7(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_) {}

Carte7::~Carte7() {}

QString Carte7::action(Joueur* joueur1, Joueur* joueur2, Carte*) const {
    // On échange les mains (les 1 carte des joueurs)
    Carte* carteTmp = joueur1->avoirMain()[0];
    joueur1->retirerCarte(0);
    joueur1->ajouterCarte(joueur2->avoirMain()[0]);
    joueur2->ajouterCarte(carteTmp);
    // Donc chaqu'un connais la carte de l'autre
    joueur1->voirCarteDUnJoueur(joueur2->avoirMain()[0], joueur2->avoirID());
    joueur2->voirCarteDUnJoueur(carteTmp, joueur1->avoirID());
    return " à jouer le roi [7] et choisit " + joueur2->avoirNom() + "pour échanger leurs mains.";
}

// ---------------------------- num 8 ----------------------------

Carte8::Carte8(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_) {}

Carte8::~Carte8() {}

QString Carte8::action(Joueur*, Joueur*, Carte*) const {
    // ne fait rien
    return " à jouer la comtesse [8].";
}

// ---------------------------- num 9 ----------------------------

Carte9::Carte9(short int nbExemplaires_, short int num_, QString nom_, QString image_, TypeCarte type_)
    : Carte(nbExemplaires_, num_, nom_, image_,  type_) {}

Carte9::~Carte9() {}

QString Carte9::action(Joueur* joueur1, Joueur*, Carte*) const {
    // tue le joueur
    joueur1->eliminer();
    return " à jouer la princesse [9] et en meurt. Note du dev : Pourquoi tu fais ça :(";
}