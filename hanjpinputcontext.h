#ifndef __HANJP_INPUTCONTEXT_H__
#define __HANJP_INPUTCONTEXT_H__

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

enum {
    HANJP_OUTPUT_HIRAGANA,
    HANJP_OUTPUT_KATAKANA,
    HANJP_OUTPUT_HALFKATAKANA
};

#define HANJP_TYPE_INPUTCONTEXT hanjp_ic_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpInputContext, hanjp_ic, HANJP, INPUTCONTEXT, GObject)

struct _HanjpInputContextClass
{
    GObjectClass parent_class;

    void (*reset) (HanjpInputContext *self);
    void (*flush) (HanjpInputContext *self);
    gint (*process) (HanjpInputContext *self, gint ascii);
    void (*toggle_preedit) (HanjpInputContext *self);
    void (*to_haragana_preedit) (HanjpInputContext *self);
    void (*to_katakana_preedit) (HanjpInputContext *self);
    void (*replace) (HanjpInputContext *self, int start, int end, const gunichar* str_insert);
    gboolean (*backspace) (HanjpInputContext *self);
    GArray* (*ref_preedit_string) (HanjpInputContext *self);
    GArray* (*ref_commit_string) (HanjpInputContext *self);
    GArray* (*ref_hangul_string) (HanjpInputContext *self);
    void (*set_am) (HanjpInputContext *self, gint i);
    void (*set_output_mode) (HanjpInputContext *self, gint i);
    gint (*get_output_mode) (HanjpInputContext *self);
};

HanjpInputContext *hanjp_ic_new();
void hanjp_ic_reset(HanjpInputContext *self);
void hanjp_ic_flush(HanjpInputContext *self);
gint hanjp_ic_process(HanjpInputContext *self, gint ascii);
void hanjp_ic_toggle_preedit(HanjpInputContext *self);
void hanjp_ic_to_haragana_preedit(HanjpInputContext *self);
void hanjp_ic_to_katakana_preedit(HanjpInputContext *self);
void hanjp_ic_replace(HanjpInputContext *self, int start, int end, const gunichar* str_insert);
gboolean hanjp_ic_backspace(HanjpInputContext *self);
GArray* hanjp_ic_ref_preedit_string(HanjpInputContext *self);
GArray* hanjp_ic_ref_commit_string(HanjpInputContext *self);
GArray* hanjp_ic_ref_hangul_string(HanjpInputContext *self);
void hanjp_ic_set_am(HanjpInputContext *self, gint i);
void hanjp_ic_set_output_mode(HanjpInputContext *self, gint i);
gint hanjp_ic_get_output_mode(HanjpInputContext *self);

G_END_DECLS

#endif // __HANJP_INPUTCONTEXT_H__