#include "utils.hpp"

bool isDarkMode() {
#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    const auto scheme = QGuiApplication::styleHints()->colorScheme();
    return scheme == Qt::ColorScheme::Dark;
#else
    const QPalette defaultPalette;
    const auto text = defaultPalette.color(QPalette::WindowText);
    const auto window = defaultPalette.color(QPalette::Window);
    return text.lightness() > window.lightness();
#endif // QT_VERSION
}


QString capitalize(const QString& str){
    /**
     * 1 - spezzo la stringa in parole, ovvero quando incontro uno spazio,
     *      contenuta in una lista di QString
     * 2 - per ogni parola nella lista, prendo il suo primo carattere e lo rendo ::toUpper()
     * 3 - ritorno l'insieme di parole (della lista) in una unica parola separata dallo spazio ::join(" ")
     */

    QStringList words = str.split(" ", Qt::SkipEmptyParts);
    for (QString& word : words){
        word.front() = word.front().toUpper();
    }

    return words.join(" ");
}

QString applyStyleSheet(bool isDarkMode)
{
    QString style;

    // le "Raw string" R"()" servono a rendere piu' facile l'inserimento di spazi, caratteri speciali, etc...
    if (isDarkMode) {
        style += R"(
            QWidget {
                background-color: #121212;
                color: #f0f0f0;
            }

            QLineEdit {
                background-color: #1e1e1e;
                color: #f0f0f0;
                border: 1px solid #555;
                border-radius: 6px;
                padding: 6px;
            }

            QLineEdit:focus {
                border: 1px solid #2196f3;
            }

            QTableWidget {
                background-color: #1e1e1e;
                color: #f0f0f0;
                alternate-background-color: #2c2c2c;
                gridline-color: #444;
                border: 1px solid #333;
            }

            QTableWidget::item:selected {
                background-color: #3949ab;
                color: #ffffff;
            }

            QHeaderView::section {
                background-color: #222831;
                color: #f0f0f0;
                border: 1px solid #444;
                padding: 6px;
            }

            QScrollBar:vertical, QScrollBar:horizontal {
                background: #2c2c2c;
                border: none;
                width: 12px;
            }

            QScrollBar::handle {
                background: #888; /* più visibile */
                border-radius: 6px;
            }

            QScrollBar::handle:hover {
                background: #aaa;
            }

            /* === Bottoni Principali === */
            QPushButton#btnAggiungi {
                background-color: #2e7d32;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 10px 16px;
                font-weight: bold;
                font-size: 16px;
            }

            QPushButton#btnAggiungi:hover {
                background-color: #388e3c;
            }

            QPushButton#btnElimina {
                background-color: #c62828;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 10px 16px;
                font-weight: bold;
                font-size: 16px;
            }

            QPushButton#btnElimina:hover {
                background-color: #e53935;
            }

            QPushButton#btnModifica {
                background-color: #0277bd;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 10px 16px;
                font-weight: bold;
                font-size: 16px;
            }

            QPushButton#btnModifica:hover {
                background-color: #039be5;
            }

            /* === Bottoni Conferma/Annulla === */
            QPushButton#btnConferma, QPushButton#btnConferma_2 {
                background-color: #2e7d32;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 8px 14px;
                font-weight: bold;
                font-size: 15px;
            }

            QPushButton#btnConferma:hover, QPushButton#btnConferma_2:hover {
                background-color: #388e3c;
            }

            QPushButton#btnCancel, QPushButton#btnCancel_2 {
                background-color: #c62828;
                color: white;
                border: none;
                border-radius: 6px;
                padding: 8px 14px;
                font-weight: bold;
                font-size: 15px;
            }

            QPushButton#btnCancel:hover, QPushButton#btnCancel_2:hover {
                background-color: #e53935;
            }
        )";
    } else {
        style += R"(
            QWidget {
                background-color: #ffffff;
                color: #000000;
            }

            QLineEdit {
                background-color: #ffffff;
                color: #000000;
                border: 1px solid #aaa;
                border-radius: 6px;
                padding: 6px;
            }

            QLineEdit:focus {
                border: 1px solid #3498db;
                background-color: #f0f8ff;
            }

            QTableWidget {
                background-color: #ffffff;
                color: #000000;
                alternate-background-color: #f9f9f9;
                gridline-color: #ccc;
                border: 1px solid #ddd;
            }

            QTableWidget::item:selected {
                background-color: #aed6f1;
                color: #000000;
            }

            QHeaderView::section {
                background-color: #d6eaf8;
                color: #154360;
                border: 1px solid #ccc;
                padding: 6px;
            }

            QScrollBar:vertical, QScrollBar:horizontal {
                background: #f0f0f0;
                border: none;
                width: 12px;
            }

            QScrollBar::handle {
                background: #bbb;
                border-radius: 6px;
            }

            QScrollBar::handle:hover {
                background: #888;
            }

            /* === Bottoni Principali === */
            QPushButton#btnAggiungi {
                background-color: #81c784;
                color: black;
                border: none;
                border-radius: 6px;
                padding: 10px 16px;
                font-weight: bold;
                font-size: 16px;
            }

            QPushButton#btnAggiungi:hover {
                background-color: #66bb6a;
            }

            QPushButton#btnElimina {
                background-color: #ef9a9a;
                color: black;
                border: none;
                border-radius: 6px;
                padding: 10px 16px;
                font-weight: bold;
                font-size: 16px;
            }

            QPushButton#btnElimina:hover {
                background-color: #e57373;
            }

            QPushButton#btnModifica {
                background-color: #81d4fa;
                color: black;
                border: none;
                border-radius: 6px;
                padding: 10px 16px;
                font-weight: bold;
                font-size: 16px;
            }

            QPushButton#btnModifica:hover {
                background-color: #4fc3f7;
            }

            /* === Bottoni Conferma/Annulla === */
            QPushButton#btnConferma, QPushButton#btnConferma_2 {
                background-color: #81c784;
                color: black;
                border: none;
                border-radius: 6px;
                padding: 8px 14px;
                font-weight: bold;
                font-size: 15px;
            }

            QPushButton#btnConferma:hover, QPushButton#btnConferma_2:hover {
                background-color: #66bb6a;
            }

            QPushButton#btnCancel, QPushButton#btnCancel_2 {
                background-color: #ef9a9a;
                color: black;
                border: none;
                border-radius: 6px;
                padding: 8px 14px;
                font-weight: bold;
                font-size: 15px;
            }

            QPushButton#btnCancel:hover, QPushButton#btnCancel_2:hover {
                background-color: #e57373;
            }
        )";
    }

    return style;
}
