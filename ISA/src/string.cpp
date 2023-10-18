/*
 * Autor: Petr Kaška
 * Datum: 2. října 2023
 */
#include "string.hpp"
#include "globals.hpp"
#include "customErr.hpp"

namespace str
{
    void replaceCharacter(std::string& input, char target)
    {
        std::replace(input.begin(), input.end(), target, SPACE);
    }

    std::vector<std::string> splitString(const std::string& input)
    {
        std::vector<std::string> elements;
        std::string element;

        for (char character : input)
        {
            if (character == SPACE)
            {
                elements.push_back(element);
                element.clear();
            }
            else
            {
                element.push_back(character);
            }
        }

        elements.push_back(element);
        return elements;
    }

    std::vector<std::string> divideQueryAddress(const std::string& address)
    {
        std::string modifiedAddress = address;

        str::replaceCharacter(modifiedAddress, DELIMETER);

        return str::splitString(modifiedAddress);
    }

    char* allocateMem()
    {
        char* buf = (char*)malloc(MAX_SIZE);
        if(buf == NULL)
        {
            err::errHandle("Error: malloc");
        }
        return buf;
    }

}