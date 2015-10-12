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
        RegexParser(std::stringstream& stream, std::string& pattern);

        virtual void parse();

        virtual ~RegexParser();
    private:
        std::string& pattern;
        std::vector<std::string>::iterator it;
    };
}


#endif //BLACK_WIDOW_REGEXPARSER_H
