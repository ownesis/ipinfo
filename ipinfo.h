#ifndef __IP_INFO_H__
#define __IP_INFO_H__

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

enum _E_IPInfo_ {
    IPINFO_OK = 0,
    IPINFO_MEM_ERR,
    IPINFO_JSON_ERR,
    IPINFO_CURL_ERR,
    IPINFO_ENDPOINT_ERR,
    IPINFO_IP_ERR
};

int ipinfo_get(struct IPInfo **info, const char *ipstr);
void ipinfo_free(struct IPInfo *info);

#endif /* __IP_INFO_H__ */
