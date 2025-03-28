/**
 * @file list.hpp
 * @brief Contact list class definition
 */

#ifndef LIST_HPP
#define LIST_HPP

#include "contatto.hpp"
#include <QObject>
#include <QVector>

/**
 * @brief The Node struct with the Contact values and pointer to the next node
 */
struct Node {
    Contact contact; /* Values of the node */
    Node* next;      /* Pointers to the next node */

    /**
     * @brief costructor for the struct Node
     * @param c contact values
     * @param n pointer to the next node, with default value of nullptr
     */
    Node(const Contact& c, Node* n = nullptr) : contact(c), next(n) {}
};

/**
 * @class ContactList
 * @brief Manages a collection of Contact objects
 */
class ContactList : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent Parent QObject
     */
    explicit ContactList(QObject* parent = nullptr);

    /**
     * @brief Destructor
     */
    ~ContactList();

    /**
     * @brief Add a new contact to the list
     * @param contact Contact to add
     */
    void addContact(const Contact& contact);

    /**
     * @brief Remove a contact by name
     * @param name Name of contact to remove
     * @return True if contact was found and removed
     */
    bool removeContact(const QString& name);

    /**
     * @brief Update an existing contact
     * @param originalName Current name of the contact
     * @param updatedContact New contact data
     * @return True if contact was found and updated
     */
    bool updateContact(const QString& originalName, const Contact& updatedContact);

    /**
     * @brief Search contacts by query string
     * @param query Search string
     * @return Vector of matching contacts
     */
    QVector<Contact> searchContacts(const QString& query) const;

    /**
     * @brief Get all contacts
     * @return Vector of all contacts
     */
    QVector<Contact> allContacts() const;

    /**
     * @brief Check if a contact exists
     * @param name Name to check
     * @return True if contact exists
     */
    bool contains(const QString& name) const;

    /**
     * @brief Get number of contacts
     * @return Contact count
     */
    int count() const;

    /**
     * @brief Check if list is empty
     * @return True if no contacts exist
     */
    bool isEmpty() const;

    /**
     * @brief Save contacts to file
     * @param filePath Path to save file
     * @return True if save succeeded
     */
    bool saveToFile(const QString& filePath = "contacts.csv") const;

    /**
     * @brief Load contacts from file
     * @param filePath Path to load file
     * @return True if load succeeded
     */
    bool loadFromFile(const QString& filePath = "contacts.csv");

signals:
    /**
     * @brief Emitted when contacts change, without this function the table wouldn't update
     */
    void dataChanged();

private:
    Node* m_head; /* pointer to the head of the list */
    int m_count;  /* counter of node in the list     */

    /**
     * @brief clear every node in the list but without destroing the object
     */
    void clear();

    /**
     * @brief method for finding a node the list
     * @param name searching parametr
     * @return return the Node pointer if @param name == contact.name()
     */
    Node* findNode(const QString& name) const;

    /**
     * @brief sort the list by using the merge sort algorithm
     */
    void sort();
};

#endif // LIST_HPP
