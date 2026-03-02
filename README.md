# 💿 Ante-Millennium OS (Ante-M) x86

![Version](https://img.shields.io/badge/version-0.1_Alpha-blue.svg)
![Architecture](https://img.shields.io/badge/arch-x86_32--bit-red.svg)
![License](https://img.shields.io/badge/license-Freeware-green.svg)

<img width="1280" height="720" alt="miniatura_Ante-M" src="https://github.com/user-attachments/assets/8b46803c-6b04-4937-a5f3-08228682edf3" />


**Ante-Millennium OS** è un sistema operativo sperimentale a 32-bit (x86) scritto interamente da zero (bare-metal) in C e Assembly. 

Nasce come progetto di esplorazione tecnica per ricreare l'architettura dei sistemi operativi classici, unita a un'interfaccia grafica ("Retained Mode") ispirata all'estetica iconica degli anni '90 e dei primi anni 2000. Il sistema è autosufficiente, non si appoggia ad alcuna libreria standard (compilato in `-ffreestanding -nostdlib`) e comunica direttamente con l'hardware.

<img width="1022" height="765" alt="screenshot" src="https://github.com/user-attachments/assets/802f7929-1f46-44b2-8f84-81db2533a83b" />

---

## ✨ Architettura e Core Features

Nonostante la sua natura sperimentale, Ante-M implementa concetti avanzati di Ingegneria del Software:

* 🖥️ **Window Manager e GUI "Retained Mode":** Sistema gerarchico di finestre sovrapponibili con gestione dinamica dello Z-Index. Il rendering avviene interamente in memoria tramite Double-Buffering prima di essere inviato alla VRAM, garantendo animazioni fluide e assenza totale di sfarfallii (flickering). Include patch algoritmiche "anti-fantasma" per la gestione rigorosa dei click del mouse.
* ⏱️ **Multitasking Preemptive:** Il cuore del sistema. Uno scheduler basato su un Programmable Interval Timer (PIT) hardware impostato a 100Hz gestisce il context-switching dei registri della CPU. Questo permette di eseguire processi e applicazioni in parallelo in totale sicurezza tramite un algoritmo Round-Robin.
* 💾 **Driver Disco ATA PIO & File System Ext2:** Stack I/O di archiviazione scritto da zero. Il kernel interroga il disco rigido bypassando il BIOS, leggendo le Tabelle Inode e le Bitmap del File System Ext2. È in grado di allocare dinamicamente blocchi, creare file testuali e manipolare directory entry in tempo reale.
* 🧠 **Heap Memory Manager:** Sistema custom di allocazione dinamica della RAM (`kmalloc` e `kfree`) a partire dal 16° Megabyte. Implementa algoritmi di *First-Fit* con splitting dei blocchi e deframmentazione automatica ("on-the-fly" merging).
* ⚙️ **Application Programming Interface (API):** Un portiere Assembly intercetta l'Interrupt `0x80` per fornire un set di chiamate di sistema (Syscall) sicure, permettendo ai processi utente (`.bin` e `.edxi`) di stampare a schermo, leggere il mouse e disegnare sulla propria interfaccia grafica dedicata.

---

## 🚀 Guida all'Avvio (Quick Start)

Attualmente il sistema è distribuito sotto forma di file binari pronti per essere eseguiti in un emulatore. Si raccomanda **QEMU**.

1. Scarica l'archivio `.zip` dall'ultima [Release](https://github.com/511break-AS/Ante-Millennium-OS/releases) disponibile.
2. Estrai i file.
3. Apri un terminale nella cartella di estrazione e lancia questo comando (varia in base al sistema operativo utilizzato):

```bash
qemu-system-i386 -kernel Ante-M.bin -drive file=disk.img,format=raw,index=0,media=disk -m 32M
```

> **Nota Tecnica:** Il parametro `-m 32M` è strettamente necessario. Il Kernel mappa le sue strutture dati e lo spazio di Heap allocabile richiedendo matematicamente almeno 32 Megabyte di RAM per un'esecuzione sicura.

---

## ⌨️ Comandi Terminale Disponibili

Il Terminale integrato permette di interagire direttamente con il Kernel e il File System. Digita `help` per una lista rapida, oppure prova i seguenti comandi:

### File System (Ext2)
* `ls` : Esplora la directory principale (Root) stampando la lista dei file.
* `cat [nomefile]` : Cerca il file richiesto e ne stampa il contenuto a schermo gestendo l'impaginazione.
* `rm [nomefile]` : Elimina in modo sicuro un file, smontando la sua Directory Entry e riciclando lo spazio nelle Bitmap.
* `fsinfo` : Interroga il Superblocco del disco mostrando le statistiche totali (Inode e Blocchi formattati).

### Sistema e Multitasking
* `run [app.bin/.edxi]` : Caricatore ed esecutore di programmi. Legge l'header del file, alloca l'eseguibile in RAM e genera un nuovo processo.
* `ps` : Mostra la tabella dei processi attivi, indicando PID, ID Finestra e Stato (RUN/SLEEP).
* `kill [PID]` : Termina forzatamente il processo specificato, de-allocando istantaneamente lo stack privato e la RAM base utilizzata.
* `meminfo` : Analizza lo stato della memoria dinamica (Heap) stampando byte utilizzati, byte liberi e frammentazione.
* `time` : Interroga il Real Time Clock (CMOS) per data e ora.
* `clear` : Pulisce la memoria video del terminale.

---

## 👨‍💻 L'Autore

Il creatore e sviluppatore principale di Ante-Millennium OS è **Alberto Sanfelice**.

Sul web, la sua identità digitale è storicamente legata al numero **511** (da cui deriva l'estensione proprietaria degli eseguibili di sistema, `.edxi`). 

📹 **Canale YouTube Ufficiale:** [511break](https://www.youtube.com/@511break) - *Segui i "dietro le quinte" e lo sviluppo del codice.*

---

## ⚖️ Licenza (Freeware)

Questo software è concesso in licenza d'uso gratuita, non venduto. Copyright (c) 2026 Alberto Sanfelice (511break). Tutti i diritti riservati.

Il sistema operativo è gratuito per qualsiasi uso personale o educativo. Sei libero di copiare, distribuire e condividere i file binari, purché il software venga offerto gratuitamente e l'autore originale venga sempre citato chiaramente.
Contrariamente ai classici software proprietari, **l'autore incoraggia la curiosità tecnica:** sei esplicitamente autorizzato (e incoraggiato) a disassemblare, retro-ingegnerizzare o studiare i file binari per scopi didattici.

*Al momento, il codice sorgente originale in C e Assembly non è pubblico.*

> **Disclaimer:** Questo sistema operativo è un progetto sperimentale ("bare-metal") fornito "COSÌ COM'È", senza alcuna garanzia esplicita o implicita. Per i dettagli completi, leggi il file `LICENSE.TXT` incluso nella release.
