# 💿 Ante-Millennium OS (Ante-M) x86

![Version](https://img.shields.io/badge/version-0.1_Alpha-blue.svg)
![Architecture](https://img.shields.io/badge/arch-x86_32--bit-red.svg)
![License](https://img.shields.io/badge/license-Freeware-green.svg)

<img width="1280" height="720" alt="miniatura_Ante-M" src="https://github.com/user-attachments/assets/8b46803c-6b04-4937-a5f3-08228682edf3" />

**Ante-Millennium OS** è un sistema operativo sperimentale a 32-bit (x86) scritto interamente da zero (bare-metal) in C e Assembly. 

Nasce come progetto di esplorazione tecnica per ricreare l'architettura dei sistemi operativi classici, unita a un'interfaccia grafica ("Retained Mode") ispirata all'estetica iconica degli anni '90 e dei primi anni 2000. Il sistema è autosufficiente, non si appoggia ad alcuna libreria standard esterna e comunica direttamente con l'hardware.

<img width="1024" height="769" alt="build49" src="https://github.com/user-attachments/assets/bf33a3f2-cc35-432b-8ac2-fc89babfab8f" />


---

## ✨ Architettura e Novità della Build 49 0.1 Alpha

L'ultima evoluzione del sistema introduce cambiamenti strutturali profondi, implementando concetti avanzati di Ingegneria dei Sistemi Operativi:

* 🖥️ **Window Manager Event-Driven e Taskbar:** Il rendering avviene in memoria tramite Double-Buffering per garantire animazioni fluide (zero flickering). È stato introdotto un vero Menu interattivo, una Taskbar con gestione delle icone e animazioni per la chiusura e riduzione a icona delle finestre. Il consumo di CPU è abbattuto da una funzione che risveglia i processi dormienti solo quando l'utente vi interagisce.
* 🛡️ **Ring 3 e Memoria Virtuale (Paging):** Le applicazioni sono ora relegate nello User Mode (Ring 3) per garantire l'isolamento dal Kernel. Il sistema mappa la memoria virtuale assegnando dinamicamente pagine di RAM protette (512 KB per app), gestendo il context-switching dei registri (incluso il TSS per lo stack di emergenza) tramite lo scheduler a 100Hz.
* 🗂️ **File System Ext2:** Il driver ATA PIO interroga il disco rigido. È stato introdotto un *Path Parser* (`ext2_resolve_path`) capace di navigare un albero di directory tramite percorsi assoluti, permettendo la lettura, scrittura e creazione sicura di file e cartelle ovunque nel disco.
* 📚 **Libreria Standard Custom (antem_libc):** Le applicazioni in Ring 3 sono ora supportate da una libreria C proprietaria scritta da zero, dotata di un gestore per l'Heap User Space (`malloc`/`free`), parser di stringhe e un set unificato di API per accedere alle funzionalità del Kernel tramite l'Interrupt `0x80`.

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

Il Terminale integrato permette di interagire direttamente con il Kernel e il File System gerarchico. Digita `help` per una lista rapida:

### File System (Gerarchico)
* `ls [path]` : Elenca file e directory esplorando i percorsi.
* `mkdir [path]` : Crea una nuova cartella nel percorso specificato.
* `cat [path]` : Legge il contenuto di un file.
* `create [p][t]` : Crea file testuali con il contenuto specificato.
* `rm [path]` : Elimina file in modo sicuro (non le cartelle).
* `fsinfo` : Statistiche disco Ext2 leggendo il Superblocco.

### Sistema e Multitasking
* `run [app]` : Lancia eseguibile .edxi o .bin risolvendone il percorso in automatico.
* `ps` : Lista processi attivi, mostrando PID, ID Finestra e stato (RUN/SLEEP).
* `kill [PID]` : Termina un processo e libera la sua RAM.
* `meminfo` : Mostra lo stato dell'Heap e della RAM Kernel eseguendo test di allocazione.
* `time` : Legge data e ora dal chip hardware CMOS.
* `info` / `clear` : Info sistema / Pulisce il terminale.

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
