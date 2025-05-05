/**
 * @file contatto.hpp
 * @brief Definizione della classe Contact per la gestione di contatti (nome, telefono, email)
 * 
 * @details
 * Questo file contiene la definizione della classe Contact, che rappresenta una voce anagrafica
 * completa con nome, numero di telefono e email opzionale. La classe include metodi per la validazione
 * dei dati e operatori per il confronto e l'ordinamento.
 */

#ifndef CONTATTO_HPP
#define CONTATTO_HPP

#include <QMetaType>
#include <QString>
#include <QVector>

/**
 * @class Contact
 * @brief Classe per la gestione di singoli contatti anagrafici
 * 
 * @details
 * La classe rappresenta un contatto completo con:
 * - Nome completo (obbligatorio)
 * - Numero di telefono (obbligatorio)
 * - Indirizzo email (opzionale, con validazione formato)
 * 
 * Fornisce:
 * - Costruttori per creazione contatti
 * - Metodi di accesso e modifica dei campi
 * - Validazione automatica dell'email
 * - Operatori di confronto per uguaglianza e ordinamento
 * 
 */
class Contact
{
public:
    /**
     * @brief Costruttore di default
     * 
     */
    Contact();

    /**
     * @brief Costruttore parametrizzato completo
     * 
     * @param[in] name Nome completo del contatto (non vuoto)
     * @param[in] phone Numero di telefono (non vuoto)
     * @param[in] email Indirizzo email (opzionale, verrà validato se fornito)
     * 
     * @warning Se l'email è fornita deve essere una email valida
     */
    Contact(const QString &name, const QString &phone, const QString &email = "");

    /**
     * @brief Restituisce il nome del contatto
     * @return QString Nome completo corrente
     */
    QString name() const;

    /**
     * @brief Restituisce il numero di telefono
     * @return QString Numero di telefono corrente
     */
    QString phone() const;

    /**
     * @brief Restituisce l'indirizzo email
     * @return QString Email corrente (può essere vuota)
     */
    QString email() const;

    /**
     * @brief Imposta il nome del contatto
     * @param[in] name Nuovo nome completo (non vuoto)
     * 
     * @note Se viene passata una stringa vuota, l'operazione viene ignorata
     */
    void setName(const QString &name);

    /**
     * @brief Imposta il numero di telefono
     * @param[in] phone Nuovo numero (non vuoto)
     * 
     * @note Il numero non viene validato formalmente, ma non può essere vuoto
     */
    void setPhone(const QString &phone);

    /**
     * @brief Imposta l'indirizzo email
     * @param[in] email Nuovo indirizzo email
     * 
     * @details
     * Se l'email non è valida:
     * - Viene scartata
     * - Il campo rimane vuoto
     * - Non viene generato alcun errore
     */
    void setEmail(const QString &email);

    /**
     * @brief Verifica la validità dell'email
     * 
     * Esegue un controllo sintattico sull'indirizzo email:
     * - Deve contenere esattamente un '@'
     * - Deve avere un dominio valido dopo '@'
     * - Non deve contenere spazi
     * 
     * @retval true Se l'email è vuota o valida
     * @retval false Se l'email non è vuota e non valida
     */
    bool isEmail() const;

    /**
     * @brief Operatore di uguaglianza
     * 
     * Confronta tutti i campi tra due contatti (case-sensitive)
     * 
     * @param[in] other Contatto da confrontare
     * @retval true Se nome, telefono e email coincidono
     * @retval false Altrimenti
     */
    bool operator==(const Contact &other) const;

    /**
     * @brief Operatore di ordinamento
     * 
     * Confronta i contatti per nome (case-insensitive) per permettere
     * l'ordinamento alfabetico nelle liste.
     * 
     * @param[in] other Contatto da confrontare
     * @retval true Se questo contatto viene prima nell'ordinamento alfabetico
     */
    bool operator<(const Contact &other) const;

private:
    QString m_name;  /**< Nome completo (case-sensitive) */
    QString m_phone; /**< Numero di telefono (formato libero) */
    QString m_email; /**< Indirizzo email (validato se presente) */
};

/**
 * @brief Registrazione del tipo Contact nel sistema meta-oggetto di Qt
 * 
 * @details
 * Questa macro è necessaria per:
 * - Utilizzare Contact in segnali/slot Qt
 * - Serializzare l'oggetto in QVariant
 * - Integrazione con QObject properties
 */
Q_DECLARE_METATYPE(Contact)

#endif // CONTATTO_HPP
