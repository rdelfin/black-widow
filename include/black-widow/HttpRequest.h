//
// Created by Ricardo Delfin Garcia on 10/8/2015.
//

#ifndef BLACK_WIDOW_HTTPREQUEST_H
#define BLACK_WIDOW_HTTPREQUEST_H

#include <string>
#include <boost/unordered_map.hpp>
#include <boost/asio.hpp>
#include <ostream>


namespace bw {
    class HttpResponse;

    class HttpRequest {
    public:
        /**
         * Create an HTTP request with a specified HTTP method, origin and path. Will through an exception if the host
         * and path . Will confirm that the method exists.
         */
        HttpRequest();

        /**
         * Sets the origin server (i.e., server containing the resource) and confirms it is a valid URL. It does NOT
         * test the resource to confirm its existence.
         */
        bool setOrigin(std::string host, std::string path);

        /**
         * Generates the HTTP request and returns the response before it has been serviced.
         */
        virtual HttpResponse* send();

        /**
         * Lets the user add custom headers
         */
        void setHeader(const std::string& field, const std::string& value);

        virtual ~HttpRequest();

    private:
        boost::unordered_map<std::string, std::string> headers;
        std::string host, path;

        //TODO: Makes this some other data type
        std::string body;

        boost::asio::ip::tcp::socket* socket;

        /**
         * Returns the HTTP method type (i.e. GET, POST, PUT, DELETE, PATCH, HEADER, OPTIONS)
         */
        virtual std::string method() = 0;

        void setupRequestStream(std::ostream &request_stream);

        bool sendSocket(boost::asio::streambuf& data);
    };
}


#endif //BLACK_WIDOW_HTTPREQUEST_H
