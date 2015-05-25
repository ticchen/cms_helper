#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "cms.h"
#include "cms_helper.h"

int main(int argc, char* argv[])
{
	//test();
	//exit(0);

	cms_init();
	cms_config_import("test.conf");

	int int_value_err = -999;
	unsigned int uint_value_err = 999;

	assert( cms_get_int("int_name_nonexist", int_value_err) == int_value_err);
	assert( cms_get_int("int_value_positive", int_value_err) == 1);
	assert( cms_get_int("int_value_zero", int_value_err) == 0);
	assert( cms_get_int("int_value_negative", int_value_err) == -1);
	assert( cms_get_int("int_value_incorrect", int_value_err) == int_value_err);
	assert( cms_get_int("int_value_empy", int_value_err) == int_value_err);

	assert( cms_get_uint("uint_name_nonexist", uint_value_err) == uint_value_err);
	assert( cms_get_uint("uint_value_positive", uint_value_err) == 1);
	assert( cms_get_uint("uint_value_zero", uint_value_err) == 0);
	assert( cms_get_uint("uint_value_negative", uint_value_err) == uint_value_err);
	assert( cms_get_uint("uint_value_incorrect", uint_value_err) == uint_value_err);
	assert( cms_get_uint("uint_value_empy", uint_value_err) == uint_value_err);

	assert( strcmp(cms_get_str("str_name_nonexist", "empty"), "empty") == 0);
	assert( cms_get_str("str_name_nonexist", NULL) == NULL);
	assert( strcmp(cms_get_str("str_value", "0"), "1") == 0 );
	assert( strcmp(cms_get_str("str_value_empy", "0"), "0") == 0 );

	assert( cms_get_int_index("array", int_value_err, 0, ";: |") == int_value_err);	//support zero length token
	assert( cms_get_int_index("array", int_value_err, 1, ";: |") == 1);	//normal case
	assert( cms_get_int_index("array", int_value_err, 4, ";: |") == -4);	//negative value
	assert( cms_get_int_index("array", int_value_err, 7, ";: |") == int_value_err);	//invalue digital
	assert( cms_get_int_index("array", int_value_err, 100, ";: |") == int_value_err);	//over range
	assert( cms_get_int_index("array_array", int_value_err, 100, ";: |") == int_value_err);	//empty array

	assert( cms_get_uint_index("array", uint_value_err, 0, ";: |") == uint_value_err);	//support zero length token
	assert( cms_get_uint_index("array", uint_value_err, 1, ";: |") == 1);	//normal case
	assert( cms_get_uint_index("array", uint_value_err, 4, ";: |") == uint_value_err);	//negative value
	assert( cms_get_uint_index("array", uint_value_err, 7, ";: |") == uint_value_err);	//invalue digital
	assert( cms_get_uint_index("array", uint_value_err, 100, ";: |") == uint_value_err);	//over range
	assert( cms_get_uint_index("array_array", uint_value_err, 100, ";: |") == uint_value_err);	//empty array

	assert( strlen(cms_get_str_index("array", "!@#$%", 0, ";: |")) == 0);	//support zero length token
	assert( strcmp(cms_get_str_index("array", "", 1, ";: |"), "1") == 0);	//normal case
	assert( strcmp(cms_get_str_index("array", "", 7, ";: |"), "seven") == 0);	//normal case
	assert( strcmp(cms_get_str_index("array", "NULL_STR", 100, ";: |"),"NULL_STR") == 0);	//over range
	assert( cms_get_str_index("array_empty", NULL, 100, ";: |") == NULL);	//empty array

	int num = 0;

	int int_array[10] = {0};
	num = cms_get_int_array("array_size", int_value_err, int_array, sizeof(int_array) / sizeof(int_array[0]), ";: |" );
	{
		fprintf(stderr, "num=%d\n", num);
		int i = 0;
		for(i = 0; i < sizeof(int_array) / sizeof(int_array[0]); i++) {
			fprintf(stderr, "array[%d]=%d\n", i, int_array[i]);
		}
	}

	unsigned int uint_array[9] = {0};
	num = cms_get_uint_array("array_size", uint_value_err, uint_array, sizeof(uint_array) / sizeof(uint_array[0]), ";: |");
	{
		int i = 0;
		for(i = 0; i < sizeof(uint_array) / sizeof(uint_array[0]); i++) {
			fprintf(stderr, "uarray[%d]=%d\n", i, uint_array[i]);
		}
	}

	const char *str_array[10] = {0};
	char get_buffer[BUFFER_MAX];
	num = cms_get_str_array("array_size", "fuck", str_array, sizeof(str_array) / sizeof(str_array[0]), get_buffer, sizeof(get_buffer), ";: |");
	{
		fprintf(stderr, "num=%d\n", num);
		int i = 0;
		for(i = 0; i < sizeof(str_array) / sizeof(str_array[0]); i++) {
			fprintf(stderr, "str_array[%d]=%s\n", i, str_array[i]);
		}
	}

	char *str = cms_get_str("test_%s_%d", "0", "1", 2);
	fprintf(stderr, "str=%s\n", str);
	cms_set_str("test_%s_%d", "1234", "1", 2);

	str = cms_get_str("test_%s_%d", "0", "1", 2);
	fprintf(stderr, "str=%s\n", str);

	cms_set_int("test_%s_%d", -1, "1", 2);

	str = cms_get_str("test_%s_%d", "0", "1", 2);
	fprintf(stderr, "str=%s\n", str);

	//cms_set_str_array("test", str_array, sizeof(str_array) / sizeof(str_array[0]), ",.", NULL);

	assert( cms_set_str_index("array", "xxxx", -1, ";: |") == -1);

	cms_set_str_index("array", "zero", 0, ";: |");
	assert( strcmp(cms_get_str("array", ""), "zero;1:2:3;-4 -5 -6;seven|eight|nine") == 0);

	cms_set_str_index("array", "three", 3, ";: |");
	assert( strcmp(cms_get_str("array", ""), "zero;1:2:three;-4 -5 -6;seven|eight|nine") == 0);

	cms_set_str_index("array", "9", 9, ";: |");
	assert( strcmp(cms_get_str("array", ""), "zero;1:2:three;-4 -5 -6;seven|eight|9") == 0);

	cms_set_str_index("array", "new", 11, ";: |");
	assert( strcmp(cms_get_str("array", ""), "zero;1:2:three;-4 -5 -6;seven|eight|9;;new") == 0);

	cms_set_str_index("array", NULL, 0, ";: |");
	assert( strcmp(cms_get_str("array", ""), "1:2:three;-4 -5 -6;seven|eight|9;;new") == 0);

	cms_set_str_index("array", NULL, 7, ";: |");
	assert( strcmp(cms_get_str("array", ""), "1:2:three;-4 -5 -6;seven|9;;new") == 0);

	cms_set_str_index("array", NULL, 9, ";: |");
	assert( strcmp(cms_get_str("array", ""), "1:2:three;-4 -5 -6;seven|9;") == 0);

	cms_set_str_index("array", NULL, 11, ";: |");
	assert( strcmp(cms_get_str("array", ""), "1:2:three;-4 -5 -6;seven|9;") == 0);

	cms_set_str_index("array_empty", "xxxx", 0, ";: |");
	assert( strcmp(cms_get_str("array_empty", ""), "xxxx") == 0);

	cms_set_str("array_empty", "");
	cms_set_str_index("array_empty", "xxxx", 3, ";: |");
	assert( strcmp(cms_get_str("array_empty", ""), ";;;xxxx") == 0);

	cms_set_str("array_empty", "");
	cms_set_str_index("array_empty", NULL, 0, ";: |");
	assert( strcmp(cms_get_str("array_empty", ""), "") == 0);

	cms_set_str("array_empty", "");
	cms_set_str_index("array_empty", NULL, 3, ";: |");
	assert( strcmp(cms_get_str("array_empty", ""), "") == 0);

	cms_set_int_index("array_int", 0, 0, ";: |");
	assert( strcmp(cms_get_str("array_int", ""), "0;1;-2 -3|4;;-6;") == 0);

	cms_set_int_index("array_int", -5, 5, ";: |");
	assert( strcmp(cms_get_str("array_int", ""), "0;1;-2 -3|4;-5;-6;") == 0);

	cms_set_uint_index("array_uint", 0, 0, ";: |");
	assert( strcmp(cms_get_str("array_uint", ""), "0;1;2 3|4;;6;") == 0);

	cms_set_uint_index("array_uint", 5, 5, ";: |");
	assert( strcmp(cms_get_str("array_uint", ""), "0;1;2 3|4;5;6;") == 0);

	printf("passed\n");

	return 0;
}
