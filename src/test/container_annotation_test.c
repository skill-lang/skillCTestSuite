#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <stdbool.h>
#include "minunit.h"
#include "api.h"

int tests_run = 0;

// TODO correct this description
/*
This tests container types filled with user-defined types. The specification:
 *
Container {
  annotation[3] arr;
  annotation[] varr;
  list<annotation> l;
  set<annotation> s;
  map<annotation, annotation, annotation> f;
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

    type1 t1 = create_type1 ( state );

    type2 t2 = create_type2 ( state );

    type3 t3 = create_type3 ( state );

    GArray *constArr = g_array_new ( true, true, sizeof (skill_type) );
    g_array_append_val ( constArr, t1 );
    g_array_append_val ( constArr, t2 );
    g_array_append_val ( constArr, t3 );

    GArray *varArr = g_array_new ( true, true, sizeof (type2) );
    g_array_append_val ( varArr, t1 );
    g_array_append_val ( varArr, t2 );
    g_array_append_val ( varArr, t3 );

    GList *list = 0;
    list = g_list_append ( list, t1 );
    list = g_list_append ( list, t2 );
    list = g_list_append ( list, t3 );

    GHashTable *set = g_hash_table_new ( g_direct_hash, g_direct_equal );
    g_hash_table_insert ( set, t1, t1 );
    g_hash_table_insert ( set, t2, t2 );

    // Build a map like this:
    // "t1" -> { ( t2 -> t3 ), ( t3 -> t2 ) }
    GHashTable *map = g_hash_table_new ( g_direct_hash, g_direct_equal );
    GHashTable *test1_table = g_hash_table_new ( g_direct_hash, g_direct_equal );

    g_hash_table_insert ( test1_table, t2, t3 );
    g_hash_table_insert ( test1_table, t3, t2 );
    g_hash_table_insert ( map, t1, test1_table );

    create_container ( state, constArr, varArr, list, set, map );

    write_to_file ( state, "./resources/container_annotations.sf" );
    delete_skill_state ( state );
    g_array_free ( constArr, true );
    g_array_free ( varArr, true );
    g_list_free ( list );
    g_hash_table_destroy ( set );
    g_hash_table_destroy ( test1_table );
    g_hash_table_destroy ( map );

    state = skill_state_from_file ( "./resources/container_annotations.sf" );

    GList *instances = get_container_instances ( state );
    mu_assert ( "TEST FAILED: container_annotations_test - read_write_instances: Expected exactly 1 container instance.\n", g_list_length ( instances ) == 1 );

    container instance = (container) g_list_nth_data ( instances, 0 );

    GList *type1_instances = get_type1_instances ( state );
    t1 = (type1) g_list_nth_data ( type1_instances, 0 );

    GList *type2_instances = get_type2_instances ( state );
    t2 = (type2) g_list_nth_data ( type2_instances, 0 );

    GList *type3_instances = get_type3_instances ( state );
    t3 = (type3) g_list_nth_data ( type3_instances, 0 );

    constArr = container_get_arr ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected arr to have exactly 3 elements.\n", constArr->len == 3 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected first element of arr to be 't1'.\n", g_array_index ( constArr, type1, 0 ) == t1 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected second element of arr to be 't2'.\n", g_array_index ( constArr, type2, 1 ) == t2 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected third element of arr to be 't3'.\n", g_array_index ( constArr, type3, 2 ) == t3 );

    varArr = container_get_varr ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected varr to have exactly 3 elements.\n", varArr->len == 3 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected first element of varr to be 't1'.\n", g_array_index ( varArr, type1, 0 ) == t1 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected second element of varr to be 't2'.\n", g_array_index ( varArr, type2, 1 ) == t2 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected third element of varr to be 't3'.\n", g_array_index ( varArr, type3, 2 ) == t3 );

    list = container_get_l ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected list 'l' to have exactly 3 elements.\n", g_list_length ( list ) == 3 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected first element of list 'l' to be 't1'.\n", (type1) g_list_nth_data ( list, 0 ) == t1 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected second element of list 'l' to be 't2'.\n", (type2) g_list_nth_data ( list, 1 ) == t2 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected third element of list 'l' to be 't3'.\n", (type3) g_list_nth_data ( list, 2 ) == t3 );

    set = container_get_s ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the set 's' to have exactly 2 elements.\n", g_hash_table_size ( set ) == 2 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the set 's' to contain 't1'.\n", g_hash_table_contains ( set, t1 ) );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the set 's' to contain 't2'.\n", g_hash_table_contains ( set, t2 ) );

    map = container_get_f ( instance );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the map 'f' to have exactly 1 element.\n", g_hash_table_size ( map ) == 1 );
    test1_table = (GHashTable*) g_hash_table_lookup ( map, t1 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected an entry for key 't1' in the map 'f'.\n", test1_table != 0 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the first nested map of map 'f' to have exactly 2 elements.\n", g_hash_table_size ( test1_table ) == 2 );

    mu_assert ( "TEST FAILED: container_annotations_test - Expected the first nested map to contain 't3' for the key 't2'.\n", (type3) g_hash_table_lookup ( test1_table, t2 ) == t3 );
    mu_assert ( "TEST FAILED: container_annotations_test - Expected the first nested map to contain 't2' for the key 't3'.\n", (type2) g_hash_table_lookup ( test1_table, t3 ) == t2 );

    delete_skill_state ( state );
    g_array_free ( constArr, true );
    g_array_free ( varArr, true );
    g_list_free ( list );
    g_hash_table_destroy ( set );
    g_hash_table_destroy ( test1_table );
    g_hash_table_destroy ( map );

    g_list_free ( instances );
    g_list_free ( type1_instances );
    g_list_free ( type2_instances );
    g_list_free ( type3_instances );
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
        printf ( "container_test: all tests passed.\n" );
        return 0;
    }
}
