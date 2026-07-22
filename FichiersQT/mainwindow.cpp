#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ui_affichageactionscartes.h"
#include "ui_InfoProjet.h"
#include "ui_affichageRegles.h"
#include "nbjoueurswindow.h"
#include "choixnomjoueur.h"

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
    connect(jeu, &Jeu::demanderChangementNom, this, &MainWindow::recevoirDemanderChangementNom);
    connect(jeu, &Jeu::afficherJoueursEliminer, this, &MainWindow::recevoirJoueursEliminer);
    // connection à tout les signals/slots MainWindow -> Jeu
    connect(this, &MainWindow::envoyerChoixCarte, jeu, &Jeu::recevoirChoixCarte);
    connect(this, &MainWindow::envoyerChoixValeurGarde, jeu, &Jeu::recevoirChoixValeurGarde);
    connect(this, &MainWindow::envoyerChoixCibleJoueur, jeu, &Jeu::recevoirChoixCibleJoueur);
    connect(this, &MainWindow::rejouer, jeu, &Jeu::rejouer);
    connect(this, &MainWindow::changementNom, jeu, &Jeu::recevoirChangementNom);
    // connection à tout les signals/slots MainWindow -> Carte6 et Carte6 -> MainWindow
    connect(this, &MainWindow::envoyerSuiteAction6, dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]), &Carte6::suiteAction6);
    connect(dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]), &Carte6::choixCarteAGarder, this, &MainWindow::recevoirChoixCarteAGarder);
    connect(dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]), &Carte6::messageLog, this, &MainWindow::recevoirMessageLog);
    // connection  à tout les signals/slots Carte6 -> Jeu
    connect(dynamic_cast<Carte6*>(jeu->avoirPaquet()[6]),  &Carte6::continuer, jeu, &Jeu::recevoirContinuer);


    // On cache les Wigets inutiles au début
    ui->labelInfoActionJoueur->setVisible(false);

    // On demande le nom des joueurs
    if (not(AFermer)) // si on ne doit pas avotré le jeu
        jeu->changerNomsJoueurs();

    // Puis on les mélange pour pas faire des groupes de type de Joueurs
    jeu->melangerJoueurs();
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

void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event); //pour conserver le comportement par défaut

    // On redimentionne toutes les cartes

    // Les cartes pour le choix du garde:
    {
        int nbBtn = ui->listeCarteChoixGarde->count();
        if (nbBtn != 0){
            // on calcule le multiplicateur de taille en fonction de la fenetre
            double multHauteur = ((ui->verticalLayout_5->geometry().height() - 6) /2.0) / 290.0; // -6 = marge       /2 car 2 carte sur la hauteur
            double multLargeur = ((ui->verticalLayout_5->geometry().width() - 6*4) /5.0) / 210.0; // *4 = 4 marges       /5 car 5 carte sur la hauteur
            double mult = (multHauteur<multLargeur)?multHauteur:multLargeur; // pour ajouster la taille des cartes on prend le multiplicateur le plus petit entre la largeur et hauteur
            for (short int indice = 0; indice < nbBtn; indice++){ // pour chaque carte on change leurs tailles
                QWidget* widget = ui->listeCarteChoixGarde->itemAt(indice)->widget();
                if (widget){ // si possible
                    QPushButton* btnCarte = qobject_cast<QPushButton*>(widget);
                    if (btnCarte){ // si la convertion à marché
                        btnCarte->setIconSize(QSize(200*mult, 285*mult)); //  taille de l'icône
                        btnCarte->setFixedSize(QSize(210*mult, 290*mult)); // Taille du boutton
                    }
                }
            }
        }
    }
    // Les cartes pour le choix du chancelier :
    {
        int nbBtn = ui->listeCarteAGarder->count();
        if (nbBtn != 0){
            // on calcule le multiplicateur de taille en fonction de la fenetre
            double multHauteur = ui->verticalLayout_5->geometry().height() / 290.0; // Une seul ligne
            double multLargeur = ((ui->verticalLayout_5->geometry().width() - 6*(nbBtn-1)) / nbBtn) / 210.0; // en fonction du nb de carte qui seront montrées
            double mult = (multHauteur<multLargeur)?multHauteur:multLargeur; // pour ajouster la taille des cartes on prend le multiplicateur le plus petit entre la largeur et hauteur
            for (short int indice = 0; indice < nbBtn; indice++){ // pour chaque carte on change leurs tailles
                QWidget* widget = ui->listeCarteAGarder->itemAt(indice)->widget();
                if (widget){ // si possible
                    QPushButton* btnCarte = qobject_cast<QPushButton*>(widget);
                    if (btnCarte){ // si la convertion à marché
                        btnCarte->setIconSize(QSize(200*mult, 285*mult)); //  taille de l'icône
                        btnCarte->setFixedSize(QSize(210*mult, 290*mult)); // Taille du boutton
                    }
                }
            }
        }
    }
    // Les cartes de la main :
    if(ui->listeCarteMain->count() != 0){
        // calcule pour la taille de l'affichage des cartes de la main
        double multHauteur = ui->verticalLayout_5->geometry().height() / 290.0;
        double multLargeur = ((ui->verticalLayout_5->geometry().width() - 6) /2.0) / 210.0; // -6 = 1 marges       /2.0 car 2 carte sur la largeur
        double mult = (multHauteur<multLargeur)?multHauteur:multLargeur; // pour ajouster la taille des cartes on prend le multiplicateur le plus petit entre la largeur et hauteur
        ui->listeCarteMain->setIconSize(QSize(200*mult,285*mult));
        ui->listeCarteMain->setFixedSize(QSize(200*mult*2+13,285*mult+4)); // +13 et + 4 car y a des margre (j'ai pas fait de calcule, mais comme ça c'est plutôt clean)
        for (int i = 0; i < ui->listeCarteMain->count(); i++){ // *2 mais for() au cas où
            QListWidgetItem* item = ui->listeCarteMain->item(i);
            if (item != nullptr){
                short int numCarte = item->data(Qt::UserRole).toInt();
                item->setIcon(QIcon(QPixmap(jeu->avoirPaquet()[numCarte]->avoirImage()).scaled(QSize(200*mult,285*mult), Qt::IgnoreAspectRatio, Qt::SmoothTransformation))); // permet de faire plus grand que la taille originale
            }
        }
    }
}

// -----------------public slots---------------------- Jeu -> MainWindow

void MainWindow::recevoirChoixCarteAGarder(Joueur* joueurARenvoyer){
    ui->labelInfoActionJoueur->setVisible(true);
    ui->labelInfoActionJoueur->setText("Choisissez une carte à garder :");

    double multHauteur = ((ui->verticalLayout_5->geometry().height())) / 290.0;
    double multLargeur = ((ui->verticalLayout_5->geometry().width() - 6*(joueurARenvoyer->avoirMain().size()-1)) / joueurARenvoyer->avoirMain().size()) / 210.0; // en fonction du nb de carte qui seront montrées
    double mult = (multHauteur<multLargeur)?multHauteur:multLargeur; // pour ajouster la taille des cartes on prend le multiplicateur le plus petit entre la largeur et hauteur
    for (short int indice =0; indice < joueurARenvoyer->avoirMain().size(); indice++){ // On affiche toutes les cartes du joueur
        QPushButton* btnCarte= new QPushButton();
        btnCarte->setIcon(QIcon(joueurARenvoyer->avoirMain()[indice]->avoirImage()));

        btnCarte->setIconSize(QSize(200*mult, 285*mult)); //  taille de l'icône
        btnCarte->setFixedSize(QSize(210*mult, 290*mult)); // Taille du boutton

        // équivalent de item->data(Qt::UserRole) mais ici la fonction se crée individuellement pour chaque immages
        connect(btnCarte, &QPushButton::clicked, this, [=]() {
            ui->labelInfoActionJoueur->setVisible(false);

            // Nettoyage du layout
            QLayoutItem *child;
            while ((child = ui->listeCarteAGarder->takeAt(0)) != nullptr) {
                child->widget()->deleteLater();
                delete child;
            }

            emit envoyerSuiteAction6(joueurARenvoyer, indice, true); // On renvoie les infos nésésaire pour l'action
        });

        // Ajout au Layout
        ui->listeCarteAGarder->addWidget(btnCarte);
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
    ui->labelInfoActionJoueur->setVisible(true);
    ui->labelInfoActionJoueur->setText("Choisissez une carte pour l'action du grade :");

    int nbColonnes = 0; // le nombre max de cartes par ligne
    for (short int indice = 0; indice < cartes.size(); indice++) // on compte cb de carte sera afficher
        if (cartes.at(indice)->avoirNbExemplaires() > cartesJouer.at(indice) && indice != 1)
            nbColonnes++;
    nbColonnes = nbColonnes/2 + nbColonnes%2; // on calcule pour faire deux lignes (si impaire la première ligne sera plus remplis)

    double multHauteur = ((ui->verticalLayout_5->geometry().height() - 6) /2.0) / 290.0; // -6 = marge       /2 car 2 carte sur la hauteur
    double multLargeur = ((ui->verticalLayout_5->geometry().width() - 6*4) /5.0) / 210.0; // *4 = 4 marges       /5 car 5 carte sur la largeur
    double mult = (multHauteur<multLargeur)?multHauteur:multLargeur; // pour ajouster la taille des cartes on prend le multiplicateur le plus petit entre la largeur et hauteur
    int ligne = 0;
    int colonne = 0;
    // On rajoute toutes les cartes possibles
    for (short int indice = 0; indice < cartes.size(); indice++){
        if (cartes.at(indice)->avoirNbExemplaires() > cartesJouer.at(indice) && indice != 1){ // si la carte est possible à demander
            // Création du bouton avec l'image de la carte
            QPushButton *btnCarte = new QPushButton();
            btnCarte->setIcon(QIcon(cartes[indice]->avoirImage()));

            btnCarte->setIconSize(QSize(200*mult, 285*mult)); //  taille de l'icône
            btnCarte->setFixedSize(QSize(210*mult, 290*mult)); // Taille du boutton

            // équivalent de item->data(Qt::UserRole) mais ici la fonction se crée individuellement pour chaque immages
            connect(btnCarte, &QPushButton::clicked, this, [=]() {
                ui->labelInfoActionJoueur->setVisible(false);

                // Nettoyage du layout
                QLayoutItem *child;
                while ((child = ui->listeCarteChoixGarde->takeAt(0)) != nullptr) {
                    child->widget()->deleteLater();
                    delete child;
                }

                emit envoyerChoixValeurGarde(indice); // puis on donne le numéro de la carte
            });

            // Ajout au QGridLayout
            ui->listeCarteChoixGarde->addWidget(btnCarte, ligne, colonne);

            // calcule de l'emplacement du prochain boutton
            colonne++;
            if (colonne >= nbColonnes) {
                colonne = 0;
                ligne++;
            }
        }
    }
    // On réorganise le placement des bouttons
    if (ui->listeCarteChoixGarde->count() == 0){ // si vide, on ne peut pas choisir
        ui->labelInfoActionJoueur->setVisible(false);
        emit envoyerChoixValeurGarde(-1); // Pour signaler que c'est impossible
    }
}

void MainWindow::recevoirInitialiserListeJoueurs(QVector<QString> nomJoueurs){
    ui->listeJoueurs->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listeJoueurs->setColumnCount(nomJoueurs.size());
    ui->listeJoueurs->setRowCount(2);

    for (short int indice = 0; indice < nomJoueurs.size(); indice++){ // On liste les joueurs avec leur nom et points (0 au début)
        ui->listeJoueurs->setItem(0, indice, new QTableWidgetItem(nomJoueurs[indice]));
        ui->listeJoueurs->setItem(1, indice, new QTableWidgetItem("0"));
        // Pour ne pas avoir la casse en surbiance si on passe dessus :
        ui->listeJoueurs->item(0, indice)->setFlags(ui->listeJoueurs->item(0, indice)->flags() & ~Qt::ItemIsSelectable & ~Qt::ItemIsEnabled);
        ui->listeJoueurs->item(1, indice)->setFlags(ui->listeJoueurs->item(0, indice)->flags());
    }
}

void MainWindow::recevoirDemanderChoixCibleJoueur(QVector<QString> nomJoueurs, QVector<short int> idJoueurs){
    ui->labelInfoActionJoueur->setVisible(true);
    ui->labelInfoActionJoueur->setText("Choisissez un joueur à cibler :");

    for (short int indice =0; indice < nomJoueurs.size(); indice++){
        QPushButton* btnJoueur = new QPushButton();
        btnJoueur->setText(nomJoueurs[indice]);
        btnJoueur->setMaximumWidth(120);

        // équivalent de item->data(Qt::UserRole) mais ici la fonction se crée individuellement pour chaque immages
        connect(btnJoueur, &QPushButton::clicked, this, [=]() {
            ui->labelInfoActionJoueur->setVisible(false);

            // Nettoyage du layout
            QLayoutItem *child;
            while ((child = ui->listeJoueursCible->takeAt(0)) != nullptr) {
                child->widget()->deleteLater();
                delete child;
            }

            emit envoyerChoixCibleJoueur(idJoueurs[indice]); // puis on donne l'id du joueur
        });

        // Ajout au QGridLayout
        ui->listeJoueursCible->addWidget(btnJoueur);
    }
}

void MainWindow::recevoirMiseAJourPointsJoueurs(QVector<short int> ptJoueurs){
    for (short int indice = 0; indice < ptJoueurs.size(); indice++)
        ui->listeJoueurs->setItem(1, indice, new QTableWidgetItem(QString::number(ptJoueurs[indice])));
}

void MainWindow::recevoirMessageAlerteMainJoueurVasEtreMontre(QString nom, bool popup){
    ui->labelTourDe->setText("Tour de " + nom);
    // On signal au joueur que c'est son tour
    if (popup){
        QMessageBox alerteMessage(this);
        alerteMessage.setWindowTitle("Tour suivant");
        alerteMessage.setText("C'est au tour de " + nom + " de jouer");
        alerteMessage.addButton("Continuer", QMessageBox::AcceptRole);
        alerteMessage.setStyleSheet("margin: auto;"); // tente de centrer
        alerteMessage.exec(); // bloquant, attend le clic
    }
    // On le met en surbriance dans la liste, les couleurs son rénitialiser à chaque tour avec recevoirJoueursEliminer(), donc on colore par dessus
    for (int indice = 0; indice < ui->listeJoueurs->columnCount(); indice++){
        QTableWidgetItem* item = ui->listeJoueurs->item(0, indice);
        if (item != nullptr && item->text() == nom){
            item->setBackground(Qt::red);
            break;
        }
    }
}

void MainWindow::recevoirAfficherMain(Carte* carte1, Carte* carte2){
    ui->listeCarteMain->setVisible(true);
    ui->labelInfoActionJoueur->setVisible(true);
    ui->labelInfoActionJoueur->setText("Choisissez une carte de votre main :");

    ui->listeCarteMain->clear();

    // on crée les deux cartes
    QListWidgetItem* itemCarte1 = new QListWidgetItem(ui->listeCarteMain);
    itemCarte1->setData(Qt::UserRole, carte1->avoirNum());
    QListWidgetItem* itemCarte2 = new QListWidgetItem(ui->listeCarteMain);
    itemCarte2->setData(Qt::UserRole, carte2->avoirNum());

    // si on as la comptesse avec le prince ou le roi on doit sélectionner seullement la comptesse
    if (carte1->avoirNum() == 8 && (carte2->avoirNum() == 5 || carte2->avoirNum() == 7)) // comptesse = carte1
        itemCarte2->setFlags(Qt::NoItemFlags);
    else if (carte2->avoirNum() == 8 && (carte1->avoirNum() == 5 || carte1->avoirNum() == 7)) // comptesse = carte2
        itemCarte1->setFlags(Qt::NoItemFlags);

    // calcule pour la taille de l'affichage des cartes de la main
    double multHauteur = ui->verticalLayout_5->geometry().height() / 290.0;
    double multLargeur = ((ui->verticalLayout_5->geometry().width() - 6) /2.0) / 210.0; // -6 = 1 marges       /2.0 car 2 carte sur la largeur
    double mult = (multHauteur<multLargeur)?multHauteur:multLargeur; // pour ajouster la taille des cartes on prend le multiplicateur le plus petit entre la largeur et hauteur
    ui->listeCarteMain->setIconSize(QSize(200*mult,285*mult));
    ui->listeCarteMain->setFixedSize(QSize(200*mult*2+13,285*mult+4)); // +13 et + 4 car y a des margre (j'ai pas fait de calcule, mais comme ça c'est plutôt clean)
    for (int i = 0; i < ui->listeCarteMain->count(); i++){ // *2 mais for() au cas où
        QListWidgetItem* item = ui->listeCarteMain->item(i);
        if (item != nullptr){
            short int numCarte = item->data(Qt::UserRole).toInt();
            item->setIcon(QIcon(QPixmap(jeu->avoirPaquet()[numCarte]->avoirImage()).scaled(QSize(200*mult,285*mult), Qt::IgnoreAspectRatio, Qt::SmoothTransformation))); // permet de faire plus grand que la taille originale
        }
    }
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

    // On surligne le ou les gagants :
    for (int indice = 0; indice < ui->listeJoueurs->columnCount(); indice++){
        QTableWidgetItem* item = ui->listeJoueurs->item(0, indice);
        if (item != nullptr && nomJoueurs.contains(item->text()))
            item->setBackground(QColor("orange")); // si gagant
        else
            item->setBackground(Qt::FDiagPattern); // sinon mort
    }

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

    // On surligne le ou les gagants :
    for (int indice = 0; indice < ui->listeJoueurs->columnCount(); indice++){
        QTableWidgetItem* item = ui->listeJoueurs->item(0, indice);
        if (item != nullptr && nomJoueurs.contains(item->text())){
            item->setBackground(QColor("green")); // si gagant
            ui->listeJoueurs->item(1, indice)->setBackground(QColor("green"));
        } else
            item->setBackground(Qt::FDiagPattern); // sinon mort
    }

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
    QPushButton* bouton = new QPushButton(" OK, j'ai vu la carte ");
    connect(bouton, &QPushButton::clicked, popup, &QDialog::accept);
    layout->addWidget(bouton);

    popup->setLayout(layout);
    popup->exec(); // bloquant
    delete popup;
}

void MainWindow::recevoirDemanderChangementNom(short int idJoueur, QString nom){
    ChoixNomJoueur *fenetre = new ChoixNomJoueur(this, idJoueur, nom);
    connect(fenetre, SIGNAL(envoyerNom(short int, QString)), this, SLOT(recevoirNom(short int, QString)));
    fenetre->setFixedSize(fenetre->size());  // taille fixe basée sur la taille
    fenetre->setWindowTitle("Choix du nom de Joueur");
    fenetre->exec(); // bloque la fenetre
}

void MainWindow::recevoirNom(short int idJoueur, QString nom){
    emit changementNom(idJoueur, nom);
}

void MainWindow::recevoirJoueursEliminer(QVector<bool> joueursMort){
    for (int indice = 0; indice < ui->listeJoueurs->columnCount(); indice++){
        QTableWidgetItem* item = ui->listeJoueurs->item(0, indice);
        if (item != nullptr && joueursMort.at(indice))
            item->setBackground(Qt::FDiagPattern);
        else
            item->setBackground(Qt::NoBrush);
    }
}

// ---------------------------- ui -------------------------------------


void MainWindow::on_listeCarteMain_itemClicked(QListWidgetItem *item){
    if (!(item->flags() == Qt::NoItemFlags)){ // Si on ne doit pas sélectionner la carte, alors on ne fait rien
        ui->listeCarteMain->setVisible(false);
        ui->labelInfoActionJoueur->setVisible(false);

        emit envoyerChoixCarte(item->data(Qt::UserRole).toInt());
    }
}

