#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#define DOCTEST_CONFIG_TREAT_CHAR_STAR_AS_STRING
#include "doctest/doctest.h"

#include "ulog.h"
#include "ut_callback.h"

// Need to omit '\x1b' from the color strings
// to avoid issues with doctest string comparison
static const char *level_colors[] = {
    "[37m",  // TRACE : White #000
    "[36m",  // DEBUG : Cyan #0ff
    "[32m",  // INFO : Green #0f0
    "[33m",  // WARN : Yellow #ff0
    "[31m",  // ERROR : Red #f00
    "[35m"   // FATAL : Magenta #f0f
};
#define COLOR_TERMINATOR "\x1b[0m"  // Reset color

struct ColorTestFixture {
  public:
    static bool callback_is_set;
    static const c_str_opts callback_opts;

    ColorTestFixture() {
        // Per-test setup
        if (!callback_is_set) {
            ulog_add_callback(ut_callback, (void *)&callback_opts, LOG_TRACE);
            callback_is_set = true;
        }
        ulog_set_quiet(false);
        ut_callback_reset();
    }

    ~ColorTestFixture() = default;
};

bool ColorTestFixture::callback_is_set           = false;
const c_str_opts ColorTestFixture::callback_opts = {false, true, false};

void _check_color_start(const char *color) {
    const char *last               = ut_callback_get_last_message();
    const doctest::String last_str = last;

    REQUIRE(last != nullptr);
    REQUIRE(strlen(last) >= 5);
    // Check if it starts with escape character
    CHECK(last[0] == '\x1b');
    CHECK(last_str.substr(1, 4) == color);
    // Check if it ends with the color terminator
    CHECK(last_str.substr(last_str.size() - 4, 4) == COLOR_TERMINATOR);
}

TEST_CASE_FIXTURE(ColorTestFixture, "Color Levels") {
    log_trace("trace test");
    _check_color_start(level_colors[LOG_TRACE]);
    log_debug("debug test");
    _check_color_start(level_colors[LOG_DEBUG]);
    log_info("info test");
    _check_color_start(level_colors[LOG_INFO]);
    log_warn("warn test");
    _check_color_start(level_colors[LOG_WARN]);
    log_error("error test");
    _check_color_start(level_colors[LOG_ERROR]);
    log_fatal("fatal test");
    _check_color_start(level_colors[LOG_FATAL]);
    CHECK(ut_callback_get_message_count() == 6);
}
