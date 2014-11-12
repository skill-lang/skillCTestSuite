#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <stdbool.h>
#include "minunit.h"
#include "api.h"

int tests_run = 0;

/*
This tests container types filled with user-defined types. The specification:
 *
Container {
  string[3] arr;
  string[] varr;
  list<string> l;
  set<string> s;
  map<string, string, string> f;
}
 */

char *write_read_instances () {

    skill_state state = empty_skill_state ();

    char *string1 = "test1";
    char *string2 = "test2";
    char *string3 = "test3";

    GArray *constArr = g_array_new ( true, true, sizeof (char*) );
    g_array_append_val ( constArr, string1 );
    g_array_append_val ( constArr, string2 );
    g_array_append_val ( constArr, string3 );

    GArray *varArr = g_array_new ( true, true, sizeof (char*) );
    g_array_append_val ( varArr, string1 );
    g_array_append_val ( varArr, string2 );
    g_array_append_val ( varArr, string3 );

    GList *list = 0;
    list = g_list_append ( list, "test1" );
    list = g_list_append ( list, "test2" );
    list = g_list_append ( list, "test3" );

    GHashTable *set = g_hash_table_new ( g_str_hash, g_str_equal );
    g_hash_table_insert ( set, "test1", "test1" );
    g_hash_table_insert ( set, "test2", "test2" );

    // Build a map like this:
    // "test1" -> { ( "test2" -> "test3" ) }
    GHashTable *map = g_hash_table_new ( g_str_hash, g_str_equal );
    GHashTable *test1_table = g_hash_table_new ( g_str_hash, g_str_equal );

    g_hash_table_insert ( test1_table, "test2", "test3" );
    g_hash_table_insert ( map, "test1", test1_table );

    create_container ( state, constArr, varArr, list, set, map );

    write_to_file ( state, "./resources/container_strings.sf" );
    delete_skill_state ( state );
    g_array_free ( constArr, true );
    g_array_free ( varArr, true );
    g_list_free ( list );
    g_hash_table_destroy ( set );
    g_hash_table_destroy ( test1_table );
    g_hash_table_destroy ( map );

    state = skill_state_from_file ( "./resources/container_strings.sf" );

    GList *instances = get_container_instances ( state );
    mu_assert ( "TEST FAILED: container_annotations_test - read_write_instances: Expected exactly 1 container instance.\n", g_list_length ( instances ) == 1 );

    container instance = (container) g_list_nth_data ( instances, 0 );

    constArr = container_get_arr ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected arr to have exactly 3 elements.\n", constArr->len == 3 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected first element of arr to be 'test1'.\n", strcmp ( g_array_index ( constArr, char*, 0 ), "test1" ) == 0 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected second element of arr to be 'test2'.\n", strcmp ( g_array_index ( constArr, char*, 1 ), "test2" ) == 0 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected third element of arr to be 'test3'.\n", strcmp ( g_array_index ( constArr, char*, 2 ), "test3" ) == 0 );

    varArr = container_get_varr ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected varr to have exactly 3 elements.\n", varArr->len == 3 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected first element of varr to be 'test1'.\n", strcmp ( g_array_index ( varArr, char*, 0 ), "test1" ) == 0 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected second element of varr to be 'test2'.\n", strcmp ( g_array_index ( varArr, char*, 1 ), "test2" ) == 0 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected third element of varr to be 'test3'.\n", strcmp ( g_array_index ( varArr, char*, 2 ), "test3" ) == 0 );

    list = container_get_l ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected list 'l' to have exactly 3 elements.\n", g_list_length ( list ) == 3 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected first element of list 'l' to be 'test1'.\n", strcmp ( (char*) g_list_nth_data ( list, 0 ), "test1" ) == 0 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected second element of list 'l' to be 'test2'.\n", strcmp ( (char*) g_list_nth_data ( list, 1 ), "test2" ) == 0 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected third element of list 'l' to be 'test3'.\n", strcmp ( (char*) g_list_nth_data ( list, 2 ), "test3" ) == 0 );

    set = container_get_s ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the set 's' to have exactly 2 elements.\n", g_hash_table_size ( set ) == 2 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the set 's' to contain 'test1'.\n", g_hash_table_contains ( set, "test1" ) );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the set 's' to contain 'test2'.\n", g_hash_table_contains ( set, "test2" ) );

    map = container_get_f ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the map 'f' to have exactly 1 element.\n", g_hash_table_size ( map ) == 1 );
    test1_table = (GHashTable*) g_hash_table_lookup ( map, "test1" );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected an entry for key 'test1' in the map 'f'.\n", test1_table != 0 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the first nested map of map 'f' to have exactly 1 element.\n", g_hash_table_size ( test1_table ) == 1 );

    mu_assert ( "TEST FAILED: container_annotations_test - Expected the first nested map to contain 't3' for the key 't2'.\n", strcmp ( (char*) g_hash_table_lookup ( test1_table, "test2" ), "test3" ) == 0 );

    delete_skill_state ( state );
    g_array_free ( constArr, true );
    g_array_free ( varArr, true );
    g_list_free ( list );
    g_hash_table_destroy ( set );
    g_hash_table_destroy ( test1_table );
    g_hash_table_destroy ( map );

    g_list_free ( instances );
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
        printf ( "container_string_test: all tests passed.\n" );
        return 0;
    }
}
