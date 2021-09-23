#include "hanjpinputcontext.h"
#include "hanjpautomata.h"
#include "hanjpkeyboard.h"

static gboolean is_hiragana(gunichar ch) {
    return ch >= 0x3040 && ch <= 0x309F;
}

static gboolean is_katakana(gunichar ch) {
    return ch >= 0x30A0 && ch <= 0x30FF;
}

#define KANA_GAP 0x60

typedef struct {
    HanjpAutomata *ams[2];
    HanjpAutomata *cur_am;
    HanjpKeyboard *keyboard;
    GArray *preedit;
    gint kana_len;
    GArray *committed;
    GArray *hangul;
    gint output_mode;
} HanjpInputContextPrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HanjpInputContext, hanjp_ic, G_TYPE_OBJECT)

static void
hanjp_ic_dispose(GObject *self)
{
    HanjpInputContextPrivate *priv;
    priv = hanjp_ic_get_instance_private(HANJP_INPUTCONTEXT(self));

    g_clear_object(&priv->ams[0]);
    g_clear_object(&priv->ams[1]);
    g_clear_object(&priv->cur_am);
    g_clear_object(&priv->keyboard);
    g_clear_object(&priv->preedit);
    g_clear_object(&priv->committed);
    g_clear_object(&priv->hangul);

    //chain up
    G_OBJECT_CLASS(hanjp_ic_parent_class)->dispose(self);
}

static void
hanjp_ic_finalize(GObject *self)
{
    G_OBJECT_CLASS(hanjp_ic_parent_class)->finalize(self);
}

static void
hanjp_ic_class_init(HanjpInputContextClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);

    object_class->dispose = hanjp_ic_dispose;
    object_class->finalize = hanjp_ic_finalize;
}

static void
hanjp_ic_init(HanjpInputContext *self)
{
    gint i;
    HanjpInputContextPrivate *priv;
    priv = hanjp_ic_get_instance_private(self);

    priv->ams[0] = HANJP_AUTOMATA(hanjp_amdefault_new());
    priv->ams[1] = NULL;
    priv->cur_am = g_object_ref(priv->ams[0]);
    priv->keyboard = HANJP_KEYBOARD(hanjp_keyboarddefault_new());
    priv->preedit = g_array_sized_new(TRUE, TRUE, sizeof(gunichar), 64);
    priv->committed = g_array_sized_new(TRUE, TRUE, sizeof(gunichar), 64);
    priv->hangul = g_array_sized_new(TRUE, TRUE, sizeof(gunichar), 64);

    priv->output_mode = HANJP_OUTPUT_HIRAGANA;
}

HanjpInputContext* hanjp_ic_new()
{
    return g_object_new(HANJP_TYPE_INPUTCONTEXT, NULL);
}

void hanjp_ic_reset(HanjpInputContext *self)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    // to implement
}

void hanjp_ic_flush(HanjpInputContext *self)
{
    gint i;
    HanjpInputContextPrivate *priv;

    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));
    priv = hanjp_ic_get_instance_private(self);

    hanjp_am_flush(priv->cur_am);
    for(i = 0; i < priv->preedit->len; i++) {
        g_array_append_val(priv->committed, g_array_index(priv->preedit, gunichar, i));
    }
    g_array_set_size(priv->preedit, 0);
    priv->kana_len = 0;
}

gint hanjp_ic_process(HanjpInputContext *self, gint ascii)
{
    int i;
    gunichar ch;
    gint res;
    HanjpInputContextPrivate *priv;

    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));
    priv = hanjp_ic_get_instance_private(self);

    //map jaso from ascii
    ch = hanjp_keyboard_get_mapping(priv->keyboard, 0, ascii);
    if(ch == 0) {
        ch = (gunichar)ascii;
    }
    //shrink preedit before push
    g_array_set_size(priv->preedit, priv->kana_len);
    //push jaso into automata
    res = hanjp_am_push(priv->cur_am, priv->preedit, priv->hangul, ch);

    if(res < 0) {
        priv->kana_len += -res;
        hanjp_ic_flush(self);
    }
    else if(res > 0) {
        // if mode is katakana, change result to katakana
        if(priv->output_mode == HANJP_OUTPUT_KATAKANA) {
            for(i = priv->kana_len; i < priv->preedit->len; i++) {
                if(!is_hiragana(g_array_index(priv->preedit, gunichar, i))){
                    continue;
                }
                g_array_index(priv->preedit, gunichar, i) += KANA_GAP;
            }
        }
        priv->kana_len += res;
    }

    return res;
}

void hanjp_ic_toggle_preedit(HanjpInputContext *self)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    // to implement
}

void hanjp_ic_to_haragana_preedit(HanjpInputContext *self)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    // to implement

}

void hanjp_ic_to_katakana_preedit(HanjpInputContext *self)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    // to implement
}

void hanjp_ic_replace(HanjpInputContext *self, int start, int end, const gunichar* str_insert)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));
    g_return_if_fail(str_insert != NULL);

    // to implement
}

gboolean hanjp_ic_backspace(HanjpInputContext *self)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    // to implement
    return TRUE;
}

GArray* hanjp_ic_ref_preedit_string(HanjpInputContext *self)
{
    HanjpInputContextPrivate *priv;

    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    priv = hanjp_ic_get_instance_private(self);
    return g_array_ref(priv->preedit);
}

GArray* hanjp_ic_ref_commit_string(HanjpInputContext *self)
{
    HanjpInputContextPrivate *priv;

    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    priv = hanjp_ic_get_instance_private(self);
    return g_array_ref(priv->committed);
}

GArray* hanjp_ic_ref_hangul_string(HanjpInputContext *self)
{
    HanjpInputContextPrivate *priv;

    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    priv = hanjp_ic_get_instance_private(self);
    return g_array_ref(priv->hangul);
}

void hanjp_ic_set_am(HanjpInputContext *self, gint i)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    // to implement
}

void hanjp_ic_set_output_mode(HanjpInputContext *self, gint i)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    // to implement
}

gint hanjp_ic_get_output_mode(HanjpInputContext *self)
{
    g_return_if_fail(HANJP_IS_INPUTCONTEXT(self));

    // to implement
}