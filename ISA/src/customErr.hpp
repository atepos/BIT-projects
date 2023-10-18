/*
 * Autor: Petr Kaška
 * Datum: 2. října 2023
 */
#ifndef ERR
#define ERR

#define ERROR_EXIT 1
#define OK_EXIT 0

namespace err
{
    bool errHandle(const char* message);
    void errHandleReturn(const int code);
    enum
    {
        NOERROR = 0,
        FORMERR,
        SERVFAIL,
        NXDOMAIN,
        NOTIMP,
        REFUSED,
        YXDOMAIN,
        XRRSET,
        NOTAUTH,
        NOTZONE
    };
}

#endif //ERR