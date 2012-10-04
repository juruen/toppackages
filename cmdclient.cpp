#include "cmdclient.h"

#include <iostream>
#include <cstdio>

namespace {
  const int buffer_size = 1024;
}

cmdclient::cmdclient(settings& sett) :
 m_receiver_endpoint("socket.unix"),
 m_sender_endpoint(std::tmpnam(nullptr)),
 m_socket(m_io_service, m_sender_endpoint),
 m_buffer(buffer_size),
 m_sett(sett)
{

}

bool cmdclient::send_cmd(cmd::cmd cmd, cmd::cmd& response) {
  m_socket.send_to(
      boost::asio::buffer(reinterpret_cast<char*>(&cmd), sizeof(cmd)),
      m_receiver_endpoint
  );

  size_t len = m_socket.receive_from(boost::asio::buffer(m_buffer), m_ep);
  if (len != sizeof(cmd)) {
    std::cout << "Received " << len << " bytes" << std::endl;
    return false;
  }
  cmd::buffer_to_cmd(m_buffer, response);

  return true;
}

bool cmdclient::dump_top()
{
  try
  {

    cmd::cmd cmd;
    cmd::cmd response;
    cmd.type = cmd::cmdtype::request;
    cmd.id = cmd::cmdid::dumptop;
    cmd.data = m_sett.top;

    if (!send_cmd(cmd, response)) {
      std::cout << "Sending cmd failed" << std::endl;
      return false;
    }

    if (!(response.type == cmd::cmdtype::response &&
        response.id == cmd::cmdid::dumptop)) {
      std::cout << "Wrong response" << std::endl;
      return false;
    }

    for (size_t i = response.data; i > 0; i--) {
      size_t len = m_socket.receive_from(
          boost::asio::buffer(m_buffer), m_ep);
      for (size_t j = 0; j < len; j++) {
        std::cout << m_buffer[j];
      }
    }

    return true;
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
    return false;
  }
}
