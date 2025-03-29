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

    // Connect contact list changes to refresh function
    connect(&m_contactList, &ContactList::dataChanged,
            this, &MainWindow::onContactListChanged);

    // Load any existing contacts
    m_contactList.loadFromFile();
    refreshContactTable();
}

MainWindow::~MainWindow()
{
    // Save contacts when window closes
    m_contactList.saveToFile();
    delete ui;
}

void MainWindow::initializeUI()
{
    // Configuro la tabella, 3 colonne Nome, Telefono, Email
    ui->tableWidget->setColumnCount(3);
    QStringList headers{"Nome", "Telefono", "Email"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Connetto tutti pulsanti della UI
    connect(ui->btnAggiungi, &QPushButton::clicked,
            this, &MainWindow::onAddButtonClicked);
    connect(ui->btnCerca, &QPushButton::clicked,
            this, &MainWindow::onSearchButtonClicked);
    connect(ui->btnElimina, &QPushButton::clicked,
            this, &MainWindow::onRemoveButtonClicked);
    connect(ui->btnModifica, &QPushButton::clicked,
            this, &MainWindow::onEditButtonClicked);
    // Cambio il testo e la funzionalità del primo bottone, per evitare di crearne un'altro
    connect(ui->tableWidget, &QTableWidget::itemSelectionChanged, this, [this]() {
        if(ui->btnAggiungi->text() == "Conferma Modifica") {
            resetForm();
        }
    });
}

void MainWindow::refreshContactTable()
{
    // Salva la selezione corrente
    int currentRow = ui->tableWidget->currentRow();

    // Pulisci e ripopola la tabella
    ui->tableWidget->setRowCount(0);

    // contacsts contiene tutti i contatti della lista
    const auto contacts = m_contactList.allContacts();
    for (const auto& contact : contacts) {
        // Salta i contatti vuoti
        if(contact.name().isEmpty() && contact.phone().isEmpty() && contact.email().isEmpty()) {
            continue;
        }

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        auto nameItem = new QTableWidgetItem(contact.name());
        nameItem->setFlags(nameItem->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row, 0, nameItem);

        auto phoneItem = new QTableWidgetItem(contact.phone());
        phoneItem->setFlags(phoneItem->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row, 1, phoneItem);

        auto emailItem = new QTableWidgetItem(contact.email());
        emailItem->setFlags(emailItem->flags() | Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        ui->tableWidget->setItem(row, 2, emailItem);
    }

    // Ripristina la selezione se possibile
    if (currentRow >= 0 && currentRow < ui->tableWidget->rowCount()) {
        ui->tableWidget->setCurrentCell(currentRow, 0);
    }
}

void MainWindow::onAddButtonClicked()
{
    // Salva i valori correnti degli input
    // Utilizzo .trimmed() per rimuovere spazi iniziali o finali indesiderati
    QString currentName = ui->inputNome->text().trimmed();
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


    // Validazione email (opzionale ma se presente deve essere valida)
    if(!currentEmail.isEmpty()) {
        Contact tempContact("", "", currentEmail);
        if(!tempContact.isEmail()) {
            showErrorMessage("Errore", "Inserisci un indirizzo email valido o lascia il campo vuoto");
            ui->inputEmail->selectAll();
            ui->inputEmail->setFocus();
            return;
        }
    }

    // Se tutte le validazioni passano
    Contact newContact(currentName, currentPhone, currentEmail);
    m_contactList.addContact(newContact);

    // Pulisci i campi solo se l'inserimento è avvenuto con successo
    clearInputFields();
}


void MainWindow::onSearchButtonClicked()
{
    // la query rappresenta l'input della ricerca
    QString query = QInputDialog::getText(this, "Cerca Contatti", "Inserisci nome o numero di telefono:");
    if(query.isEmpty()) {
        clearHighlights();
        return;
    }

    // results è un QVector<Contact> visto che contiene tutti i contatti trovati da searchContacts(query)
    const auto results = m_contactList.searchContacts(query);
    if(results.isEmpty()) {
        QMessageBox::information(this, "Risultati", "Nessun contatto trovato");
        clearHighlights();
    } else {
        // stampo il numero di contatti trovati e evidenzio nella tabella i contatti che matchano la query
        highlightSearchResults(results);
        QString message = QString("Trovati %1 contatti").arg(results.size());
        QMessageBox::information(this, "Risultati", message);
    }
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

void MainWindow::onEditButtonClicked()
{
    // 1. Verifica selezione
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        showErrorMessage("Errore", "Seleziona un contatto da modificare");
        return;
    }

    // 2. Recupera dati esistenti
    QString originalName = ui->tableWidget->item(currentRow, 0)->text();
    QString originalPhone = ui->tableWidget->item(currentRow, 1)->text();
    QString originalEmail = ui->tableWidget->item(currentRow, 2)->text();

    // 3. Popola i campi di input senza chiedere conferma
    ui->inputNome->setText(originalName);
    ui->inputTelefono->setText(originalPhone);
    ui->inputEmail->setText(originalEmail);

    // 4. Cambia testo del pulsante "Aggiungi" in "Conferma Modifica"
    ui->btnAggiungi->setText("Conferma");
    ui->btnAggiungi->disconnect();
    connect(ui->btnAggiungi, &QPushButton::clicked, this, [this, originalName]() {
        confirmEdit(originalName);
    });
    ui->btnElimina->hide();
    ui->btnCerca->hide();
    ui->btnModifica->hide();
}

void MainWindow::confirmEdit(const QString& originalName)
{
    // 1. Ottieni nuovi valori
    QString newName = ui->inputNome->text().trimmed();
    QString newPhone = ui->inputTelefono->text().trimmed();
    QString newEmail = ui->inputEmail->text().trimmed();

    // 2. Validazione
    if(newName.isEmpty() || newPhone.isEmpty()) {
        showErrorMessage("Errore", "Nome e telefono sono obbligatori");
        return;
    }

    if(newPhone.size() != 10 || !newPhone.toLongLong()) {
        showErrorMessage("Errore", "Telefono deve avere 10 cifre");
        ui->inputTelefono->setFocus();
        return;
    }

    if(!newEmail.isEmpty() && !Contact("", "", newEmail).isEmail()) {
        showErrorMessage("Errore", "Email non valida");
        ui->inputEmail->setFocus();
        return;
    }

    // 3. Esegui modifica
    Contact updatedContact(newName, newPhone, newEmail);
    if(m_contactList.updateContact(originalName, updatedContact)) {
        QMessageBox::information(this, "Successo", "Contatto modificato");
        refreshContactTable();
        resetForm();
    } else {
        showErrorMessage("Errore", "Modifica fallita");
    }

    // Dopo avere eseguito la modifica ripristino i bottoni
    ui->btnElimina->show();
    ui->btnCerca->show();
    ui->btnModifica->show();

    // Reimposta il testo del pulsante "Aggiungi" se necessario
    ui->btnAggiungi->setText("Aggiungi");

    // Puoi anche ricollegare il segnale del pulsante "Aggiungi" se necessario
    ui->btnAggiungi->disconnect();
    connect(ui->btnAggiungi, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
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
    clearHighlights(); // Rimuovi evidenziazioni quando i dati cambiano
}

Contact MainWindow::getContactFromInput() const
{
    return Contact(
        ui->inputNome->text().trimmed(),
        ui->inputTelefono->text().trimmed(),
        ui->inputEmail->text().trimmed()
    );
}

bool MainWindow::validateInput() const
{
    return !ui->inputNome->text().trimmed().isEmpty() &&
           !ui->inputTelefono->text().trimmed().isEmpty();
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


void MainWindow::highlightSearchResults(const QVector<Contact>& results)
{
    clearHighlights();

    for(int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QString name = ui->tableWidget->item(i, 0)->text();
        QString phone = ui->tableWidget->item(i, 1)->text();



        for(const auto& contact : results) {
            if(name == contact.name() || phone == contact.phone()) {
                for(int j = 0; j < ui->tableWidget->columnCount(); ++j) {
                    if(isDarkMode()){
                        ui->tableWidget->item(i, j)->setForeground(Qt::cyan);
                    }else{
                        ui->tableWidget->item(i, j)->setForeground(Qt::red);
                    }
                }
                break;
            }
        }
    }
}

void MainWindow::clearHighlights()
{
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j) {
            if(isDarkMode()){
                ui->tableWidget->item(i, j)->setForeground(Qt::white);
            }else{
                ui->tableWidget->item(i, j)->setBackground(Qt::white);
            }
        }
    }
}

