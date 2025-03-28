/**
 * @file list.cpp
 * @brief ContactList class implementation
 */

#include "list.hpp"
#include <QFile>
#include <QTextStream>
#include <thread>

namespace {
    // Soglia per utilizzare i thread
    #define THRESHOLD 3

    /**
    * @brief Unisce due liste ordinate in una singola lista ordinata.
    *
    * @param left Puntatore alla testa della prima lista ordinata.
    * @param right Puntatore alla testa della seconda lista ordinata.
    * @return `Node*` Puntatore alla testa della lista risultante ordinata.
    */
    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;


        if (left->contact.name().toLower() < right->contact.name().toLower()) {
            left->next = merge(left->next, right);
            return left;
        } else {
            right->next = merge(left, right->next);
            return right;
        }
    }

    /**
    * @brief Divide una lista in due sottoliste di dimensione simile.
    *
    * @param head Puntatore alla testa della lista da dividere.
    * @param left Puntatore al puntatore che riceverà la testa della prima metà.
    * @param right Puntatore al puntatore che riceverà la testa della seconda metà.
    */
    void split(Node* head, Node** left, Node** right) {
        if (!head || !head->next) {
            *left = head;
            *right = nullptr;
            return;
        }

        Node* slow = head;
        Node* fast = head->next;

        while (fast && fast->next) {
            slow = slow->next;
            fast = fast->next->next;
        }

        *left = head;
        *right = slow->next;
        slow->next = nullptr;
    }

    /**
    * @brief Ordina una lista collegata utilizzando Merge Sort.
    *
    * @param head Puntatore alla testa della lista da ordinare.
    * @return `Node*` Puntatore alla testa della lista ordinata.
    */
    Node* merge_sort(Node* head, int depth = 0) {
        if (!head || !head->next) return head;

        Node* left;
        Node* right;
        split(head, &left, &right);

        if (depth < THRESHOLD){
            left = merge_sort(left, depth + 1);
            right = merge_sort(right, depth + 1);
        } else {
            Node* sorted_left = nullptr;
            Node* sorted_right = nullptr;

            std::thread left_thread([&](){
                sorted_left = merge_sort(left, depth + 1);
            });
            std::thread right_thread([&](){
                sorted_right = merge_sort(right, depth + 1);
            });

            left_thread.join();
            right_thread.join();

            left = sorted_left;
            right = sorted_right;
        }

        return merge(left, right);
    }
}

ContactList::ContactList(QObject* parent)
    : QObject(parent), m_head(nullptr), m_count(0) {}

ContactList::~ContactList()
{
    clear();
}

void ContactList::addContact(const Contact& contact)
{
    // viene eseguito un push in testa e poi ordinamo la lista
    // più veloce rispetto aggiungere il nodo già ordinato

    Node* newNode = new Node(contact, m_head);
    m_head = newNode;
    m_count++;
    sort();
    emit dataChanged();
}

bool ContactList::removeContact(const QString& name)
{
    if (!m_head) return false;

    if (m_head->contact.name() == name) {
        Node* to_delete = m_head;
        m_head = m_head->next;
        delete to_delete;
        m_count--;
        emit dataChanged();
        return true;
    }

    Node* current = m_head;
    while (current->next && current->next->contact.name() != name) {
        current = current->next;
    }

    if (current->next) {
        Node* to_delete = current->next;
        current->next = current->next->next;
        delete to_delete;
        m_count--;
        emit dataChanged();
        return true;
    }

    return false;
}

bool ContactList::updateContact(const QString& originalName, const Contact& updatedContact)
{
    Node* node = findNode(originalName);
    if (!node) return false;

    node->contact = updatedContact;
    sort();
    emit dataChanged();
    return true;
}

QVector<Contact> ContactList::searchContacts(const QString& query) const
{
    QVector<Contact> results;
    Node* current = m_head;

    while (current) {
        if (current->contact.name().contains(query, Qt::CaseInsensitive) ||
            current->contact.phone().contains(query)) {
            results.append(current->contact);
        }
        current = current->next;
    }

    return results;
}

QVector<Contact> ContactList::allContacts() const
{
    QVector<Contact> contacts;
    Node* current = m_head;

    while (current) {
        contacts.append(current->contact);
        current = current->next;
    }

    return contacts;
}

constexpr bool ContactList::contains(const QString& name) const
{
    return findNode(name) != nullptr;
}

constexpr int ContactList::count() const { return m_count; }

constexpr bool ContactList::isEmpty() const { return m_count == 0; }

bool ContactList::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    Node* current = m_head;

    while (current) {
        // Salva solo contatti non vuoti
        if(!current->contact.name().isEmpty() ||
            !current->contact.phone().isEmpty() ||
            !current->contact.email().isEmpty()) {
            out << current->contact.name() << ","
                << current->contact.phone() << ","
                << current->contact.email() << "\n";
        }
        current = current->next;
    }

    file.close();
    return true;
}

bool ContactList::loadFromFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    clear(); // Pulisci la lista corrente

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if(line.isEmpty()) continue;

        // separa la stringa in sotto stringhe quando il carattere ',' compare
        QStringList parts = line.split(',');
        if(parts.size() >= 2) {
            QString name = parts[0].trimmed();
            QString phone = parts[1].trimmed();
            // se la size della sotto stringa è maggiore di due,
            // vuol dire che c'è l'email e la carichiamo, altrimenti mettiamo ""
            QString email = parts.size() > 2 ? parts[2].trimmed() : "";

            // Aggiungi solo se almeno nome o telefono non sono vuoti
            if(!name.isEmpty() || !phone.isEmpty()) {
                addContact(Contact(name, phone, email));
            }
        }
    }

    file.close();
    emit dataChanged();
    return true;
}

void ContactList::clear()
{
    while (m_head) {
        Node* to_delete = m_head;
        m_head = m_head->next;
        delete to_delete;
    }
    m_count = 0;
}

Node* ContactList::findNode(const QString& name) const
{
    Node* current = m_head;
    while (current) {
        if (current->contact.name() == name)
            return current;
        current = current->next;
    }
    return nullptr;
}

void ContactList::sort()
{
    m_head = merge_sort(m_head, this->count());
}
