#ifndef DPKG_H
#define DPKG_H

#include <unordered_map>
#include <string>
#include <list>
#include <boost/asio.hpp>
#include "dllist.h"

class dpkg {
  public:
    dpkg(boost::asio::io_service&);
    void openfile(std::string path);
    void dumpTop();

  private:
    void _loadFileLists();
    void timer_handle(const boost::system::error_code& ec);
    void add_handle();

  private:
    boost::asio::deadline_timer m_timer;
    typedef std::pair<std::string, long long> pkgUsage;
    dllist<pkgUsage> packages_list;
    typedef dlnodelist<pkgUsage>* pkg_list_node;
    std::unordered_map<std::string, pkg_list_node>  fileToPackage;
    std::list<pkg_list_node> top_list;

};

#endif
