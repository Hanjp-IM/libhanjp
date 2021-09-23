#include "hanjpkeyboard.h"
#include "hangul.h"

extern ucschar
hangul_keyboard_get_mapping(const HangulKeyboard* keyboard, int tableid, unsigned key);

G_DEFINE_INTERFACE(HanjpKeyboard, hanjp_keyboard, G_TYPE_OBJECT)

static void
hanjp_keyboard_default_init(HanjpKeyboardInterface *iface) {
	// Nothing to do
}

gunichar hanjp_keyboard_get_mapping(HanjpKeyboard* self, gint tableid, gint ascii)
{
    HanjpKeyboardInterface *iface;

    g_return_if_fail(HANJP_IS_KEYBOARD(self));

    iface = HANJP_KEYBOARD_GET_IFACE(self);
    g_return_if_fail(iface->get_mapping != NULL);
    return iface->get_mapping(self, tableid, ascii);
}

typedef struct {
    HangulKeyboard *keyboard;
} HanjpKeyboardDefaultPrivate;

static void hanjp_keyboarddefault_keyboard_interface_init(HanjpKeyboardInterface *iface);
G_DEFINE_TYPE_WITH_CODE(HanjpKeyboardDefault, hanjp_keyboarddefault, G_TYPE_OBJECT,
        G_ADD_PRIVATE(HanjpKeyboardDefault)
        G_IMPLEMENT_INTERFACE(HANJP_TYPE_KEYBOARD,
            hanjp_keyboarddefault_keyboard_interface_init))

HanjpKeyboardDefault *hanjp_keyboarddefault_new()
{
    return g_object_new(HANJP_TYPE_KEYBOARDDEFAULT, NULL);
}

static gunichar
hanjp_keyboarddefault_get_mapping(HanjpKeyboard *self, gint tableid, gint ascii)
{
    HanjpKeyboardDefaultPrivate *priv;
    priv = hanjp_keyboarddefault_get_instance_private(HANJP_KEYBOARDDEFAULT(self));
	g_return_if_fail(priv->keyboard != NULL);

	return hangul_keyboard_get_mapping(priv->keyboard, tableid, ascii);
}

static void 
hanjp_keyboarddefault_init(HanjpKeyboardDefault *self)
{
    HanjpKeyboardDefaultPrivate *priv;
    priv = hanjp_keyboarddefault_get_instance_private(self);

    priv->keyboard = hangul_keyboard_list_get_keyboard("2");
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
}

static void
hanjp_keyboarddefault_class_init(HanjpKeyboardDefaultClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->dispose = hanjp_keyboarddefault_dispose;
    object_class->finalize = hanjp_keyboarddefault_finalize;
}

static void
hanjp_keyboarddefault_keyboard_interface_init(HanjpKeyboardInterface *iface)
{
    iface->get_mapping = hanjp_keyboarddefault_get_mapping;
}
