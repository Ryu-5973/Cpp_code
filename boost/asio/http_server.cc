/*
 * @Author: Ryu-59073
 * @Date: 2022-07-23 22:25:06
 * @LastEditors: Ryu-59073
 * @LastEditTime: 2022-07-23 22:54:55
 * @FilePath: /Cpp_code/boost/asio/http_server.cc
 * @Description: asio实现HTTP server
 * 
 * Copyright (c) 2022 by Ryu-59073, All Rights Reserved. 
 */
#include <iostream>
#include <string>
#include <memory>

#include <boost/asio.hpp>

using namespace std;

class HttpConnection: public std::enable_shared_from_this<HttpConnection> {
public:
    HttpConnection(boost::asio::io_context &io): m_socket(io) {}

    void Start() {
        auto p = shared_from_this();
        boost::asio::async_read_until(m_socket, boost::asio::dynamic_buffer(m_request), "\r\n\r\n",
            [p, this](const boost::system::error_code& err, size_t len){
                if(err) {
                    cout << "recv err: " << err.message() << "\n";
                    return ;
                }
                string first_line = m_request.substr(0, m_request.find("\r\n"));
                cout << first_line << "\n";
                // process with request
                // ...
                char str[] = "HTTP/1.0 200 OK\r\n\r\n"
                    "<html>hello from http server</html>";
                boost::asio::async_write(m_socket, boost::asio::buffer(str), 
                    [p, this](const boost::system::error_code& err, size_t len){
                    m_socket.close();
                });
        });
    }

    boost::asio::ip::tcp::socket& Socket() {
        return m_socket;
    }
private:
    boost::asio::ip::tcp::socket m_socket;
    string m_request;
};


class HttpServer {
public:
    HttpServer(boost::asio::io_context &io, boost::asio::ip::tcp::endpoint ep)
        : m_io(io), m_acceptor(io, ep) {}
    
    void Start() {
        auto p = std::make_shared<HttpConnection>(m_io);
        m_acceptor.async_accept(p->Socket(), 
            [p, this](const boost::system::error_code& err){
            if(err) {
                cout << "accept err: " << err.message() << "\n";
                return ;
            }    
            p->Start();
            Start();
        });
    }
private:
    boost::asio::io_context& m_io;
    boost::asio::ip::tcp::acceptor m_acceptor;
};

int main(int argc, char const *argv[]) {
    if(argc != 3) {
        cout << "Usage: " << argv[0] << " ip port\n";
        return 0;
    }

    boost::asio::io_context io;
    boost::asio::ip::tcp::endpoint ep(boost::asio::ip::make_address(argv[1]), std::stoi(argv[2]));
    HttpServer server(io, ep);
    server.Start();
    io.run();
    return 0;
}
