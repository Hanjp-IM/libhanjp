#include "hanjp.h"
#include <hangul.h>
#include "keyboard.h"
#include "automata.h"

using namespace Hanjp;
using namespace std;
        

static inline bool is_hiragana(char32_t ch) {
    return ch >= 0x3040 && ch <= 0x309F;
}

static inline bool is_katakana(char32_t ch) {
    return ch >= 0x30A0 && ch <= 0x30FF;
}

#define KANA_GAP 0x60

static inline void convert(u32string& str, OutputType type) {
    for(auto& ch : str) {
        switch(type) {
            case HIRAGANA:
            if(is_katakana(ch)) {
                ch -= KANA_GAP;
            }
            break;
            case KATAKANA:
            if(is_hiragana(ch)) {
                ch += KANA_GAP;
            }
            break;
            case HALF_KATAKANA:
            default:
            break;
        }
    }
}

int Hanjp::init() {
    return hangul_init();
}

int Hanjp::fini() {
    return hangul_fini();
}

InputContext::InputContext() : output_type(HIRAGANA) {
    am = new AutomataDefault;
    keyboard = new Keyboard;
}

InputContext::InputContext(const InputContext& copy) : output_type(HIRAGANA) {
    // It doesn't allow to use
    InputContext();
}

InputContext::~InputContext() {
    delete am;
    delete keyboard;
}

void InputContext::flush_internal() {
    committed = preedit;
    preedit.clear();
}

void InputContext::reset() {
    am->flush(); 
    hangul.clear();
    preedit.clear();
    committed.clear();
    output_type = HIRAGANA;
}

u32string InputContext::flush() {
    am->flush();
    flush_internal();
    return committed;
}

AMSIG InputContext::process(int ascii) {
    char32_t ch;
    AMSIG signal;
    u32string popped;

    committed.clear();

    ch = keyboard->get_mapping(0, ascii);
    signal = am->push(ch, popped, hangul); //push to automata and signal and results
    convert(popped, output_type); //Convert popped string to output type
    preedit += popped;

    switch(signal) {
        case FLUSH:
        flush_internal();
        break;
        case EAT:
        case POP:
        default:
        break;
    }

    return signal;
}

bool InputContext::backspace() {
    if(!am->backspace()) {
        if(preedit.empty()) {
            return false;
        }

        preedit.pop_back();
    }

    return true;
}

void InputContext::set_output_type(OutputType type) {
    output_type = type;
}

const u32string& InputContext::get_preedit_string() const {
    return preedit;
}

const u32string& InputContext::get_commit_string() const {
    return committed;
}

OutputType InputContext::get_output_type() const {
    return output_type;
}