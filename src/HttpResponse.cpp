//
// Created by Ricardo Delfin Garcia on 10/7/2015.
// Some code was obtained from Boost example file for HTTP clients:
// http://www.boost.org/doc/libs/1_47_0/doc/html/boost_asio/example/http/client/sync_client.cpp
//

#include <iostream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <sstream>

#include "black-widow/HttpResponse.h"

namespace bw
{

    HttpResponse::HttpResponse(boost::asio::ip::tcp::socket *originSkt, boost::asio::io_service* io_service)
        : headersFetched(false), bodyFetched(false), originSkt(originSkt), io_service(io_service)
    {

    }


    bool HttpResponse::fetchHeaders() {
        if(headersFetched || bodyFetched) {
            // TODO: Throw an exception (InvalidStateException?)
            return false;
        }

        // Read status line in response. In ABNS: status-line = HTTP-version SP status-code SP reason-phrase CRLF
        boost::asio::streambuf response;

        boost::system::error_code ec;
        boost::asio::read_until(*originSkt, response, "\r\n", ec);

        std::istream response_stream(&response);
        std::string http_version;
        unsigned int status_code;
        std::string status_message;

        response_stream >> http_version;
        response_stream >> status_code;
        std::getline(response_stream, status_message);

        if (!response_stream || http_version.substr(0, 5) != "HTTP/")
        {
            // TODO: Throw exception
            return false;
        }
        if (status_code != 200)
        {
            // TODO: Throw exception
            return false;
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(*originSkt, response, "\r\n\r\n");

        // Iterate over the response headers.
        std::string headerString;
        while (std::getline(response_stream, headerString) && headerString != "\r") {
            std::string field, value;

            // Split string into field and value. Will return false if this is not a valid header
            if(separateHeader(headerString, field, value)) {
                header[field] = value;
            }
        }

        // Potentially, we could have a more complex headersFetched checking system.
        headersFetched = true;
        return headersFetched;
    }



    bool HttpResponse::fetchBody() {
        if(bodyFetched || !headersFetched) {
            // TODO: Throw exception
            return false;
        }

        std::stringstream responseStream;
        boost::asio::streambuf response;

        // Read until EOF, writing data to response stream as we go.
        boost::system::error_code error;
        while (boost::asio::read(*originSkt, response, boost::asio::transfer_at_least(1), error)) {
            responseStream << &response;
        }

        // Throw error
        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);

        body = responseStream.str();

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
