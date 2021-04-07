#-------------------------------------------------
#
# Project created by QtCreator 2014-05-16T14:38:23
#
#-------------------------------------------------

QT       += sql core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dvdorganizer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    actor.cpp \
    db.cpp \
    movie.cpp \
    category.cpp \
    movieWindow.cpp \
    type.cpp \
    selectionwindow.cpp \
    actorwindow.cpp

HEADERS  += mainwindow.h \
    actor.h \
    db.h \
    movie.h \
    category.h \
    movieWindow.h \
    type.h \
    selectionwindow.h \
    roles.h \
    actorwindow.h

FORMS    += mainwindow.ui \
    movieWindow.ui \
    selectionwindow.ui \
    actorwindow.ui


yourset.path = $$OUT_PWD
yourset.files = database/data.sqlite

yourset2.path = $$OUT_PWD/res
yourset2.files = res/*
INSTALLS += yourset yourset2
