#include <math.h>
#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "api/node_api.h"

START_TEST ( read_node_instances ) {
    printf ( "started test.\n" );
    mark_point ();
    skill_state state = skill_state_from_file ( "./resources/node.sf" );

    GList *nodes = get_node_instances ( state );
    node first = (node) g_list_nth ( nodes, 0 );
    node second = (node) g_list_nth ( nodes, 1 );

    ck_assert ( node_get_node ( first ) == second );
    ck_assert ( node_get_node ( second ) == first );
} END_TEST

int main () {

    Suite *test_suite = suite_create ( "nodes test suite" );
    TCase *testcase_1 = tcase_create ( "read the two instances and verify that they point to each other\n" );
    SRunner *test_runner = srunner_create ( test_suite );
    int number_of_tests_failed;

    suite_add_tcase ( test_suite, testcase_1);
    tcase_add_test ( testcase_1, read_node_instances );

    srunner_run_all ( test_runner, CK_VERBOSE );
    number_of_tests_failed = srunner_ntests_failed ( test_runner );
    srunner_free ( test_runner );

    printf ( "Number of failed tests: %d\n", number_of_tests_failed );

    return number_of_tests_failed == 0 ? 0 : 1;
}
