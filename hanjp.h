#ifndef HANJP
#define HANJP

#include <string>

namespace Hanjp
{
    int init();
    int fini();

    enum OutputType {
        HIRAGANA,
        KATAKANA,
        HALF_KATAKANA,
    };

    enum AMSIG {
        EAT,
        POP,
        FLUSH,
        FAIL,
    };

    class Automata;
    class Keyboard;

    class InputContext {
    private:
        Automata* am;
        Keyboard* keyboard;
        std::u32string hangul;
        std::u32string preedit;
        std::u32string committed;
        void flush_internal();
        OutputType output_type;

    public:
        InputContext();
        ~InputContext();
        void reset();
        std::u32string flush();
        AMSIG process(int ascii);
        void toggle_preedit();
        void to_hiragana_preedit();
        void to_katakana_preedit();
        bool replace(int start, int end, const std::u32string str);
        bool backspace();
        void set_output_type(OutputType type);
        const std::u32string& get_preedit_string() const;
        const std::u32string& get_commit_string() const;
        const std::u32string& get_hangul_pronunciation() const;
        OutputType get_output_type() const;
    };
}

#endif