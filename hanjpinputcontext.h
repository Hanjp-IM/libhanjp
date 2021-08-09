#ifndef __HANJP_INPUTCONTEXT_H__
#define __HANJP_INPUTCONTEXT_H__

#include <glib-object.h>
#include <gmodule.h>

G_BEGIN_DECLS

#define HANJP_TYPE_INPUTCONTEXT hanjp_inputcontext_get_type()
G_DECLARE_DERIVABLE_TYPE(HanjpInputContext, hanjp_inputcontext, HANJP, INPUTCONTEXT, GObject)

struct _HanjpInputContextClass
{
    GObjectClass parent_class;

    void (*reset) (HanjpInputContext *self);
    void (*toggle_preedit) (HanjpInputContext *self);
    void (*to_haragana_preedit) (HanjpInputContext *self);
    void (*to_katakana_preedit) (HanjpInputContext *self);
    gboolean (*replace) (HanjpInputContext *self, int start, int end, GArray *str_insert);
};

HanjpInputContext *hanjp_inputcontext_new();

G_END_DECLS

#endif // __HANJP_INPUTCONTEXT_H__