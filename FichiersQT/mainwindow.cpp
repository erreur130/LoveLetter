#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ui_affichageactionscartes.h"
#include "ui_InfoProjet.h"
#include "ui_affichageRegles.h"
#include "nbjoueurswindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), jeu(nullptr), joueursCible(QVector<QString>()){
    ui->setupUi(this);
    setWindowTitle("Love Letter");

    // demande la répartition des joueurs
    NbJoueursWindow *fenetre = new NbJoueursWindow(this);
    connect(fenetre, SIGNAL(envoyerJoueur(short int, short int, short int, short int)), this, SLOT(recevoirJoueur(short int, short int, short int, short int)));
    fenetre->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint); // retire la croix et pein écrant
    fenetre->setFixedSize(fenetre->size());  // taille fixe basée sur la taille
    fenetre->setWindowTitle("Choix des joueurs");
    fenetre->exec(); // bloque MainWindow

    // connection à tout les signals/slots Jeu -> MainWindow
    connect(jeu, &Jeu::messageLog, this, &MainWindow::recevoirMessageLog);
    connect(jeu, &Jeu::reinitialiserLog, this, &MainWindow::recevoirReinitialiserLog);
    connect(jeu, &Jeu::demanderChoixValeurGarde, this, &MainWindow::recevoirDemanderChoixValeurGarde);
    connect(jeu, &Jeu::initialiserListeJoueurs, this, &MainWindow::recevoirInitialiserListeJoueurs);
    connect(jeu, &Jeu::demanderChoixCibleJoueur, this, &MainWindow::recevoirDemanderChoixCibleJoueur);
    connect(jeu, &Jeu::miseAJourPointsJoueurs, this, &MainWindow::recevoirMiseAJourPointsJoueurs);
    connect(jeu, &Jeu::messageAlerteMainJoueurVasEtreMontre, this, &MainWindow::recevoirMessageAlerteMainJoueurVasEtreMontre);
    connect(jeu, &Jeu::afficherMain, this, &MainWindow::recevoirAfficherMain);
    connect(jeu, &Jeu::afficherVictoireManche, this, &MainWindow::recevoirAfficherVictoireManche);
    connect(jeu, &Jeu::afficherVictoireJeu, this, &MainWindow::recevoirAfficherVictoireJeu);
    connect(jeu, &Jeu::miseAJourCartesJouees, this, &MainWindow::recevoirMiseAJourCartesJouees);
    connect(jeu, &Jeu::miseAJourNbCartesRestantes, this, &MainWindow::recevoirMiseAJourNbCartesRestantes);
    // connection à tout les signals/slots MainWindow -> Jeu
    connect(this, &MainWindow::envoyerChoixCarte, jeu, &Jeu::recevoirChoixCarte);
    connect(this, &MainWindow::envoyerChoixValeurGarde, jeu, &Jeu::recevoirChoixValeurGarde);
    connect(this, &MainWindow::envoyerChoixCibleJoueur, jeu, &Jeu::recevoirChoixCibleJoueur);
    connect(this, &MainWindow::rejouer, jeu, &Jeu::rejouer);
    // connection à tout les signals/slots MainWindow -> Carte6 et Carte6 -> MainWindow
    connect(this, &MainWindow::envoyerSuiteAction6, dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]), &Carte6::suiteAction6);
    connect(dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]), &Carte6::choixCarteAGarder, this, &MainWindow::recevoirChoixCarteAGarder);


    // On cache les Wigets inutiles au début
    ui->listeCarteChoixGarde->setVisible(false);
    ui->listeCarteMain->setVisible(false);
    ui->listeJoueursCible->setVisible(false);
    ui->buttonRejouer->setVisible(false);
    ui->labelInfoActionJoueur->setVisible(false);
}

MainWindow::~MainWindow(){
    delete ui;
    delete jeu;
}

void MainWindow::on_actionRegles_de_jeu_triggered(){
    QDialog *fenetre = new QDialog();
    Ui::AffichageRegles ui;
    ui.setupUi(fenetre);
    fenetre->setWindowFlags(Qt::Window);  // force que ce soit une fenêtre indépendante
    fenetre->setWindowTitle(" ");
    fenetre->show();
}


void MainWindow::on_actionListe_des_cartes_triggered(){
    QWidget *fenetre = new QWidget();
    Ui::AffichageActionsCartes ui;
    ui.setupUi(fenetre);
    fenetre->setWindowFlags(Qt::Window);  // force que ce soit une fenêtre indépendante
    fenetre->setWindowTitle(" ");
    fenetre->show();
}


void MainWindow::on_actionProjet_triggered(){
    QDialog *fenetre = new QDialog();
    Ui::InfoProjet  ui;
    ui.setupUi(fenetre);
    fenetre->setWindowFlags(Qt::Window);  // force que ce soit une fenêtre indépendante
    fenetre->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint); // retire la croix et pein écrant
    fenetre->setFixedSize(fenetre->size());  // taille fixe basée sur la taille
    fenetre->setWindowTitle(" ");
    fenetre->show();
}

void MainWindow::recevoirJoueur(short int h, short int inul, short int inorm, short int itri){
    qDebug() << h << inul << inorm << itri;
    jeu = new Jeu(this, h,inul ,inorm ,itri);
}

void MainWindow::lancer(){
    jeu->rejouer(); // démarre la première manche, premier tour et s'enchaine à la suite
}

// -----------------public slots---------------------- Jeu -> MainWindow

void MainWindow::recevoirChoixCarteAGarder(Joueur* joueurARenvoyer){ //-----------------------------------------------------

}

void MainWindow::recevoirMessageLog(QString msg){
    ui->textLog->append(msg);
}

void MainWindow::recevoirReinitialiserLog(){
    //ui->textLog->setText(""); // à remettre après la phase de débug
}

void MainWindow::recevoirDemanderChoixValeurGarde(){

}

void MainWindow::recevoirInitialiserListeJoueurs(QVector<QString> nomJoueurs){
    ui->listeJoueurs->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listeJoueurs->setColumnCount(nomJoueurs.size());
    ui->listeJoueurs->setRowCount(2);

    for (short int indice = 0; indice < nomJoueurs.size(); indice++){ // On liste les joueurs avec leur nom et points (0 au début)
        ui->listeJoueurs->setItem(0, indice, new QTableWidgetItem(nomJoueurs[indice]));
        ui->listeJoueurs->setItem(1, indice, new QTableWidgetItem("0"));
    }
}

void MainWindow::recevoirDemanderChoixCibleJoueur(QVector<QString> nomJoueurs, QVector<short int> idJoueurs){

}

void MainWindow::recevoirMiseAJourPointsJoueurs(QVector<short int>){

}

void MainWindow::recevoirMessageAlerteMainJoueurVasEtreMontre(QString){

}

void MainWindow::recevoirAfficherMain(QString imageCarte1, QString imageCarte2){

}

void MainWindow::recevoirAfficherVictoireManche(QVector<QString> nomJoueurs){

}

void MainWindow::recevoirAfficherVictoireJeu(QVector<QString> nomJoueurs){

}

void MainWindow::recevoirMiseAJourCartesJouees(QVector<short int> listeCartes){

}

void MainWindow::recevoirMiseAJourNbCartesRestantes(short int cartesRestantes){

}

