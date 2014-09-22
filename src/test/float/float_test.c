#include <math.h>
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "api/float_api.h"

START_TEST ( test_read_write_float_instances ) {
    const float FLOAT_EPSILON = 0.0001;
    skill_state state = empty_skill_state ();

    create_floattest ( state, -0.1, 0.0, 0.1, 1.0, 99.99 );
    create_floattest ( state, 2.0, 2.0, 2.0, 2.0, 2.0 );

    write_to_file ( state, "./resources/floats.sf" );

    state = skill_state_from_file ( "./resources/floats.sf" );
    GList *floattest_instances = get_floattest_instances ( state );
    ck_assert_msg ( g_list_length ( floattest_instances ) == 2, "Expected exactly 2 instances" );

    floattest first_instance = g_list_nth_data ( floattest_instances, 0 );
    floattest second_instance = g_list_nth_data ( floattest_instances, 1 );

    ck_assert ( fabs ( floattest_get_zero ( first_instance ) - ( -0.1) ) < FLOAT_EPSILON );
    ck_assert ( fabs ( floattest_get_minuszero ( first_instance ) - 0.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( floattest_get_two ( first_instance ) - 0.1 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( floattest_get_pi ( first_instance ) - 1.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( floattest_get_nan ( first_instance ) - 99.99 ) < FLOAT_EPSILON );

    ck_assert ( fabs ( floattest_get_zero ( second_instance ) - 2.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( floattest_get_minuszero ( second_instance ) - 2.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( floattest_get_two ( second_instance ) - 2.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( floattest_get_pi ( second_instance ) - 2.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( floattest_get_nan ( second_instance ) - 2.0 ) < FLOAT_EPSILON );

} END_TEST

int main () {

    Suite *test_suite = suite_create ( "test suite" );
    TCase *testcase_1 = tcase_create ( "check the values of floattest instances, which have been written to a file and read in again\n" );
    SRunner *test_runner = srunner_create ( test_suite );
    int number_of_tests_failed;

    suite_add_tcase ( test_suite, testcase_1);
    tcase_add_test ( testcase_1, test_read_write_float_instances );

    srunner_run_all ( test_runner, CK_VERBOSE );
    number_of_tests_failed = srunner_ntests_failed ( test_runner );
    srunner_free ( test_runner );

    printf ( "Number of failed tests: %d\n", number_of_tests_failed );

    return number_of_tests_failed == 0 ? 0 : 1;
}
