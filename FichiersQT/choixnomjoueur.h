#ifndef CHOIXNOMJOUEUR_H
#define CHOIXNOMJOUEUR_H

#include <QDialog>

namespace Ui {
class ChoixNomJoueur;
}

class ChoixNomJoueur : public QDialog
{
    Q_OBJECT
    short int idJoueur;
public:
    explicit ChoixNomJoueur(QWidget *parent = nullptr, short int idJoueur_ = 0, QString nom = "");
    ~ChoixNomJoueur();

signals:
    void envoyerNom(short int, QString);

private slots:
    void on_boutton_Valider_clicked();

private:
    Ui::ChoixNomJoueur *ui;
};

#endif // CHOIXNOMJOUEUR_H
