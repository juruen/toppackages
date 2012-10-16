#ifndef DPKG_H
#define DPKG_H

#include <unordered_map>
#include <string>
#include <vector>
#include <list>
#include <boost/asio.hpp>
#include "settings.h"
#include "commonserver.h"

class dpkg {
  public:
    dpkg(boost::asio::io_service&, settings& sett);
    void open_file(std::string path);
    void current_top(const commonserver::toptype type, const size_t max, std::vector<std::string>& output);
    void dump_top();

  private:
    void load_filelists();
    void timer_handle(const boost::system::error_code& ec);
    void add_handle();

  private:
    void top_n(size_t n, std::vector<std::string>& output);
    void bottom_n(size_t n, std::vector<std::string>& output);

    boost::asio::deadline_timer m_timer;
    settings& m_sett;
    
    typedef std::pair<std::string, long long> pkg_usage;
    std::list<pkg_usage> m_packages_list;
    typedef std::list<pkg_usage>::iterator packages_list_iter;
    std::unordered_map<std::string, packages_list_iter> m_file_to_package;
};

#endif
