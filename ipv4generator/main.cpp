/*
 * main.cpp
 *
 *  Created on: Aug 6, 2024
 *      Author: dorozhin
 */

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <exception>
#include <cstring>


std::vector<std::string> generateIPAddresses(std::string &str)
{
    const auto strSize = str.size();
    const auto minIPSize = 4;
    const auto maxIPSize = 12;
    const auto maxOctetSize = 3;

    if (strSize < minIPSize || strSize > maxIPSize)
    throw std::runtime_error("Bad input string size");

    for (size_t i = 0; i < strSize; i++)
    {
        if (str[i] < '0' || str[i] > '9')
        throw std::runtime_error("Invalid symbol in the input string");
    }

    std::vector<std::string> validIPs;
    char octets[maxOctetSize + 1][maxOctetSize + 1];
    
    for (auto octet4DigitCnt = 1; octet4DigitCnt <= maxOctetSize; octet4DigitCnt++)
    {
        for (auto octet3DigitCnt = 1; octet3DigitCnt <= maxOctetSize; octet3DigitCnt++)
        {
            for (auto octet2DigitCnt = 1; octet2DigitCnt <= maxOctetSize; octet2DigitCnt++)
            {
                for (auto octet1DigitCnt = 1; octet1DigitCnt <= maxOctetSize; octet1DigitCnt++)
                {
                    size_t currentIPSize = octet1DigitCnt + octet2DigitCnt + octet3DigitCnt + octet4DigitCnt;
                    
                    //std::cout << octet1DigitCnt << "." << octet2DigitCnt << "." << octet3DigitCnt << "." << octet4DigitCnt << std::endl;

                    if (currentIPSize > strSize)
                    {
                        break;
                    }

                    if (currentIPSize < strSize)
                    continue;

//TODO: move code below to separate function to avoid code duplication
const size_t octet1Offset = 0;

        if (octet1DigitCnt > 1 && '0' == str[octet1Offset])
        continue;
        
        memset(octets[0], 0, maxOctetSize + 1);
        memcpy(octets[0], &(str.c_str())[octet1Offset], octet1DigitCnt);

        if (atoi(octets[0]) > 255)
        continue;

const size_t octet2Offset = octet1DigitCnt;

        if (octet2DigitCnt > 1 && '0' == str[octet2Offset])
        continue;
        
        memset(octets[1], 0, maxOctetSize + 1);
        memcpy(octets[1], &(str.c_str())[octet2Offset], octet2DigitCnt);

        if (atoi(octets[1]) > 255)
        continue;

                    const size_t octet3Offset = octet1DigitCnt + octet2DigitCnt;

        if (octet3DigitCnt > 1 && '0' == str[octet3Offset])
        continue;
        
        memset(octets[2], 0, maxOctetSize + 1);
        memcpy(octets[2], &(str.c_str())[octet3Offset], octet3DigitCnt);

        if (atoi(octets[2]) > 255)
        continue;

        const size_t octet4Offset = octet1DigitCnt + octet2DigitCnt + octet3DigitCnt;

        if (octet4DigitCnt > 1 && '0' == str[octet4Offset])
        continue;

        memset(octets[3], 0, maxOctetSize + 1);
        memcpy(octets[3], &(str.c_str())[octet4Offset], octet4DigitCnt);

        if (atoi(octets[3]) > 255)
        continue;
//======================
        std::string currentIP(octets[0]);
        currentIP.append(".");
        currentIP.append(octets[1]);
        currentIP.append(".");
        currentIP.append(octets[2]);
        currentIP.append(".");
        currentIP.append(octets[3]);
        //std::cout << "Pushing IP: " << currentIP << std::endl;
        validIPs.push_back(currentIP);
                }
            }
        }
    }

    return validIPs;
}

int main(int argc, char* argv[])
{
    try
    {
        std::cout << "Please enter the string containing digits>\a";

    std::string inputStr;
    std::cin >> inputStr;
    std::vector<std::string> generatedIPs(generateIPAddresses(inputStr));

    std::cout << "Generated IPs" << std::endl;
    for (auto currentIP : generatedIPs)
    {
        std::cout << currentIP << std::endl;
    }
    std::cout << "------------" << std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return EXIT_SUCCESS;
}


