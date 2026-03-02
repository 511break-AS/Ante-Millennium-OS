// ==========================================
// testgui.c - Prima app con finestre EDXI!
// ==========================================
void yield_cpu();

typedef struct {
    int x, y, clicked;
} mouse_state_t;
void get_mouse(mouse_state_t* state);

typedef struct {
    int type;
    int x, y, w, h;
    unsigned int color1;
    unsigned int color2;
    char text[16];
} gui_element_t;

void draw_gui(gui_element_t* elem);

void main() {
    // Struttura di pulizia (type 0)
    gui_element_t clear = {0, 0,0,0,0, 0,0, ""};
    
    // I nostri elementi grafici
    gui_element_t text = {3, 20, 20, 0, 0, 0xFF000000, 0, "Premi qui!"};
    gui_element_t btn1 = {2, 20, 50, 100, 30, 0xFFC0C0C0, 0xFF000000, "Cliccami"};

    // Inviamo i disegni al Kernel!
    draw_gui(&clear);
    draw_gui(&text);
    draw_gui(&btn1);

    mouse_state_t m;
    int prev_click = 0;
    int toggle = 0;

    while(1) {
        get_mouse(&m);

        if (m.clicked && !prev_click) {
            // Se l'utente ha cliccato esattamente dentro l'area del bottone...
            if (m.x >= btn1.x && m.x <= btn1.x + btn1.w && m.y >= btn1.y && m.y <= btn1.y + btn1.h) {
                toggle = !toggle;
                
                if (toggle) {
                    btn1.color1 = 0xFF00FF00; // Sfondo verde
                    btn1.text[0] = 'V'; btn1.text[1] = 'A'; btn1.text[2] = 'I'; btn1.text[3] = '!'; btn1.text[4] = '\0';
                } else {
                    btn1.color1 = 0xFFC0C0C0; // Sfondo grigio
                    btn1.text[0] = 'C'; btn1.text[1] = 'l'; btn1.text[2] = 'i'; btn1.text[3] = 'c'; btn1.text[4] = 'c'; btn1.text[5] = 'a'; btn1.text[6] = 'm'; btn1.text[7] = 'i'; btn1.text[8] = '\0';
                }
                
                // Aggiorniamo la finestra svuotandola e ridisegnando gli elementi modificati!
                draw_gui(&clear);
                draw_gui(&text);
                draw_gui(&btn1);
            }
        }
        prev_click = m.clicked;
        yield_cpu(); // Dormiamo per non pesare sulla CPU!
    }
}
void draw_gui(gui_element_t* elem) {
    asm volatile ("int $0x80" : : "a"(7), "b"((unsigned int)elem));
}
void get_mouse(mouse_state_t* state) { asm volatile ("int $0x80" : : "a"(5), "b"((unsigned int)state)); }
void yield_cpu() { asm volatile ("int $0x80" : : "a"(6)); }