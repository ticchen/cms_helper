#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "cms.h"

extern char **environ;

void env_dump()
{
	int i = 0;

	if(environ == NULL) {
		return;
	}

	while( environ[i] != NULL ) {
		printf("%s\r\n", environ[i]);
		i++;
	}

	return;
}


int cms_init()
{
	static cms_init_flag = 0;

	if(cms_init_flag) {
		return 0; //init already
	}

	//flush origin env
	clearenv();
	cms_init_flag = 1;
	return 0;
}

void cms_config_import(char *config_file)
{
	if(config_file == NULL) {
		return;
	}

	FILE *fp = fopen(config_file, "r");
	if(fp == NULL) {
		return;
	}

	char buff[80] = {0};
	char *next = NULL;
	char *start = NULL;
	while( (start = fgets(buff, sizeof(buff), fp)) != NULL) {
		if(start[0] == '#') {
			continue;
		}
		start = strsep(&start, "\n");	//remove tailing newline

		//seperate =
		next = start;
		start = strsep(&next, "=");

		//printf("%s=%s\n",start,next);
		setenv(start, next, 1);
	}

	fclose(fp);
}

char *cms_getenv(const char *name)
{
	static value_buff[BUFFER_MAX];

	cms_init();

	if( name == NULL ) {
		return NULL;
	}

	char *value = getenv(name);
	if(value == NULL) {
		return NULL;
	}

	if(strlen(value) == 0) {
		return NULL;	//treat empty string as NULL
	}

	memset(value_buff, 0x00, BUFFER_MAX);
	strncpy(value_buff, value, BUFFER_MAX - 1);
	return value_buff;
}

int cms_setenv(const char *name, const char *value)
{
	cms_init();

	if( name == NULL || value == NULL) {
		return -1;
	}

	char *origin_value = getenv(name);
	int ret = 0;
	if( origin_value && strcmp(origin_value, value) == 0 ) {
		ret = 0; //value not change
	} else {
		ret = 1; //value changed
	}
	setenv(name, value, 1);

	return ret;
}
