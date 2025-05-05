/**
 * @file utils.hpp
 * @brief Funzioni di utilità generiche per l'applicazione
 *
 * @details
 * Raccolta di funzioni helper per:
 * - Gestione temi grafici (light/dark mode)
 * - Formattazione testo
 * - Utility varie
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <QGuiApplication>
#include <QPalette>
#include <QString>
#include <QStyleHints>

/**
 * @brief Verifica se il sistema utilizza il tema scuro
 * @return Stato del tema del sistema
 * @retval true Tema scuro attivo
 * @retval false Tema chiaro attivo
 *
 * @note Utilizza le API Qt per la rilevazione
 */
bool isDarkMode();

/**
 * @brief Formatta una stringa con le maiuscole all'inizio di ogni parola
 * @param[in] str Stringa da formattare
 * @return Stringa formattata
 *
 * @details
 * Trasforma in maiuscolo il primo carattere di ogni parola:
 * - Riconosce gli spazi come separatori
 * - Mantiene il case degli altri caratteri
 *
 */
QString capitalize(const QString &str);

/**
 * @brief Genera un foglio di stile CSS in base al tema
 * @param[in] isDarkMode Se generare lo stile per dark mode
 * @return Stringa CSS completa
 *
 * @details
 * Produce uno stylesheet che include:
 * - Colori di sfondo/testo
 * - Stili per QWidget, QMenu, QLineEdit
 * - Effetti di hover/pressione
 * - Regole per la dark/light mode
 *
 * @note Lo stylesheet è ottimizzato per prestazioni
 */
QString applyStyleSheet(bool isDarkMode);

#endif // UTILS_HPP
