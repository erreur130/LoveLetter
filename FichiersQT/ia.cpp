#include "ia.h"

IA::IA(QString nom, bool inte, short int nbJoueurs) :
    Joueur(nom), intelligent(inte), cartesPotentiellesDesAutres(nbJoueurs, QVector<short int>()), cartesConnuesDesAutres(nbJoueurs, QVector<short int>()){}

IA::~IA() {}

Carte* IA::choisirCarte() const{

}

Joueur* IA::choisirJoueur(TypeCarte) const{

}

void IA::miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur, short int cartePerdent){
    QVector<short int> listeCartePotentielDuJoueur; // liste des carte à rajouter dans la "BDD de l'ia"
    QVector<short int> listeCarteImpossibleDuJoueur; // liste des carte à retirer de la "BDD de l'ia"
    QVector<short int> listeCarteCertainesDuJoueur; // liste les cartes certaines dans la "BDD de l'ia"

    QVector<short int> joueursAMettreAJour; // changement à faire dans la "BDD de l'ia" sur 1 ou 2 joueurs (pour parcourir)
    joueursAMettreAJour.append(joueurActuel->avoirID()); // par défaut on change sur le joueurActuel

    // On retire la carte jouer / supression de la "BDD de l'ia"
    if (cartesConnuesDesAutres[joueurActuel->avoirID()].removeOne(carteJouer)) // On retire la carte jouer de la "BDD de l'ia"
        cartesConnuesDesAutres[joueurActuel->avoirID()].clear(); // Si il joue une des cartes dont on était sûr, c'est très probable que nos certitudes ne le sont plus
    if (cartesPotentiellesDesAutres[joueurActuel->avoirID()].removeOne(carteJouer)) // On retire la carte jouer de la "BDD de l'ia"
        cartesPotentiellesDesAutres[joueurActuel->avoirID()].clear(); // Presque même suposition que au dessus

    switch (carteJouer){
        case 0: // On ne peut pas savoir
        case 1: // même cas
        case 2: // même cas
        case 4: // même cas
        case 5: // même cas
        case 9: // Il est déjà mort si il fait ça
            // listeCartePotentielDuJoueur reste vide
            break;
        case 3: // 5,6,7,8,9 sont les cartes sûr du gagant
            // On recherche le ou les survivants
            if (not(joueurActuel->estEnVie())){ // si le joueur actuel est mort, alors on vas changer l'autre joueur
                joueursAMettreAJour.pop_back();
                joueursAMettreAJour.push_back(autreJoueur->avoirID());
            } else if (autreJoueur->estEnVie()){ // si l'autre joueur est en vie (si l'actuel est en vie alors ils sont les deux dans le QVector)
                joueursAMettreAJour.push_back(autreJoueur->avoirID());
            } // else : (joueurActuel est le seul survivant) par défaut c'est joueurActuel qui est dans joueursAMettreAJour

            for (short int indice = 0; indice <= cartePerdent; indice++) // liste les nb inférieurs ou égal au nb qui à perdu dans les cartes impossible
                listeCarteImpossibleDuJoueur.append(indice);

            if ( joueursAMettreAJour.size() == 1 ){ // si un des deux à gagner alors 5,6,7,8,9
                if (cartePerdent < 5 && (cartesJouer[5] <= 2 || (cartesJouer[5] <= 1 && avoirMain().at(0)->avoirNum() != 5 )) ){ // si il en manque ou que tu n'as pas la dernière et sup à la carte du perdent
                    listeCarteCertainesDuJoueur.append(5);
                    listeCartePotentielDuJoueur.append(5);
                }
                if (cartePerdent < 6 && (cartesJouer[6] <= 2 || (cartesJouer[6] <= 1 && avoirMain().at(0)->avoirNum() != 6 )) ){ // si il en manque ou que tu n'as pas la dernière et sup à la carte du perdent
                    listeCarteCertainesDuJoueur.append(6);
                    listeCartePotentielDuJoueur.append(6);
                }
                if (cartePerdent < 7 && (cartesJouer[7] <= 0 && avoirMain().at(0)->avoirNum() != 7 ) ){ // si tu n'as pas la dernière et sup à la carte du perdent
                    listeCarteCertainesDuJoueur.append(7);
                    listeCartePotentielDuJoueur.append(7);
                }
                if (cartePerdent < 8 && (cartesJouer[8] <= 0 && avoirMain().at(0)->avoirNum() != 8 ) ){ // si tu n'as pas la dernière et sup à la carte du perdent
                    listeCarteCertainesDuJoueur.append(8);
                    listeCartePotentielDuJoueur.append(8);
                }
                if (cartesJouer[9] <= 0 || avoirMain().at(0)->avoirNum() != 9 ){ // si tu n'as pas la dernière
                    listeCarteCertainesDuJoueur.append(9);
                    listeCartePotentielDuJoueur.append(9);
                }
            } else { // cas d'égalitée alors 5 ou 6
                if (cartesJouer[5] <= 2 || (cartesJouer[5] <= 1 && avoirMain().at(0)->avoirNum() != 5 )){ // si il en manque ou que tu n'as pas la dernière
                    listeCarteCertainesDuJoueur.append(5);
                    listeCartePotentielDuJoueur.append(5);
                }
                if (cartesJouer[6] <= 2 || (cartesJouer[6] <= 1 && avoirMain().at(0)->avoirNum() != 6 )){ // si il en manque ou que tu n'as pas la dernière
                    listeCarteCertainesDuJoueur.append(6);
                    listeCartePotentielDuJoueur.append(6);
                }
            }
            // rajout de listeCarteCertainesDuJoueur ----- dans la bdd -----------------------------------------------------------------------*-*-*-*-*-*-*-*-*-*-
            break;
        case 6: // le plus probable : 0,6,7,8,9
            if (cartesJouer[0] <= 2 || (cartesJouer[0] <= 1 && avoirMain().at(0)->avoirNum() != 0 )) // si il en manque ou que tu n'as pas la dernière
                listeCartePotentielDuJoueur.append(0);
            if (cartesJouer[6] <= 2 || (cartesJouer[6] <= 1 && avoirMain().at(0)->avoirNum() != 6 )) // si il en manque ou que tu n'as pas la dernière
                listeCartePotentielDuJoueur.append(6);
            if (cartesJouer[7] <= 0 && avoirMain().at(0)->avoirNum() != 7 ) // si tu n'as pas la dernière
                listeCartePotentielDuJoueur.append(7);
            if (cartesJouer[8] <= 0 && avoirMain().at(0)->avoirNum() != 8 ) // si tu n'as pas la dernière
                listeCartePotentielDuJoueur.append(8);
            if (cartesJouer[9] <= 0 || avoirMain().at(0)->avoirNum() != 9 ) // si tu n'as pas la dernière
                listeCartePotentielDuJoueur.append(9);
            break;
        case 7: // On joue comme si c'était sûr que c'est un 9
            // On échange les supositions (suposition autreJoueur écrasé car on vas mettre le 9 après)
            cartesPotentiellesDesAutres[joueurActuel->avoirID()] = cartesPotentiellesDesAutres[autreJoueur->avoirID()];
            cartesConnuesDesAutres[joueurActuel->avoirID()] = cartesConnuesDesAutres[autreJoueur->avoirID()];
            // On met la suposition du 9 dans la cible
            cartesConnuesDesAutres[autreJoueur->avoirID()].clear();
            cartesConnuesDesAutres[autreJoueur->avoirID()].append(9); // C'est la cible qui à probablement récupérer le 9
            cartesPotentiellesDesAutres[autreJoueur->avoirID()].clear();
            cartesPotentiellesDesAutres[autreJoueur->avoirID()].append(9); // C'est la cible qui à probablement récupérer le 9

            joueursAMettreAJour.pop_back(); // pas nécésaire de mettre à jour car déjà fait
            return; // On peut même sortir de la fonction pour évité lire des trucs inutiles
        case 8: // peut être 5,7,9 mais pas sûr
            if (cartesJouer[5] <= 2 || (cartesJouer[5] <= 1 && avoirMain().at(0)->avoirNum() != 5 )) // si il en manque ou que tu n'as pas la dernière
                listeCartePotentielDuJoueur.append(5);
            if (cartesJouer[7] <= 0 && avoirMain().at(0)->avoirNum() != 7 ) // si tu n'as pas la dernière
                listeCartePotentielDuJoueur.append(7);
            if (cartesJouer[9] <= 0 || avoirMain().at(0)->avoirNum() != 9 ) // si tu n'as pas la dernière
                listeCartePotentielDuJoueur.append(9);
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
                    intersection.append(valeur);
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
                    intersection.append(valeur);
                }
            }
            if (not(intersection.isEmpty()))
                cartesConnuesDesAutres[idJoueur] = intersection; // sinon l'intersection est plus probable
            // Sinon il est vide mais :
            // Cas où il n'y a pas d'intersection, donc on garde les anciennes données car elles sont plus fiables que ce que le num 3 permet de déduire (trop d'info qui peut perturbé une certitude)
            // même si ce cas n'est pas physiquement possible car le baron montre un carte<x et que si il y a déjà une certitude elle devrait être là
        }
    }
}