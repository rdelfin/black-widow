//
// Created by ricar on 10/12/2015.
//

#include "black-widow/RegexParser.h"
#include <boost/regex.hpp>

namespace bw
{

    RegexParser::RegexParser(std::stringstream& ss, std::string& pattern)
        : StringParser(ss), pattern(pattern)
    {

    }

    void RegexParser::parse() {
        std::string data = stream.str();
        boost::regex rx(pattern);

        boost::sregex_token_iterator it(data.begin(), data.end(), rx, 0);
        boost::sregex_token_iterator end;

        for( ; it != end; ++it) {
            parsedData.push_back(*it);
        }
    }

    RegexParser::~RegexParser() {

    }
}
