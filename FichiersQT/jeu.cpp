#include "jeu.h"

Jeu::Jeu(QObject* parent, short int h, short int inul, short int inorm, short int itri)
    :QObject(parent), objectifPoints(0), joueurs(QVector<Joueur*>()), joueurActuel(nullptr), joueurCible(nullptr), carteEnCourDeJeux(nullptr), pioche(Paquet()){
    // création des différent type ce joueurs
    for (short int indice = 0; indice < h; indice++)
        joueurs.push_back(new Humain(QString("Joueur_") + QString::number(indice + 1)));
    for (short int indice = 0; indice < inul; indice++)
        joueurs.push_back(new IANul(QString("IAnul_") + QString::number(indice + 1), h+inul+inorm+itri));
    for (short int indice = 0; indice < inorm; indice++)
        joueurs.push_back(new IANormale(QString("IAnormale_") + QString::number(indice + 1), h+inul+inorm+itri));
    for (short int indice = 0; indice < itri; indice++)
        joueurs.push_back(new IATriche(QString("IAtriche_") + QString::number(indice + 1), joueurs));

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
            joueurs[indice]->miseAJourCartesPotentiel(pioche.avoirCartesJouer(), joueurActuel, numCarte, joueurCible, numCartePerdent); // j'aurais pue faire en fonction du type
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
    pioche.remplir();

    // On remet à 0 l'état des joueurs et on donne 1 carte à tout le monde
    for (short int indice = 0; indice < joueurs.size(); indice++){
        Joueur* joueur = joueurs[indice];
        joueur->reinitialiser();
        joueur->ajouterCarte(pioche.piocher());
    }

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

    if (tourSuivant()){ // Si on peut continuer
        joueurActuel->retirerProtection(); // On retire sa protection (servante ne protège qu'un seul tour)
        joueurActuel->ajouterCarte(pioche.piocher()); // il pioche

        // On change l'affichaque de ce qui reste en cartes
        emit miseAJourCartesJouees(pioche.avoirCartesJouer());
        emit miseAJourNbCartesRestantes(pioche.avoirNbCartesRestantes());

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
            short int cible = -1;
            // les fonctions font en sorte de retirer this du choix des joueurs, donc on ne modifie pas joueursChoix
            switch (carteEnCourDeJeux->estType()){
                case TypeCarte::Offensif: // sur autre personne et le choix d'une carte (num : 1)
                    cible = joueurActuel->choisirJoueur(carteEnCourDeJeux, joueursChoix, pioche.avoirNbCartesRestantes());
                    if (cible == -1){ // cas où on ne peut pas choisir de joueur
                        emit messageLog(joueurActuel->avoirNom() + " joue la carte " + carteEnCourDeJeux->avoirNom() + " sur personne.");
                        break;
                    }
                    joueurCible = joueurs[cible];
                    carteADemander = joueurActuel->demanderCarteAJoueur(joueurCible, pioche.avoirCartesJouer());
                    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible, pioche[carteADemander]));
                    break;

                case TypeCarte::Duel: // sur une autres personne et soi même (num : 2,3,7) / num 2 car la personne qui la joue regarde l'autre
                case TypeCarte::OffensifOuDefensif: // sur une autres personne et soi même! (num 5)
                    cible = joueurActuel->choisirJoueur(carteEnCourDeJeux, joueursChoix, pioche.avoirNbCartesRestantes());
                    if (cible == -1){ // cas où on ne peut pas choisir de joueur
                        emit messageLog(joueurActuel->avoirNom() + " joue la carte " + carteEnCourDeJeux->avoirNom() + " sur personne.");
                        break;
                    }
                    joueurCible = joueurs[cible];
                    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible));

                    if(not(joueurActuel->estEnVie()))// Si 1 des deux meure alors on prend ça carte dans carteADemander (pour connaisance baron)
                        carteADemander = joueurActuel->avoirMain()[0]->avoirNum();
                    else if(not(joueurCible->estEnVie()))
                        carteADemander = joueurCible->avoirMain()[0]->avoirNum();

                    // ici la mise à jour des connaisances des IA nécésite une info suplémentaire
                    if (carteEnCourDeJeux->avoirNum() == 1) // le garde (num 1)
                        miseAJourCartesPotentiel(pioche[carteADemander]); // il faut la carte demander
                    else
                        miseAJourCartesPotentiel(); // met à jour les connaisances des IA, version normale
                    break;

                case TypeCarte::Defensif: // sur soi (num : 0,4,6,9) le 9 ne protège pas vraiment ;) mais ce joue sur soie même
                case TypeCarte::SansEffet: // ne fait rien (num 8)
                    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux));
                    miseAJourCartesPotentiel(); // met à jour les connaisances des IA
            }

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
            if (joueur->avoirMain().at(0)->avoirNum() > numMax) // si plus grand on le note
                numMax = joueur->avoirMain()[0]->avoirNum();
        for (Joueur* joueur : joueurs)
            if(joueur->estEnVie() && joueur->avoirMain().at(0)->avoirNum() < numMax) // si plus petit que le max alors il meurt
                joueur->eliminer();
    }

    // recherche les pts bonnus, si deux, ils annulent
    Joueur* joueur1 = nullptr;
    for (Joueur* joueur : joueurs){
        if (joueur->avoirPointBonus()){
            if (joueur1 == nullptr)
                joueur1 = joueur;
            else { // cas où y a les deux
                joueur1->retirerPtBonus();
                joueur->retirerPtBonus();
                break;
            }
        }
    }

    // On donne les points et on les affiches
    QVector<QString> nomJoueurs;
    QVector<short int> pointsJoueurs;
    for (Joueur* joueur : joueurs){
        nomJoueurs.push_back(joueur->avoirNom()); // On les listes
        joueur->gainPoints(); // On donne les points (gainPoints donne des pt si enVie et si ptBonnus)
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
        case TypeCarte::OffensifOuDefensif: // sur une autres personne ou soi même! (num 5)
            joueursPossible = JoueursChoixPossible();
            if (carteEnCourDeJeux->estType() != TypeCarte::OffensifOuDefensif) // avec TypeCarte::OffensifOuDefensif on peut ce choisir soit même
                joueursPossible.removeOne(joueurActuel); // Pour ne pas ce viser soit même
            if (joueursPossible.isEmpty()){ // cas où on ne peut pas choisir de joueur
                emit messageLog(joueurActuel->avoirNom() + " joue la carte " + carteEnCourDeJeux->avoirNom() + " sur personne.");
                break;
            }
            for (short int indice = 0; indice < joueursPossible.size(); indice++){
                nomJoueursPossible.push_back(joueursPossible[indice]->avoirNom());
                idJoueursPossible.push_back(joueursPossible[indice]->avoirID());
            }
            emit demanderChoixCibleJoueur(nomJoueursPossible, idJoueursPossible);
            break;
            // la suite est différante pour Offensif / Duel / OffensifOuDefensif
        case TypeCarte::Defensif: // sur soi (num : 0,4,6,9) le 9 ne protège pas vraiment ;) mais ce joue sur soie même
        case TypeCarte::SansEffet: // ne fait rien (num 8)
            emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux));
            miseAJourCartesPotentiel(); // met à jour les connaisances des IA
            lancerTour(); // permet de continuer la partie
            break;
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

    // On affiche la liste des joueurs
    QVector<QString> nomJoueurs;
    for (Joueur* joueur : joueurs)
        nomJoueurs.push_back(joueur->avoirNom());
    emit initialiserListeJoueurs(nomJoueurs);

    // Affiche le nb de base de carte
    emit miseAJourCartesJouees(pioche.avoirCartesJouer());
    emit miseAJourNbCartesRestantes(pioche.avoirNbCartesRestantes());

    lancerManche();
}

