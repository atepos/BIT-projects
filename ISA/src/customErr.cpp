/*
 * Autor: Petr Kaška
 * Datum: 2. října 2023
 */
#include <iostream>

#include "customErr.hpp"

using namespace std;

namespace err
{
    bool errHandle(const char* message)
    {
            std::cout << message << std::endl;
            exit(ERROR_EXIT);
    }

    void errHandleReturn(const int code)
    {
        switch (code)
        {
            case FORMERR:
                std::cout << "Error: DNS Query Format" << std::endl;
                break;
            case SERVFAIL:
                std::cout << "Error: Server failed to complete the DNS request" << std::endl;
                break;
            case NXDOMAIN:
                std::cout << "Error: Domain name does not exist" << std::endl;
                break;
            case NOTIMP: 
                std::cout << "Error: Function not implemented" << std::endl;
                break;
            case REFUSED:
                std::cout << "Error: The server refused to answer for the query" << std::endl;
                break;
            case YXDOMAIN:
                std::cout << "Error: Name that should not exist, does exist" << std::endl;
                break;
            case XRRSET:
                std::cout << "Error: RR set that should not exist, does exist" << std::endl;
                break;
            case NOTAUTH:
                std::cout << "Error: Server not authoritative for the zone" << std::endl;
                break;
            case NOTZONE:
                std::cout << "Error: Name not in zone" << std::endl;
                break;
            default:
                break;
        }
        exit(ERROR_EXIT);
    }
}