#include "pty_hell.h"
int keyboard=0;
char key[64][64] = {"", "[BOOKS]", "[MENU]", "[VOL-]", "[VOL+]", "[SEARCH]",
    "[ZOOM]", "[<<]", "[>||]", "[>>]", "[OPTION]", "q", "w", "e", "r", "t",
    "y", "u", "i", "o", "p", "a", "s", "d", "f", "g", "h", "j", "k", "l",
    "[BACKSPACE]", "z", "x", "c", "v", "b", "n", "m", "[BACK]", "[UP]",
    "\n", "[SHIFT]", "[SYM]", "[LAYOUT]", " ", "[LEFT]", "[DOWN]", "[RIGHT]", "[>]", "[<]", "[>]", "[<]", "[POWER]", "[POWER2]"};


char key_high[64][64] = {"", "[BOOKS]", "[MENU]", "[VOL-]", "[VOL+]", "[SEARCH]",
    "[ZOOM]", "[<<]", "[>||]", "[>>]", "[OPTION]", "Q", "W", "E", "R", "T",
    "Y", "U", "I", "O", "P", "A", "S", "D", "F", "G", "H", "J", "K", "L",
    "[BACKSPACE]", "Z", "X", "C", "V", "B", "N", "M", "[BACK]", "[UP]",
    "\n", "[SHIFT]", "[SYM]", "[LAYOUT]", " ", "[LEFT]", "[DOWN]", "[RIGHT]", "[>]", "[<]", "[>]", "[<]", "[POWER]", "[POWER2]"};

char key_sym[64][64] = {"", "[BOOKS]", "[MENU]", "[VOL-]", "[VOL+]", "[SEARCH]",
    "[ZOOM]", "[<<]", "[>||]", "[>>]", "|", "1", "2", "3", "4", "5",
    "6", "7", "8", "9", "0", "~", "!", "@", "^", "?", "(", ")", ";", ":",
    "/", "-", "_", "+", "=", "'", ",", ".", "*", "[UP]",
    "\n", "[SHIFT]", "[SYM]", "[LAYOUT]", " ", "[LEFT]", "[DOWN]", "[RIGHT]", "[>]", "[<]", "[>]", "[<]", "[POWER]", "[POWER2]"};

int open_keyboard(void)
{
    keyboard = open("/dev/input/event0", O_RDONLY);
    
    if(keyboard < 0) return 0;
    
    return 1;
}

void read_keyboard(void){
    int value,i,rd;
    struct input_event ev[64];
    static char low=1, sym=0;
    rd = read (keyboard, ev, (sizeof (struct input_event)) * 64);
    for(i=0; i<(rd/(sizeof (struct input_event))); i++){
        value = ev[i].value;
    
        if (/*value != ' ' && ev[1].value == 1 &&*/ ev[i].value == 1){ // Only read the key press event
            //update_part(bitmap, 800*600/2);
            switch(ev[i].code){
                case 0x35: system("poweroff"); break;
                case 0x29: low=0; break;
                case 42: sym=1; break;
                default: if(low&&(!sym))
                            {//put_str(key[ev[0].code]);
                            write_ptm(key[ev[i].code], 1);
                            //command[com_p] = key[ev[0].code][0];
                            //com_p++;
                        }
                            //fwrite(key[ev[0].code], sizeof(char), strlen(key[ev[0].code]), shell);
                        else if(sym) {
                            //put_str(key_sym[ev[0].code]);
                            write_ptm(key_sym[ev[i].code], 1);
                            //command[com_p] = key_sym[ev[0].code][0];
                            //com_p++;
                        } else
                        {//put_str(key_high[ev[0].code]);
							write_ptm(key_high[ev[i].code], 1);
                            //command[com_p] = key_high[ev[0].code][0];
                            //com_p++;
                        }
                            //fwrite(key[ev[0].code], sizeof(char), strlen(key_high[ev[0].code]), shell);
                //update_part();
                break;
            }
        }
        if(ev[i].value == 0){
            switch(ev[i].code){
            case 0x29: low=1; break;
            case 42: sym=0; break;
            }
        }
	}
    //    return 0;
}

int get_keyboard(void)
{
	return keyboard;
}
