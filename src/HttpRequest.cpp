//
// Created by Ricardo Delfin Garcia on 10/8/2015.
//

#include "black-widow/HttpRequest.h"

#include <boost/regex.hpp>

#include <iostream>

#include "black-widow/HttpResponse.h"

using boost::asio::ip::tcp;

namespace bw {

    HttpRequest::HttpRequest()
        : host(""), path(""), body(), headers()
    {
        socket = nullptr;
    }

    bool HttpRequest::setOrigin(std::string host, std::string path) {
        boost::regex isHost("^(\\w+\\.)*\\w+(:\\d?)?$");
        boost::regex isPath("^\\/([\\w-]+\\/)*([\\w-]+(\\.\\w+)?)?(\\?(\\w+=[\\w%]+)(&(\\w+=[\\w%]+))*)?$");

        if(!boost::regex_match(host, isHost, boost::match_default)
           && boost::regex_match(path, isPath, boost::match_default)) {
            this->host = host;
            this->path = path;
        }

        return false;
    }

    HttpResponse* HttpRequest::send() {
        // Build up the request
        boost::asio::streambuf request;
        std::ostream request_stream(&request);

        // Setup request stream
        setupRequestStream(request_stream);

        if(!sendSocket(request))
            return NULL;

        // Create a new response with the socket and hand over ownership. Return the response.
        HttpResponse* response = new HttpResponse(socket);
        socket = nullptr;
        return response;
    }

    bool HttpRequest::sendSocket(boost::asio::streambuf& data) {
        try {

            boost::asio::io_service io_service;

            // Get a list of endpoints corresponding to the server name.
            tcp::resolver resolver(io_service);
            tcp::resolver::query query(host, "http");
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            // Try each endpoint until we successfully establish a connection.
            socket = new tcp::socket(io_service);
            tcp::resolver::iterator it = boost::asio::connect(*socket, endpoint_iterator);

            // Send the request.
            boost::asio::write(*socket, data);

        } catch(boost::system::system_error e) {
            // There was an error. Delete socket, clean up and return false.
            if(socket != nullptr) {
                boost::system::error_code ec;
                socket->close(ec);
                delete socket;
                socket = nullptr;
            }
            return false;
        }

        return true;
    }

    void HttpRequest::setupRequestStream(std::ostream& request_stream) {
        // Remove any content-length headers set by the user and replace with body size at this moment
        if(body.size() != 0) {
            std::stringstream ss;
            ss << body.size();
            headers["Content-Length"] = ss.str();
        }

        // Request line
        request_stream << method() << " " << path << " HTTP/1.1\r\n"
        << "Host: " << host << "\r\n";

        // Additional header fields
        for(boost::unordered_map<std::string, std::string>::iterator it = headers.begin();
            it != headers.end(); ++it) {
            request_stream << it->first << ": " << it->second << "\r\n";
        }

        // If there is content, add request body
        if(body.size() != 0) {
            // New line between header and content
            request_stream << "\r\n";

            //Append body
            request_stream << body;
        }
    }

    HttpRequest::~HttpRequest() {

    }
}