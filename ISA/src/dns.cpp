/*
 * Autor: Petr Kaška
 * Datum: 2. října 2023
 */
#include <cstdio>
#include <iostream>
#include <ostream>
#include <unistd.h>
#include <cstring>
#include <netdb.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cstdlib>  // For exit
#include <sys/types.h>
#include <sys/socket.h>

#include "customErr.hpp"
#include "dnsServerResponseHeader.hpp"
#include "dnsQuestionAnswer.hpp"
#include "globals.hpp"
#include "string.hpp"

 
using namespace std;

#define DEFAULT_PORT "53"
#define DELIMETER '.'


/**
 * @brief Class for parsing and storing command-line arguments for a DNS resolver.
 *
 * The `Arguments` class provides methods to parse and store command-line arguments for a DNS resolver program.
 * It allows specifying options for DNS queries, such as enabling recursion, reverse queries, IPv6 queries, and more.
 *
 */
class Arguments
{
    public:
        bool enableRecursion;
        bool enableReverseQuery;
        bool enableIPv6;
        bool enableMX;
        string serverPort;
        string queryAdress;
        string serverDns;
    /**
     * @brief Default constructor for the Arguments class.
     *
     * Initializes all member variables with default values.
     */
    Arguments()
    {
        this->enableReverseQuery = false;
        this->enableIPv6 = false;
        this->enableMX = false;
        this->serverPort = DEFAULT_PORT;
        this->enableRecursion = false;
        this->serverDns = "";
        this->queryAdress = "";
    }
    /**
     * @brief Parses command-line arguments and populates the DNS request configuration.
     *
      * @param argc The number of command-line arguments.
     * @param argv An array of C-style strings containing the command-line arguments.
     * @return A pointer to an `Arguments` object representing the DNS request configuration.
     */
    Arguments* parseArguments(
        int argc, 
        char** argv, 
        Arguments *args)
    {
        bool isDnsServer = false;
        bool isPort = false;
        char opt;

            while((opt = getopt(argc, argv, "m::r::x::6::s:p:h::")) != -1)
            {
                switch(opt)
                {
                    case 'x':
                        this->addReverseQuery(); break;
                    case 'r':
                        this->addRecursion(); break;
                    case '6':
                        this->addIPv6(); break;
                    case 's':
                        this->addServerDns(isDnsServer);
                        isDnsServer =  true; 
                        break;
                    case 'p':
                        this->addPort(isPort); break;
                    case 'h':
                        this->help(); break;
                    case 'm':
                        this->addMX(); break;
                    case '?':
                        err::errHandle("Error: bad arguments");
                }

            }
            countOfArguments(argc, isDnsServer);

        this->queryAdress = argv[optind];
        return args;
    }

    private:

        /**
         * @brief Enables MX queries.
         *
         * If MX queries are already enabled, throws a CustomException.
         */
        void addMX()
        {
            (this->enableMX || this->enableIPv6 || this->enableReverseQuery) ? err::errHandle("Error: bad arguments") : this->enableMX = true;
        }

        /**
         * @brief Verifies the count of command-line arguments.
         *
         * @param argc The number of command-line arguments.
         * @param isDnsServer Indicates if the DNS server argument has been provided.
         * @throw CustomException if the argument count is incorrect.
         */
        void countOfArguments(
            int argc, 
            bool isDnsServer)
        {
            ((optind + 1) != argc || !isDnsServer) ? err::errHandle("Error: bad arguments") : 0;
        }

        /**
         * @brief Enables recursion for DNS queries.
         *
         * If recursion is already enabled, throws a CustomException.
         */
        void addRecursion()
        {
            this->enableRecursion ? err::errHandle("Error: bad arguments") : this->enableRecursion = true; 
        }

        /**
         * @brief Enables reverse DNS queries.
         *
         * If reverse query is already enabled, throws a CustomException.
         */
        void addReverseQuery()
        {
            (this->enableMX || this->enableIPv6 || this->enableReverseQuery) ? err::errHandle("Error: bad arguments") : this->enableReverseQuery = true;
        }

        /**
         * @brief Enables IPv6 DNS queries.
         *
         * If IPv6 is already enabled, throws a CustomException.
         */
        void addIPv6()
        {
           (this->enableMX || this->enableIPv6 || this->enableReverseQuery) ? err::errHandle("Error: bad arguments") : this->enableIPv6 = true;
        }

        /**
         * @brief Sets the DNS server address.
         *
         * @param isDnsServer Indicates if the DNS server argument has been provided.
         * @throw CustomException if a DNS server address is already set.
         */
        void addServerDns(
            bool isDnsServer)
        {
            if(isDnsServer)
            {
                err::errHandle("Error: bad arguments");
            }
            else
            {
                this->serverDns = optarg;
            }
        }

        /**
         * @brief Sets the server port.
         *
         * @param isPort Indicates if the port argument has been provided.
         * @throw CustomException if a port is already set.
         */
        void addPort(
            bool isPort)
        {  
            if(isPort)
            {
                err::errHandle("Error: bad arguments");
            }
            else
            {
                this->serverPort = optarg;
            }
        }

        /**
         * @brief Displays a help message describing program usage and exits.
         */
        void help()
        {
            cout << "DNS resolver " << endl;
            cout << "Použití: dns [-r] [-x] [-6] -s server [-p port] adresa" << endl;
            cout << "Pořadí parametrů je libovolné. Popis parametrů:" << endl;
            cout << "-r: Požadována rekurze (Recursion Desired = 1), jinak bez rekurze." << endl;
            cout << "-x: Reverzní dotaz místo přímého." << endl;
            cout << "-6: Dotaz typu AAAA místo výchozího A." << endl;
            cout << "-s: IP adresa nebo doménové jméno serveru, kam se má zaslat dotaz." << endl;
            cout << "-m: Dotaz typu MX " << endl;
            cout << "-p port: Číslo portu, na který se má poslat dotaz, výchozí 53." << endl;
            cout << "adresa: Dotazovaná adresa." << endl;
            exit(OK_EXIT);
        }

};

/**
 * @class DNSrequest
 * @brief A class for sending DNS requests and receiving responses.
 */
class DNSrequest
{
    private:
        int Socket;
        /**
         * @brief Initializes the DNS socket.
         * @param addressInfoList The list of address information.
         * @return True if initialization fails, false otherwise.
         */
        bool initializeDnsSocket(
            addrinfo* addressInfoList)
        {
           return (this->Socket = socket(addressInfoList->ai_family, SOCK_DGRAM, 0)) == -1;
        }
        /**
         * @brief Connects the DNS socket to the server.
         * @param server The server address information.
         * @return True if connection fails, false otherwise.
         */
        bool connectDnsSocket(
            addrinfo *server)
        {
            return connect(this->Socket, server->ai_addr, server->ai_addrlen) == -1;
        }
        /**
         * @brief Resolves the server address using DNS and stores the result in 'result'.
         *
         * @param serverDNS The server's DNS address.
         * @param serverPort The server's port.
         * @param hints The address information hints.
         * @param result A pointer to the address information result.
         * @return True if the address resolution fails, false if successful.
         */
        bool resolveServerAddress(
            const string& serverDNS, 
            const string& serverPort, 
            struct addrinfo* hints, 
            struct addrinfo** result)
        {
            return getaddrinfo(serverDNS.c_str(), serverPort.c_str(), hints, result) != 0;
        }
        /**
         * @brief Checks if the given address family is valid for DNS communication.
         *
         * This function checks if the provided address family is valid for DNS communication.
         * Valid address families are AF_INET (IPv4) and AF_INET6 (IPv6).
         *
         * @param ai_family The address family to check.
         * @return True if the address family is valid for DNS communication, false otherwise.
         */
        bool isAddressFamilyValid(
            int ai_family)
        {
            return (ai_family == AF_INET || ai_family == AF_INET6);
        } 
        /**
         * @brief Iterates through a list of address information and connects to a valid address.
         *
         * This function iterates through the provided list of address information and attempts to connect
         * to a valid address for DNS communication. It checks the address family, initializes a DNS socket,
         * and establishes a connection.
         *
         * @param addressInfoList A list of address information to iterate through.
         */
        void connectToAddress(
            struct addrinfo* addressInfoList)
        {
            bool isConnected = false;

            for (; addressInfoList != NULL; addressInfoList = addressInfoList->ai_next)
            {
                if (isAddressFamilyValid(addressInfoList->ai_family))
                {
                    if (initializeDnsSocket(addressInfoList))
                    {
                        err::errHandle("Error: creating Socket");
                    }

                    if (connectDnsSocket(addressInfoList))
                    {
                        err::errHandle("Error: creating Socket");
                    }

                    isConnected = true;
                    break;
                }
            }

            if (!isConnected)
            {
                err::errHandle("Error: creating Socket");
            }
        }
        /**
         * @brief Initializes an addrinfo hints structure with default values for DNS communication.
         *
         * @param hints A pointer to the addrinfo hints structure to be initialized.
         */
        void initializeHintsTemplate(
            struct addrinfo* hints)
        {
            memset(hints, 0, sizeof(*hints));
            hints->ai_family = AF_UNSPEC;
            hints->ai_socktype = SOCK_DGRAM;
            // hints->ai_protocol = IPPROTO_UDP;
        }
        /**
         * @brief Creates a DNS socket, resolves the server address, and establishes a connection.
         *
         * @param serverDNS The DNS address of the server.
         * @param serverPort The port of the server.
         */
        void createSocket(
            string serverDNS, 
            string serverPort)
        {
            struct addrinfo hintsTemplate;
            struct addrinfo* addressInfoList = nullptr;

            initializeHintsTemplate(&hintsTemplate);

            resolveServerAddress(serverDNS, serverPort, &hintsTemplate, &addressInfoList);

            connectToAddress(addressInfoList);

            freeaddrinfo(addressInfoList);
        }
        /**
         * @brief Converts an IPv6 address in string format to binary format.
         *
         * @param ipAddress The IPv6 address in string format.
         * @param binaryIPv6 A pointer to a char buffer to store the binary IPv6 address.
         * @return True if the conversion is successful, false otherwise.
         */
        bool convertIPv6ToBinary(
            const string& ipAddress, 
            char* binaryIPv6)
        {
         
            return (inet_pton(AF_INET6, ipAddress.c_str(), binaryIPv6));
        }

        /**
         * @brief Converts a binary IPv6 address to a hexadecimal string representation.
         *
         * This function converts a binary IPv6 address to a hexadecimal string representation,
         * where each byte is represented by two hexadecimal digits.
         *
         * @param binaryIPv6 A pointer to the binary IPv6 address.
         * @param elements A vector to store the hexadecimal string elements.
         */
        void convertBinaryToHex(
            const char* binaryIPv6, 
            vector<string>& elements)
        {
            for (int i = 15; i >= 0; i--)
            {
                char c[2];
                sprintf(c, "%x",  binaryIPv6[i] & MASK);
                elements.push_back(c);
                sprintf(c, "%x", (binaryIPv6[i] >> FOUR_BIT_SHIFT) & MASK);
                elements.push_back(c);
            }
        }
        /**
         * @brief Converts an IPv6 address in string format to a vector of hexadecimal strings.
         *
         * This function converts an IPv6 address represented as a string to a vector of
         * hexadecimal strings, where each byte of the binary IPv6 address is represented
         * by two hexadecimal digits.
         *
         * @param ipAddress The IPv6 address in string format.
         * @param elements A vector to store the hexadecimal string elements.
         */
        void convertIPv6ToHex(
            const string& ipAddress, 
            vector<string>& elements)
        {
            char IPv6[16];
            memset(&IPv6, 0, 16);

            convertIPv6ToBinary(ipAddress,IPv6);

            convertBinaryToHex(IPv6,elements);

            elements.push_back("ip6");
            elements.push_back("arpa");
        }
        /**
         * @brief Determines if the given IP address is of a specific address family (IPv4 or IPv6).
         *
         * @param ip The address family to check (AF_INET for IPv4, AF_INET6 for IPv6).
         * @param address The IP address in string format.
         * @return True if the address is of the specified address family, false otherwise.
         */
        bool whichIPv(
            int ip, 
            const string& address)
        {
            struct in_addr buffer[16];
            return inet_pton(ip, address.c_str(), buffer) == 1;
        } 
        /**
         * @brief Determines the DNS query type based on the provided arguments.
         *
         * @param args The DNS request arguments.
         * @return The DNS query type (QTYPE) code.
         */
        uint16_t determineQueryType(
            const Arguments &args)
        {
            if (args.enableReverseQuery)
            {
                return htons(PTR);
            }
            else if (args.enableMX)
            {
                return htons(MX);
            }
            else
            {
                return args.enableIPv6 ? htons(AAAA) : htons(A);
            }
        }
        /**
         * @brief Generates a reverse DNS query address based on the input queryAddress.
         *
         * This function takes an input query address, divides it into elements, reverses the elements,
         * and appends "in-addr" and "arpa" to create a reverse DNS query address.
         *
         * @param queryAddress The original query address.
         * @return A vector of strings representing the reverse DNS query address elements.
         */
        vector<string> reverseAndAppendInAddr(
            const string& queryAddress)
        {
            vector<string> elements = str::divideQueryAddress(queryAddress);
            vector<string> reverseElements;

            for (int i = (elements.size() - 1); i >= 0; i--) {
                reverseElements.push_back(elements[i]);
            }

            reverseElements.push_back("in-addr");
            reverseElements.push_back("arpa");

            return reverseElements;
        }
        /**
         * @brief Processes an IP address and sets DNS request parameters accordingly.
         *
         * This function processes the given IP address, determines its address family (IPv4 or IPv6),
         * and sets the DNS request parameters accordingly. If it's an IPv6 address, it converts it to
         * hexadecimal format; otherwise, it prepares a reverse DNS query address for an IPv4 address.
         *
         * @param args Reference to the DNS request arguments to update.
         * @param elements Vector to store DNS query elements based on the IP address.
         */
        void processIpAddress(
            Arguments& args, 
            vector<string> &elements)
        {
            if (whichIPv(AF_INET, args.queryAdress.c_str()))
            {
                args.enableIPv6 = false;
            }
            else if (whichIPv(AF_INET6, args.queryAdress.c_str()))
            {
                args.enableIPv6 = true;
            }
            else
            {
                err::errHandle("Error: Invalid IP address");
            }

            if (args.enableIPv6)
            {
                convertIPv6ToHex(args.queryAdress.c_str(), elements);
            }
            else
            {
                elements = reverseAndAppendInAddr(args.queryAdress);
            }
        }
        /**
         * @brief Creates a DNS header based on the provided DNS request arguments.
         *
         * This function dynamically allocates memory for a DNS header, initializes it with
         * default values, and configures specific fields based on the provided DNS request arguments.
         *
         * @param args Pointer to the DNS request arguments for configuration.
         * @return A pointer to the created DNS header.
         */
        dnsServerResponseHeader* createDnsHeader(
            const Arguments* args)
        {
            dnsServerResponseHeader* header = (dnsServerResponseHeader*)calloc(1, sizeof(dnsServerResponseHeader));
            header->message_id = htons(1347);
            header->recursion_desired = args->enableRecursion ? 1 : 0;
            header->question_count = htons(1);
            return header;
        }
        /**
         * @brief Adds DNS query elements to a DNS packet.
         *
         * This function adds DNS query elements represented as strings to a DNS packet.
         * It updates the packet pointer, offset, and includes the length of each element.
         *
         * @param packet A pointer to the DNS packet to which elements are added.
         * @param offset A pointer to the offset within the packet.
         * @param elements A vector of strings representing DNS query elements.
         */
        void addElementsToPacket(
            char** packet, 
            int* offset, 
            const vector<string>& elements)
        {
            for (const string& element : elements)
            {
                int len = element.length();
                *packet = (char*)realloc(*packet, (*offset) + len + sizeof(char));
                memcpy((*packet + *offset), &len, sizeof(char));
                *offset += sizeof(char);
                memcpy((*packet + *offset), element.c_str(), len);
                *offset += len;
            }
        }
        /**
         * @brief Adds a null byte (zero) to a DNS packet to separate sections of the messae.
         *
         * @param packet A pointer to the DNS packet to which the null byte is added.
         * @param offset A pointer to the offset within the packet.
         */
        void addZeroToPacket(
            char** packet, 
            int* offset)
        {
            *packet = (char*)realloc(*packet, (*offset) + sizeof(char));
            (*packet)[*offset] = 0;
            (*offset) += sizeof(char);
        }
        /**
         * @brief Adds a DNS question section to a DNS packet.
         *
         * This function adds a DNS question section to a DNS packet by reallocating memory
         * for the packet and initializing the question section using the provided DNS request
         * arguments. It updates the packet pointer and offset accordingly.
         *
         * @param packet A pointer to the DNS packet to which the question section is added.
         * @param offset A pointer to the offset within the packet.
         * @param args Pointer to the DNS request arguments for configuring the question section.
         */
        void addDnsQuestionToPacket(
            char** packet, 
            int* offset, 
            const Arguments* args)
        {
            *packet = (char*)realloc(*packet, (*offset) + sizeof(DNS_Q));
            initializeDnsQuestion(*packet, *offset, *args);
            *offset += sizeof(DNS_Q);
        }
        /**
         * @brief Constructs a complete DNS packet based on DNS request arguments and elements.
         *
         * This function constructs a complete DNS packet by creating a DNS header, adding DNS query
         * elements and a DNS question section to the packet. It calculates the packet size and returns
         * the dynamically allocated packet. The packetSize parameter is updated with the size of the packet.
         *
         * @param args Pointer to the DNS request arguments for configuring the packet.
         * @param packetSize A pointer to an integer that will be updated with the size of the packet.
         * @param elements Vector of strings representing DNS query elements.
         * @return A pointer to the constructed DNS packet.
         */
        char* constructDnsPacket(
            const Arguments* args, 
            int* packetSize, 
            vector<string>& elements)
        {
            dnsServerResponseHeader* header = createDnsHeader(args);
            char* packet = (char*)header;
            int offset = sizeof(dnsServerResponseHeader);

            addElementsToPacket(&packet, &offset, elements);
            addZeroToPacket(&packet, &offset);
            addDnsQuestionToPacket(&packet, &offset, args);

            *packetSize = offset;
            return packet;
        }
        /**
         * @brief Creates a DNS packet based on DNS request arguments.
         *
         * This function creates a DNS packet based on DNS request arguments. It determines the type
         * of DNS query (reverse or forward) and constructs the packet accordingly. The packetSize
         * parameter is updated with the size of the packet.
         *
         * @param args Pointer to the DNS request arguments for configuring the packet.
         * @param packetSize A pointer to an integer that will be updated with the size of the packet.
         * @return A pointer to the created DNS packet.
         */
        char* createPacket(
            Arguments* args, 
            int* packetSize)
        {
            vector<string> elements;

            if(args->enableReverseQuery)// IP -> DOMAIN
            {
               processIpAddress(*args, elements);
            }
            else// DOMAIN -> IP
            {
                elements = str::divideQueryAddress(args->queryAdress);
            }

            return constructDnsPacket(args, packetSize, elements);

        }
        /**
         * @brief Initializes a DNS question section within a DNS packet.
         *
         * This function initializes a DNS question section within a DNS packet at the specified offset.
         * It configures the question type based on the provided DNS request arguments and sets the question
         * class to IN (Internet). The packet is expected to have adequate space allocated for the question section.
         *
         * @param packet A pointer to the DNS packet containing the question section.
         * @param offset The offset within the packet where the question section should be initialized.
         * @param args Reference to the DNS request arguments for configuring the question section.
         */
        void initializeDnsQuestion(
            void* packet, 
            size_t offset, 
            const Arguments& args)
        {
            DNS_Q* question = (DNS_Q*)(static_cast<uint8_t*>(packet) + offset);

            question->Type = determineQueryType(args);

            question->Class = htons(IN);
        }
        /**
         * @brief Sets a receive timeout for a socket.
         *
         * This function sets a receive timeout for a socket to limit the maximum time
         * the socket will wait for incoming data. It configures the socket's receive
         * timeout to the specified number of seconds.
         *
         * @param socket The socket for which the receive timeout is to be set.
         * @param seconds The number of seconds to set as the receive timeout.
         */
        void setSocketReceiveTimeout(
            int socket, 
            int seconds)
        {
            struct timeval timeout;
            timeout.tv_sec = seconds;
            timeout.tv_usec = 0;
            
            setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
        }
        /**
         * @brief Receives data from a socket and returns it as a buffer.
         *
         * @param socket The socket from which data will be received.
         * @param bytesRead A pointer to an integer that will store the number of bytes received.
         * @return A pointer to the received data buffer or `nullptr` if an error occurs.
         */
        char* receiveData(
            int socket, 
            int* bytesRead)
        {
            char* buffer = str::allocateMem();
            *bytesRead = recv(socket, buffer, MAX_SIZE, 0);
            
            if (*bytesRead < 0) 
            {
                free(buffer);
                err::errHandle("Error: corrupted received data or no data received");
            }
            
            return buffer;
        }
        /**
         * @brief Receives a DNS response from a socket with a specified timeout.
         *
         *
         * @param socket The socket from which the DNS response will be received.
         * @param bytesRead A pointer to an integer that will store the number of bytes received.
         * @param timeoutSeconds The maximum number of seconds to wait for data reception.
         * @return A pointer to the received DNS response buffer or `nullptr` if a timeout or error occurs.
         */
        char* receiveDNSResponse(
            int socket, 
            int* bytesRead, 
            int timeoutSeconds)
        {
            setSocketReceiveTimeout(socket, timeoutSeconds);
            
            return receiveData(socket, bytesRead);
        }
        /**
         * @brief Sends a DNS packet over the established socket connection.
         *
         * @param packetDNS The DNS packet data to be sent.
         * @param packetSize The size of the DNS packet in bytes.
         */
        void sendPacket(
            const char* packetDNS, 
            int packetSize)
        {
            send(this->Socket, packetDNS, packetSize, 0);
        }

    public:
        /**
         * @brief Sends a DNS request and receives a response.
         * @param args The DNS request arguments.
         * @param SizeOfRequestResponse Pointer to store the size of the response.
         * @return The DNS response data.
         */
        char* sendRequestDNS(
            Arguments* args, 
            int* SizeOfRequestResponse)
        {
            this->createSocket(args->serverDns, args->serverPort);

            int packeSize = 0;
            char* packetDNS = this->createPacket(args, &packeSize);
            this->sendPacket(packetDNS, packeSize);
            free(packetDNS);
            
            char* responseBuffer;
            responseBuffer = this->receiveDNSResponse(this->Socket, SizeOfRequestResponse, WAIT);
            
            return responseBuffer;
        }
};

class MessageParser
{
    private:
        /**
         * @brief Handles the parsing and printing of A (IPv4) record data.
         *
         * @param data A reference to a pointer to character data containing the IPv4 address.
         */
        void handleARecord(
            char*& data)
        {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, data, ip, INET_ADDRSTRLEN);
            cout << ip;
        }
        /**
         * @brief Handles the parsing and printing of AAAA (IPv6) record data.
         *
         * @param data A reference to a pointer to character data containing the IPv6 address.
         */
        void handleAAAARecord(
            char*& data)
        {
            char ipv6[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET6, data, ipv6, INET6_ADDRSTRLEN);
            cout << ipv6;
        }
        /**
         * @brief Handles the parsing of CNAME, NS, PTR, MG, MB, MF, and MD records.
         *
         * This function parses and advances the `data` pointer to the next label in the DNS response.
         *
         * @param data A reference to a pointer to character data to be parsed.
         * @param response The original DNS response data for reference.
         */
        void handleCNAME_NS_PTR_MG_MB_MF_MDRecord(
            char*& data, 
            char* response)
        {
            data = parseLabels(data, true, response);
        }
        /**
         * @brief Parses and prints data and response labels.
         *
         * This function parses and advances the `data` pointer to the next label in the DNS response
         * while also printing each label followed by an ELEMENT_DELIMITER.
         *
         * @param data A reference to a pointer to character data to be parsed.
         * @param response The original DNS response data for reference.
         */
        void parseAndPrintDataAndResponse(
            char*& data,
            char* response)
        {
            data = parseLabels(data, true, response);
            cout << ELEMENT_DELIMITER; 

            data = parseLabels(data, true, response);
            cout << ELEMENT_DELIMITER;
        }
        /**
         * @brief Handles the parsing and printing of SOA (Start of Authority) records.
         *
         * This function parses SOA records and prints their components, including data and response labels,
         * and various numeric fields such as Serial, Refresh, Retry, Expire, and Minimum.
         *
         * @param data A reference to a pointer to character data containing the SOA record.
         * @param response The original DNS response data for reference.
         */
        void handleSOARecord(
            char*& data, 
            char* response)
        {
            parseAndPrintDataAndResponse(data, response);
                    
            SOA_* soaRecord = (SOA_*)data;
            cout << ntohl(soaRecord->Serial) << ELEMENT_DELIMITER;
            cout << ntohl(soaRecord->Refresh) << ELEMENT_DELIMITER;
            cout << ntohl(soaRecord->Retry) << ELEMENT_DELIMITER;
            cout << ntohl(soaRecord->Expire) << ELEMENT_DELIMITER;
            cout << ntohl(soaRecord->Minimum);
        }
        /**
         * @brief Handles the parsing of MX (Mail Exchange) records.
         *
         * @param data A reference to a pointer to character data containing the MX record.
         * @param response The original DNS response data for reference.
         */
        void handleMXRecord(
            char*& data, 
            char* response)
        {
            int16_t* priority = (int16_t*)data;
            cout << ntohs(*priority);
            data = data + sizeof(int16_t);
            data = parseLabels(data, true, response);
        }
        /**
         * @brief Handles unsupported DNS record types.
         *
         * This function prints a message indicating that the DNS record type is unknown.
         */
        void handleUnsupportedRecord()
        {
            cout << "unknown record";
        }
        /**
         * @brief Prints the name associated with an A (IPv4) record type.
         *
         * @param type The DNS record type code.
         */
        void printATypeName(
            const int type)
        {
            for (const Mapping& map : aTypeMappinfs)
            {
                if (type == map.type)
                {
                    cout << map.name << ELEMENT_DELIMITER;
                    break;
                }
            }
        }
        /**
         * @brief Prints the name associated with a DNS class type.
         *
         *
         * @param type The DNS class type code.
         */
        void printMappingName(
            const int type)
        {
            for (const Mapping& map : qClassMappings)
            {
                if (type == map.type)
                {
                    cout << map.name << ELEMENT_DELIMITER;
                    break;
                }
            }
        }
        /**
         * @brief Parses DNS resource records in a DNS response.
         *
         * This function parses and prints DNS resource records in a DNS response. It iterates through
         * the resource records, extracts information such as type, class, TTL, and data, and prints them.
         *
         * @param ans A pointer to character data representing the DNS response records.
         * @param countOfmessages The number of resource records to parse.
         * @param response The original DNS response data for reference.
         * @return A pointer to the next DNS response record after parsing.
         */
        char* parseServeResponse(
            char *ans, 
            int countOfmessages, 
            char *response)
        {
            DNS_resource *dnsAnswerMiddle;
                
                for(int i = 0; i < countOfmessages; i++)
                {

                    ans = this->parseLabels(ans, true, response);
                    cout << ELEMENT_DELIMITER;

                    dnsAnswerMiddle = (DNS_resource *)ans;

                    char *data = (char *)(dnsAnswerMiddle) + sizeof(DNS_A);
                    
                    printATypeName(ntohs(dnsAnswerMiddle->ans.Type));
                    printMappingName(ntohs(dnsAnswerMiddle->ans.Class));
    
                    cout << ntohs(dnsAnswerMiddle->ans.Ttl) << ELEMENT_DELIMITER; // time to live

                    switch (ntohs(dnsAnswerMiddle->ans.Type))
                    {
                    case A:
                        handleARecord(data);
                        break;
                    case CNAME:
                    case PTR:
                    case MG:
                    case MB:
                    case MF:
                    case MD:
                    case NS:
                        handleCNAME_NS_PTR_MG_MB_MF_MDRecord(data, response);
                        break;
                    case AAAA:
                        handleAAAARecord(data);
                        break;

                    case MX:
                        handleMXRecord(data, response);
                        break;
                    case SOA:
                        handleSOARecord(data, response);
                        break;
                    default:
                        handleUnsupportedRecord();
                        break;
                    }

                    ans = ans + sizeof(DNS_A) + ntohs(dnsAnswerMiddle->ans.RdLength);
                    cout << "\n";
                }

                return ans;
        }
        /**
         * @brief Prints the root label or a space character.
         *
         * @param root A pointer to a character representing a DNS label.
         */
        void printRootLabel(
            char* root)
        {
            if (*root== 0)
            {
                cout << " <root>";
            }
            else
            {
                cout << " ";
            }
        }
        /**
         * @brief Prints a DNS label.
         *
         * @param label A pointer to a character representing the DNS label to be printed.
         * @param length The length of the DNS label.
         */
        void printLabel(
            char* label, 
            int8_t length)
        {
            for (int i = 0; i < length; i++)
            {
                std::cout << label[i + 1];
            }   
        }
        /**
         * @brief Advances the DNS label pointer to the next label.
         *
         * This function advances the DNS label pointer `label` to the next label in the DNS data.
         *
         * @param label A reference to a pointer to a character representing the current DNS label.
         * @param length The length of the current DNS label.
         */
        void nextLabel(
            char*& label, 
            int8_t length)
        {
            label += (1 + length); // Move to the next label
        }
        /**
         * @brief Processes a normal DNS label.
         *
         * This function processes a normal DNS label by printing its characters, advancing the label pointer,
         * and printing a dot '.' to separate labels.
         *
         * @param label A reference to a pointer to a character representing the current DNS label.
         */
        void processNormalLabel(
            char*& label)
        {
            int8_t labelLength = *((int8_t*)(label));
            printLabel(label, labelLength);
            nextLabel(label, labelLength);
            std::cout << ".";
        }
        /**
         * @brief Get the next DNS label pointer based on whether it was saved.
         *
         *
         * @param isSave A boolean flag indicating whether the label pointer was saved.
         * @param label A pointer to a character representing the current DNS label.
         * @param isSaveLabel A pointer to a character representing the saved DNS label pointer.
         * @return A pointer to the next DNS label or label and skip first 0000 0000.
         */
        char* getLabelPointer(
            bool isSave, 
            char* label, 
            char* isSaveLabel)
        {
            if (isSave) {
                return isSaveLabel;
            } else {
                return label + 1;
            }
        }
        /**
         * @brief Check if a DNS label is compressed.
         *
         * @param isHigher An unsigned 16-bit integer representing the higher bits of a DNS label.
         * @param isPointer A boolean flag indicating whether the label is a pointer.
         * @return True if the DNS label is compressed; otherwise, false.
         */
        bool isCompressed(
            uint16_t isHigher, 
            bool isPointer)
        {
            return (isHigher >= 0xC000) ? (isPointer ? true : false) : false;
        }
        /**
         * @brief Return the DNS label pointer to a previous position if not recovered.
         *
         * @param label A reference to a pointer to a character representing the current DNS label.
         * @param recover A reference to a boolean flag indicating whether the label pointer has been recovered.
         * @param labelBegin A pointer to a character representing the original DNS label position.
         */
        void returnPointer(
            char*& label, 
            bool& recover, 
            char* labelBegin)
        {
            if (!recover)
            {
                label = labelBegin;
                recover = true;
            }
        }
        /**
         * @brief Manipulate the DNS label and backup the original position.
         *
         * This function manipulates the DNS label by advancing it based on the provided `issCompressed`
         * value and backs up the original label position if it hasn't been recovered before.
         *
         * @param labelBegin A reference to a pointer to a character representing the current DNS label.
         * @param backupLabelStart A reference to a pointer to a character representing the backup label position.
         * @param recover A reference to a boolean flag indicating whether the label pointer has been recovered.
         * @param dnsMess A pointer to a character representing the DNS message data.
         * @param issCompressed An unsigned 16-bit integer representing the compressed label offset.
         */
        void manipulateLabelAndBackup(
            char*& labelBegin,
            char*& backupLabelStart,
            bool& recover,
            char* dnsMess,
            uint16_t issCompressed) 
        {
            issCompressed = issCompressed - 0xC000;
            labelBegin = labelBegin + sizeof(issCompressed);

            returnPointer(backupLabelStart, recover, labelBegin);

            labelBegin = dnsMess + issCompressed;
        }
        /**
         * @brief Parse and manipulate DNS labels.
         *
         * This function parses and manipulates DNS labels from the given `label` pointer, taking into account
         * whether it is a pointer or not. It handles label compression, advances the label pointer,
         * and returns the next label pointer.
         *
         * @param label A pointer to a character representing the current DNS label.
         * @param isPointer A boolean flag indicating whether the label is a pointer.
         * @param begin A pointer to a character representing the beginning of the DNS message.
         * @return A pointer to the next DNS label.
         */
        char* parseLabels(
            char *label, 
            bool isPointer, 
            char *begin)
        {
            char *backupLabelStart = label;
            bool isSave = false;
            uint16_t issCompressed;

            printRootLabel(label);
            
            while(*label != 0)
            {

                memcpy(&issCompressed, label, sizeof(issCompressed));

                if(isCompressed(ntohs(issCompressed), isPointer))
                {
                    manipulateLabelAndBackup(label, backupLabelStart, isSave, begin, ntohs(issCompressed));   
                }
                else
                {
                    processNormalLabel(label);
                }
            } 
            return getLabelPointer(isSave, label, backupLabelStart);
        }
        /**
         * @brief Print the DNS question type name.
         *
         * @param questionEnd A pointer to a DNS_Q structure representing the end of a DNS question.
         */
        void printQTypeName(
            DNS_Q* questionEnd)
        {
            questionEnd->Type = ntohs(questionEnd->Type);
            for (const Mapping& map : qTypeMappings)
            {
                if (questionEnd->Type == map.type)
                {
                    cout << map.name << ELEMENT_DELIMITER;
                    break;
                }
            }
        }
/**
         * @brief Print the DNS question class name.
         *
         * @param questionEnd A pointer to a DNS_Q structure representing the end of a DNS question.
         */
        void printQClassName(
            DNS_Q* questionEnd)
        {
            questionEnd->Class = ntohs(questionEnd->Class);
            for(const Mapping& map: qClassMappings)
            {
                if(questionEnd->Class == map.type)
                {
                    cout << map.name << " \n";
                    break;
                }
            }
        }
        /**
         * @brief Print DNS server response header flags.
         *
         * @param header A pointer to a dnsServerResponseHeader structure representing the DNS server response header.
         */

        void printQuestionFlags(
            dnsServerResponseHeader *header)
        {
            cout << "Authoritative:" << (header->authoritative_answer ? "Yes" : "No");
            cout << ",Recursive:" << (header->recursion_desired ? "Yes" : "No");
            cout << ",Truncated:" << (header->truncated_message ? "Yes\n" : "No\n");
        }
        /**
         * @brief Print DNS server response's Answer section.
         *
         *
         * @param header A pointer to a dnsServerResponseHeader structure representing the DNS server response header.
         * @param ans A reference to a character pointer representing the current position in the response data.
         * @param response A pointer to the DNS response data.
         */
        void printAnswer(
            dnsServerResponseHeader* header, 
            char*& ans, 
            char* response)
        {
            header->answer_record_count = ntohs(header->answer_record_count);
            cout << "Answer section (" << header->answer_record_count << ")\n";
            ans = parseServeResponse(ans, header->answer_record_count, response);
        }
        /**
         * @brief Print DNS server response's Additional section.
         *
         * @param header A pointer to a dnsServerResponseHeader structure representing the DNS server response header.
         * @param ans A reference to a character pointer representing the current position in the response data.
         * @param response A pointer to the DNS response data.
         */
        void printAdditional(
            dnsServerResponseHeader* header,
            char*& ans,
            char* response)
        {
            header->additional_record_count = ntohs(header->additional_record_count);
            cout << "Additional section (" << header->additional_record_count << ")\n";
            ans = parseServeResponse(ans, header->additional_record_count, response);
        }
        /**
         * @brief Print DNS server response's Authority section.
         *
         * @param header A pointer to a dnsServerResponseHeader structure representing the DNS server response header.
         * @param ans A reference to a character pointer representing the current position in the response data.
         * @param response A pointer to the DNS response data.
         */
        void printAuthority(
            dnsServerResponseHeader* header, 
            char*& ans, 
            char* response)
        {
            header->authority_record_count = ntohs(header->authority_record_count);
            cout << "Authority section (" << header->authority_record_count << ")\n";
            ans = parseServeResponse(ans, header->authority_record_count, response);
        }
        /**
         * @brief Process sections of a DNS server response.
         *
         * @param header A pointer to a dnsServerResponseHeader structure representing the DNS server response header.
         * @param quest A reference to a character pointer representing the current position in the question section of the response.
         * @param response A pointer to the DNS response data.
         */
        void processSections(
            dnsServerResponseHeader* header, 
            char*& quest, 
            char* response)
        {
            char* ans = quest;
            printAnswer(header, ans, response);
            printAuthority(header, ans, response);
            printAdditional(header, ans, response);
        }

    public:
        /**
         * @brief Parse and process a DNS server response message.
         *
         * This function parses and processes a DNS server response message, including the header, question section, and various
         * resource sections (Answer, Authority, Additional). It also handles error codes and prints relevant information.
         *
         * @param response A pointer to the DNS server response message.
         */
        void parseMessage(
            char *response)
        {
            dnsServerResponseHeader *header = (dnsServerResponseHeader*)response;
            
            if(header->rcode)
            {
                err::errHandleReturn(header->rcode);
            }

            printQuestionFlags(header);

            char* quest= ((char*)header) + sizeof(dnsServerResponseHeader);

            cout << "Question section (" << ntohs(header->question_count) << ")\n";
            DNS_Q* questionEnd = nullptr;

            for (int i = 0; i < ntohs(header->question_count); i++)
            {
                questionEnd = (DNS_Q*)(this->parseLabels(quest, false, response));
                cout << ",";

                printQTypeName(questionEnd);

                printQClassName(questionEnd);
                
                quest = ((char *)questionEnd) + sizeof(DNS_Q);
            }

            processSections(header, quest, response);
        }
};


/**
 * @brief Main function to perform a DNS request and parse the response.
 *
 * This function performs the following steps:
 * 1. Parses command-line arguments using the Arguments class.
 * 2. Sends a DNS request using the DNSrequest class and retrieves the server's response.
 * 3. Parses the DNS response using the MessageParser class.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of command-line argument strings.
 * @return An integer representing the exit status of the program.
 */
int main(int argc, char* argv[])
{
    int size;
    Arguments* args = new Arguments();

    args->parseArguments(argc, argv, args);
    // PRINT_HELP(args);

    DNSrequest dns;
    char* serverResponse = dns.sendRequestDNS(args, &size);

    MessageParser parser;
    parser.parseMessage(serverResponse);
    
    delete args;
    free(serverResponse);
    return OK_EXIT;
}