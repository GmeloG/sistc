/*
 * Jorge Manuel Estrela da Silva, 2023
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *json_find_next_object(const char *data, int offset)
{
  char *b = strchr(data, '{');
  if(b==NULL)
    return b;
  char *e = strchr(b, '}');
  if(e==NULL)
    return e;
      
  int n = e-b+1;
  char *s = (char *) malloc(n+1);
  memcpy(s,b,n);
  s[n] = 0;
  
  return s;
};


char *json_get_string(const char *json, const char *key){
  char *p,*p2;
  int n;
  
  p = strstr(json, key);
  if(p == NULL)
    return NULL;
    
  p = strchr(p, ':');
  p = strchr(p, '\"');
  ++p;
  p2 = strchr(p, '\"');
  n = p2-p;
  
  char *s = (char *) malloc(n+1);
  memcpy(s,p,n);
  s[n] = 0;
    
  return s; 
}

void json_for_each(const char *json_array, void (*func)(const char *))
{
  char *pjson_obj;
  int bytes_parsed=0;
  while(1) {
    pjson_obj = json_find_next_object(json_array+bytes_parsed, bytes_parsed);
    if(pjson_obj==NULL)
      break;
    else  
      bytes_parsed += strlen(pjson_obj);
    
    func(pjson_obj);
        
    free(pjson_obj);
  }  
}
