/**
 * @file list.hpp
 * @brief Gestione di una rubrica utilizzando una linked list
 *
 * @details
 * Implementazione di una linked list per la gestione di contatti anagrafici.
 * La classe offre operazioni CRUD(Create, Read, Update, Delete), 
 *  persistenza su file CSV e funzionalità di ricerca/ordinamento.
 */

#ifndef LIST_HPP
#define LIST_HPP

#include <QObject>
#include <QTableWidget>
#include <QVector>
#include "contatto.hpp"

/**
 * @struct Node
 * @brief Nodo base per l'implementazione della linked list
 *
 * @details
 * Struttura fondamentale che contiene:
 * - Un oggetto Contact con i dati del contatto
 * - Un puntatore al nodo successivo (nullptr se è l'ultimo nodo)
 */
struct Node
{
    Contact contact; /**< Dati anagrafici del contatto */
    Node *next;      /**< Puntatore al nodo successivo (nullptr per fine lista) */

    /**
     * @brief Costruttore del nodo
     * @param[in] c Contatto da memorizzare
     * @param[in] n Puntatore al nodo successivo (default: nullptr)
     */
    Node(const Contact &c, Node *n = nullptr)
        : contact(c)
        , next(n)
    {}
};

/**
 * @class ContactList
 * @brief linked list per la gestione avanzata di contatti
 *
 * @details
 * Classe derivata da QObject che implementa una linked list singola con:
 * - Inserimento/rimozione/aggiornamento contatti
 * - Ricerca case-insensitive
 * - Ordinamento automatico
 * - Persistenza su file CSV
 * - Notifiche di cambiamento dati via segnali Qt
 *
 */
class ContactList : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Costruttore principale
     * @param[in] parent Oggetto padre nella gerarchia Qt (opzionale)
     */
    explicit ContactList(QObject *parent = nullptr);

    /**
     * @brief Distruttore che libera la memoria allocata
     * @details
     * Scorre tutta la lista e dealloca ogni singolo nodo
     */
    ~ContactList();

    /**
     * @brief Aggiunge un nuovo contatto alla lista
     * @param[in] contact Contatto da aggiungere
     * @post La lista viene riordinata automaticamente
     * @emits dataChanged()
     */
    void addContact(const Contact &contact);

    /**
     * @brief Rimuove un contatto per nome
     * @param[in] name Nome esatto del contatto da rimuovere (case-sensitive)
     * @retval true Contatto trovato e rimosso
     * @retval false Contatto non trovato
     * @emits dataChanged() se la rimozione ha successo
     */
    bool removeContact(const QString &name);

    /**
     * @brief Aggiorna un contatto esistente
     * @param[in] originalName Nome attuale del contatto da modificare
     * @param[in] updatedContact Nuovi dati del contatto
     * @retval true Contatto trovato e aggiornato
     * @retval false Contatto non trovato
     * @post La lista viene riordinata automaticamente
     * @emits dataChanged() se l'aggiornamento ha successo
     */
    bool updateContact(const QString &originalName, const Contact &updatedContact);

    /**
     * @brief Ricerca avanzata nella rubrica
     * @param[in] query Stringa di ricerca (case-insensitive)
     * @param[in] table Widget tabella per visualizzazione risultati (può essere nullptr)
     * @return Vector di indici dei contatti trovati
     * @details
     * Cerca la stringa in:
     * - Nome completo
     * - Numero di telefono
     * - Indirizzo email
     */
    QVector<int> search(const QString &query, QTableWidget *table);

    /**
     * @brief Restituisce tutti i contatti
     * @return Vector con copia di tutti i contatti
     * @note Complessità O(n) visto che deve scorrere tutta la lista
     */
    QVector<Contact> allContacts() const;

    /**
     * @brief Verifica l'esistenza di un contatto
     * @param[in] name Nome esatto da cercare (case-sensitive) oppure numero di telefono
     * @retval true Contatto presente
     * @retval false Contatto assente
     */
    bool contains(const QString &value) const;

    /**
     * @brief Conta i contatti presenti
     * @return Numero di contatti nella lista
     */
    constexpr size_t size() const;

    /**
     * @brief Verifica se la lista è vuota
     * @retval true Lista vuota
     * @retval false Lista contiene elementi
     */
    constexpr bool isEmpty() const;

    /**
     * @brief Salvataggio su file CSV
     * @param[in] filePath Percorso del file (default: "contacts.csv")
     * @retval true Salvataggio riuscito
     * @retval false Errore nel salvataggio
     * @details Formato file:
     * Nome,Telefono,Email\n
     * (Un contatto per riga senza intestazione)
     * 
     * @copydoc std::cout << a
     * 
     * 
     * @code std::cout << a
     */
    bool saveToFile(const QString &filePath = "contacts.csv") const;

    /**
     * @brief Caricamento da file CSV
     * @param[in] filePath Percorso del file (default: "contacts.csv")
     * @retval true Caricamento riuscito
     * @retval false Errore nel caricamento
     * @details
     * Il file deve avere il formato:
     * Nome,Telefono,Email\n
     * (Un contatto per riga senza intestazione)
     * @note Sostituisce tutti i contatti esistenti
     * @emits dataChanged() se il caricamento ha successo
     */
    bool loadFromFile(const QString &filePath = "contacts.csv");

    /**
     * @brief Accesso diretto a un contatto per indice
     * @param[in] index Posizione nella lista (partendo da 0)
     * @return Contatto alla posizione richiesta
     * @throw Contact vuoto Se l'indice è invalido
     */
    Contact at(size_t index) const;

    /**
     * @brief Aggiornamento per indice
     * @param[in] index Posizione da aggiornare (partendo da 0)
     * @param[in] updatedContact Nuovi dati del contatto
     * @retval true Aggiornamento riuscito
     * @retval false Indice non valido
     * @post La lista viene riordinata automaticamente
     * @emits dataChanged() se l'aggiornamento ha successo
     */
    bool updateAt(size_t index, const Contact &updatedContact);

signals:
    /**
     * @brief Segnale di cambiamento dati
     * @details
     * Emesso quando:
     * - Viene aggiunto/rimosso un contatto
     * - Un contatto viene modificato
     * - La lista viene caricata da file
     */
    void dataChanged();

private:
    Node *m_head; /**< Puntatore alla testa della lista */
    size_t m_count;  /**< Contatore dei nodi presenti */

    /**
     * @brief Svuota completamente la lista
     * @details
     * Dealloca tutti i nodi e reimposta lo stato iniziale
     */
    void clear();

    /**
     * @brief Ricerca un nodo per nome
     * @param[in] name Nome esatto da cercare (case-sensitive) oppure numero di telefono
     * @return Puntatore al nodo trovato o nullptr
     */
    Node *findNode(const QString &value) const;

    /**
     * @brief Ordina la lista per nome
     * @details
     * Implementa l'algoritmo merge sort ricorsivo.
     * L'ordinamento è:
     * - Crescente
     * - Case-insensitive
     * - Basato sul nome completo
     */
    void sort();
};

#endif // LIST_HPP
