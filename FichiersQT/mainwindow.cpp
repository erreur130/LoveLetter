#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "affichageactionscartes.h"
#include "nbjoueurswindow.h"
#include "QDebug"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setWindowTitle("Love Letter");

    NbJoueursWindow *fenetre = new NbJoueursWindow(this);
    connect(fenetre, SIGNAL(envoyerJoueur(short int, short int, short int)), this, SLOT(recevoirJoueur(short int, short int, short int)));
    fenetre->setWindowFlags(Qt::Window | Qt::CustomizeWindowHint | Qt::WindowMinimizeButtonHint); // retire la croix et pein écrant
    fenetre->setFixedSize(fenetre->size());  // taille fixe basée sur la taille
    fenetre->setWindowTitle("Choix des joueurs");
    fenetre->exec(); // bloque MainWindow

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionReges_de_jeu_triggered(){

}


void MainWindow::on_actionListe_des_cartes_triggered(){
    AffichageActionsCartes *fenetre = new AffichageActionsCartes(this);
    fenetre->setWindowFlags(Qt::Window);  // force que ce soit une fenêtre indépendante
    fenetre->show();
}


void MainWindow::on_actionProjet_triggered(){

}

void MainWindow::recevoirJoueur(short int h, short int i, short int ii){
    qDebug() << h << i << ii;
}

