#include "nbjoueurswindow.h"
#include "ui_nbjoueurswindow.h"

NbJoueursWindow::NbJoueursWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NbJoueursWindow)
{
    ui->setupUi(this);
}

NbJoueursWindow::~NbJoueursWindow(){
    delete ui;
}

void NbJoueursWindow::on_button_Jouer_clicked(){
    short int h = ui->spinBox_Humain->value();
    short int inul = ui->spinBox_IANul->value();
    short int inorm = ui->spinBox_IANorm->value();
    short int itri = ui->spinBox_IATri->value();

    if (h+inul+inorm+itri <= 6 && h+inul+inorm+itri >= 2){ // min 2 personnes et max 6
        emit envoyerJoueur(h, inul, inorm, itri);
        accept(); // signale que le dialog est validé → exec() retourne QDialog::Accepted
    }
}

