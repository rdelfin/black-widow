#include <iostream>

#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <black-widow/HttpRequest.h>
#include <black-widow/HttpGetRequest.h>
#include <black-widow/HttpResponse.h>

using boost::asio::ip::tcp;

using namespace std;

int main(int argc, char* argv[]) {
    try
    {
        if (argc != 3)
        {
            std::cout << "Usage: sync_client <server> <path>\n";
            std::cout << "Example:\n";
            std::cout << "  " << argv[0] << " www.boost.org /LICENSE_1_0.txt\n";
            return 1;
        }

        bw::HttpRequest* request = new bw::HttpGetRequest;

        if(!request->setOrigin(argv[1], argv[2])) {
            std::cout << "Wat" << endl;
            std::exception e;
            throw e;
        }

        request->setHeader("Accept", "*/*");
        request->setHeader("Connection", "close");

        bw::HttpResponse* response = request->send();

        response->fetch();

        cout << "Body: " << endl;
        cout << response->getBody();
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }

    return 0;
}