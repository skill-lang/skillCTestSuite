#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <glib.h>
#include "minunit.h"
#include "date_api.h"
#include "sub_api.h"

int tests_run = 0;

// This test uses two bindings at once, checking, theat the prefixing of symbols in the binding works,
// and appending to a binary file created by the other binding.
char *write_append_read_instances () {
    date_skill_state state1 = date_empty_skill_state ();
    date_create_date ( state1, 1 );
    date_write_to_file ( state1, "one_date_instance.sf" );
    date_delete_skill_state ( state1 );

    sub_skill_state state2 = sub_skill_state_from_file ( "one_date_instance.sf" ); // should contain one unknown type
    GList *all_instances = sub_get_all_instances ( state2 );

    // TODO check, that the list is empty

    sub_a _a = sub_create_a ( state2, 0 );
    sub_b _b = sub_create_b ( state2, 0, 0 );
    sub_c _c = sub_create_c ( state2, 0, 0 );
    sub_d _d = sub_create_d ( state2, 0, 0, 0 );

    sub_a_set_a ( _a, _a );
    sub_b_set_a ( _b, (sub_a) _b );
    sub_c_set_a ( _c, (sub_a) _c );
    sub_d_set_a ( _d, (sub_a) _d );

    sub_b_set_b ( _b, _b );
    sub_d_set_b ( _d, (sub_b) _b );

    sub_d_set_d ( _d, _d );

    sub_c_set_c ( _c, _c );

    sub_append_to_file ( state2 );
    sub_delete_skill_state ( state2 );

    state1 = date_skill_state_from_file ( "one_date_instance.sf" );
    // The date-binding should only know the one date-instance


    state2 = sub_skill_state_from_file ( "one_date_instance.sf" );
    // The sub-binding should know the for a, b, c, d - instances

    // TODO check instances

    date_delete_skill_state ( state1 );
    sub_delete_skill_state ( state2 );

    return 0;
}

static char *all_tests () {
    mu_run_test ( write_append_read_instances );
    return 0;
}

int main () {
    char *result = all_tests ();
    if ( result != 0 ) {
        printf( "%s\n", result );
        return 1;
    } else {
        printf ( "prefix_test: all tests passed.\n" );
        return 0;
    }
}
