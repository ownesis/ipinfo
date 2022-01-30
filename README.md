# IPInfo
IPInfo is a C library for https://ipapi.co/

# Installation
```bash
git clone https://github.com/ownesis/ipinfo
cd ipinfo
mkdir build && cd build/
cmake .. && make
sudo make install
sudo ldconfig /usr/local/lib
```

# Documentation
First, you need to initialize a NULL `struct IPInfo` pointer context.

`struct IPInfo` is:
```c
struct IPInfo {
    char *ip;
    char *version;
    char *city;
    char *region;
    char *region_code;
    char *country;
    char *country_name;
    char *country_code;
    char *country_code_iso3;
    char *country_capital;
    char *country_tld;
    char *continent_code;
    char *postal;
    double latitude;
    double longitude;
    char *timezone;
    char *utc_offset;
    char *country_calling_code;
    char *currency;
    char *currency_name;
    char *languages;
    double country_area;
    size_t country_population;
    char *asn;
    char *org;
    _Bool in_eu;
};
```

After initilization of the context, you can call the function `ipinfo_get`:
```c
int ipinfo_get(struct IPInfo **info, const char *ipstr);
```
This function fill the context with IP address information. 

This functiontake `2` arguments:

- `info`: it's an **output** argument, it take address of `struct IPInfo` pointer context initilized to `NULL`.
- `ipstr`: it's an **input** argument, it take a string format of an IP address (like "8.8.8.8" or "2001:4860:4860::8888"), if is `NULL` it use the IP address of the caller.

The function return `IPINFO_OK` if no error, else:

- `IPINFO_MEM_ERR`: If an error occurred when allocate memory.
- `IPINFO_JSON_ERR`: If an error occured when parsing json result of the api endpoit.
- `IPINFO_CURL_ERR`: If an error occured with curl api.
- `IPINFO_ENDPOINT_ERR`: If the api endpoint `ipapi.co` return response code different to `200`.

After using data, you must to free the context with `ipinfo_free`:
```c
void ipinfo_free(struct IPInfo *info);
```
This function take only one argument:

- `info`: it's an **input** argument, it the `struct IPInfo` context pointer.

# Example
```c
#include <stdio.h>
#include <ipinfo.h>

int main(int argc, char *argv[]) {
    int res;
    struct IPInfo *info = NULL;

    res = ipinfo_get(&info, (argc >= 2) ? argv[1] : NULL);

    if (res != IPINFO_OK) {
        fprintf(stderr, "An error occurred\n");
        return 1; 
    }

    printf("IP (%s): %s\n", info->version, info->ip);
    printf("Location: %s (%s), %s (%s), %s (%s)\n", 
            info->city,
            info->postal,
            info->region,
            info->region_code,
            info->country,
            info->country_code);

    ipinfo_free(info);

    return 0;
}
```

# Compilation
`gcc -o show_info example_code.c -lipinfo`
