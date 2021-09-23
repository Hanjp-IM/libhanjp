#ifndef __HANJP_AUTOMATA_H__
#define __HANJP_AUTOMATA_H__

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

typedef union HanjpBuffer HanjpBuffer;

#define HANJP_TYPE_AUTOMATA hanjp_am_get_type()
G_DECLARE_INTERFACE(HanjpAutomata, hanjp_am, HANJP, AUTOMATA, GObject)

struct _HanjpAutomataInterface
{
    GTypeInterface parant_iface;

    gboolean (*to_kana) (HanjpAutomata* self, GArray *dest, HanjpBuffer *buffer);
    gint (*push) (HanjpAutomata *self, GArray *result, GArray *hangul, gunichar ch);
    gboolean (*backspace) (HanjpAutomata *self);
    void (*flush) (HanjpAutomata *self);
};

gint hanjp_am_to_kana(HanjpAutomata *am, GArray *dest, HanjpBuffer *buffer);
gint hanjp_am_push(HanjpAutomata *am, GArray *preedit, GArray *hangul, gunichar ch);
gboolean hanjp_am_backspace(HanjpAutomata *am);
void hanjp_am_flush(HanjpAutomata *am);

#define HANJP_TYPE_AUTOMATABASE hanjp_ambase_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpAutomataBase, hanjp_ambase, HANJP, AUTOMATABASE, GObject)

struct _HanjpAutomataBaseClass
{
    GObjectClass parent_class;

    gboolean (*to_kana) (HanjpAutomata* self, GArray *dest, HanjpBuffer *buffer);
    gint (*push) (HanjpAutomata *self, GArray *result, GArray *hangul, gunichar ch);
    gboolean (*backspace) (HanjpAutomata *self);
    void (*flush) (HanjpAutomata *self);
};

#define HANJP_TYPE_AUTOMATADEFAULT hanjp_amdefault_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpAutomataDefault, hanjp_amdefault, HANJP, AUTOMATADEFAULT, HanjpAutomataBase)

struct _HanjpAutomataDefaultClass
{
    HanjpAutomataBaseClass parent_class;
};

HanjpAutomataDefault *hanjp_amdefault_new();

G_END_DECLS

#endif //__HANJP_AUTOMATA_H__