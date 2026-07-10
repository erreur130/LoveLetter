#include "ianul.h"

IANul::IANul(QString nom, short int nbJoueurs):
    Joueur(nom), cartesConnuesDesAutres(nbJoueurs, QVector<short int>()){}

IANul::~IANul(){

}

Carte* IANul::choisirCarte(short int, QVector<bool>) const{
    // On considère que le joueur viens de pioché, donc il à 2 cartes (1ère carte est l'ancienne et la deuxème est celle pioché)

    // cas obligatoires
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

    // pas de cas d'algo (volontairement)
    return main.at(QRandomGenerator::global()->bounded(2)); // on prend 0 ou 1
}

short int  IANul::choisirJoueur(Carte* carte, QVector<bool> joueursNonProteger, short int) const{
    // normellement la fonction peut recevoir les cartes 1,2,3,5,7
    short int numCarte = carte->avoirNum();
    QVector<short int> listeDesjoueurChoisi;

    switch (numCarte) {
    case 1: // si garde (num 1)
        // Regarde les certitudes
        for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++) // si non vide et qu'il ne contient pas seullement une garde
            if (not(cartesConnuesDesAutres.at(indice).isEmpty()) && not(cartesConnuesDesAutres.at(indice).size() == 1 && cartesConnuesDesAutres.at(indice).at(0) == 1))
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
        break;
    case 5: // si prince (num 5)
        // On cherche ceux qui on la princesse
        for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++)
            if (cartesConnuesDesAutres.at(indice).contains(9))
                listeDesjoueurChoisi.push_back(indice);
    case 7: // si roi (num 7)
        // le choix n'est pas important
    default:
        break;
    }

    if (listeDesjoueurChoisi.isEmpty()){ // si on arrive toujours pas à trouver, on prend tout le monde
        for (short int indice = 0; indice < cartesConnuesDesAutres.size(); indice++)
            listeDesjoueurChoisi.push_back(indice);
    }

    qDebug() << "joueursNonProteger avant filtrage :" << listeDesjoueurChoisi << " / et filtre : " << joueursNonProteger;

    // On retire ceux qui sont protégé et ou mort
    for (short int indice = 0; indice < joueursNonProteger.size(); indice++)
        if (not(joueursNonProteger.at(indice)))
            listeDesjoueurChoisi.removeOne(indice);
    if (numCarte != 5) // On peut se choisir avec le prince
        listeDesjoueurChoisi.removeOne(avoirID()); // On se retire (au cas où)

    qDebug() << "joueursNonProteger après filtrage :" << listeDesjoueurChoisi;

    if (listeDesjoueurChoisi.isEmpty())
        return -1;

    return listeDesjoueurChoisi.at(QRandomGenerator::global()->bounded(listeDesjoueurChoisi.size()));
}

void IANul::miseAJourCartesPotentiel(QVector<short int> cartesJouer, Joueur* joueurActuel, short int carteJouer, Joueur* autreJoueur, short int cartePerdent){
    QVector<short int> listeCarteImpossibleDuJoueur; // liste des carte à retirer de la "BDD de l'ia"
    QVector<short int> listeCarteCertainesDuJoueur; // liste les cartes certaines dans la "BDD de l'ia"

    QVector<short int> joueursAMettreAJour; // changement à faire dans la "BDD de l'ia" sur 1 ou 2 joueurs (pour parcourir)
    joueursAMettreAJour.push_back(joueurActuel->avoirID()); // par défaut on change sur le joueurActuel

    // On retire la carte jouer / supression de la "BDD de l'ia"
    if (cartesConnuesDesAutres[joueurActuel->avoirID()].removeOne(carteJouer)) // On retire la carte jouer de la "BDD de l'ia"
        cartesConnuesDesAutres[joueurActuel->avoirID()].clear(); // Si il joue une des cartes dont on était sûr, c'est très probable que nos certitudes ne le sont plus

    // On retire toutes les cartes qui ne sont plus possible d'obtenir pour tout les joueurs
    for (short int indiceJoueur = 0; indiceJoueur < cartesConnuesDesAutres.size(); indiceJoueur++){ // pour chaque joueur
        for (short int indiceCarte = 0; indiceCarte < cartesJouer.size(); indiceCarte++){ // pour chaque cartes
            if (not(carteEstPossible(indiceCarte, ((indiceCarte == 1) ? 6 : (indiceCarte > 6) ? 1 : 2) , cartesJouer[indiceCarte])))
                cartesConnuesDesAutres[indiceJoueur].removeOne(indiceCarte);
        }
    }

    switch (carteJouer){
    case 1: // On retire directement la suposition en question sur la personne, si -1 ça ne fera rien
        if (autreJoueur != nullptr) // si on à pue choisir le joueur
            cartesConnuesDesAutres[autreJoueur->avoirID()].removeOne(cartePerdent); // Ici cartePerdent correspond à la carte suposé
        return; // pas besoin de continuer
    case 5: // On repart de 0 pour ses supositions
        if (autreJoueur != nullptr) // si on à pue choisir le joueur
            cartesConnuesDesAutres[autreJoueur->avoirID()].clear();
        return; // pas besoin de continuer
    case 3: // 5,6,7,8,9 sont les cartes sûr du gagant
        // si on à pue choisir le joueur et que nous ne somme pas concerné par l'action du baron (car les deux concerné connaise déjà la carte de l'autre)
        if (autreJoueur != nullptr && not(avoirID() == joueurActuel->avoirID() || avoirID() == autreJoueur->avoirID())){
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
                if (cartePerdent < 5 && carteEstPossible(5, 2, cartesJouer[5])) // si il en manque ou que tu n'as pas la dernière et sup à la carte du perdent
                    listeCarteCertainesDuJoueur.push_back(5);
                if (cartePerdent < 6 && carteEstPossible(6, 2, cartesJouer[6])) // si il en manque ou que tu n'as pas la dernière et sup à la carte du perdent
                    listeCarteCertainesDuJoueur.push_back(6);
                if (cartePerdent < 7 && carteEstPossible(7, 2, cartesJouer[7])) // si tu n'as pas la dernière et sup à la carte du perdent
                    listeCarteCertainesDuJoueur.push_back(7);
                if (cartePerdent < 8 && carteEstPossible(8, 1, cartesJouer[8])) // si tu n'as pas la dernière et sup à la carte du perdent
                    listeCarteCertainesDuJoueur.push_back(8);
                if (carteEstPossible(9, 1, cartesJouer[9])) // si tu n'as pas la dernière
                    listeCarteCertainesDuJoueur.push_back(9);
            } else { // cas d'égalitée alors 5 ou 6
                if (cartesJouer.at(5) == 0 && not(avoirMain().at(0)->avoirNum() == 5) ) // si il n'en manque pas ou que tu n'as pas la dernière (si tu n'est pas visé)
                    listeCarteCertainesDuJoueur.push_back(5);
                if (cartesJouer.at(6) == 0 && not(avoirMain().at(0)->avoirNum() == 6) ) // si il n'en manque pas ou que tu n'as pas la dernière (si tu n'est pas visé)
                    listeCarteCertainesDuJoueur.push_back(6);
            }
        }
        break;
    case 6: // que de la suposition donc on ne regarde pas, mais on retire les certitudes
        cartesConnuesDesAutres[joueurActuel->avoirID()].clear();
        break;
    case 7: // On joue comme si c'était sûr que c'est un 9
        if (autreJoueur != nullptr){ // si on à pue choisir le joueur
            // On échange les supositions (suposition autreJoueur écrasé car on vas mettre le 9 après)
            cartesConnuesDesAutres[joueurActuel->avoirID()] = cartesConnuesDesAutres[autreJoueur->avoirID()];
            // On met la suposition du 9 dans la cible
            cartesConnuesDesAutres[autreJoueur->avoirID()].clear();
            cartesConnuesDesAutres[autreJoueur->avoirID()].push_back(9); // C'est la cible qui à probablement récupérer le 9

            joueursAMettreAJour.pop_back(); // pas nécésaire de mettre à jour car déjà fait
        }
        return; // On peut même sortir de la fonction pour évité lire des trucs inutiles
    case 0: // On ne peut pas savoir
    case 2: // même cas
    case 4: // même cas
    case 8: // peut être 5,7,9 mais pas sûr
    case 9: // Il est déjà mort si il fait ça
    default:
        // listeCartePotentielDuJoueur reste vide
        break;
    }

    // On Ajoute les nouvelles données à ce qu'on avait déjà sur chaque joueurs à modif
    for (short int idJoueur : joueursAMettreAJour){
        // Avant, on nettoie toutes les données parasites potentiels de partout
        for (short int valeur : listeCarteImpossibleDuJoueur){
            listeCarteCertainesDuJoueur.removeOne(valeur);
            cartesConnuesDesAutres[idJoueur].removeOne(valeur);
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
}

void IANul::voirCarteDUnJoueur(Carte* carte, short int joueur){
    // On retire ce qu'on croyait
    cartesConnuesDesAutres[joueur].clear();
    // On rajoute la carte vue dans les certitudes
    cartesConnuesDesAutres[joueur].push_back(carte->avoirNum());
}

short int IANul::choisir1DeNos3Cartes() const{
    // Ici, le joueur est censé avoir 3 cartes dans la main, mais si le paquet à moins de cartes alors il y aura moins de choix

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

    // Sinon on fait au pif
    return QRandomGenerator::global()->bounded(avoirMain().size()); // on prend 0 ou 1 ou 2;
}

short int IANul::demanderCarteAJoueur(Joueur* joueur, QVector<short int> cartesJouer) const{
    short int val = 1;
    // On regadre dans les cartes connus
    if (not(cartesConnuesDesAutres.at(joueur->avoirID()).isEmpty())){
        while (val == 1){
            val = cartesConnuesDesAutres[joueur->avoirID()][QRandomGenerator::global()->bounded(cartesConnuesDesAutres[joueur->avoirID()].size())];
            if (val != 1) // Si il choisit le garde, c'est impossible
                return val;
        }
    }

    // On fait au "pif", on regarde pour toutes les cartes de 0 à 9 sauf le 1 car on peut pas le demander
    QVector<short int> listeCartePotentielDuJoueur;
    for (short int indiceCarte = 0; indiceCarte < cartesJouer.size(); indiceCarte++){ // pour chaque cartes
        if (indiceCarte != 1 && carteEstPossible(indiceCarte, ((indiceCarte == 1) ? 6 : (indiceCarte > 6) ? 1 : 2) , cartesJouer[indiceCarte]))
            listeCartePotentielDuJoueur.push_back(indiceCarte);
    }

    // On prend aléatoirement dans la liste des cartes plausibles
    while (val == 1){
        val = listeCartePotentielDuJoueur[QRandomGenerator::global()->bounded(listeCartePotentielDuJoueur.size())];
        if (val != 1) // Si il choisit le garde, c'est impossible
            return val;
    }
    return val; // par défaut mais cas impossible
}