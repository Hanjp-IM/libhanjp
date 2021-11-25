#include "hanjpbuffer.h"
#include <gmodule.h>
#include <hangul.h>

/* Extern function signature which isn't exported in libhangul header */
extern ucschar hangul_choseong_to_jongseong(ucschar c);
extern ucschar hangul_jongseong_to_choseong(ucschar c);

static const int BUFF_SIZE =
	(sizeof(((HanjpBuffer*)0)->stack) / sizeof(((HanjpBuffer*)0)->stack[0]));

/* HanjpBuffer Implementation */
gunichar
hanjp_buffer_push(HanjpBuffer *buffer, gunichar ch) {
    if ((hangul_is_choseong(ch)) && (buffer->cho == 0)) {
        buffer->cho = ch;
		return 0;
    }
    else if (hangul_is_jungseong(ch)) {
        if(buffer->jung == 0) {
            buffer->jung = ch;
			return 0;
        }
        else if(buffer->jung2 == 0) {
            buffer->jung2 = ch;
			return 0;
        }
    }
    else if ((hangul_is_jongseong(ch)) && (buffer->jong == 0)) {
        buffer->jong = ch;
		return 0;
    }

    return ch;
}

gint
hanjp_buffer_peek(HanjpBuffer *buffer) {
	int i;

    for (i = 0; i < BUFF_SIZE; i++) {
        if (buffer->stack[i]) {
            return buffer->stack[i];
        }
    }

	return 0;
}

gunichar
hanjp_buffer_pop(HanjpBuffer *buffer) {
	gint ret = 0;
    int i;

    for (i = BUFF_SIZE-1; i > -1; i--) {
        if (buffer->stack[i]) {
			ret = buffer->stack[i];
            buffer->stack[i] = 0;
            break;
        }
    }

    return ret;
}

void
hanjp_buffer_flush(HanjpBuffer *buffer) {
    buffer->cho = 0;
    buffer->jung = 0;
    buffer->jung2 = 0;
    buffer->jong = 0;
}

/**/
