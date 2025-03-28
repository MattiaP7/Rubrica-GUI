/**
 * @file mainwindow.h
 * @brief Main application window header
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include "list.hpp"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief The main application window class
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAddButtonClicked();
    void onSearchButtonClicked();
    void onRemoveButtonClicked();
    void onEditButtonClicked();
    void onContactListChanged();

private:
    Ui::MainWindow *ui;
    ContactList m_contactList;

    void initializeUI();
    void refreshContactTable();
    void clearInputFields();
    void showErrorMessage(const QString& title, const QString& message);
    Contact getContactFromInput() const;
    bool validateInput() const;

    void highlightSearchResults(const QVector<Contact>& results);
    void clearHighlights();
    int showSelectionDialog(const QString& title, const QString& message, const QVector<Contact>& contacts);
    void resetForm();
    void confirmEdit(const QString& originalName);
};

#endif // MAINWINDOW_H
