/**
 * @file utils.hpp
 * @brief Utility function definitions
 */

#ifndef UTILS_HPP
#define UTILS_HPP

#pragma once
#include <QGuiApplication>
#include <QPalette>
#include <QStyleHints>
#include <QString>

/**
 * @brief utility function to check the color scheme of the system
 * @return true - colorscheme is dark mode
 * @return false - colorscheme isn't dark mode
 */
bool isDarkMode();

/**
 * @brief function for capitalize every word in a QString.
 * @param str
 * @return the same string but capitalize
 */
QString capitalize(const QString &str);


#endif // UTILS_HPP
