#include<string>
#include <map>
#define OK_RESPONSE "HTTP/1.1 200 OK\r\n"
#define NOT_FOUND_RESPONSE "HTTP/1.1 404 NOT_FOUND\r\n"
#define PACKET_SIZE 4048


namespace fhttp{
    class Server
    {
        private:
            std::map<std::string, std::string(*)(std::string)> responce_functions;
        public:
            void request_hendler(std::string url, std::string (*obr)(std::string));
            void run(int port);
    };
}