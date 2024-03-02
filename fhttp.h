#include<string>
#include <map>
#include<queue>
#define OK_RESPONSE "HTTP/1.1 200 OK\r\n"
#define NOT_FOUND_RESPONSE "HTTP/1.1 404 NOT_FOUND\r\n"
#define PACKET_SIZE 4048


namespace fhttp{
    class Server
    {
        private:
            std::map<std::string, std::string(*)(std::string)> responce_functions;
            void requests_accept_thread();
            void requests_handler_thread();
            unsigned short int port;
            char* addr;
        public:
            void request_handler(std::string url, std::string (*obr)(std::string));
            void run(unsigned short int thread_num);
            Server(char* ip, int p);
    };

    class Responce{ //parsing http requests and make responce
        private:
            std::map<std::string, std::string> request;
            std::map<std::string, std::string> response;
        public:
            Responce(std::string request);
            char* c_str();
            std::string get_str_header(std::string header);
            int get_int_header(std::string header);
            void render(std::string html);
            void set_header(std::string header, std::string value);
            void set_cookie(std::string name, std::string value);
    };
}