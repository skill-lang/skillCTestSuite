#include <math.h>
#include <check.h>
#include <stdlib.h>
#include "access/float_skill_state.h"
#include "access/float_floattest_access.h"
#include "access/float_doubletest_access.h"
#include "types/float_types.h"

START_TEST ( empty_test ) {
    mark_point ();
    int i = 1;
} END_TEST

START_TEST ( test_read_write_float_instances ) {
    printf ( "started test.\n" );
    mark_point ();
    const float FLOAT_EPSILON = 0.0001;
    skill_state *state = skill_state_empty_skill_state ();

    floattest_access *float_access = state->floattest_access;
    doubletest_access *double_access = state->doubletest_access;

    floattest_access_create_instance ( float_access, -0.1, 0.0, 0.1, 1.0, 99.99 );
    floattest_access_create_instance ( float_access, 2.0, 2.0, 2.0, 2.0, 2.0 );

    skill_state_write_to_file ( state, "./resources/floats.sf" );

    state = skill_state_from_file ( "./resources/floats.sf" );
    int number_of_float_instances = floattest_access_get_number_of_instances ( state->floattest_access );
    ck_assert_msg ( number_of_float_instances == 2, "Expected exactly 2 instances" );

    FloatTest *instances = floattest_access_get_instances ( state->floattest_access );

    FloatTest *first_instance = &instances[0];
    FloatTest *second_instance = &instances[1];

    ck_assert ( fabs ( first_instance->zero - ( -0.1) ) < FLOAT_EPSILON );
    ck_assert ( fabs ( first_instance->minusZero - 0.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( first_instance->two - 0.1 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( first_instance->pi - 1.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( first_instance->NaN - 99.99 ) < FLOAT_EPSILON );


    ck_assert ( fabs ( second_instance->zero - 2.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( second_instance->minusZero - 2.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( second_instance->two - 2.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( second_instance->pi - 2.0 ) < FLOAT_EPSILON );
    ck_assert ( fabs ( second_instance->NaN - 2.0 ) < FLOAT_EPSILON );

} END_TEST

int main () {

    /*
    printf ( "1.\n" );
    const float FLOAT_EPSILON = 0.0001;
    skill_state *state = skill_state_empty_skill_state ();
    printf ( "2.\n" );

    floattest_access *float_access = state->floattest_access;
    doubletest_access *double_access = state->doubletest_access;

    printf ( "3.\n" );
    floattest_access_create_instance ( float_access, -0.1, 0.0, 0.1, 1.0, 99.99 );
    floattest_access_create_instance ( float_access, 2.0, 2.0, 2.0, 2.0, 2.0 );

    printf ( "4.\n" );
    skill_state_write_to_file ( state, "./resources/floats.sf" );

    printf ( "5.\n" );
    state = skill_state_from_file ( "./resources/floats.sf" );
    int number_of_float_instances = floattest_access_get_number_of_instances ( state->floattest_access );
    ck_assert_msg ( number_of_float_instances == 2, "Expected exactly 2 instances" );

    printf ( "6.\n" );
    FloatTest *instances = floattest_access_get_instances ( state->floattest_access );

    printf ( "7.\n" );
    FloatTest *first_instance = &instances[0];
    FloatTest *second_instance = &instances[1];




*/


    Suite *test_suite = suite_create ( "test suite" );
    TCase *testcase_1 = tcase_create ( "check the values of floattest instances, which have been written to a file and read in again\n" );
    SRunner *test_runner = srunner_create ( test_suite );
    int number_of_tests_failed;

    suite_add_tcase ( test_suite, testcase_1);
    tcase_add_test ( testcase_1, empty_test );
    tcase_add_test ( testcase_1, test_read_write_float_instances );

    srunner_run_all ( test_runner, CK_VERBOSE );
    number_of_tests_failed = srunner_ntests_failed ( test_runner );
    srunner_free ( test_runner );

    printf ( "Number of failed tests: %d\n", number_of_tests_failed );

    return number_of_tests_failed == 0 ? 0 : 1;
}
