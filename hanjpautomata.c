#include "hanjpautomata.h"
#include "hanjpunicode.h"
#include <gmodule.h>

G_DEFINE_INTERFACE(HanjpAutomata, hanjp_am, G_TYPE_OBJECT)

static void
hanjp_am_default_init(HanjpAutomataInterface *iface) {
    /* add properties and signals to the interface here */
}

gboolean hanjp_am_to_kana(HanjpAutomata *am, GArray *dest, gunichar cho, gunichar jung, gunichar jung2, gunichar jong) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AUTOMATA(am));
    
    iface = HANJP_AUTOMATA_GET_IFACE(am);
    g_return_if_fail(iface->to_kana != NULL);
    iface->to_kana(am, dest, cho, jung, jung2, jong);
}

gint hanjp_am_push(HanjpAutomata *am, GArray *result, GArray *hangul, gunichar ch) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AUTOMATA(am));
    
    iface = HANJP_AUTOMATA_GET_IFACE(am);
    g_return_if_fail(iface->push != NULL);
    iface->push(am, result, hangul, ch);
}

gboolean hanjp_am_backspace(HanjpAutomata *am) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AUTOMATA(am));

    iface = HANJP_AUTOMATA_GET_IFACE(am);
    g_return_if_fail(iface->backspace != NULL);
    iface->backspace(am);
}

gunichar hanjp_am_flush(HanjpAutomata *am) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AUTOMATA(am));

    iface = HANJP_AUTOMATA_GET_IFACE(am);
    g_return_if_fail(iface->flush != NULL);
    iface->flush(am);
}

//fifty notes
static const gunichar kana_table[][5] = {
    // A, I, U, E, O
    {0x3042, 0x3044, 0x3046, 0x3048, 0x304A}, // A
    {0x304B, 0x304D, 0x304F, 0x3051, 0x3053}, // KA
    {0x3055, 0x3057, 0x3059, 0x305B, 0x305D}, // SA
    {0x305F, 0x3061, 0x3064, 0x3066, 0x3068}, // TA
    {0x306A, 0x306B, 0x306C, 0x306D, 0x306E}, // NA
    {0x306F, 0x3072, 0x3075, 0x3078, 0x307B}, // HA
    {0x307E, 0x307F, 0x3080, 0x3081, 0x3082}, // MO
    {0x3084, 0x0000, 0x3086, 0x0000, 0x3088}, // YA
    {0x3089, 0x308A, 0x308B, 0x308C, 0x308D}, // RA
    {0x308F, 0x3090, 0x0000, 0x3091, 0x3092}  // WA
};

typedef struct {
    struct HangulBuffer {
        gunichar cho;
        gunichar jung;
        gunichar jung2;
        gunichar jong;
    } buffer;
    GHashTable *combine_table;
} HanjpAutomataBasePrivate;

static void hanjp_ambase_am_interface_init(HanjpAutomataInterface *iface);
G_DEFINE_ABSTRACT_TYPE_WITH_CODE(HanjpAutomataBase, hanjp_ambase, G_TYPE_OBJECT,
        G_ADD_PRIVATE(HanjpAutomataBase)
        G_IMPLEMENT_INTERFACE(HANJP_TYPE_AUTOMATA,
            hanjp_ambase_am_interface_init))

static gboolean
hanjp_ambase_to_kana(HanjpAutomata *am, GArray *dest, gunichar cho, gunichar jung, gunichar jung2, gunichar jong)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_ambase_get_instance_private(HANJP_AUTOMATABASE(am));

    //to implement

    return TRUE;
}

static gint
hanjp_ambase_push(HanjpAutomata *am, GArray *result, GArray *hangul, gunichar ch)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_ambase_get_instance_private(HANJP_AUTOMATABASE(am));

    // to implement

    return TRUE;
}

static gboolean
hanjp_ambase_backspace(HanjpAutomata *am)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_ambase_get_instance_private(HANJP_AUTOMATABASE(am));

    // to implement

    return TRUE;
}

static gunichar
hanjp_ambase_flush(HanjpAutomata *am)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_ambase_get_instance_private(HANJP_AUTOMATABASE(am));

    // to implement

    return TRUE;
}

#define N_COMBINE_TABLE_ELEMENTS 1

static void
hanjp_ambase_init(HanjpAutomataBase *am)
{
    int i;
    static guint64 combine_table_keys[N_COMBINE_TABLE_ELEMENTS] = {(guint64)HANGUL_JUNGSEONG_O << 32 | HANGUL_JUNGSEONG_A,};
    static guint32 combine_table_vals[N_COMBINE_TABLE_ELEMENTS] = {HANGUL_JUNGSEONG_WA,};
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_ambase_get_instance_private(am);

    priv->buffer.cho = 0;
    priv->buffer.jung = 0;
    priv->buffer.jung2 = 0;
    priv->buffer.jong = 0;

    priv->combine_table = g_hash_table_new(g_int64_hash, g_int64_equal);

    for(i = 0; i < N_COMBINE_TABLE_ELEMENTS; i++) {
        g_hash_table_insert(priv->combine_table, &combine_table_keys[i], &combine_table_vals[i]);
    }
}

static void
hanjp_ambase_dispose(GObject *gobject)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_ambase_get_instance_private(HANJP_AUTOMATABASE(gobject));

    g_clear_object(&priv->combine_table);

    G_OBJECT_CLASS(hanjp_ambase_parent_class)->dispose(gobject);
}

static void
hanjp_ambase_finalize(GObject *gobject)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_ambase_get_instance_private(HANJP_AUTOMATABASE(gobject));

    G_OBJECT_CLASS(hanjp_ambase_parent_class)->finalize(gobject);
}

static void
hanjp_ambase_class_init(HanjpAutomataBaseClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->dispose = hanjp_ambase_dispose;
    object_class->finalize = hanjp_ambase_finalize;
}

static void
hanjp_ambase_am_interface_init(HanjpAutomataInterface *iface)
{
    iface->to_kana = hanjp_ambase_to_kana;
    iface->push = hanjp_ambase_push;
    iface->backspace = hanjp_ambase_backspace;
    iface->flush = hanjp_ambase_flush;
}

typedef struct {

} HanjpAutomataDefaultPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HanjpAutomataDefault, hanjp_amdefault, HANJP_TYPE_AUTOMATABASE)

static void
hanjp_amdefault_init(HanjpAutomataDefault *am)
{
    // to implement
}

static gint
hanjp_amdefault_push(HanjpAutomataDefault *am, GArray *result, GArray *hangul, gunichar ch)
{
    // to implement
}

static gboolean
hanjp_amdefault_backspace(HanjpAutomataDefault *am)
{
    // to implement
    return TRUE;
}

static gunichar
hanjp_amdefault_flush(HanjpAutomataDefault *am)
{
    // to implement
}

static void
hanjp_amdefault_class_init(HanjpAutomataDefaultClass *klass)
{
    // to implement
}

HanjpAutomataDefault *hanjp_amdefault_new()
{
    return g_object_new(HANJP_TYPE_AUTOMATADEFAULT, NULL);
}