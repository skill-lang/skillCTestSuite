#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <glib.h>
#include "../../minunit.h"
#include "api.h"

int tests_run = 0;

// This tests writing instances, then appending more instances, so there are several type blocks.
// And then reading all instances.
// The specification looks like this:
// A { A a; }
// B : A { B b; }
// C : A { C c; }
// D : B { D d; }
// with one instance of each type, named with the lower case letter of that type.
char *write_append_read_instances () {

    skill_state state = empty_skill_state ();
    a _a = create_a ( state, 0 );
    b _b = create_b ( state, 0, 0 );
    c _c = create_c ( state, 0, 0 );
    d _d = create_d ( state, 0, 0, 0 );

    a_set_a ( _a, _a );

    b_set_a ( _b, _a );
    b_set_b ( _b, _b );

    c_set_a ( _c, _a );
    c_set_c ( _c, _c );

    d_set_a ( _d, _a );
    d_set_b ( _d, _b );
    d_set_d ( _d, _d );

    write_to_file ( state, "./resources/subtypes.sf" );
    _a = create_a ( state, 0 );
    _b = create_b ( state, 0, 0 );
    _c = create_c ( state, 0, 0 );
    _d = create_d ( state, 0, 0, 0 );

    a_set_a ( _a, _a );

    b_set_a ( _b, _a );
    b_set_b ( _b, _b );

    c_set_a ( _c, _a );
    c_set_c ( _c, _c );

    d_set_a ( _d, _a );
    d_set_b ( _d, _b );
    d_set_d ( _d, _d );
    append_to_file ( state );

    delete_skill_state ( state );

    state = skill_state_from_file ( "./resources/subtypes.sf" );

    GList *a_instances = get_a_instances ( state );
    GList *b_instances = get_b_instances ( state );
    GList *c_instances = get_c_instances ( state );
    GList *d_instances = get_d_instances ( state );

    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 8 instances of type a.\n", g_list_length ( a_instances ) == 8 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 4 instances of type b.\n", g_list_length ( b_instances ) == 4 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 2 instance of type c.\n", g_list_length ( c_instances ) == 2 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 2 instance of type d.\n", g_list_length ( d_instances ) == 2 );

    _d = (d) g_list_nth_data ( d_instances, 0 );
    d _d2 = g_list_nth_data ( d_instances, 1 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_d (_d) to return 'true'.\n", instanceof_d ( (skill_type) _d ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_a (_d) to return 'true'.\n", instanceof_a ( (skill_type) _d ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_b (_d) to return 'true'.\n", instanceof_b ( (skill_type) _d ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_c (_d) to return 'false'.\n", instanceof_c ( (skill_type) _d ) == false );

    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_d (_d2) to return 'true'.\n", instanceof_d ( (skill_type) _d2 ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_a (_d2) to return 'true'.\n", instanceof_a ( (skill_type) _d2 ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_b (_d2) to return 'true'.\n", instanceof_b ( (skill_type) _d2 ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_c (_d2) to return 'false'.\n", instanceof_c ( (skill_type) _d2 ) == false );

    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->d to point to _d.\n", d_get_d ( _d ) == _d );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d2->d to point to _d2.\n", d_get_d ( _d2 ) == _d2 );

    _c = (c) g_list_nth_data ( c_instances, 0 );
    c _c2 = (c) g_list_nth_data ( c_instances, 1 );

    mu_assert ( "TEST FAILED: subtypes_test - Expected _c->c to point to _c.\n", c_get_c ( _c ) == _c );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _c2->c to point to _c2.\n", c_get_c ( _c2 ) == _c2 );

    g_list_free ( a_instances );
    g_list_free ( b_instances );
    g_list_free ( c_instances );
    g_list_free ( d_instances );
    delete_skill_state ( state );
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
        printf ( "appending_test: all tests passed.\n" );
        return 0;
    }
}
