#pragma once

#include "ulog.h"

#ifdef __cplusplus
extern "C" {
#endif

#define UT_LOG_BUFFER_SIZE 256

// Options for creating c_str
typedef struct {
  bool full_time;
  bool color;
  bool new_line;
} c_str_opts;

void ut_callback(ulog_Event *ev, void *arg);

int ut_callback_get_message_count();
char *ut_callback_get_last_message();
void ut_callback_reset();

#ifdef __cplusplus
}
#endif
