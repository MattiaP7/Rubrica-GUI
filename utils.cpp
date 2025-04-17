#include "utils.hpp"

/**
 * @brief utility function to check the color scheme of the system
 * @return true - colorscheme is dark mode
 * @return false - colorscheme isn't dark mode
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
 * @param str
 * @return the same string but capitalize
 */
QString capitalize(const QString& str){
    QStringList words = str.split(" ", Qt::SkipEmptyParts);
    for (QString& word : words)
        word.front() = word.front().toUpper();

    return words.join(" ");
}
