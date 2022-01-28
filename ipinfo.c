#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <json-c/json.h>
#include "ipinfo.h"

#define FREE_IF_NONULL(p) if ((p)) free((p))

#define CURL_ERR(f) if ((f) != CURLE_OK) 

#define JSON_GET_STR(j, k, s)                               \
    do {                                                    \
        json_object *_tmp = NULL;                           \
        if (!json_object_object_get_ex((j), #k, &_tmp)) {   \
            free((s));                                      \
            json_object_put((j));                           \
            return IPINFO_JSON_ERR;                         \
        }                                                   \
        (s)->k = strdup(json_object_get_string(_tmp));      \
        if (!(s)->k) {                                      \
            free((s));                                      \
            json_object_put((j));                           \
            return IPINFO_MEM_ERR;                          \
        }                                                   \
    } while (0)

#define JSON_GET_TYPE(t, j, k, s)                           \
    do {                                                    \
        json_object *_tmp = NULL;                           \
        if (!json_object_object_get_ex((j), #k, &_tmp)) {   \
            free((s));                                      \
            json_object_put((j));                           \
            return IPINFO_MEM_ERR;                          \
        }                                                   \
        (s)->k = json_object_get_##t(_tmp);                 \
    } while (0)

#define API_ENDPOINT "https://ipapi.co/"
#define API_ENDPOINT_TYPE "/json"

struct Mem {
    char *ptr;
    size_t size;
};

static size_t _write_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    size_t total_size = (size * nmemb);
    struct Mem *chunk = (struct Mem *)userdata;
    char *tmp = realloc(chunk->ptr, chunk->size+total_size+1);

    if (!tmp)
        return 0;

    chunk->ptr = tmp;
    memcpy(chunk->ptr+chunk->size, ptr, total_size);
    chunk->size += total_size;

    return total_size;
}

void ipinfo_free(struct IPInfo *info) {
    if (!info)
        return;

    FREE_IF_NONULL(info->ip);
    FREE_IF_NONULL(info->version);
    FREE_IF_NONULL(info->city);
    FREE_IF_NONULL(info->region);
    FREE_IF_NONULL(info->region_code);
    FREE_IF_NONULL(info->country);
    FREE_IF_NONULL(info->country_name);
    FREE_IF_NONULL(info->country_code);
    FREE_IF_NONULL(info->country_code_iso3);
    FREE_IF_NONULL(info->country_capital);
    FREE_IF_NONULL(info->country_tld);
    FREE_IF_NONULL(info->continent_code);
    FREE_IF_NONULL(info->postal);
    FREE_IF_NONULL(info->timezone);
    FREE_IF_NONULL(info->utc_offset);
    FREE_IF_NONULL(info->country_calling_code);
    FREE_IF_NONULL(info->currency);
    FREE_IF_NONULL(info->currency_name);
    FREE_IF_NONULL(info->languages);
    FREE_IF_NONULL(info->asn);
    FREE_IF_NONULL(info->org);

    free(info);
}

int ipinfo_get(struct IPInfo **info, const char *ipaddr) {
    CURL *curl_handle;
    CURLcode res;
    struct Mem chunk;
    struct json_object *jobj;
    struct json_object *loc_obj;
    long code = 0;
    char *endpoint = NULL;
    size_t ipaddr_len = 0;

    if (ipaddr) {
        ipaddr_len = strlen(ipaddr);
    } else {
        ipaddr_len = 0;
        ipaddr = "";
    }

    endpoint = calloc(1, (sizeof(API_ENDPOINT)
                    + sizeof(API_ENDPOINT_TYPE)
                    + ipaddr_len
                    + 1));

    if (!endpoint)
        return IPINFO_MEM_ERR;

    if (!strcat(endpoint, API_ENDPOINT))
        return IPINFO_MEM_ERR;

    if (!strcat(endpoint, ipaddr))
        return IPINFO_MEM_ERR;
 
    if (!strcat(endpoint, API_ENDPOINT_TYPE))
        return IPINFO_MEM_ERR;
 
    *info = calloc(1, sizeof(struct IPInfo));
    if (!(*info))
        return IPINFO_MEM_ERR;

    chunk.ptr = calloc(1, 1);

    if (!chunk.ptr) {
        free(*info);

        return IPINFO_MEM_ERR;
    }

    chunk.size = 0;

    curl_handle = curl_easy_init();

    if (!curl_handle) {
        free(*info);
        free(chunk.ptr);

        return IPINFO_CURL_ERR;
    }

    CURL_ERR(curl_easy_setopt(curl_handle, CURLOPT_URL, endpoint)) {
        free(*info);
        free(chunk.ptr);
        curl_easy_cleanup(curl_handle);

        return IPINFO_CURL_ERR;
    }

    free(endpoint);
 
    CURL_ERR(curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "ipinfo")) {
        free(*info);
        free(chunk.ptr);

        return IPINFO_CURL_ERR;
    }

    CURL_ERR(curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, _write_callback)) {
        free(*info);
        free(chunk.ptr);
        curl_easy_cleanup(curl_handle);

        return IPINFO_CURL_ERR;
    }
    
    CURL_ERR(curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk)) {
        free(*info);
        free(chunk.ptr);
        curl_easy_cleanup(curl_handle);
        
        return IPINFO_CURL_ERR;
    }

    CURL_ERR(curl_easy_perform(curl_handle)) {
        free(*info);
        free(chunk.ptr);
        curl_easy_cleanup(curl_handle);

        return IPINFO_CURL_ERR;
    }

    CURL_ERR(curl_easy_getinfo(curl_handle, CURLINFO_RESPONSE_CODE, &code)) {
        free(*info);
        free(chunk.ptr);
        curl_easy_cleanup(curl_handle);

        return IPINFO_CURL_ERR;
    }

    curl_easy_cleanup(curl_handle);

    if (code != 200) {
        free(*info);
        free(chunk.ptr);

        return IPINFO_CURL_ERR;
    }

    chunk.ptr[chunk.size] = 0;

    jobj = json_tokener_parse(chunk.ptr);

    free(chunk.ptr);

    if (!jobj) {
        free(*info);

        return IPINFO_JSON_ERR;
    }

    JSON_GET_STR(jobj, ip, *info);
    JSON_GET_STR(jobj, version, *info);
    JSON_GET_STR(jobj, city, *info);
    JSON_GET_STR(jobj, region, *info);
    JSON_GET_STR(jobj, region_code, *info);
    JSON_GET_STR(jobj, country, *info);
    JSON_GET_STR(jobj, country_name, *info);
    JSON_GET_STR(jobj, country_code, *info);
    JSON_GET_STR(jobj, country_code_iso3, *info);
    JSON_GET_STR(jobj, country_capital, *info);
    JSON_GET_STR(jobj, country_tld, *info);
    JSON_GET_STR(jobj, continent_code, *info);
    JSON_GET_STR(jobj, postal, *info);
    JSON_GET_STR(jobj, timezone, *info);
    JSON_GET_STR(jobj, utc_offset, *info);
    JSON_GET_STR(jobj, country_calling_code, *info);
    JSON_GET_STR(jobj, currency, *info);
    JSON_GET_STR(jobj, currency_name, *info);
    JSON_GET_STR(jobj, languages, *info);
    JSON_GET_STR(jobj, asn, *info);
    JSON_GET_STR(jobj, org, *info);

    JSON_GET_TYPE(int64, jobj, country_population, *info);

    JSON_GET_TYPE(double, jobj, latitude, *info);
    JSON_GET_TYPE(double, jobj, longitude, *info);
    JSON_GET_TYPE(double, jobj, country_area, *info);

    JSON_GET_TYPE(boolean, jobj, in_eu, *info);

    json_object_put(jobj);

    return IPINFO_OK;
}
