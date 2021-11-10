#ifndef __HANJP_KEYBOARD_H__
#define __HANJP_KEYBOARD_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define HANJP_TYPE_KB hanjp_kb_get_type()
G_DECLARE_INTERFACE(HanjpKeyboard, hanjp_kb, HANJP, KB, GObject)

struct _HanjpKeyboardInterface {
    GTypeInterface parent_iface;

    gunichar (*get_mapping) (HanjpKeyboard *self, gint tableid, gint ascii);
};

gunichar hanjp_kb_get_mapping(HanjpKeyboard *self, gint tableid, gint ascii);

#define HANJP_TYPE_KEYBOARDDEFAULT hanjp_kb_builtin_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpKeyboardBuiltin, hanjp_kb_builtin, HANJP, KB_BUILTIN, GObject)

struct _HanjpKeyboardBuiltinClass {
    GObjectClass parent_class;
};

HanjpKeyboardBuiltin *hanjp_kb_builtin_new();

G_END_DECLS

#endif // __HANJP_KEYBOARD_H__