#ifndef LOG_H
#define LOG_H

// A modified version of Log class from:
// http://stackoverflow.com/a/4457138

#include <syslog.h>
#include <iostream>

namespace logging {

  // See man 3 syslog for details
  enum class priority {
      emerg   = LOG_EMERG,
      alert   = LOG_ALERT,
      crit    = LOG_CRIT,
      err     = LOG_ERR,
      warning = LOG_WARNING,
      notice  = LOG_NOTICE,
      info    = LOG_INFO,
      debug   = LOG_DEBUG
  };
  
  // See man 3 syslog for details
  enum class facility {
      user      = LOG_USER,
      mail      = LOG_MAIL,
      daemon    = LOG_DAEMON,
      auth      = LOG_AUTH,
      syslog    = LOG_SYSLOG,
      lpr       = LOG_LPR,
      news      = LOG_NEWS,
      uucp      = LOG_UUCP,
      cron      = LOG_CRON,
      authpriv  = LOG_AUTHPRIV,
      ftp       = LOG_FTP,
      local0    = LOG_LOCAL0,
      local1    = LOG_LOCAL1,
      local2    = LOG_LOCAL2,
      local3    = LOG_LOCAL3,
      local4    = LOG_LOCAL4,
      local5    = LOG_LOCAL5,
      local6    = LOG_LOCAL6,
      local7    = LOG_LOCAL7
  };

  std::ostream& operator<<(std::ostream& os, const logging::priority& priority);

  class log_streambuf
      : public std::basic_streambuf<char, std::char_traits<char>>
  {
    public:
      explicit log_streambuf(std::string ident, const logging::facility& facility);

    protected:
      int sync();
      int overflow(int c);

    private:
      friend std::ostream& operator<<(
          std::ostream& os,
          const logging::priority& priority
        );

      std::string m_buffer;
      logging::facility m_facility;
      logging::priority m_priority;
      std::string m_ident;
  };

  class logger
  {
    public:
      explicit logger(std::string ident, const logging::facility& facility)
        : 
          m_log_streambuf(ident, facility),
          m_old_clog_streambuf(nullptr)
      {
        m_old_clog_streambuf = std::clog.rdbuf(&m_log_streambuf);
      }

      ~logger()
      {
        std::clog.rdbuf(m_old_clog_streambuf);
      }

    private:
      logging::log_streambuf m_log_streambuf;
      std::streambuf* m_old_clog_streambuf;
  };
}

#endif
