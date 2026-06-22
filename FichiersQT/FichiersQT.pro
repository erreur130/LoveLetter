QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    affichageactionscartes.cpp \
    carte.cpp \
    humain.cpp \
    ia.cpp \
    jeu.cpp \
    joueur.cpp \
    main.cpp \
    mainwindow.cpp \
    nbjoueurswindow.cpp \
    paquet.cpp

HEADERS += \
    affichageactionscartes.h \
    carte.h \
    humain.h \
    ia.h \
    jeu.h \
    joueur.h \
    mainwindow.h \
    nbjoueurswindow.h \
    paquet.h

FORMS += \
    affichageactionscartes.ui \
    mainwindow.ui \
    nbjoueurswindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
