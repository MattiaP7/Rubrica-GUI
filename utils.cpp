#include "utils.hpp"

/**
 * @brief utility function to check the color scheme of the system
 * @return true - colorscheme is dark mode
 * @return false - colorscheme isn't dark mode
 *
 * in base alla versione di qt, cambia il modo di controllo
 */
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

/**
 * @brief function for capitalize every word in a QString.
 * @brief funzione che capitalizza l'iniziale di ogni parola in una stringa
 * @param str
 * @return the same string but capitalize
 * @return la stessa stringa ma capitalizzata
 */
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


