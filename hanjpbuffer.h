#ifndef __HANJP_BUFFER_H__
#define __HANJP_BUFFER_H__

#include <glib-object.h>
#include <gmodule.h>


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

#endif //__HANJP_BUFFER_H__
