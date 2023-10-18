/*
 * Autor: Petr Kaška
 * Datum: 2. října 2023
 */
#ifndef STRING_HPP
#define STRING_HPP

#include <string>
#include <vector>
#include <algorithm>

namespace str
{
    void replaceCharacter(std::string& input, char target);
    std::vector<std::string> splitString(const std::string& input);
    std::vector<std::string> divideQueryAddress(const std::string& address);
    char* allocateMem();
}

#endif // STRING_HPP