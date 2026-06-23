#include "joueur.h"
#include "carte.h"

Joueur::Joueur(QString nom) : nom(nom), points(0), pointBonusManche(0), protection(0), enVie(0), main(QVector<Carte*>()){}
