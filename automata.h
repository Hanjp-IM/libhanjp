#ifndef HANJP_AUTOMATA
#define HANJP_AUTOMATA

#include <string>

namespace Hanjp
{
    enum AMSIG {
        EAT,
        POP
    };

    struct HangulBuffer {
        char16_t cho;
        char16_t jung;
        char16_t jong;
    };

    class Automata {
    private:
        HangulBuffer buffer;
    public:
        Automata() {
            buffer.cho = 0;
            buffer.jung = 0;
            buffer.jong = 0;
        }
        virtual AMSIG push(char16_t ch, u16string& result) = 0;
        virtual std::u16string flush() {
            buffer.cho = 0;
            buffer.jung = 0;
            buffer.jong = 0;
        }
        virtual bool backspace() = 0;
    };
    
    class AutomataDefault : public Automata {
    private:
        char16_t cur;
    public:
        AutomataDefault() : cur(0) {}
        AMSIG push(char16_t ch, u16string& result);
        bool backspace();
    };

    /*
        To use custom automata, write AutomataCustom class in this file and fill all member functions to automata.cpp
        then, target that is built will contain your automata.
        The below is the template

        class AutomataCustom : public Automata {

        };
    */
}

#endif