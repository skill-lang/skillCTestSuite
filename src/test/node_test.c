#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "minunit.h"
#include "api.h"

int tests_run = 0;

// This verifies reading of types with references to user-defined types.
// In the example, there are two nodes, with references pointing to each other.
char *read_node_instances () {
    skill_state state = skill_state_from_file ( "./resources/node.sf" );

    GList *nodes = get_node_instances ( state );
    node first = (node) g_list_nth_data ( nodes, 0 );
    node second = (node) g_list_nth_data ( nodes, 1 );

    mu_assert ( "TEST FAILED: node_test - read_node_instances: Expected the first node to point to the second.\n", node_get_node ( first ) == second );
    mu_assert ( "TEST FAILED: node_test - read_node_instances: Expected the second node to point to the first.\n", node_get_node ( second ) == first );

    g_list_free ( nodes );
    delete_skill_state ( state );
    return 0;
}

// This writes two node_instances before reading them again to verify, that the serialization of references to user-defined-types works aswell.
// This will create 3 instances a, b, c, with
//  a->node == b,
//  b->node == c,
//  c->node == a
char *read_write_node_instances () {
    skill_state state = empty_skill_state ();
    node a = create_node ( state, 0 );
    node b = create_node ( state, 0 );
    node c = create_node ( state, 0 );

    node_set_node ( a, b );
    node_set_node ( b, c );
    node_set_node ( c, a );

    write_to_file ( state, "./resources/node_2.sf" );
    delete_skill_state ( state );

    state = skill_state_from_file ( "./resources/node_2.sf" );

    GList *nodes = get_node_instances ( state );
    a = (node) g_list_nth_data ( nodes, 0 );
    b = (node) g_list_nth_data ( nodes, 1 );
    c = (node) g_list_nth_data ( nodes, 2 );

    mu_assert ( "TEST FAILED: node_test - read_node_instances: Expected node a to point to b.\n", node_get_node ( a ) == b );
    mu_assert ( "TEST FAILED: node_test - read_node_instances: Expected node b to point to c.\n", node_get_node ( b ) == c );
    mu_assert ( "TEST FAILED: node_test - read_node_instances: Expected node c to point to a.\n", node_get_node ( c ) == a );

    g_list_free ( nodes );
    delete_skill_state ( state );
    return 0;
}

static char *all_tests () {
     mu_run_test ( read_node_instances );
     mu_run_test ( read_write_node_instances );
     return 0;
}

int main () {
    char *result = all_tests ();
    if ( result != 0 ) {
        printf( "%s\n", result );
        return 1;
    } else {
        printf ( "node_test: all tests passed.\n" );
        return 0;
    }
}
