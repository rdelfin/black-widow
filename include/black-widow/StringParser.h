//
// Created by Ricardo Delfin Garcia on 10/12/2015.
//

#ifndef BLACK_WIDOW_STRINGPARSER_H
#define BLACK_WIDOW_STRINGPARSER_H

#include <string>
#include <sstream>

#include "Parser.h"

namespace bw {
    /**
     * Generic string parser for string data. Only provides a common stringstream interface for all string data.
     */
    class StringParser : public Parser<std::string> {
    public:
        /**
         * Default constructor uses stringstream instead of generic istream data structure.
         */
        StringParser(std::stringstream& ss) : stream(ss) { }
    protected:
        std::stringstream& stream;
    };
}


#endif //BLACK_WIDOW_STRINGPARSER_H
