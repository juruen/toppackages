#include "logger.h"

logging::log_streambuf::log_streambuf(
    std::string ident, 
    const logging::facility& facility
  ) : 
    m_buffer(),
    m_facility(facility),
    m_priority(logging::priority::debug),
    m_ident(ident)
{
  openlog(m_ident.c_str(), LOG_PID, static_cast<int>(m_facility));
}

int logging::log_streambuf::sync()
{
  if(m_buffer.length())
  {
    syslog(static_cast<int>(m_priority), m_buffer.c_str());
    m_buffer.erase();
    m_priority = logging::priority::debug;
  }

  return 0;
}

int logging::log_streambuf::overflow(int c)
{
  if(c != EOF)
  {
    m_buffer += static_cast<char>(c);
  } 
  else
  {
    sync();
  }

  return c;
}

std::ostream& logging::operator<<(
    std::ostream& os,
    const logging::priority& priority
  )
{
  static_cast<logging::log_streambuf*>(os.rdbuf())->m_priority = priority;

  return os;
}
