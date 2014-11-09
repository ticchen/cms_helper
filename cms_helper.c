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
long int strtol_default(const char *nptr, char **endptr, int base, long int default_value)
{
	if(nptr == NULL){
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
	if(nptr == NULL){
		return default_value;
	}

	errno = 0;
	char *neg_sign=strchr(nptr,'-');
	if(neg_sign){
		//found a negative sign
		if( (unsigned long int)(neg_sign-nptr)+1 == strcspn(nptr,"1234567890") ){
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


static char *cms_get_str_valist(const char *format, char *default_value, va_list args)
{
	if(format == NULL){
		return default_value;
	}
	
	const char *name = format;
	if(strchr(format,'%') != NULL){
		name=alloca(MaxStringLen);
		int len = vsnprintf((char *)name, MaxStringLen, format, args);
		if(len < 0 || MaxStringLen <= len){ \
			return default_value;
		} 
	}
	
	char *value_str=CMSGetValue(name,GetValueUser);
	if(value_str == NULL){
		return default_value;
	}
	
	return value_str;
}


char *cms_get_str(const char *format, char *default_value, ...)
{
	if(format == NULL){
		return default_value;
	}

	va_list args;
	va_start(args, default_value);
	char *value_str = cms_get_str_valist(format, default_value, args);
	va_end(args);
	return value_str;
}	


int cms_get_int(const char *format, int default_value, ...)
{
	if(format == NULL){
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
	if(format == NULL){
		return default_value;
	}

	va_list args;
	va_start(args, default_value);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	char *endptr = NULL;
	return strtoul_default(value_str, &endptr, 10, default_value);
}



static char *cms_get_str_index_valist(const char *format, char *default_value, unsigned int index, char *separator, va_list args)
{
	if(format == NULL || separator == NULL){
		return default_value;
	}

	char *value_str = cms_get_str_valist(format, NULL, args);
	if(value_str == NULL){
		return default_value;
	}

	unsigned int i=0;
	char *token=NULL;
	for(i = 0, token = strsep(&value_str, separator); i <= index && token != NULL; i++, token = strsep(&value_str, separator)){
		if(i == index){
			return token;
		}
	}
	return default_value;
}


char *cms_get_str_index(const char *format, char *default_value,unsigned int index, char *separator, ...)
{	
	if(format == NULL || separator == NULL){
		return default_value;
	}

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_index_valist(format, NULL, index, separator, args);
	va_end(args);

	return value_str;
}


int cms_get_int_index(const char *format, int default_value, unsigned int index, char *separator, ...)
{
	if(format == NULL || separator == NULL){
		return default_value;
	}

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_index_valist(format, NULL, index, separator, args);
	va_end(args);

	char *endptr=NULL;
	return strtol_default(value_str, &endptr, 10, default_value);
}


unsigned int cms_get_uint_index(const char *format, unsigned int default_value, unsigned int index, char *separator, ...)
{
	if(format == NULL || separator == NULL){
		return default_value;
	}

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_index_valist(format, NULL, index, separator, args);
	va_end(args);

	char *endptr=NULL;
	return strtoul_default(value_str, &endptr, 10, default_value);
}


int cms_get_str_array(const char *format, char *default_value, const char *array[], size_t array_size, char *get_buffer, size_t get_buffer_size, char *separator, ...)
{
	int i=0;
	int token_num=0;

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	int is_array_exist=0;
	if(array != NULL && array_size !=0 && get_buffer != NULL && get_buffer_size !=0 ){
		is_array_exist=1;
	}

	if( value_str != NULL){
		if(get_buffer !=NULL && get_buffer_size != 0){
			snprintf(get_buffer, get_buffer_size , "%s", value_str);
		}else{
			get_buffer = value_str;
			get_buffer_size = BUFFER_MAX;
		}
		
		char *token=NULL ,*last_token = NULL;
		for(token_num=0, token = strsep(&get_buffer, separator); token != NULL; token_num++, token = strsep(&get_buffer, separator)){
			last_token = token;
			if(is_array_exist && token_num<array_size){
				array[token_num] = token;
			}
		}
		if(last_token != NULL && strlen(last_token) == 0){
			token_num--; //tailing zero-length token is not a real token. 
		}
	}

	if(is_array_exist){
		for(i=token_num;i<array_size;i++){
			array[i] = default_value;
		}
	}

	return token_num;
}


int cms_get_int_array(const char *format, int default_value, int array[], size_t array_size, char *separator, ...)
{
	int i=0;
	int token_num=0;

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	int is_array_exist=0;
	if(array != NULL && array_size !=0){
		is_array_exist=1;
	}

	if( value_str != NULL){
		char *token=NULL ,*last_token = NULL;
		for(token_num=0, token = strsep(&value_str, separator); token != NULL; token_num++, token = strsep(&value_str, separator)){
			last_token = token;
			if(is_array_exist && token_num<array_size){
				char *endptr = NULL;
				array[token_num] = strtol_default(token,&endptr, 10, default_value);
			}
		}
		if(last_token != NULL && strlen(last_token) == 0){
			token_num--; //tailing zero-length token is not a real token. 
		}
	}

	if(is_array_exist){
		for(i=token_num;i<array_size;i++){
			array[i] = default_value;
		}
	}

	return token_num;
}

int cms_get_uint_array(const char *format, unsigned int default_value, int array[], size_t array_size, char *separator, ...)
{
	int i=0;
	int token_num=0;

	va_list args;
	va_start(args, separator);
	char *value_str = cms_get_str_valist(format, NULL, args);
	va_end(args);

	int is_array_exist=0;
	if(array != NULL && array_size !=0){
		is_array_exist=1;
	}

	if( value_str != NULL){
		char *token=NULL ,*last_token = NULL;
		for(token_num=0, token = strsep(&value_str, separator); token != NULL; token_num++, token = strsep(&value_str, separator)){
			last_token = token;
			if(is_array_exist && token_num<array_size){
				char *endptr = NULL;
				array[token_num] = strtoul_default(token,&endptr, 10, default_value);
			}
		}
		if(last_token != NULL && strlen(last_token) == 0){
			token_num--; //tailing zero-length token is not a real token. 
		}
	}

	if(is_array_exist){
		for(i=token_num;i<array_size;i++){
			array[i] = default_value;
		}
	}

	return token_num;
}
