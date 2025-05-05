/**
 * @file contatto.cpp
 * @brief Contact class implementation
 */

#include "contatto.hpp"

// costruttore di default
Contact::Contact() : m_name(""), m_phone(""), m_email("") {}


// costruttore con parametri
Contact::Contact(const QString& name, const QString& phone, const QString& email)
    : m_name(name), m_phone(phone), m_email(email) {}


// getter del nome, telefono, email
QString Contact::name()  const { return m_name;  }
QString Contact::phone() const { return m_phone; }
QString Contact::email() const { return m_email; }

// setter del nome, telefono, email
void Contact::setName(const QString& name)   { m_name = name; }
void Contact::setPhone(const QString& phone) { m_phone = phone; }
void Contact::setEmail(const QString& email) { m_email = email; }

// override del operatore di confronto d'uguaglianza tra due contatti
bool Contact::operator==(const Contact& other) const
{
    return m_name == other.m_name &&
           m_phone == other.m_phone &&
           m_email == other.m_email;
}

// override del operatore di confronto di minoranza tra due contatti
bool Contact::operator<(const Contact& other) const
{
    return m_name.toLower() < other.m_name.toLower();
}

// funzione per controllare se la email e' valida
bool Contact::isEmail() const
{
    // essendo la email opzionale, se il suo campo e' vuoto ritorno true,
    //  altrimenti continuo il controllo
    if(m_email.isEmpty()) return true;

    // salvo la posizione del carattere '@', se non viene trovato la sua posizione sara' -1
    //  ritorno quindi false, altrimenti continuo il controllo
    auto position = m_email.indexOf('@');
    if (position == -1) return false;

    // salvo in una QString la posizione del domain tramite e creo un QVector per tutti i domain validi
    QString domain = m_email.mid(position + 1);
    QVector<QString> allowed_domains = {
        "gmail.com",      "outlook.com",   "yahoo.com",     "hotmail.com",
        "icloud.com",     "aol.com",       "zoho.com",      "protonmail.com",
        "yandex.com",     "mail.com",      "gmx.com",       "live.com",
        "inbox.com",      "fastmail.com",  "icloud.com",    "rocketmail.com",
        "rediffmail.com", "cox.net",       "earthlink.net", "att.net",
        "verizon.net",    "sbcglobal.net", "ymail.com",     "me.com",
        "msn.com",        "bluewin.ch",    "t-online.de",   "web.de",
        "libero.it",      "alice.it",      "tin.it",        "outlook.it"
    };
    // scorro tutto il QVector, se trovo una corrispondenza ritorno true, altrimenti false

    bool valid_domain = false;

    for(const QString& allowed_domain : allowed_domains){
        if(domain == allowed_domain){
            valid_domain = true;
            break;
        }
    }

    return valid_domain;
}
