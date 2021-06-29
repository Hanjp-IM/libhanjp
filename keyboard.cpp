#include "keyboard.h"
#include <hangul.h>

using namespace Hanjp;

extern "C" {
    ucschar hangul_keyboard_get_mapping(const HangulKeyboard* keyboard, int tableid, unsigned key);
} 


Keyboard::Keyboard() {
    keyboard = hangul_keyboard_new_from_file("keyboard.xml");
}

Keyboard::~Keyboard() {
    hangul_keyboard_delete(keyboard);
}

void Keyboard::reset() {}

char32_t Keyboard::get_mapping(int tableid, int ascii) {
    return hangul_keyboard_get_mapping(keyboard, tableid, ascii);
}

