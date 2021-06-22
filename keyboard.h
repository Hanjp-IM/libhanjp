#ifndef HANJP_KEYBOARD
#define HANJP_KEYBOARD

#include <hangul.h>

namespace Hanjp {
    class Keyboard {
    private:
        HangulKeyboard* keyboard;
    public:
        Keyboard();
        ~Keyboard();
        void reset();
        char32_t get_mapping(int tableid, int ascii);
    };
}

#endif