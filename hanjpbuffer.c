#include "hanjpbuffer.h"
#include <gmodule.h>
#include <hangul.h>

/* Extern function signature which isn't exported in libhangul header */
extern ucschar hangul_choseong_to_jongseong(ucschar c);
extern ucschar hangul_jongseong_to_choseong(ucschar c);

/* HanjpBuffer Implementation */
gunichar
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

gint
hanjp_buffer_peek(HanjpBuffer *buffer) {
    int i;

    for(i = 0; i < 4; i++) {
        if(buffer->stack[i] != 0) {
            return buffer->stack[i];
        }
    }

	return 0;
}

gunichar
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

void
hanjp_buffer_flush(HanjpBuffer *buffer) {
    buffer->cho = 0;
    buffer->jung = 0;
    buffer->jung2 = 0;
    buffer->jong = 0;
}

/**/
