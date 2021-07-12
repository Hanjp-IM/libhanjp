#ifndef HANJP_AUTOMATA
#define HANJP_AUTOMATA

#include <string>
#include <map>
#include "hanjp.h"

// Popped string have to be a Hiragana string(not Katakana)

namespace Hanjp
{
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
            void flush();
            char32_t flush(const std::map<std::pair<char32_t, char32_t>, char32_t>& combine_map);
        } buffer;
    public:
        Automata() {
            buffer.cho = 0;
            buffer.jung = 0;
            buffer.jong = 0;
        }
        void to_kana(std::u32string& dest, char32_t cho, char32_t jung, char32_t jung2, char32_t jong);
        virtual ~Automata() {}
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
        char32_t flush() {
            return buffer.flush(combine_map);
        }
    };
    
    class AutomataDefault : public Automata {
    private:
        void to_kana(std::u32string& dest);
    public:
        AutomataDefault();
        ~AutomataDefault();
        AMSIG push(char32_t ch, std::u32string& result, std::u32string& hangul);
    };

    /*
        To use custom automata, define AutomataCustom class in this file and fill all member functions to automata.cpp
        The below is the template

        class AutomataCustom : public Automata {
        public:
            AMSIG push(char32_t ch, std::u32string& result, std::u32string& hangul);
        };
    */
}

#endif