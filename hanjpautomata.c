#include "hanjpautomata.h"
#include "hangulunicode.h"

G_DEFINE_INTERFACE(HanjpAutomata, hanjp_automata, G_TYPE_OBJECT)

static void hanjp_automata_default_init(HanjpAutomataInterface *iface) {
    /* add properties and signals to the interface here */
}

gboolean hanjp_automata_to_kana(HanjpAutomata *am, GArray *dest, gunichar cho, gunichar jung, gunichar jung2, gunichar jong) {

}

HanjpAmSig hanjp_automata_push(HanjpAutomata *am) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AUTOMATA(am));
    
    iface = HANJP_AUTOMATA_GET_IFACE(am);
    g_return_if_fail(iface->push != NULL);
    iface->push(am);
}

gboolean hanjp_automata_backspace(HanjpAutomata *am) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AUTOMATA(am));

    iface = HANJP_AUTOMATA_GET_IFACE(am);
    g_return_if_fail(iface->backspace != NULL);
    iface->backspace(am);
}

gunichar hanjp_automata_flush(HanjpAutomata *am) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AUTOMATA(am));

    iface = HANJP_AUTOMATA_GET_IFACE(am);
    g_return_if_fail(iface->flush != NULL);
    iface->flush(am);
}

static void hanjp_automatadefault_automata_interface_init(HanjpAutomataInterface *iface);
G_DEFINE_TYPE_WITH_CODE(HanjpAutomataDefault, hanjp_automatadefault, G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(HANJP_TYPE_AUTOMATA,
        hanjp_automatadefault_automata_interface_init))

static HanjpAmSig hanjp_automatadefault_push(HanjpAutomataDefault *am) {

}

static gboolean hanjp_automatadefault_backspace(HanjpAutomataDefault *am) {

}

static gunichar hanjp_automatadefault_flush(HanjpAutomataDefault *am) {

}

static void hanjp_automatadefault_automata_interface_init(HanjpAutomataInterface *iface) {
    iface->push = hanjp_automatadefault_push;
    iface->backspace = hanjp_automatadefault_backspace;
    iface->flush = hanjp_automatadefault_flush;
}