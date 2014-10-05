#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <stdbool.h>
#include "../../minunit.h"
#include "api.h"

int tests_run = 0;

//  This test uses all container types. The specification:
// Container {
//   v64[3] arr;
//   string[] varr;
//   list<bool> l;
//   set<i64> s;
//   map<string, v64, bool> f;
// }

char *write_read_instances () {

    skill_state state = empty_skill_state ();

    GArray *constArr = g_array_new ( true, true, sizeof (int64_t) );
    int64_t k1 = 1;
    int64_t k2 = -1;
    int64_t k3 = 2;
    g_array_append_val ( constArr, k1 );
    g_array_append_val ( constArr, k2 );
    g_array_append_val ( constArr, k3 );


    GArray *varArr = g_array_new ( true, true, sizeof (char*) );
    char *val1 = "val1";
    char *val2 = "val2";
    g_array_append_val ( varArr, val1 );
    g_array_append_val ( varArr, val2 );

    GList *list = 0;
    bool b1 = true;
    bool b2 = true;
    bool b3 = false;
    list = g_list_append ( list, &b1 );
    list = g_list_append ( list, &b2 );
    list = g_list_append ( list, &b3 );

    GHashTable *set = g_hash_table_new ( g_int_hash, g_int_equal );
    g_hash_table_insert ( set, &k1, &k1 );
    g_hash_table_insert ( set, &k2, &k2 );
    g_hash_table_insert ( set, &k3, &k3 );

    // Build a map like this:
    // "test1" -> { ( 1 -> true ), ( -1 -> false ) }
    // "test2" -> { ( 2 -> true ) }
    GHashTable *map = g_hash_table_new ( g_str_hash, g_str_equal );
    GHashTable *test1_table = g_hash_table_new ( g_int_hash, g_int_equal );
    GHashTable *test2_table = g_hash_table_new ( g_int_hash, g_int_equal );

    g_hash_table_insert ( test1_table, &k1, &b1 );
    g_hash_table_insert ( test1_table, &k2, &b3 );
    g_hash_table_insert ( test2_table, &k3, &b2 );
    g_hash_table_insert ( map, "test1", test1_table );
    g_hash_table_insert ( map, "test2", test2_table );

    container instance = create_container ( state, constArr, varArr, list, set, map );

    write_to_file ( state, "./resources/container1.sf" );
    delete_skill_state ( state );
    g_array_free ( varArr, true );
    g_list_free ( list );
    g_hash_table_destroy ( set );
    g_hash_table_destroy ( test1_table );
    g_hash_table_destroy ( test2_table );
    g_hash_table_destroy ( map );

    state = skill_state_from_file ( "./resources/container1.sf" );

    GList *instances = get_container_instances ( state );
    instance = (container) g_list_nth_data ( instances, 0 );

    mu_assert ( "TEST FAILED: container1_test - read_write_instances: Expected exactly 1 container instance.\n", g_list_length ( instances ) == 1 );

    constArr = container_get_arr ( instance );
    mu_assert ( "TEST FAILED: container1_test - Expected arr->len to be 3.\n", constArr->len == 3 );
    mu_assert ( "TEST FAILED: container1_test - Expected first element of arr to be 1.\n", g_array_index ( constArr, int64_t, 0 ) == 1 );
    mu_assert ( "TEST FAILED: container1_test - Expected second element of arr to be -1.\n", g_array_index ( constArr, int64_t, 1 ) == -1 );
    mu_assert ( "TEST FAILED: container1_test - Expected third element of arr to be 2.\n", g_array_index ( constArr, int64_t, 2 ) == 2 );

    varArr = container_get_varr ( instance );
    mu_assert ( "TEST FAILED: container1_test - Expected varr to have exactly two elements.\n", varArr->len == 2 );
    mu_assert ( "TEST FAILED: container1_test - Expected first element of varr to be 'val1'.\n", strcmp ( g_array_index ( varArr, char*, 0 ), "val1" ) == 0 );
    mu_assert ( "TEST FAILED: container1_test - Expected second element of varr to be 'val2'.\n", strcmp ( g_array_index ( varArr, char*, 1 ), "val2" ) == 0 );

    list = container_get_l ( instance );
    mu_assert ( "TEST FAILED: container1_test - Expected list 'l' to have exactly 3 elements.\n", g_list_length ( list ) == 3 );
    mu_assert ( "TEST FAILED: container1_test - Expected first element of list 'l' to be true.\n", *( (bool*) g_list_nth_data ( list, 0 ) ) == true );
    mu_assert ( "TEST FAILED: container1_test - Expected second element of list 'l' to be true.\n", *( (bool*) g_list_nth_data ( list, 1 ) ) == true );
    mu_assert ( "TEST FAILED: container1_test - Expected third element of list 'l' to be false.\n", *( (bool*) g_list_nth_data ( list, 2 ) ) == false );

    set = container_get_s ( instance );
    mu_assert ( "TEST FAILED: container1_test - Expected the set 's' to have exactly 3 elements.\n", g_hash_table_size ( set ) == 3 );
    mu_assert ( "TEST FAILED: container1_test - Expected the set 's' to contain the number '1'.\n", g_hash_table_contains ( set, &k1 ) );
    mu_assert ( "TEST FAILED: container1_test - Expected the set 's' to contain the number '2'.\n", g_hash_table_contains ( set, &k2 ) );
    mu_assert ( "TEST FAILED: container1_test - Expected the set 's' to contain the number '3'.\n", g_hash_table_contains ( set, &k3 ) );

    map = container_get_f ( instance );
    mu_assert ( "TEST FAILED: container1_test - Expected the map 'f' to have exactly 2 elements.\n", g_hash_table_size ( map ) == 2 );
    test1_table = (GHashTable*) g_hash_table_lookup ( map, "test1" );
    mu_assert ( "TEST FAILED: Expected an entry for key 'test1' in the map 'f'.\n", test1_table != 0 );
    test2_table = (GHashTable*) g_hash_table_lookup ( map, "test2" );
    mu_assert ( "TEST FAILED: Expected an entry for key 'test2' in the map 'f'.\n", test2_table != 0 );
    mu_assert ( "TEST FAILED: container1_test - Expected the first nested map of map 'f' to have exactly 2 elements.\n", g_hash_table_size ( test1_table ) == 2 );
    mu_assert ( "TEST FAILED: container1_test - Expected the second nested map of map 'f' to have exactly 1 element.\n", g_hash_table_size ( test2_table ) == 1 );

    mu_assert ( "TEST FAILED: Expected the first nested map to contain 'true' for the key '1'.\n", *( (bool*) g_hash_table_lookup ( test1_table, &k1 ) ) == true );
    mu_assert ( "TEST FAILED: Expected the first nested map to contain 'false' for the key '-1'.\n", *( (bool*) g_hash_table_lookup ( test1_table, &k2 ) ) == false );
    mu_assert ( "TEST FAILED: Expected the second nested map to contain 'true' for the key '2'.\n", *( (bool*) g_hash_table_lookup ( test2_table, &k3 ) ) == true );

    g_list_free ( instances );
    delete_skill_state ( state );
    g_array_free ( constArr, true );
    g_array_free ( varArr, true );
    g_list_free ( list );
    g_hash_table_destroy ( set );
    g_hash_table_destroy ( test1_table );
    g_hash_table_destroy ( test2_table );
    g_hash_table_destroy ( map );
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
