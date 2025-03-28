/**
 * @file contatto.hpp
 * @brief Contact class definition
 */

#ifndef CONTATTO_HPP
#define CONTATTO_HPP

#include <QString>
#include <QVector>
#include <QMetaType>

/**
 * @class Contact
 * @brief Represents a contact with name, phone number, and email
 * @brief Rappresenta un contatto con nome e numero di telefono, può anche avere una email
 */
class Contact
{
public:
    /**
     * @brief Default constructor
     * @brief Costruttore di default
     */
    Contact();

    /**
     * @brief Parameterized constructor
     * @param name The contact's full name
     * @param phone The contact's phone number
     * @param email The contact's email address (optional)
     */
    Contact(const QString& name, const QString& phone, const QString& email = "");

    /**
     * @brief Get the contact's name
     * @return Contact name as QString
     */
    QString name() const;

    /**
     * @brief Get the contact's phone number
     * @return Phone number as QString
     */
    QString phone() const;

    /**
     * @brief Get the contact's email
     * @return Email address as QString
     */
    QString email() const;

    /**
     * @brief check if a string is considered as 'email' (MUST have '@' and a valid domain, such as '@gmail.com')
     * @return true - if all checks pass
     * @return false - if check doesn't pass
     */
    bool isEmail();

    /**
     * @brief Set the contact's name
     * @param name New name to set
     */
    void setName(const QString& name);

    /**
     * @brief Set the contact's phone number
     * @param phone New phone number to set
     */
    void setPhone(const QString& phone);

    /**
     * @brief Set the contact's email
     * @param email New email to set
     */
    void setEmail(const QString& email);

    /**
     * @brief Equality comparison operator
     * @param other Contact to compare with
     * @return True if contacts are identical
     */
    bool operator==(const Contact& other) const;

    /**
     * @brief Less-than comparison operator for sorting
     * @param other Contact to compare with
     * @return True if this contact should come before the other
     */
    bool operator<(const Contact& other) const;

private:
    QString m_name;     ///< Contact's full name
    QString m_phone;    ///< Contact's phone number
    QString m_email;    ///< Contact's email address
};

Q_DECLARE_METATYPE(Contact)

#endif // CONTATTO_HPP
