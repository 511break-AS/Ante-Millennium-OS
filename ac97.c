// Ante-Millennium Operating System - Driver AC97
// Copyright (C) 2026  Alberto Sanfelice

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2
// of the License.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, see
// <https://www.gnu.org/licenses/>.



// ========================================================
// DRIVER AUDIO: Intel AC'97 (Demone di Sistema Residente)
// ========================================================

typedef struct {
    void (*print_term)(const char*);
    void* (*kmalloc)(unsigned int);
    void (*kfree)(void*);
    void (*outb)(unsigned short, unsigned char);
    unsigned char (*inb)(unsigned short);
    void (*outw)(unsigned short, unsigned short);
    unsigned short (*inw)(unsigned short);
    void* (*kmalloc_dma)(unsigned int);
    int (*ext2_read_file)(const char*, char*, int, unsigned int*);
    void (*kfree_dma)(void*);
    void (*register_audio)(void*); 
} kernel_api_t;

// --- PROTOTIPI DELLE FUNZIONI ---
int ac97_play_file(const char* path);
void outl(unsigned short port, unsigned int val);
unsigned int inl(unsigned short port);
unsigned int pci_read_dword(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset);
void pci_write_dword(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset, unsigned int val);

typedef struct {
    unsigned int pointer;       
    unsigned short length;      
    unsigned short flags;       
} __attribute__((packed)) ac97_bdl_t;

// =====================================================================
// MEMORIA GLOBALE DEL DRIVER (Sicurezza Flat Binary)
// Inizializzate a 1 per forzare la loro esistenza fisica nel file .drv
// =====================================================================
char* current_audio_ram = (char*)1;
ac97_bdl_t* current_bdl = (ac97_bdl_t*)1;
unsigned short nam_port_global = 1;
unsigned short nabm_port_global = 1;
kernel_api_t* kapi = (kernel_api_t*)1;


// =====================================================================
// FUNZIONE INIZIALE (Offset 0): ORA RICEVE IL SUO VERO INDIRIZZO
// =====================================================================
int _start(kernel_api_t* api, void* my_base_address) {
    // 0. Azzera la memoria globale forzata
    current_audio_ram = 0;
    current_bdl = 0;
    nam_port_global = 0;
    nabm_port_global = 0;
    kapi = api; 

    // 1. RICERCA SCHEDA PCI
    unsigned char ac97_bus = 0, ac97_slot = 0;
    int found = 0;
    for (int b = 0; b < 256; b++) {
        for (int s = 0; s < 32; s++) {
            if (pci_read_dword(b, s, 0, 0) == 0x24158086) { 
                ac97_bus = b; ac97_slot = s; found = 1; break;
            }
        }
        if (found) break;
    }

    if (!found) return -1;

    unsigned int bar0 = pci_read_dword(ac97_bus, ac97_slot, 0, 0x10); 
    unsigned int bar1 = pci_read_dword(ac97_bus, ac97_slot, 0, 0x14); 
    nam_port_global = bar0 & 0xFFFE;
    nabm_port_global = bar1 & 0xFFFE;

    if (nam_port_global == 0 || nabm_port_global == 0) return -1;

    // 2. RESET HARDWARE AC97
    api->outb(nabm_port_global + 0x1B, 0x02); 
    unsigned int cmd_reg = pci_read_dword(ac97_bus, ac97_slot, 0, 0x04);
    pci_write_dword(ac97_bus, ac97_slot, 0, 0x04, cmd_reg | 0x0005); 
    
    api->outw(nam_port_global + 0x00, 1); 
    for(volatile int i = 0; i < 500000; i++); 
    api->outw(nam_port_global + 0x02, 0x0000); 
    api->outw(nam_port_global + 0x18, 0x0000); 

    // ==========================================================
    // 3. LA MAGIA: RILOCAZIONE MANUALE DEL PUNTATORE
    // Calcoliamo l'indirizzo fisico in base a dove ci ha caricato il Kernel
    // ==========================================================
    unsigned int base = (unsigned int)my_base_address;
    unsigned int relative_func_addr = (unsigned int)&ac97_play_file;
    unsigned int absolute_func_addr = base + relative_func_addr;

    // Registriamo l'indirizzo ASSOLUTO E VERO nel Kernel!
    api->register_audio((void*)absolute_func_addr);

    char msg_ok[] = "AC97: Servizio Audio installato e pronto.";
    api->print_term(msg_ok);

    return 1; // Restiamo in memoria per sempre!
}


// =====================================================================
// FUNZIONE DI RIPRODUZIONE (Chiamata su richiesta del Kernel/App)
// =====================================================================
int ac97_play_file(const char* path) {
    if (!kapi || nabm_port_global == 0) return -1;

    kapi->outb(nabm_port_global + 0x1B, 0x02); // Reset DMA PCM Out
    for(volatile int i = 0; i < 50000; i++);   // Pausa per l'hardware

    if (current_audio_ram) { kapi->kfree(current_audio_ram); current_audio_ram = 0; }
    if (current_bdl) { kapi->kfree(current_bdl); current_bdl = 0; }

    unsigned int file_size = 0;
    kapi->ext2_read_file(path, 0, 0, &file_size);
    if (file_size == 0) return 0; 

    current_audio_ram = (char*)kapi->kmalloc(file_size);
    if (!current_audio_ram) return 0; 

    kapi->ext2_read_file(path, current_audio_ram, file_size, &file_size);

    int data_offset = -1;
    unsigned int data_size = 0;
    int offset = 12; 
    while (offset < (int)file_size - 8) {
        char id0 = current_audio_ram[offset]; char id1 = current_audio_ram[offset+1];
        char id2 = current_audio_ram[offset+2]; char id3 = current_audio_ram[offset+3];
        unsigned int chunk_size = *(unsigned int*)(current_audio_ram + offset + 4);
        
        if (id0 == 'd' && id1 == 'a' && id2 == 't' && id3 == 'a') {
            data_offset = offset + 8;
            data_size = chunk_size;
            break;
        }
        unsigned int skip = chunk_size;
        if (skip % 2 != 0) skip++; 
        offset += 8 + skip; 
    }

    if (data_offset == -1) { 
        kapi->kfree(current_audio_ram); current_audio_ram = 0; 
        return 0; 
    }

    current_bdl = (ac97_bdl_t*)kapi->kmalloc(32 * sizeof(ac97_bdl_t));
    if (!current_bdl) { 
        kapi->kfree(current_audio_ram); current_audio_ram = 0; 
        return 0; 
    }

    char* raw_audio = current_audio_ram + data_offset;
    unsigned int max_chunk_bytes = 64000; 
    unsigned int bytes_processed = 0;
    int bdl_index = 0;

    while (bytes_processed < data_size && bdl_index < 32) {
        unsigned int chunk = data_size - bytes_processed;
        if (chunk > max_chunk_bytes) chunk = max_chunk_bytes;
        chunk = chunk - (chunk % 4); 

        current_bdl[bdl_index].pointer = (unsigned int)(raw_audio + bytes_processed);
        current_bdl[bdl_index].length = chunk / 2; 
        current_bdl[bdl_index].flags = 0; 

        bytes_processed += chunk;
        bdl_index++;
    }

    current_bdl[bdl_index - 1].flags = 0x4000; 

    outl(nabm_port_global + 0x10, (unsigned int)current_bdl);
    kapi->outb(nabm_port_global + 0x15, bdl_index - 1);
    kapi->outb(nabm_port_global + 0x1B, 0x01); // PLAY!

    return 1; 
}

// ========================================================
// LE FUNZIONI DI SUPPORTO
// ========================================================
void outl(unsigned short port, unsigned int val) { asm volatile ( "outl %0, %1" : : "a"(val), "Nd"(port) ); }
unsigned int inl(unsigned short port) { unsigned int ret; asm volatile ( "inl %1, %0" : "=a"(ret) : "Nd"(port) ); return ret; }
unsigned int pci_read_dword(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset) {
    unsigned int address = (unsigned int)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    outl(0xCF8, address); return inl(0xCFC);
}
void pci_write_dword(unsigned char bus, unsigned char slot, unsigned char func, unsigned char offset, unsigned int val) {
    unsigned int address = (unsigned int)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | 0x80000000);
    outl(0xCF8, address); outl(0xCFC, val);
}