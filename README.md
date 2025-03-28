# Gestore Rubrica Contatti Qt

![Qt](https://img.shields.io/badge/Qt-6.0%2B-green)
![c++](https://img.shields.io/badge/-C++-blue?logo=cplusplus)

Un'applicazione desktop per la gestione di una rubrica contatti sviluppata con Qt C++.

![image alt text](https://mgdanv9sq0.ufs.sh/f/UyBJYEvFMyYw0FWf84QsagDFYmt5o3PjNecOE4XdTJkBCU6b "Interfaccia applicazione")

## Caratteristiche Principali

- 🧑‍💻 Interfaccia grafica intuitiva
- 📝 Aggiunta, modifica e rimozione di contatti
- 🔍 Funzionalità di ricerca avanzata
- 💾 Salvataggio e caricamento automatico in formato CSV
- 🗃️ Ordinamento automatico dei contatti
- 📱 Supporto per nome, telefono e email

## Requisiti di Sistema

- Qt 6.0 o superiore
- Compilatore C++17 compatibile
- CMake 3.5+

## Installazione

1. Clonare il repository:
```bash
git clone https://github.com/MattiaP7/Rubrica-GUI
cd rubrica-qt
```
ricorda di eliminare la cartella `.git`
2. Compilare con CMake
```bash
mkdir build
cd build
cmake ..
make
```
3. Esegui l'applicazione

# Utilizzo
1. Aggiungere un contatto
	- Compilare i campi Nome, Telefono e (opzionale) Email
	- Cliccare "Aggiungi"
2. Cercare contatti
	- Inserire testo nella barra di ricerca
	- I risultati verranno evidenziati
3. Modificare un contatto
	- Selezionare il contatto dalla tabella
	- Modificare i campi desiderati
	- Cliccare "Modifica"
4. Eliminare un contatto
	- Selezionare il contatto dalla tabella
	- Cliccare "Rimuovi"