#ifndef __HANJP_KEYBOARD_H__
#define __HANJP_KEYBOARD_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define HANJP_TYPE_KEYBOARD hanjp_keyboard_get_type()
G_DECLARE_INTERFACE(HanjpKeyboard, hanjp_keyboard, HANJP, KEYBOARD, GObject)

struct _HanjpKeyboardInterface {
    GTypeInterface parent_iface;

    gunichar (*get_mapping) (HanjpKeyboard *self, gint tableid, gint ascii);
};

gunichar hanjp_keyboard_get_mapping(HanjpKeyboard *self, gint tableid, gint ascii);

#define HANJP_TYPE_KEYBOARDDEFAULT hanjp_keyboarddefault_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpKeyboardDefault, hanjp_keyboarddefault, HANJP, KEYBOARDDEFAULT, GObject)

struct _HanjpKeyboardDefaultClass {
    GObjectClass parent_class;
};

HanjpKeyboardDefault *hanjp_keyboarddefault_new();

G_END_DECLS

#endif // __HANJP_KEYBOARD_H__