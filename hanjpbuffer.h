#ifndef __HANJP_BUFFER_H__
#define __HANJP_BUFFER_H__

#include <glib-object.h>
#include <gmodule.h>
#include <stdbool.h>

typedef union {
    struct {
        gunichar cho;
        gunichar jung;
        gunichar jung2;
        gunichar jong;
    };
    gunichar stack[4];
} HanjpBuffer;

extern gunichar hanjp_buffer_push(HanjpBuffer *buffer, gunichar ch);
extern gint hanjp_buffer_peek(HanjpBuffer *buffer);
extern gunichar hanjp_buffer_pop(HanjpBuffer *buffer);
extern void hanjp_buffer_flush(HanjpBuffer *buffer);

extern bool hanjp_buffer_is_valid(HanjpBuffer *buffer);
extern gint hanjp_buffer_copy_jamoes(HanjpBuffer *buffer, GArray *arr);
extern void hanjp_buffer_clear_filler(HanjpBuffer *buffer);
extern void hanjp_buffer_align_jungseong(HanjpBuffer *buffer);

extern gunichar hanjp_buffer_pop_choseong(HanjpBuffer *buffer);
extern gunichar hanjp_buffer_pop_jungseong(HanjpBuffer *buffer);
extern gunichar hanjp_buffer_pop_jongseong(HanjpBuffer *buffer);

#endif //__HANJP_BUFFER_H__
