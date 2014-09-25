#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "../../minunit.h"
#include "api/subtypes_api.h"

int tests_run = 0;

// This tests reading and writing instances with inheritance.
// The specification looks like this:
// A { A a; }
// B : A { B b; }
// C : A { C c; }
// D : B { D d; }
// with one instance of each type, named with the lower case letter of that type.
char *read_write_instances () {

    skill_state state = empty_skill_state ();
    a _a = create_a ( state, 0 );
    b b = create_b ( state, 0, 0 );
    c c = create_c ( state, 0, 0 );
    d d = create_d ( state, 0, 0, 0 );

    a_set_a ( _a, _a );

    b_set_a ( b, _a );
    b_set_b ( b, b );

    c_set_a ( c, _a );
    c_set_c ( c, c );

    d_set_a ( d, _a );
    d_set_b ( d, b );
    d_set_d ( d, d );

    write_to_file ( state, "./resources/subtypes.sf" );
    delete_skill_state ( state );

    state = skill_state_from_file ( "./resources/subtypes.sf" );

    GList *a_instances = get_a_instances ( state );
    GList *b_instances = get_b_instances ( state );
    GList *c_instances = get_c_instances ( state );
    GList *d_instances = get_d_instances ( state );

    mu_assert ( "TEST FAILED: subtypes_test - read_write_instances: Expected exactly 4 instances of type a.\n", g_list_length ( a_instances ) == 4 );
    mu_assert ( "TEST FAILED: subtypes_test - read_write_instances: Expected exactly 2 instances of type b.\n", g_list_length ( b_instances ) == 2 );
    mu_assert ( "TEST FAILED: subtypes_test - read_write_instances: Expected exactly 1 instance of type c.\n", g_list_length ( c_instances ) == 1 );
    mu_assert ( "TEST FAILED: subtypes_test - read_write_instances: Expected exactly 1 instance of type d.\n", g_list_length ( d_instances ) == 1 );

    GList *iterator;
    a instance;
    for ( iterator = a_instances; iterator; iterator = iterator->next ) {
        _a = (a) iterator->data;
        mu_assert ( "TEST FAILED: subtypes_test - read_write_instances: Expected the a-field of all instances to point to the instance itself.\n", a_get_a ( _a ) == _a );
    }

    g_list_free ( a_instances );
    g_list_free ( b_instances );
    g_list_free ( c_instances );
    g_list_free ( d_instances );
    delete_skill_state ( state );
    return 0;
}

static char *all_tests () {
     mu_run_test ( read_write_instances );
     return 0;
}

int main () {
    char *result = all_tests ();
    if ( result != 0 ) {
        printf( "%s\n", result );
        return 1;
    } else {
        printf ( "subtypes_test: all tests passed.\n" );
        return 0;
    }
}
