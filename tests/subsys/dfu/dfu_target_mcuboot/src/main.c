/*
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <string.h>
#include <zephyr/types.h>
#include <stdbool.h>
#include <ztest.h>
#include <dfu_target.h>
#include <dfu_target_mcuboot.h>

/* Create buffer which we will fill with strings to test with
 * This is needed since dfu_ctx_mcuboot_set_b1_file will modify its
 * 'file' parameter.
 */
char buf[1024];

#define S0_S1 "s0 s1"
#define NO_SPACE "s0s1"

char *const flash_ptr = S0_S1;

static void test_dfu_ctx_mcuboot_set_b1_file(void)
{
	int err;
	const char *update;
	bool s0_active;

	memcpy(buf, S0_S1, sizeof(S0_S1));
	s0_active = true;
	err = dfu_ctx_mcuboot_set_b1_file(buf, s0_active, &update);

	zassert_equal(err, 0, "Selection of file from valid dual file "
			      "file specifier should not fail.");
	zassert_not_null(update, "Selected update for valid dual "
				 "file specifier should never be NULL");
	zassert_true(strcmp("s1", update) == 0, "s1 should be selected "
				 "when s0 is active");

	memcpy(buf, S0_S1, sizeof(S0_S1));
	s0_active = false;
	err = dfu_ctx_mcuboot_set_b1_file(buf, s0_active, &update);

	zassert_equal(err, 0, "Selection of file from valid dual file "
			      "file specifier should not fail.");
	zassert_not_null(update, "Selected update for valid dual "
				 "file specifier should never be NULL");
	zassert_true(strcmp("s0", update) == 0, "s0 should be selected "
				 "when s0 is inactive");
}

#ifdef CONFIG_BOARD_NATIVE_POSIX
static void test_dfu_ctx_mcuboot_set_b1_file__ptr_placement(void)
{
	/* This test cannot be executed on native posix as there is
	 * no mechanism in place do decude if an address maps to non-volatile
	 * storage or RAM
	 */
	ztest_test_skip();
}
#else
static void test_dfu_ctx_mcuboot_set_b1_file__ptr_placement(void)
{
	int err;
	const char *update;
	bool s0_active = true;

	err = dfu_ctx_mcuboot_set_b1_file(flash_ptr, s0_active, &update);
	zassert_true(err != 0, "Did not fail when given flash pointer");
}
#endif /* CONFIG_BOARD_NATIVE_POSIX */


static void test_dfu_ctx_mcuboot_set_b1_file__no_separator(void)
{
	int err;
	const char *update;
	bool s0_active = true;

	memcpy(buf, NO_SPACE, sizeof(NO_SPACE));
	err = dfu_ctx_mcuboot_set_b1_file(buf, s0_active, &update);

	zassert_equal(err, 0, "Should not get error when missing separator");
	zassert_equal(update, NULL, "update should be NULL when no separator");
}

static void test_dfu_ctx_mcuboot_set_b1_file__null(void)
{
	int err;
	const char *update;
	bool s0_active = true;

	err = dfu_ctx_mcuboot_set_b1_file(NULL, s0_active, &update);
	zassert_true(err < 0, NULL);

	err = dfu_ctx_mcuboot_set_b1_file(buf, s0_active, NULL);
	zassert_true(err < 0, NULL);
}

static void test_dfu_ctx_mcuboot_set_b1_file__not_terminated(void)
{
	int err;
	const char *update;
	bool s0_active = true;

	/* Remove any null terminator */
	for (int i = 0; i < sizeof(buf); ++i) {
		buf[i] = 'a';
	}
	err = dfu_ctx_mcuboot_set_b1_file(buf, s0_active, &update);
	zassert_true(err < 0, NULL);
}

static void test_dfu_ctx_mcuboot_set_b1_file__empty(void)
{
	int err;
	const char *update;
	bool s0_active = true;
	char empty[] = "";

	err = dfu_ctx_mcuboot_set_b1_file(empty, s0_active, &update);
	zassert_true(update == NULL, "update should not be set");
}

void test_main(void)
{
	ztest_test_suite(lib_dfu_target_mcuboot_test,
	     ztest_unit_test(test_dfu_ctx_mcuboot_set_b1_file__ptr_placement),
	     ztest_unit_test(test_dfu_ctx_mcuboot_set_b1_file__no_separator),
	     ztest_unit_test(test_dfu_ctx_mcuboot_set_b1_file__null),
	     ztest_unit_test(test_dfu_ctx_mcuboot_set_b1_file__not_terminated),
	     ztest_unit_test(test_dfu_ctx_mcuboot_set_b1_file__empty),
	     ztest_unit_test(test_dfu_ctx_mcuboot_set_b1_file)
	 );

	ztest_run_test_suite(lib_dfu_target_mcuboot_test);
}
