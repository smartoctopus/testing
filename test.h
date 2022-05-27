#ifndef TEST_H_
#define TEST_H_

#define UTILITY_IMPL
#include "utility.h"

/* Utility */
#define RANDOM_NUMBER __LINE__
#define RANDOM_IDENT__(_ident, _num) _ident##_num
#define RANDOM_IDENT_(_ident, _num) RANDOM_IDENT__(_ident, _num)
#define RANDOM_IDENT(_ident) RANDOM_IDENT_(_ident, RANDOM_NUMBER)

/* COLORS */
#define COLOR_RESET "\033[0m"
#define COLOR_RED "\033[1;31m"
#define COLOR_GREEN "\033[1;32m"
#define COLOR_YELLOW "\033[1;33m"
#define COLOR_WHITE "\033[1;37m"

/* Fundamental macros */
#define test(_str)                                                             \
  const char *test_name = (_str);                                              \
  void test_func(TestContext *ctx)

#define describe__(_str, _ident)                                               \
  for (int prev = ctx->current_describe,                                       \
           _ident = register_describe(ctx, _str);                              \
       _ident && run_describe(ctx, _str); _ident = describe_end(ctx, prev))
#define describe_(_str, _ident) describe__(_str, _ident)
#define describe(_str) describe_((_str), RANDOM_IDENT(describe))

#define it__(_str, _ident)                                                     \
  for (bool _ident = register_it(ctx, _str, false);                            \
       _ident && run_it(ctx, _str); _ident = false)
#define it_(_str, _ident) it__(_str, _ident)
#define it(_str) it_((_str), RANDOM_IDENT(it))

#define skip(_str) for ((void)register_it(ctx, (_str), true); false;)

#define before__(_ident)                                                       \
  for (bool _ident = register_before(ctx); _ident && run_it(ctx, "before");    \
       _ident = false)
#define before_(_ident) before__(_ident)
#define before() before_(RANDOM_IDENT(before))

#define before_each__(_ident)                                                  \
  for (bool _ident = register_before_each(ctx);                                \
       _ident && run_it(ctx, "before_each"); _ident = false)
#define before_each_(_ident) before_each__(_ident)
#define before_each() before_each_(RANDOM_IDENT(before_each))

#define after__(_ident)                                                        \
  for (bool _ident = register_after(ctx); _ident && run_it(ctx, "after");      \
       _ident = false)
#define after_(_ident) after__(_ident)
#define after() after_(RANDOM_IDENT(after))

#define after_each__(_ident)                                                   \
  for (bool _ident = register_after_each(ctx);                                 \
       _ident && run_it(ctx, "after_each"); _ident = false)
#define after_each_(_ident) after_each__(_ident)
#define after_each() after_each_(RANDOM_IDENT(after_each))

#define expect(_expr) expect_(ctx, _expr, #_expr)

/* Types */
typedef struct Test {
  const char *name;
  bool skip;
} Test;

typedef struct Describe {
  uint32_t current_test;
  int8_t before;
  int8_t after;
  int8_t before_each;
  int8_t after_each;
  const char *name;
  array(Test) tests;
} Describe;

typedef struct TestContext {
  bool use_color;
  bool pre_run;
  uint16_t passed_tests;
  uint16_t skipped_tests;
  uint16_t failed_tests;
  uint32_t current_describe;
  uint32_t indent;
  array(Describe) describes;
} TestContext;

/* Functions */
bool cmp_str(const char *s1, const char *s2) {
  size_t s1_length = strlen(s1);
  size_t s2_length = strlen(s2);
  if (s1_length != s2_length) {
    return false;
  }
  return strncmp(s1, s2, s1_length) == 0;
}

int register_describe(TestContext *ctx, const char *name) {
  if (ctx->pre_run) {
    Describe describe;
    describe.name = name;
    describe.tests = NULL;
    describe.current_test = 0;
    describe.before = -1;
    describe.after = -1;
    describe.before_each = -1;
    describe.after_each = -1;

    array_push(ctx->describes, describe);
    ctx->current_describe = array_length(ctx->describes) - 1;
  }
  return true;
}

bool run_describe(TestContext *ctx, const char *name) {
  if (ctx->pre_run) {
    return true;
  }
  return cmp_str(ctx->describes[ctx->current_describe].name, name);
}

int describe_end(TestContext *ctx, int prev) {
  ctx->current_describe = prev;
  return false;
}

bool register_it(TestContext *ctx, const char *name, bool skip) {
  if (ctx->pre_run) {
    Test test;
    test.name = name;
    test.skip = skip;
    array_push(ctx->describes[ctx->current_describe].tests, test);
  }
  return true;
}

bool run_it(TestContext *ctx, const char *name) {
  if (ctx->pre_run) {
    return false;
  }
  Describe describe = ctx->describes[ctx->current_describe];
  return cmp_str(describe.tests[describe.current_test].name, name);
}

bool register_before(TestContext *ctx) {
  if (ctx->pre_run) {
    Test before = {"before", false};
    array_push(ctx->describes[ctx->current_describe].tests, before);
    ctx->describes[ctx->current_describe].before =
        array_length(ctx->describes[ctx->current_describe].tests) - 1;
  }
  return true;
}

bool register_before_each(TestContext *ctx) {
  if (ctx->pre_run) {
    Test before_each = {"before_each", false};
    array_push(ctx->describes[ctx->current_describe].tests, before_each);
    ctx->describes[ctx->current_describe].before_each =
        array_length(ctx->describes[ctx->current_describe].tests) - 1;
  }
  return true;
}

bool register_after(TestContext *ctx) {
  if (ctx->pre_run) {
    Test after = {"after", false};
    array_push(ctx->describes[ctx->current_describe].tests, after);
    ctx->describes[ctx->current_describe].after =
        array_length(ctx->describes[ctx->current_describe].tests) - 1;
  }
  return true;
}

bool register_after_each(TestContext *ctx) {
  if (ctx->pre_run) {
    Test after_each = {"after_each", false};
    array_push(ctx->describes[ctx->current_describe].tests, after_each);
    ctx->describes[ctx->current_describe].after_each =
        array_length(ctx->describes[ctx->current_describe].tests) - 1;
  }
  return true;
}

void expect_(TestContext *ctx, bool value, const char *expr) {
  if (value) {
    ctx->passed_tests++;
    fprintf(stderr, "%*s" COLOR_GREEN "✓" COLOR_RESET " %s\n", ctx->indent * 2,
            "", expr);
  } else {
    ctx->failed_tests++;
    fprintf(stderr, "%*s" COLOR_RED "◯" COLOR_RESET " %s\n", ctx->indent * 2,
            "", expr);
  }
}

#ifndef CUSTOM_MAIN
extern const char *test_name;
extern void test_func(TestContext *ctx);

int main(void) {
  TestContext ctx = {0};

  /* Pre run */
  ctx.pre_run = true;
  (void)register_describe(&ctx, test_name);
  test_func(&ctx);

  /* Actual run */
  ctx.pre_run = false;
  for (; ctx.current_describe < array_length(ctx.describes);
       ctx.current_describe++) {
    Describe *describe = &ctx.describes[ctx.current_describe];
    fprintf(stderr, "%*s• %s:\n", ctx.indent * 2, "", describe->name);
    // Describe indentation
    ctx.indent++;
    /* Before */
    if (describe->before != -1) {
      describe->current_test = describe->before;
      test_func(&ctx);
      describe->current_test = 0;
    }
    while (describe->current_test < array_length(describe->tests)) {
      int test = describe->current_test;
      if (test == describe->before || test == describe->before_each ||
          test == describe->after || test == describe->after_each) {
        describe->current_test++;
        continue;
      }

      if (describe->before_each != -1) {
        describe->current_test = describe->before_each;
        test_func(&ctx);
        describe->current_test = test;
      }

      fprintf(stderr, "%*s• %s:", ctx.indent * 2, "",
              describe->tests[test].name);
      if (describe->tests[test].skip) {
        ctx.skipped_tests++;
        fprintf(stderr, " " COLOR_YELLOW "Skipped" COLOR_RESET);
      }
      fprintf(stderr, "\n");

      // Test
      ctx.indent++;
      test_func(&ctx);
      ctx.indent--;

      if (describe->after_each != -1) {
        describe->current_test = describe->after_each;
        test_func(&ctx);
        describe->current_test = test;
      }

      describe->current_test++;
    }
    if (describe->after != -1) {
      describe->current_test = describe->after;
      test_func(&ctx);
    }

    if (cmp_str(describe->name, test_name)) {
      ctx.indent++;
    }

    // Describe indentation
    ctx.indent--;
  }

  // Print the report
  ctx.indent--;
  fprintf(stderr, "\n" COLOR_WHITE "Tests:" COLOR_RESET "\n");
  fprintf(stderr, "  • " COLOR_GREEN "%d Passed" COLOR_RESET "\n",
          ctx.passed_tests);
  fprintf(stderr, "  • " COLOR_YELLOW "%d Skipped" COLOR_RESET "\n",
          ctx.skipped_tests);
  fprintf(stderr, "  • " COLOR_RED "%d Failed" COLOR_RESET "\n",
          ctx.failed_tests);

  return 0;
}
#endif

#endif /* TEST_H_ */
