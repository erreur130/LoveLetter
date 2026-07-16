#include "jeu.h"
#include <QDebug>

Jeu::Jeu(QObject* parent)
    :QObject(parent), objectifPoints(0), joueurs(QVector<Joueur*>()), joueurActuel(nullptr), joueurCible(nullptr), carteEnCourDeJeux(nullptr), pioche(Paquet())
{}

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
    for (short int indice = joueurs.size() - 1; indice > 0; --indice){
        int nb = QRandomGenerator::global()->bounded(indice + 1);
        joueurs.swapItemsAt(indice, nb); // échange les cartes entre 2 endroit aléatoire
    }
    // On leur donne la bonne id
    for (short int indice = 0; indice < joueurs.size(); indice++){
        joueurs[indice]->changerID(indice);
    }

    joueurActuel = joueurs[0]; // On choisis le premier joueur

}

Jeu::~Jeu(){
    joueurActuel = nullptr;
    joueurCible = nullptr;
    carteEnCourDeJeux = nullptr;

    qDebug() << "~Jeu, nb joueurs à détruire :" << joueurs.size();
    for (short int indice = 0; indice < joueurs.size(); indice++){
        qDebug() << "delete joueur" << indice;
        delete joueurs[indice];
    }
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

    if (pioche.avoirNbCartesRestantes() > 0){ // si il y a encore des cartes
        if (JoueursRestant().size() > 1){ // si plusieurs joueurs en jeu
            // ici on peut continuer, donc on cherche le joueur suivant:
            // en partant de l'ID actuel + 1, et en sautant les morts
            short int idActuel = joueurActuel->avoirID();

            for (short int i = 1; i <= joueurs.size(); i++){
                short int idSuivant = (idActuel + i) % joueurs.size(); // indice du suivant
                if (joueurs[idSuivant]->estEnVie()){ // on vérifie si c'est possible
                    joueurActuel = joueurs[idSuivant];
                    return true;
                }
            }
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
    short int maxi = objectifPoints;
    for (short int indice = 0; indice < joueurs.size(); indice++)
        if (joueurs[indice]->avoirPoints() >= objectifPoints)
            if (joueurs[indice]->avoirPoints() > maxi)
                maxi = joueurs[indice]->avoirPoints();
    // tri au max
    QVector<Joueur*> result;
    for (short int indice = 0; indice < joueurs.size(); indice++)
        if (joueurs[indice]->avoirPoints() == maxi)
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

    joueurActuel = joueurs[joueurs.size()-1]; // on remet le premier joueur (toursuivant vas faire revenir à 0)

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
    // vérifie si quelqu'un est mort au dernier tour il faut dire au paquet que sa carte est vue (visible)
    if (not(joueurActuel->estEnVie())) // joueurActuel est mort
        pioche.carteAEtaitJouer(joueurActuel->avoirMain()[0]->avoirNum());
    else if (joueurCible != nullptr && not(joueurCible->estEnVie())) // joueurCible est mort
        pioche.carteAEtaitJouer(joueurCible->avoirMain()[0]->avoirNum());

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
            qDebug() << "Début tour Joueur";
            emit messageAlerteMainJoueurVasEtreMontre(joueurActuel->avoirNom(), 1);
            emit afficherMain(joueurActuel->avoirMain()[0], joueurActuel->avoirMain()[1]);
            return;

        } else { // Si ia on fait la suite de calcules (tout d'un coups)
            qDebug() << "Début tour IA";
            emit messageAlerteMainJoueurVasEtreMontre(joueurActuel->avoirNom(), 0);
            carteEnCourDeJeux = choixCarte;

            // On retire la carte jouée de la main du joueur avant de faire l'action (considère que la carte est focément dans sa main)
            if (joueurActuel->avoirMain().at(0)->avoirNum() == choixCarte->avoirNum())
                joueurActuel->retirerCarte(0);
            else
                joueurActuel->retirerCarte(1);

            pioche.carteAEtaitJouer(carteEnCourDeJeux->avoirNum()); // On signale à la pioche que cette carte à était jouer (visible)
            short int carteADemander = -1;
            short int cible = -1;
            // les fonctions font en sorte de retirer this du choix des joueurs, donc on ne modifie pas joueursChoix
            switch (carteEnCourDeJeux->estType()){
                case TypeCarte::Offensif: // sur autre personne et le choix d'une carte (num : 1)
                    qDebug() << joueurActuel->avoirNom() << "choisirJoueur avec joueursChoix :" << joueursChoix;
                    cible = joueurActuel->choisirJoueur(carteEnCourDeJeux, joueursChoix, pioche.avoirNbCartesRestantes());
                    qDebug() << "cible retournée :" << cible;
                    if (cible == -1){ // cas où on ne peut pas choisir de joueur
                        emit messageLog(joueurActuel->avoirNom() + " joue la carte " + carteEnCourDeJeux->avoirNom() + " [1] sur personne.");
                        miseAJourCartesPotentiel(); // met à jour les connaisances des IA, version normale
                        break;
                    }
                    joueurCible = joueurs[cible];
                    carteADemander = joueurActuel->demanderCarteAJoueur(joueurCible, pioche.avoirCartesJouer());
                    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible, pioche[carteADemander]));
                    miseAJourCartesPotentiel(pioche[carteADemander]); // il faut la carte demander pour la mise à jour
                    break;

                case TypeCarte::Duel: // sur une autres personne et soi même (num : 2,3,7) / num 2 car la personne qui la joue regarde l'autre
                case TypeCarte::OffensifOuDefensif: // sur une autres personne et soi même! (num 5)
                    qDebug() << joueurActuel->avoirNom() << "choisirJoueur avec joueursChoix :" << joueursChoix;
                    cible = joueurActuel->choisirJoueur(carteEnCourDeJeux, joueursChoix, pioche.avoirNbCartesRestantes());
                    qDebug() << "cible retournée :" << cible;
                    if (cible == -1){ // cas où on ne peut pas choisir de joueur
                        emit messageLog(joueurActuel->avoirNom() + " joue la carte " + carteEnCourDeJeux->avoirNom() + " [" + QString::number(carteEnCourDeJeux->avoirNum()) + "] sur personne.");
                    } else {
                        joueurCible = joueurs[cible];
                        emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible));
                    }
                    miseAJourCartesPotentiel(); // met à jour les connaisances des IA, version normale
                    break;

                case TypeCarte::Defensif: // sur soi (num : 0,4,6,9) le 9 ne protège pas vraiment ;) mais ce joue sur soie même
                case TypeCarte::SansEffet: // ne fait rien (num 8)
                    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux));
                    miseAJourCartesPotentiel(); // met à jour les connaisances des IA
            }

            // Vérification de l'invariant : tout joueur vivant doit avoir exactement 1 carte -----------------------------------------------------------------------------
            for (Joueur* joueur : joueurs){
                if (joueur->estEnVie()){
                    if (joueur->avoirMain().isEmpty())
                        qDebug() << "BUG : " << joueur->avoirNom() << " est vivant mais n'a pas de carte !";
                    else if (joueur->avoirMain().size() > 1)
                        qDebug() << "BUG : " << joueur->avoirNom() << " a trop de cartes :" << joueur->avoirMain().size();
                }
            }

            QTimer::singleShot(0, this, &Jeu::lancerTour); // de façon asynchrone // permet de continuer la partie // permet de continuer pour le tour d'une IA
        }
    } else {
        emit miseAJourCartesJouees(pioche.avoirCartesJouer());
        finDeManche(pioche.avoirNbCartesRestantes() <= 0);
    }
}

void Jeu::finDeManche(bool finDuPaquet){
    if(finDuPaquet && JoueursRestant().size() > 1){ // Si fin de la manche par la fin du paquet et qu'il y a plus d'une persone, on garde les joueurs avec la plus grosse carte
        emit messageLog("La pioche est vide, on confronte les joueurs en lice :");
        short int numMax = 0;
        // pour ceux qui sont en vie :
        for (Joueur* joueur : joueurs)
            if (joueur->estEnVie()){ // si plus grand on le note + affichage de la personne
                emit messageLog(joueur->avoirNom() + " à la carte " + joueur->avoirMain()[0]->avoirNom() + " [" + QString::number(joueur->avoirMain().at(0)->avoirNum()) + "]");
                if (joueur->avoirMain().at(0)->avoirNum() > numMax)
                    numMax = joueur->avoirMain()[0]->avoirNum();
            }
        for (Joueur* joueur : joueurs)
            if(joueur->estEnVie() && joueur->avoirMain().at(0)->avoirNum() < numMax) // si plus petit que le max alors il meurt
                joueur->eliminer();
    } else { // sinon on affiche la carte du gagnant
        emit messageLog("Il ne reste plus qu'un seul joueur en lice :");
        for (Joueur* joueur : joueurs)
            if (joueur->estEnVie())
                emit messageLog(joueur->avoirNom() + " à la carte " + joueur->avoirMain()[0]->avoirNom() + " [" + QString::number(joueur->avoirMain().at(0)->avoirNum()) + "]");
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
        if (joueur->estEnVie())
            nomJoueurs.push_back(joueur->avoirNom()); // On le listes
        joueur->gainPoints(); // On donne les points (gainPoints donne des pt si enVie et si ptBonnus)
        pointsJoueurs.push_back(joueur->avoirPoints()); // met à jours l'affichage des points
    }
    qDebug() << "Fin de manche";
    emit miseAJourPointsJoueurs(pointsJoueurs);
    emit afficherVictoireManche(nomJoueurs);

    QVector<Joueur*> gagnants = verifSiGagnants();
    if(gagnants.empty()) // si pas de gagant on continue
        lancerManche();
    else { // Sinon on arrête
        QVector<QString> nomGagnants;
        for (Joueur* joueur : gagnants)
            nomGagnants.push_back(joueur->avoirNom());
        emit afficherVictoireJeu(nomGagnants);
    }
}

// ---------------------------slots------------------------------------------- MainWindow -> Jeu

void Jeu::recevoirChoixCarte(short int idCarte){
    carteEnCourDeJeux = pioche[idCarte]; // On note la carte choisit
    pioche.carteAEtaitJouer(idCarte); // On signale à la pioche que cette carte à était jouer (visible)
    // On change l'affichaque de ce qui reste en cartes
    emit miseAJourCartesJouees(pioche.avoirCartesJouer());

    if (joueurActuel->avoirMain().at(0)->avoirNum() == idCarte)
        joueurActuel->retirerCarte(0);
    else
        joueurActuel->retirerCarte(1);

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
                emit messageLog(joueurActuel->avoirNom() + " joue la carte " + carteEnCourDeJeux->avoirNom() + " [" + QString::number(carteEnCourDeJeux->avoirNum()) + "] sur personne.");
                // Vérification de l'invariant : tout joueur vivant doit avoir exactement 1 carte -----------------------------------------------------------------------------
                for (Joueur* joueur : joueurs){
                    if (joueur->estEnVie()){
                        if (joueur->avoirMain().isEmpty())
                            qDebug() << "BUG : " << joueur->avoirNom() << " est vivant mais n'a pas de carte !";
                        else if (joueur->avoirMain().size() > 1)
                            qDebug() << "BUG : " << joueur->avoirNom() << " a trop de cartes :" << joueur->avoirMain().size();
                    }
                }
                QTimer::singleShot(0, this, &Jeu::lancerTour); // de façon asynchrone // permet de continuer la partie // permet de continuer la partie
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
            if (carteEnCourDeJeux->avoirNum() != 6){ // pour le chancelier on vas demander la carte à garder, lancerTour() est autre part
                // Vérification de l'invariant : tout joueur vivant doit avoir exactement 1 carte -----------------------------------------------------------------------------
                for (Joueur* joueur : joueurs){
                    if (joueur->estEnVie()){
                        if (joueur->avoirMain().isEmpty())
                            qDebug() << "BUG : " << joueur->avoirNom() << " est vivant mais n'a pas de carte !";
                        else if (joueur->avoirMain().size() > 1)
                            qDebug() << "BUG : " << joueur->avoirNom() << " a trop de cartes :" << joueur->avoirMain().size();
                    }
                }
                QTimer::singleShot(0, this, &Jeu::lancerTour); // de façon asynchrone // permet de continuer la partie
            }
            break;
    }
}

void Jeu::recevoirChoixValeurGarde(short int valeur){
    emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux /*num 1*/, joueurCible, pioche[valeur]));
    miseAJourCartesPotentiel(pioche[valeur]); // met à jour les connaisances des IA

    // Vérification de l'invariant : tout joueur vivant doit avoir exactement 1 carte -----------------------------------------------------------------------------
    for (Joueur* joueur : joueurs){
        if (joueur->estEnVie()){
            if (joueur->avoirMain().isEmpty())
                qDebug() << "BUG : " << joueur->avoirNom() << " est vivant mais n'a pas de carte !";
            else if (joueur->avoirMain().size() > 1)
                qDebug() << "BUG : " << joueur->avoirNom() << " a trop de cartes :" << joueur->avoirMain().size();
        }
    }
    QTimer::singleShot(0, this, &Jeu::lancerTour); // de façon asynchrone // permet de continuer la partie // de façon asynchrone // permet de continuer la partie(); // permet de continuer la partie
}

void Jeu::recevoirChoixCibleJoueur(short int joueur){
    joueurCible = joueurs[joueur]; // On note la cible choisit

    // soit Offensif / Duel / OffensifOuDefensif
    switch (carteEnCourDeJeux->estType()) {
        case TypeCarte::Offensif: // sur autre personne et le choix d'une carte (num : 1)
            emit demanderChoixValeurGarde(pioche.avoirListeDesCartes(), pioche.avoirCartesJouer());
            break;
        case TypeCarte::Duel: // sur une autres personne et soi même (num : 2,3,7) / num 2 car la personne qui la joue regarde l'autre
        case TypeCarte::OffensifOuDefensif: // sur une autres personne et soi même! (num 5)
            emit messageLog(joueurActuel->jouerCarte(carteEnCourDeJeux, joueurCible));
            miseAJourCartesPotentiel(); // met à jour les connaisances des IA

            if(carteEnCourDeJeux->avoirNum() == 2) // si prêtre (num 2) on affiche la carte de l'autre
                emit afficherCarte(joueurCible->avoirMain()[0]);
            else if (carteEnCourDeJeux->avoirNum() == 7) // si Roi (num 7) on affiche la carte que l'on a récupéré
                emit afficherCarte(joueurActuel->avoirMain()[0]);

            // Vérification de l'invariant : tout joueur vivant doit avoir exactement 1 carte -----------------------------------------------------------------------------
            for (Joueur* joueur : joueurs){
                if (joueur->estEnVie()){
                    if (joueur->avoirMain().isEmpty())
                        qDebug() << "BUG : " << joueur->avoirNom() << " est vivant mais n'a pas de carte !";
                    else if (joueur->avoirMain().size() > 1)
                        qDebug() << "BUG : " << joueur->avoirNom() << " a trop de cartes :" << joueur->avoirMain().size();
                }
            }
            QTimer::singleShot(0, this, &Jeu::lancerTour); // de façon asynchrone // permet de continuer la partie // permet de continuer la partie
            break;
        default:
            break;
    }
}

void Jeu::rejouer(){
    if (not(joueurs.isEmpty())){ // sécurité si on quite avant la sélection des joueurs
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
}

void Jeu::recevoirContinuer(){
    // Vérification de l'invariant : tout joueur vivant doit avoir exactement 1 carte -----------------------------------------------------------------------------
    for (Joueur* joueur : joueurs){
        if (joueur->estEnVie()){
            if (joueur->avoirMain().isEmpty())
                qDebug() << "BUG : " << joueur->avoirNom() << " est vivant mais n'a pas de carte !";
            else if (joueur->avoirMain().size() > 1)
                qDebug() << "BUG : " << joueur->avoirNom() << " a trop de cartes :" << joueur->avoirMain().size();
        }
    }
    QTimer::singleShot(0, this, &Jeu::lancerTour); // de façon asynchrone // permet de continuer la partie
}
