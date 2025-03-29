/**
 * @file mainwindow.h
 * @brief Main application window header
 * @brief Intestazione della finestra principale dell'applicazione
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
 * @brief La classe della finestra principale dell'applicazione
 *
 * This class represents the main window of the application and handles
 * all user interface interactions and contact management operations.
 *
 * Questa classe rappresenta la finestra principale dell'applicazione e gestisce
 * tutte le interazioni con l'interfaccia utente e le operazioni di gestione dei contatti.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @brief Costruttore
     *
     * @param parent The parent widget (default is nullptr)
     *
     * @param parent Il widget genitore (default è nullptr)
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Destructor
     * @brief Distruttore
     */
    ~MainWindow();

private slots:
    /**
     * @brief Slot for handling the Add button click
     * @brief Slot per gestire il click sul pulsante Aggiungi
     */
    void onAddButtonClicked();

    /**
     * @brief Slot for handling the Search button click
     * @brief Slot per gestire il click sul pulsante Cerca
     */
    void onSearchButtonClicked();

    /**
     * @brief Slot for handling the Remove button click
     * @brief Slot per gestire il click sul pulsante Rimuovi
     */
    void onRemoveButtonClicked();

    /**
     * @brief Slot for handling the Edit button click
     * @brief Slot per gestire il click sul pulsante Modifica
     */
    void onEditButtonClicked();

    /**
     * @brief Slot called when the contact list changes
     * @brief Slot chiamato quando la lista contatti cambia
     *
     * Updates the UI to reflect changes in the contact list.
     * Aggiorna l'interfaccia utente per riflettere i cambiamenti nella lista contatti.
     */
    void onContactListChanged();

private:
    Ui::MainWindow *ui;             /**< Pointer to the UI components / Puntatore ai componenti dell'interfaccia */
    ContactList m_contactList;      /**< The contact list instance / Istanza della lista contatti */

    /**
     * @brief Initializes the user interface components
     * @brief Inizializza i componenti dell'interfaccia utente
     */
    void initializeUI();

    /**
     * @brief Refreshes the contact table with current data
     * @brief Aggiorna la tabella contatti con i dati correnti
     */
    void refreshContactTable();

    /**
     * @brief Clears all input fields
     * @brief Pulisce tutti i campi di input
     */
    void clearInputFields();

    /**
     * @brief Shows an error message dialog
     * @brief Mostra una finestra di dialogo per messaggi di errore
     *
     * @param title The dialog title / Il titolo della finestra
     * @param message The error message / Il messaggio di errore
     */
    void showErrorMessage(const QString& title, const QString& message);

    /**
     * @brief Creates a Contact object from input fields
     * @brief Crea un oggetto Contact dai campi di input
     *
     * @return Contact object created from input fields
     *
     * @return Oggetto Contact creato dai campi di input
     */
    Contact getContactFromInput() const;

    /**
     * @brief Validates the user input
     * @brief Valida l'input dell'utente
     *
     * @return true if input is valid, false otherwise
     *
     * @return true se l'input è valido, false altrimenti
     */
    bool validateInput() const;

    /**
     * @brief Highlights search results in the table
     * @brief Evidenzia i risultati della ricerca nella tabella
     *
     * @param results The search results to highlight
     *
     * @param results I risultati della ricerca da evidenziare
     */
    void highlightSearchResults(const QVector<Contact>& results);

    /**
     * @brief Clears all highlights from the table
     * @brief Rimuove tutte le evidenziazioni dalla tabella
     */
    void clearHighlights();


    /**
     * @brief Resets the form to its initial state
     * @brief Riporta il form al suo stato iniziale
     */
    void resetForm();

    /**
     * @brief Confirms and applies contact edits
     * @brief Conferma e applica le modifiche al contatto
     *
     * @param originalName The original name of the contact (for identification)
     *
     * @param originalName Il nome originale del contatto (per identificazione)
     */
    void confirmEdit(const QString& originalName);
};

#endif // MAINWINDOW_H
