#ifndef __HANJP_KEYBOARD_H__
#define __HANJP_KEYBOARD_H__

G_BEGIN_DECLS

#define HANJP_TYPE_KEYBOARD hanjp_keyboard_get_type()
G_DECLARE_INTERFACE(HanjpKeyboard, hanjp_keyboard, HANJP, KEYBOARD, GObject)

struct _HanjpKeyboardInterface {
    GTypeInterface parent_iface;

    
};

G_END_DECLS

#endif // __HANJP_KEYBOARD_H__