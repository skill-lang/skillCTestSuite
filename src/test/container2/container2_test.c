#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <stdbool.h>
#include "../../minunit.h"
#include "api.h"

int tests_run = 0;

/*
This tests container types filled with user-defined types. The specification:
 *
Container {
  Type1[3] arr;
  Type2[] varr;
  list<Type3> l;
  set<Type3> s;
  map<Type1, Type2, Type3> f;
}

Type1{
}

Type2{
}

Type3{
}
 */

char *write_read_instances () {

    skill_state state = empty_skill_state ();

    type1 t1a = create_type1 ( state );
    type1 t1b = create_type1 ( state );

    type2 t2a = create_type2 ( state );
    type2 t2b = create_type2 ( state );

    type3 t3a = create_type3 ( state );
    type3 t3b = create_type3 ( state );

    GArray *constArr = g_array_new ( true, true, sizeof (type1) );
    g_array_append_val ( constArr, t1a );
    g_array_append_val ( constArr, t1b );
    g_array_append_val ( constArr, t1a );

    GArray *varArr = g_array_new ( true, true, sizeof (type2) );
    g_array_append_val ( varArr, t2a );
    g_array_append_val ( varArr, t2a );
    g_array_append_val ( varArr, t2b );

    GList *list = 0;
    list = g_list_append ( list, t3b );
    list = g_list_append ( list, t3b );
    list = g_list_append ( list, t3a );

    GHashTable *set = g_hash_table_new ( g_direct_hash, g_direct_equal );
    g_hash_table_insert ( set, t3b, t3b );
    g_hash_table_insert ( set, t3a, t3a );

    // Build a map like this:
    // "t1a" -> { ( t2a -> t3b ), ( t2b -> t3a ) }
    // "t1b" -> { ( t2b -> t3b ) }
    GHashTable *map = g_hash_table_new ( g_direct_hash, g_direct_equal );
    GHashTable *test1_table = g_hash_table_new ( g_direct_hash, g_direct_equal );
    GHashTable *test2_table = g_hash_table_new ( g_direct_hash, g_direct_equal );

    g_hash_table_insert ( test1_table, t2a, t3b );
    g_hash_table_insert ( test1_table, t2b, t3a );
    g_hash_table_insert ( test2_table, t2b, t3b );
    g_hash_table_insert ( map, t2a, test1_table );
    g_hash_table_insert ( map, t2b, test2_table );

    create_container ( state, constArr, varArr, list, set, map );

    write_to_file ( state, "./resources/container2.sf" );
    delete_skill_state ( state );
    g_array_free ( constArr, true );
    g_array_free ( varArr, true );
    g_list_free ( list );
    g_hash_table_destroy ( set );
    g_hash_table_destroy ( test1_table );
    g_hash_table_destroy ( test2_table );
    g_hash_table_destroy ( map );

    state = skill_state_from_file ( "./resources/container2.sf" );

    GList *instances = get_container_instances ( state );
    mu_assert ( "TEST FAILED: container_test - read_write_instances: Expected exactly 1 container instance.\n", g_list_length ( instances ) == 1 );

    container instance = (container) g_list_nth_data ( instances, 0 );

    GList *type1_instances = get_type1_instances ( state );
    t1a = (type1) g_list_nth_data ( type1_instances, 0 );
    t1b = (type1) g_list_nth_data ( type1_instances, 1 );

    GList *type2_instances = get_type2_instances ( state );
    t2a = (type2) g_list_nth_data ( type2_instances, 0 );
    t2b = (type2) g_list_nth_data ( type2_instances, 1 );

    GList *type3_instances = get_type3_instances ( state );
    t3a = (type3) g_list_nth_data ( type3_instances, 0 );
    t3b = (type3) g_list_nth_data ( type3_instances, 1 );

    constArr = container_get_arr ( instance );
    mu_assert ( "TEST FAILED: container2_test - Expected arr to have exactly 3 elements.\n", constArr->len == 3 );
    mu_assert ( "TEST FAILED: container2_test - Expected first element of arr to be 't1a'.\n", g_array_index ( constArr, type1, 0 ) == t1a );
    mu_assert ( "TEST FAILED: container2_test - Expected second element of arr to be 't1b'.\n", g_array_index ( constArr, type1, 1 ) == t1b );
    mu_assert ( "TEST FAILED: container2_test - Expected third element of arr to be 't1a'.\n", g_array_index ( constArr, type1, 2 ) == t1a );

    varArr = container_get_varr ( instance );
    mu_assert ( "TEST FAILED: container2_test - Expected varr to have exactly 3 elements.\n", varArr->len == 3 );
    mu_assert ( "TEST FAILED: container2_test - Expected first element of varr to be 't2a'.\n", g_array_index ( varArr, type2, 0 ) == t2a );
    mu_assert ( "TEST FAILED: container2_test - Expected second element of varr to be 't2a'.\n", g_array_index ( varArr, type2, 1 ) == t2a );
    mu_assert ( "TEST FAILED: container2_test - Expected third element of varr to be 't2b'.\n", g_array_index ( varArr, type2, 2 ) == t2b );

    list = container_get_l ( instance );
    mu_assert ( "TEST FAILED: container2_test - Expected list 'l' to have exactly 3 elements.\n", g_list_length ( list ) == 3 );
    mu_assert ( "TEST FAILED: container2_test - Expected first element of list 'l' to be 't3b'.\n", (type3) g_list_nth_data ( list, 0 ) == t3b );
    mu_assert ( "TEST FAILED: container2_test - Expected second element of list 'l' to be 't3b'.\n", (type3) g_list_nth_data ( list, 1 ) == t3b );
    mu_assert ( "TEST FAILED: container2_test - Expected third element of list 'l' to be 't3a'.\n", (type3) g_list_nth_data ( list, 2 ) == t3a );

    set = container_get_s ( instance );
    mu_assert ( "TEST FAILED: container2_test - Expected the set 's' to have exactly 2 elements.\n", g_hash_table_size ( set ) == 2 );
    mu_assert ( "TEST FAILED: container2_test - Expected the set 's' to contain 't3b'.\n", g_hash_table_contains ( set, t3b ) );
    mu_assert ( "TEST FAILED: container1_test - Expected the set 's' to contain 't3a'.\n", g_hash_table_contains ( set, t3a ) );

    map = container_get_f ( instance );
    mu_assert ( "TEST FAILED: container2_test - Expected the map 'f' to have exactly 2 elements.\n", g_hash_table_size ( map ) == 2 );
    test1_table = (GHashTable*) g_hash_table_lookup ( map, t1a );
    mu_assert ( "TEST FAILED: Expected an entry for key 'test1' in the map 'f'.\n", test1_table != 0 );
    test2_table = (GHashTable*) g_hash_table_lookup ( map, t1b );
    mu_assert ( "TEST FAILED: Expected an entry for key 'test2' in the map 'f'.\n", test2_table != 0 );
    mu_assert ( "TEST FAILED: container1_test - Expected the first nested map of map 'f' to have exactly 2 elements.\n", g_hash_table_size ( test1_table ) == 2 );
    mu_assert ( "TEST FAILED: container1_test - Expected the second nested map of map 'f' to have exactly 1 element.\n", g_hash_table_size ( test2_table ) == 1 );

    mu_assert ( "TEST FAILED: Expected the first nested map to contain 't3b' for the key 't2a'.\n", (type3) g_hash_table_lookup ( test1_table, t2a ) == t3b );
    mu_assert ( "TEST FAILED: Expected the first nested map to contain 't3a' for the key 't2b'.\n", (type3) g_hash_table_lookup ( test1_table, t2b ) == t3a );
    mu_assert ( "TEST FAILED: Expected the second nested map to contain 't3b' for the key 't2b'.\n", (type3) g_hash_table_lookup ( test2_table, t2b ) == t3b );

    delete_skill_state ( state );
    g_array_free ( constArr, true );
    g_array_free ( varArr, true );
    g_list_free ( list );
    g_hash_table_destroy ( set );
    g_hash_table_destroy ( test1_table );
    g_hash_table_destroy ( test2_table );
    g_hash_table_destroy ( map );

    g_list_free ( instances );
    g_list_free ( type1_instances );
    g_list_free ( type2_instances );
    g_list_free ( type3_instances );
    return 0;
}

static char *all_tests () {
     mu_run_test ( write_read_instances );
     return 0;
}

int main () {
    char *result = all_tests ();
    if ( result != 0 ) {
        printf( "%s\n", result );
        return 1;
    } else {
        printf ( "container_test: all tests passed.\n" );
        return 0;
    }
}
