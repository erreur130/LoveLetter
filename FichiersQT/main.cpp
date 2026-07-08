#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    MainWindow w;
    w.show(); // affiche d'abord normalement
    w.showMaximized(); // force l'agrandissement

    if (not(w.doitEtreFerme())){ // Vérifie si on doit avorté le jeu
        QTimer::singleShot(0, &w, &MainWindow::lancer); // de façon asynchrone pour attendre l'agrandicement de MainWindow // tout le jeu
        return QApplication::exec();
    }

    return 0;
}
