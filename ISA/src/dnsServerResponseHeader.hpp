/***************************************************************************************
*    Title: DNS Query Code in C with winsock
*    Author: Silver Moon
*    Date:  July 28, 2020
*    Availability: https://www.binarytides.com/dns-query-code-in-c-with-winsock/
*
***************************************************************************************/
#ifndef DNS_HPP
#define DNS_HPP

// Set the packing alignment to 1 byte to ensure no padding between structure members
#pragma pack(1)

// Define a structure to represent a DNS server response header
typedef struct dnsServerResponseHeader {
    unsigned short message_id;            // ID of the DNS question

    // Flags
    unsigned char  recursion_desired :1;  // Recursion Desired
    unsigned char  truncated_message :1;  // Truncated Message
    unsigned char  authoritative_answer :1; // Authoritative Answer
    unsigned char  response_code :4;      // Response Code
    unsigned char  qr :1;                 // Query (0) or Response (1) Bit

    // More Flags
    unsigned char  rcode :4;              // Response Code
    unsigned char  z :1;                  // Reserved (Zero) Bit
    unsigned char  ra :1;                 // Recursion Available
    unsigned char  ad :1;                 // Authenticated Data
    unsigned char  cd :1;                 // Checking Disabled

    unsigned short question_count;        // Question Count
    unsigned short answer_record_count;   // Answer Record Count
    unsigned short authority_record_count; // Authority Record Count
    unsigned short additional_record_count; // Additional Record Count
} dnsServerResponseHeader;

#endif //DNS_HPP