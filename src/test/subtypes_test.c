#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <glib.h>
#include "minunit.h"
#include "api.h"

int tests_run = 0;

// This tests reading and writing instances with inheritance.
// The specification looks like this:
// A { A a; }
// B : A { B b; }
// C : A { C c; }
// D : B { D d; }
// with one instance of each type, named with the lower case letter of that type.
char *write_and_read_instances () {

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
    delete_skill_state ( state );

    state = skill_state_from_file ( "./resources/subtypes.sf" );

    GList *a_instances = get_a_instances ( state );
    GList *b_instances = get_b_instances ( state );
    GList *c_instances = get_c_instances ( state );
    GList *d_instances = get_d_instances ( state );
    GList *all_instances = get_all_instances ( state );

    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 4 instances total.\n", g_list_length ( all_instances ) == 4 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 4 instances of type a.\n", g_list_length ( a_instances ) == 4 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 2 instances of type b.\n", g_list_length ( b_instances ) == 2 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 1 instance of type c.\n", g_list_length ( c_instances ) == 1 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 1 instance of type d.\n", g_list_length ( d_instances ) == 1 );

    _d = (d) g_list_nth_data ( d_instances, 0 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_d (_d) to return 'true'.\n", instanceof_d ( (skill_type) _d ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_a (_d) to return 'true'.\n", instanceof_a ( (skill_type) _d ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_b (_d) to return 'true'.\n", instanceof_b ( (skill_type) _d ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_c (_d) to return 'false'.\n", instanceof_c ( (skill_type) _d ) == false );

    _c = (c) g_list_nth_data ( c_instances, 0 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_c (_c) to return 'true'.\n", instanceof_c ( (skill_type) _c ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_a (_c) to return 'true'.\n", instanceof_a ( (skill_type) _c ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_b (_c) to return 'false'.\n", instanceof_b ( (skill_type) _c ) == false );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_d (_c) to return 'false'.\n", instanceof_d ( (skill_type) _c ) == false );

    if ( instanceof_b ( (skill_type) g_list_nth_data ( b_instances, 0 ) ) ) {
        _b = (b) g_list_nth_data ( b_instances, 0 );
    } else {
        _b = (b) g_list_nth_data ( b_instances, 1 );
    }

    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_b (_b) to return 'true'.\n", instanceof_b ( (skill_type) _b ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_a (_b) to return 'true'.\n", instanceof_a ( (skill_type) _b ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_c (_b) to return 'false'.\n", instanceof_c ( (skill_type) _b ) == false );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_d (_b) to return 'false'.\n", instanceof_d ( (skill_type) _b ) == false );

    GList *iterator;
    skill_type t;
    for ( iterator = a_instances; iterator; iterator = iterator->next ) {
        t = (skill_type) iterator->data;
        if ( !instanceof_b ( t ) && !instanceof_d ( t ) && !instanceof_c ( t ) ) {
            _a = (a) t;
        }
    }
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_a (_a) to return 'true'.\n", instanceof_a ( (skill_type) _a ) == true );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_b (_a) to return 'false'.\n", instanceof_b ( (skill_type) _a ) == false );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_c (_a) to return 'false'.\n", instanceof_c ( (skill_type) _a ) == false );
    mu_assert ( "TEST FAILED: subtypes_test - Expected instanceof_d (_a) to return 'false'.\n", instanceof_d ( (skill_type) _a ) == false );

    mu_assert ( "TEST FAILED: subtypes_test - Expected _a->a to point to _a.\n", a_get_a ( _a ) == _a );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _b->a to point to _a.\n", a_get_a ( (a) _b ) == _a );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _b->a to point to _a.\n", b_get_a ( _b ) == _a );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _c->a to point to _a.\n", a_get_a ( (a) _c ) == _a );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _c->a to point to _a.\n", c_get_a ( _c ) == _a );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->a to point to _a.\n", a_get_a ( (a) _d ) == _a );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->a to point to _a.\n", b_get_a ( (b) _d ) == _a );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->a to point to _a.\n", d_get_a ( _d ) == _a );

    mu_assert ( "TEST FAILED: subtypes_test - Expected _b->b to point to _b.\n", b_get_b ( _b ) == _b );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->b to point to _b.\n", b_get_b ( (b) _d ) == _b );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->b to point to _b.\n", d_get_b ( _d ) == _b );

    mu_assert ( "TEST FAILED: subtypes_test - Expected _c->c to point to _c.\n", c_get_c ( _c ) == _c );

    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->d to point to _d.\n", d_get_d ( _d ) == _d );

    g_list_free ( all_instances );
    g_list_free ( a_instances );
    g_list_free ( b_instances );
    g_list_free ( c_instances );
    g_list_free ( d_instances );
    delete_skill_state ( state );
    return 0;
}

char *read_example_instances () {
    // This reads instances spread over 3 blocks. (see localBasePoolStartIndex.sf.txt)
    skill_state state = skill_state_from_file ( "./resources/localBasePoolStartIndex.sf" );

    GList *a_instances = get_a_instances ( state );
    GList *b_instances = get_b_instances ( state );
    GList *c_instances = get_c_instances ( state );
    GList *d_instances = get_d_instances ( state );

    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 13 instances of type a.\n", g_list_length ( a_instances ) == 13 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 8 instances of type b.\n", g_list_length ( b_instances ) == 8 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 2 instance of type c.\n", g_list_length ( c_instances ) == 2 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 3 instance of type d.\n", g_list_length ( d_instances ) == 3 );

    g_list_free ( a_instances );
    g_list_free ( b_instances );
    g_list_free ( c_instances );
    g_list_free ( d_instances );

    delete_skill_state ( state );
    return 0;
}

char *read_instances_from_two_blocks () {
    // This file contains an A-Instance in the first block, and a D-Instance in the second block.
    // All references are set to self-references.
    skill_state state = skill_state_from_file ( "./resources/inheritance_two_blocks.sf" );

    GList *a_instances = get_a_instances ( state );
    GList *b_instances = get_b_instances ( state );
    GList *c_instances = get_c_instances ( state );
    GList *d_instances = get_d_instances ( state );

    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 2 instances of type a.\n", g_list_length ( a_instances ) == 2 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 1 instances of type b.\n", g_list_length ( b_instances ) == 1 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 0 instance of type c.\n", g_list_length ( c_instances ) == 0 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 1 instance of type d.\n", g_list_length ( d_instances ) == 1 );

    GList *iterator;
    skill_type t;
    a _a;
    for ( iterator = a_instances; iterator; iterator = iterator->next ) {
        t = (skill_type) iterator->data;
        if ( !instanceof_b ( t ) && !instanceof_d ( t ) && !instanceof_c ( t ) ) {
            _a = (a) t;
        }
    }

    d _d = (d) g_list_nth_data ( d_instances, 0 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _a->a to point to _a.\n", a_get_a ( _a ) == _a );

    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->a to point to _d.\n", d_get_a ( _d ) == (a) _d );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->b to point to _d.\n", d_get_b ( _d ) == (b) _d );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->d to point to _d.\n", d_get_d ( _d ) == _d );


    g_list_free ( a_instances );
    g_list_free ( b_instances );
    g_list_free ( c_instances );
    g_list_free ( d_instances );

    delete_skill_state ( state );
    return 0;
}

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

    write_to_file ( state, "./resources/appending.sf" );
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

    state = skill_state_from_file ( "./resources/appending.sf" );

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

// This test checks, that the serialization mechanism can handle deleted instances
// And that they are not written to file.
// (internally they are not deleted directly, but their id is set to 0.)
char *create_delete_append () {

    skill_state state = empty_skill_state ();
    a _a = create_a ( state, 0 );
    b _b = create_b ( state, 0, 0 );
    c _c = create_c ( state, 0, 0 );
    d _d = create_d ( state, 0, 0, 0 );

    a_set_a ( _a, _a );

    b_set_a ( _b, (a) _b );
    b_set_b ( _b, _b );

    c_set_a ( _c, (a) _c );
    c_set_c ( _c, _c );

    d_set_a ( _d, (a) _d );
    d_set_b ( _d, (b) _d );
    d_set_d ( _d, _d );

    delete_a ( _a );
    delete_b ( _b );
    delete_c ( _c );

    write_to_file ( state, "./resources/appending.sf" );
    _a = create_a ( state, 0 );
    _b = create_b ( state, 0, 0 );
    _c = create_c ( state, 0, 0 );
    _d = create_d ( state, 0, 0, 0 );

    a_set_a ( _a, _a );

    b_set_a ( _b, (a) _b );
    b_set_b ( _b, _b );

    c_set_a ( _c, (a) _c );
    c_set_c ( _c, _c );

    d_set_a ( _d, (a) _d );
    d_set_b ( _d, (b) _d );
    d_set_d ( _d, _d );

    delete_b ( _b );
    delete_c ( _c );
    delete_d ( _d );

    append_to_file ( state );
    append_to_file ( state );
    delete_skill_state ( state );

    state = skill_state_from_file ( "./resources/appending.sf" );
    GList *a_instances = get_a_instances ( state );
    GList *b_instances = get_b_instances ( state );
    GList *c_instances = get_c_instances ( state );
    GList *d_instances = get_d_instances ( state );

    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 2 instances of type a.\n", g_list_length ( a_instances ) == 2 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 1 instances of type b.\n", g_list_length ( b_instances ) == 1 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 0 instance of type c.\n", g_list_length ( c_instances ) == 0 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected exactly 1 instance of type d.\n", g_list_length ( d_instances ) == 1 );

    _d = (d) g_list_nth_data ( d_instances, 0 );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->d to point to _d.\n", d_get_d ( _d ) == _d );

    _a = (a) g_list_nth_data ( a_instances, 0 );
    if ( instanceof_d ( (skill_type) _a ) ) {
        _a = (a) g_list_nth_data ( a_instances, 1 );
    }
    mu_assert ( "TEST FAILED: subtypes_test - Expected _d->a to point to _d.\n", d_get_a ( _d ) == (a) _d );
    mu_assert ( "TEST FAILED: subtypes_test - Expected _a->a to point to _a.\n", a_get_a ( _a ) == _a );

    g_list_free ( a_instances );
    g_list_free ( b_instances );
    g_list_free ( c_instances );
    g_list_free ( d_instances );
    delete_skill_state ( state );
    return 0;
}

static char *all_tests () {
    mu_run_test ( write_and_read_instances );
    mu_run_test ( read_example_instances );
    mu_run_test ( read_instances_from_two_blocks );
    mu_run_test ( create_delete_append );
    mu_run_test ( write_append_read_instances );
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
