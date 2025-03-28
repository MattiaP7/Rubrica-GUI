/**
 * @file contatto.hpp
 * @brief Contact class definition with name, phone and email
 * @brief Definizione della classe Contact con nome, telefono e email
 */

#ifndef CONTATTO_HPP
#define CONTATTO_HPP

#include <QString>
#include <QVector>
#include <QMetaType>

/**
 * @class Contact
 * @brief Represents a contact entry with name, phone number and optional email
 * @brief Rappresenta una voce della rubrica con nome, numero di telefono e email opzionale
 *
 * This class encapsulates all information about a single contact,
 * providing methods to access and modify the data, and validation capabilities.
 *
 * Questa classe incapsula tutte le informazioni su un singolo contatto,
 * fornendo metodi per accedere e modificare i dati, e capacità di validazione.
 */
class Contact
{
public:
    /**
     * @brief Default constructor creates an empty contact
     * @brief Costruttore di default crea un contatto vuoto
     */
    Contact();

    /**
     * @brief Parameterized constructor creates a contact with given data
     * @brief Costruttore parametrizzato crea un contatto con i dati forniti
     *
     * @param name Full name of the contact (cannot be empty)
     * @param phone Phone number of the contact (cannot be empty)
     * @param email Email address (optional, must be valid if provided)
     *
     * @param name Nome completo del contatto (non può essere vuoto)
     * @param phone Numero di telefono del contatto (non può essere vuoto)
     * @param email Indirizzo email (opzionale, deve essere valido se fornito)
     */
    Contact(const QString& name, const QString& phone, const QString& email = "");

    /**
     * @brief Gets the contact's full name
     * @brief Ottiene il nome completo del contatto
     *
     * @return QString containing the name
     *
     * @return QString contenente il nome
     */
    QString name() const;

    /**
     * @brief Gets the contact's phone number
     * @brief Ottiene il numero di telefono del contatto
     *
     * @return QString containing the phone number
     *
     * @return QString contenente il numero di telefono
     */
    QString phone() const;

    /**
     * @brief Gets the contact's email address
     * @brief Ottiene l'indirizzo email del contatto
     *
     * @return QString containing the email (may be empty)
     *
     * @return QString contenente l'email (può essere vuota)
     */
    QString email() const;

    /**
     * @brief Validates the email format
     * @brief Valida il formato dell'email
     *
     * Checks if the email contains '@' and a valid domain.
     * Verifica se l'email contiene '@' e un dominio valido.
     *
     * @return true if email is valid (or empty), false otherwise
     *
     * @return true se l'email è valida (o vuota), false altrimenti
     */
    bool isEmail();

    /**
     * @brief Sets the contact's full name
     * @brief Imposta il nome completo del contatto
     *
     * @param name New name (cannot be empty)
     *
     * @param name Nuovo nome (non può essere vuoto)
     */
    void setName(const QString& name);

    /**
     * @brief Sets the contact's phone number
     * @brief Imposta il numero di telefono del contatto
     *
     * @param phone New phone number (cannot be empty)
     *
     * @param phone Nuovo numero di telefono (non può essere vuoto)
     */
    void setPhone(const QString& phone);

    /**
     * @brief Sets the contact's email address
     * @brief Imposta l'indirizzo email del contatto
     *
     * @param email New email address (will be validated)
     *
     * @param email Nuovo indirizzo email (sarà validato)
     */
    void setEmail(const QString& email);

    /**
     * @brief Equality comparison operator
     * @brief Operatore di confronto di uguaglianza
     *
     * Two contacts are considered equal if all their fields match.
     * Due contatti sono considerati uguali se tutti i loro campi corrispondono.
     *
     * @param other Contact to compare with
     * @return true if contacts are identical, false otherwise
     *
     * @param other Contatto con cui confrontare
     * @return true se i contatti sono identici, false altrimenti
     */
    bool operator==(const Contact& other) const;

    /**
     * @brief Less-than comparison operator for sorting
     * @brief Operatore di confronto minore-per per l'ordinamento
     *
     * Compares contacts by name (case-insensitive) for sorting purposes.
     * Confronta i contatti per nome (case-insensitive) per scopi di ordinamento.
     *
     * @param other Contact to compare with
     * @return true if this contact should come before the other
     *
     * @param other Contatto con cui confrontare
     * @return true se questo contatto dovrebbe venire prima dell'altro
     */
    bool operator<(const Contact& other) const;

private:
    QString m_name;     /**< Contact's full name / Nome completo del contatto */
    QString m_phone;    /**< Contact's phone number / Numero di telefono del contatto */
    QString m_email;    /**< Contact's email address / Indirizzo email del contatto */
};

/**
 * @brief Registers Contact type with Qt's meta-object system
 * @brief Registra il tipo Contact con il sistema meta-oggetto di Qt
 *
 * This macro makes the Contact class usable in Qt's signal/slot mechanism.
 * Questa macro rende la classe Contact utilizzabile nel meccanismo di segnali/slot di Qt.
 */
Q_DECLARE_METATYPE(Contact)

#endif // CONTATTO_HPP
