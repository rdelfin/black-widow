#include <iostream>

#include <boost/asio.hpp>
#include <boost/regex.hpp>
#include <black-widow/HttpRequest.h>
#include <black-widow/HttpGetRequest.h>
#include <black-widow/HttpResponse.h>
#include <black-widow/Parser.h>
#include <black-widow/RegexParser.h>

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
            std::cerr << "Error when setting origin " << argv[1]
                      << " with path " << argv[2] << std::endl;
            std::exception e;
            throw e;
        }

        request->setHeader("Accept", "*/*");
        request->setHeader("Connection", "close");

        bw::HttpResponse* response = request->send();

        response->fetch();

        std::string body = response->getBody();
        std::stringstream ss(body);

        std::string pattern = "(?<=href=\")[^\\s]+(?=\")";
        bw::RegexParser p(ss, pattern);
        p.parse();

        cout << "Parsed links: " << endl;

        for(size_t i = 0; i < p.size(); i++) {
            cout << p[i] << endl;
        }
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << std::endl;
    }

    return 0;
}