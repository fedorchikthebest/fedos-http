#include<string> 


namespace fhttp{
    class Server
    {
        private:
            /* data */
        public:
            void get(std::string url, std::string (*obr)());
            void post(std::string url, std::string (*obr)());
            Server(int port);
    };
}