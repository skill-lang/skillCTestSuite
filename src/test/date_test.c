#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <glib.h>
#include "minunit.h"
#include "api.h"

int tests_run = 0;

// This reads the two date instances from the example and checks their values '1' and '-1'.
char *test_read_date_instances () {
    skill_state state = skill_state_from_file ( "./resources/date.sf" );

    GList *date_instances = get_date_instances ( state );
    mu_assert ( "TEST FAILED: date_test - read_date_instances: Expected exactly 2 instances.\n", g_list_length ( date_instances ) == 2 );

    date first_instance = (date) g_list_nth_data ( date_instances, 0 );
    date second_instance = (date) g_list_nth_data ( date_instances, 1 );

    mu_assert ( "TEST FAILED: date_test - read_date_instances: date value of the first instance was not 1.\n", date_get_date ( first_instance ) == 1 );
    mu_assert ( "TEST FAILED: date_test - read_date_instances: date value of the first instance was not -1.\n", date_get_date ( second_instance ) == -1 );

    g_list_free ( date_instances );
    delete_skill_state ( state );
    return 0;
}

char *test_write_1000000_instances () {
    skill_state state = empty_skill_state ();

    int i;
    for ( i = 0; i < 1000000; i++ ) {
        create_date ( state, i );
    }
    write_to_file ( state, "./resources/date_1000000_instances.sf" );

    state = skill_state_from_file ( "./resources/date_1000000_instances.sf" );

    GList *instances = get_date_instances ( state );
    mu_assert ( "TEST FAILED: date_test - write_1000000_instances: Expected exactly 1000000 date instances.\n", g_list_length ( instances ) == 1000000 );

    g_list_free ( instances );
    delete_skill_state ( state );
    return 0;
}

static char *all_tests () {
    mu_run_test ( test_read_date_instances );
    mu_run_test ( test_write_1000000_instances );
    return 0;
}

int main () {
    char *result = all_tests ();
    if ( result != 0 ) {
        printf( "%s\n", result );
        return 1;
    } else {
        printf ( "date_test: all tests passed.\n" );
        return 0;
    }
}
