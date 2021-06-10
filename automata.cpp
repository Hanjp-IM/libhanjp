#include "automata.h"
#include "hangulunicode.h"
#include <hangul.h>

using namespace Hanjp;
using namespace std;

static void convert_to_kana(u32string& result, const u32string& hangul);
extern ucschar hangul_jongseong_to_choseong(ucschar c);

#if defined(USE_AM_CUSTOM)
// Fill here to write your own automata
#else

AMSIG AutomataDefault::push(char32_t ch, u32string& result, u32string& hangul) {
    if(!hangul_is_jamo(ch)) {
        hangul += flush();
        hangul += ch;
        convert_to_kana(result, hangul);
        return FLUSH;
    }

    if(hangul_is_choseong(ch)) {
        if(buffer.cho) {
            hangul += flush();
            buffer.cho = ch;
            convert_to_kana(result, hangul);
            return POP;
        }
        else {
            buffer.cho = ch;
            return EAT;
        }
    }
    else if(hangul_is_jungseong(ch)) {
        buffer.jung = ch;
        hangul += flush();
        convert_to_kana(result, hangul);
        return POP;
    }
    else if(hangul_is_jongseong(ch)) {
        return push(hangul_jongseong_to_choseong(ch), result, hangul);
    }
    else {
        return FAIL;
    }
}

#endif