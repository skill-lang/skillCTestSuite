#include <math.h>
#include <check.h>
#include <stdlib.h>
#include "access/node_skill_state.h"
#include "access/node_node_access.h"
#include "types/node_types.h"

START_TEST ( read_node_instances ) {
    printf ( "started test.\n" );
    mark_point ();
    skill_state *state = skill_state_from_file ( "./resources/node.sf" );

    node_access *node_access = state->node_access;

    int number_of_instances = node_access_get_number_of_instances ( node_access );
    Node** nodes = node_access_get_instances ( node_access );

    ck_assert ( nodes[0]->node == nodes[1] );
    ck_assert ( nodes[1]->node == nodes[0] );
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
