/*
  skeleton.c

  > mingw32-make -f makefile.tdmgcc64

  curl_easy_strerror(res)
    SSL connect error
    Couldn't resolve host name
*/

#ifndef UNICODE
#define UNICODE
#endif
#include <windows.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFFN_T(DCL, FN, RES, ...) \
typedef RES (DCL *LP_##FN)(__VA_ARGS__); \
LP_##FN FN = NULL;

#define LOADFNC(R, M, FN) do{ \
  FN = (LP_##FN)GetProcAddress(M, #FN); \
  if(!FN){ fprintf(stderr, "cannot find function: %s\n", #FN); R = 0; } \
}while(0)

#define LDFN(FN) LOADFNC(result, m, FN)

#define LIBBZ2 "libbz2"
#define LIBCURL "libcurl"
#define LIBEAY32 "libeay32" // "ssleay32"
#define LIBSQLITE3 "sqlite3"

typedef void CURL; // same in curl.h

typedef enum {
  CURLOPT_FOLLOWLOCATION = 52,
  CURLOPT_URL = 10002
} CURLoption; // fake enum in curl.h

typedef enum {
  CURLE_OK = 0
} CURLcode; // fake enum in curl.h

DEFFN_T(__cdecl, curl_free, int, void *)
DEFFN_T(__cdecl, curl_getenv, char *, char *)
DEFFN_T(__cdecl, curl_version, char *, void)
DEFFN_T(__cdecl, curl_easy_strerror, const char *, CURLcode)
DEFFN_T(__cdecl, curl_easy_cleanup, void, CURL *)
DEFFN_T(__cdecl, curl_easy_init, CURL *, void)
DEFFN_T(__cdecl, curl_easy_setopt, CURLcode, CURL *, CURLoption, ...)
DEFFN_T(__cdecl, curl_easy_perform, CURLcode, CURL *)

char *ENV_KEYS[] = {"dummy", "tmp", "Temp", "tz"};

#define URL_DUMMY "http://localhost:8080/dummy.html"

HMODULE load_func(char *libname)
{
  unsigned int result = 1;
  HMODULE m = (HMODULE)LoadLibraryA(libname);
  printf("%08x\n", m);
  if(!m) return NULL;
  printf("loaded: %s\n", libname);
  if(!strcmp(libname, LIBBZ2)){
    fprintf(stderr, "not implemented\n");
    result = 0;
  }else if(!strcmp(libname, LIBCURL)){
    LDFN(curl_free); LDFN(curl_getenv); LDFN(curl_version);
    LDFN(curl_easy_strerror); LDFN(curl_easy_cleanup); LDFN(curl_easy_init);
    LDFN(curl_easy_setopt); LDFN(curl_easy_perform);
  }else if(!strcmp(libname, LIBEAY32)){
    fprintf(stderr, "not implemented\n");
    result = 0;
  }else if(!strcmp(libname, LIBSQLITE3)){
    fprintf(stderr, "not implemented\n");
    result = 0;
  }else{
    fprintf(stderr, "undefined library\n");
    result = 0;
  }
  if(!result){
    fprintf(stderr, "unload: %s\n", libname);
    FreeLibrary(m);
    return NULL;
  }
  return m;
}

int test_curl()
{
  int i;
  CURL *curl;
  CURLcode res;
  printf("[%s]\n", curl_version());
  for(i = 0; i < sizeof(ENV_KEYS) / sizeof(ENV_KEYS[0]); ++i){
    char *v = curl_getenv(ENV_KEYS[i]);
    printf("[%s] = [%s]\n", ENV_KEYS[i], v ? v : "-undefined-");
    if(v) curl_free(v);
  }
  curl = curl_easy_init();
  if(curl){
    curl_easy_setopt(curl, CURLOPT_URL, URL_DUMMY);
    // curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L); // when redirected
    res = curl_easy_perform(curl);
    if(res != CURLE_OK)
      fprintf(stderr, "error: %s\n", curl_easy_strerror(res));
    curl_easy_cleanup(curl);
  }
  return 0;
}

int main(int ac, char **av)
{
  int i;
  HMODULE m = load_func(LIBCURL);
  if(!m) return 1;
  printf("sizeof(size_t): %08x\n", sizeof(size_t));
  for(i = 0; i < ac; ++i)
    printf("%d %s\n", i, av[i]);
  test_curl();
  FreeLibrary(m);
  return 0;
}
