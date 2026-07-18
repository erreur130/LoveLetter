#include "choixnomjoueur.h"
#include "ui_choixnomjoueur.h"

ChoixNomJoueur::ChoixNomJoueur(QWidget *parent, short int idJoueur_, QString nom)
    : QDialog(parent), idJoueur(idJoueur_), ui(new Ui::ChoixNomJoueur){
    ui->setupUi(this);

    ui->label->setText("Nouveau nom de " + nom);
    ui->lineNom->setText(nom);
    ui->labelErreur->setStyleSheet("color: red;");
}

ChoixNomJoueur::~ChoixNomJoueur(){
    delete ui;
}

void ChoixNomJoueur::on_boutton_Valider_clicked(){
    if (ui->lineNom->text() == "")
        ui->labelErreur->setText("Le nom est vide !");
    else if (ui->lineNom->text().size() > 10)
        ui->labelErreur->setText("Le nom est trop grand !");
    else {
        emit envoyerNom(idJoueur, ui->lineNom->text());
        close();
    }
}

