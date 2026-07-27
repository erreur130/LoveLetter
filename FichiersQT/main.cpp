#include "mainwindow.h"

#include <QApplication>
#include <QScreen>

int main(int argc, char *argv[]){
    QApplication a(argc, argv);
    //a.setWindowIcon(QIcon(":/images/Love_Letter_icon.png")); // à mettre ?

    MainWindow w;
    w.setWindowIcon(QIcon(":/images/Love_Letter_icon.png"));

    QScreen* screen = QApplication::primaryScreen();
    w.resize(screen->availableSize()); // force l'agrandissement maximale

    w.show(); // affiche normalement avec la taille max

    if (not(w.doitEtreFerme())){ // Vérifie si on doit avorté le jeu
        // Bug, si on joue avec ia, l'agrandissement ce fait mal -----------------------------------------------------------------------------------------------------------
        QTimer::singleShot(0, &w, &MainWindow::lancer); // de façon asynchrone pour attendre l'agrandicement de MainWindow // tout le jeu
        return QApplication::exec();
    }

    return 0;
}
