#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow{
        Q_OBJECT
    Ui::MainWindow *ui;
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;
    private:
        void miseAJourJoueursStatistiques();
        void miseAJourPioche();
        void miseAJourListCartesJouer();
        void creationCartesJoueurActuel();
        void afficheActionCarte0();
        void afficheActionCarte1();
        void afficheActionCarte2();
        void afficheActionCarte3();
        void afficheActionCarte4();
        void afficheActionCarte5();
        void afficheActionCarte6();
        void afficheActionCarte7();
        void afficheActionCarte8();
        void afficheActionCarte9();
        void afficherVictoireManche();
        void afficherVictoireJeu();
        void afficheActionsCartes();
    private slots:
        //void on_ListWiged_listCartes_activated(index : int)
        //void recevoirJoueur(short int, short int);
};
#endif // MAINWINDOW_H
