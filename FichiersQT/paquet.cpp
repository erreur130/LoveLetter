#include "paquet.h"

Paquet::Paquet() : cartes(QQueue<Carte*>()), cartesJouer(QVector<short int>(10, 0)) {
    // On créé les dif cartes ici et on mélange -------------------------------------
}

Paquet::~Paquet() {}
