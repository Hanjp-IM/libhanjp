#include "hanjpkeyboard.h"
#include <hangul.h>

G_DEFINE_INTERFACE(HanjpKeyboard, hanjp_keyboard, G_TYPE_OBJECT)

static void
hanjp_keyboard_default_init(HanjpKeyboardInterface *iface) {
    /* add properties and signals to the interface here */
}

gunichar hanjp_keyboard_get_mapping(HanjpKeyboard* self, gint tableid, gint ascii)
{
    HanjpKeyboardInterface *iface;

    g_return_if_fail(HANJP_IS_KEYBOARD(self));

    iface = HANJP_KEYBOARD_GET_IFACE(self);
    g_return_if_fail(iface->get_mapping != NULL);
    iface->get_mapping(self, tableid, ascii);
}

typedef struct {
    HangulKeyboard *keyboard;
} HanjpKeyboardDefaultPrivate;

static void hanjp_keyboarddefault_keyboard_interface_init(HanjpKeyboardInterface *iface);
G_DEFINE_TYPE_WITH_CODE(HanjpKeyboardDefault, hanjp_keyboarddefault, G_TYPE_OBJECT,
        G_ADD_PRIVATE(HanjpKeyboardDefault)
        G_IMPLEMENT_INTERFACE(HANJP_TYPE_KEYBOARD,
            hanjp_keyboarddefault_keyboard_interface_init))

static gunichar
hanjp_keyboarddefault_get_mapping(HanjpKeyboard *self, gint tableid, gint ascii)
{
    HanjpKeyboardDefaultPrivate *priv;
    priv = hanjp_keyboarddefault_get_instance_private(HANJP_KEYBOARDDEFAULT(self));

    // to implement
}

static void 
hanjp_keyboarddefault_init(HanjpKeyboardDefault *self)
{
    HanjpKeyboardDefaultPrivate *priv;
    priv = hanjp_keyboarddefault_get_instance_private(self);

    priv->keyboard = hangul_keyboard_new_from_file("keyboard.xml");
}

static void
hanjp_keyboarddefault_dispose(GObject *gobject)
{
    G_OBJECT_CLASS(hanjp_keyboarddefault_parent_class)->dispose(gobject);
}

static void
hanjp_keyboarddefault_finalize(GObject *gobject)
{
    HanjpKeyboardDefaultPrivate *priv;
    priv = hanjp_keyboarddefault_get_instance_private(HANJP_KEYBOARDDEFAULT(gobject));
    hangul_keyboard_delete(priv->keyboard);
    G_OBJECT_CLASS(hanjp_keyboarddefault_parent_class)->finalize(gobject);
    hangul_fini();
}

static void
hanjp_keyboarddefault_class_init(HanjpKeyboardDefaultClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    hangul_init();

    object_class->dispose = hanjp_keyboarddefault_dispose;
    object_class->finalize = hanjp_keyboarddefault_finalize;
}

static void
hanjp_keyboarddefault_keyboard_interface_init(HanjpKeyboardInterface *iface)
{
    iface->get_mapping = hanjp_keyboarddefault_get_mapping;
}
