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
     * 3 - ritorno l'insieme di parole (della lista) in una unica parola separata dallo spazio
     */

    QStringList words = str.split(" ", Qt::SkipEmptyParts);
    for (QString& word : words){
        word.front() = word.front().toUpper();
    }

    return words.join(" ");
}




QString applyStyleSheet(bool isDarkMode){
    QString style;
    if (isDarkMode) {
        style += R"(
            QPushButton#btnAggiungi {
                background-color: #2e8b57; /* Verde scuro */
                color: white;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnAggiungi:hover {
                background-color: #256e44; /* Verde più scuro */
            }

            QPushButton#btnConferma {
                background-color: #006400; /* Verde più intenso */
                color: white;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnConferma:hover {
                background-color: #004d00; /* Verde scuro */
            }

            QPushButton#btnCancel {
                background-color: #8b0000; /* Rosso scuro */
                color: white;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnCancel:hover {
                background-color: #6b0000; /* Rosso più scuro */
            }

            QPushButton#btnElimina {
                background-color: #b22222; /* Rosso fuoco */
                color: white;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnElimina:hover {
                background-color: #9e1c1c; /* Rosso più scuro */
            }

            QPushButton#btnModifica {
                background-color: #4682b4; /* Blu acciaio */
                color: white;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnModifica:hover {
                background-color: #3b6a8c; /* Blu scuro */
            }
        )";
    } else {
        style += R"(
            QPushButton#btnAggiungi {
                background-color: #90ee90; /* Verde chiaro */
                color: black;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnAggiungi:hover {
                background-color: #80e080; /* Verde chiaro scuro */
            }

            QPushButton#btnConferma {
                background-color: #32cd32; /* Verde lime */
                color: black;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnConferma:hover {
                background-color: #28a428; /* Verde più scuro */
            }

            QPushButton#btnCancel {
                background-color: #ff7f7f; /* Rosso chiaro */
                color: black;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnCancel:hover {
                background-color: #ff6a6a; /* Rosso più scuro */
            }

            QPushButton#btnElimina {
                background-color: #ff4c4c; /* Rosso vivo */
                color: black;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnElimina:hover {
                background-color: #e64a4a; /* Rosso più scuro */
            }

            QPushButton#btnModifica {
                background-color: #add8e6; /* Azzurro */
                color: black;
                border-radius: 5px;
                padding: 10px;
            }
            QPushButton#btnModifica:hover {
                background-color: #99c2d9; /* Azzurro più scuro */
            }
        )";
    }

    return style;
}
