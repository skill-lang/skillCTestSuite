#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include <stdbool.h>
#include "../../minunit.h"
#include "api/container2_api.h"

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

    GHashTable *set = g_hash_table_new ( g_int_hash, g_int_equal );
    g_hash_table_insert ( set, t3b, t3b );
    g_hash_table_insert ( set, t3a, t3a );

    // Build a map like this:
    // "t1a" -> { ( t2a -> t3b ), ( t2b -> t3a ) }
    // "t1b" -> { ( t2b -> t3b ) }
    GHashTable *map = g_hash_table_new ( g_direct_hash, g_direct_equal );
    GHashTable *test1_table = g_hash_table_new ( g_int_hash, g_int_equal );
    GHashTable *test2_table = g_hash_table_new ( g_int_hash, g_int_equal );

    g_hash_table_insert ( test1_table, t2a, t3b );
    g_hash_table_insert ( test1_table, t2b, t3a );
    g_hash_table_insert ( test2_table, t2b, t3b );
    g_hash_table_insert ( map, t2a, test1_table );
    g_hash_table_insert ( map, t2b, test2_table );

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
    container instance = (container) g_list_nth_data ( instances, 0 );

    mu_assert ( "TEST FAILED: container_test - read_write_instances: Expected exactly 1 container instance.\n", g_list_length ( instances ) == 1 );

    // TODO other tests

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
