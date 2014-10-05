#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <stdbool.h>
#include "../../minunit.h"
#include "api.h"

int tests_run = 0;
const float FLOAT_EPSILON = 0.000001;
const float DOUBLE_EPSILON = 0.0000000000000005;

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

    char *string1 = "test1";
    char *string2 = "test2";
    char *string3 = "test3";
    // One instance with basic values
    create_basic ( state, 1, 2, 3, 4, 5, 1.1, 1.2, string1, true );
    // One instance with negative values
    create_basic ( state, -1, -2, -3, -4, -5, -1.1, -1.2, string2, false );
    // One instance testing range or precision
    // for the integer types, use the highest number, they should support.
    // For the floats, check the precision
    create_basic ( state, 127, 32767, 2147483647, 9223372036854775807, -1, 1.000001, 1.0000000000000005, string3, true );

    write_to_file ( state, "./resources/basic.sf" );
    delete_skill_state ( state );
    state = skill_state_from_file ( "./resources/basic.sf" );

    GList *instances = get_basic_instances ( state );
    basic instance1 = (basic) g_list_nth_data ( instances, 0 );
    basic instance2 = (basic) g_list_nth_data ( instances, 1 );
    basic instance3 = (basic) g_list_nth_data ( instances, 2 );

    mu_assert ( "TEST FAILED: value should be 1.\n", basic_get_i8 ( instance1 ) == 1 );
    mu_assert ( "TEST FAILED: value should be 2.\n", basic_get_i16 ( instance1 ) == 2 );
    mu_assert ( "TEST FAILED: value should be 3.\n", basic_get_i32 ( instance1 ) == 3 );
    mu_assert ( "TEST FAILED: value should be 4.\n", basic_get_i64 ( instance1 ) == 4 );
    mu_assert ( "TEST FAILED: value should be 5.\n", basic_get_v64 ( instance1 ) == 5 );
    mu_assert ( "TEST FAILED: value should be 1.1.\n", fabs ( basic_get_f32 ( instance1 ) - 1.1f ) < FLOAT_EPSILON );
    mu_assert ( "TEST FAILED: value should be 1.2.\n", fabs ( basic_get_f64 ( instance1 ) - 1.2 ) < DOUBLE_EPSILON );
    mu_assert ( "TEST FAILED: value should be 'test1'.\n", strcmp ( basic_get_string ( instance1 ), "test1" ) == 0 );
    mu_assert ( "TEST FAILED: value should be true.\n", basic_get_bool( instance1 ) == true );

    mu_assert ( "TEST FAILED: value should be -1.\n", basic_get_i8 ( instance2 ) == -1 );
    mu_assert ( "TEST FAILED: value should be -2.\n", basic_get_i16 ( instance2 ) == -2 );
    mu_assert ( "TEST FAILED: value should be -3.\n", basic_get_i32 ( instance2 ) == -3 );
    mu_assert ( "TEST FAILED: value should be -4.\n", basic_get_i64 ( instance2 ) == -4 );
    mu_assert ( "TEST FAILED: value should be -5.\n", basic_get_v64 ( instance2 ) == -5 );
    mu_assert ( "TEST FAILED: value should be -1.1.\n", fabs ( basic_get_f32 ( instance2 ) + 1.1f ) < FLOAT_EPSILON );
    mu_assert ( "TEST FAILED: value should be -1.2.\n", fabs ( basic_get_f64 ( instance2 ) + 1.2 ) < DOUBLE_EPSILON );
    mu_assert ( "TEST FAILED: value should be 'test2'.\n", strcmp ( basic_get_string ( instance2 ), "test2" ) == 0 );
    mu_assert ( "TEST FAILED: value should be false.\n", basic_get_bool( instance2 ) == false );

    mu_assert ( "TEST FAILED: value should be 127.\n", basic_get_i8 ( instance3 ) == 127 );
    mu_assert ( "TEST FAILED: value should be 32767.\n", basic_get_i16 ( instance3 ) == 32767 );
    mu_assert ( "TEST FAILED: value should be 2147483647.\n", basic_get_i32 ( instance3 ) == 2147483647 );
    mu_assert ( "TEST FAILED: value should be 9223372036854775807.\n", basic_get_i64 ( instance3 ) == 9223372036854775807 );
    mu_assert ( "TEST FAILED: value should be -1.\n", basic_get_v64 ( instance3 ) == -1 );
    mu_assert ( "TEST FAILED: value should be 1.000001.\n", ( basic_get_f32 ( instance3 ) - 1.f ) != 0 );
    mu_assert ( "TEST FAILED: value should be 1.0000000000000005.\n", ( basic_get_f64 ( instance3 ) - 1.0 ) != 0 );
    mu_assert ( "TEST FAILED: value should be 'test3'.\n", strcmp ( basic_get_string ( instance3 ), "test3" ) == 0 );
    mu_assert ( "TEST FAILED: value should be true.\n", basic_get_bool( instance3 ) == true );

    g_list_free ( instances );
    delete_skill_state ( state );
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
        printf ( "basicTypes_test: all tests passed.\n" );
        return 0;
    }
}
