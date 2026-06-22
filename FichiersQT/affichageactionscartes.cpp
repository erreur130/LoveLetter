#include "affichageactionscartes.h"
#include "ui_affichageactionscartes.h"

AffichageActionsCartes::AffichageActionsCartes(QWidget *parent) : QWidget(parent), ui(new Ui::AffichageActionsCartes){
    ui->setupUi(this);
    setWindowTitle(" ");
}

AffichageActionsCartes::~AffichageActionsCartes()
{
    delete ui;
}

void AffichageActionsCartes::on_ok_button_clicked(){
    close();
}

