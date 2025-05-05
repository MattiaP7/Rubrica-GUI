/**
 * @file mainwindow.h
 * @brief Finestra principale dell'applicazione rubrica
 *
 * @details
 * Classe che gestisce l'interfaccia grafica principale e coordina:
 * - Visualizzazione dei contatti
 * - Gestione delle operazioni CRUD
 * - Ricerca e filtraggio
 * - Gestione degli stati dell'interfaccia
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QTableWidgetItem>
#include "list.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

/**
 * @class MainWindow
 * @brief Classe principale dell'interfaccia grafica
 *
 * @details
 * Responsabile di:
 * - Gestire tutti gli elementi dell'UI
 * - Coordinare le operazioni sulla lista contatti
 * - Gestire la logica di visualizzazione
 * - Fornire feedback all'utente
 *
 * Implementa il pattern MVC come:
 * - View: Interfaccia grafica
 * - Controller: Gestione eventi
 * - Model: ContactList (separato)
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    /**
     * @brief Costruttore della finestra principale
     * @param[in] parent Widget genitore (opzionale)
     */
    explicit MainWindow(QWidget *parent = nullptr);

    /**
     * @brief Distruttore che gestisce la pulizia delle risorse
     */
    ~MainWindow();

private slots:
    /**
     * @brief Slot per l'aggiunta di un nuovo contatto
     * @details
     * Gestisce il click sul pulsante "Aggiungi":
     * 1. Abilita la modalità inserimento
     * 2. Sblocca i campi di input
     * 3. Prepara l'interfaccia per l'inserimento
     */
    void onAddButtonClicked();

    /**
     * @brief Slot per la conferma dell'inserimento/modifica
     * @details
     * Gestisce il click su "Conferma":
     * - In modalità aggiunta: crea nuovo contatto
     * - In modalità modifica: applica cambiamenti
     * - Esegue validazione input
     * - Aggiorna l'interfaccia
     */
    void onConfirmButtonClicked();

    /**
     * @brief Slot per annullare l'operazione corrente
     * @details
     * Gestisce il click su "Annulla":
     * - Ripristina lo stato iniziale
     * - Pulisce i campi
     * - Disabilita la modalità modifica/inserimento
     */
    void onCancelButtonClicked();

    /**
     * @brief Slot per la rimozione di un contatto
     * @details
     * Gestisce il click su "Rimuovi":
     * - Elimina il contatto selezionato
     * - Mostra conferma all'utente
     * - Aggiorna la visualizzazione
     */
    void onRemoveButtonClicked();

    /**
     * @brief Slot per l'inizio modifica contatto
     * @details
     * Gestisce il click su "Modifica":
     * 1. Abilita la modalità modifica
     * 2. Carica i dati del contatto selezionato
     * 3. Sblocca i campi di input
     */
    void onEditButtonClicked();

    /**
     * @brief Slot per l'aggiornamento dell'interfaccia
     * @details
     * Chiamato quando la lista contatti cambia:
     * - Ricarica la tabella
     * - Aggiorna i contatori
     * - Ripristina lo stato iniziale
     */
    void onContactListChanged();

    /**
     * @brief Slot per la ricerca in tempo reale
     * @param[in] query Testo da cercare
     * @details
     * Filtra i contatti in base a:
     * - Nome (case-insensitive)
     * - Telefono (parziale)
     * - Email (parziale)
     * Evidenzia i risultati nella tabella
     */
    void on_inputSearch_textChanged(const QString &query);

private:
    Ui::MainWindow *ui;                  /**< Puntatore all'interfaccia generata da Qt Designer */
    ContactList m_contactList;           /**< Istanza della lista contatti (model) */
    int m_editingRow = -1;               /**< Riga in modifica (-1 = nessuna modifica) */
    QVector<int> m_searchResultsIndices; /**< Indici dei risultati di ricerca */
    QSortFilterProxyModel *m_proxyModel; /**< Modello per il filtraggio dei dati */

    /**
     * @brief Inizializza l'interfaccia grafica
     * @details
     * Configura:
     * - Connessioni segnali/slot
     * - Stato iniziale dei widget
     * - Intestazioni tabelle
     * - Validatori di input
     */
    void initializeUI();

    /**
     * @brief Aggiorna la tabella dei contatti
     * @details
     * Ricarica tutti i contatti:
     * - Cancella il contenuto corrente
     * - Ricostruisce le righe
     * - Mantiene l'ordinamento
     * - Ripristina le selezioni
     */
    void refreshContactTable();

    /**
     * @brief Pulisce i campi di input
     * @details
     * Svuota e resetta:
     * - Campo nome
     * - Campo telefono
     * - Campo email
     * - Eventuali stati di errore
     */
    void clearInputFields();

    /**
     * @brief Mostra un messaggio di errore
     * @param[in] title Titolo della finestra
     * @param[in] message Messaggio da visualizzare
     */
    void showErrorMessage(const QString &title, const QString &message);

    /**
     * @brief Ripristina il form allo stato iniziale
     * @details
     * - Pulisce i campi
     * - Disabilita la modifica
     * - Ripristina i pulsanti
     * - Annulla la selezione
     */
    void resetForm();

    /**
     * @brief Conferma le modifiche a un contatto
     * @param[in] originalName Nome originale (per identificazione)
     */
    void confirmEdit(const QString &originalName);

    /**
     * @brief Slot alternativo per conferma modifica
     */
    void on_btnConferma_2_clicked();

    /**
     * @brief Slot alternativo per annullamento
     */
    void on_btnCancel_2_clicked();

    /**
     * @brief Slot per annullare la modifica
     */
    void onCancelEditClicked();
};

#endif // MAINWINDOW_H
