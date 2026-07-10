#include "iatriche.h"

IATriche::IATriche(QString nom, QVector<Joueur*> listeJoueurs) : Joueur(nom), joueurs(listeJoueurs){}

IATriche::~IATriche(){

}

Carte* IATriche::choisirCarte(short int nbCartesRestantes, QVector<bool>) const{
    // On considère que le joueur viens de pioché, donc il à 2 cartes (1ère carte est l'ancienne et la deuxème est celle pioché)

    // cas obligatoires ---------------------------------------------------------------------------------- cas obligatoires
    QVector<Carte *> main = avoirMain();
    if (main.at(0) == main.at(1)) // cas où les deux cartes sont les mêmes
        return main.at(0);

    // si princesse (num 9) alors on joue l'autre carte
    if (main.at(0)->avoirNum() == 9)
        return main.at(1);
    else if (main.at(1)->avoirNum() == 9)
        return main.at(0);

    // si contesse (num 8) et soit 5 soit 7 alors on joue la contesse
    if (main.at(0)->avoirNum() == 8 && (main.at(1)->avoirNum() == 7 || main.at(1)->avoirNum() == 5) )
        return main.at(0);
    else if (main.at(1)->avoirNum() == 8 && (main.at(0)->avoirNum() == 7 || main.at(0)->avoirNum() == 5))
        return main.at(1);

    // cas d'algo ---------------------------------------------------------------------------------- cas d'algo

    // cas prioritaire : -----------

    // si garde (num 1) et que des joueurs ne sont pas protégé
    if (main.at(0)->avoirNum() == 1 || main.at(1)->avoirNum() == 1){
        bool possible = false;
        for (Joueur* joueur : joueurs){
            if (not(joueur->estPorteger()) && not(joueur != this)){ // Si non protéger et que ce n'est pas nous
                possible = true;
                break; // pas besoin de continuer
            }
        }
        if (possible && main.at(0)->avoirNum() == 1)
            return main.at(0);
        else if (possible && main.at(1)->avoirNum() == 1)
            return main.at(1);
    }

    // si baron (num 3) et que tu as une plus grosse carte qu'un joueur non protégé
    if (main.at(1)->avoirNum() == 3)
        for (Joueur* joueur : joueurs)
            if (not(joueur->estPorteger()) && main.at(0) > joueur->avoirMain().at(0)) // Si non protéger et en desous de notre carte
                return main.at(1);
    if (main.at(0)->avoirNum() == 3)
        for (Joueur* joueur : joueurs)
            if (not(joueur->estPorteger()) && main.at(1) > joueur->avoirMain().at(0)) // Si non protéger et en desous de notre carte
                return main.at(0);

    // si prince (num 5) et que quelqu'un à la princesse (num 9) et n'est pas protégé alors on le joue (et pas nous)
    if (main.at(1)->avoirNum() == 5 || main.at(0)->avoirNum() == 5){
        bool onConnaisUn9 = false;
        for (Joueur* joueur : joueurs){
            if (joueur->avoirMain().at(0)->avoirNum() == 9 && (not(joueur->estPorteger()) && joueur != this)){ // Si la liste contient la princesse (num 9) et qu'on peut le viser
                onConnaisUn9 = true;
                break; // pas besoin de continuer
            }
        }
        if (onConnaisUn9 && main.at(0)->avoirNum() == 5)
            return main.at(0);
        else if (onConnaisUn9 && main.at(1)->avoirNum() == 5)
            return main.at(1);
    }

    // si prince (num 5) et que on as une carte connue alors on joue le prince (si c'est une grosse carte et que l'ont est à la fin, on la garde)
    if ( (estDecouvert() && main.at(1)->avoirNum() == 5) && (main.at(0)->avoirNum() <= 6 && nbCartesRestantes > joueurs.size()) ){
        return main.at(1);
    }

    // cas semi prioritaire : -----------

    // si espionne (num 0) alors on la joue
    if (main.at(0)->avoirNum() == 0)
        return main.at(0);
    else if (main.at(1)->avoirNum() == 0)
        return main.at(1);

    // si servante (num 4) et que tu as une grosse carte connue alors on joue la servante
    if (main.at(1)->avoirNum() == 4 && (estDecouvert() && main.at(0)->avoirNum() >= 7) ){
        return main.at(1);
    }

    // si 1ère carte est connue par quelqu'un (si c'est une grosse carte et que l'ont est à la fin, on la garde)
    if (estDecouvert() && (main.at(0)->avoirNum() <= 6 && nbCartesRestantes > joueurs.size())){
        return main.at(0);
    }

    // cas non prioritaires : -----------

    // si chancelier (num 6)
    if (main.at(0)->avoirNum() == 6)
        return main.at(0);
    else if (main.at(1)->avoirNum() == 6)
        return main.at(1);

    // si prêtre (num 2)
    if (main.at(0)->avoirNum() == 2)
        return main.at(0);
    else if (main.at(1)->avoirNum() == 2)
        return main.at(1);

    return main.at(0); // au cas où
}

short int IATriche::choisirJoueur(Carte* carte, QVector<bool> joueursNonProteger, short int nbCartesRestantes) const{
    // normellement la fonction peut recevoir les cartes 1,2,3,5,7
    short int numCarte = carte->avoirNum();
    QVector<short int> listeDesjoueurChoisi;


    switch (numCarte) {
    case 1: // si garde (num 1)
        // Regarde personne non protéger
        for (short int indice = 0; indice < joueurs.size(); indice++) // si il ne contient pas de garde
            if (joueurs.at(indice)->avoirMain().at(0)->avoirNum() != 1)
                listeDesjoueurChoisi.push_back(indice);
        break;
    case 3: // si baron (num 3)
        // On cherce ceux qui ont une carte plus petites que la notre
        for (short int indice = 0; indice < joueurs.size(); indice++){
            if (joueurs.at(indice)->avoirMain().at(0)->avoirNum() < avoirMain().at(0)->avoirNum())
                listeDesjoueurChoisi.push_back(indice);
        }
        break;
    case 5: // si prince (num 5)
        // On cherche celui qui à la princesse
        for (short int indice = 0; indice < joueurs.size(); indice++)
            if (joueurs.at(indice)->avoirMain().at(0)->avoirNum() == 9)
                listeDesjoueurChoisi.push_back(indice);

        // On vérifie si on est à découvert et que c'est rentable de retirer sa main
        if ((estDecouvert() && listeDesjoueurChoisi.isEmpty()) && (avoirMain().at(0)->avoirNum() <= 6 && nbCartesRestantes > joueurs.size())) // "cartesConnuesDesAutres.size()" = nb de joueur
            return avoirID(); // On se choisis soit même (pas besoin de continuer)
        break;
    case 2: // si prètre (num 2)
    case 7: // si roi (num 7)
    default:
        // le choix n'est pas important
        break;
    }

    if (listeDesjoueurChoisi.isEmpty()){ // si on arrive toujours pas à trouver, on prend tout le monde
        for (short int indice = 0; indice < joueurs.size(); indice++)
            listeDesjoueurChoisi.push_back(indice);
    }

    // On retire ceux qui sont protégé
    for (short int indice = 0; indice < joueursNonProteger.size(); indice++)
        if (not(joueursNonProteger.at(indice)))
            listeDesjoueurChoisi.removeOne(indice);
    if (numCarte != 5) // On peut se choisir avec le prince
        listeDesjoueurChoisi.removeOne(avoirID()); // On se retire (au cas où)

    if (listeDesjoueurChoisi.isEmpty())
        return -1;

    return listeDesjoueurChoisi.at(QRandomGenerator::global()->bounded(listeDesjoueurChoisi.size()));
}

void IATriche::voirCarteDUnJoueur(Carte*, short int){
    // il le sait déjà
}

short int IATriche::choisir1DeNos3Cartes() const{
    // Ici, le joueur est censé avoir 3 cartes dans la main, mais si le paquet à moins de cartes alors il y aura moins de choix
    // si c'est la princesse, on la garde obligatoirement (règle du maxi le fait pas défaut :)

    // On prend la plus grosse carte si c'est un 7 ou 8 ou 9
    Carte* maxi = nullptr;
    short int idmaxi = -1;

    for (short int indice = 0; indice < avoirMain().size(); indice++)
        if (avoirMain().at(indice)->avoirNum() > idmaxi){
            maxi = avoirMain()[indice];
            idmaxi = indice;
        }
    if (maxi != nullptr && maxi->avoirNum() >= 7)
        return idmaxi;

    // Sinon on prend dans l'ordre de priorité 1, 3, 5, 0, 4, 6, 2 (plus agressif)
    QVector<short int> prio = {1, 3, 5, 0, 4, 6, 2};
    for (short int carteprio : prio){
        // On vérifie pour les 3 cartes ou moins et si c'est bon, on la return
        for (short int indice = 0; indice < avoirMain().size(); indice++)
            if (avoirMain().at(indice)->avoirNum() == carteprio)
                return indice;
    }
    return 0; // au cas où mais cas impossible
}

short int IATriche::demanderCarteAJoueur(Joueur* joueur, QVector<short int>) const{
    short int numCarte = joueur->avoirMain()[0]->avoirNum();
    if (numCarte != 1) // Si ce n'est pas un garde
        return joueur->avoirMain()[0]->avoirNum(); // c'est une victime à ce point
    // Sinon on prend une carte aléatoirement
    while (numCarte != 1){
        numCarte = QRandomGenerator::global()->bounded(10); // 0 à 9
        if (numCarte != 1) // Si il choisit le garde, c'est impossible
            return numCarte;
    }
    return 9; // par défaut mais cas impossible
}