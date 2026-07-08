#include "ianormale.h"

IANormale::IANormale(QString nom, short int nbJoueurs) :
    Joueur(nom), cartesPotentiellesDesAutres(nbJoueurs, QVector<short int>()), cartesConnuesDesAutres(nbJoueurs, QVector<short int>()){}

IANormale::~IANormale() {}

Carte* IANormale::choisirCarte(short int nbCartesRestantes, QVector<bool> joueursNonProteger) const{
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
        return main.at(1);
    else if (main.at(1)->avoirNum() == 8 && (main.at(0)->avoirNum() == 7 || main.at(0)->avoirNum() == 5))
        return main.at(0);

    // cas d'algo ---------------------------------------------------------------------------------- cas d'algo
    // cas prioritaires : -----------

    // si servante (num 4) et que tu as une grosse carte connue alors on joue la servante
    if (main.at(1)->avoirNum() == 4 && (estDecouvert() && main.at(0)->avoirNum() >= 7) ){
        return main.at(1);
    }

    // si 1ère carte est connue par quelqu'un (si c'est une grosse carte et que l'ont est à la fin, on la garde)
    if (estDecouvert() && (main.at(0)->avoirNum() <= 6 && nbCartesRestantes > cartesConnuesDesAutres.size())){ // "cartesConnuesDesAutres.size()" = nb de joueur
        return main.at(0);
    }

    // si espionne (num 0) alors on la joue
    if (main.at(0)->avoirNum() == 0)
        return main.at(1);
    else if (main.at(1)->avoirNum() == 0)
        return main.at(0);

    // si prince (num 5) et que on as une carte connue alors on joue le prince (si c'est une grosse carte et que l'ont est à la fin, on la garde)
    if ( (estDecouvert() && main.at(1)->avoirNum() == 5) && (main.at(0)->avoirNum() <= 6 && nbCartesRestantes > cartesConnuesDesAutres.size()) ){ // "cartesConnuesDesAutres.size()" = nb de joueur
        return main.at(5);
    }

    // si garde (num 1) et que l'on connais (sûr) une carte d'un autre alors on le joue
    if (main.at(0)->avoirNum() == 1 || main.at(1)->avoirNum() == 1){
        bool onConnais = false;
        for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++){
            if (not(cartesConnuesDesAutres[indice].isEmpty()) && joueursNonProteger[indice]){ // Si la liste contient quelque chause et qu'on peut le viser
                onConnais = true;
                break; // pas besoin de continuer
            }
        }
        if (onConnais && main.at(0)->avoirNum() == 1)
            return main.at(1);
        else if (onConnais && main.at(1)->avoirNum() == 1)
            return main.at(0);
    }

    // si baron (num 3) et que tu as une grosse carte (num 6 que au début) alors on joue le barron
    if (main.at(1)->avoirNum() == 3 && main.at(0)->avoirNum() >= 6){
        if (main.at(0)->avoirNum() > 6 || /*num 6 que si ->*/ nbCartesRestantes > 21/2)
            return main.at(1);
    } else if(main.at(0)->avoirNum() == 3 && main.at(1)->avoirNum() >= 6){
        if (main.at(1)->avoirNum() > 6 || /*num 6 que si ->*/ nbCartesRestantes > 21/2)
            return main.at(0);
    }

    // si prince (num 5) et que l'on connais (sûr) que quelqu'un à la princesse (num 9) alors on le joue
    if (main.at(1)->avoirNum() == 5 || main.at(0)->avoirNum() == 5){
        bool onConnaisUn9 = false;
        for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++){
            if (cartesConnuesDesAutres[indice].contains(9) && joueursNonProteger[indice]){ // Si la liste contient la princesse (num 9) et qu'on peut le viser
                onConnaisUn9 = true;
                break; // pas besoin de continuer
            }
        }
        if (onConnaisUn9 && main.at(0)->avoirNum() == 5)
            return main.at(1);
        else if (onConnaisUn9 && main.at(1)->avoirNum() == 5)
            return main.at(0);
    }

    // cas semi prioritaires : -----------

    // si chancelier (num 6)
    if (main.at(0)->avoirNum() == 6)
        return main.at(1);
    else if (main.at(1)->avoirNum() == 6)
        return main.at(0);

    // si prêtre (num 2)
    if (main.at(0)->avoirNum() == 2)
        return main.at(1);
    else if (main.at(1)->avoirNum() == 2)
        return main.at(0);

    // cas non prioritaires : -----------

    // si garde (num 1) ici, c'est un tir au pif (avec les supositions)
    if (main.at(0)->avoirNum() == 1)
        return main.at(1);
    else if (main.at(1)->avoirNum() == 1)
        return main.at(0);

    return main.at(0); // au cas où
}

short int  IANormale::choisirJoueur(Carte* carte, QVector<bool> joueursNonProteger, short int nbCartesRestantes) const{
    // normellement la fonction peut recevoir les cartes 1,2,3,5,7
    short int numCarte = carte->avoirNum();
    QVector<short int> listeDesjoueurChoisi;


    switch (numCarte) {
        case 1: // si garde (num 1)
            // Regarde les certitudes
            for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++) // si non vide et qu'il ne contient pas seullement une garde
                if (not(cartesConnuesDesAutres.at(indice).isEmpty()) && not(cartesConnuesDesAutres.at(indice).size() == 1 && cartesConnuesDesAutres.at(indice).at(0) == 1))
                    listeDesjoueurChoisi.push_back(indice);
            if (not(listeDesjoueurChoisi.isEmpty()))
                break;

            // Regarde les suposition
            for (short int indice = 0; indice < cartesPotentiellesDesAutres.size(); indice++) // si non vide et qu'il ne contient pas seullement une garde
                if (not(cartesPotentiellesDesAutres.at(indice).isEmpty()) && not(cartesConnuesDesAutres.at(indice).size() == 1 && cartesConnuesDesAutres.at(indice).at(0) == 1))
                    listeDesjoueurChoisi.push_back(indice);
            break;
        case 2: // si prètre (num 2)
            // On cherche ceux qui n'ont pas de certitude
            for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++)
                if (cartesConnuesDesAutres.at(indice).isEmpty())
                    listeDesjoueurChoisi.push_back(indice);
            break;
        case 3: // si baron (num 3)
            // On cherce ceux qui ont une carte plus petites dans leurs plus grosse carte connus
            for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++){
                // On cherche la carte max de la personne
                short int maxi = -1;
                for (short int indiceC = 0; indiceC < cartesConnuesDesAutres.at(indice).size(); indiceC++)
                    if (maxi < cartesConnuesDesAutres.at(indice).at(indiceC))
                        maxi = cartesConnuesDesAutres.at(indice).at(indiceC);
                // si la plus grosse carte connus est plus petite que la notre alors on choisis la personne
                if (maxi != -1 &&  maxi < avoirMain().at(0)->avoirNum())
                    listeDesjoueurChoisi.push_back(indice);
            }
            if (not(listeDesjoueurChoisi.isEmpty()))
                break;
            // Sinon on fait pareil avec les supositions
            for (short int indice = 0; indice < cartesPotentiellesDesAutres.size(); indice++){
                // On cherche la carte max de la personne
                short int maxi = -1;
                for (short int indiceC = 0; indiceC < cartesPotentiellesDesAutres.at(indice).size(); indiceC++)
                    if (maxi < cartesPotentiellesDesAutres.at(indice).at(indiceC))
                        maxi = cartesPotentiellesDesAutres.at(indice).at(indiceC);
                // si la plus grosse carte connus est plus petite que la notre alors on choisis la personne
                if (maxi != -1 &&  maxi < avoirMain().at(0)->avoirNum())
                    listeDesjoueurChoisi.push_back(indice);
            }
            break;
        case 5: // si prince (num 5)
            // On cherche ceux qui on la princesse
            for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++)
                if (cartesConnuesDesAutres.at(indice).contains(9))
                    listeDesjoueurChoisi.push_back(indice);
            if (not(listeDesjoueurChoisi.isEmpty()))
                break;
            // On cherche ceux qui on peut être la princesse
            for (short int indice = 0; indice < cartesPotentiellesDesAutres.size(); indice++)
                if (cartesPotentiellesDesAutres.at(indice).contains(9))
                    listeDesjoueurChoisi.push_back(indice);

            // On vérifie si on est à découvert et que c'est rentable de retirer sa main
            if (estDecouvert() && (avoirMain().at(0)->avoirNum() <= 6 && nbCartesRestantes > cartesConnuesDesAutres.size())) // "cartesConnuesDesAutres.size()" = nb de joueur
                return avoirID(); // On se choisis soit même (pas besoin de continuer)
            break;
        case 7: // si roi (num 7)
            // le choix n'est pas important
        default:
            break;
    }

    if (listeDesjoueurChoisi.isEmpty()){ // si on arrive toujours pas à trouver, on prend tout le monde
        for (short int indice = 0; indice < cartesPotentiellesDesAutres.size(); indice++)
            listeDesjoueurChoisi.push_back(indice);
    }

    // On retire ceux qui sont protégé
    for (short int indice = 0; indice < cartesPotentiellesDesAutres.size(); indice++)
        if (not(joueursNonProteger.at(indice)))
            listeDesjoueurChoisi.removeOne(indice);
    listeDesjoueurChoisi.removeOne(avoirID()); // On se retire (au cas où)

    if (listeDesjoueurChoisi.isEmpty())
        return -1;

    return listeDesjoueurChoisi.at(QRandomGenerator::global()->bounded(listeDesjoueurChoisi.size()));
}

void IANormale::miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur, short int cartePerdent){
    QVector<short int> listeCartePotentielDuJoueur; // liste des carte à rajouter dans la "BDD de l'ia"
    QVector<short int> listeCarteImpossibleDuJoueur; // liste des carte à retirer de la "BDD de l'ia"
    QVector<short int> listeCarteCertainesDuJoueur; // liste les cartes certaines dans la "BDD de l'ia"

    QVector<short int> joueursAMettreAJour; // changement à faire dans la "BDD de l'ia" sur 1 ou 2 joueurs (pour parcourir)
    joueursAMettreAJour.push_back(joueurActuel->avoirID()); // par défaut on change sur le joueurActuel

    // On retire la carte jouer / supression de la "BDD de l'ia"
    if (cartesConnuesDesAutres[joueurActuel->avoirID()].removeOne(carteJouer)) // On retire la carte jouer de la "BDD de l'ia"
        cartesConnuesDesAutres[joueurActuel->avoirID()].clear(); // Si il joue une des cartes dont on était sûr, c'est très probable que nos certitudes ne le sont plus
    if (cartesPotentiellesDesAutres[joueurActuel->avoirID()].removeOne(carteJouer)) // On retire la carte jouer de la "BDD de l'ia"
        cartesPotentiellesDesAutres[joueurActuel->avoirID()].clear(); // Presque même suposition que au dessus

    switch (carteJouer){
        case 0: // On ne peut pas savoir
        case 2: // même cas
        case 4: // même cas
        case 9: // Il est déjà mort si il fait ça
            // listeCartePotentielDuJoueur reste vide
            break;
        case 1: // On retire directement la suposition en question sur la personne, si -1 ça ne fera rien
            if (autreJoueur != nullptr){ // si on à pue choisir le joueur
                cartesConnuesDesAutres[autreJoueur->avoirID()].removeOne(cartePerdent); // Ici cartePerdent correspond à la carte suposé
                cartesPotentiellesDesAutres[autreJoueur->avoirID()].removeOne(cartePerdent); // Ici cartePerdent correspond à la carte suposé
            }
            return; // pas besoin de continuer
        case 5: // On repart de 0 pour ses supositions
            if (autreJoueur != nullptr){ // si on à pue choisir le joueur
                cartesConnuesDesAutres[autreJoueur->avoirID()].clear();
                cartesPotentiellesDesAutres[autreJoueur->avoirID()].clear();
            }
            return; // pas besoin de continuer
        case 3: // 5,6,7,8,9 sont les cartes sûr du gagant
            if (autreJoueur != nullptr){ // si on à pue choisir le joueur
                // On recherche le ou les survivants
                if (not(joueurActuel->estEnVie())){ // si le joueur actuel est mort, alors on vas changer l'autre joueur
                    joueursAMettreAJour.pop_back();
                    joueursAMettreAJour.push_back(autreJoueur->avoirID());
                    cartePerdent = joueurActuel->avoirMain()[0]->avoirNum(); // La carte du perdant est celle du joueur actuel
                } else if (autreJoueur->estEnVie()){ // si l'autre joueur est en vie (si l'actuel est en vie alors ils sont les deux dans le QVector)
                    joueursAMettreAJour.push_back(autreJoueur->avoirID());
                    cartePerdent = joueurActuel->avoirMain()[0]->avoirNum(); // La carte du perdant est celle du joueur actuel
                } else // (joueurActuel est le seul survivant) par défaut c'est joueurActuel qui est dans joueursAMettreAJour
                    cartePerdent = autreJoueur->avoirMain()[0]->avoirNum(); // La carte du perdant est celle de l'autre joueur

                for (short int indice = 0; indice <= cartePerdent; indice++) // liste les nb inférieurs ou égal au nb qui à perdu dans les cartes impossible
                    listeCarteImpossibleDuJoueur.push_back(indice);

                if ( joueursAMettreAJour.size() == 1 ){ // si un des deux à gagner alors 5,6,7,8,9
                    if (cartePerdent < 8 && ( (cartesJouer.at(5) + (avoirMain().at(0)->avoirNum() == 5)) < 2) ){ // si il en manque ou que tu n'as pas la dernière et sup à la carte du perdent
                        listeCarteCertainesDuJoueur.push_back(5);
                        listeCartePotentielDuJoueur.push_back(5);
                    }
                    if (cartePerdent < 8 && ( (cartesJouer.at(6) + (avoirMain().at(0)->avoirNum() == 6)) < 2) ){ // si il en manque ou que tu n'as pas la dernière et sup à la carte du perdent
                        listeCarteCertainesDuJoueur.push_back(6);
                        listeCartePotentielDuJoueur.push_back(6);
                    }
                    if (cartePerdent < 8 && ( (cartesJouer.at(7) + (avoirMain().at(0)->avoirNum() == 7)) < 1) ){ // si tu n'as pas la dernière et sup à la carte du perdent
                        listeCarteCertainesDuJoueur.push_back(7);
                        listeCartePotentielDuJoueur.push_back(7);
                    }
                    if (cartePerdent < 8 && ( (cartesJouer.at(8) + (avoirMain().at(0)->avoirNum() == 8)) < 1) ){ // si tu n'as pas la dernière et sup à la carte du perdent
                        listeCarteCertainesDuJoueur.push_back(8);
                        listeCartePotentielDuJoueur.push_back(8);
                    }
                    if ((cartesJouer.at(9) + (avoirMain().at(0)->avoirNum() == 9)) < 1){ // si tu n'as pas la dernière
                        listeCarteCertainesDuJoueur.push_back(9);
                        listeCartePotentielDuJoueur.push_back(9);
                    }
                } else { // cas d'égalitée alors 5 ou 6
                    if ((cartesJouer.at(5) + (avoirMain().at(0)->avoirNum() == 5)) == 0){ // si il en manque ou que tu n'as pas la dernière
                        listeCarteCertainesDuJoueur.push_back(5);
                        listeCartePotentielDuJoueur.push_back(5);
                    }
                    if ((cartesJouer.at(6) + (avoirMain().at(0)->avoirNum() == 6)) == 0){ // si il en manque ou que tu n'as pas la dernière
                        listeCarteCertainesDuJoueur.push_back(6);
                        listeCartePotentielDuJoueur.push_back(6);
                    }
                }
            }
            break;
        case 6: // le plus probable : 0,6,7,8,9
            // les suposition revienne à 0
            cartesPotentiellesDesAutres[joueurActuel->avoirID()].clear();
            cartesConnuesDesAutres[joueurActuel->avoirID()].clear();
            if ((cartesJouer.at(0) + (avoirMain().at(0)->avoirNum() == 0)) < 2) // si il en manque ou que tu n'as pas la dernière
                cartesPotentiellesDesAutres[joueurActuel->avoirID()].push_back(0);
            if ((cartesJouer.at(6) + (avoirMain().at(0)->avoirNum() == 6)) < 2) // si il en manque ou que tu n'as pas la dernière
                cartesPotentiellesDesAutres[joueurActuel->avoirID()].push_back(6);
            if ((cartesJouer.at(7) + (avoirMain().at(0)->avoirNum() == 7)) < 1) // si tu n'as pas la dernière
                cartesPotentiellesDesAutres[joueurActuel->avoirID()].push_back(7);
            if ((cartesJouer.at(8) + (avoirMain().at(0)->avoirNum() == 8)) < 1) // si tu n'as pas la dernière
                cartesPotentiellesDesAutres[joueurActuel->avoirID()].push_back(8);
            if ((cartesJouer.at(9) + (avoirMain().at(0)->avoirNum() == 9)) < 1) // si tu n'as pas la dernière
                cartesPotentiellesDesAutres[joueurActuel->avoirID()].push_back(9);
            return; // On peut même sortir de la fonction pour évité lire des trucs inutiles
        case 7: // On joue comme si c'était sûr que c'est un 9
            if (autreJoueur != nullptr){ // si on à pue choisir le joueur
                // On échange les supositions (suposition autreJoueur écrasé car on vas mettre le 9 après)
                cartesPotentiellesDesAutres[joueurActuel->avoirID()] = cartesPotentiellesDesAutres[autreJoueur->avoirID()];
                cartesConnuesDesAutres[joueurActuel->avoirID()] = cartesConnuesDesAutres[autreJoueur->avoirID()];
                // On met la suposition du 9 dans la cible
                cartesConnuesDesAutres[autreJoueur->avoirID()].clear();
                cartesConnuesDesAutres[autreJoueur->avoirID()].push_back(9); // C'est la cible qui à probablement récupérer le 9
                cartesPotentiellesDesAutres[autreJoueur->avoirID()].clear();
                cartesPotentiellesDesAutres[autreJoueur->avoirID()].push_back(9); // C'est la cible qui à probablement récupérer le 9

                joueursAMettreAJour.pop_back(); // pas nécésaire de mettre à jour car déjà fait
            }
            return; // On peut même sortir de la fonction pour évité lire des trucs inutiles
        case 8: // peut être 5,7,9 mais pas sûr
            if ((cartesJouer.at(5) + (avoirMain().at(0)->avoirNum() == 5)) < 2) // si il en manque ou que tu n'as pas la dernière
                listeCartePotentielDuJoueur.push_back(5);
            if ((cartesJouer.at(7) + (avoirMain().at(0)->avoirNum() == 7)) < 1) // si tu n'as pas la dernière
                listeCartePotentielDuJoueur.push_back(7);
            if ((cartesJouer.at(9) + (avoirMain().at(0)->avoirNum() == 9)) < 1) // si tu n'as pas la dernière
                listeCartePotentielDuJoueur.push_back(9);
            break;
        default:
        break;
    }

    // On Ajoute les nouvelles données à ce qu'on avait déjà sur chaque joueurs à modif
    for (short int idJoueur : joueursAMettreAJour){
        // Avant, on nettoie toutes les données parasites potentiels de partout
        for (short int valeur : listeCarteImpossibleDuJoueur){
            listeCartePotentielDuJoueur.removeOne(valeur);
            listeCarteCertainesDuJoueur.removeOne(valeur);
            cartesPotentiellesDesAutres[idJoueur].removeOne(valeur);
            cartesConnuesDesAutres[idJoueur].removeOne(valeur);
        }

        // pour les données probables
        if (cartesPotentiellesDesAutres[idJoueur].isEmpty()){ // si vide alors on replis normalement
            cartesPotentiellesDesAutres[idJoueur] = listeCartePotentielDuJoueur;
        } else {
            // Sinon garde seulement ce qui est compatible avec les deux (+ probable)
            QVector<short int> intersection;
            for (short int valeur : cartesPotentiellesDesAutres[idJoueur]){
                if (listeCartePotentielDuJoueur.contains(valeur)){
                    intersection.push_back(valeur);
                }
            }
            if (intersection.isEmpty()) // cas où il n'y a pas d'intersection, donc on prend les données les plus fraiches
                cartesPotentiellesDesAutres[idJoueur] = listeCartePotentielDuJoueur;
            else
                cartesPotentiellesDesAutres[idJoueur] = intersection; // sinon l'intersection est plus probable
        }
        // pour les données certaines
        if (cartesConnuesDesAutres[idJoueur].isEmpty()){ // si vide alors on replis normalement
            cartesConnuesDesAutres[idJoueur] = listeCarteCertainesDuJoueur;
        } else {
            // Sinon garde seulement ce qui est compatible avec les deux (+ probable)
            QVector<short int> intersection;
            for (short int valeur : cartesConnuesDesAutres[idJoueur]){
                if (listeCarteCertainesDuJoueur.contains(valeur)){
                    intersection.push_back(valeur);
                }
            }
            if (not(intersection.isEmpty()))
                cartesConnuesDesAutres[idJoueur] = intersection; // sinon l'intersection est plus probable
            // Sinon il est vide mais :
            // Cas où il n'y a pas d'intersection, donc on garde les anciennes données car elles sont plus fiables que ce que le num 3 permet de déduire (trop d'info qui peut perturbé une certitude)
            // même si ce cas n'est pas physiquement possible car le baron montre un carte<x et que si il y a déjà une certitude elle devrait être là
        }
    }
    // On suprime les données qui nous concerne pour pas s'attaquer soit même plus tard
    cartesConnuesDesAutres[avoirID()].clear();
    cartesPotentiellesDesAutres[avoirID()].clear();
}

void IANormale::voirCarteDUnJoueur(Carte* carte, short int joueur){
    // On retire ce qu'on croyait
    cartesConnuesDesAutres[joueur].clear();
    cartesPotentiellesDesAutres[joueur].clear();
    // On rajoute la carte vue dans les certitudes
    cartesConnuesDesAutres[joueur].push_back(carte->avoirNum());
}

short int IANormale::choisir1DeNos3Cartes() const{
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

    // Sinon on prend dans l'ordre de priorité 0, 1, 5, 6, 4, 3, 2
    QVector<short int> prio = {0, 1, 5, 6, 4, 3, 2};
    for (short int carteprio : prio){
        // On vérifie pour les 3 cartes ou moins et si c'est bon, on la return
        for (short int indice = 0; indice < avoirMain().size(); indice++)
            if (avoirMain().at(indice)->avoirNum() == carteprio)
                return indice;
    }
    return 0; // au cas où mais cas impossible
}

short int IANormale::demanderCarteAJoueur(Joueur* joueur, QVector<short int> cartesJouer) const{
    short int val = 1;
    // On regadre dans les cartes connus
    if (not(cartesConnuesDesAutres.at(joueur->avoirID()).isEmpty())){
        while (val != 1){
            val = cartesConnuesDesAutres[joueur->avoirID()][QRandomGenerator::global()->bounded(cartesConnuesDesAutres[joueur->avoirID()].size())];
            if (val != 1) // Si il choisit le garde, c'est impossible
                return val;
        }
    }

    // On regarde les supositions
    if (not(cartesPotentiellesDesAutres.at(joueur->avoirID()).isEmpty())){
        while (val != 1){
            val = cartesPotentiellesDesAutres[joueur->avoirID()][QRandomGenerator::global()->bounded(cartesPotentiellesDesAutres[joueur->avoirID()].size())];
            if (val != 1) // Si il choisit le garde, c'est impossible
                return val;
        }
    }

    // On fait au "pif", on regarde pour toutes les cartes de 0 à 9 sauf le 1 car on peut pas le demander
    QVector<short int> listeCartePotentielDuJoueur;
    if ( (cartesJouer.at(0) + (avoirMain().at(0)->avoirNum() == 0)) < 2) // si il en manque ou que tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(0);
    if ( (cartesJouer.at(2) + (avoirMain().at(0)->avoirNum() == 2)) < 2) // si il en manque ou que tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(2);
    if ( (cartesJouer.at(3) + (avoirMain().at(0)->avoirNum() == 3)) < 2) // si il en manque ou que tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(3);
    if ( (cartesJouer.at(4) + (avoirMain().at(0)->avoirNum() == 4)) < 2) // si il en manque ou que tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(4);
    if ( (cartesJouer.at(5) + (avoirMain().at(0)->avoirNum() == 5)) < 2) // si il en manque ou que tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(5);
    if ( (cartesJouer.at(6) + (avoirMain().at(0)->avoirNum() == 6)) < 2) // si il en manque ou que tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(6);
    if ( (cartesJouer.at(7) + (avoirMain().at(0)->avoirNum() == 7)) < 1) // si tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(7);
    if ( (cartesJouer.at(8) + (avoirMain().at(0)->avoirNum() == 8)) < 1) // si tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(8);
    if ( (cartesJouer.at(9) + (avoirMain().at(0)->avoirNum() == 9)) < 1) // si tu n'as pas la dernière
        listeCartePotentielDuJoueur.push_back(9);

    // On prend aléatoirement dans la liste des cartes plausibles
    while (val != 1){
        val = listeCartePotentielDuJoueur[QRandomGenerator::global()->bounded(listeCartePotentielDuJoueur.size())];
        if (val != 1) // Si il choisit le garde, c'est impossible
            return val;
    }
    return 9; // par défaut mais cas impossible
}