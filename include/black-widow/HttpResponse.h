//
// Created by Ricardo Delfin Garcia on 10/7/2015.
//

#ifndef BLACK_WIDOW_HTTPRESPONSE_H
#define BLACK_WIDOW_HTTPRESPONSE_H

#include <boost/asio.hpp>
#include <boost/unordered_map.hpp>
#include <string>

using boost::asio::ip::tcp;

namespace bw {
    class HttpResponse {
    public:
        /**
         * Default constructor for the HTTP Response. Receives and takes ownership of the TCP Socket. This means
         * that the destructor will destroy (delete) the socket
         */
        HttpResponse(tcp::socket* originSkt, boost::asio::io_service* io_service);

        /**
         * Obtains data form the request and stores into the appropriate data structures. If this has not been called
         * the request will not have been fulfilled.
         */
        bool fetchHeaders();

        /**
         * Obtains the body of the request. Will only do so if the headers have already been marked as fetched and the
         * socket is in the appropriate state (open). It will store into the body member variable.
         */
        bool fetchBody();

        std::string getBody();
        std::string getHeader(const std::string& field);

        virtual ~HttpResponse() { delete originSkt; };
    private:
        boost::unordered_map<std::string, std::string> header;

        tcp::socket* originSkt;
        boost::asio::io_service* io_service;

        bool separateHeader(const std::string& line, std::string& field, std::string& value);

        bool headersFetched, bodyFetched;

        std::string body;
    };

}


#endif //BLACK_WIDOW_HTTPRESPONSE_H
