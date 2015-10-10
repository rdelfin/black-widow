//
// Created by Ricardo Delfin Garcia on 10/10/2015.
//

#ifndef BLACK_WIDOW_HTTPGETREQUEST_H
#define BLACK_WIDOW_HTTPGETREQUEST_H

#include "HttpRequest.h"

namespace bw {
    class HttpGetRequest : public HttpRequest {
    public:
        HttpGetRequest();
        virtual ~HttpGetRequest();
    private:
        virtual std::string method() { return "GET"; }
    };
}


#endif //BLACK_WIDOW_HTTPGETREQUEST_H
