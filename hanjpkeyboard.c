#include "hanjpkeyboard.h"
#include "hangul.h"

extern ucschar
hangul_keyboard_get_mapping(const HangulKeyboard* keyboard, int tableid, unsigned key);

G_DEFINE_INTERFACE(HanjpKeyboard, hanjp_kb, G_TYPE_OBJECT)

static void
hanjp_kb_default_init(HanjpKeyboardInterface *iface) {
	// Nothing to do
}

gunichar hanjp_kb_get_mapping(HanjpKeyboard* self, gint tableid, gint ascii)
{
    HanjpKeyboardInterface *iface;

    g_return_if_fail(HANJP_IS_KB(self));

    iface = HANJP_KB_GET_IFACE(self);
    g_return_if_fail(iface->get_mapping != NULL);
    return iface->get_mapping(self, tableid, ascii);
}

typedef struct {
    HangulKeyboard *keyboard;
} HanjpKeyboardBuiltinPrivate;

static void hanjp_kb_builtin_interface_init(HanjpKeyboardInterface *iface);
G_DEFINE_TYPE_WITH_CODE(HanjpKeyboardBuiltin, hanjp_kb_builtin, G_TYPE_OBJECT,
        G_ADD_PRIVATE(HanjpKeyboardBuiltin)
        G_IMPLEMENT_INTERFACE(HANJP_TYPE_KB,
            hanjp_kb_builtin_interface_init))

HanjpKeyboardBuiltin *hanjp_kb_builtin_new()
{
    return g_object_new(HANJP_TYPE_KEYBOARDDEFAULT, NULL);
}

static gunichar
hanjp_kb_builtin_get_mapping(HanjpKeyboard *self, gint tableid, gint ascii)
{
    HanjpKeyboardBuiltinPrivate *priv;
    priv = hanjp_kb_builtin_get_instance_private(HANJP_KB_BUILTIN(self));
	g_return_if_fail(priv->keyboard != NULL);

	return hangul_keyboard_get_mapping(priv->keyboard, tableid, ascii);
}

static void 
hanjp_kb_builtin_init(HanjpKeyboardBuiltin *self)
{
    HanjpKeyboardBuiltinPrivate *priv;
    priv = hanjp_kb_builtin_get_instance_private(self);

    priv->keyboard = hangul_keyboard_list_get_keyboard("2");
}

static void
hanjp_kb_builtin_dispose(GObject *gobject)
{
    G_OBJECT_CLASS(hanjp_kb_builtin_parent_class)->dispose(gobject);
}

static void
hanjp_kb_builtin_finalize(GObject *gobject)
{
    HanjpKeyboardBuiltinPrivate *priv;
    priv = hanjp_kb_builtin_get_instance_private(HANJP_KB_BUILTIN(gobject));
    hangul_keyboard_delete(priv->keyboard);
    G_OBJECT_CLASS(hanjp_kb_builtin_parent_class)->finalize(gobject);
}

static void
hanjp_kb_builtin_class_init(HanjpKeyboardBuiltinClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->dispose = hanjp_kb_builtin_dispose;
    object_class->finalize = hanjp_kb_builtin_finalize;
}

static void
hanjp_kb_builtin_interface_init(HanjpKeyboardInterface *iface)
{
    iface->get_mapping = hanjp_kb_builtin_get_mapping;
}
