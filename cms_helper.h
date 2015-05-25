#ifndef _CMS_HELPER_H
#define _CMS_HELPER_H

#include "cms.h"

/***************************************
 * Common function
 **************************************/
long int strtol_default(const char *nptr, char **endptr, int base, long int default_value);
unsigned long int strtoul_default(const char *nptr, char **endptr, int base, unsigned long int default_value);
const char *format_string(const char *str, size_t str_size, const char *format, ...);

/***************************************
 * cms get helper
 **************************************/
int cms_get_int(const char *format, int default_value, ...);
unsigned int cms_get_uint(const char *format, unsigned int default_value, ...);
char *cms_get_str(const char *format, char *default_value, ...);

int cms_get_int_index(const char *format, int default_value, unsigned int index, char *separator, ...);
unsigned int cms_get_uint_index(const char *format, unsigned int default_value, unsigned int index, char *separator, ...);
char *cms_get_str_index(const char *format, char *default_value, unsigned int index, char *separator, ...);

int cms_get_int_array(const char *format, int default_value, int array[], size_t array_size, char *separator, ...);
int cms_get_uint_array(const char *format, unsigned int default_value, unsigned int array[], size_t array_size, char *separator, ...);
int cms_get_str_array(const char *format, char *default_value, const char *array[], size_t array_size, char *get_buffer, size_t get_buffer_size, char *separator, ...);

/***************************************
 * cms virtual set/replace helper
 **************************************/
int cms_vset_int(int vset_type, const char *format, const int value, ...);
int cms_vset_uint(int vset_type, const char *format, const unsigned int value, ...);
int cms_vset_str(int vset_type, const char *format, const char *value, ...);

int cms_vset_int_index(int vset_type, const char *format, const int value, unsigned int index, char *separator, ...);
int cms_vset_uint_index(int vset_type, const char *format, const unsigned int value, unsigned int index, char *separator, ...);
int cms_vset_str_index(int vset_type, const char *format, const char *value, unsigned int index, char *separator, ...);

int cms_vset_int_array(int vset_type, const char *format, int array[], size_t array_size, const char *separator, const char *tail_separator, ...);
int cms_vset_uint_array(int vset_type, const char *format, int array[], size_t array_size, const char *separator, const char *tail_separator, ...);
int cms_vset_str_array(int vset_type, const char *format, const char *array[], size_t array_size, const char *separator, const char *tail_separator, ...);


/***************************************
 * cms set helper
 **************************************/
#define cms_set_int(format, value ...)	cms_vset_int(SetValueUser, format, value)
#define cms_set_uint(format, value ...)	cms_vset_uint(SetValueUser, format, value)
#define cms_set_str(format, value ...)	cms_vset_str(SetValueUser, format, value)

#define cms_set_int_index(format, value, index, separator ...)	cms_vset_int_index(SetValueUser, format, value, index, separator)
#define cms_set_uint_index(format, value, index, separator ...)	cms_vset_uint_index(SetValueUser, format, value, index, separator)
#define cms_set_str_index(format, value, index, separator...)	cms_vset_str_index(SetValueUser, format, value, index, separator)

#define cms_set_int_array(format, array, array_size, separator, tail_separator ...) 	cms_vset_int_array(SetValueUser, format, array, array_size, separator, tail_separator)
#define cms_set_uint_array(format, array, array_size, separator, tail_separator ...)	cms_vset_uint_array(SetValueUser, format, array, array_size, separator, tail_separator)
#define cms_set_str_array(format, array, array_size, separator, tail_separator ...)		cms_vset_str_array(SetValueUser, format, array, array_size, separator, tail_separator)

/***************************************
 * cms replace helper
 **************************************/
#define cms_replace_int(format, value ...)	cms_vset_int(ReplaceValueUser, format, value)
#define cms_replace_uint(format, value ...)	cms_vset_uint(ReplaceValueUser, format, value)
#define cms_replace_str(format, value ...)	cms_vset_str(ReplaceValueUser, format, value)

#define cms_replace_int_index(format, value, index, separator ...)	cms_vset_int_index(ReplaceValueUser, format, value, index, separator)
#define cms_replace_uint_index(format, value, index, separator ...)	cms_vset_uint_index(ReplaceValueUser, format, value, index, separator)
#define cms_replace_str_index(format, value, index, separator...)	cms_vset_str_index(ReplaceValueUser, format, value, index, separator)

#define cms_replace_int_array(format, array, array_size, separator, tail_separator ...) 	cms_vset_int_array(ReplaceValueUser, format, array, array_size, separator, tail_separator)
#define cms_replace_uint_array(format, array, array_size, separator, tail_separator ...)	cms_vset_uint_array(ReplaceValueUser, format, array, array_size, separator, tail_separator)
#define cms_replace_str_array(format, array, array_size, separator, tail_separator ...)		cms_vset_str_array(ReplaceValueUser, format, array, array_size, separator, tail_separator)

#endif //_CMS_HELPER_H
