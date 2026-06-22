#include "nbjoueurswindow.h"
#include "ui_nbjoueurswindow.h"

NbJoueursWindow::NbJoueursWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NbJoueursWindow)
{
    ui->setupUi(this);
}

NbJoueursWindow::~NbJoueursWindow()
{
    delete ui;
}

void NbJoueursWindow::on_button_Jouer_clicked(){
    short int h = ui->spinBox_Humain->value();
    short int i = ui->spinBox_IA->value();
    short int ii = ui->spinBox_IA_Int->value();

    if (h+i+ii <= 6 && h+i+ii >= 2){ // min 2 personnes et max 6
        emit envoyerJoueur(h, i, ii);
        close();
    }
}

