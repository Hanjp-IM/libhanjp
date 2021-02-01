#include "automata.h"

using namespace Hanjp;
using namespace std;

#if defined(USE_AM_CUSTOM)
// Fill here to write your own automata
#else

AMSIG AutomataDefault::push(char16_t ch, u16string& result, u16string& hangul) {
    return {};
}

bool AutomataDefault::backspace() {
    return {};
}

#endif