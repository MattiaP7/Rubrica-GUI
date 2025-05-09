/**
 * @file mainwindow.cpp
 * @brief Main application window implementation
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.hpp"
#include <QMessageBox>
#include <QInputDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_contactList(this)
{
    ui->setupUi(this);
    initializeUI();

    // i connect servono a collegare un segnale ad uno slot
    connect(&m_contactList, &ContactList::dataChanged,
            this, &MainWindow::onContactListChanged);

    // Carico i contatti se esistenti e aggiorno la tabella
    m_contactList.loadFromFile();
    refreshContactTable();
}

MainWindow::~MainWindow()
{
    // Salvo i contatti quando chiudo l'applicazione
    m_contactList.saveToFile();
    delete ui;
}

void MainWindow::initializeUI()
{
    // Configuro la tabella, 3 colonne Nome, Telefono, Email
    // imposto che nella tabella si può selezionare una riga
    ui->tableWidget->setColumnCount(3);
    QStringList headers{"Nome", "Telefono", "Email"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    // Connetto l'input della ricerca
    connect(ui->inputSearch, &QLineEdit::textChanged, this, &MainWindow::on_inputSearch_textChanged);

    // Connetto tutti pulsanti della UI
    connect(ui->btnAggiungi, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(ui->btnConferma, &QPushButton::clicked, this, &MainWindow::onConfirmButtonClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &MainWindow::onCancelButtonClicked);
    connect(ui->btnElimina, &QPushButton::clicked, this, &MainWindow::onRemoveButtonClicked);
    connect(ui->btnModifica, &QPushButton::clicked, this, &MainWindow::onEditButtonClicked);
    connect(ui->btnConferma_2, &QPushButton::clicked, this, &MainWindow::on_btnConferma_2_clicked);
    connect(ui->btnCancel_2, &QPushButton::clicked, this, &MainWindow::on_btnCancel_2_clicked);

    // Applico lo stile
    QString style = applyStyleSheet(isDarkMode());
    this->setStyleSheet(style);

    ui->tableWidget->viewport()->update();
    ui->tableWidget->update();
    ui->inputSearch->update();
}

void MainWindow::refreshContactTable()
{
    m_searchResultsIndices.clear(); // Resetta gli indici
    ui->tableWidget->setRowCount(0);

    const auto contacts = m_contactList.allContacts();
    for (size_t originalIndex = 0; originalIndex < contacts.size(); ++originalIndex) {
        const auto &contact = contacts[originalIndex];

        // Aggiungi riga alla tabella
        size_t row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // Salva l'indice ORIGINALE
        QTableWidgetItem *nameItem = new QTableWidgetItem(contact.name());
        nameItem->setData(Qt::UserRole, originalIndex); // Salva indice originale
        ui->tableWidget->setItem(row, 0, nameItem);

        // Popola gli altri campi
        QTableWidgetItem *phoneItem = new QTableWidgetItem(contact.phone());
        QTableWidgetItem *emailItem = new QTableWidgetItem(contact.email());

        ui->tableWidget->setItem(row, 1, phoneItem);
        ui->tableWidget->setItem(row, 2, emailItem);

        // Memorizza l'indice originale
        m_searchResultsIndices.append(originalIndex); // Riempie il vettore
    }
}

void MainWindow::onAddButtonClicked()
{
    // vado alla pagina per aggiungere un contatto
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::onConfirmButtonClicked(){
    // Salva i valori correnti degli input
    // Utilizzo .trimmed() per rimuovere spazi iniziali o finali indesiderati
    QString currentName = capitalize(ui->inputNome->text().trimmed());
    QString currentPhone = ui->inputTelefono->text().trimmed();
    QString currentEmail = ui->inputEmail->text().trimmed();

    // Validazione nome e telefono, essi NON POSSONO essere vuoti,
    // al contrario della email, ma se inserita deve essere valida
    if(currentName.isEmpty()) {
        showErrorMessage("Errore", "Il nome è obbligatorio");
        ui->inputNome->setFocus();
        return;
    }
    if(currentPhone.isEmpty()) {
        showErrorMessage("Errore", "Il numero di telefono è obbligatorio");
        ui->inputTelefono->setFocus();
        return;
    }

    // converto la stringa del telefono in un numero
    // toULongLong ritorna il valore di ok_phone,
    // se ok_phone == false vuol dire che la stringa non era composta da soli numeri
    bool ok_phone;
    currentPhone.toULongLong(&ok_phone, 10);
    if(!ok_phone){
        QString message = QString("Il numero di telefono deve contenere solo cifre");
        showErrorMessage("Errore", message);
        ui->inputTelefono->selectAll();
        ui->inputTelefono->setFocus();
        return;
    }

    if(currentPhone.size() != 10 || !currentPhone.toLongLong()) {
        QString message = QString("Devi inserire un numero di telefono a 10 cifre, hai inserito %1 cifre").arg(currentPhone.size());
        showErrorMessage("Errore", message);
        ui->inputTelefono->selectAll();
        ui->inputTelefono->setFocus();
        return;
    }

    if(m_contactList.contains(currentPhone)){
        QString message = QString("Numero di telefono già esistente");
        showErrorMessage("Errore", message);
        ui->inputTelefono->selectAll();
        ui->inputTelefono->setFocus();
        return;
    }

    // Validazione email (opzionale ma se presente deve essere valida)
    if(!currentEmail.isEmpty()) {
        Contact tempContact("", "", currentEmail);
        if(!tempContact.isEmail()) {
            showErrorMessage("Errore",
                             "Inserisci un indirizzo email valido o lascia il campo vuoto");
            ui->inputEmail->selectAll();
            ui->inputEmail->setFocus();
            return;
        }
    }

    // Se tutte le validazioni passano, creo un nuovo nodo e lo aggiungo alla lista
    Contact newContact(currentName, currentPhone, currentEmail);
    m_contactList.addContact(newContact);

    // torno alla home page
    ui->stackedWidget->setCurrentIndex(0);

    // Pulisco i campi solo se l'inserimento è avvenuto con successo
    clearInputFields();
}

void MainWindow::onCancelButtonClicked(){
    // torno alla pagina di home
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onRemoveButtonClicked()
{
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        showErrorMessage("Errore", "Seleziona un contatto da eliminare");
        return;
    }

    QString name_to_delete = ui->tableWidget->item(currentRow, 0)->text();

    if(m_contactList.removeContact(name_to_delete)) {
        QMessageBox::information(this, "Successo", "Contatto eliminato");
        refreshContactTable();
    } else {
        QMessageBox::warning(this, "Errore", "Eliminazione fallita");
    }
}

/*
 * SLOT: onEditButtonClicked
 * 
 * Triggerato quando si clicca "Modifica" su un contatto nella tabella filtrata.
 * - Recupera l'indice ORIGINALE dal Qt::UserRole dell'item Nome
 * - Popola i campi di modifica con i dati del contatto ORIGINALE
 * - Salva l'indice originale in m_editingRow per usarlo nella conferma
 */
void MainWindow::onEditButtonClicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= ui->tableWidget->rowCount()) {
        showErrorMessage("Errore", "Seleziona un contatto valido");
        return;
    }

    // Ottieni l'indice ORIGINALE dalla colonna "Nome"
    QTableWidgetItem *nameItem = ui->tableWidget->item(row, 0);
    int originalIndex = nameItem->data(Qt::UserRole).toInt();

    // Popola i campi di modifica
    Contact contact = m_contactList.at(originalIndex);
    ui->inputNome_2->setText(contact.name());
    ui->inputTelefono_2->setText(contact.phone());
    ui->inputEmail_2->setText(contact.email());

    m_editingRow = originalIndex; // Conserva per la conferma
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_btnConferma_2_clicked()
{
    QString nome = ui->inputNome_2->text();
    QString telefono = ui->inputTelefono_2->text();
    QString email = ui->inputEmail_2->text();
    if (nome.isEmpty() || telefono.isEmpty()) {
        showErrorMessage("Errore", "Nome e telefono sono obbligatori");
        return;
    }

    if (telefono.size() != 10 || !telefono.toLongLong()) {
        showErrorMessage("Errore", "Telefono deve avere 10 cifre");
        ui->inputTelefono->setFocus();
        return;
    }

    if (m_contactList.contains(telefono)) {
        Contact contatto_attule = m_contactList.at(m_editingRow);

        if(contatto_attule.phone() != telefono){
            QString message = QString("Numero di telefono già esistente");
            showErrorMessage("Errore", message);
            ui->inputTelefono->selectAll();
            ui->inputTelefono->setFocus();
            return;
        }
    }

    if (!email.isEmpty() && !Contact("", "", email).isEmail()) {
        showErrorMessage("Errore", "Email non valida");
        ui->inputEmail->setFocus();
        return;
    }

    Contact updatedContact(nome, telefono, email);

    // Usa l'indice originale conservato
    m_contactList.updateAt(m_editingRow, updatedContact);

    // Ri-applica la ricerca per aggiornare la vista
    m_contactList.search(ui->inputSearch->text(), ui->tableWidget);
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_btnCancel_2_clicked() {
    // imposto l'indice di selezioni a -1 e torno alla home page
    m_editingRow = -1;
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::onCancelEditClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::resetForm()
{
    // Ripristina il form allo stato originale
    clearInputFields();
    ui->btnAggiungi->setText("Aggiungi");
    connect(ui->btnAggiungi, &QPushButton::clicked,
            this, &MainWindow::onAddButtonClicked);
}

void MainWindow::onContactListChanged()
{
    refreshContactTable();
}

void MainWindow::clearInputFields()
{
    ui->inputNome->clear();
    ui->inputTelefono->clear();
    ui->inputEmail->clear();
    ui->inputNome->setFocus();
}

void MainWindow::showErrorMessage(const QString& title, const QString& message)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(title);
    msgBox.setText(message);
    msgBox.setIcon(QMessageBox::Critical);
    msgBox.setStandardButtons(QMessageBox::Ok);

    // Centra la message box rispetto alla finestra principale
    QPoint center = mapToGlobal(rect().center());
    msgBox.move(center.x() - msgBox.width()/2, center.y() - msgBox.height()/2);

    msgBox.exec();
}

/**
 * - Esegue la ricerca nella contactList usando la query
 * - Salva gli indici originali dei risultati in m_searchResultsIndices
 * - Aggiorna la tabella UI con solo i risultati trovati
 */
void MainWindow::on_inputSearch_textChanged(const QString &query)
{
    m_searchResultsIndices = m_contactList.search(query, ui->tableWidget);
}



