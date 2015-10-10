//
// Created by Ricardo Delfin Garcia on 10/7/2015.
//

#ifndef BLACK_WIDOW_HTTPRESPONSE_H
#define BLACK_WIDOW_HTTPRESPONSE_H

#include <boost/asio.hpp>



namespace bw {

    class HttpResponse {
    public:
        HttpResponse(boost::asio::ip::tcp::socket* socket);
    private:
    };

}


#endif //BLACK_WIDOW_HTTPRESPONSE_H
