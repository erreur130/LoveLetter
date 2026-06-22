#ifndef NBJOUEURSWINDOW_H
#define NBJOUEURSWINDOW_H

#include <QDialog>

namespace Ui {
class NbJoueursWindow;
}

class NbJoueursWindow : public QDialog
{
    Q_OBJECT

public:
    explicit NbJoueursWindow(QWidget *parent = nullptr);
    ~NbJoueursWindow();

private slots:
    void on_button_Jouer_clicked();

signals:
    void envoyerJoueur(short int, short int, short int);

private:
    Ui::NbJoueursWindow *ui;
};

#endif // NBJOUEURSWINDOW_H
