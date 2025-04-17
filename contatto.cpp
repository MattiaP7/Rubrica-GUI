/**
 * @file contatto.cpp
 * @brief Contact class implementation
 */

#include "contatto.hpp"

Contact::Contact() : m_name(""), m_phone(""), m_email("") {}

Contact::Contact(const QString& name, const QString& phone, const QString& email)
    : m_name(name), m_phone(phone), m_email(email) {}

QString Contact::name()  const { return m_name;  }
QString Contact::phone() const { return m_phone; }
QString Contact::email() const { return m_email; }

void Contact::setName(const QString& name)   { m_name = name; }
void Contact::setPhone(const QString& phone) { m_phone = phone; }
void Contact::setEmail(const QString& email) { m_email = email; }

bool Contact::operator==(const Contact& other) const
{
    return m_name == other.m_name &&
           m_phone == other.m_phone &&
           m_email == other.m_email;
}

bool Contact::operator<(const Contact& other) const
{
    return m_name.toLower() < other.m_name.toLower();
}

bool Contact::isEmail(){
    if(m_email.isEmpty()) return true; // essendo opzionale

    auto position = m_email.indexOf('@');
    if (position == -1) return false;

    QString domain = m_email.mid(position + 1);
    // questo vector contiene domains riconosciuti
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
    bool valid_domain = false;

    for(const QString& allowed_domain : allowed_domains){
        if(domain == allowed_domain){
            valid_domain = true;
            break;
        }
    }

    return valid_domain;
}
