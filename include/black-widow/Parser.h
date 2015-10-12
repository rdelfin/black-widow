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
        Parser() { }

        /**
         * Parses the data and stores it in the inner vector data structure.
         */
        virtual void parse() = 0;

        size_t size() { return parsedData.size(); }
        const T& operator[](unsigned int i)const{ return parsedData[i]; }

        /**
         * Default destructor does nothing
         */
        virtual ~Parser() { }
    protected:
        std::vector<T> parsedData;
    };
}


#endif //BLACK_WIDOW_PARSER_H
