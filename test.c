#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include "string.h"
// #include <time.h>

typedef struct ldap_config
{
    char *uri;

    char *binddn;
    char *bindpw;

    int version;
    int timeout;

    /* TLS/SSL */
    char *ssl;
    char *tls_cacertfile;
    char *tls_cacertdir;
    char *tls_certfile;
    char *tls_certkey;
    char *tls_ciphersuite;
    char *tls_reqcert;

} ldap_config_t;

char * ngx_get_format_time_t(char fmt[],char *nval)
{
    time_t rawtime;
    time(&rawtime);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    struct tm timeinfo;

    localtime_r(&rawtime, &timeinfo);
    strftime(nval, 32, fmt, &timeinfo);
    printf("%s\n", nval);
    return nval;
}

int main(void)
{
    char *year;
    year=malloc(30);
    memset(year,0,30);
    printf("%lu\n", sizeof(struct ldap_config));
    // printf("%s",
    ngx_get_format_time_t("%Y/%m/%d %T",year);
    // );

    printf("%s\n", year);
    free(year);
}