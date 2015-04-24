#ifndef _CMS_HELPER_H
#define _CMS_HELPER_H


/***************************************
 * Common function
 **************************************/
long int strtol_default(const char *nptr, char **endptr, int base, long int default_value);
unsigned long int strtoul_default(const char *nptr, char **endptr, int base, unsigned long int default_value);

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
int cms_get_uint_array(const char *format, unsigned int default_value, int array[], size_t array_size, char *separator, ...);
int cms_get_str_array(const char *format, char *default_value, const char *array[], size_t array_size, char *get_buffer, size_t get_buffer_size, char *separator, ...);
/***************************************
 * cms set helper
 **************************************/
int cms_set_int(const char *format, const int value, ...);
int cms_set_uint(const char *format, const unsigned int value, ...);
int cms_set_str(const char *format, const char *value, ...);

int cms_set_int_index(const char *format, const int value, ...);
int cms_set_uint_index(const char *format, const unsigned int value, ...);
int cms_set_str_index(const char *format, const char *value, unsigned int index, char *separator, ...);

int cms_set_int_array(const char *format, int array[], size_t array_size, const char *separator, const char *tail_separator, ...);
int cms_set_uint_array(const char *format, int array[], size_t array_size, const char *separator, const char *tail_separator, ...);
int cms_set_str_array(const char *format, const char *array[], size_t array_size, const char *separator, const char *tail_separator, ...);
#endif //_CMS_HELPER_H
