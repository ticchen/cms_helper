#ifndef _CMS_HELPER_H
#define _CMS_HELPER_H

/*************************
 * cms set helper
 *************************/
int cms_get_int(const char *format, int default_value, ...);
unsigned int cms_get_uint(const char *format, unsigned int default_value, ...);
char *cms_get_str(const char *format, char *default_value, ...);

int cms_get_int_index(const char *format, int default_value, unsigned int index, char *separator, ...);
unsigned int cms_get_uint_index(const char *format, unsigned int default_value, unsigned int index, char *separator, ...);
char *cms_get_str_index(const char *format, char *default_value, unsigned int index, char *separator, ...);

int cms_get_int_array(const char *format, int default_value, int array[], size_t array_size, char *separator, ...);
int cms_get_uint_array(const char *format, unsigned int default_value, int array[], size_t array_size, char *separator, ...);
int cms_get_str_array(const char *format, char *default_value, const char *array[], size_t array_size, char *get_buffer, size_t get_buffer_size, char *separator, ...);
/*************************
 * cms set helper
 *************************/

#endif //_CMS_HELPER_H
