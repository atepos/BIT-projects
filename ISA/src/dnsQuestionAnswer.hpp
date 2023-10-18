/*
 * Autor: Petr Kaška
 * Datum: 2. října 2023
 */
#ifndef DNSQA_H
#define DNSQA_H

#include <stdint.h>

using namespace std;

typedef struct DNS_Qs
{
    uint16_t Type;
    uint16_t Class;
    
}DNS_Q;

typedef struct DNS_As
{
    uint16_t Type;
    uint32_t Class;
    uint16_t Ttl;
    uint16_t RdLength;
}DNS_A;

typedef union DNS_resources
{
    DNS_A ans; //answer
    DNS_Q q; //question
}DNS_resource;


typedef struct SOA_s
{
     uint32_t Serial;
     uint32_t Refresh;
     uint32_t Retry;
     uint32_t Expire; 
     uint32_t Minimum;
}SOA_;


struct Mapping
{
    int type;
    std::string name;
};

const Mapping qTypeMappings[] = 
{
    {1, "A"},
    {2, "NS"},
    {5, "CNAME"},
    {6, "SOA"},
    {11, "WKS"},
    {12, "PTR"},
    {15, "MX"},
    {33, "SRV"},
    {28, "AAAA"},
    {255, "ANY"}
};

const Mapping aTypeMappinfs[] = 
{
    {1, "A"},
    {2, "NS"},
    {3, "MD"},
    {4, "MF"},
    {5, "CNAME"},
    {6, "SOA"},
    {7, "MB"},
    {8, "MG"},
    {9, "MR"},
    {10, "NULL"},
    {11, "WKS"},
    {12, "PTR"},
    {13, "HINFO"},
    {14, "MINFO"},
    {15, "MX"},
    {16, "TXT"},
    {28, "AAAA"},
    {33, "SRV"}
};

const Mapping qClassMappings[] =
{
    {1, "IN"},
    {2, "CS"},
    {3, "CH"},
    {4, "HS"}
};



#endif