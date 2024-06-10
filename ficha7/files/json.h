/*
 * Jorge Manuel Estrela da Silva, 2023
 */
 
#ifndef INCLUDE_SISTC_JSON_H
#define INCLUDE_SISTC_JSON_H

char *json_get_string(const char *json_object, const char *key);
char *json_find_next_object(const char *json_data, int offset);
void json_for_each(const char *json_array, void (*process)(const char *));

#endif
