/**
 * @thiedparty file
 */
 
#ifndef __INI_FILE_H__
#define __INI_FILE_H__

#ifdef __cplusplus
extern "C"
{
#endif

int read_profile_string( const char *section, const char *key,char *value, int size,const char *default_value, const char *file);
int read_profile_int( const char *section, const char *key,int default_value, const char *file);
int write_profile_string( const char *section, const char *key,const char *value, const char *file);

int   read_inistring_string(const char* aIniBuffer,const char *aSection, const char *aKey,char *aValue, unsigned int aValueLength, const char *aDefaultValue);
int   read_inistring_int(const char* aIniBuffer,const char *aSection, const char *aKey, int aDefaultValue);

#ifdef __cplusplus
}; //end of extern "C" {
#endif

#endif //end of INI_FILE_H_

