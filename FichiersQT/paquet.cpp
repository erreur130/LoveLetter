#include "paquet.h"

Paquet::Paquet() : cartes(QQueue<Carte*>()), cartesJouer(QVector<short int>(10, 0)), listeDesCartes(QVector<Carte*>()) {
    // On créé les dif cartes ici et on les mélanges
    listeDesCartes.push_back(new Carte0(        2 /*nbExemplaires*/, 0 /*num*/, "Espionne"  , ":/Images/Espionne.png"   , TypeCarte::Defensif));
    listeDesCartes.push_back(new Carte1(        6 /*nbExemplaires*/, 1 /*num*/, "Garde"     , ":/Images/Garde.png"      , TypeCarte::Offensif));
    listeDesCartes.push_back(new Carte2(        2 /*nbExemplaires*/, 2 /*num*/, "Prêtre"    , ":/Images/Prêtre.png"     , TypeCarte::Duel));
    listeDesCartes.push_back(new Carte3(        2 /*nbExemplaires*/, 3 /*num*/, "Baron"     , ":/Images/Baron.png"      , TypeCarte::Duel));
    listeDesCartes.push_back(new Carte4(        2 /*nbExemplaires*/, 4 /*num*/, "Servante"  , ":/Images/Servante.png"   , TypeCarte::Defensif));
    listeDesCartes.push_back(new Carte5(        2 /*nbExemplaires*/, 5 /*num*/, "Prince"    , ":/Images/Prince.png"     , TypeCarte::OffensifOuDefensif , this));
    listeDesCartes.push_back(new Carte6(nullptr,2 /*nbExemplaires*/, 6 /*num*/, "Chancelier", ":/Images/Chancelier.png" , TypeCarte::Defensif           , this));
    listeDesCartes.push_back(new Carte7(        1 /*nbExemplaires*/, 7 /*num*/, "Roi"       , ":/Images/Roi.png"        , TypeCarte::Duel));
    listeDesCartes.push_back(new Carte8(        1 /*nbExemplaires*/, 8 /*num*/, "Comtesse"  , ":/Images/Comtesse.png"   , TypeCarte::SansEffet));
    listeDesCartes.push_back(new Carte9(        1 /*nbExemplaires*/, 9 /*num*/, "Princesse" , ":/Images/Princesse.png"  , TypeCarte::Defensif));

    // On mélange et on remplit
    remplir();
}

Paquet::~Paquet() {
    for(short int indice =0; indice < listeDesCartes.size(); indice++)
        delete listeDesCartes[indice];
}

Carte* Paquet::piocher(){
    return cartes.dequeue();
}

void Paquet::defausser(Carte* carte){
    cartes.enqueue(carte);
}

void Paquet::remplir(){
    // On vide si y reste des cartes
    while(not(cartes.isEmpty()))
        cartes.dequeue();

    // On rajoute les cartes
    for (Carte* carte : listeDesCartes){
        for (short int indice = 0; indice < carte->avoirNbExemplaires(); indice++)
            cartes.enqueue(carte);
    }

    // On mélange les cartes
    for (int i = cartes.size() - 1; i > 0; --i) {
        int j = QRandomGenerator::global()->bounded(i + 1);
        cartes.swapItemsAt(i, j); // échange les cartes entre 2 endroit aléatoire
    }
}