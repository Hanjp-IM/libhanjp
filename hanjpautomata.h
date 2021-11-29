#ifndef __HANJP_AUTOMATA_H__
#define __HANJP_AUTOMATA_H__

#include <glib-object.h>
#include <gmodule.h>
#include "hanjpbuffer.h"

G_BEGIN_DECLS

#define HANJP_TYPE_AM hanjp_am_get_type()
G_DECLARE_INTERFACE(HanjpAutomata, hanjp_am, HANJP, AM, GObject)

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

#define HANJP_TYPE_AM_BASE hanjp_am_base_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpAutomataBase, hanjp_am_base, HANJP, AM_BASE, GObject)

struct _HanjpAutomataBaseClass
{
    GObjectClass parent_class;

    gboolean (*to_kana) (HanjpAutomata* self, GArray *dest, HanjpBuffer *buffer);
    gint (*push) (HanjpAutomata *self, GArray *result, GArray *hangul, gunichar ch);
    gboolean (*backspace) (HanjpAutomata *self);
    void (*flush) (HanjpAutomata *self);
};

#define HANJP_TYPE_AM_BUILTIN hanjp_am_builtin_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpAutomataBuiltin, hanjp_am_builtin, HANJP, AM_BUILTIN, HanjpAutomataBase)

struct _HanjpAutomataBuiltinClass
{
    HanjpAutomataBaseClass parent_class;
};

HanjpAutomataBuiltin *hanjp_am_builtin_new();

G_END_DECLS

#endif //__HANJP_AUTOMATA_H__
