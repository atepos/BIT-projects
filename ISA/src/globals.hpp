/*
 * Autor: Petr Kaška
 * Datum: 2. října 2023
 */
#ifndef GLOBALS_H
#define GLOBALS_H

using namespace std;

enum
{
    A = 1,
    NS,
    MD,
    MF,
    CNAME,
    SOA,
    MB,
    MG,
    MR,
    NULL_R,
    WKS,
    PTR,
    HINFO,
    MINFO,
    MX,
    TXT,
    AAAA = 28,
    SRV = 33
};

enum
{
    IN = 1,
    CS,
    CH,
    HS,
};

#define SPACE ' '
#define DELIMETER '.'
#define ELEMENT_DELIMITER ", "
#define WAIT 10
#define MAX_SIZE 512
#define MASK 15 // 1111
#define FOUR_BIT_SHIFT 4

#endif //GLOBALS_H