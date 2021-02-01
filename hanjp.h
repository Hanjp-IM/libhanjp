#ifndef HANJP
#define HANJP

#include <string>
#include "automata.h"
#include "hangul.h"

namespace Hanjp
{
    int init() {
        return hangul_init();
    }

    int fini() {
        return hangul_fini();
    }

    enum OutputType {
        HIRAGANA,
        KATAKANA,
    };

    class InputContext {
    private:
        Automata* am;
        HangulKeyboard* keyboard;
        std::u16string preedit;
        std::u16string committed;
        std::u16string flush_internal();
        OutputType output_type;

    public:
        InputContext() : output_type(HIRAGANA) {
            #if defined(USE_AM_CUSTOM)
                am = new AutomataCustom;
            #else
                am = new AutomataDefault;
            #endif
            
            keyboard = hangul_keyboard_new_from_file("keyboard.xml");
        }
        ~InputContext() {
            delete am;
            hangul_keyboard_delete(keyboard);
        }
        std::u16string flush();
        AMSIG process(int ascii);
        void toggle_preedit();
        void to_hiragana_preedit();
        void to_katakana_preedit();
        bool replace(int start, int end, const std::u16string str);
        bool backspace();
        void set_output_type(OutputType type);
        const std::u16string& get_preedit_string() const;
        const std::u16string& get_commit_string() const;
        OutputType get_output_type() const;
    };
}

#endif