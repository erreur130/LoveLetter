#ifndef AFFICHAGEACTIONSCARTES_H
#define AFFICHAGEACTIONSCARTES_H

#include <QWidget>

namespace Ui {
class AffichageActionsCartes;
}

class AffichageActionsCartes : public QWidget
{
    Q_OBJECT

public:
    explicit AffichageActionsCartes(QWidget *parent = nullptr);
    ~AffichageActionsCartes();

private slots:
    void on_ok_button_clicked();

private:
    Ui::AffichageActionsCartes *ui;
};

#endif // AFFICHAGEACTIONSCARTES_H
