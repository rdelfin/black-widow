//
// Created by Ricardo Delfin Garcia on 10/12/2015.
//

#ifndef BLACK_WIDOW_PARSER_H
#define BLACK_WIDOW_PARSER_H

#include <iostream>
#include <vector>

namespace bw {
    /**
     * Defines a generic stream parser that tokenizes the data in some form or fashion. Eases parsing incomming HTTP
     * response data. The data will be stored in a vector<T> data structure. Therefore, the T represents the type of
     * data that a parser results in.
     */
    template <class T>
    class Parser {
    public:
        /**
         * Default constructor. Uses a data stream as an input.
         */
        Parser(std::istream& stream) : stream(stream) { }

        /**
         * Parses the data and returns a std::vector itterator. THe
         */
        virtual std::vector<T>::iterator& parse() = 0;

        /**
         * Default destructor does nothing
         */
        virtual ~Parser() { }
    protected:

        const std::istream& stream;
        std::vector<T> parsedData;
    };
}


#endif //BLACK_WIDOW_PARSER_H
