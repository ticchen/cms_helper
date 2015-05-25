#ifndef _CMS_H
#define _CMS_H


#define MaxStringLen 256	//max key size
#define BUFFER_MAX 1024		//max value size

#define CMSGetValue(name,type)			cms_getenv(name)
#define CMSSetValue(name,value,type)	cms_setenv(name,value)

char *cms_getenv(const char *name);
int cms_setenv(const char *name, const char *value);


#define SetValueUser 2
#define ReplaceValueUser 17

#endif //_CMS_H
