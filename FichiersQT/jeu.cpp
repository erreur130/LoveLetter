#include "jeu.h"

Jeu::Jeu(QObject* parent, short int h, short int inul, short int inorm, short int itri)
    :QObject(parent), objectifPoints(0), joueurs(QVector<Joueur*>()), joueurActuel(nullptr), joueurCible(nullptr), carteEnCourDeJeux(nullptr), pioche(Paquet()){
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

    // On affiche la liste des joueurs
    QVector<QString> nomJoueurs;
    for (Joueur* joueur : joueurs)
        nomJoueurs.push_back(joueur->avoirNom());
    emit initialiserListeJoueurs(nomJoueurs);

    // Affiche le nb de base de carte
    emit miseAJourCartesJouees(pioche.avoirCartesJouer());
    emit miseAJourNbCartesRestantes(pioche.avoirNbCartesRestantes());
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

QVector<Joueur*> Jeu::JoueursChoixPossible() const{
    QVector<Joueur*> result;
    for (short int indice = 0; indice < joueurs.size(); indice++)
        if (joueurs[indice]->estEnVie() == true && joueurs[indice]->estPorteger() == false)
            result.push_back(joueurs[indice]);
    return result;
}

bool Jeu::tourSuivant(){
    joueurCible = nullptr; // On vide les actions dernier tour
    carteEnCourDeJeux = nullptr; // On vide les actions dernier tour

    if (pioche.avoirNbCartesRestantes() > 0){ // si plus de carte
        QVector<Joueur*> joueurEnVie = JoueursRestant();
        if (joueurEnVie.size() > 1){ // si plusieurs joueurs en jeu
            // ici on peut continuer, donc on cherche le joueur suivant:
            short int indiceActuel = joueurActuel->avoirID();

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

void Jeu::miseAJourCartesPotentiel(Carte* cartePerdent){

    short int numCarte = carteEnCourDeJeux->avoirNum(); // num de la carte qui vient d'être joué

    short int numCartePerdent = -1;
    if (cartePerdent != nullptr)
        numCartePerdent = cartePerdent->avoirNum();

    short int indiceActuel = joueurActuel->avoirID();
    for (short int indice = 0; indice < joueurs.size(); indice++){
        if (indice != indiceActuel && joueurs[indice]->estEnVie() == true)
            joueurs[indice]->miseAJourCartesPotentiel(pioche.avoirCartesJouer(), joueurActuel, numCarte, joueurCible, numCartePerdent);
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
    emit reinitialiserLog();
}

void Jeu::eliminationJoueur(Joueur* joueur){
    emit messageLog(joueur->avoirNom() + "est éliminé !");
    joueur->eliminer();
}

void Jeu::lancerManche(){
    reinitialiserManche();

    lancerTour(); // démarre la première manche, premier tour et s'enchaine à la suite
}

void Jeu::lancerTour(){
    // On change l'affichaque de ce qui reste en cartes
    emit miseAJourCartesJouees(pioche.avoirCartesJouer());
    emit miseAJourNbCartesRestantes(pioche.avoirNbCartesRestantes());

    if (tourSuivant()){ // Si on peut continuer
        joueurActuel->ajouterCarte(pioche.piocher()); // il pioche

        // On fournit les infos nésésaire pour une ia
        QVector<bool> joueursChoix;
        for (Joueur* joueur : joueurs)
            joueursChoix.push_back(not(joueur->estPorteger()) && joueur->estEnVie()); // si il est en vie et pas proteger

        Carte* choixCarte = joueurActuel->choisirCarte(pioche.avoirNbCartesRestantes(), joueursChoix);

        if (choixCarte == nullptr){ // Si hummain on affiche
            emit messageAlerteMainJoueurVasEtreMontre(joueurActuel->avoirNom());
            emit afficherMain(joueurActuel->avoirMain()[0]->avoirImage(), joueurActuel->avoirMain()[1]->avoirImage());

        } else { // Si ia on fait la suite de calcules (tout d'un coups)
            carteEnCourDeJeux = choixCarte;
            pioche.carteAEtaitJouer(carteEnCourDeJeux->avoirNum()); // On signale à la pioche que cette carte à était jouer (visible)
            short int carteADemander = -1;
            // les fonctions font en sorte de retirer this du choix des joueurs, donc on ne modifie pas joueursChoix
            switch (carteEnCourDeJeux->estType()){
                case TypeCarte::Offensif: // sur autre personne et le choix d'une carte (num : 1)
                    joueurCible = joueurs[joueurActuel->choisirJoueur(carteEnCourDeJeux, joueursChoix)];
                    carteADemander = joueurActuel->demanderCarteAJoueur(joueurCible, pioche.avoirCartesJouer());
                    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible, pioche[carteADemander]));
                    break;

                case TypeCarte::Duel: // sur une autres personne et soi même (num : 2,3,7) / num 2 car la personne qui la joue regarde l'autre
                case TypeCarte::OffensifOuDefensif: // sur une autres personne et soi même! (num 5)
                    joueurCible = joueurs[joueurActuel->choisirJoueur(carteEnCourDeJeux, joueursChoix)];
                    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible));

                    if(not(joueurActuel->estEnVie()))// Si 1 des deux meure alors on prend ça carte dans carteADemander (pour connaisance baron)
                        carteADemander = joueurActuel->avoirMain()[0]->avoirNum();
                    else if(not(joueurCible->estEnVie()))
                        carteADemander = joueurCible->avoirMain()[0]->avoirNum();
                    break;

                case TypeCarte::Defensif: // sur soi (num : 0,4,6,9) le 9 ne protège pas vraiment ;) mais ce joue sur soie même
                case TypeCarte::SansEffet: // ne fait rien (num 8)
                default:
                    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux));
            }
            miseAJourCartesPotentiel(pioche[carteADemander]); // met à jour les connaisances des IA

            lancerTour(); // permet de continuer la partie (récurcivité)
        }
    } else {
        if (pioche.avoirNbCartesRestantes() <= 0)
            finDeManche(true);
        else
            finDeManche(false);
    }
}

void Jeu::finDeManche(bool finDuPaquet){
    if(finDuPaquet){ // Si fin de la manche par la fin du paquet, on garde les joueurs avec la plus grosse carte
        short int numMax = 0;
        for (Joueur* joueur : joueurs)
            if (joueur->avoirMain().at(0)->avoirNum() > numMax)
                numMax = joueur->avoirMain()[0]->avoirNum();
        for (Joueur* joueur : joueurs)
            if(joueur->estEnVie() && joueur->avoirMain().at(0)->avoirNum() < numMax)
                joueur->eliminer();
    }

    QVector<QString> nomJoueurs;
    QVector<short int> pointsJoueurs;
    for (Joueur* joueur : joueurs)
        if (joueur->estEnVie()){
            nomJoueurs.push_back(joueur->avoirNom()); // On les listes
            joueur->gainPoints(); // On donne les points aux gagants
            pointsJoueurs.push_back(joueur->avoirPoints()); // met à jours l'affichage des points
    }
    emit miseAJourPointsJoueurs(pointsJoueurs);
    emit afficherVictoireManche(nomJoueurs);

    QVector<Joueur*> gagnants = verifSiGagnants();
    if(gagnants.empty()) // si pas de gagant on continue
        lancerManche();
    else { // Sinon on arrête
        QVector<QString> nomGagnants;
        for (Joueur* joueur : joueurs)
            nomGagnants.push_back(joueur->avoirNom());
        emit afficherVictoireJeu(nomGagnants);
    }
}

// ---------------------------slots------------------------------------------- MainWindow -> Jeu

void Jeu::recevoirChoixCarte(short int idCarte){
    carteEnCourDeJeux = pioche[idCarte]; // On note la carte choisit
    pioche.carteAEtaitJouer(carteEnCourDeJeux->avoirNum()); // On signale à la pioche que cette carte à était jouer (visible)

    QVector<Joueur*> joueursPossible;
    QVector<QString> nomJoueursPossible;
    QVector<short int> idJoueursPossible;

    switch (carteEnCourDeJeux->estType()){
        case TypeCarte::Offensif: // sur autre personne et le choix d'une carte (num : 1)
        case TypeCarte::Duel: // sur une autres personne et soi même (num : 2,3,7) / num 2 car la personne qui la joue regarde l'autre
            joueursPossible = JoueursChoixPossible();
            joueursPossible.removeOne(joueurActuel); // Pour pas ce viser soit même
            for (short int indice = 0; indice < joueursPossible.size(); indice++){
                nomJoueursPossible.push_back(joueursPossible[indice]->avoirNom());
                idJoueursPossible.push_back(joueursPossible[indice]->avoirID());
            }
            emit demanderChoixCibleJoueur(nomJoueursPossible, idJoueursPossible);
            // la suite demandera la carte à choisir pour Offensif
            break;
        case TypeCarte::OffensifOuDefensif: // sur une autres personne et soi même! (num 5)
            joueursPossible = JoueursChoixPossible();
            //joueursPossible.removeOne(joueurActuel); // Pour ce viser soit même on le garde !
            for (short int indice = 0; indice < joueursPossible.size(); indice++){
                nomJoueursPossible.push_back(joueursPossible[indice]->avoirNom());
                idJoueursPossible.push_back(joueursPossible[indice]->avoirID());
            }
            emit demanderChoixCibleJoueur(nomJoueursPossible, idJoueursPossible);
            break;
        case TypeCarte::Defensif: // sur soi (num : 0,4,6,9) le 9 ne protège pas vraiment ;) mais ce joue sur soie même
        case TypeCarte::SansEffet: // ne fait rien (num 8)
        default:
            emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux));
            miseAJourCartesPotentiel(); // met à jour les connaisances des IA
            lancerTour(); // permet de continuer la partie
    }
}

void Jeu::recevoirChoixValeurGarde(short int valeur){
    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux /*num 1*/, joueurCible, pioche[valeur]));
    miseAJourCartesPotentiel(pioche[valeur]); // met à jour les connaisances des IA
    lancerTour(); // permet de continuer la partie
}

void Jeu::recevoirChoixCibleJoueur(short int joueur){
    joueurCible = joueurs[joueur]; // On note la cible choisit
    short int carteADemander = -1;

    // soit Offensif / Duel / OffensifOuDefensif
    switch (carteEnCourDeJeux->estType()) {
        case TypeCarte::Offensif: // sur autre personne et le choix d'une carte (num : 1)
            emit demanderChoixValeurGarde();
            break;
        case TypeCarte::Duel: // sur une autres personne et soi même (num : 2,3,7) / num 2 car la personne qui la joue regarde l'autre
            emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible));

            if(not(joueurActuel->estEnVie()))// Si 1 des deux meure alors on prend ça carte dans carteADemander (pour connaisance baron)
                carteADemander = joueurActuel->avoirMain()[0]->avoirNum();
            else if(not(joueurCible->estEnVie()))
                carteADemander = joueurCible->avoirMain()[0]->avoirNum();
            miseAJourCartesPotentiel(pioche[carteADemander]); // met à jour les connaisances des IA

            lancerTour(); // permet de continuer la partie
        case TypeCarte::OffensifOuDefensif: // sur une autres personne et soi même! (num 5)
            emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible));
            miseAJourCartesPotentiel(); // met à jour les connaisances des IA
            lancerTour(); // permet de continuer la partie
        default:
            break;
    }
}

void Jeu::rejouer(){
    joueurActuel = joueurs[0]; // On choisis le premier joueur
    for (Joueur* joueur : joueurs)
        joueur->retirerPoints(); // On remet les compteurs à 0
    lancerManche();
}

