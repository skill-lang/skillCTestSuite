#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <stdbool.h>
#include "minunit.h"
#include "api.h"

int tests_run = 0;
// this writes and reads instances with only constant fields.
char *write_read_instances () {

    skill_state state = empty_skill_state ();

    create_autofield ( state, 3 );

    write_to_file ( state, "./resources/auto.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/auto.sf" );

    GList *instances = get_autofield_instances ( state );

    mu_assert ( "TEST FAILED: auto_test - there should be 1 instance.\n", g_list_length ( instances ) == 1 );
    autofield instance = (autofield) g_list_nth_data ( instances, 0 );

    mu_assert ( "TEST FAILED: auto_test - this field should be set to 0.\n", autofield_get_test ( instance ) == 0 );

    g_list_free ( instances );
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
    mu_run_test ( write_read_instances );
    return 0;
}

int main () {
    char *result = all_tests ();
    if ( result != 0 ) {
        printf( "%s\n", result );
        return 1;
    } else {
        printf ( "auto_test: all tests passed.\n" );
        return 0;
    }
}
