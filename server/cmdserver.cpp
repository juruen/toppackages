#include "cmdserver.h"
#include "cmd.h"
#include <boost/bind.hpp>

namespace {
  const std::string socket_path("socket.unix");
  const unsigned int buffer_size = 1024 * 8;
};

cmdserver::cmdserver(
    boost::asio::io_service& io_service,
    std::function<void(commonserver::toptype, const size_t, std::vector<std::string>&)> get_top
)
:
  m_get_top(get_top)
{
  ::unlink(socket_path.c_str());
  std::shared_ptr<boost::asio::local::datagram_protocol::socket> socket(
       new boost::asio::local::datagram_protocol::socket(
        io_service,
        boost::asio::local::datagram_protocol::endpoint(socket_path)
      )
   );
  m_socket = socket;
  m_buffer.resize(buffer_size);
  add_handle();
}

void cmdserver::read_handle(
    const boost::system::error_code& ec,
    std::size_t bytes_read)
{
  std::cout << "Unix read handle. Bytes read: " << bytes_read << std::endl;
 if (!ec) {
   if (bytes_read == sizeof(cmd::cmd)) {
     std::cout << "Looks like a command packet" << std::endl;
     process_command();
   }
   add_handle();
  } else {
    std::cout << "There was an error" << std::endl;
  }
}

void cmdserver::add_handle()
{
  m_socket->async_receive_from(
      boost::asio::buffer(m_buffer, buffer_size),
      m_remote,
      boost::bind(
        &cmdserver::read_handle,
        this,
        boost::asio::placeholders::error,
        boost::asio::placeholders::bytes_transferred
        )
      );
}

void cmdserver::sent_handle(
    boost::shared_ptr<std::string> msg,
    const boost::system::error_code& error,
    std::size_t size)
{
}

void cmdserver::process_command()
{
  cmd::cmd cmd;
  buffer_to_cmd(m_buffer, cmd);  

 if (cmd.type != cmd::cmdtype::request) {
    std::cout << "Not a request command: " << std::endl;
    return;
  }

 switch (cmd.id) {
   case cmd::cmdid::dumptop:
     dump_top(commonserver::toptype::top, cmd.data);
     break;
   case cmd::cmdid::dumpbottom:
     dump_top(commonserver::toptype::bottom, cmd.data);
     break;
 }
}

void cmdserver::dump_top(commonserver::toptype toptype, size_t n)
{
  std::cout << "dump_top: " << std::endl;

  std::vector<std::string> top;
  m_get_top(toptype, n, top);

  cmd::cmd cmd;
  cmd.type = cmd::cmdtype::response;
  cmd.data = top.size();
  if (toptype == commonserver::toptype::top) {
    cmd.id = cmd::cmdid::dumptop;
  } else {
    cmd.id = cmd::cmdid::dumpbottom;
  }

  boost::shared_ptr<std::string> txt;
  m_socket->async_send_to(
      boost::asio::buffer(reinterpret_cast<char*>(&cmd), sizeof(cmd)),
      m_remote,
       boost::bind(&cmdserver::sent_handle, this, txt,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred
        )
  );

  for (auto s: top) {
    boost::shared_ptr<std::string> msg(new std::string(s));
    msg->append("\n");
    m_socket->async_send_to(boost::asio::buffer(*msg), m_remote,
        boost::bind(&cmdserver::sent_handle, this, msg,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }
}

void cmdserver::stop()
{
}

