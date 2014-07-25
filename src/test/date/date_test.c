#include <math.h>
#include <check.h>
#include <stdlib.h>
#include "access/date_skill_state.h"
#include "access/date_date_access.h"
#include "types/date_types.h"

START_TEST ( test_read_date_instances ) {
    skill_state *state = skill_state_from_file ( "./resources/date.sf" );
    date_access *date_access = state->date_access;

    int number_of_instances = date_access_get_number_of_instances ( date_access );
    ck_assert_msg ( number_of_instances == 2, "Expected exactly 2 instances" );

    Date **instances = date_access_get_instances ( date_access );

    ck_assert ( instances[0]->date == 1 );
    ck_assert ( instances[1]->date == -1 );

} END_TEST

START_TEST ( test_write_1000_instances ) {
    skill_state *state = skill_state_empty_skill_state ();
    date_access *date_access = state->date_access;

    int i;
    for ( i = 0; i < 10; i++ ) {
        date_access_create_instance ( date_access, i );
    }
    skill_state_write_to_file ( state, "./resources/date_1000_instances.sf" );

    state = skill_state_from_file ( "./resources/date_1000_instances.sf" );
    int number_of_instances = date_access_get_number_of_instances ( state->date_access );

    Date **instances = date_access_get_instances ( state->date_access );

} END_TEST

int main () {

    Suite *test_suite = suite_create ( "test suite" );
    TCase *testcase_1 = tcase_create ( "read the instances from the example file and check their values (1 and -1)\n" );
    SRunner *test_runner = srunner_create ( test_suite );
    int number_of_tests_failed;

    suite_add_tcase ( test_suite, testcase_1);
    tcase_add_test ( testcase_1, test_read_date_instances );
    tcase_add_test ( testcase_1, test_write_1000_instances );

    srunner_run_all ( test_runner, CK_VERBOSE );
    number_of_tests_failed = srunner_ntests_failed ( test_runner );
    srunner_free ( test_runner );

    printf ( "Number of failed tests: %d\n", number_of_tests_failed );

    return number_of_tests_failed == 0 ? 0 : 1;
}
