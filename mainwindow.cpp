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

/*
 * se volessi applicare uno stile per gli headers...
    QStringList headers{"Nome", "Telefono", "Email"};
    QStringList colors{"#FFD700", "#87CEEB", "#90EE90"}; // oro, azzurro, verde chiaro

    ui->tableWidget->setColumnCount(headers.size());

    for (int i = 0; i < headers.size(); ++i) {
        QTableWidgetItem* headerItem = new QTableWidgetItem(headers[i]);
        headerItem->setBackground(QColor(colors[i]));
        headerItem->setTextAlignment(Qt::AlignCenter);
        headerItem->setForeground(QBrush(Qt::black)); // colore del testo
        ui->tableWidget->setHorizontalHeaderItem(i, headerItem);
    }

    // IN CASO TOGLI LE LINEE, IMPOSTA NEI BUTTON DEI COLOR
*/
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);



    // Connetto l'input della ricerca
    connect(
        ui->inputSearch,
        &QLineEdit::textChanged,
        this,
        &MainWindow::on_inputSearch_textChanged
    );


    // Connetto tutti pulsanti della UI
    connect(
        ui->btnAggiungi,
        &QPushButton::clicked,
        this,
        &MainWindow::onAddButtonClicked
    );
    connect(
        ui->btnElimina,
        &QPushButton::clicked,
        this,
        &MainWindow::onRemoveButtonClicked
    );
    connect(
        ui->btnModifica,
        &QPushButton::clicked,
        this,
        &MainWindow::onEditButtonClicked
    );

}

void MainWindow::refreshContactTable()
{
    // Salva la selezione corrente
    int currentRow = ui->tableWidget->currentRow();

    // Pulisci e ripopola la tabella
    ui->tableWidget->setRowCount(0);

    // contacts contiene tutti i contatti della lista
    const auto contacts = m_contactList.allContacts();
    for (const auto& contact : contacts) {
        // Salta i contatti vuoti
        if(
            contact.name().isEmpty() &&
            contact.phone().isEmpty() &&
            contact.email().isEmpty()
        ) continue;

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        auto nameItem = new QTableWidgetItem(capitalize(contact.name()));
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
    // se false vuol dire che la stringa non era composta da soli numeri = errore
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
            showErrorMessage(
                "Errore",
                "Inserisci un indirizzo email valido o lascia il campo vuoto"
            );
            ui->inputEmail->selectAll();
            ui->inputEmail->setFocus();
            return;
        }
    }

    // Se tutte le validazioni passano, creo un nuovo nodo e lo aggiungo alla lista
    Contact newContact(currentName, currentPhone, currentEmail);
    m_contactList.addContact(newContact);

    // Pulisco i campi solo se l'inserimento è avvenuto con successo
    clearInputFields();
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
    // Nasconde tutte le righe inizialmente
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHidden(i, true);
    }

    // Mostra solo le righe che corrispondono ai risultati trovati
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QString name = ui->tableWidget->item(i, 0)->text();
        QString phone = ui->tableWidget->item(i, 1)->text();

        for(const auto& contact : results) {
            if(name == contact.name() || phone == contact.phone()) {
                ui->tableWidget->setRowHidden(i, false);
                break;
            }
        }
    }
}

void MainWindow::clearHighlights()
{
    // Mostra tutte le righe e resetta i colori
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        ui->tableWidget->setRowHidden(i, false);
        for(int j = 0; j < ui->tableWidget->columnCount(); ++j) {
            if(isDarkMode()) {
                ui->tableWidget->item(i, j)->setForeground(Qt::white);
            } else {
                ui->tableWidget->item(i, j)->setBackground(Qt::white);
            }
        }
    }
}

void MainWindow::on_inputSearch_textChanged(const QString &query)
{
    ui->tableWidget->setRowCount(0);
    m_contactList.search(query, ui->tableWidget);
}



