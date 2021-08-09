#ifndef __HANJP_AUTOMATA_H__
#define __HANJP_AUTOMATA_H__

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

typedef enum {
    EAT,
    POP,
    FAIL
} HanjpAmSig;

#define HANJP_TYPE_AUTOMATA hanjp_automata_get_type()
G_DECLARE_INTERFACE(HanjpAutomata, hanjp_automata, HANJP, AUTOMATA, GObject)

struct _HanjpAutomataInterface
{
    GTypeInterface parant_iface;

    HanjpAmSig (*push) (HanjpAutomata *self);
    gboolean (*backspace) (HanjpAutomata *self);
    gunichar (*flush) (HanjpAutomata *self);
};

gboolean hanjp_automata_to_kana(HanjpAutomata *am, GArray *dest, gunichar cho, gunichar jung, gunichar jung2, gunichar jong);
HanjpAmSig hanjp_automata_push(HanjpAutomata *am);
gboolean hanjp_automata_backspace(HanjpAutomata *am);
gunichar hanjp_automata_flush(HanjpAutomata *am);

#define HANJP_TYPE_AUTOMATABASE hanjp_automatabase_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpAutomataBase, hanjp_automatabase, HANJP, AUTOMATABASE, GObject)

struct _HanjpAutomataBaseClass
{
    GObjectClass parent_class;
};

#define HANJP_TYPE_AUTOMATADEFAULT hanjp_automatadefault_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpAutomataDefault, hanjp_automatadefault, HANJP, AUTOMATADEFAULT, HanjpAutomataBase)

struct _HanjpAutomataDefaultClass
{
    HanjpAutomataBaseClass parent_class;
};

HanjpAutomataDefault *hanjp_automatadeafult_new();

G_END_DECLS

#endif //__HANJP_AUTOMATA_H__