#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
using boost::asio::ip::tcp;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;
int main(int argc, char** argv){
    if(argc == 0){
        std::cout << "Usage: ./client <url>\n Example: ./client www.example.com";
    }
    std::string buf = argv[1];
    boost::asio::io_context io_context;
    tcp::resolver resolver(io_context);
    tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "3000");

    tcp::socket socket(io_context);
    boost::asio::connect(socket, endpoints);
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(&buf[0], buf.size()), error);
    boost::asio::streambuf response;
    //While the socket has stuff, keep reading
    while (boost::asio::read(socket, response,
              boost::asio::transfer_at_least(1), error))
    std::cout << &response;


}
