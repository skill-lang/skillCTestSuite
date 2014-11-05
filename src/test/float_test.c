#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "minunit.h"
#include "api.h"

int tests_run = 0;

// This tests reading and writing float values.
char *read_write_float_instances () {
    const float FLOAT_EPSILON = 0.000001;
    skill_state state = empty_skill_state ();

    create_floattest ( state, 0.0, 1 / -INFINITY, 2.0, 3.1415926536, NAN );

    write_to_file ( state, "./resources/floats.sf" );

    state = skill_state_from_file ( "./resources/floats.sf" );
    GList *floattest_instances = get_floattest_instances ( state );
    mu_assert ( "TEST FAILED: float_test - read_write_float_instances: Expected exactly 1 instance\n",
            g_list_length ( floattest_instances ) == 1 );

    floattest instance = g_list_nth_data ( floattest_instances, 0 );

    mu_assert ( "TEST FAILED: float_test - read_write_float_instances: Zero-value not set correctly.\n",
            fabs ( floattest_get_zero ( instance ) - ( -0.0) ) < FLOAT_EPSILON );
    mu_assert ( "TEST FAILED: float_test - read_write_float_instances: Minus-zero value not set correctly.\n",
            fabs ( floattest_get_minuszero ( instance ) - ( 1 / -INFINITY ) ) < FLOAT_EPSILON );
    mu_assert ( "TEST FAILED: float_test - read_write_float_instances: Two-value not set correctly.\n",
            fabs ( floattest_get_two ( instance ) - 2.0 ) < FLOAT_EPSILON );
    mu_assert ( "TEST FAILED: float_test - read_write_float_instances: Pi value not set correctly.\n",
            fabs ( floattest_get_pi ( instance ) - 3.141592653 ) < FLOAT_EPSILON );
    // Nan is the only number where f != f is true (for a float f)
    mu_assert ( "TEST FAILED: float_test - read_write_float_instances: NAN value not set correctly.\n",
            floattest_get_nan ( instance ) != floattest_get_nan ( instance ) );
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
    mu_run_test ( read_write_float_instances );
    return 0;
}

int main () {
    char *result = all_tests ();
    if ( result != 0 ) {
        printf( "%s\n", result );
        return 1;
    } else {
        printf ( "float_test: all tests passed.\n" );
        return 0;
    }
}
