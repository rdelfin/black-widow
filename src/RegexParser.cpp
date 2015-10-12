//
// Created by ricar on 10/12/2015.
//

#include "black-widow/RegexParser.h"
#include <boost/regex.hpp>

namespace bw
{

    RegexParser::RegexParser(std::stringstream& ss, std::string& patern)
        : StringParser(ss), patern(patern)
    {

    }

    const std::vector<std::string>::iterator& RegexParser::parse() {
        std::string data = stream.str();
        boost::regex rx(patern);
        boost::smatch tokens;


        if(boost::regex_match(data, tokens, rx, boost::match_extra))
            for(size_t i = 0; i < tokens.size(); i++)
                parsedData.push_back(tokens[i]);

        it = parsedData.begin();
        return it;
    }

    RegexParser::~RegexParser() {

    }
}
