#include <math.h>
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "api/date_api.h"

START_TEST ( test_read_date_instances ) {
    skill_state state = skill_state_from_file ( "./resources/date.sf" );

    GList *date_instances = get_date_instances ( state );
    ck_assert_msg ( g_list_length ( date_instances ) == 2, "Expected exactly 2 instances" );

    date first_instance = (date) g_list_nth_data ( date_instances, 0 );
    date second_instance = (date) g_list_nth_data ( date_instances, 1 );
    ck_assert ( date_get_date ( first_instance ) == 1 );
    ck_assert ( date_get_date ( second_instance ) == -1 );

} END_TEST

START_TEST ( test_write_10000_instances ) {
    skill_state state = empty_skill_state ();

    int i;
    for ( i = 0; i < 10000; i++ ) {
        create_date ( state, i );
    }
    write_to_file ( state, "./resources/date_10000_instances.sf" );

    state = skill_state_from_file ( "./resources/date_10000_instances.sf" );

    GList *instances = get_date_instances ( state );

} END_TEST

int main () {

    Suite *test_suite = suite_create ( "test suite" );
    TCase *testcase_1 = tcase_create ( "read the instances from the example file and check their values (1 and -1)\n" );
    SRunner *test_runner = srunner_create ( test_suite );
    int number_of_tests_failed;

    suite_add_tcase ( test_suite, testcase_1);
    tcase_add_test ( testcase_1, test_read_date_instances );
 //   tcase_add_test ( testcase_1, test_write_10000_instances );

    srunner_run_all ( test_runner, CK_VERBOSE );
    number_of_tests_failed = srunner_ntests_failed ( test_runner );
    srunner_free ( test_runner );

    printf ( "Number of failed tests: %d\n", number_of_tests_failed );

    return number_of_tests_failed == 0 ? 0 : 1;
    return 0;
}
