//
// Created by ricar on 10/12/2015.
//

#ifndef BLACK_WIDOW_PARSER_H
#define BLACK_WIDOW_PARSER_H

#include <iostream>
#include <vector>

namespace bw {
    template <class T>
    class Parser {
    public:
        Parser(std::istream& stream) : stream(stream) { }

        virtual std::vector<T>::iterator& parse() = 0;

        virtual ~Parser() { }
    private:

        const std::istream& stream;
        std::vector<T> parsedData;
    };
}


#endif //BLACK_WIDOW_PARSER_H
