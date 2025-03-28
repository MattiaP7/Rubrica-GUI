/**
 * @mainpage Rubruca GUI tramite Qt creator
 *
 * @section intro Introduzione
 * Documentazione del progetto 'Rubrica GUI'
 *
 * @section features Funzionalità
 * - Aggiungi contatti
 * - Cerca contatti
 * - Elimina contatti
 * - Ordinamento automatico dei contatti
 * - e molto altro...
 * @section author Autore
 * Sviluppato da <a href="https://github.com/MattiaP7/" target="_blank">Mattia Pirazzi</a> .
 */


#include "mainwindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowIcon(QIcon("app_icon.png"));
    w.show();
    return a.exec();
}
