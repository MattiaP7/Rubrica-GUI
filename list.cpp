/**
 * @file list.cpp
 * @brief ContactList class implementation
 */

#include "list.hpp"
#include "utils.hpp"
#include <QFile>
#include <QTextStream>
#include <thread>


/**
 * @brief namespace for utility function used ONLY in this file.
 * @brief namespace per funzioni utilizzate UNICAMENTE in questo file.
 */
namespace m_list_namespace {
// Soglia per l'utilizzo della parallelizzazione
#define THRESHOLD 500

/**
 * @brief Unisce due liste ordinate in una singola lista ordinata (ricorsiva)
 * 
 * Algoritmo:
 * 1. Confronta i nodi testa delle due liste
 * 2. Sceglie il nodo minore come prossimo elemento
 * 3. Richiama ricorsivamente merge sul resto delle liste
 * 4. Concatena il risultato
 * 
 */
Node *merge(Node *left, Node *right)
{
    // Caso base: se una lista è vuota, ritorna l'altra
    if (!left)
        return right;
    if (!right)
        return left;

    // Confronto case-insensitive dei nomi
    if (left->contact.name().toLower() < right->contact.name().toLower()) {
        // Il nodo left è minore, diventa il prossimo
        left->next = merge(left->next, right); // Ricorsione sul resto
        return left;                           // Ritorno la nuova lista
    } else {
        // Il nodo right è minore o uguale
        right->next = merge(left, right->next); // Ricorsione sul resto
        return right;                           // Ritorno la nuova testa
    }
}

/**
 * @brief Divide la lista in due sottoliste 
* 
 * Algoritmo:
 * 1. Usa due puntatori: slow (avanza di 1) e fast (avanza di 2)
 * 2. Quando fast raggiunge la fine, slow è a metà
 * 3. Divide la lista in corrispondenza di slow
 * 
 */
void split(Node *head, Node **left, Node **right)
{
    // Caso base: lista vuota o con un solo elemento
    if (!head || !head->next) {
        *left = head;
        *right = nullptr;
        return;
    }

    Node *slow = head;       // Puntatore lento (avanza di 1 nodo)
    Node *fast = head->next; // Puntatore veloce (avanza di 2 nodi)

    // Avanza finché fast non raggiunge la fine
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // Imposta le due metà:
    *left = head;         // Prima metà dall'inizio
    *right = slow->next;  // Seconda metà dopo slow
    slow->next = nullptr; // Termina la prima metà
}

/**
 * @brief Implementa il merge sort ricorsivo con thread
 * 
 * Algoritmo:
 * 1. Divide la lista in due metà (split)
 * 2. Ordina ricorsivamente ciascuna metà
 * 3. Fonde le metà ordinate (merge)
 * 
 * Versione parallela:
 * - Usa thread separati per metà grandi (THRESHOLD)
 * - Limita la profondità ricorsiva per overhead thread
 * 
 */
Node *merge_sort(Node *head, int depth = 0)
{
    // Caso base: lista vuota o con un solo elemento
    if (!head || !head->next)
        return head;

    Node *left;
    Node *right;

    // Divide la lista in due metà
    split(head, &left, &right);

    // In base la grandezza della lista utilizzo i thread o meno
    if (depth < THRESHOLD) {
        left = merge_sort(left, depth + 1);
        right = merge_sort(right, depth + 1);
    } else {
        Node *sorted_left = nullptr;
        Node *sorted_right = nullptr;

        std::thread left_thread([&]() { sorted_left = merge_sort(left, depth + 1); });

        std::thread right_thread([&]() { sorted_right = merge_sort(right, depth + 1); });

        // Attesa completamento thread
        left_thread.join();
        right_thread.join();

        left = sorted_left;
        right = sorted_right;

        /* 
         * A cosa servono i thread
         * - Parallelizza l'ordinamento delle due metà
         * - Sfrutta i core multipli della CPU
         * - Riduce il tempo totale di esecuzione
         * - solo per liste grandi (THRESHOLD)
         */
    }

    // Fusione delle due metà ordinate
    return merge(left, right);
}
} // namespace m_list_namespace

ContactList::ContactList(QObject *parent)
    : QObject(parent)
    , m_head(nullptr)
    , m_count(0)
{}

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

    this->sort();

    if (capitalize(m_head->contact.name()) == capitalize(name)) {
        Node* to_delete = m_head;
        m_head = m_head->next;
        delete to_delete;
        m_count--;
        emit dataChanged();
        return true;
    }else{
        Node* current = m_head;
        while (current->next != nullptr && capitalize(current->next->contact.name()) != capitalize(name)) {
            current = current->next;
        }

        if (current->next != nullptr) {
            Node* to_delete = current->next;
            current->next = current->next->next;
            delete to_delete;
            m_count--;
            emit dataChanged();
            return true;
        }
    }

    return false;
}

bool ContactList::updateContact(const QString& originalName, const Contact& updatedContact)
{
    // cerco il contatto in base al nome originale
    Node* node = findNode(originalName);
    if (!node) return false;

    // aggiorno le informazioni del contatto con il nuovo contatto
    node->contact = updatedContact;
    sort();
    emit dataChanged();
    return true;
}

QVector<int> ContactList::search(const QString &query, QTableWidget *table)
{
    QVector<int> originalIndices; // Conserverà gli indici ORIGINALI dei contatti trovati
    table->setRowCount(0);

    Node *current = m_head;
    int originalIndex = 0;
    int displayedRow = 0;
    QString searchStr = query.toUpper();

    while (current) {
        QString name = current->contact.name().toUpper();
        QString email = current->contact.email().toUpper();
        QString phone = current->contact.phone().toUpper();

        if (name.contains(searchStr) || email.contains(searchStr) || phone.contains(searchStr)) {
            table->insertRow(displayedRow);

            // Salva l'indice ORIGINALE nell'item
            QTableWidgetItem *nameItem = new QTableWidgetItem(current->contact.name());
            nameItem->setData(Qt::UserRole, originalIndex);

            QTableWidgetItem *phoneItem = new QTableWidgetItem(current->contact.phone());
            QTableWidgetItem *emailItem = new QTableWidgetItem(current->contact.email());

            table->setItem(displayedRow, 0, nameItem);
            table->setItem(displayedRow, 1, phoneItem);
            table->setItem(displayedRow, 2, emailItem);

            originalIndices.append(originalIndex); // Aggiungi alla lista degli indici

            displayedRow++;
        }
        current = current->next;
        originalIndex++;
    }

    return originalIndices; // Ritorna tutti gli indici originali dei risultati
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

constexpr size_t ContactList::size() const
{
    return m_count;
}

constexpr bool ContactList::isEmpty() const { return m_count == 0; }

bool ContactList::saveToFile(const QString& filePath) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file); // Stream di scrittura per il file
    Node* current = m_head;

    while (current) {
        // Salva solo contatti non vuoti
        if(!current->contact.name().isEmpty() ||
            !current->contact.phone().isEmpty() ||
            !current->contact.email().isEmpty()) {
            // scrivo i contatti nel file
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

    this->clear(); // Pulisci la lista corrente

    QTextStream in(&file); // stream di input per il file
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
    // se this->size() > 1000 attiverà l'ordinamento con i thread
    // altrimenti fare un merge sort classico
    m_head = m_list_namespace::merge_sort(m_head, this->size());

    /*
        il merge sort funziona così.
        dato un container, esso viene diviso in due metà con stesso numero di elementi (se possibile)[1], a questo punto facciamo ricorsivamente la stessa cosa finchè non ci troviamo ad avere sotto container formati da un elemento. Una volta fatto ciò uniamo i sotto continer di singoli elementi nei precedenti sotto container da 2 elementi ma ordinati, facciamo questo cosa ricorsivamente finchè non abbiamo il container originale tutto ordinato[2]


        [1] => continer [38, 27, 43, 10]
                        /               \
                    [38, 27]            [43, 10]
                      /  \                /  \
                   [38]  [27]           [43] [10]
        [2]                      MERGE
                            [10, 27, 38, 43]
    */
}

Contact ContactList::at(size_t index) const
{
    // controllo per verifica se l'indici non è al difuori dei limiti
    if (index < 0 || index >= m_count) {
        return Contact{};
    }

    Node* current = m_head;
    size_t i = 0;
    while (current != nullptr && i < index) {
        current = current->next;
        ++i;
    }

    // se il contatto è stato trovato lo ritorno, altrimenti no
    if (current)
        return current->contact;
    return Contact{};
}

bool ContactList::updateAt(size_t index, const Contact &updatedContact)
{
    // controllo dei limiti
    if (index < 0 || index >= m_count) {
        return false;
    }

    Node* current = m_head;
    size_t i = 0;
    while (current != nullptr && i < index) {
        current = current->next;
        ++i;
    }

    // se il contatto è stato trovato, aggiorno i suoi dati
    if (current) {
        current->contact = updatedContact;
        emit dataChanged();
        return true;
    }

    return false;
}
