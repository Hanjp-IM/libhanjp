#ifndef HANJP_AUTOMATA
#define HANJP_AUTOMATA

#include <string>
#include <map>

// popped string have to be a hiragana string(not katakana)

namespace Hanjp
{
    enum AMSIG {
        EAT,
        POP,
        FLUSH,
        FAIL,
    };

    class Automata {
    protected:
        std::map<std::pair<char32_t, char32_t>, char32_t> combine_map;
        char32_t combine(char32_t first, char32_t second);
        struct HangulBuffer {
            char32_t cho;
            char32_t jung;
            char32_t jung2;
            char32_t jong;
            char32_t pop();
            char32_t flush();
        } buffer;
    public:
        Automata() {
            buffer.cho = 0;
            buffer.jung = 0;
            buffer.jong = 0;
        }
        virtual ~Automata() {}
        virtual void to_kana(std::u32string& dest) = 0;
        virtual AMSIG push(char32_t ch, std::u32string& result, std::u32string& hangul) = 0; //result and hangul must be empty strings
        virtual bool backspace() {
            if(buffer.jong) {
                buffer.jong = 0;
                return true;
            }
            else if(buffer.jung2) {
                buffer.jung2 = 0;
                return true;
            }
            else if(buffer.jung){
                buffer.jung = 0;
                return true;
            }
            else if(buffer.cho){
                buffer.cho = 0;
                return true;
            }
            else {
                return false;
            }
        }
    };
    
    class AutomataDefault : public Automata {
    private:
    public:
        AutomataDefault();
        ~AutomataDefault();
        void to_kana(std::u32string& dest);
        AMSIG push(char32_t ch, std::u32string& result, std::u32string& hangul);
    };

    /*
        To use custom automata, define AutomataCustom class in this file and fill all member functions to automata.cpp
        Then, built target will contain your own automata.
        The below is the template

        class AutomataCustom : public Automata {
        public:
            AMSIG push(char32_t ch, std::u32string& result, std::u32string& hangul);
        };
    */
}

#endif