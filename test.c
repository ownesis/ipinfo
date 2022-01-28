#include <stdio.h>
#include "ipinfo.h"

int main(void) {

    struct IPInfo *info = NULL;

    ipinfo_get(&info);
    puts(info->ip);
    puts(info->version);
    puts(info->city);
    puts(info->region);
    puts(info->region_code);
    puts(info->country);
    puts(info->country_name);
    puts(info->country_code);
    puts(info->country_code_iso3);
    puts(info->country_capital);
    puts(info->country_tld);
    puts(info->continent_code);
    puts(info->postal);
    puts(info->timezone);
    puts(info->utc_offset);
    puts(info->country_calling_code);
    puts(info->currency);
    puts(info->currency_name);
    puts(info->languages);
    puts(info->asn);
    puts(info->org);
    printf("%f\n", info->latitude);
    printf("%f\n", info->longitude);
    printf("%f\n", info->country_area);
    printf("%lu", info->country_population);

    ipinfo_free(info);

    return 0;
}
