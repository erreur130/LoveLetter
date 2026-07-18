QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    carte.cpp \
    choixnomjoueur.cpp \
    humain.cpp \
    ianormale.cpp \
    ianul.cpp \
    iatriche.cpp \
    jeu.cpp \
    joueur.cpp \
    main.cpp \
    mainwindow.cpp \
    nbjoueurswindow.cpp \
    paquet.cpp

HEADERS += \
    carte.h \
    choixnomjoueur.h \
    humain.h \
    ianormale.h \
    ianul.h \
    iatriche.h \
    jeu.h \
    joueur.h \
    mainwindow.h \
    nbjoueurswindow.h \
    paquet.h

FORMS += \
    InfoProjet.ui \
    affichageRegles.ui \
    affichageactionscartes.ui \
    choixnomjoueur.ui \
    mainwindow.ui \
    nbjoueurswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc
