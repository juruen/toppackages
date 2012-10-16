#include <string>
#include <iostream>
#include <stdio.h>
#include <boost/bind.hpp>
#include "systemtap.h"

namespace {
  const std::string cmd("stap  opencall.stp");
  const unsigned int buffer_size = 1024 * 8;
};

systemtap::systemtap(
    boost::asio::io_service& io_service,
    std::function<void(std::string)> processFile
)
:
  m_pipe(io_service),
  m_buffer(buffer_size),
  m_processFile(processFile),
  m_line("")
{

}

bool systemtap::start()
{
  FILE* fpipe = popen(cmd.c_str(), "r");
  if (!fpipe) {
    std::cerr << "Couldn't open pipe for " << cmd << std::endl;
    return false;
  }
  m_pipe.assign(fileno(fpipe));
  add_handle();
  return true;
}

void systemtap::read_handle(
    const boost::system::error_code& ec,
    std::size_t bytes_read)
{
  if (!ec) {
    for (std::size_t i = 0; i < bytes_read; i++) {
      if (m_buffer[i] == '\n') {
        m_processFile(m_line);
        m_line = "";
      } else {
        m_line.push_back(m_buffer[i]);
      }
    }
    add_handle();
  } else {
    std::cout << "There was an error" << std::endl;
  }
}

void systemtap::add_handle()
{
  m_pipe.async_read_some(boost::asio::buffer(m_buffer, buffer_size),
      boost::bind(
        &systemtap::read_handle,
        this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred
        )
      );
}

void systemtap::stop()
{
}

