#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <iostream>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

void handle_request(tcp::acceptor& acceptor) {
    acceptor.async_accept([&](std::error_code ec, tcp::socket&& socket) {
    std::cout << "Hello, up and running\n";
    std::string buf;
    buf.resize(256);
    boost::system::error_code error;
    size_t len = socket.read_some(boost::asio::buffer(buf), error);
    std::string str_url = buf.data();
    std::cout << "Len : " << str_url.size() << std::endl;
    std::cout << "Message received: " << buf.data() << " \n";
    boost::asio::io_context ioc;
    //Used to look up the IP address using the domain name
    tcp::resolver resolver(ioc);
    //Socket to connect to the server
    boost::beast::tcp_stream stream(ioc);

    // Look up the domain name, always on port 80
    auto const results = resolver.resolve(str_url, "80");

    //Connect to the server
    stream.connect(results);

   // Set up an HTTP GET request message
    boost::beast::http::request<boost::beast::http::string_body> req{boost::beast::http::verb::get, "/", 10};
    req.set(boost::beast::http::field::host, str_url);
    req.set(boost::beast::http::field::user_agent, BOOST_BEAST_VERSION_STRING);

   // Send the HTTP request
    boost::beast::http::write(stream, req);

   // This buffer is used for reading and must be persisted
    boost::beast::flat_buffer buffer;

   // Best response object to hold th result
    boost::beast::http::response<boost::beast::http::dynamic_body> res;

   // Receive the HTTP response
    boost::beast::http::read(stream, buffer, res);

   // Write the message to a string
    std::string s =  boost::beast::buffers_to_string(res.body().data());
    std::cout << s << std::endl;
    boost::beast::error_code error_code;
    stream.socket().shutdown(tcp::socket::shutdown_both, error_code);


    //Write the string to the original socket

    boost::asio::write(socket, boost::asio::buffer(s), error);
    handle_request(acceptor);
  });
}


int main(){
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 3000));
    handle_request(acceptor);
    io_context.run();
    return 0;
}



