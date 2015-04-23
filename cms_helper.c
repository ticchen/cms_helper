#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <stdarg.h>

#include "cms.h"
#include "cms_helper.h"

/***************************************
 * Common function
 **************************************/
static const char *format_string_valist(const char *str, size_t str_size, const char *format, va_list args)
{
	if(str == NULL || str_size == 0 || format == NULL) {
		return "";
	}

	int len = vsnprintf((char *)str, str_size, format, args);
	if(len < 0 || str_size <= len) {
		fprintf(stderr, "Error: str is truncated");
	}
	return str;
}

const char *format_string(const char *str, size_t str_size, const char *format, ...)
{
	va_list args;
	va_start(args, format);
	str = format_string_valist(str, str_size, format, args);
	va_end(args);

	return str;
}


long int strtol_default(const char *nptr, char **endptr, int base, long int default_value)
{
	if(nptr == NULL) {
		return default_value;
	}

	errno = 0;
	long int value = strtol(nptr, endptr, base);
	if ((errno == ERANGE && (value == LONG_MIN || value == LONG_MAX))	//range error
	    || (errno != 0 && value == 0)) {	//format error
		return default_value;
	}

	if (*endptr == nptr) {
		//fprintf(stderr, "No digits were found\n");
		return default_value;
	}
	return value;
}


unsigned long int strtoul_default(const char *nptr, char **endptr, int base, unsigned long int default_value)
{
	if(nptr == NULL) {
		return default_value;
	}

	errno = 0;
	char *neg_sign = strchr(nptr, '-');
	if(neg_sign) {
		//found a negative sign
		if( (unsigned long int)(neg_sign - nptr) + 1 == strcspn(nptr, "1234567890") ) {
			//the negative sign is at the beginning of digital. // ex: "   -1234"
			return default_value;
		}
	}

	unsigned long int value = strtoul(nptr, endptr, base);
	if ((errno == ERANGE && value == ULONG_MAX ) //range error
	    || (errno != 0 && value == 0 )) {	//format error
		return default_value;
	}

	if (*endptr == nptr) {
		//fprintf(stderr, "No digits were found\n");
		return default_value;
	}

	return value;
}


/***************************************
 * cms get helper
 **************************************/
static char *cms_get_str_valist(const char *format, char *default_value, va_list args)
{
	if(format == NULL) {
		return default_value;
	}

	const char *name = format;
	if(strchr(format, '%') != NULL) {
		name = alloca(MaxStringLen);
		int len = vsnprintf((char *)name, MaxStringLen, format, args);
		if(len < 0 || MaxStringLen <= len) {
			return default_value;
		}
	}

	char *value_str = CMSGetValue((char *)name, GetValueUser);
	if(value_str == NULL) {
		return default_value;
	}

	return value_str;
}


int cms_get_int(const char *format, int default_value, ...)
{
	if(format == NULL) {
		return default_value;
	}

	va_list args;
	va_start(args, default_value);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	char *endptr = NULL;
	return strtol_default(value_str, &endptr, 10, default_value);
}


unsigned int cms_get_uint(const char *format, unsigned int default_value, ...)
{
	if(format == NULL) {
		return default_value;
	}

	va_list args;
	va_start(args, default_value);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	char *endptr = NULL;
	return strtoul_default(value_str, &endptr, 10, default_value);
}


char *cms_get_str(const char *format, char *default_value, ...)
{
	if(format == NULL) {
		return default_value;
	}

	va_list args;
	va_start(args, default_value);
	char *value_str = cms_get_str_valist(format, default_value, args);
	va_end(args);
	return value_str;
}


static char *cms_get_str_index_valist(const char *format, char *default_value, unsigned int index, char *separator, va_list args)
{
	if(format == NULL || separator == NULL) {
		return default_value;
	}

	char *value_str = cms_get_str_valist(format, NULL, args);
	if(value_str == NULL) {
		return default_value;
	}

	unsigned int i = 0;
	char *token = NULL;
	for(i = 0, token = strsep(&value_str, separator); i <= index && token != NULL; i++, token = strsep(&value_str, separator)) {
		if(i == index) {
			return token;
		}
	}
	return default_value;
}


int cms_get_int_index(const char *format, int default_value, unsigned int index, char *separator, ...)
{
	if(format == NULL || separator == NULL) {
		return default_value;
	}

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_index_valist(format, NULL, index, separator, args);
	va_end(args);

	if(value_str == NULL){
		return default_value;
	}

	char *endptr = NULL;
	return strtol_default(value_str, &endptr, 10, default_value);
}


unsigned int cms_get_uint_index(const char *format, unsigned int default_value, unsigned int index, char *separator, ...)
{
	if(format == NULL || separator == NULL) {
		return default_value;
	}

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_index_valist(format, NULL, index, separator, args);
	va_end(args);

	if(value_str == NULL){
		return default_value;
	}

	char *endptr = NULL;
	return strtoul_default(value_str, &endptr, 10, default_value);
}


char *cms_get_str_index(const char *format, char *default_value, unsigned int index, char *separator, ...)
{
	if(format == NULL || separator == NULL) {
		return default_value;
	}

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_index_valist(format, default_value, index, separator, args);
	va_end(args);

	return value_str;
}


int cms_get_int_array(const char *format, int default_value, int array[], size_t array_size, char *separator, ...)
{
	int i = 0;
	int token_num = 0;

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	int is_array_exist = 0;
	if(array != NULL && array_size != 0) {
		is_array_exist = 1;
	}

	if( value_str != NULL) {
		char *token = NULL , *last_token = NULL;
		for(token_num = 0, token = strsep(&value_str, separator); token != NULL; token_num++, token = strsep(&value_str, separator)) {
			last_token = token;
			if(is_array_exist && token_num < array_size) {
				char *endptr = NULL;
				array[token_num] = strtol_default(token, &endptr, 10, default_value);
			}
		}
		if(last_token != NULL && strlen(last_token) == 0) {
			token_num--; //tailing zero-length token is not a real token.
		}
	}

	if(is_array_exist) {
		for(i = token_num; i < array_size; i++) {
			array[i] = default_value;
		}
	}

	return token_num;
}


int cms_get_uint_array(const char *format, unsigned int default_value, int array[], size_t array_size, char *separator, ...)
{
	int i = 0;
	int token_num = 0;

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	int is_array_exist = 0;
	if(array != NULL && array_size != 0) {
		is_array_exist = 1;
	}

	if( value_str != NULL) {
		char *token = NULL , *last_token = NULL;
		for(token_num = 0, token = strsep(&value_str, separator); token != NULL; token_num++, token = strsep(&value_str, separator)) {
			last_token = token;
			if(is_array_exist && token_num < array_size) {
				char *endptr = NULL;
				array[token_num] = strtoul_default(token, &endptr, 10, default_value);
			}
		}
		if(last_token != NULL && strlen(last_token) == 0) {
			token_num--; //tailing zero-length token is not a real token.
		}
	}

	if(is_array_exist) {
		for(i = token_num; i < array_size; i++) {
			array[i] = default_value;
		}
	}

	return token_num;
}


int cms_get_str_array(const char *format, char *default_value, const char *array[], size_t array_size, char *get_buffer, size_t get_buffer_size, char *separator, ...)
{
	int i = 0;
	int token_num = 0;

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	int is_array_exist = 0;
	if(array != NULL && array_size != 0 && get_buffer != NULL && get_buffer_size != 0 ) {
		is_array_exist = 1;
	}

	if( value_str != NULL) {
		if(get_buffer != NULL && get_buffer_size != 0) {
			snprintf(get_buffer, get_buffer_size , "%s", value_str);
		} else {
			get_buffer = value_str;
			get_buffer_size = BUFFER_MAX;
		}

		char *token = NULL , *last_token = NULL;
		for(token_num = 0, token = strsep(&get_buffer, separator); token != NULL; token_num++, token = strsep(&get_buffer, separator)) {
			last_token = token;
			if(is_array_exist && token_num < array_size) {
				array[token_num] = token;
			}
		}
		if(last_token != NULL && strlen(last_token) == 0) {
			token_num--; //tailing zero-length token is not a real token.
		}
	}

	if(is_array_exist) {
		for(i = token_num; i < array_size; i++) {
			array[i] = default_value;
		}
	}

	return token_num;
}
/***************************************
 * cms set helper
 **************************************/
static int cms_set_str_valist(const char *format, const char *value, va_list args)
{
	if(format == NULL) {
		return 0;
	}

	if(value == NULL) {
		value = "";
	}

	char name[MaxStringLen] = {0};
	return CMSSetValue( (char *)format_string_valist(name, sizeof(name), format, args), (char *)value, SetValueUser);
}


int cms_set_int(const char *format, const int value, ...)
{
	int changed = 0;
	char value_str[BUFFER_MAX] = {0};

	va_list args;
	va_start(args, value);
	changed = cms_set_str_valist(format, format_string(value_str, sizeof(value_str), "%d", value), args);
	va_end(args);
	return changed;
}


int cms_set_uint(const char *format, const unsigned int value, ...)
{
	int changed = 0;
	char value_str[BUFFER_MAX] = {0};

	va_list args;
	va_start(args, value);
	changed = cms_set_str_valist(format, format_string(value_str, sizeof(value_str), "%u", value), args);
	va_end(args);
	return changed;
}


int cms_set_str(const char *format, const char *value, ...)
{
	int changed = 0;

	va_list args;
	va_start(args, value);
	changed = cms_set_str_valist(format, value, args);
	va_end(args);
	return changed;
}

int cms_set_str_index(const char *format, const char *value, unsigned int index, char *separator, ...)
{
	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_valist(format, "", args);
	va_end(args);

	char new_value_str[BUFFER_MAX] = {0};
	int len = 0;
	len += snprintf(new_value_str + len, sizeof(new_value_str) - len, "%s", value_str);

	fprintf(stderr, "new_value_str=%s\n", new_value_str);

	int i = 0;
	char *value_str_start = value_str;
	char *token = NULL , *last_token = NULL;
	for(i = 0, token = strsep(&value_str, separator); token != NULL; i++, token = strsep(&value_str, separator)) {
		last_token = token;
		if(i  == index) {
			//found
			char *left_tokens = value_str;
			if(left_tokens == NULL) {
				left_tokens = "";
			}

			len = (int)(token - value_str_start);
			fprintf(stderr, "len=%d\n", len);
			if(value == NULL) {
				//delete mode
				snprintf(new_value_str + len, sizeof(new_value_str) - len, "%s", left_tokens);
				fprintf(stderr, "delete: new_value_str=%s\n", new_value_str);
			} else {
				//update mode
				snprintf(new_value_str + len, sizeof(new_value_str) - len, "%s%.1s%s", value, separator, left_tokens);
				fprintf(stderr, "update: new_value_str=%s\n", new_value_str);

			}
			break;
		}
	}

	if(last_token != NULL && strlen(last_token) == 0) {
		i--; //tailing zero-length token is not a real token.
	}

	if(token == NULL) { // not found in index, add more
		// fixed: tail token must be with a separator
		if(len > 0 && new_value_str[len - 1] != separator[0] ) {
			len += snprintf(new_value_str + len, sizeof(new_value_str) - len, "%.1s", separator);
		}

		for(; i <= index; i++) {
			if(i == index) {
				len += snprintf(new_value_str + len, sizeof(new_value_str) - len, "%s%.1s", value, separator);
			} else {
				len += snprintf(new_value_str + len, sizeof(new_value_str) - len, "%.1s", separator);
			}
		}
	}

	fprintf(stderr, "new_value_str=%s\n", new_value_str);
}




int cms_set_int_array(const char *format, int array[], size_t array_size, const char *separator, const char *tail_separator, ...)
{
	if(array == NULL || array_size == 0 || separator == NULL) {
		return 0;	//nothing to set and no changed
	}

	//concatenate string
	const char *sep[2] = {"", separator};
	char value_str[BUFFER_MAX] = {0};
	int  value_str_len = 0;
	int i = 0;
	for(i = 0; i < array_size; i++) {
		value_str_len += snprintf( value_str + value_str_len, sizeof(value_str) - value_str_len, "%.1s%d", sep[!!i] , array[i] );
	}
	if(tail_separator) {
		value_str_len += snprintf( value_str + value_str_len, sizeof(value_str) - value_str_len, "%.1s", tail_separator );
	}

	int changed = 0;
	va_list args;
	va_start(args, tail_separator);
	changed = cms_set_str_valist(format, value_str, args);
	va_end(args);
	return changed;
}


int cms_set_uint_array(const char *format, int array[], size_t array_size, const char *separator, const char *tail_separator, ...)
{
	if(array == NULL || array_size == 0 || separator == NULL) {
		return 0;	//nothing to set and no changed
	}

	//concatenate string
	const char *sep[2] = {"", separator};
	char value_str[BUFFER_MAX] = {0};
	int  value_str_len = 0;
	int i = 0;
	for(i = 0; i < array_size; i++) {
		value_str_len += snprintf( value_str + value_str_len, sizeof(value_str) - value_str_len, "%.1s%u", sep[!!i] , array[i] );
	}
	if(tail_separator) {
		value_str_len += snprintf( value_str + value_str_len, sizeof(value_str) - value_str_len, "%.1s", tail_separator );
	}

	int changed = 0;
	va_list args;
	va_start(args, tail_separator);
	changed = cms_set_str_valist(format, value_str, args);
	va_end(args);
	return changed;
}


int cms_set_str_array(const char *format, const char *array[], size_t array_size, const char *separator, const char *tail_separator, ...)
{
	if(array == NULL || array_size == 0 || separator == NULL) {
		return 0;	//nothing to set and no changed
	}

	//concatenate string
	const char *sep[2] = {"", separator};
	char value_str[BUFFER_MAX] = {0};
	int  value_str_len = 0;
	int i = 0;
	for(i = 0; i < array_size; i++) {
		const char *token = array[i];
		if(token == NULL) {
			token = "";
		}
		value_str_len += snprintf( value_str + value_str_len, sizeof(value_str) - value_str_len, "%.1s%s", sep[!!i] , token );
	}
	if(tail_separator) {
		value_str_len += snprintf( value_str + value_str_len, sizeof(value_str) - value_str_len, "%.1s", tail_separator );
	}

	int changed = 0;
	va_list args;
	va_start(args, tail_separator);
	changed = cms_set_str_valist(format, value_str, args);
	va_end(args);
	return changed;
}
