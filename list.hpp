/**
 * @file list.hpp
 * @brief Contact list management using a linked list structure
 * @brief Gestione della rubrica utilizzando una struttura a lista concatenata
 */

#ifndef LIST_HPP
#define LIST_HPP

#include "contatto.hpp"
#include <QObject>
#include <QVector>

/**
 * @struct Node
 * @brief Node structure for the linked list implementation
 * @brief Struttura Node per l'implementazione della lista concatenata
 *
 * This structure represents a single node in the linked list, containing
 * a Contact object and a pointer to the next node in the sequence.
 *
 * Questa struttura rappresenta un singolo nodo nella lista concatenata, contenente
 * un oggetto Contact e un puntatore al nodo successivo nella sequenza.
 */
struct Node {
    Contact contact; /**< Contact data stored in the node / Dati del contatto memorizzati nel nodo */
    Node* next;      /**< Pointer to the next node in the list / Puntatore al nodo successivo nella lista */

    /**
     * @brief Constructor for Node structure
     * @brief Costruttore per la struttura Node
     *
     * @param c Contact object to store in the node
     * @param n Pointer to the next node (defaults to nullptr)
     *
     * @param c Oggetto Contact da memorizzare nel nodo
     * @param n Puntatore al nodo successivo (default a nullptr)
     */
    Node(const Contact& c, Node* n = nullptr) : contact(c), next(n) {}
};

/**
 * @class ContactList
 * @brief A linked list implementation for managing Contact objects
 * @brief Implementazione di una lista concatenata per la gestione di oggetti Contact
 *
 * This class provides functionality to manage a collection of contacts
 * using a singly linked list structure. It supports basic CRUD operations,
 * file I/O, and sorting capabilities.
 *
 * Questa classe fornisce funzionalità per gestire una collezione di contatti
 * utilizzando una struttura a lista concatenata singola. Supporta operazioni CRUD di base,
 * I/O su file e capacità di ordinamento.
 */
class ContactList : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructs an empty ContactList
     * @brief Costruisce una ContactList vuota
     *
     * @param parent Parent QObject for Qt's object hierarchy
     *
     * @param parent QObject genitore per la gerarchia di oggetti Qt
     */
    explicit ContactList(QObject* parent = nullptr);

    /**
     * @brief Destructor that cleans up all nodes
     * @brief Distruttore che pulisce tutti i nodi
     */
    ~ContactList();

    /**
     * @brief Adds a new contact to the list
     * @brief Aggiunge un nuovo contatto alla lista
     *
     * @param contact Contact object to add
     * @return void
     *
     * @param contact Oggetto Contact da aggiungere
     * @return void
     */
    void addContact(const Contact& contact);

    /**
     * @brief Removes a contact by name
     * @brief Rimuove un contatto per nome
     *
     * @param name Name of the contact to remove
     * @return true if contact was found and removed, false otherwise
     *
     * @param name Nome del contatto da rimuovere
     * @return true se il contatto è stato trovato e rimosso, false altrimenti
     */
    bool removeContact(const QString& name);

    /**
     * @brief Updates an existing contact
     * @brief Aggiorna un contatto esistente
     *
     * @param originalName Current name of the contact to update
     * @param updatedContact New contact data
     * @return true if contact was found and updated, false otherwise
     *
     * @param originalName Nome attuale del contatto da aggiornare
     * @param updatedContact Nuovi dati del contatto
     * @return true se il contatto è stato trovato e aggiornato, false altrimenti
     */
    bool updateContact(const QString& originalName, const Contact& updatedContact);

    /**
     * @brief Searches contacts matching a query string
     * @brief Cerca contatti che corrispondono a una stringa di ricerca
     *
     * The search is case-insensitive and looks for matches in name, phone, or email.
     * La ricerca è case-insensitive e cerca corrispondenze in nome, telefono o email.
     *
     * @param query Search string
     * @return QVector<Contact> of matching contacts
     *
     * @param query Stringa di ricerca
     * @return QVector<Contact> di contatti corrispondenti
     */
    QVector<Contact> searchContacts(const QString& query) const;

    /**
     * @brief Gets all contacts in the list
     * @brief Ottiene tutti i contatti nella lista
     *
     * @return QVector<Contact> containing all contacts
     *
     * @return QVector<Contact> contenente tutti i contatti
     */
    QVector<Contact> allContacts() const;

    /**
     * @brief Checks if a contact exists in the list
     * @brief Verifica se un contatto esiste nella lista
     *
     * @param name Name to check
     * @return true if contact exists, false otherwise
     *
     * @param name Nome da verificare
     * @return true se il contatto esiste, false altrimenti
     */
    bool contains(const QString& name) const;

    /**
     * @brief Gets the number of contacts in the list
     * @brief Ottiene il numero di contatti nella lista
     *
     * @return int count of contacts
     *
     * @return int conteggio dei contatti
     */
    int count() const;

    /**
     * @brief Checks if the list is empty
     * @brief Verifica se la lista è vuota
     *
     * @return true if list is empty, false otherwise
     *
     * @return true se la lista è vuota, false altrimenti
     */
    bool isEmpty() const;

    /**
     * @brief Saves contacts to a CSV file
     * @brief Salva i contatti in un file CSV
     *
     * The file format is: Name,Phone,Email (one contact per line)
     * Il formato del file è: Nome,Telefono,Email (un contatto per riga)
     *
     * @param filePath Path to the output file (default: "contacts.csv")
     * @return true if save succeeded, false otherwise
     *
     * @param filePath Percorso al file di output (default: "contacts.csv")
     * @return true se il salvataggio è riuscito, false altrimenti
     */
    bool saveToFile(const QString& filePath = "contacts.csv") const;

    /**
     * @brief Loads contacts from a CSV file
     * @brief Carica i contatti da un file CSV
     *
     * The file format should be: Name,Phone,Email (one contact per line)
     * Il formato del file dovrebbe essere: Nome,Telefono,Email (un contatto per riga)
     *
     * @param filePath Path to the input file (default: "contacts.csv")
     * @return true if load succeeded, false otherwise
     *
     * @param filePath Percorso al file di input (default: "contacts.csv")
     * @return true se il caricamento è riuscito, false altrimenti
     */
    bool loadFromFile(const QString& filePath = "contacts.csv");

signals:
    /**
     * @brief Signal emitted when the contact data changes
     * @brief Segnale emesso quando i dati dei contatti cambiano
     *
     * This signal is used to notify observers (like UI components)
     * that the underlying data has changed and needs refreshing.
     *
     * Questo segnale è usato per notificare agli osservatori (come componenti UI)
     * che i dati sottostanti sono cambiati e necessitano di aggiornamento.
     */
    void dataChanged();

private:
    Node* m_head; /**< Pointer to the first node in the list / Puntatore al primo nodo nella lista */
    int m_count;  /**< Number of contacts in the list / Numero di contatti nella lista */

    /**
     * @brief Clears all nodes from the list
     * @brief Svuota tutti i nodi dalla lista
     *
     * This method deallocates all nodes but keeps the ContactList object valid.
     *
     * Questo metodo dealloca tutti i nodi ma mantiene valido l'oggetto ContactList.
     */
    void clear();

    /**
     * @brief Finds a node by contact name
     * @brief Trova un nodo per nome del contatto
     *
     * @param name Name to search for
     * @return Node* Pointer to the found node, or nullptr if not found
     *
     * @param name Nome da cercare
     * @return Node* Puntatore al nodo trovato, o nullptr se non trovato
     */
    Node* findNode(const QString& name) const;

    /**
     * @brief Sorts the list using merge sort algorithm
     * @brief Ordina la lista usando l'algoritmo merge sort
     *
     * Contacts are sorted alphabetically by name in ascending order.
     * I contatti sono ordinati alfabeticamente per nome in ordine crescente.
     */
    void sort();
};

#endif // LIST_HPP
