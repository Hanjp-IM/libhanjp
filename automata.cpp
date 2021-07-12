#include "automata.h"
#include "hangulunicode.h"
#include <hangul.h>

using namespace Hanjp;
using namespace std;

extern "C" {
    ucschar hangul_choseong_to_jongseong(ucschar c);
    ucschar hangul_jongseong_to_choseong(ucschar c);
}

char32_t Automata::HangulBuffer::pop() {
    char32_t ret;

    if(jong) {
        ret = jong;
        jong = 0;
    }
    else if(jung2) {
        ret = jung2;
        jung2 = 0;
    }
    else if(jung) {
        ret = jung;
        jung = 0;
    }
    else if(cho) {
        ret = cho;
        cho = 0;
    }
    else {
        ret = 0;
    }

    return ret;
}

void Automata::HangulBuffer::flush(){
    cho = 0;
    jung = 0;
    jung2 = 0;
    jong = 0;
}

char32_t Automata::HangulBuffer::flush(const std::map<std::pair<char32_t, char32_t>, char32_t>& combine_map) {
    ucschar c;
    auto it = combine_map.find(make_pair(jung, jung2));

    if(it != combine_map.end()) {
        jung = it->second;
        jung2 = 0;
    }

    c = hangul_jamo_to_syllable(cho, jung, jong);
    if(c == 0) {
        if(cho) {
            c = hangul_jamo_to_cjamo(cho);
        }
        else if(jung) {
            c = hangul_jamo_to_cjamo(jung);
        }
        else{
            c = hangul_jamo_to_cjamo(jong);
        }
    }

    cho = 0;
    jung = 0;
    jung2 = 0;
    jong = 0;

    return (char32_t)c;
}

//fifty notes
static const char32_t kana_table[][5] = {
    // A, I, U, E, O
    {0x3042, 0x3044, 0x3046, 0x3048, 0x304A}, // A
    {0x304B, 0x304D, 0x304F, 0x3051, 0x3053}, // KA
    {0x3055, 0x3057, 0x3059, 0x305B, 0x305D}, // SA
    {0x305F, 0x3061, 0x3064, 0x3066, 0x3068}, // TA
    {0x306A, 0x306B, 0x306C, 0x306D, 0x306E}, // NA
    {0x306F, 0x3072, 0x3075, 0x3078, 0x307B}, // HA
    {0x307E, 0x307F, 0x3080, 0x3081, 0x3082}, // MO
    {0x3084, 0x0000, 0x3086, 0x0000, 0x3088}, // YA
    {0x3089, 0x308A, 0x308B, 0x308C, 0x308D}, // RA
    {0x308F, 0x3090, 0x0000, 0x3091, 0x3092}  // WA
};

static const char32_t kana_nn = 0x3093;

void Automata::to_kana(std::u32string& dest, char32_t cho, char32_t jung, char32_t jung2, char32_t jong) {
    int i, j;
    int adj;
    map<pair<char32_t, char32_t>,char32_t>::iterator it;
    bool jung_void;

    while(cho || jung || jong) {
        adj = 0;
        jung_void = false;

        // select row index
        switch(cho) {
            case 0:
            case HANGUL_CHOSEONG_FILLER:        // VOID
            adj = -1;
            case HANGUL_CHOSEONG_IEUNG:         // ㅇ
            i = 0; break;
            case HANGUL_CHOSEONG_KIYEOK:        // ㄱ
            adj = 1;
            case HANGUL_CHOSEONG_KHIEUKH:       // ㅋ
            case HANGUL_CHOSEONG_SSANGKIYEOK:   // ㄲ
            i = 1; break;   // K
            case HANGUL_CHOSEONG_CIEUC:         // ㅈ
            adj = 1;
            case HANGUL_CHOSEONG_SIOS:          // ㅅ
            case HANGUL_CHOSEONG_SSANGSIOS:     // ㅆ
            i = 2; break;   // S
            case HANGUL_CHOSEONG_TIKEUT:        // ㄷ
            adj = 1;
            case HANGUL_CHOSEONG_SSANGTIKEUT:   // ㄸ
            case HANGUL_CHOSEONG_CHIEUCH:       // ㅊ
            i = 3; break;   // T
            case HANGUL_CHOSEONG_NIEUN:         // ㄴ
            i = 4; break;   // N
            case HANGUL_CHOSEONG_PHIEUPH:       // ㅍ
            adj = 1;
            case HANGUL_CHOSEONG_SSANGPIEUP:    // ㅃ
            adj += 1;
            case HANGUL_CHOSEONG_HIEUH:         // ㅎ
            i = 5; break;   // H
            case HANGUL_CHOSEONG_MIEUM:         // ㅁ
            i = 6; break;   // M
            case HANGUL_CHOSEONG_RIEUL:         // ㄹ
            i = 8; break;   // R
            case HANGUL_CHOSEONG_SSANGNIEUN:
            cho = 0;
            dest.push_back(kana_nn);
            continue;
            default: return;
        }

        if(jung == 0) {
            jung = jung2;
        }
        else {
            it = combine_map.find(make_pair(jung, jung2));
            if(it != combine_map.end()) {
                jung = it->second;
            }
        }
        jung2 = 0;

        // divide jungseong
        switch(jung) {
            case 0:
            case HANGUL_JUNGSEONG_FILLER:
            jung_void = true; break;
            case HANGUL_JUNGSEONG_WA:
            if(i == 0){
                i = 9;
            }
            else {
                jung = HANGUL_JUNGSEONG_O;
                jung2 = HANGUL_JUNGSEONG_A;
            }
            break;
            case HANGUL_JUNGSEONG_YA:
            case HANGUL_JUNGSEONG_YU:
            case HANGUL_JUNGSEONG_YO:
            case HANGUL_JUNGSEONG_YEO:
            if(i == 0) {
                i = 7;
            }
            else{
                jung = HANGUL_JUNGSEONG_I;
                jung2 = jung;
            }
            break;
            case HANGUL_JUNGSEONG_YE:
            case HANGUL_JUNGSEONG_YAE:
            jung = HANGUL_JUNGSEONG_I;
            jung2 = HANGUL_JUNGSEONG_E;
            break;
            default:
            jung = 0;
        }
        //select column index
        switch(jung) {
            case HANGUL_JUNGSEONG_WA:
            case HANGUL_JUNGSEONG_YA:
            case HANGUL_JUNGSEONG_A:
            j = 0; break;
            case HANGUL_JUNGSEONG_I:
            j = 1; break;
            case HANGUL_JUNGSEONG_YU:
            case HANGUL_JUNGSEONG_EU:
            case HANGUL_JUNGSEONG_U:
            case HANGUL_JUNGSEONG_FILLER:
            case 0:
            j = 2; break;
            case HANGUL_JUNGSEONG_AE:
            case HANGUL_JUNGSEONG_E:
            j = 3; break;
            case HANGUL_JUNGSEONG_YO:
            case HANGUL_JUNGSEONG_YEO:
            case HANGUL_JUNGSEONG_EO:
            case HANGUL_JUNGSEONG_O:
            j = 4; break;
            default:
            return;
        }

        // append taken kana charactor
        if(jong == 0 && jung_void && !dest.empty()) {
            const char32_t c = dest.back();
            if(c != kana_nn && c != kana_table[3][2] - 1) {
                jong = hangul_choseong_to_jongseong(cho);
            }
        }
        else {
            dest.push_back(kana_table[i][j] + adj);
        }

        // eat choseong
        cho = 0;
        // eat jungseong
        jung = 0;
        // eat jongseong
        if(jung == 0) {
            switch(jong) {
                case HANGUL_JONGSEONG_KIYEOK:
                case HANGUL_JONGSEONG_SSANGKIYEOK:
                case HANGUL_JONGSEONG_SIOS:
                case HANGUL_JONGSEONG_SSANGSIOS:
                case HANGUL_JONGSEONG_KHIEUKH:
                dest.push_back(kana_table[3][2] - 1); break;
                case HANGUL_JONGSEONG_NIEUN:
                case HANGUL_JONGSEONG_MIEUM:
                case HANGUL_JONGSEONG_PIEUP:
                case HANGUL_JONGSEONG_PHIEUPH:
                case HANGUL_JONGSEONG_IEUNG:
                dest.push_back(kana_nn); break;
                default:
                cho = hangul_jongseong_to_choseong(jong);
            }
            jong = 0;
        }
    }
}

void AutomataDefault::to_kana(u32string& dest) {
    Automata::to_kana(dest, buffer.cho, buffer.jung, buffer.jung2, buffer.jong);
}

AMSIG AutomataDefault::push(char32_t ch, u32string& result, u32string& hangul) {
    AMSIG signal;

    if(!hangul_is_jamo(ch)) {
        to_kana(result);
        result += ch;
        hangul += buffer.flush(combine_map);
        hangul += ch;
        return FLUSH;
    }

    if(hangul_is_choseong(ch)) {
        if(buffer.cho) {
            to_kana(result);
            hangul += buffer.flush(combine_map);
            signal = POP;
        }
        else {
            signal = EAT;
        }
        buffer.cho = ch;
    }
    else if(hangul_is_jungseong(ch)) {
        if(buffer.jung) {
            buffer.jung2 = ch;
        }
        else {
            buffer.jung = ch;
        }

        if(buffer.jung2 == 0 && buffer.jung == HANGUL_JUNGSEONG_O) {
            signal = EAT;
        }
        else {
            to_kana(result);
            hangul += buffer.flush(combine_map);
            signal = POP;
        }
    }
    else if(hangul_is_jongseong(ch)) {
        signal = push(hangul_jongseong_to_choseong(ch), result, hangul);
    }
    else {
        buffer.flush();
        signal = FAIL;
    }

    return signal;
}

AutomataDefault::AutomataDefault() {
    combine_map.insert(make_pair(make_pair(HANGUL_JUNGSEONG_O, HANGUL_JUNGSEONG_A), HANGUL_JUNGSEONG_WA)); // ㅗ + ㅏ -> ㅘ
}

AutomataDefault::~AutomataDefault() {}