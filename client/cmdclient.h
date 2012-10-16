#ifndef CMDCLIENT_H
#define CMDCLIENT_H

#include "cmd.h"
#include "settings.h"

#include <boost/asio.hpp>
#include <boost/asio/local/datagram_protocol.hpp>

class cmdclient {
  public:
    cmdclient(settings& sett);

    bool dump_top();

  private:
    bool send_cmd(cmd::cmd, cmd::cmd&);

  private:
    boost::asio::io_service m_io_service;
    boost::asio::local::datagram_protocol::endpoint m_ep;
    boost::asio::local::datagram_protocol::endpoint m_receiver_endpoint;
    boost::asio::local::datagram_protocol::endpoint m_sender_endpoint;
    boost::asio::local::datagram_protocol::socket m_socket;
    std::vector<char> m_buffer;
    settings& m_sett;
};

#endif
