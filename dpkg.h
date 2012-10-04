#ifndef DPKG_H
#define DPKG_H

#include <unordered_map>
#include <string>
#include <vector>
#include <list>
#include <boost/asio.hpp>
#include "dllist.h"
#include "settings.h"

class dpkg {
  public:
    dpkg(boost::asio::io_service&, settings& sett);
    void open_file(std::string path);
    void current_top(const size_t max, std::vector<std::string>& output);
    void dump_top();

  private:
    void load_filelists();
    void timer_handle(const boost::system::error_code& ec);
    void add_handle();

  private:
    boost::asio::deadline_timer m_timer;
    typedef std::pair<std::string, long long> pkg_usage;
    dllist<pkg_usage> packages_list;
    typedef dlnodelist<pkg_usage>* pkg_list_node;
    std::unordered_map<std::string, pkg_list_node>  file_to_package;
    std::list<pkg_list_node> top_list;
    settings& sett;
};

#endif
