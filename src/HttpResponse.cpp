//
// Created by Ricardo Delfin Garcia on 10/7/2015.
// Some code was obtained from Boost example file for HTTP clients:
// http://www.boost.org/doc/libs/1_47_0/doc/html/boost_asio/example/http/client/sync_client.cpp
//

#include <iostream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/asio/ssl/stream.hpp>

#include "black-widow/HttpResponse.h"

namespace bw
{

    HttpResponse::HttpResponse(boost::asio::ssl::stream<boost::asio::ip::tcp::socket> *originSkt,
                               boost::asio::io_service* io_service, boost::asio::ssl::context* ssl_context)
        : originSkt(originSkt), io_service(io_service), ssl_context(ssl_context)
    {

    }


    bool HttpResponse::fetch() {
        // Read until EOF, and call processors after ALL data has been received. Otherwise data might be lost.
        boost::system::error_code error;
        while (boost::asio::read(*originSkt, response, boost::asio::transfer_at_least(1), error));

        // Throw error
        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);

        return processHeaders() && processBody();
    }


    bool HttpResponse::processHeaders() {
        // Read status line in response. In ABNS: status-line = HTTP-version SP status-code SP reason-phrase CRLF
        std::istream response_stream(&response);
        std::string http_version;
        unsigned int status_code;
        std::string status_message;

        response_stream >> http_version;
        response_stream >> status_code;

        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            // TODO: Throw exception
            return false;
        }

        // Store the status code
        this->status_code = status_code;
        this->status_message = status_message;

        // Iterate over the response headers.
        std::string headerString;
        while (std::getline(response_stream, headerString) && headerString != "\r") {
            std::string field, value;

            // Split string into field and value. Will return false if this is not a valid header
            if(separateHeader(headerString, field, value)) {
                header[field] = value;
            }
        }

        return true;
    }


    bool HttpResponse::processBody() {
        // Response stream used to parse over HTTP response
        std::istream response_stream(&response);

        // Read every line in the body. Insert \n at the end of each line
        std::string bodyLine;
        std::stringstream bodyBuilder; // Huehuehuehue
        while (!response_stream.eof() && std::getline(response_stream, bodyLine)) {
            bodyBuilder << bodyLine << std::endl;
        }

        body = bodyBuilder.str();

        return true;
    }


    bool HttpResponse::separateHeader(const std::string& line, std::string& field, std::string& value) {
        std::vector<std::string> values;
        std::string headerString(line);
        boost::regex separator(": ?");
        boost::sregex_token_iterator it(headerString.begin(), headerString.end(), separator, -1);
        boost::sregex_token_iterator emptyIt;

        // Trim out optional whitespace in accordance to RFC 7230 Section 3.2
        boost::trim(headerString);

        // This is perplexing but it works
        while(it != emptyIt && values.size() <= 2)
        {
            values.push_back(*it);
            it++;
        }

        // THis is not a key-value pair. I have no idea what's going on then.
        if(values.size() != 2) {
            return false;
        }

        field = values[0];
        value = values[1];
    }



    std::string HttpResponse::getBody() {
        return body;
    }

    std::string HttpResponse::getHeader(const std::string& field) {
        return header[field];
    }


}
