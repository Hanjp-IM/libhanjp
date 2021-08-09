#include "hanjpinputcontext.h"
#include "hanjpautomata.h"
#include "hanjpkeyboard.h"

#define MAX_AMS 3

struct _HanjpInputConext {
    GObject parent;

    HanjpAutomata *ams[MAX_AMS];
    int idx_am;
    int n_ams;
    HanjpKeyboard *keyboard;
    GArray *preedit;
    GArray *committed;
    GArray *hangul;
};