/*
  skeleton.c

  > mingw32-make -f makefile.tdmgcc64
*/

#define UNICODE
#include <windows.h>

#include <stdio.h>

#define LIBCURL "libcurl"
#define N_CURL_FREE "curl_free"
#define N_CURL_GETENV "curl_getenv"
#define N_CURL_VERSION "curl_version"

typedef int WINAPI (*LP_curl_free)(void *);
typedef char * WINAPI (*LP_curl_getenv)(char *);
typedef char * WINAPI (*LP_curl_version)();

LP_curl_free curl_free = NULL;
LP_curl_getenv curl_getenv = NULL;
LP_curl_version curl_version = NULL;

char *ENV_KEYS[] = {"dummy", "tmp", "Temp", "tz"};

int curl()
{
  HMODULE m = (HMODULE)LoadLibraryA(LIBCURL);
  printf("%08x\n", m);
  if(m){
    int i;

    printf("loaded: %s\n", LIBCURL);
    curl_free = (LP_curl_free)GetProcAddress(m, N_CURL_FREE);
    if(!curl_free){ fprintf(stderr, "error: %s\n", N_CURL_FREE); }
    curl_getenv = (LP_curl_getenv)GetProcAddress(m, N_CURL_GETENV);
    if(!curl_getenv){ fprintf(stderr, "error: %s\n", N_CURL_GETENV); }
    curl_version = (LP_curl_version)GetProcAddress(m, N_CURL_VERSION);
    if(!curl_version){ fprintf(stderr, "error: %s\n", N_CURL_VERSION); }

    printf("[%s]\n", curl_version());
    for(i = 0; i < sizeof(ENV_KEYS) / sizeof(ENV_KEYS[0]); ++i){
      char *v = curl_getenv(ENV_KEYS[i]);
      printf("[%s] = [%s]\n", ENV_KEYS[i], v ? v : "-undefined-");
      if(v) curl_free(v);
    }
    FreeLibrary(m);
  }
}

int main(int ac, char **av)
{
  int i;
  printf("sizeof(size_t): %08x\n", sizeof(size_t));
  for(i = 0; i < ac; ++i)
    printf("%d %s\n", i, av[i]);
  curl();
  return 0;
}
