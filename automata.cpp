#include "automata.h"

using namespace Hanjp;
using namespace std;

#if defined(USE_AM_CUSTOM)
// Fill here to write your own automata
#else

AMSIG AutomataDefault::push(char32_t ch, u32string& result, u32string& hangul) {
    return {};
}

bool AutomataDefault::backspace() {
    return {};
}

#endif