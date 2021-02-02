#ifndef HANJP_AUTOMATA
#define HANJP_AUTOMATA

#include <string>

// popped string have to be a hiragana string(not katakana)

namespace Hanjp
{
    enum AMSIG {
        EAT,
        POP
    };

    class Automata {
    private:
        struct HangulBuffer {
            char32_t cho;
            char32_t jung;
            char32_t jong;
        } buffer;
    public:
        Automata() {
            buffer.cho = 0;
            buffer.jung = 0;
            buffer.jong = 0;
        }

        virtual ~Automata() {}

        virtual AMSIG push(char32_t ch, std::u32string& result, std::u32string& hangul) = 0; //result and hangul must be empty strings
        virtual std::u32string flush() {
            buffer.cho = 0;
            buffer.jung = 0;
            buffer.jong = 0;
            return {};
        }
        virtual bool backspace() = 0;
    };
    
    class AutomataDefault : public Automata {
    private:
        char32_t cur;
    public:
        AutomataDefault() : cur(0) {}
        AMSIG push(char32_t ch, std::u32string& result, std::u32string& hangul);
        bool backspace();
    };

    /*
        To use custom automata, define AutomataCustom class in this file and fill all member functions to automata.cpp
        Then, built target will contain your own automata.
        The below is the template

        class AutomataCustom : public Automata {

        };
    */
}

#endif