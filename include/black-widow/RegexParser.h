//
// Created by ricar on 10/12/2015.
//

#ifndef BLACK_WIDOW_REGEXPARSER_H
#define BLACK_WIDOW_REGEXPARSER_H

#include "StringParser.h"

namespace bw
{
    class RegexParser : public StringParser {
    public:
        RegexParser(std::stringstream&, std::string&);

        virtual const std::vector<std::string>::iterator& parse();

        virtual ~RegexParser();
    private:
        std::string& patern;
        std::vector<std::string>::iterator it;
    };
}


#endif //BLACK_WIDOW_REGEXPARSER_H
