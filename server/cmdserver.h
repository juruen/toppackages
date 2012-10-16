#ifndef CMDSERVER_H
#define CMDSERVER_H

#include <boost/asio.hpp>
#include <boost/asio/local/datagram_protocol.hpp>
#include "commonserver.h"

class cmdserver {
  public:
    cmdserver(
      boost::asio::io_service&,
      std::function<void(commonserver::toptype,const size_t, std::vector<std::string>&)>
    );
    void stop();
  private:
    void read_handle(
        const boost::system::error_code& ec,
        std::size_t bytes_read
    );
    void add_handle();
    void sent_handle(
      boost::shared_ptr<std::string> msg,
      const boost::system::error_code& error,
      std::size_t size
    );
    void process_command();
    void dump_top(commonserver::toptype, size_t);
  private:
    boost::asio::local::datagram_protocol::endpoint m_remote;;
    std::shared_ptr<boost::asio::local::datagram_protocol::socket> m_socket;
    std::vector<char> m_buffer;
    std::function<void(commonserver::toptype, const size_t, std::vector<std::string>&)> m_get_top;
};

#endif
