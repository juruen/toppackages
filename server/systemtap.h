#ifndef SYSTEMTAP_H
#define SYSTEMTAP_H

#include <string>
#include <functional>
#include <boost/asio.hpp>

class systemtap {
  public:
    systemtap(
      boost::asio::io_service&,
      std::function<void(std::string)>
    );
    bool start();
    void stop();
  private:
    void read_handle(
        const boost::system::error_code& ec,
        std::size_t bytes_read
    );
    void add_handle();
  private:
    boost::asio::posix::stream_descriptor m_pipe;
    std::vector<char> m_buffer;
    std::function<void(std::string)> m_processFile;
    std::string m_line;
};

#endif
