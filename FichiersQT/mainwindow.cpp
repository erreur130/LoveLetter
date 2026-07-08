#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ui_affichageactionscartes.h"
#include "ui_InfoProjet.h"
#include "ui_affichageRegles.h"
#include "nbjoueurswindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), jeu(nullptr), joueursCible(QVector<QString>()), AFermer(false){
    ui->setupUi(this);
    setWindowTitle("Love Letter");

    // demande la répartition des joueurs
    NbJoueursWindow *fenetre = new NbJoueursWindow(this);
    connect(fenetre, SIGNAL(envoyerJoueur(short int, short int, short int, short int)), this, SLOT(recevoirJoueur(short int, short int, short int, short int)));
    fenetre->setFixedSize(fenetre->size());  // taille fixe basée sur la taille
    fenetre->setWindowTitle("Choix des joueurs");
    if (not(fenetre->exec() == QDialog::Accepted)){ // si la croix
        qDebug() << "close";
        jeu = new Jeu(this); // car non définit avant (sinon free nullptr)
        AFermer = true; // dit à main de fermer MainWindow
    }

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
    connect(jeu, &Jeu::afficherCarte, this, &MainWindow::recevoirAfficherCarte);
    // connection à tout les signals/slots MainWindow -> Jeu
    connect(this, &MainWindow::envoyerChoixCarte, jeu, &Jeu::recevoirChoixCarte);
    connect(this, &MainWindow::envoyerChoixValeurGarde, jeu, &Jeu::recevoirChoixValeurGarde);
    connect(this, &MainWindow::envoyerChoixCibleJoueur, jeu, &Jeu::recevoirChoixCibleJoueur);
    connect(this, &MainWindow::rejouer, jeu, &Jeu::rejouer);
    // connection à tout les signals/slots MainWindow -> Carte6 et Carte6 -> MainWindow
    connect(this, &MainWindow::envoyerSuiteAction6, dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]), &Carte6::suiteAction6);
    connect(dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]), &Carte6::choixCarteAGarder, this, &MainWindow::recevoirChoixCarteAGarder);
    connect(dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]), &Carte6::messageLog, this, &MainWindow::recevoirMessageLog);
    // connection  à tout les signals/slots Carte6 -> Jeu
    connect(dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]),  &Carte6::continuer, jeu, &Jeu::recevoirContinuer);


    // On cache les Wigets inutiles au début
    ui->listeCarteChoixGarde->setVisible(false);
    ui->listeCarteMain->setVisible(false);
    ui->listeJoueursCible->setVisible(false);
    ui->listeCarteAGarder->setVisible(false);
    ui->labelInfoActionJoueur->setVisible(false);
}

MainWindow::~MainWindow(){
    qDebug() << "~MainWindow";
    delete ui;
    //delete jeu;
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
    //qDebug() << h << inul << inorm << itri;
    jeu = new Jeu(this, h,inul ,inorm ,itri);
}

void MainWindow::lancer(){
    jeu->rejouer(); // démarre la première manche, premier tour et s'enchaine à la suite
}

// -----------------public slots---------------------- Jeu -> MainWindow

void MainWindow::recevoirChoixCarteAGarder(Joueur* joueurARenvoyer){
    ui->listeCarteAGarder->setVisible(true);
    ui->labelInfoActionJoueur->setVisible(true);
    ui->labelInfoActionJoueur->setText("Choisissez une carte à garder :");

    ui->listeCarteAGarder->clear();
    for (short int indice =0; indice < joueurARenvoyer->avoirMain().size(); indice++){
        QListWidgetItem* joueurItem = new QListWidgetItem(ui->listeCarteAGarder);
        joueurItem->setIcon(QIcon(joueurARenvoyer->avoirMain()[indice]->avoirImage()));
        joueurItem->setData(Qt::UserRole, indice);
        joueurItem->setData(Qt::UserRole + 1, QVariant::fromValue(joueurARenvoyer));
    }
}

void MainWindow::recevoirMessageLog(QString msg){
    ui->textLog->append(msg + "\n");
    qDebug() << msg + "\n"; // log dans le terminale ------------------------
}

void MainWindow::recevoirReinitialiserLog(){
    ui->textLog->setText("");
}

void MainWindow::recevoirDemanderChoixValeurGarde(QVector<Carte*> cartes, QVector<short int> cartesJouer){
    ui->listeCarteChoixGarde->setVisible(true);
    ui->labelInfoActionJoueur->setVisible(true);
    ui->labelInfoActionJoueur->setText("Choisissez une carte pour l'action du grade :");

    ui->listeCarteChoixGarde->clear();
    for (short int indice = 0; indice < cartes.size(); indice++){
        if (cartes.at(indice)->avoirNbExemplaires() > cartesJouer.at(indice) && indice != 1){ // si la carte est possible à demander
            QListWidgetItem* carteItem = new QListWidgetItem(ui->listeCarteChoixGarde);
            carteItem->setIcon(QIcon(cartes[indice]->avoirImage()));
            carteItem->setData(Qt::UserRole, cartes[indice]->avoirNum());
        }
    }
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
    ui->listeJoueursCible->setVisible(true);
    ui->labelInfoActionJoueur->setVisible(true);
    ui->labelInfoActionJoueur->setText("Choisissez un joueur à cibler :");

    ui->listeJoueursCible->clear();
    for (short int indice =0; indice < nomJoueurs.size(); indice++){
        QListWidgetItem* joueurItem = new QListWidgetItem(ui->listeJoueursCible);
        joueurItem->setText(nomJoueurs[indice]);
        joueurItem->setData(Qt::UserRole, idJoueurs[indice]);
    }
}

void MainWindow::recevoirMiseAJourPointsJoueurs(QVector<short int> ptJoueurs){
    for (short int indice = 0; indice < ptJoueurs.size(); indice++)
        ui->listeJoueurs->setItem(1, indice, new QTableWidgetItem(QString::number(ptJoueurs[indice])));
}

void MainWindow::recevoirMessageAlerteMainJoueurVasEtreMontre(QString nom, bool popup){
    ui->labelTourDe->setText("Tour de " + nom);
    if (popup){
        QMessageBox alerteMessage(this);
        alerteMessage.setWindowTitle("Tour suivant");
        alerteMessage.setText("C'est au tour de " + nom + " de jouer");
        alerteMessage.addButton("Continuer", QMessageBox::AcceptRole);
        alerteMessage.setStyleSheet("margin: auto;"); // tente de centrer
        alerteMessage.exec(); // bloquant, attend le clic
    }
}

void MainWindow::recevoirAfficherMain(Carte* carte1, Carte* carte2){
    ui->listeCarteMain->setVisible(true);
    ui->labelInfoActionJoueur->setVisible(true);
    ui->labelInfoActionJoueur->setText("Choisissez une carte de votre main :");

    ui->listeCarteMain->clear();
    QListWidgetItem* itemCarte1 = new QListWidgetItem(ui->listeCarteMain);
    itemCarte1->setIcon(QIcon(carte1->avoirImage()));
    itemCarte1->setData(Qt::UserRole, carte1->avoirNum());
    QListWidgetItem* itemCarte2 = new QListWidgetItem(ui->listeCarteMain);
    itemCarte2->setIcon(QIcon(carte2->avoirImage()));
    itemCarte2->setData(Qt::UserRole, carte2->avoirNum());

    // si on as la comptesse avec le prince ou le roi on doit sélectionner seullement la comptesse
    if (carte1->avoirNum() == 8 && (carte2->avoirNum() == 5 || carte2->avoirNum() == 7)) // comptesse = carte1
        itemCarte2->setFlags(Qt::NoItemFlags);
    else if (carte2->avoirNum() == 8 && (carte1->avoirNum() == 5 || carte1->avoirNum() == 7)) // comptesse = carte2
        itemCarte1->setFlags(Qt::NoItemFlags);
}

void MainWindow::recevoirAfficherVictoireManche(QVector<QString> nomJoueurs){
    if (nomJoueurs.isEmpty()) return; // Sécurité

    QMessageBox* alerteMessage = new QMessageBox(this);
    alerteMessage->setWindowTitle("Victoire manche !");

    QString message;
    for (short int indice = 0; indice < nomJoueurs.size() -1; indice++) // sauf le dernier
        message += nomJoueurs[indice] + ", ";
    message += nomJoueurs[nomJoueurs.size() - 1]; // On rajoute le dernier

    if (nomJoueurs.size() > 1)
        message += " ont gagné la manche !";
    else
        message += " a gagné la manche !";

    alerteMessage->setText(message);
    alerteMessage->addButton("Continuer", QMessageBox::AcceptRole);
    alerteMessage->setStyleSheet("margin: auto;"); // tente de centrer
    alerteMessage->exec(); // bloquant, attend le clic
}

void MainWindow::recevoirAfficherVictoireJeu(QVector<QString> nomJoueurs){
    if (nomJoueurs.isEmpty()) return; // Sécurité

    QMessageBox alerteMessage(this);
    alerteMessage.setWindowTitle("Victoire de la partie !");

    QString message;
    for (short int indice = 0; indice < nomJoueurs.size() -1; indice++) // sauf le dernier
        message += nomJoueurs[indice] + ", ";
    message += nomJoueurs[nomJoueurs.size() - 1]; // On rajoute le dernier

    if (nomJoueurs.size() > 1)
        message += " ont gagné la partie !";
    else
        message += " a gagné la partie !";

    alerteMessage.setText(message);
    alerteMessage.addButton("Quitter", QMessageBox::RejectRole);
    alerteMessage.addButton("Rejouer", QMessageBox::AcceptRole);
    alerteMessage.setStyleSheet("margin: auto;"); // tente de centrer
    alerteMessage.exec(); // bloquant, attend le clic

    if (alerteMessage.buttonRole(alerteMessage.clickedButton()) == QMessageBox::AcceptRole) // on rejoue si demander
        lancer();
    else{ // si boutton quitter ou croix on ferme
        QCoreApplication::sendEvent(qApp, new QEvent(QEvent::Quit));
        //close(); ne suprime pas se procésus de Jeu
    }
}

void MainWindow::recevoirMiseAJourCartesJouees(QVector<short int> listeCartes){
    ui->labelNbEspionne->setText(   QString::number(2 - listeCartes[0]) + " x Espionne");
    ui->labelNbGarde->setText(      QString::number(6 - listeCartes[1]) + " x Garde");
    ui->labelNbPretre->setText(     QString::number(2 - listeCartes[2]) + " x Prêtre");
    ui->labelNbBaron->setText(      QString::number(2 - listeCartes[3]) + " x Baron");
    ui->labelNbServante->setText(   QString::number(2 - listeCartes[4]) + " x Servante");
    ui->labelNbPrince->setText(     QString::number(2 - listeCartes[5]) + " x Prince");
    ui->labelNbChancelier->setText( QString::number(2 - listeCartes[6]) + " x Chancelier");
    ui->labelNbRoi->setText(        QString::number(1 - listeCartes[7]) + " x Roi");
    ui->labelNbComtesse->setText(   QString::number(1 - listeCartes[8]) + " x Comtesse");
    ui->labelNbPrincesse->setText(  QString::number(1 - listeCartes[9]) + " x Princesse");
}

void MainWindow::recevoirMiseAJourNbCartesRestantes(short int cartesRestantes){
    ui->labelNbCartesRestantes->setText("Cartes restantes : " + QString::number(cartesRestantes));
}

void MainWindow::recevoirAfficherCarte(Carte* carte){
    // affiche une popup avec un boutton et la carte
    QDialog* popup = new QDialog();
    popup->setWindowTitle("Voici la carte du joueur");

    QVBoxLayout* layout = new QVBoxLayout(popup);

    QLabel* image = new QLabel();
    image->setPixmap(QPixmap(carte->avoirImage()).scaled(240, 335, Qt::KeepAspectRatio));
    image->setAlignment(Qt::AlignCenter);
    layout->addWidget(image);
    // Le bouton
    QPushButton* bouton = new QPushButton("OK, j'ai vu la carte");
    connect(bouton, &QPushButton::clicked, popup, &QDialog::accept);
    layout->addWidget(bouton);

    popup->setLayout(layout);
    popup->exec(); // bloquant
    delete popup;
}

// ---------------------------- ui -------------------------------------

void MainWindow::on_listeCarteChoixGarde_itemClicked(QListWidgetItem *item){
    ui->listeCarteChoixGarde->setVisible(false);
    ui->labelInfoActionJoueur->setVisible(false);

    emit envoyerChoixValeurGarde(item->data(Qt::UserRole).toInt());
}


void MainWindow::on_listeJoueursCible_itemClicked(QListWidgetItem *item){
    ui->listeJoueursCible->setVisible(false);
    ui->labelInfoActionJoueur->setVisible(false);

    emit envoyerChoixCibleJoueur(item->data(Qt::UserRole).toInt());
}


void MainWindow::on_listeCarteMain_itemClicked(QListWidgetItem *item){
    if (!(item->flags() == Qt::NoItemFlags)){ // Si on ne doit pas sélectionner la carte, alors on ne fait rien
        ui->listeCarteMain->setVisible(false);
        ui->labelInfoActionJoueur->setVisible(false);

        emit envoyerChoixCarte(item->data(Qt::UserRole).toInt());
    }
}


void MainWindow::on_listeCarteAGarder_itemClicked(QListWidgetItem *item){
    ui->listeCarteAGarder->setVisible(false);
    ui->labelInfoActionJoueur->setVisible(false);

    emit envoyerSuiteAction6(item->data(Qt::UserRole + 1).value<Joueur*>(), item->data(Qt::UserRole).toInt(), true);
}

