//
// Created by Ricardo Delfin Garcia on 10/8/2015.
// Some code was obtained from Boost example file for HTTP clients:
// http://www.boost.org/doc/libs/1_47_0/doc/html/boost_asio/example/http/client/sync_client.cpp
//

#include "black-widow/HttpRequest.h"

#include <cstdlib>

#include <boost/regex.hpp>
#include <boost/bind.hpp>

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

        if(boost::regex_match(host, isHost, boost::match_default)
           && boost::regex_match(path, isPath, boost::match_default)) {
            this->host = host;
            this->path = path;
            return true;
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
        HttpResponse* httpResponse = new HttpResponse(socket, io_service);
        socket = nullptr;
        return httpResponse;
    }

    bool HttpRequest::sendSocket(boost::asio::streambuf& data) {
        try {

            io_service = new boost::asio::io_service;
            ssl_context = new boost::asio::ssl::context(boost::asio::ssl::context::sslv23);

            // Get a list of endpoints corresponding to the server name
            tcp::resolver resolver(*io_service);
            tcp::resolver::query query(host, "http");
            tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

            // Try each endpoint until we successfully establish a connection.
            socket = new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>(*io_service, *ssl_context);

            // Setup SSL
            socket->set_verify_mode(boost::asio::ssl::verify_peer);
            socket->set_verify_callback(boost::bind(&HttpRequest::verify_certificate, this, _1, _2));

            tcp::resolver::iterator it = boost::asio::connect(socket->lowest_layer(), endpoint_iterator);

            // Send the request.
            boost::asio::write(*socket, data);

        } catch(boost::system::system_error e) {
            // There was an error. Delete socket, clean up and return false.
            if(socket != nullptr) {
                boost::system::error_code ec;
                socket->shutdown(ec);
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
            std::stringstream ss;
            ss << it->first << ": " << it->second << "\r\n";
            std:: string line = ss.str();
            request_stream << line;
        }

        // New line between header and content (even if content is empty)
        request_stream << "\r\n";

        // If there is content, add request body
        if(body.size() != 0) {
            //Append body
            request_stream << body;
        }
    }

    void HttpRequest::setHeader(const std::string& field, const std::string& value) {
        // TODO: Check if it is a registered header field and warn the user
        headers[field] = value;
    }

    bool HttpRequest::verify_certificate(bool preverified, boost::asio::ssl::verify_context& ctx) {
        char subject_name[256];
        X509* cert = X509_STORE_CTX_get_current_cert(ctx.native_handle());
        X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
        std::cout << "Verifying " << subject_name << "\n";

        return preverified;
    }

    HttpRequest::~HttpRequest() {

    }
}