#include "hanjpautomata.h"
#include "hanjpunicode.h"
#include <gmodule.h>
#include <hangul.h>

/* Extern function signature which isn't exported in libhangul header */
extern ucschar hangul_choseong_to_jongseong(ucschar c);
extern ucschar hangul_jongseong_to_choseong(ucschar c);

/* HanjpBuffer Implementation */
static gunichar
hanjp_buffer_push(HanjpBuffer *buffer, gunichar ch) {
    if(hangul_is_choseong(ch)) {
        if(buffer->cho == 0) {
            buffer->cho = ch;
        }
        else {
            return ch;
        }
    }
    else if(hangul_is_jungseong(ch)) {
        if(buffer->jung == 0) {
            buffer->jung = ch;
        }
        else {
            if(buffer->jung2 == 0) {
                buffer->jung2 = ch;
            }
            else {
                return ch;
            }
        }
    }
    else if(hangul_is_jongseong(ch)) {
        if(buffer->jong == 0) {
            buffer->jong = ch;
        }
        else {
            return ch;
        }
    }
    else {
        return ch;
    }

    return 0;
}

static gint
hanjp_buffer_peek(HanjpBuffer *buffer, GArray *dest) {
    int i;

    for(i = 0; i < 4; i++) {
        if(buffer->stack[i] != 0) {
            g_array_append_val(dest, buffer->stack[i]);
        }
    }
}

static gunichar
hanjp_buffer_pop(HanjpBuffer *buffer) {
    int i;
    gunichar r;

    for(i = 3; i >= 0; i--) {
        r = buffer->stack[i];
        if(r != 0) {
            buffer->stack[i] = 0;
            break;
        }
    }

    return r;
}

static void
hanjp_buffer_flush(HanjpBuffer *buffer) {
    buffer->cho = 0;
    buffer->jung = 0;
    buffer->jung2 = 0;
    buffer->jong = 0;
}

/**/

/* Kana Table index enums */
enum {
    KANA_VOWEL_A,
    KANA_VOWEL_I,
    KANA_VOWEL_U,
    KANA_VOWEL_E,
    KANA_VOWEL_O
};

enum {
    KANA_CONSONANT__,
    KANA_CONSONANT_K,
    KANA_CONSONANT_S,
    KANA_CONSONANT_T,
    KANA_CONSONANT_N,
    KANA_CONSONANT_H,
    KANA_CONSONANT_M,
    KANA_CONSONANT_Y,
    KANA_CONSONANT_R,
    KANA_CONSONANT_W
};

// Fifty notes
// For example か(ka) = kana_table[KANA_CONSONANT_K][KANA_VOWEL_A]
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
static const gunichar kana_nn = 0x3093;

// Combine multiple JungSeong into single int code
typedef union {
    struct {
        gunichar jung;
        gunichar jung2;
    };
    guint64 value;
} JungBox;

#define N_COMBINE_TABLE_ELEMENTS 30


/* Automata Interface Definition */
G_DEFINE_INTERFACE(HanjpAutomata, hanjp_am, G_TYPE_OBJECT)

static void
hanjp_am_default_init(HanjpAutomataInterface *iface) {
    /* add properties and signals to the interface here */
}

gint hanjp_am_to_kana(HanjpAutomata *am, GArray *dest, HanjpBuffer *buffer) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AM(am));
    
    iface = HANJP_AM_GET_IFACE(am);
    g_return_if_fail(iface->to_kana != NULL);
    return iface->to_kana(am, dest, buffer);
}

gint hanjp_am_push(HanjpAutomata *am, GArray *result, GArray *hangul, gunichar ch) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AM(am));
    
    iface = HANJP_AM_GET_IFACE(am);
    g_return_if_fail(iface->push != NULL);
    return iface->push(am, result, hangul, ch);
}

gboolean hanjp_am_backspace(HanjpAutomata *am) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AM(am));

    iface = HANJP_AM_GET_IFACE(am);
    g_return_if_fail(iface->backspace != NULL);
    return iface->backspace(am);
}

void hanjp_am_flush(HanjpAutomata *am) {
    HanjpAutomataInterface *iface;

    g_return_if_fail(HANJP_IS_AM(am));

    iface = HANJP_AM_GET_IFACE(am);
    g_return_if_fail(iface->flush != NULL);
    iface->flush(am);
}

/**/

/* AutomataBase Implementation */
typedef struct {
    HanjpBuffer buffer;
    GHashTable *combine_table;
    JungBox combine_table_keys[N_COMBINE_TABLE_ELEMENTS];
    guint32 combine_table_vals[N_COMBINE_TABLE_ELEMENTS];
} HanjpAutomataBasePrivate;

G_DEFINE_TYPE_WITH_PRIVATE(HanjpAutomataBase, hanjp_am_base, G_TYPE_OBJECT)

static gint
hanjp_am_base_to_kana(HanjpAutomata *am, GArray *dest, HanjpBuffer *buffer)
{
    gint r = 0;
    gint adj;
    gint i, j;
    JungBox jungkey;
    gunichar ch;
    gunichar *val;
    HanjpAutomataBasePrivate *priv;

    priv = hanjp_am_base_get_instance_private(HANJP_AM_BASE(am));

    // Check buffer
    for(i = 0; i < 4; i++) {
        if(buffer->stack[i] != 0 && !hangul_is_jamo(buffer->stack[i])) {
            for(i = 0; i < 4; i++) {
                r = 0;
                if(buffer->stack[i] != 0) {
                    g_array_append_val(dest, buffer->stack[i]);
                    r++;
                }
            }
            return -r;
        }
    }

    // Replace Choseong filler and Jungseong filler with '0'
    for(i = 0; i < 4; i++) {
        if(buffer->stack[i] == HANGUL_CHOSEONG_FILLER
				|| buffer->stack[i] == HANGUL_JUNGSEONG_FILLER) {
            buffer->stack[i] = 0;
        }
        buffer->cho = 0;
    }

    // check whether batchim is available and move choseong to jongseong if conditions are met
    if(buffer->cho != 0 && buffer->jung == 0 && dest->len != 0) {
        ch = g_array_index(dest, gunichar, dest->len - 1); // Last kana character
        if(ch != kana_nn && ch != kana_table[KANA_CONSONANT_T][KANA_VOWEL_U] - 1) {
            buffer->jong = hangul_choseong_to_jongseong(buffer->cho);
            buffer->cho = 0;
        }
    }
    
    //eat Choseong and Jungseong
    while(buffer->cho || buffer->jung || buffer->jung2) {
        adj = 0;
        ch = buffer->cho; // victim
        buffer->cho = 0;
        // Select row index and set adjuster
        switch(ch) {
            case 0:         // VOID
            adj = -1;
            case HANGUL_CHOSEONG_IEUNG:         // ㅇ
            i = KANA_CONSONANT__; break;
            case HANGUL_CHOSEONG_KIYEOK:        // ㄱ
            adj = 1;
            case HANGUL_CHOSEONG_KHIEUKH:       // ㅋ
            case HANGUL_CHOSEONG_SSANGKIYEOK:   // ㄲ
            i = KANA_CONSONANT_K; break;   // K
            case HANGUL_CHOSEONG_CIEUC:         // ㅈ
            adj = 1;
            case HANGUL_CHOSEONG_SIOS:          // ㅅ
            case HANGUL_CHOSEONG_SSANGSIOS:     // ㅆ
            i = KANA_CONSONANT_S; break;   // S
            case HANGUL_CHOSEONG_TIKEUT:        // ㄷ
            adj = 1;
            case HANGUL_CHOSEONG_SSANGTIKEUT:   // ㄸ
            case HANGUL_CHOSEONG_CHIEUCH:       // ㅊ
            case HANGUL_CHOSEONG_THIEUTH:       // ㅌ
            i = KANA_CONSONANT_T; break;   // T
            case HANGUL_CHOSEONG_NIEUN:         // ㄴ
            i = KANA_CONSONANT_N; break;   // N
            case HANGUL_CHOSEONG_PHIEUPH:       // ㅍ
            case HANGUL_CHOSEONG_SSANGPIEUP:    // ㅃ
			adj = 1;
			case HANGUL_CHOSEONG_PIEUP:			// ㅂ
            adj += 1;
            case HANGUL_CHOSEONG_HIEUH:         // ㅎ
            i = KANA_CONSONANT_H; break;   // H
            case HANGUL_CHOSEONG_MIEUM:         // ㅁ
            i = KANA_CONSONANT_M; break;   // M
            case HANGUL_CHOSEONG_RIEUL:         // ㄹ
            i = KANA_CONSONANT_R; break;   // R
            case HANGUL_CHOSEONG_SSANGNIEUN:
            g_array_append_val(dest, kana_nn);
            r++;
            continue;
            default:
            for(i = 0; i < 4; i++) {
                ch = buffer->stack[i];
                if(ch != 0) {
                    g_array_append_val(dest, ch);
                    r++;
                }
            }
            return -r;
        }

        // Reduce Jungseong to single character
        ch = buffer->jung2; // victim
        buffer->jung2 = 0;
        if(buffer->jung == 0) {
            buffer->jung = ch;
        }
        else {
            jungkey.jung = buffer->jung;
            jungkey.jung2 = ch;
            val = (gunichar *)g_hash_table_lookup(priv->combine_table, &jungkey.value);
            if(val != NULL) {
                buffer->jung = *val;
            }
            else {
                buffer->jung2 = ch;
            }
        }

        // Divide Jungseong into eatable
        ch = buffer->jung;
        switch(ch) {
            case HANGUL_JUNGSEONG_WA:
            if(i == KANA_CONSONANT__) {
                i = KANA_CONSONANT_W;
            }
            else {
                buffer->jung = HANGUL_JUNGSEONG_O;
                buffer->jung2 = HANGUL_JUNGSEONG_A;
            }
            break;
            case HANGUL_JUNGSEONG_YA:
            case HANGUL_JUNGSEONG_YU:
            case HANGUL_JUNGSEONG_YO:
            case HANGUL_JUNGSEONG_YEO:
            if(i == KANA_CONSONANT__) {
                i = KANA_CONSONANT_Y;
            }
            else{
                buffer->jung = HANGUL_JUNGSEONG_I;
                buffer->jung2 = ch;
            }
            break;
            case HANGUL_JUNGSEONG_YE:
            case HANGUL_JUNGSEONG_YAE:
            buffer->jung = HANGUL_JUNGSEONG_I;
            buffer->jung2 = HANGUL_JUNGSEONG_E;
            break;
        }

        //select column index
        ch = buffer->jung; // victim
        buffer->jung = 0;
        switch(ch) {
            case HANGUL_JUNGSEONG_WA:
            case HANGUL_JUNGSEONG_YA:
            case HANGUL_JUNGSEONG_A:
            j = KANA_VOWEL_A; break;
            case HANGUL_JUNGSEONG_I:
            j = KANA_VOWEL_I; break;
            case HANGUL_JUNGSEONG_YU:
            case HANGUL_JUNGSEONG_EU:
            case HANGUL_JUNGSEONG_U:
            case 0:
            j = KANA_VOWEL_U; break;
            case HANGUL_JUNGSEONG_AE:
            case HANGUL_JUNGSEONG_E:
            j = KANA_VOWEL_E; break;
            case HANGUL_JUNGSEONG_YO:
            case HANGUL_JUNGSEONG_YEO:
            case HANGUL_JUNGSEONG_EO:
            case HANGUL_JUNGSEONG_O:
            j = KANA_VOWEL_O; break;
            default:
            for(i = 0; i < 4; i++) {
                ch = buffer->stack[i];
                if(ch != 0) {
                    g_array_append_val(dest, ch);
                    r++;
                }
            }
            return -r;
        }

        ch = kana_table[i][j] + adj;
        g_array_append_val(dest, ch);
        r++;
    }

    // eat jongseong
    ch = buffer->jong;
    buffer->jong = 0;
    if(ch != 0) {
        switch(ch) {
            case HANGUL_JONGSEONG_KIYEOK:
            case HANGUL_JONGSEONG_SSANGKIYEOK:
            case HANGUL_JONGSEONG_SIOS:
            case HANGUL_JONGSEONG_SSANGSIOS:
            case HANGUL_JONGSEONG_KHIEUKH:
            ch = kana_table[KANA_CONSONANT_T][KANA_VOWEL_U] - 1; break;
            case HANGUL_JONGSEONG_NIEUN:
            case HANGUL_JONGSEONG_MIEUM:
            case HANGUL_JONGSEONG_PIEUP:
            case HANGUL_JONGSEONG_PHIEUPH:
            case HANGUL_JONGSEONG_IEUNG:
            ch = kana_nn; break;
            default:
            buffer->cho = hangul_jongseong_to_choseong(ch);
            return r + hanjp_am_base_to_kana(am, dest, buffer);
        }
        g_array_append_val(dest, ch);
        r++;
    }

    return r;
}

static void
hanjp_am_base_peek(HanjpAutomata *am, GArray *hangul)
{
    JungBox jungkey;
    gunichar c;
    gunichar *val;
    gint i;
    HanjpAutomataBasePrivate *priv;

    priv = hanjp_am_base_get_instance_private(HANJP_AM_BASE(am));

    if(priv->buffer.jung == 0) {
        priv->buffer.jung = priv->buffer.jung2;
        priv->buffer.jung2 = 0;
    }
    
    if(priv->buffer.jung != 0 && priv->buffer.jung2 != 0) {
        jungkey.jung = priv->buffer.jung;
        jungkey.jung2 = priv->buffer.jung2;
        val = (gunichar*)g_hash_table_lookup(priv->combine_table, &jungkey.value);
        if(val != NULL) {
            priv->buffer.jung = *val;
            priv->buffer.jung2 = 0;
        }
    }

    c = hangul_jamo_to_syllable(priv->buffer.cho, priv->buffer.jung, priv->buffer.jong);
    if(c == 0) {
        for(i = 0; i < 4; i++) {
            c = priv->buffer.stack[i];
            if(c != 0) {
                c = hangul_jamo_to_cjamo(c);
                g_array_append_val(hangul, c);
                break;
            }
        }  
    }
    else {
        g_array_append_val(hangul, c);
    }
}

static gboolean
hanjp_am_base_backspace(HanjpAutomata *am)
{
    int i;
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_am_base_get_instance_private(HANJP_AM_BASE(am));

    return (hanjp_buffer_pop(&priv->buffer) != 0);
}

static void
hanjp_am_base_flush(HanjpAutomata *am)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_am_base_get_instance_private(HANJP_AM_BASE(am));
    hanjp_buffer_flush(&priv->buffer);
}

static void
hanjp_am_base_init(HanjpAutomataBase *am)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_am_base_get_instance_private(am);

    priv->buffer.cho = 0;
    priv->buffer.jung = 0;
    priv->buffer.jung2 = 0;
    priv->buffer.jong = 0;

    priv->combine_table = g_hash_table_new(g_int64_hash, g_int64_equal);
}

static void
hanjp_am_base_finalize(GObject *gobject)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_am_base_get_instance_private(HANJP_AM_BASE(gobject));

    g_hash_table_destroy(priv->combine_table);

    G_OBJECT_CLASS(hanjp_am_base_parent_class)->finalize(gobject);
}

static void
hanjp_am_base_class_init(HanjpAutomataBaseClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    
    object_class->finalize = hanjp_am_base_finalize;

    klass->to_kana = hanjp_am_base_to_kana;
    klass->push = NULL;
    klass->backspace = hanjp_am_base_backspace;
    klass->flush = hanjp_am_base_flush;
}

/**/

/* AutomataBuiltin Implementation */
static void hanjp_am_builtin_interface_init(HanjpAutomataInterface *iface);
G_DEFINE_TYPE_WITH_CODE(HanjpAutomataBuiltin, hanjp_am_builtin, HANJP_TYPE_AM_BASE,
        G_IMPLEMENT_INTERFACE(HANJP_TYPE_AM,
                hanjp_am_builtin_interface_init))

static void
hanjp_am_builtin_init(HanjpAutomataBuiltin *am)
{
    HanjpAutomataBasePrivate *priv;
    priv = hanjp_am_base_get_instance_private(HANJP_AM_BASE(am));
    priv->combine_table_keys[0].jung = HANGUL_JUNGSEONG_O;
    priv->combine_table_keys[0].jung2 = HANGUL_JUNGSEONG_A;
    priv->combine_table_vals[0] = HANGUL_JUNGSEONG_WA;

    g_hash_table_insert(priv->combine_table, &priv->combine_table_keys[0].value, &priv->combine_table_vals[0]);
}

static gint
hanjp_am_builtin_push(HanjpAutomata *am, GArray *preedit, GArray *hangul, gunichar ch)
{
    gint r = 0;
    HanjpAutomataBasePrivate *priv;
    HanjpBuffer *buffer;

    priv = hanjp_am_base_get_instance_private(HANJP_AM_BASE(am));
    buffer = &priv->buffer;

    if(ch == 0) {
        return 0;
    }

    // push jaso to buffer
    ch = hanjp_buffer_push(buffer, ch);

    // post-step
    // convert poped string to kana
    if(ch != 0 && buffer->jong == 0) {
        hanjp_am_base_peek(am, hangul);
        r = hanjp_am_base_to_kana(am, preedit, buffer);
        if(!hangul_is_jamo(ch)) {
            g_array_append_val(hangul, ch);
            g_array_append_val(preedit, ch);
            r++;
            r = -r;
        }
        else {
            hanjp_buffer_push(buffer, ch);
        }
    }
    hanjp_am_base_peek(am, preedit);

    return r;
}

static void
hanjp_am_builtin_class_init(HanjpAutomataBuiltinClass *klass)
{
    HanjpAutomataBaseClass *base_class = HANJP_AM_BASE_CLASS(klass);

    base_class->push = hanjp_am_builtin_push;
}

HanjpAutomataBuiltin *hanjp_am_builtin_new()
{
    return g_object_new(HANJP_TYPE_AM_BUILTIN, NULL);
}

static void
hanjp_am_builtin_interface_init(HanjpAutomataInterface *iface)
{
    iface->to_kana = hanjp_am_base_to_kana;
    iface->push = hanjp_am_builtin_push;
    iface->backspace = hanjp_am_base_backspace;
    iface->flush = hanjp_am_base_flush;
}

/**/
