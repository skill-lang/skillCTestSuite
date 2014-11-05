#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <glib.h>
#include "minunit.h"
#include "api.h"

int tests_run = 0;

// This test writes annotations, then appends annotations then reads all and checks the result.
// The specification looks like this:
// Date { v64 date; }
// Test { annotation f; }
char *write_append_read_instances () {

    skill_state state = empty_skill_state ();
    date d = create_date ( state, 1 );
    test t = create_test ( state, (skill_type) d );
    write_to_file ( state, "./resources/annotation.sf" );
    delete_skill_state ( state );

    state = skill_state_from_file ( "./resources/annotation.sf" );
    GList *test_instances = get_test_instances ( state );
    t = (test) g_list_nth_data ( test_instances, 0 );
    create_test ( state, (skill_type) t );
    append_to_file ( state );
    delete_skill_state ( state );
    g_list_free ( test_instances );

    state = skill_state_from_file ( "./resources/annotation.sf" );

    GList *date_instances = get_date_instances ( state );
    test_instances = get_test_instances ( state );

    mu_assert ( "TEST FAILED: annotation_test - Expected exactly 1 date instance.\n", g_list_length ( date_instances ) == 1 );
    mu_assert ( "TEST FAILED: annotation_test - Expected exactly 2 instances of test b.\n", g_list_length ( test_instances ) == 2 );

    d = (date) g_list_nth_data ( date_instances, 0 );

    t = (test) g_list_nth_data ( test_instances, 0 );
    test t2  = (test) g_list_nth_data ( test_instances, 1 );


    mu_assert ( "TEST FAILED: annotation_test - Expected d->date to be 1.\n", date_get_date ( d ) == 1 );

    mu_assert ( "TEST FAILED: annotation_test - Expected t->f to be of type 'date'.\n", instanceof_date ( test_get_f ( t ) ) == true );
    mu_assert ( "TEST FAILED: annotation_test - Expected t->f == d.\n", test_get_f ( t ) == (skill_type) d );

    mu_assert ( "TEST FAILED: annotation_test - Expected t2->f to be of type 'test'.\n", instanceof_test ( test_get_f ( t2 ) ) == true );
    mu_assert ( "TEST FAILED: annotation_test - Expected t2->f == t.\n", test_get_f ( t2 ) == (skill_type) t );

    g_list_free ( date_instances );
    g_list_free ( test_instances );

    delete_skill_state ( state );
    return 0;
}

// This test just reads a bunch of binary files to check, that the parsing does not cause errors.
char *generic_parse_test () {
    skill_state state = skill_state_from_file ( "./resources/working/coloredNodes.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/date.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/emptyBlocks.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/fourColoredNodes.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/localBasePoolStartIndex.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/node.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/noFieldRegressionTest.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/trivialType.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/twoNodeBlocks.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/twoTypes.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/working/unicode-reference.sf" );
    delete_skill_state ( state );
    return 0;
}

static char *all_tests () {
    mu_run_test ( generic_parse_test );
    mu_run_test ( write_append_read_instances );
    return 0;
}

int main () {
    char *result = all_tests ();
    if ( result != 0 ) {
        printf( "%s\n", result );
        return 1;
    } else {
        printf ( "annotation_test: all tests passed.\n" );
        return 0;
    }
}
