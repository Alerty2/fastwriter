#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <unistd.h>
#include "libuiohook/include/uiohook.h"
#include "json/cJSON.h"
#include <wchar.h>
#include <locale.h>

#define OS 0 // 0 -> Linux | 1 -> Windows | 2 -> MAC

#define WORDS_NUM 64
#define CHAR_PER_WORD 256
#define SUBSTITUTE_WORD_CHAR 1024

char words[WORDS_NUM][2][CHAR_PER_WORD] = {
    {"nombre", "Juan"},
    {"trabajo", "Programador"},
    {"hobby", "Bailar"}
};
char currently_typing[SUBSTITUTE_WORD_CHAR] = "";
int ptr = 0;
char to_substitute[SUBSTITUTE_WORD_CHAR];
char current_word[SUBSTITUTE_WORD_CHAR] = "";
int writing = 0;

void load_words_configuration(char words[][2][CHAR_PER_WORD], const char *json_string){
    cJSON *json = cJSON_Parse(json_string);
    if (!json) return;

    printf("JSON parsed\n");

    // Get length
    cJSON *length_prop = cJSON_GetObjectItemCaseSensitive(json, "Length");
    int length = 0;
    if (length_prop && cJSON_IsString(length_prop) && length_prop->valuestring)
        length = atoi(length_prop->valuestring);

    // Load the data into the array
    cJSON *item = json->child; // First member
    for (int i = 0; item && i < length; ++i, item = item->next) {
        if (item->string) strncpy(words[i][0], item->string, CHAR_PER_WORD-1);
        else words[i][0][0] = '\0';

        if (cJSON_IsString(item) && item->valuestring) strncpy(words[i][1], item->valuestring, CHAR_PER_WORD-1);
        else words[i][1][0] = '\0';

        words[i][0][CHAR_PER_WORD-1] = '\0';
        words[i][1][CHAR_PER_WORD-1] = '\0';
    }

    cJSON_Delete(json);
}

char get_char_from_event(uiohook_event *event) {
    uint16_t code = event->data.keyboard.keycode;
    uint16_t mask = event->mask;

    switch (code) {
        // Letters
        case VC_A: return 'a';
        case VC_B: return 'b';
        case VC_C: return 'c';
        case VC_D: return 'd';
        case VC_E: return 'e';
        case VC_F: return 'f';
        case VC_G: return 'g';
        case VC_H: return 'h';
        case VC_I: return 'i';
        case VC_J: return 'j';
        case VC_K: return 'k';
        case VC_L: return 'l';
        case VC_M: return 'm';
        case VC_N: return 'n';
        case VC_O: return 'o';
        case VC_P: return 'p';
        case VC_Q: return 'q';
        case VC_R: return 'r';
        case VC_S: return 's';
        case VC_T: return 't';
        case VC_U: return 'u';
        case VC_V: return 'v';
        case VC_W: return 'w';
        case VC_X: return 'x';
        case VC_Y: return 'y';
        case VC_Z: return 'z';

        // Numbers
        case VC_1: return (mask & MASK_SHIFT) ? '!' : '1';
        case VC_2: return (mask & MASK_SHIFT) ? '"' : '2';
        case VC_3: return (mask & MASK_SHIFT) ? '*' : '3';
        case VC_4: return (mask & MASK_SHIFT) ? '$' : '4';
        case VC_5: return (mask & MASK_SHIFT) ? '%' : '5';
        case VC_6: return (mask & MASK_SHIFT) ? '&' : '6';
        case VC_7: return (mask & MASK_SHIFT) ? '/' : '7';
        case VC_8: return (mask & MASK_SHIFT) ? '(' : '8';
        case VC_9: return (mask & MASK_SHIFT) ? ')' : '9';
        case VC_0: return (mask & MASK_SHIFT) ? '=' : '0';

        // Numeric keyboard
        case VC_KP_0: return '0';
        case VC_KP_1: return '1';
        case VC_KP_2: return '2';
        case VC_KP_3: return '3';
        case VC_KP_4: return '4';
        case VC_KP_5: return '5';
        case VC_KP_6: return '6';
        case VC_KP_7: return '7';
        case VC_KP_8: return '8';
        case VC_KP_9: return '9';

        // Special chars
        case VC_SPACE: return ' ';
        case VC_ENTER: return '\n';
        case VC_TAB:   return '\t';

        default: return '\0';
    }
}

char* look_for_word(char word[]){
    for (int i = 0; i < WORDS_NUM; i++){
        if (strcmp(word, words[i][0]) == 0){
            return words[i][1];
        }
    }
    return "None";
}

void type_key(uint16_t keycode) {
    uiohook_event event;
    memset(&event, 0, sizeof(uiohook_event));

    // Configure pressing event
    event.type = EVENT_KEY_PRESSED;
    event.data.keyboard.keycode = keycode;
    hook_post_event(&event);

    usleep(10000); 

    // Configure releasing event
    event.type = EVENT_KEY_RELEASED;
    hook_post_event(&event);
}

void type_hex_char(char c) {
    switch(c) {
        case '0': type_key(VC_0); break;
        case '1': type_key(VC_1); break;
        case '2': type_key(VC_2); break;
        case '3': type_key(VC_3); break;
        case '4': type_key(VC_4); break;
        case '5': type_key(VC_5); break;
        case '6': type_key(VC_6); break;
        case '7': type_key(VC_7); break;
        case '8': type_key(VC_8); break;
        case '9': type_key(VC_9); break;
        case 'a': type_key(VC_A); break;
        case 'b': type_key(VC_B); break;
        case 'c': type_key(VC_C); break;
        case 'd': type_key(VC_D); break;
        case 'e': type_key(VC_E); break;
        case 'f': type_key(VC_F); break;
    }
}

int get_digit(int number, int position) {
    // Always positive
    if (number < 0) {
        number = -number;
    }
    int divisor = (int)pow(10, position);
    return (number / divisor) % 10;
}

void type_unicode_linux(wchar_t wc) {
    uiohook_event event = {0};

    // Press Ctrl + Shift
    event.type = EVENT_KEY_PRESSED;
    event.data.keyboard.keycode = VC_CONTROL_L;
    hook_post_event(&event);
    usleep(5000);
    event.data.keyboard.keycode = VC_SHIFT_L;
    hook_post_event(&event);
    usleep(5000);

    // Press U
    type_key(VC_U);

    // Release Ctrl + Shift
    event.type = EVENT_KEY_RELEASED;
    event.data.keyboard.keycode = VC_SHIFT_L;
    hook_post_event(&event);
    usleep(5000);
    event.data.keyboard.keycode = VC_CONTROL_L;
    hook_post_event(&event);
    usleep(5000);

    // Transform wchar to hex
    char hex_str[10];
    sprintf(hex_str, "%x", wc);

    // Write hex code
    for (int i = 0; hex_str[i] != '\0'; i++) {
        type_hex_char(hex_str[i]);
    }

    // Press space (to confirm)
    type_key(VC_SPACE);
}

void type_unicode_windows(wchar_t wc) {

    // Select Alt code
    int number = 0;
    switch (wc) {
        case L'á': number = 225; break;
        case L'é': number = 233; break;
        case L'í': number = 237; break;
        case L'ó': number = 243; break;
        case L'ú': number = 250; break;
        case L'ñ': number = 241; break;
        case L'Á': number = 193; break;
        case L'É': number = 201; break;
        case L'Í': number = 205; break;
        case L'Ó': number = 211; break;
        case L'Ú': number = 218; break;
        case L'Ñ': number = 209; break;
        default: return;
    }

    // Press Alt
    uiohook_event alt_event = {0};
    alt_event.type = EVENT_KEY_PRESSED;
    alt_event.data.keyboard.keycode = VC_ALT_L;
    hook_post_event(&alt_event);
    usleep(5000);

    // Write the number
    for (int i = 3; i >= 0; i--) {
        int digit = get_digit(number, i);
        switch (digit) {
            case 0: type_key(VC_KP_0); break;
            case 1: type_key(VC_KP_1); break;
            case 2: type_key(VC_KP_2); break;
            case 3: type_key(VC_KP_3); break;
            case 4: type_key(VC_KP_4); break;
            case 5: type_key(VC_KP_5); break;
            case 6: type_key(VC_KP_6); break;
            case 7: type_key(VC_KP_7); break;
            case 8: type_key(VC_KP_8); break;
            case 9: type_key(VC_KP_9); break;
        }
    }

    // Release alt
    alt_event.type = EVENT_KEY_RELEASED;
    alt_event.data.keyboard.keycode = VC_ALT_L;
    hook_post_event(&alt_event);
    usleep(5000);
}

void type_unicode_mac(wchar_t wc) {
    // TODO
}

uint16_t get_keycode_from_wchar(wchar_t c, bool *needs_shift, bool *needs_alt) {
    *needs_shift = false;
    *needs_alt = false;

    // Capital letters
    if (c >= L'A' && c <= L'Z') {
        *needs_shift = true;
        c = c + (L'a' - L'A'); 
    }

    switch (c) {
        // Letters
        case L'a': return VC_A; case L'b': return VC_B; case L'c': return VC_C;
        case L'd': return VC_D; case L'e': return VC_E; case L'f': return VC_F;
        case L'g': return VC_G; case L'h': return VC_H; case L'i': return VC_I;
        case L'j': return VC_J; case L'k': return VC_K; case L'l': return VC_L;
        case L'm': return VC_M; case L'n': return VC_N; case L'o': return VC_O;
        case L'p': return VC_P; case L'q': return VC_Q; case L'r': return VC_R;
        case L's': return VC_S; case L't': return VC_T; case L'u': return VC_U;
        case L'v': return VC_V; case L'w': return VC_W; case L'x': return VC_X;
        case L'y': return VC_Y; case L'z': return VC_Z; 
        
        // Numbers
        case L'1': return VC_1; case L'2': return VC_2; case L'3': return VC_3;
        case L'4': return VC_4; case L'5': return VC_5; case L'6': return VC_6;
        case L'7': return VC_7; case L'8': return VC_8; case L'9': return VC_9;
        case L'0': return VC_0;

        // Special chars
        case L' ': return VC_SPACE;
        case L'\n': return VC_ENTER;
        case L'.': return VC_PERIOD;
        case L'!': *needs_shift = true; return VC_1;
        case L'"': *needs_shift = true; return VC_2;
        case L'·': *needs_shift = true; return VC_3;
        case L'$': *needs_shift = true; return VC_4;
        case L'%': *needs_shift = true; return VC_5;
        case L'&': *needs_shift = true; return VC_6;
        case L'/': *needs_shift = true; return VC_7;
        case L'(': *needs_shift = true; return VC_8;
        case L')': *needs_shift = true; return VC_9;
        case L'=': *needs_shift = true; return VC_0;
        
        default: return 0; // Return 0 for complex  characters
    }
}

void write_word(char word[], int substituted_word_length) {

    // Transform UTF-8 to wchar
    wchar_t w_word[SUBSTITUTE_WORD_CHAR];
    mbstowcs(w_word, word, SUBSTITUTE_WORD_CHAR);

    // 1. CALCULAR EXACTAMENTE CUÁNTOS EVENTOS "PRESSED" VAMOS A GENERAR
    int presses = substituted_word_length; // Los backspaces
    for (int i = 0; w_word[i] != L'\0'; i++) {
        bool needs_shift = false;
        bool needs_alt = false;
        uint16_t keycode = get_keycode_from_wchar(w_word[i], &needs_shift, &needs_alt);

        if (keycode != 0) {
            if (needs_shift) presses++;
            if (needs_alt) presses++;
            presses++;
        } else {
            // Special keys depending on the OS
            switch(OS){
                case 0: { // Linux
                    char hex_str[10];
                    sprintf(hex_str, "%x", w_word[i]);
                    // Ctrl+Shift (2) + U (1) + hex length + Space (1)
                    presses += 4 + strlen(hex_str); 
                    break;
                }
                case 1: { // Windows
                    presses += 5; // Alt (1) + 4 digits (4)
                    break;
                }
                case 2: // MAC
                    break;
            }
        }
    }
    
    // Ignote exactly this number of events
    writing = presses;

    uiohook_event press_event = {0};
    press_event.type = EVENT_KEY_PRESSED;
    press_event.data.keyboard.keycode = VC_BACKSPACE;

    uiohook_event release_event = {0};
    release_event.type = EVENT_KEY_RELEASED;
    release_event.data.keyboard.keycode = VC_BACKSPACE;

    for (int i = 0; i < substituted_word_length; i++) {
        hook_post_event(&press_event);
        usleep(5000);
        hook_post_event(&release_event);
        usleep(5000);
    }

    for (int i = 0; w_word[i] != L'\0'; i++) {
        bool needs_shift = false;
        bool needs_alt = false;
        
        uint16_t keycode = get_keycode_from_wchar(w_word[i], &needs_shift, &needs_alt);

        if (keycode != 0) {

            // Presses shift if needed
            if (needs_shift) {
                uiohook_event shift_event = {0};
                shift_event.type = EVENT_KEY_PRESSED;
                shift_event.data.keyboard.keycode = VC_SHIFT_L;
                hook_post_event(&shift_event);
                usleep(5000);
            }
            // Presses alt if needed
            if (needs_alt) {
                uiohook_event alt_event = {0};
                alt_event.type = EVENT_KEY_PRESSED;
                alt_event.data.keyboard.keycode = VC_ALT_R;
                hook_post_event(&alt_event);
                usleep(5000);
            }

            type_key(keycode);

            // Releases shift if needed
            if (needs_shift) {
                uiohook_event shift_event = {0};
                shift_event.type = EVENT_KEY_RELEASED;
                shift_event.data.keyboard.keycode = VC_SHIFT_L;
                hook_post_event(&shift_event);
                usleep(5000);
            }
            // Releases alt if needed
            if (needs_alt) {
                uiohook_event alt_event = {0};
                alt_event.type = EVENT_KEY_RELEASED;
                alt_event.data.keyboard.keycode = VC_ALT_R;
                hook_post_event(&alt_event);
                usleep(5000);
            }
        } else {
            switch(OS){ // Type special chars depending on the OS
                case 0: type_unicode_linux(w_word[i]); break;
                case 1: type_unicode_windows(w_word[i]); break;
                case 2: type_unicode_mac(w_word[i]); break; 
            }
        }
    }
}

void dispatch_event(uiohook_event * const event) {

    if (event->type != EVENT_KEY_PRESSED) {
        return; 
    }

    // Ignore simulating events
    if (writing > 0){ 
        writing--;
        return; 
    }

    char ch = get_char_from_event(event);
    if (ch != 0) {
        currently_typing[ptr] = ch;
        ptr++;
        currently_typing[ptr] = '\0';
    }

    // Memory safety
    if (ptr >= SUBSTITUTE_WORD_CHAR - 1) {
        for (int i = 0; i < SUBSTITUTE_WORD_CHAR - 1; i++) {
            currently_typing[i] = currently_typing[i + 1];
        }
        ptr--;
        currently_typing[ptr] = '\0'; 
    }

    // Detects if what you write is one of the selected words
    for (int i = 0; i < ptr; i++) {
        char *sufix = &currently_typing[i]; 
        char *substitution = look_for_word(sufix);

        if (strcmp(substitution, "None") != 0) {
            wchar_t w_sufix[SUBSTITUTE_WORD_CHAR];
            mbstowcs(w_sufix, sufix, SUBSTITUTE_WORD_CHAR);
            
            // Clean buffer before writing for evading problems on windows
            ptr = 0;
            currently_typing[0] = '\0';
            
            write_word(substitution, wcslen(w_sufix));
            break;
        }
    }
}

int main(void) {

    // Use systems language
    setlocale(LC_ALL, "");

    // Reads the config
    FILE *fptr;
    fptr = fopen("config.json", "rb");
    if (!fptr){
        printf("Error loading JSON\n");
    }
    char json_string[1024];
    printf("Configuration read...\n");
    fread(json_string, 1, sizeof(json_string) - 1, fptr);
    fclose(fptr);

    // Loads the config
    load_words_configuration(words, json_string);

    // Hooks for keyboard events
    hook_set_dispatch_proc(dispatch_event);
    if (hook_run() != UIOHOOK_SUCCESS) {
        fprintf(stderr, "hook_run failed\n");
        return 1;
    }
    hook_stop();
    return 0;
}
